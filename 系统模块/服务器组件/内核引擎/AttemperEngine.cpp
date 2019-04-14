#include "StdAfx.h"
#include "TraceService.h" // #include "EventService.h"
#include "AttemperEngine.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CAttemperEngine::CAttemperEngine(void) {
    m_bService = false;
    m_pITCPSocketService = NULL;
    m_pTCPNetworkEngine = NULL;
    m_pIAttemperEngineSink = NULL;
    return;
}

//析构函数
CAttemperEngine::~CAttemperEngine(void) {
}

//接口查询
void * CAttemperEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer) {
    QUERYINTERFACE(IAttemperEngine, Guid, dwQueryVer);
    QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
    QUERYINTERFACE(IDataBaseEngineEvent, Guid, dwQueryVer);
    QUERYINTERFACE(ITCPNetworkEngineEvent, Guid, dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX(IAttemperEngine, Guid, dwQueryVer);
    return NULL;
}

//启动服务
bool CAttemperEngine::StartService() {
    //判断状态
    if (m_bService == true) {
        CTraceService::TraceString(TEXT("调度引擎重复启动，启动操作忽略"), TraceLevel_Warning);
        return true;
    }

    //外挂接口
    if (m_pIAttemperEngineSink == NULL) {
        CTraceService::TraceString(TEXT("调度引擎外挂服务不存在"), TraceLevel_Exception);
        return false;
    }

    //设置队列
    if (m_QueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false) {
        CTraceService::TraceString(TEXT("调度引擎与队列服务绑定失败"), TraceLevel_Exception);
        return false;
    }

    //启动外挂
    if (m_pIAttemperEngineSink->OnAttemperEngineStart(QUERY_ME_INTERFACE(IUnknownEx)) == false) {
        CTraceService::TraceString(TEXT("调度引擎外挂服务启动失败"), TraceLevel_Exception);
        return false;
    }

    m_QueueServiceEvent.SetQueueService(&m_QueueService);
    //启动队列
    if (m_QueueService.StartService() == false) {
        CTraceService::TraceString(TEXT("调度引擎队列服务启动失败"), TraceLevel_Exception);
        return false;
    }

    //清空机器人
    m_AndroidUserSocketIDArray.RemoveAll();

    //设置变量
    m_bService = true;

    return true;
}

//停止服务
bool CAttemperEngine::ConcludeService() {
    //设置变量
    m_bService = false;

    //停止请求队列
    m_QueueService.ConcludeService();

    //停止外挂
    if (m_pIAttemperEngineSink != NULL) {
        m_pIAttemperEngineSink->OnAttemperEngineConclude(QUERY_ME_INTERFACE(IUnknownEx));
        m_pIAttemperEngineSink = NULL;
    }

    //清空机器人
    m_AndroidUserSocketIDArray.RemoveAll();

    return true;
}

//设置网络
bool CAttemperEngine::SetSocketEngine(IUnknownEx * pIUnknownEx) {
    ASSERT(pIUnknownEx != NULL);
    m_pITCPSocketService = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPSocketService);
    ASSERT(m_pITCPSocketService != NULL);
    return (m_pITCPSocketService != NULL);
}

//设置网络
bool CAttemperEngine::SetNetworkEngine(IUnknownEx * pIUnknownEx) {
    ASSERT(pIUnknownEx != NULL);
    m_pTCPNetworkEngine = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPNetworkEngine);
    ASSERT(m_pTCPNetworkEngine != NULL);
    return (m_pTCPNetworkEngine != NULL);
}

//注册钩子
bool CAttemperEngine::SetAttemperEngineSink(IUnknownEx * pIUnknownEx) {
    //效验参数
    ASSERT(pIUnknownEx != NULL);
    ASSERT(m_pIAttemperEngineSink == NULL);
    if (pIUnknownEx == NULL) return false;
    if (m_pIAttemperEngineSink != NULL) return false;

    //查询接口
    m_pIAttemperEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngineSink);
    if (m_pIAttemperEngineSink == NULL) {
        CTraceService::TraceString(TEXT("调度引擎外挂服务接口获取失败，挂接操作失败"), TraceLevel_Exception);
        return false;
    }

    return true;
}

//获取接口
void * CAttemperEngine::GetQueueService(const IID & Guid, DWORD dwQueryVer) {
    return m_QueueService.QueryInterface(Guid, dwQueryVer);
}

//获取接口
void * CAttemperEngine::GetQueue() {
    return &m_QueueService;
}


//控制事件
//////////////////////////////////////////////////////////////////////////
//自定事件
bool CAttemperEngine::OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize) {
    //ASSERT(m_pIAttemperEngineSink);
    if (m_pIAttemperEngineSink == NULL) return false;

    return m_pIAttemperEngineSink->OnEventAttemperData(wRequestID, pData, wDataSize);
}

//控制事件
bool CAttemperEngine::OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize) {
    //ASSERT(m_pIAttemperEngineSink);
    if (m_pIAttemperEngineSink == NULL) return false;

    return m_pIAttemperEngineSink->OnEventControl(wControlID, pData, wDataSize);
}

//队列接口
void CAttemperEngine::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize) {
    //内核事件
    ASSERT(m_pIAttemperEngineSink != NULL);
    switch (wIdentifier) {
    case EVENT_TIMER: {		//定时器事件
        //效验参数
        ASSERT(wDataSize == sizeof(NTY_TimerEvent));
        if (wDataSize != sizeof(NTY_TimerEvent)) return;

        //处理消息
        NTY_TimerEvent * pTimerEvent = (NTY_TimerEvent *)pBuffer;
        try {
            m_pIAttemperEngineSink->OnEventTimer(pTimerEvent->dwTimerID, pTimerEvent->dwBindParameter);
        } catch (...) {
        }

        return;
    }

    case EVENT_DATABASE: {	//数据库事件
        //效验参数
        ASSERT(wDataSize >= sizeof(NTY_DataBaseEvent));
        if (wDataSize < sizeof(NTY_DataBaseEvent)) return;

        //处理消息
        NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)pBuffer;
        try {
            m_pIAttemperEngineSink->OnEventDataBase(pDataBaseEvent->wRequestID, pDataBaseEvent->dwContextID, pDataBaseEvent + 1, wDataSize - sizeof(NTY_DataBaseEvent));
        } catch (...) {
        }
        return;
    }

    case EVENT_TCP_NETWORK_ACCEPT: {	//网络应答事件
        //效验大小
        ASSERT(wDataSize == sizeof(NTY_TCPNetworkAcceptEvent));
        if (wDataSize != sizeof(NTY_TCPNetworkAcceptEvent)) return;

        //处理消息
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

    case EVENT_TCP_NETWORK_READ: {	//网络读取事件
        //效验大小
        NTY_TCPNetworkReadEvent * pSocketReadEvent = (NTY_TCPNetworkReadEvent *)pBuffer;
        ASSERT(wDataSize >= sizeof(NTY_TCPNetworkReadEvent));
        ASSERT(wDataSize == (sizeof(NTY_TCPNetworkReadEvent) + pSocketReadEvent->wDataSize));
        if (wDataSize < sizeof(NTY_TCPNetworkReadEvent)) return;
        if (wDataSize != (sizeof(NTY_TCPNetworkReadEvent) + pSocketReadEvent->wDataSize)) return;

        //处理消息
        bool bSuccess = false;
        try {
            bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkRead(pSocketReadEvent->Command, pSocketReadEvent + 1, pSocketReadEvent->wDataSize, pSocketReadEvent->dwSocketID);
        } catch (...) { }
        if (bSuccess == false) {
            TCHAR szDescribe[128] = TEXT("");
            _sntprintf(szDescribe, CountArray(szDescribe), TEXT("服务器处理到错误消息,mainID:%d,subID:%d"), (int)pSocketReadEvent->Command.wMainCmdID,(int)pSocketReadEvent->Command.wSubCmdID);
            CTraceService::TraceString(szDescribe, TraceLevel_Debug);
            m_pTCPNetworkEngine->CloseSocket(pSocketReadEvent->dwSocketID);
        }

        return;
    }

    case EVENT_TCP_NETWORK_SHUT: {	//关闭读取事件
        //效验大小
        ASSERT(wDataSize == sizeof(NTY_TCPNetworkShutEvent));
        if (wDataSize != sizeof(NTY_TCPNetworkShutEvent)) return;

        //处理消息
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
    case EVENT_TCP_NETWORK_FOREGROUND: {	//网络应答事件
        //效验大小
        ASSERT(wDataSize == sizeof(NTY_TCPNetworkForegroundEvent));
		if (wDataSize != sizeof(NTY_TCPNetworkForegroundEvent)) return;

        //处理消息
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

    case EVENT_TCP_SOCKET_READ: {	//网络读取事件
        //效验大小
        NTY_TCPSocketReadEvent * pSocketReadEvent = (NTY_TCPSocketReadEvent *)pBuffer;
        ASSERT(wDataSize >= sizeof(NTY_TCPSocketReadEvent));
        ASSERT(wDataSize == (sizeof(NTY_TCPSocketReadEvent) + pSocketReadEvent->wDataSize));
        if (wDataSize < sizeof(NTY_TCPSocketReadEvent)) return;
        if (wDataSize != (sizeof(NTY_TCPSocketReadEvent) + pSocketReadEvent->wDataSize)) return;


        //处理消息
        bool bSuccess = false;
        try {
            bSuccess = m_pIAttemperEngineSink->OnEventTCPSocketRead(pSocketReadEvent->wServiceID, pSocketReadEvent->Command, pSocketReadEvent + 1, pSocketReadEvent->wDataSize);
        } catch (...)	{ }
        if (bSuccess == false)
            m_pITCPSocketService->CloseSocket();

        return;
    }

    case EVENT_TCP_SOCKET_SHUT: {	//网络关闭事件
        //效验大小
        ASSERT(wDataSize == sizeof(NTY_TCPSocketShutEvent));
        if (wDataSize != sizeof(NTY_TCPSocketShutEvent)) return;

        //处理消息
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
        //效验大小
        ASSERT(wDataSize == sizeof(NTY_TCPSocketLinkEvent));
        if (wDataSize != sizeof(NTY_TCPSocketLinkEvent)) return;

        //处理消息
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
        //效验大小
        ASSERT(wDataSize == sizeof(NTY_ControlEvent));
        if (wDataSize != sizeof(NTY_ControlEvent)) return;

        //处理消息
        NTY_ControlEvent * pControlEvent = (NTY_ControlEvent *)pBuffer;
        try {
            m_pIAttemperEngineSink->OnEventControl(wIdentifier, pBuffer, wDataSize);
        } catch (...)	{ }

        return;
    }
    }

    //其他事件
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

//机器模拟接口
//////////////////////////////////////////////////////////////////////////
//应答事件
bool CAttemperEngine::OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientAddr) {
    //缓冲锁定
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

//关闭事件
bool CAttemperEngine::OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientAddr, DWORD dwActiveTime) {
    //缓冲锁定
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
        //投递消息
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

//读取事件
bool CAttemperEngine::OnEventTCPNetworkRead(DWORD dwSocketID, TCP_Command Command, VOID * pData, WORD wDataSize) {
    //缓冲锁定
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
//建立对象函数
//
extern "C" __declspec(dllexport) void * CreateAttemperEngine(const GUID & Guid, DWORD dwInterfaceVer) {
    //建立对象
    CAttemperEngine * pAttemperEngine = NULL;
    try {
        pAttemperEngine = new CAttemperEngine();
        if (pAttemperEngine == NULL) throw TEXT("创建失败");
        void * pObject = pAttemperEngine->QueryInterface(Guid, dwInterfaceVer);
        if (pObject == NULL) throw TEXT("接口查询失败");
        return pObject;
    } catch (...) {}

    //清理对象
    SafeDelete(pAttemperEngine);
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
