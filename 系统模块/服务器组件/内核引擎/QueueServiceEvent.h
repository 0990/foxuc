#ifndef QUEUE_SERVICE_EVENT_HEAD_FILE
#define QUEUE_SERVICE_EVENT_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//数据队列事件
class CQueueServiceEvent
{
	//变量定义
public:
	CCriticalSection				m_CriticalSection;						//同步锁定
	CQueueService*					m_pIQueueService;						//队列接口
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];		//接收缓冲

	//函数定义
public:
	//构造函数
	CQueueServiceEvent()
	{
	}
	//析构函数
	virtual ~CQueueServiceEvent()
	{
	}
	//线程句柄
	HANDLE GetThreadHandle()
	{
		return m_pIQueueService->GetThreadHandle();
	}
	//管理函数
public:
	void SetQueueService(CQueueService*	pIQueueService){
		m_pIQueueService = pIQueueService;
	}
	CQueueService* GetQueueService(){
		return m_pIQueueService;
	}
	////设置接口
	//bool SetQueueServiceSink(IUnknownEx * pIUnknownEx)
	//{
	//	if (NULL != pIUnknownEx)
	//	{
	//		bool bRet = m_pIQueueService->SetQueueServiceSink(pIUnknownEx);
	//		if (bRet)
	//			bRet = m_pIQueueService->StartService();

	//		return bRet;
	//	}
	//	else
	//	{
	//		m_pIQueueService->ConcludeService();
	//		return true;
	//	}
	//}

	//通知函数
public:
	//定时器事件
	bool PostTimerEvent(DWORD wTimerID, WPARAM wBindParam)
	{
		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_TimerEvent * pTimerEvent = (NTY_TimerEvent *)m_cbBuffer;
		pTimerEvent->dwTimerID = wTimerID;
		pTimerEvent->dwBindParameter = wBindParam;
		m_pIQueueService->AddToQueue(EVENT_TIMER, m_cbBuffer, sizeof(NTY_TimerEvent));

		return true;
	}

	//数据库事件
	bool PostDataBaseEvent(WORD wRequestID, DWORD dwContextID, const void * pDataBuffer, WORD wDataSize)
	{
		ASSERT((wDataSize + sizeof(NTY_DataBaseEvent)) <= MAX_ASYNCHRONISM_DATA);
		if ((wDataSize + sizeof(NTY_DataBaseEvent)) > MAX_ASYNCHRONISM_DATA) return false;

		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)m_cbBuffer;
		pDataBaseEvent->wRequestID = wRequestID;
		pDataBaseEvent->dwContextID = dwContextID;
		if (wDataSize > 0)
		{
			ASSERT(pDataBuffer != NULL);
			CopyMemory(m_cbBuffer + sizeof(NTY_DataBaseEvent), pDataBuffer, wDataSize);
		}
		m_pIQueueService->AddToQueue(EVENT_DATABASE, m_cbBuffer, sizeof(NTY_DataBaseEvent) + wDataSize);

		return true;
	}

	//数据库控制事件
	bool PostControlEvent(WORD wControlID, VOID * pData, WORD wDataSize)
	{
		//效验参数
		ASSERT((wDataSize + sizeof(NTY_ControlEvent)) <= MAX_ASYNCHRONISM_DATA);
		if ((wDataSize + sizeof(NTY_ControlEvent)) > MAX_ASYNCHRONISM_DATA) return false;

		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_ControlEvent * pDataBaseEvent = (NTY_ControlEvent *)m_cbBuffer;
		pDataBaseEvent->wControlID = wControlID;
		if (wDataSize > 0)
		{
			ASSERT(pData != NULL);
			CopyMemory(m_cbBuffer + sizeof(NTY_ControlEvent), pData, wDataSize);
		}
		m_pIQueueService->AddToQueue(EVENT_CONTROL, m_cbBuffer, sizeof(NTY_ControlEvent) + wDataSize);

		return true;
	}

	//SOCKET引擎
	//////////////////////////////////////////////////////////////////////////
	//网络应答事件
	bool PostNetworkAcceptEvent(DWORD dwSocketID, DWORD dwClientAddr)
	{
		//效验参数
		ASSERT(sizeof(NTY_TCPNetworkAcceptEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPNetworkAcceptEvent) > MAX_ASYNCHRONISM_DATA) return false;

		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_TCPNetworkAcceptEvent * pSocketAcceptEvent = (NTY_TCPNetworkAcceptEvent *)m_cbBuffer;
		pSocketAcceptEvent->dwSocketID = dwSocketID;
		pSocketAcceptEvent->dwClientAddr = dwClientAddr;
		m_pIQueueService->AddToQueue(EVENT_TCP_NETWORK_ACCEPT, m_cbBuffer, sizeof(NTY_TCPNetworkAcceptEvent));

		return true;
	}

	//网络读取事件
	bool PostNetworkReadEvent(DWORD dwSocketID, TCP_Command Command, const void * pDataBuffer, WORD wDataSize)
	{
		//效验参数
		ASSERT((wDataSize + sizeof(NTY_TCPNetworkReadEvent)) <= MAX_ASYNCHRONISM_DATA);
		if ((wDataSize + sizeof(NTY_TCPNetworkReadEvent)) > MAX_ASYNCHRONISM_DATA) return false;

		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_TCPNetworkReadEvent * pSocketReadEvent = (NTY_TCPNetworkReadEvent *)m_cbBuffer;
		pSocketReadEvent->dwSocketID = dwSocketID;
		pSocketReadEvent->Command = Command;
		pSocketReadEvent->wDataSize = wDataSize;
		if (wDataSize > 0)
		{
			ASSERT(pDataBuffer != NULL);
			CopyMemory(m_cbBuffer + sizeof(NTY_TCPNetworkReadEvent), pDataBuffer, wDataSize);
		}
		m_pIQueueService->AddToQueue(EVENT_TCP_NETWORK_READ, m_cbBuffer, sizeof(NTY_TCPNetworkReadEvent) + wDataSize);

		return true;
	}

	//网络关闭事件
	bool PostNetworkShutEvent(DWORD dwSocketID, DWORD dwClientAddr, DWORD dwActiveTime)
	{
		//效验参数
		ASSERT(sizeof(NTY_TCPNetworkShutEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPNetworkShutEvent) > MAX_ASYNCHRONISM_DATA) return false;

		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_TCPNetworkShutEvent * pNetworkShutEvent = (NTY_TCPNetworkShutEvent *)m_cbBuffer;
		pNetworkShutEvent->dwSocketID = dwSocketID;
		pNetworkShutEvent->dwActiveTime = dwActiveTime;
		pNetworkShutEvent->dwClientAddr = dwClientAddr;
		m_pIQueueService->AddToQueue(EVENT_TCP_NETWORK_SHUT, m_cbBuffer, sizeof(NTY_TCPNetworkShutEvent));

		return true;
	}

	//网络应答事件
	bool PostNetworkForegroundEvent(DWORD dwSocketID, bool foreground)
	{
		//效验参数
		ASSERT(sizeof(NTY_TCPNetworkForegroundEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPNetworkForegroundEvent) > MAX_ASYNCHRONISM_DATA) return false;

		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_TCPNetworkForegroundEvent * pSocketForegroundEvent = (NTY_TCPNetworkForegroundEvent *)m_cbBuffer;
		pSocketForegroundEvent->dwSocketID = dwSocketID;
		pSocketForegroundEvent->bForeground = foreground;
		m_pIQueueService->AddToQueue(EVENT_TCP_NETWORK_FOREGROUND, m_cbBuffer, sizeof(NTY_TCPNetworkForegroundEvent));

		return true;
	}

	//SOCKET服务
	//////////////////////////////////////////////////////////////////////////
	//网络关闭事件
	bool PostTCPSocketShutEvent(WORD wServiceID, BYTE cbShutReason)
	{
		//效验参数
		ASSERT(sizeof(NTY_TCPSocketShutEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPSocketShutEvent) > MAX_ASYNCHRONISM_DATA) return false;
		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_TCPSocketShutEvent * pSocketShutEvent = (NTY_TCPSocketShutEvent *)m_cbBuffer;
		pSocketShutEvent->wServiceID = wServiceID;
		pSocketShutEvent->cbShutReason = cbShutReason;
		return m_pIQueueService->AddToQueue(EVENT_TCP_SOCKET_SHUT, m_cbBuffer, sizeof(NTY_TCPSocketShutEvent));
	}

	//网络关闭事件
	bool PostTCPSocketReadEvent(WORD wServiceID, TCP_Command Command, const void * pDataBuffer, WORD wDataSize)
	{
		//效验参数
		ASSERT((wDataSize + sizeof(NTY_TCPSocketReadEvent)) <= MAX_ASYNCHRONISM_DATA);
		if ((wDataSize + sizeof(NTY_TCPSocketReadEvent)) > MAX_ASYNCHRONISM_DATA) return false;

		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_TCPSocketReadEvent * pSocketCloseEvent = (NTY_TCPSocketReadEvent *)m_cbBuffer;
		pSocketCloseEvent->wDataSize = wDataSize;
		pSocketCloseEvent->wServiceID = wServiceID;
		pSocketCloseEvent->Command = Command;
		if (wDataSize > 0)
		{
			ASSERT(pDataBuffer != NULL);
			CopyMemory(m_cbBuffer + sizeof(NTY_TCPSocketReadEvent), pDataBuffer, wDataSize);
		}
		m_pIQueueService->AddToQueue(EVENT_TCP_SOCKET_READ, m_cbBuffer, sizeof(NTY_TCPSocketReadEvent) + wDataSize);

		return true;
	}

	//网络关闭事件
	bool PostTCPSocketLinkEvent(WORD wServiceID, INT nErrorCode)
	{
		//效验参数
		ASSERT(sizeof(NTY_TCPSocketLinkEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPSocketLinkEvent) > MAX_ASYNCHRONISM_DATA) return false;
		//缓冲锁定
		CWHDataLocker lock(m_CriticalSection);

		//投递消息
		NTY_TCPSocketLinkEvent * pSocketLinkEvent = (NTY_TCPSocketLinkEvent *)m_cbBuffer;
		pSocketLinkEvent->wServiceID = wServiceID;
		pSocketLinkEvent->nErrorCode = nErrorCode;
		m_pIQueueService->AddToQueue(EVENT_TCP_SOCKET_LINK, m_cbBuffer, sizeof(NTY_TCPSocketLinkEvent));

		return true;
	}

};

//////////////////////////////////////////////////////////////////////////

#endif