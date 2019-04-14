#include "StdAfx.h"
#include "TimerEngine.h"
#include "TraceService.h" //

//�궨��
#define NO_TIME_LEFT						DWORD(-1)				//����Ӧʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerThread::CTimerThread(void) {
    m_pTimerEngine = NULL;
}

//��������
CTimerThread::~CTimerThread(void) {
}

//���ú���
bool CTimerThread::InitThread(CTimerEngine * pTimerEngine) {
    if (pTimerEngine == NULL) return false;

    //���ñ���
    m_pTimerEngine = pTimerEngine;

    return true;
}

//���к���
bool CTimerThread::OnEventThreadRun() {
    ASSERT(m_pTimerEngine != NULL);
    Sleep(TIMER_SPACE);
    m_pTimerEngine->OnTimerThreadSink();
    return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerEngine::CTimerEngine(void) {
    m_bService = false;
    m_pIQueueServiceSink = NULL;
    m_dwTimeLeave = NO_TIME_LEFT;
    m_dwTimePass = 0;
}

//��������
CTimerEngine::~CTimerEngine(void) {
    INT_PTR i = 0;
    //ֹͣ����
    ConcludeService();

    //�����ڴ�
    tagTimerItem * pTimerItem = NULL;
    for (i = 0; i < m_TimerItemFree.GetCount(); i++) {
        pTimerItem = m_TimerItemFree[i];
        ASSERT(pTimerItem != NULL);
        SafeDelete(pTimerItem);
    }
    for (i = 0; i < m_TimerItemActive.GetCount(); i++) {
        pTimerItem = m_TimerItemActive[i];
        ASSERT(pTimerItem != NULL);
        SafeDelete(pTimerItem);
    }
    m_TimerItemFree.RemoveAll();
    m_TimerItemActive.RemoveAll();

    return;
}

//�ӿڲ�ѯ
void * CTimerEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer) {
    QUERYINTERFACE(ITimerEngine, Guid, dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX(ITimerEngine, Guid, dwQueryVer);
    return NULL;
}

//���ö�ʱ��
bool CTimerEngine::SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter) {
    //������Դ
    CWHDataLocker lock(m_CriticalSection);//

    //Ч�����
    ASSERT(dwRepeat > 0L);
    if (dwRepeat == 0) return false;
	dwElapse = (dwElapse + TIMER_SPACE - 1) / TIMER_SPACE*TIMER_SPACE;
    //���Ҷ�ʱ��
    bool bTimerExist = false;
    tagTimerItem * pTimerItem = NULL;
    for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++) {
        pTimerItem = m_TimerItemActive[i];
        ASSERT(pTimerItem != NULL);
        if (pTimerItem->wTimerID == dwTimerID) {
            bTimerExist = true;
            break;
        }
    }

    //������ʱ��
    if (bTimerExist == false) {
        INT_PTR nFreeCount = m_TimerItemFree.GetCount();
        if (nFreeCount > 0) {
            pTimerItem = m_TimerItemFree[nFreeCount-1];
            ASSERT(pTimerItem != NULL);
            m_TimerItemFree.RemoveAt(nFreeCount - 1);
        } else {
            try {
                pTimerItem = new tagTimerItem;
                ASSERT(pTimerItem != NULL);
                if (pTimerItem == NULL) return false;
            } catch (...) {
                return false;
            }
        }
    }

    //���ò���
    ASSERT(pTimerItem != NULL);
    pTimerItem->wTimerID = dwTimerID;
    pTimerItem->wBindParam = dwBindParameter;
    pTimerItem->dwElapse = dwElapse;
	pTimerItem->dwRepeatTimes = dwRepeat;
	pTimerItem->dwTimeLeave = dwElapse + m_dwTimePass;

	m_dwTimeLeave = min(m_dwTimeLeave, dwElapse);
	
	ASSERT(pTimerItem->dwElapse%TIMER_SPACE == 0);
	ASSERT(pTimerItem->dwTimeLeave%TIMER_SPACE == 0);
	ASSERT(m_dwTimeLeave%TIMER_SPACE == 0);
    //���ʱ��
    if (bTimerExist == false)
        m_TimerItemActive.Add(pTimerItem);

    return true;
}

//ɾ����ʱ��
bool CTimerEngine::KillTimer(DWORD dwTimerID) {
    //������Դ
    CWHDataLocker lock(m_CriticalSection);

    //���Ҷ�ʱ��
    tagTimerItem * pTimerItem = NULL;
    for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++) {
        pTimerItem = m_TimerItemActive[i];
        ASSERT(pTimerItem != NULL);
        if (pTimerItem->wTimerID == dwTimerID) {
            m_TimerItemActive.RemoveAt(i);
            m_TimerItemFree.Add(pTimerItem);
			if (m_TimerItemActive.GetCount() == 0){
				m_dwTimePass = 0L;
				m_dwTimeLeave = NO_TIME_LEFT;
			}
            return true;;
        }
    }

    return false;
}

//ɾ����ʱ��
bool CTimerEngine::KillAllTimer() {
    //������Դ
    CWHDataLocker lock(m_CriticalSection);

    //ɾ����ʱ��
    m_TimerItemFree.Append(m_TimerItemActive);
    m_TimerItemActive.RemoveAll();

	m_dwTimePass = 0L;
	m_dwTimeLeave = NO_TIME_LEFT;

    return true;
}

//��ʼ����
bool CTimerEngine::StartService() {

	m_dwTimePass = 0L;
	m_dwTimeLeave = NO_TIME_LEFT;
    //Ч��״̬
    if (m_bService == true) {
        CTraceService::TraceString(TEXT("��ʱ�������ظ�������������������"), TraceLevel_Warning);
        return true;
    }

    //���ñ���
    if (m_TimerThread.InitThread(this) == false) {
        CTraceService::TraceString(TEXT("��ʱ�������̷߳����ʼ��ʧ��"), TraceLevel_Exception);
        return false;
    }

    //��������
    if (m_TimerThread.StartThread() == false) {
        CTraceService::TraceString(TEXT("��ʱ�������̷߳�������ʧ��"), TraceLevel_Exception);
        return false;
    }

    SetThreadPriority(m_TimerThread.GetThreadHandle(), REALTIME_PRIORITY_CLASS);


    //���ñ���
    m_bService = true;

    return true;
}

//ֹͣ����
bool CTimerEngine::ConcludeService() {
    //���ñ���
    m_bService = false;
    //ֹͣ�߳�
    m_TimerThread.ConcludeThread(INFINITE);

    //���ñ���
    m_TimerItemFree.Append(m_TimerItemActive);
    m_TimerItemActive.RemoveAll();
	m_dwTimePass = 0L;
	m_dwTimeLeave = NO_TIME_LEFT;
    return true;
}

//���ýӿ�
bool CTimerEngine::SetTimerEngineEvent(IUnknownEx * pIUnknownEx) {
    //Ч�����
    ASSERT(pIUnknownEx != NULL);
    ASSERT(m_bService == false);
    if (m_bService == true) return false;
    if (pIUnknownEx == NULL) return false;

    //���ýӿ�
    ASSERT(pIUnknownEx != NULL);
    //���ýӿ�
    //if (m_QueueServiceEvent.SetQueueServiceSink(pIUnknownEx) == false)
    //{
    //	CTraceService::TraceString(TEXT("��ʱ�������봥�������ʧ��"), TraceLevel_Exception);
    //	return false;
    //}
    //edit by xujialong
    m_pIQueueServiceSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IQueueServiceSink);
    ASSERT(m_pIQueueServiceSink != NULL);
    IAttemperEngine* pIAttemperSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngine);
    ASSERT(pIAttemperSink != NULL);
    m_QueueServiceEvent.SetQueueService((CQueueService*)pIAttemperSink->GetQueue());
    return (m_pIQueueServiceSink != NULL);
    //return true;
}

//��ʱ��֪ͨ
void CTimerEngine::OnTimerThreadSink() {
    //��������
    CWHDataLocker lock(m_CriticalSection);
    if (m_dwTimeLeave == NO_TIME_LEFT) {
        ASSERT(m_TimerItemActive.GetCount() == 0);
        return;
    }
    ASSERT(m_dwTimeLeave >= TIMER_SPACE);
    m_dwTimeLeave -= TIMER_SPACE;
    m_dwTimePass += TIMER_SPACE;
	//��ѯ��ʱ��
    if (m_dwTimeLeave == 0) {
		bool bKillTimer = false;
		DWORD dwTimeLeave = NO_TIME_LEFT;
        tagTimerItem * pTimerItem = NULL;
        for (INT_PTR i = 0; i < m_TimerItemActive.GetCount();) {
            pTimerItem = m_TimerItemActive[i];
            ASSERT(pTimerItem != NULL);
            if (pTimerItem == NULL) return;

            bKillTimer = false;

			ASSERT(pTimerItem->dwTimeLeave >= m_dwTimePass);
			pTimerItem->dwTimeLeave -= m_dwTimePass;

            if (pTimerItem->dwTimeLeave == 0L) {
                m_QueueServiceEvent.PostTimerEvent(pTimerItem->wTimerID, pTimerItem->wBindParam);
                //���ô���
                if (pTimerItem->dwRepeatTimes != TIMES_INFINITY) {
                    ASSERT(pTimerItem->dwRepeatTimes > 0);
                    pTimerItem->dwRepeatTimes--;
                    if (pTimerItem->dwRepeatTimes == 0L) {
                        bKillTimer = true;
                        m_TimerItemActive.RemoveAt(i);
                        m_TimerItemFree.Add(pTimerItem);
                    }
                }
                if (bKillTimer == false) pTimerItem->dwTimeLeave = pTimerItem->dwElapse;
            }

            if (bKillTimer == false) {
                i++;
				dwTimeLeave = min(dwTimeLeave, pTimerItem->dwTimeLeave);
				ASSERT(dwTimeLeave%TIMER_SPACE == 0);
            }
        }
		m_dwTimePass = 0;
		m_dwTimeLeave = dwTimeLeave;
    }

}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * CreateTimerEngine(const GUID & Guid, DWORD dwInterfaceVer) {
    //��������
    CTimerEngine * pTimerEngine = NULL;
    try {
        pTimerEngine = new CTimerEngine();
        if (pTimerEngine == NULL) throw TEXT("����ʧ��");
        void * pObject = pTimerEngine->QueryInterface(Guid, dwInterfaceVer);
        if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
        return pObject;
    } catch (...) {}

    //�������
    SafeDelete(pTimerEngine);
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
