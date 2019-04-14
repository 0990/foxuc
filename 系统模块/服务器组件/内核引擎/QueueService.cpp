#include "StdAfx.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CQueueServiceThread::CQueueServiceThread(void)
{
	m_hCompletionPort = NULL;
	memset(m_cbBuffer, 0, sizeof(m_cbBuffer));
}

//析构函数
CQueueServiceThread::~CQueueServiceThread(void)
{
}

//配置函数
bool CQueueServiceThread::InitThread(HANDLE hCompletionPort)
{
	//效验参数
	ASSERT(IsRuning() == false);
	ASSERT(m_hCompletionPort == NULL);

	//设置变量
	m_hCompletionPort = hCompletionPort;
	memset(m_cbBuffer, 0, sizeof(m_cbBuffer));

	return true;
}

//取消配置
bool CQueueServiceThread::UnInitThread()
{
	//效验参数
	ASSERT(IsRuning() == false);

	//设置变量
	m_hCompletionPort = NULL;
	memset(m_cbBuffer, 0, sizeof(m_cbBuffer));

	return true;
}

//运行函数
bool CQueueServiceThread::OnEventThreadRun()
{
	//效验参数
	ASSERT(m_hCompletionPort != NULL);

	//变量定义
	DWORD dwThancferred = 0;
	OVERLAPPED * pOverLapped = NULL;
	CQueueService * pQueueService = NULL;

	//等待完成端口
	if (GetQueuedCompletionStatus(m_hCompletionPort, &dwThancferred, (PULONG_PTR)&pQueueService, &pOverLapped, INFINITE))
	{
		//判断退出
		if (pQueueService == NULL) return false;

		//获取数据
		tagDataHead DataHead;
		bool bSuccess = pQueueService->GetData(DataHead, m_cbBuffer, sizeof(m_cbBuffer));
		ASSERT(bSuccess == true);

		//处理数据
		if (bSuccess == true)
			pQueueService->OnQueueServiceThread(DataHead, m_cbBuffer, DataHead.wDataSize);

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CQueueService::CQueueService(void)
{
	m_bService = false;
	m_hCompletionPort = NULL;
	m_pIQueueServiceSink = NULL;
}

//析构函数
CQueueService::~CQueueService(void)
{
	//停止服务
	ConcludeService();

	return;
}

//接口查询
void * CQueueService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IQueueService, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IQueueService, Guid, dwQueryVer);
	return NULL;
}

//设置接口
bool CQueueService::SetQueueServiceSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx != NULL);
	m_pIQueueServiceSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IQueueServiceSink);
	ASSERT(m_pIQueueServiceSink != NULL);
	return (m_pIQueueServiceSink != NULL);
}

//负荷信息
bool CQueueService::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	CWHDataLocker lock(m_CriticalSection);//
	m_DataQueue.GetBurthenInfo(BurthenInfo);
	return true;
}

//加入数据
bool CQueueService::AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)
{
	CWHDataLocker lock(m_CriticalSection);//
	m_DataQueue.InsertData(wIdentifier, pBuffer, wDataSize);
	PostQueuedCompletionStatus(m_hCompletionPort, wDataSize, (ULONG_PTR)this, NULL);
	return true;
}

//开始服务
bool CQueueService::StartService()
{
	//效验参数
	ASSERT(m_bService == false);
	ASSERT(m_hCompletionPort == NULL);
	ASSERT(m_pIQueueServiceSink != NULL);

	//建立完成端口
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (m_hCompletionPort == NULL) throw TEXT("队列对象完成端口创建失败");

	//启动线程
	if (m_QueueServiceThread.InitThread(m_hCompletionPort) == false) throw TEXT("队列对象线程初始化失败");
	if (m_QueueServiceThread.StartThread() == false) throw TEXT("队列对象线程启动失败");
//	SetThreadPriority(m_QueueServiceThread.GetThreadHandle(), REALTIME_PRIORITY_CLASS);

	//设置参数
	m_bService = true;

	return true;
}

//停止服务
bool CQueueService::ConcludeService()
{
	//设置变量
	m_bService = false;

	//停止线程
	if (m_hCompletionPort != NULL)
		PostQueuedCompletionStatus(m_hCompletionPort, 0, NULL, NULL);

	m_QueueServiceThread.ConcludeThread(INFINITE);
	m_QueueServiceThread.UnInitThread();

	//关闭完成端口
	if (m_hCompletionPort != NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}

	//设置数据
	m_DataQueue.RemoveData(false);

	m_pIQueueServiceSink = NULL;

	return true;
}

//线程句柄
HANDLE CQueueService::GetThreadHandle()
{
	return m_QueueServiceThread.GetThreadHandle();
}

//提取数据
bool CQueueService::GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize)
{
	CWHDataLocker lock(m_CriticalSection);//
	return m_DataQueue.DistillData(DataHead, pBuffer, wBufferSize);
}

//数据消息
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
