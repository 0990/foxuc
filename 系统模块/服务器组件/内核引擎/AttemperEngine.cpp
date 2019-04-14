#include "StdAfx.h"
#include "TraceService.h" // #include "EventService.h"
#include "AttemperEngine.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngine::CAttemperEngine(void) {
    m_bService = false;
    m_pITCPSocketService = NULL;
    m_pTCPNetworkEngine = NULL;
    m_pIAttemperEngineSink = NULL;
    return;
}

//��������
CAttemperEngine::~CAttemperEngine(void) {
}

//�ӿڲ�ѯ
void * CAttemperEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer) {
    QUERYINTERFACE(IAttemperEngine, Guid, dwQueryVer);
    QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
    QUERYINTERFACE(IDataBaseEngineEvent, Guid, dwQueryVer);
    QUERYINTERFACE(ITCPNetworkEngineEvent, Guid, dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX(IAttemperEngine, Guid, dwQueryVer);
    return NULL;
}

//��������
bool CAttemperEngine::StartService() {
    //�ж�״̬
    if (m_bService == true) {
        CTraceService::TraceString(TEXT("���������ظ�������������������"), TraceLevel_Warning);
        return true;
    }

    //��ҽӿ�
    if (m_pIAttemperEngineSink == NULL) {
        CTraceService::TraceString(TEXT("����������ҷ��񲻴���"), TraceLevel_Exception);
        return false;
    }

    //���ö���
    if (m_QueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false) {
        CTraceService::TraceString(TEXT("������������з����ʧ��"), TraceLevel_Exception);
        return false;
    }

    //�������
    if (m_pIAttemperEngineSink->OnAttemperEngineStart(QUERY_ME_INTERFACE(IUnknownEx)) == false) {
        CTraceService::TraceString(TEXT("����������ҷ�������ʧ��"), TraceLevel_Exception);
        return false;
    }

    m_QueueServiceEvent.SetQueueService(&m_QueueService);
    //��������
    if (m_QueueService.StartService() == false) {
        CTraceService::TraceString(TEXT("����������з�������ʧ��"), TraceLevel_Exception);
        return false;
    }

    //��ջ�����
    m_AndroidUserSocketIDArray.RemoveAll();

    //���ñ���
    m_bService = true;

    return true;
}

//ֹͣ����
bool CAttemperEngine::ConcludeService() {
    //���ñ���
    m_bService = false;

    //ֹͣ�������
    m_QueueService.ConcludeService();

    //ֹͣ���
    if (m_pIAttemperEngineSink != NULL) {
        m_pIAttemperEngineSink->OnAttemperEngineConclude(QUERY_ME_INTERFACE(IUnknownEx));
        m_pIAttemperEngineSink = NULL;
    }

    //��ջ�����
    m_AndroidUserSocketIDArray.RemoveAll();

    return true;
}

//��������
bool CAttemperEngine::SetSocketEngine(IUnknownEx * pIUnknownEx) {
    ASSERT(pIUnknownEx != NULL);
    m_pITCPSocketService = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPSocketService);
    ASSERT(m_pITCPSocketService != NULL);
    return (m_pITCPSocketService != NULL);
}

//��������
bool CAttemperEngine::SetNetworkEngine(IUnknownEx * pIUnknownEx) {
    ASSERT(pIUnknownEx != NULL);
    m_pTCPNetworkEngine = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPNetworkEngine);
    ASSERT(m_pTCPNetworkEngine != NULL);
    return (m_pTCPNetworkEngine != NULL);
}

//ע�ṳ��
bool CAttemperEngine::SetAttemperEngineSink(IUnknownEx * pIUnknownEx) {
    //Ч�����
    ASSERT(pIUnknownEx != NULL);
    ASSERT(m_pIAttemperEngineSink == NULL);
    if (pIUnknownEx == NULL) return false;
    if (m_pIAttemperEngineSink != NULL) return false;

    //��ѯ�ӿ�
    m_pIAttemperEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngineSink);
    if (m_pIAttemperEngineSink == NULL) {
        CTraceService::TraceString(TEXT("����������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"), TraceLevel_Exception);
        return false;
    }

    return true;
}

//��ȡ�ӿ�
void * CAttemperEngine::GetQueueService(const IID & Guid, DWORD dwQueryVer) {
    return m_QueueService.QueryInterface(Guid, dwQueryVer);
}

//��ȡ�ӿ�
void * CAttemperEngine::GetQueue() {
    return &m_QueueService;
}


//�����¼�
//////////////////////////////////////////////////////////////////////////
//�Զ��¼�
bool CAttemperEngine::OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize) {
    //ASSERT(m_pIAttemperEngineSink);
    if (m_pIAttemperEngineSink == NULL) return false;

    return m_pIAttemperEngineSink->OnEventAttemperData(wRequestID, pData, wDataSize);
}

//�����¼�
bool CAttemperEngine::OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize) {
    //ASSERT(m_pIAttemperEngineSink);
    if (m_pIAttemperEngineSink == NULL) return false;

    return m_pIAttemperEngineSink->OnEventControl(wControlID, pData, wDataSize);
}

//���нӿ�
void CAttemperEngine::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize) {
    //�ں��¼�
    ASSERT(m_pIAttemperEngineSink != NULL);
    switch (wIdentifier) {
    case EVENT_TIMER: {		//��ʱ���¼�
        //Ч�����
        ASSERT(wDataSize == sizeof(NTY_TimerEvent));
        if (wDataSize != sizeof(NTY_TimerEvent)) return;

        //������Ϣ
        NTY_TimerEvent * pTimerEvent = (NTY_TimerEvent *)pBuffer;
        try {
            m_pIAttemperEngineSink->OnEventTimer(pTimerEvent->dwTimerID, pTimerEvent->dwBindParameter);
        } catch (...) {
        }

        return;
    }

    case EVENT_DATABASE: {	//���ݿ��¼�
        //Ч�����
        ASSERT(wDataSize >= sizeof(NTY_DataBaseEvent));
        if (wDataSize < sizeof(NTY_DataBaseEvent)) return;

        //������Ϣ
        NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)pBuffer;
        try {
            m_pIAttemperEngineSink->OnEventDataBase(pDataBaseEvent->wRequestID, pDataBaseEvent->dwContextID, pDataBaseEvent + 1, wDataSize - sizeof(NTY_DataBaseEvent));
        } catch (...) {
        }
        return;
    }

    case EVENT_TCP_NETWORK_ACCEPT: {	//����Ӧ���¼�
        //Ч���С
        ASSERT(wDataSize == sizeof(NTY_TCPNetworkAcceptEvent));
        if (wDataSize != sizeof(NTY_TCPNetworkAcceptEvent)) return;

        //������Ϣ
        NTY_TCPNetworkAcceptEvent * pSocketAcceptEvent = (NTY_TCPNetworkAcceptEvent *)pBuffer;
        bool bSuccess = false;
        try {
            bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkBind(pSocketAcceptEvent->dwClientAddr, pSocketAcceptEvent->dwSocketID);
        } catch (...) { }
        if (bSuccess == false) {
            m_pTCPNetworkEngine->CloseSocket(pSocketAcceptEvent->dwSocketID);
        }

        return;
    }

    case EVENT_TCP_NETWORK_READ: {	//�����ȡ�¼�
        //Ч���С
        NTY_TCPNetworkReadEvent * pSocketReadEvent = (NTY_TCPNetworkReadEvent *)pBuffer;
        ASSERT(wDataSize >= sizeof(NTY_TCPNetworkReadEvent));
        ASSERT(wDataSize == (sizeof(NTY_TCPNetworkReadEvent) + pSocketReadEvent->wDataSize));
        if (wDataSize < sizeof(NTY_TCPNetworkReadEvent)) return;
        if (wDataSize != (sizeof(NTY_TCPNetworkReadEvent) + pSocketReadEvent->wDataSize)) return;

        //������Ϣ
        bool bSuccess = false;
        try {
            bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkRead(pSocketReadEvent->Command, pSocketReadEvent + 1, pSocketReadEvent->wDataSize, pSocketReadEvent->dwSocketID);
        } catch (...) { }
        if (bSuccess == false) {
            TCHAR szDescribe[128] = TEXT("");
            _sntprintf(szDescribe, CountArray(szDescribe), TEXT("����������������Ϣ,mainID:%d,subID:%d"), (int)pSocketReadEvent->Command.wMainCmdID,(int)pSocketReadEvent->Command.wSubCmdID);
            CTraceService::TraceString(szDescribe, TraceLevel_Debug);
            m_pTCPNetworkEngine->CloseSocket(pSocketReadEvent->dwSocketID);
        }

        return;
    }

    case EVENT_TCP_NETWORK_SHUT: {	//�رն�ȡ�¼�
        //Ч���С
        ASSERT(wDataSize == sizeof(NTY_TCPNetworkShutEvent));
        if (wDataSize != sizeof(NTY_TCPNetworkShutEvent)) return;

        //������Ϣ
        NTY_TCPNetworkShutEvent * pNetworkShutEvent = (NTY_TCPNetworkShutEvent *)pBuffer;

        bool bSuccess = false;
        try {
            bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkShut(pNetworkShutEvent->dwClientAddr, pNetworkShutEvent->dwActiveTime, pNetworkShutEvent->dwSocketID);
        } catch (...)	{ }
        if (bSuccess == false) {
            m_pTCPNetworkEngine->CloseSocket(pNetworkShutEvent->dwSocketID);
        }

        return;
    }
    case EVENT_TCP_NETWORK_FOREGROUND: {	//����Ӧ���¼�
        //Ч���С
        ASSERT(wDataSize == sizeof(NTY_TCPNetworkForegroundEvent));
		if (wDataSize != sizeof(NTY_TCPNetworkForegroundEvent)) return;

        //������Ϣ
		NTY_TCPNetworkForegroundEvent * pSocketForegroundEvent = (NTY_TCPNetworkForegroundEvent *)pBuffer;
        bool bSuccess = false;
        try {
			bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkForeground(pSocketForegroundEvent->bForeground, pSocketForegroundEvent->dwSocketID);
        } catch (...) {}
        if (bSuccess == false) {
			m_pTCPNetworkEngine->CloseSocket(pSocketForegroundEvent->dwSocketID);
        }

        return;
    }

    case EVENT_TCP_SOCKET_READ: {	//�����ȡ�¼�
        //Ч���С
        NTY_TCPSocketReadEvent * pSocketReadEvent = (NTY_TCPSocketReadEvent *)pBuffer;
        ASSERT(wDataSize >= sizeof(NTY_TCPSocketReadEvent));
        ASSERT(wDataSize == (sizeof(NTY_TCPSocketReadEvent) + pSocketReadEvent->wDataSize));
        if (wDataSize < sizeof(NTY_TCPSocketReadEvent)) return;
        if (wDataSize != (sizeof(NTY_TCPSocketReadEvent) + pSocketReadEvent->wDataSize)) return;


        //������Ϣ
        bool bSuccess = false;
        try {
            bSuccess = m_pIAttemperEngineSink->OnEventTCPSocketRead(pSocketReadEvent->wServiceID, pSocketReadEvent->Command, pSocketReadEvent + 1, pSocketReadEvent->wDataSize);
        } catch (...)	{ }
        if (bSuccess == false)
            m_pITCPSocketService->CloseSocket();

        return;
    }

    case EVENT_TCP_SOCKET_SHUT: {	//����ر��¼�
        //Ч���С
        ASSERT(wDataSize == sizeof(NTY_TCPSocketShutEvent));
        if (wDataSize != sizeof(NTY_TCPSocketShutEvent)) return;

        //������Ϣ
        NTY_TCPSocketShutEvent * pSocketShutEvent = (NTY_TCPSocketShutEvent *)pBuffer;

        bool bSuccess = false;
        try {
            bSuccess = m_pIAttemperEngineSink->OnEventTCPSocketShut(pSocketShutEvent->wServiceID, pSocketShutEvent->cbShutReason);
        } catch (...)	{ }
        if (bSuccess == false)
            m_pITCPSocketService->CloseSocket();

        return;
    }

    case EVENT_TCP_SOCKET_LINK: {
        //Ч���С
        ASSERT(wDataSize == sizeof(NTY_TCPSocketLinkEvent));
        if (wDataSize != sizeof(NTY_TCPSocketLinkEvent)) return;

        //������Ϣ
        NTY_TCPSocketLinkEvent * pSocketLinkEvent = (NTY_TCPSocketLinkEvent *)pBuffer;

        bool bSuccess = false;
        try {
            bSuccess = m_pIAttemperEngineSink->OnEventTCPSocketLink(pSocketLinkEvent->wServiceID, pSocketLinkEvent->nErrorCode);
        } catch (...)	{ }
        if (bSuccess == false)
            m_pITCPSocketService->CloseSocket();

        return;
    }

    case EVENT_CONTROL: {
        //Ч���С
        ASSERT(wDataSize == sizeof(NTY_ControlEvent));
        if (wDataSize != sizeof(NTY_ControlEvent)) return;

        //������Ϣ
        NTY_ControlEvent * pControlEvent = (NTY_ControlEvent *)pBuffer;
        try {
            m_pIAttemperEngineSink->OnEventControl(wIdentifier, pBuffer, wDataSize);
        } catch (...)	{ }

        return;
    }
    }

    //�����¼�
    try {
        m_pIAttemperEngineSink->OnEventAttemperData(wIdentifier, pBuffer, wDataSize);
    } catch (...)	{ }

    return;
}

bool CAttemperEngine::OnEventDataBaseResult(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //ASSERT(m_bService == true);
    if (m_bService == false) return false;
    return m_QueueServiceEvent.PostDataBaseEvent(wRequestID, dwContextID, pData, wDataSize);
    //try
    //{
    //	return m_pIAttemperEngineSink->OnEventDataBase(wRequestID, dwContextID, pData, wDataSize);
    //}
    //catch (...)
    //{
    //	return false;
    //}
}

//����ģ��ӿ�
//////////////////////////////////////////////////////////////////////////
//Ӧ���¼�
bool CAttemperEngine::OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientAddr) {
    //��������
    CWHDataLocker lock(m_CriticalSection);

    try {
        m_AndroidUserSocketIDArray.Add(dwSocketID);
        ASSERT(m_bService == true);
        m_QueueServiceEvent.PostNetworkAcceptEvent(dwSocketID, dwClientAddr);
        //m_pIAttemperEngineSink->OnEventTCPNetworkBind(dwClientAddr, dwSocketID);
    } catch (...) {
        return false;
    }

    return true;
}

//�ر��¼�
bool CAttemperEngine::OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientAddr, DWORD dwActiveTime) {
    //��������
    CWHDataLocker lock(m_CriticalSection);
    try {
        for (int i = 0; i < m_AndroidUserSocketIDArray.GetCount(); i++) {
            if (dwSocketID == m_AndroidUserSocketIDArray[i]) {
                m_AndroidUserSocketIDArray.RemoveAt(i);
                break;
            }
        }
        ASSERT(m_bService == true);

        m_QueueServiceEvent.PostNetworkShutEvent(dwSocketID, dwClientAddr, dwActiveTime);
        //Ͷ����Ϣ
        //NTY_TCPNetworkShutEvent * pNetworkShutEvent = (NTY_TCPNetworkShutEvent *)m_cbBuffer;
        //NTY_TCPNetworkShutEvent  networkShutEvent;
        //networkShutEvent.dwSocketID = dwSocketID;
        //networkShutEvent.dwActiveTime = dwActiveTime;
        //networkShutEvent.dwClientAddr = dwClientAddr;
        //m_QueueService.AddToQueue(EVENT_TCP_NETWORK_SHUT, &networkShutEvent, sizeof(NTY_TCPNetworkShutEvent));
        //m_pIAttemperEngineSink->OnEventTCPNetworkShut(dwClientAddr, dwActiveTime, dwSocketID);
    } catch (...)	{
        return false;
    }

    return true;
}

//��ȡ�¼�
bool CAttemperEngine::OnEventTCPNetworkRead(DWORD dwSocketID, TCP_Command Command, VOID * pData, WORD wDataSize) {
    //��������
    CWHDataLocker lock(m_CriticalSection);
    try {
        bool bIsConnected = false;
        for (int i = 0; i < m_AndroidUserSocketIDArray.GetCount(); i++) {
            if (dwSocketID == m_AndroidUserSocketIDArray[i]) {
                bIsConnected = true;
                break;
            }
        }
        ASSERT(bIsConnected);
        if (bIsConnected == false) return false;
        if(m_bService == false) return false;
        m_QueueServiceEvent.PostNetworkReadEvent(dwSocketID, Command, pData, wDataSize);
        //m_pIAttemperEngineSink->OnEventTCPNetworkRead(Command, pData, wDataSize, dwSocketID);
    } catch (...) {
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
//����������
//
extern "C" __declspec(dllexport) void * CreateAttemperEngine(const GUID & Guid, DWORD dwInterfaceVer) {
    //��������
    CAttemperEngine * pAttemperEngine = NULL;
    try {
        pAttemperEngine = new CAttemperEngine();
        if (pAttemperEngine == NULL) throw TEXT("����ʧ��");
        void * pObject = pAttemperEngine->QueryInterface(Guid, dwInterfaceVer);
        if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
        return pObject;
    } catch (...) {}

    //�������
    SafeDelete(pAttemperEngine);
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
