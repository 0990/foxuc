#include "StdAfx.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CQueueServiceThread::CQueueServiceThread(void)
{
	m_hCompletionPort = NULL;
	memset(m_cbBuffer, 0, sizeof(m_cbBuffer));
}

//��������
CQueueServiceThread::~CQueueServiceThread(void)
{
}

//���ú���
bool CQueueServiceThread::InitThread(HANDLE hCompletionPort)
{
	//Ч�����
	ASSERT(IsRuning() == false);
	ASSERT(m_hCompletionPort == NULL);

	//���ñ���
	m_hCompletionPort = hCompletionPort;
	memset(m_cbBuffer, 0, sizeof(m_cbBuffer));

	return true;
}

//ȡ������
bool CQueueServiceThread::UnInitThread()
{
	//Ч�����
	ASSERT(IsRuning() == false);

	//���ñ���
	m_hCompletionPort = NULL;
	memset(m_cbBuffer, 0, sizeof(m_cbBuffer));

	return true;
}

//���к���
bool CQueueServiceThread::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort != NULL);

	//��������
	DWORD dwThancferred = 0;
	OVERLAPPED * pOverLapped = NULL;
	CQueueService * pQueueService = NULL;

	//�ȴ���ɶ˿�
	if (GetQueuedCompletionStatus(m_hCompletionPort, &dwThancferred, (PULONG_PTR)&pQueueService, &pOverLapped, INFINITE))
	{
		//�ж��˳�
		if (pQueueService == NULL) return false;

		//��ȡ����
		tagDataHead DataHead;
		bool bSuccess = pQueueService->GetData(DataHead, m_cbBuffer, sizeof(m_cbBuffer));
		ASSERT(bSuccess == true);

		//��������
		if (bSuccess == true)
			pQueueService->OnQueueServiceThread(DataHead, m_cbBuffer, DataHead.wDataSize);

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CQueueService::CQueueService(void)
{
	m_bService = false;
	m_hCompletionPort = NULL;
	m_pIQueueServiceSink = NULL;
}

//��������
CQueueService::~CQueueService(void)
{
	//ֹͣ����
	ConcludeService();

	return;
}

//�ӿڲ�ѯ
void * CQueueService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IQueueService, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IQueueService, Guid, dwQueryVer);
	return NULL;
}

//���ýӿ�
bool CQueueService::SetQueueServiceSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx != NULL);
	m_pIQueueServiceSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IQueueServiceSink);
	ASSERT(m_pIQueueServiceSink != NULL);
	return (m_pIQueueServiceSink != NULL);
}

//������Ϣ
bool CQueueService::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	CWHDataLocker lock(m_CriticalSection);//
	m_DataQueue.GetBurthenInfo(BurthenInfo);
	return true;
}

//��������
bool CQueueService::AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)
{
	CWHDataLocker lock(m_CriticalSection);//
	m_DataQueue.InsertData(wIdentifier, pBuffer, wDataSize);
	PostQueuedCompletionStatus(m_hCompletionPort, wDataSize, (ULONG_PTR)this, NULL);
	return true;
}

//��ʼ����
bool CQueueService::StartService()
{
	//Ч�����
	ASSERT(m_bService == false);
	ASSERT(m_hCompletionPort == NULL);
	ASSERT(m_pIQueueServiceSink != NULL);

	//������ɶ˿�
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (m_hCompletionPort == NULL) throw TEXT("���ж�����ɶ˿ڴ���ʧ��");

	//�����߳�
	if (m_QueueServiceThread.InitThread(m_hCompletionPort) == false) throw TEXT("���ж����̳߳�ʼ��ʧ��");
	if (m_QueueServiceThread.StartThread() == false) throw TEXT("���ж����߳�����ʧ��");
//	SetThreadPriority(m_QueueServiceThread.GetThreadHandle(), REALTIME_PRIORITY_CLASS);

	//���ò���
	m_bService = true;

	return true;
}

//ֹͣ����
bool CQueueService::ConcludeService()
{
	//���ñ���
	m_bService = false;

	//ֹͣ�߳�
	if (m_hCompletionPort != NULL)
		PostQueuedCompletionStatus(m_hCompletionPort, 0, NULL, NULL);

	m_QueueServiceThread.ConcludeThread(INFINITE);
	m_QueueServiceThread.UnInitThread();

	//�ر���ɶ˿�
	if (m_hCompletionPort != NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}

	//��������
	m_DataQueue.RemoveData(false);

	m_pIQueueServiceSink = NULL;

	return true;
}

//�߳̾��
HANDLE CQueueService::GetThreadHandle()
{
	return m_QueueServiceThread.GetThreadHandle();
}

//��ȡ����
bool CQueueService::GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize)
{
	CWHDataLocker lock(m_CriticalSection);//
	return m_DataQueue.DistillData(DataHead, pBuffer, wBufferSize);
}

//������Ϣ
void CQueueService::OnQueueServiceThread(const tagDataHead & DataHead, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIQueueServiceSink != NULL);
	try
	{
		m_pIQueueServiceSink->OnQueueServiceSink(DataHead.wIdentifier, pBuffer, DataHead.wDataSize);
	}
	catch (...) {}
	return;
}
