#include "StdAfx.h"
#include "TraceService.h"
#include "TCPSocketService.h"


//宏定义
#define ID_SOCKET_WND				10								//SOCKET 窗口 ID
#define WM_SOCKET_NOTIFY			WM_USER+10						//SOCKET 消息

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTCPSocketService, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY, OnSocketNotify)
END_MESSAGE_MAP()

//函数定义
//////////////////////////////////////////////////////////////////////////
//构造函数
CTCPSocketService::CTCPSocketService(void)
{
	//恢复数据
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_hSocket = INVALID_SOCKET;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_wServiceID = 0;
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	m_wRecvSize = 0;
	m_dwServerIP = INADDR_NONE;
	m_wPort = 0;
	m_bSuccess = false;
}

//析构函数
CTCPSocketService::~CTCPSocketService(void)
{
	if (m_bService)
		ConcludeService();
}

//基础接口
//////////////////////////////////////////////////////////////////////////
//接口查询
void * CTCPSocketService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketService, Guid, dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocketService, Guid, dwQueryVer);
	return NULL;
}

//服务接口
//////////////////////////////////////////////////////////////////////////
//启动服务
bool CTCPSocketService::StartService()
{
	//效验状态
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("网络服务重复启动，启动操作忽略"), TraceLevel_Warning);
		return true;
	}

	//绑定对象
	if (m_QueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		CTraceService::TraceString(TEXT("网络服务与触发服务绑定失败"), TraceLevel_Exception);
		return false;
	}

	try
	{
		//绑定对象
		if (m_QueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
		{
			CTraceService::TraceString(TEXT("网络引擎与触发服务绑定失败"), TraceLevel_Exception);
			return false;
		}

 		//m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 		//if (m_hSocket == INVALID_SOCKET) return false;
		if ((m_hWnd == NULL) && (!Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), GetDesktopWindow(), ID_SOCKET_WND, NULL)))
			return false;

		if (m_QueueService.StartService())
		{
			m_bService = true;
		}
	}
	catch (LPCTSTR pszError)
	{
		CTraceService::TraceString(pszError, TraceLevel_Exception);
		return false;
	}

	return m_bService;
}

//停止服务
bool CTCPSocketService::ConcludeService()
{
	//设置变量
	m_bService = false;

	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		WSAAsyncSelect(m_hSocket, GetSafeHwnd(), WM_SOCKET_NOTIFY, 0);
		m_hSocket = INVALID_SOCKET;
	}

	//恢复数据
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_hSocket = INVALID_SOCKET;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	m_wRecvSize = 0;
	m_dwServerIP = INADDR_NONE;
	m_wPort = 0;
	m_bSuccess = false;

	//m_QueueServiceEvent.SetQueueServiceSink(NULL);

	m_QueueService.ConcludeService();

	return true;
}

//配置接口
//////////////////////////////////////////////////////////////////////////
//配置函数
bool CTCPSocketService::SetServiceID(WORD wServiceID)
{
	m_wServiceID = wServiceID;
	return true;
}

//设置接口
bool CTCPSocketService::SetTCPSocketEvent(IUnknownEx * pIUnknownEx)
{
	//状态判断
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("网络组件处于服务状态，绑定操作忽略"), TraceLevel_Warning);
		return false;
	}

	//设置接口
	//if (m_QueueServiceEvent.SetQueueServiceSink(pIUnknownEx) == false)
	//{
	//	CTraceService::TraceString(TEXT("网络组件与触发服务绑定失败"), TraceLevel_Warning);
	//	return false;
	//}

	IAttemperEngine* pIAttemperSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngine);
	ASSERT(pIAttemperSink != NULL);
	m_QueueServiceEvent.SetQueueService((CQueueService*)pIAttemperSink->GetQueue());

	return true;
}

//功能接口
//////////////////////////////////////////////////////////////////////////
//关闭连接
bool CTCPSocketService::CloseSocket()
{
	//关闭连接
	bool bClose = (m_hSocket != INVALID_SOCKET);
	if (bClose)
		return m_QueueService.AddToQueue(QUEUE_SAFE_CLOSE, NULL, 0);

	//恢复数据
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_hSocket = INVALID_SOCKET;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	m_wRecvSize = 0;
	m_dwServerIP = INADDR_NONE;
	m_wPort = 0;

	return true;
}

//连接地址
bool CTCPSocketService::Connect(DWORD dwServerIP, WORD wPort)
{
	if (m_bSuccess && ntohl(dwServerIP) == m_dwTempServerIP)//登录服务器有BUG -- 
		m_dwServerIP = ntohl(dwServerIP);
	else m_dwServerIP = dwServerIP;

	m_wPort = wPort;
	m_dwTempServerIP = m_dwServerIP;

	//设置参数
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0x12345678;
	m_dwRecvXorKey = 0x12345678;

	try
	{
		return m_QueueService.AddToQueue(QUEUE_CONNECT_REQUEST, NULL, 0);
	}
	catch (LPCTSTR pszError)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
		throw pszError;
	}
	catch (...)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
		throw TEXT("连接产生未知异常错误");
	}

	return false;
}

//连接地址
bool CTCPSocketService::Connect(LPCTSTR szServerIP, WORD wPort)
{
	//效验数据
	ASSERT(wPort != 0);
	ASSERT(szServerIP != NULL);
	if ((szServerIP == NULL) || (wPort == 0)) return false;
	return Connect(TranslateAddr(szServerIP), wPort);
}

//发送函数
bool CTCPSocketService::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验状态
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	//加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head), sizeof(cbDataBuffer));

	//发送数据
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//通知回调
void CTCPSocketService::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)
{
	switch (wIdentifier)
	{
		case QUEUE_SEND_REQUEST:
		{
			ASSERT(pBuffer);
			ASSERT(wDataSize > 0);
			if (!pBuffer || wDataSize <= 0) return;

			//发送数据
			WORD wSended = 0;
			while (wSended < wDataSize)
			{
				int iErrorCode = send(m_hSocket, (char *)pBuffer + wSended, wDataSize - wSended, 0);
				if (iErrorCode == SOCKET_ERROR)
				{
					if (WSAGetLastError() == WSAEWOULDBLOCK)
					{
						return;
					}
					return;
				}
				wSended += iErrorCode;
			}

			break;
		}

		case QUEUE_CONNECT_REQUEST:
		{
			//效验状态
			if (m_dwServerIP == INADDR_NONE) throw TEXT("目标服务器地址格式不正确，请检查后再次尝试！");
			if (m_wPort == 0) throw TEXT("目标服务器地址格式不正确，请检查后再次尝试！");

			//建立 SOCKET
			m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_hSocket == INVALID_SOCKET) throw TEXT("SOCKET 创建失败");

			//效验参数
			ASSERT(m_hSocket != INVALID_SOCKET);
			ASSERT(m_cbSocketStatus == SOCKET_STATUS_IDLE);

			int iErrorCode = WSAAsyncSelect(m_hSocket, m_hWnd, WM_SOCKET_NOTIFY, FD_READ | FD_CONNECT | FD_CLOSE);
			if (iErrorCode == SOCKET_ERROR) throw TEXT("绑定内部窗口错误");

			//填写服务器地址
			SOCKADDR_IN SocketAddr;
			memset(&SocketAddr, 0, sizeof(SocketAddr));
			SocketAddr.sin_family = AF_INET;
			SocketAddr.sin_port = htons(m_wPort);
			SocketAddr.sin_addr.S_un.S_addr = m_dwServerIP;

			iErrorCode = connect(m_hSocket, (sockaddr*) & SocketAddr, sizeof(sockaddr));
			if (SOCKET_ERROR == iErrorCode)
				iErrorCode = WSAGetLastError();

			break;
		}

		case QUEUE_SAFE_CLOSE:
		{
			if (m_hSocket != INVALID_SOCKET)
			{
				closesocket(m_hSocket);
				WSAAsyncSelect(m_hSocket, GetSafeHwnd(), WM_SOCKET_NOTIFY, 0);
				m_hSocket = INVALID_SOCKET;
			}

			//恢复数据
			m_cbSendRound = 0;
			m_cbRecvRound = 0;
			m_dwSendXorKey = 0;
			m_dwRecvXorKey = 0;
			m_hSocket = INVALID_SOCKET;
			m_dwSendTickCount = 0;
			m_dwRecvTickCount = 0;
			m_dwSendPacketCount = 0;
			m_dwRecvPacketCount = 0;
			m_cbSocketStatus = SOCKET_STATUS_IDLE;
			m_wRecvSize = 0;
			m_dwServerIP = INADDR_NONE;
			m_wPort = 0;

			break;
		}

		case QUEUE_DETECT_SOCKET:
		{

		}

		default:
		{
			ASSERT(FALSE);
			return;
		}
	}
}

//发送函数
bool CTCPSocketService::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	if (m_hSocket == INVALID_SOCKET) return false;
//	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//效验大小
	ASSERT(wDataSize <= SOCKET_TCP_BUFFER);
	if (wDataSize > SOCKET_TCP_BUFFER) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0)
	{
		ASSERT(pData != NULL);
		CopyMemory(pHead + 1, pData, wDataSize);
	}

	//加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize, sizeof(cbDataBuffer));

	//发送数据
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//发送数据
bool CTCPSocketService::SendDataBuffer(void * pBuffer, WORD wSendSize)
{
	//效验参数
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);
	if (NULL == pBuffer || 0 == wSendSize) return false;

	return m_QueueService.AddToQueue(QUEUE_SEND_REQUEST, pBuffer, wSendSize);
}

//关闭连接
VOID CTCPSocketService::CloseSocket(BYTE cbShutReason)
{
	//关闭连接
	m_QueueServiceEvent.PostTCPSocketShutEvent(m_wServiceID, cbShutReason);
	CloseSocket();
}

//解释服务器地址
DWORD CTCPSocketService::TranslateAddr(LPCTSTR pszServerAddr)
{
	//转化地址
	DWORD dwServerIP = inet_addr((LPCSTR)CT2A(pszServerAddr));
	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname((LPCSTR)CT2A(pszServerAddr));
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}

//加密数据
WORD CTCPSocketService::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	int i = 0;
	//效验参数
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(wBufferSize >= (wDataSize + 2*sizeof(DWORD)));
	ASSERT(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_BUFFER));

	//调整长度
	WORD wEncryptSize = wDataSize - sizeof(TCP_Command), wSnapCount = 0;
	if ((wEncryptSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wEncryptSize % sizeof(DWORD);
		memset(pcbDataBuffer + sizeof(TCP_Info) + wEncryptSize, 0, wSnapCount);
	}

	//效验码与字节映射
	BYTE cbCheckCode = 0;
	for (WORD i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		cbCheckCode += pcbDataBuffer[i];
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
	}

	//填写信息头
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.cbCheckCode = ~cbCheckCode + 1;
	pHead->TCPInfo.wPacketSize = wDataSize;
	//pHead->TCPInfo.cbVersion = SOCKET_TCP_VER;
	pHead->TCPInfo.cbDataKind = SOCKET_TCP_VER;
	//创建密钥
	DWORD dwXorKey = m_dwSendXorKey;
	if (m_dwSendPacketCount == 0)
	{
		//生成第一次随机种子
		GUID Guid;
		CoCreateGuid(&Guid);
		dwXorKey = GetTickCount() * GetTickCount();
		dwXorKey ^= Guid.Data1;
		dwXorKey ^= Guid.Data2;
		dwXorKey ^= Guid.Data3;
		dwXorKey ^= *((DWORD *)Guid.Data4);

		//随机映射种子
		dwXorKey = SeedRandMap((WORD)dwXorKey);
		dwXorKey |= ((DWORD)SeedRandMap((WORD)(dwXorKey >> 16))) << 16;
		dwXorKey ^= g_dwPacketKey;
		m_dwSendXorKey = dwXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//加密数据
	WORD * pwSeed = (WORD *)(pcbDataBuffer + sizeof(TCP_Info));
	DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(TCP_Info));
	WORD wEncrypCount = (wEncryptSize + wSnapCount) / sizeof(DWORD);
	for (WORD i = 0; i < wEncrypCount; i++)
	{
		*pdwXor++ ^= dwXorKey;
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
	}

	//插入密钥
	if (m_dwSendPacketCount == 0)
	{
		MoveMemory(pcbDataBuffer + sizeof(TCP_Head) + sizeof(DWORD), pcbDataBuffer + sizeof(TCP_Head), wDataSize);
		*((DWORD *)(pcbDataBuffer + sizeof(TCP_Head))) = m_dwSendXorKey;
		pHead->TCPInfo.wPacketSize += sizeof(DWORD);
		wDataSize += sizeof(DWORD);
	}

	//设置变量
	m_dwSendPacketCount++;
	m_dwSendXorKey = dwXorKey;

	return wDataSize;
}

//解密数据
WORD CTCPSocketService::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	//效验参数
	ASSERT(m_dwSendPacketCount > 0);
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	//调整长度
	WORD wSnapCount = 0;
	if ((wDataSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wDataSize % sizeof(DWORD);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	//解密数据
	DWORD dwXorKey = m_dwRecvXorKey;
	DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(TCP_Info));
	WORD  * pwSeed = (WORD *)(pcbDataBuffer + sizeof(TCP_Info));
	WORD wEncrypCount = (wDataSize + wSnapCount - sizeof(TCP_Info)) / 4;
	for (WORD i = 0; i < wEncrypCount; i++)
	{
		if ((i == (wEncrypCount - 1)) && (wSnapCount > 0))
		{
			BYTE * pcbKey = ((BYTE *) & m_dwRecvXorKey) + sizeof(DWORD) - wSnapCount;
			CopyMemory(pcbDataBuffer + wDataSize, pcbKey, wSnapCount);
		}
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
		*pdwXor++ ^= m_dwRecvXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//效验码与字节映射
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	BYTE cbCheckCode = pHead->TCPInfo.cbCheckCode;
	for (int i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode += pcbDataBuffer[i];
	}
	if (cbCheckCode != 0) throw TEXT("数据包效验码错误");

	return wDataSize;
}

//随机映射
WORD CTCPSocketService::SeedRandMap(WORD wSeed)
{
	DWORD dwHold = wSeed;
	return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//映射发送数据
BYTE CTCPSocketService::MapSendByte(BYTE const cbData)
{
	BYTE cbMap = g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}

//映射接收数据
BYTE CTCPSocketService::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}

//网络连接
LRESULT CTCPSocketService::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//判断状态
	int iErrorCode = WSAGETSELECTERROR(lParam);
	if (iErrorCode == 0)
	{
		m_cbSocketStatus = SOCKET_STATUS_CONNECT;
		m_bSuccess = true;
		//发送通知
		m_QueueServiceEvent.PostTCPSocketLinkEvent(m_wServiceID, iErrorCode);
	}
	else CloseSocket(SHUT_REASON_TIME_OUT);

	return 1;
}

//网络读取
LRESULT CTCPSocketService::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//读取数据
		int iRetCode = recv(m_hSocket, (char *)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) throw TEXT("网络连接关闭，读取数据失败");
		ASSERT(m_dwSendPacketCount > 0);
		m_wRecvSize += iRetCode;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		//变量定义
		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_TCP_BUFFER+sizeof(TCP_Head)];
		TCP_Head * pHead = (TCP_Head *)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(TCP_Head))
		{
			//效验参数
			wPacketSize = pHead->TCPInfo.wPacketSize;
			ASSERT(pHead->TCPInfo.cbDataKind == SOCKET_TCP_VER);
			//ASSERT(pHead->TCPInfo.cbVersion == SOCKET_TCP_VER);
			ASSERT(wPacketSize <= (SOCKET_TCP_BUFFER + sizeof(TCP_Head)));
			//if (pHead->TCPInfo.cbVersion != SOCKET_TCP_VER) throw TEXT("数据包版本错误");
			if (pHead->TCPInfo.cbDataKind != SOCKET_TCP_VER) throw TEXT("数据包版本错误");
			if (wPacketSize > (SOCKET_TCP_BUFFER + sizeof(TCP_Head))) throw TEXT("数据包太大");
			if (m_wRecvSize < wPacketSize) return 1;

			//拷贝数据
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer, m_cbRecvBuf, wPacketSize);
			m_wRecvSize -= wPacketSize;
			MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);

			//解密数据
			WORD wRealySize = CrevasseBuffer(cbDataBuffer, wPacketSize);
			ASSERT(wRealySize >= sizeof(TCP_Head));

			//解释数据
			WORD wDataSize = wRealySize - sizeof(TCP_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
			TCP_Command Command = ((TCP_Head *)cbDataBuffer)->CommandInfo;

			//内核命令
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
					case SUB_KN_DETECT_SOCKET:	//网络检测
					{
						//发送数据
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
						break;
					}
				}
				continue;
			}

			//处理数据
			bool bSuccess = m_QueueServiceEvent.PostTCPSocketReadEvent(m_wServiceID, Command, pDataBuffer, wDataSize);
			if (bSuccess == false) throw TEXT("网络数据包处理失败");
		};
	}
	catch (...)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
	}

	return 1;
}

//网络关闭
LRESULT CTCPSocketService::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	CloseSocket(SHUT_REASON_REMOTE);
	return 1;
}

//SOCKET 消息处理程序
LRESULT	CTCPSocketService::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_CONNECT:	//网络连接
		{
			return OnSocketNotifyConnect(wParam, lParam);
		}
		case FD_READ:		//数据读取
		{
			return OnSocketNotifyRead(wParam, lParam);
		}
		case FD_CLOSE:		//网络关闭
		{
			return OnSocketNotifyClose(wParam, lParam);
		}
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////////
//建立对象函数
extern "C" __declspec(dllexport) void * CreateTCPSocketService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//建立对象
	CTCPSocketService * pTCPSocketService = NULL;
	try
	{
		pTCPSocketService = new CTCPSocketService();
		if (pTCPSocketService == NULL) throw TEXT("创建失败");
		void * pObject = pTCPSocketService->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pTCPSocketService);
	return NULL;
}