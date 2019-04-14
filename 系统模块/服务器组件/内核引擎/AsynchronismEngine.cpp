#include "StdAfx.h"
#include "TraceService.h" // #include "EventService.h"
#include "AsynchronismEngine.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define WM_ASYN_REQUEST					WM_USER+10						//�첽����

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_MESSAGE(WM_ASYN_REQUEST, OnAsynRequest)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlWnd::CControlWnd()
{
	m_pAsynchronismEngine = NULL;
}

//��������
CControlWnd::~CControlWnd()
{
}

//������Ϣ
HRESULT CControlWnd::OnAsynRequest(WPARAM wParam, LPARAM lParam)
{
	//Ч�����
	ASSERT(m_pAsynchronismEngine != NULL);

	//��ȡ����
	WORD wRequestID = (WORD)wParam;
	IAsynchronismEngineSink * pIAsynchronismEngineSink = (IAsynchronismEngineSink *)lParam;

	//������
	ASSERT(pIAsynchronismEngineSink != NULL);
	m_pAsynchronismEngine->OnAsynchronismRequest(wRequestID, pIAsynchronismEngineSink);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CMessageThread::CMessageThread()
{
	m_pAsynchronismEngine = NULL;
}

//��������
CMessageThread::~CMessageThread()
{
}

//���к���
bool CMessageThread::OnEventThreadRun()
{
	MSG Message;
	if (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		return true;
	}
	return false;
}

//ֹͣ�߳�
bool CMessageThread::StopThread(DWORD dwWaitSeconds)
{
	PostThreadMessage(WM_QUIT, 0, 0);
	return __super::ConcludeThread(dwWaitSeconds);
}

//��ֹ�߳�
bool CMessageThread::TerminateThread(DWORD dwExitCode)
{
	PostThreadMessage(WM_QUIT, 0, 0);
	return __super::ConcludeThread(INFINITE);
}

//��ʼ�¼�
bool CMessageThread::OnThreadStratEvent()
{
	ASSERT(m_pAsynchronismEngine != NULL);
	return m_pAsynchronismEngine->OnMessageThreadStart();
}

//ֹͣ�¼�
bool CMessageThread::OnThreadStopEvent()
{
	ASSERT(m_pAsynchronismEngine != NULL);
	return m_pAsynchronismEngine->OnMessageThreadStop();
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CAsynchronismEngine::CAsynchronismEngine(void)
{
	//���ñ���
	m_bService = false;
	memset(m_cbBuffer, 0, sizeof(m_cbBuffer));

	//�������
	m_ControlWnd.m_pAsynchronismEngine = this;
	m_MessageThread.m_pAsynchronismEngine = this;

	return;
}

//��������
CAsynchronismEngine::~CAsynchronismEngine(void)
{
}

//�ӿڲ�ѯ
void * CAsynchronismEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAsynchronismEngine, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAsynchronismEngine, Guid, dwQueryVer);
	return NULL;
}

//��������
bool CAsynchronismEngine::StartService()
{
	//�����߳�
	if (m_MessageThread.StartThread() == false) return false;

	//���ñ���
	m_bService = true;

	return true;
}

//ֹͣ����
bool CAsynchronismEngine::ConcludeService()
{
	//���ñ���
	m_bService = false;

	//ֹͣ�߳�
	m_MessageThread.ConcludeThread(INFINITE);

	//��������
	m_DataStorage.RemoveData(false);

	return true;
}

//��������
bool CAsynchronismEngine::InsertRequest(WORD wRequestID, void * const pBuffer, WORD wDataSize, IUnknownEx * pIUnknownEx)
{
	//Ч��״̬
	ASSERT(wDataSize <= sizeof(m_cbBuffer));
	if (m_bService == false) return false;
	if (wDataSize > sizeof(m_cbBuffer)) return false;

	//��ѯ�ӿ�
	IAsynchronismEngineSink * pIAsynchronismEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAsynchronismEngineSink);
	ASSERT(pIAsynchronismEngineSink != NULL);
	if (pIAsynchronismEngineSink == NULL) return false;

	//��������
	CWHDataLocker lcok(m_CriticalSection); // CWHDataLockerHandle ThreadLockHandle(&m_ThreadLock);
	if (m_DataStorage.InsertData(wRequestID, pBuffer, wDataSize) == false) return false;

	//Ͷ����Ϣ
	PostMessage(m_ControlWnd.m_hWnd, WM_ASYN_REQUEST, wRequestID, (LPARAM)pIAsynchronismEngineSink);

	return true;
}

//���и���
bool CAsynchronismEngine::GetBurthenInfo( tagBurthenInfo & BurthenInfo )
{
	ASSERT(TRUE);ASSERT(FALSE);
	return true;
}

//ע�ṳ��
bool CAsynchronismEngine::SetAsynchronismSink(IUnknownEx * pIUnknownEx)
{
	//��ȡ�ӿ�
	IAsynchronismEngineSink * pIAsynchronismEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAsynchronismEngineSink);
	ASSERT(pIAsynchronismEngineSink != NULL);
	if (pIAsynchronismEngineSink == NULL) return false;

	//�����ִ�
	IAsynchronismEngineSink * pIAsynchronismEngineSinkTemp = NULL;
	for (INT_PTR i = 0; i < m_AsynchronismEngineSinkArray.GetCount(); i++)
	{
		pIAsynchronismEngineSinkTemp = m_AsynchronismEngineSinkArray[0];
		ASSERT(pIAsynchronismEngineSinkTemp != NULL);
		if (pIAsynchronismEngineSinkTemp == pIAsynchronismEngineSink) return false;
	}

	//ע�ṳ��
	m_AsynchronismEngineSinkArray.Add(pIAsynchronismEngineSink);

	return true;
}

//Ͷ������
bool CAsynchronismEngine::PostAsynchronismData(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(wDataSize <= sizeof(m_cbBuffer));
	if (m_bService == false) return false;
	if (wDataSize > sizeof(m_cbBuffer)) return false;

// 	//��ѯ�ӿ�
// 	IAsynchronismEngineSink * pIAsynchronismEngineSink = QUERY_ME_INTERFACE(IAsynchronismEngineSink);
// 	ASSERT(pIAsynchronismEngineSink != NULL);
// 	if (pIAsynchronismEngineSink == NULL) return false;

	//��������
	CWHDataLocker lcok(m_CriticalSection); // CWHDataLockerHandle ThreadLockHandle(&m_ThreadLock);
	if (m_DataStorage.InsertData(wIdentifier, pData, wDataSize) == false) return false;

	//Ͷ����Ϣ
	PostMessage(m_ControlWnd.m_hWnd, WM_ASYN_REQUEST, (WPARAM)pData, /*(LPARAM)pIAsynchronismEngineSink*/0);

	return true;
}

//Ͷ������
bool CAsynchronismEngine::PostAsynchronismData( WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount )
{
	ASSERT(TRUE);ASSERT(FALSE);
	return true;
}

//ȡ��ע��
bool CAsynchronismEngine::UnRegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx)
{
	//��ȡ�ӿ�
	IAsynchronismEngineSink * pIAsynchronismEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAsynchronismEngineSink);
	ASSERT(pIAsynchronismEngineSink != NULL);
	if (pIAsynchronismEngineSink == NULL) return false;

	//ɾ������
	IAsynchronismEngineSink * pIAsynchronismEngineSinkTemp = NULL;
	for (INT_PTR i = 0; i < m_AsynchronismEngineSinkArray.GetCount(); i++)
	{
		pIAsynchronismEngineSinkTemp = m_AsynchronismEngineSinkArray[0];
		ASSERT(pIAsynchronismEngineSinkTemp != NULL);
		if (pIAsynchronismEngineSinkTemp == pIAsynchronismEngineSink)
		{
			m_AsynchronismEngineSinkArray.RemoveAt(i);
			return true;
		}
	}

	return false;
}

//�߳�����
bool CAsynchronismEngine::OnMessageThreadStart()
{
	//��������
	if (m_ControlWnd.m_hWnd == NULL)
		m_ControlWnd.Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), CWnd::GetDesktopWindow(), 10);

	//�¼�֪ͨ
	bool bSuccsee = true;
	IAsynchronismEngineSink * pIAsynchronismEngineSink = NULL;
	for (INT_PTR i = 0; i < m_AsynchronismEngineSinkArray.GetCount(); i++)
	{
		pIAsynchronismEngineSink = m_AsynchronismEngineSinkArray[0];
		ASSERT(pIAsynchronismEngineSink != NULL);
		try
		{
			if (pIAsynchronismEngineSink->OnAsynchronismEngineStart() == false) bSuccsee = false;
		}
		catch (...)
		{
			bSuccsee = false;
		}
	}

	return bSuccsee;
}

//�߳�ֹͣ
bool CAsynchronismEngine::OnMessageThreadStop()
{
	//�رմ���
	m_ControlWnd.DestroyWindow();

	//�¼�֪ͨ
	bool bSuccsee = true;
	IAsynchronismEngineSink * pIAsynchronismEngineSink = NULL;
	for (INT_PTR i = 0; i < m_AsynchronismEngineSinkArray.GetCount(); i++)
	{
		pIAsynchronismEngineSink = m_AsynchronismEngineSinkArray[0];
		ASSERT(pIAsynchronismEngineSink != NULL);
		try
		{
			if (pIAsynchronismEngineSink->OnAsynchronismEngineConclude() == false) bSuccsee = false;
		}
		catch (...)
		{
			bSuccsee = false;
		}
	}

	return bSuccsee;
}

//�첽����
void CAsynchronismEngine::OnAsynchronismRequest(WORD wRequestID, IAsynchronismEngineSink * pIAsynchronismEngineSink)
{
	//������Դ
	CWHDataLocker lcok(m_CriticalSection);

	//��ȡ����
	tagDataHead DataHead;
	m_DataStorage.DistillData(DataHead, m_cbBuffer, sizeof(m_cbBuffer));

	//��Ϣ����
	try
	{
		ASSERT(DataHead.wIdentifier == wRequestID);
		pIAsynchronismEngineSink->OnAsynchronismEngineData(wRequestID, m_cbBuffer, DataHead.wDataSize);
	}
	catch (...) {}

	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * CreateAsynchronismEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CAsynchronismEngine * pAsynchronismEngine = NULL;
	try
	{
		pAsynchronismEngine = new CAsynchronismEngine();
		if (pAsynchronismEngine == NULL) throw TEXT("����ʧ��");
		void * pObject = pAsynchronismEngine->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pAsynchronismEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
