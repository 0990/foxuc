#include "StdAfx.h"
#include "TraceService.h"
#include "TCPSocketService.h"


//�궨��
#define ID_SOCKET_WND				10								//SOCKET ���� ID
#define WM_SOCKET_NOTIFY			WM_USER+10						//SOCKET ��Ϣ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTCPSocketService, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY, OnSocketNotify)
END_MESSAGE_MAP()

//��������
//////////////////////////////////////////////////////////////////////////
//���캯��
CTCPSocketService::CTCPSocketService(void)
{
	//�ָ�����
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

//��������
CTCPSocketService::~CTCPSocketService(void)
{
	if (m_bService)
		ConcludeService();
}

//�����ӿ�
//////////////////////////////////////////////////////////////////////////
//�ӿڲ�ѯ
void * CTCPSocketService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketService, Guid, dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocketService, Guid, dwQueryVer);
	return NULL;
}

//����ӿ�
//////////////////////////////////////////////////////////////////////////
//��������
bool CTCPSocketService::StartService()
{
	//Ч��״̬
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("��������ظ�������������������"), TraceLevel_Warning);
		return true;
	}

	//�󶨶���
	if (m_QueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		CTraceService::TraceString(TEXT("��������봥�������ʧ��"), TraceLevel_Exception);
		return false;
	}

	try
	{
		//�󶨶���
		if (m_QueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
		{
			CTraceService::TraceString(TEXT("���������봥�������ʧ��"), TraceLevel_Exception);
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

//ֹͣ����
bool CTCPSocketService::ConcludeService()
{
	//���ñ���
	m_bService = false;

	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		WSAAsyncSelect(m_hSocket, GetSafeHwnd(), WM_SOCKET_NOTIFY, 0);
		m_hSocket = INVALID_SOCKET;
	}

	//�ָ�����
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

//���ýӿ�
//////////////////////////////////////////////////////////////////////////
//���ú���
bool CTCPSocketService::SetServiceID(WORD wServiceID)
{
	m_wServiceID = wServiceID;
	return true;
}

//���ýӿ�
bool CTCPSocketService::SetTCPSocketEvent(IUnknownEx * pIUnknownEx)
{
	//״̬�ж�
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("����������ڷ���״̬���󶨲�������"), TraceLevel_Warning);
		return false;
	}

	//���ýӿ�
	//if (m_QueueServiceEvent.SetQueueServiceSink(pIUnknownEx) == false)
	//{
	//	CTraceService::TraceString(TEXT("��������봥�������ʧ��"), TraceLevel_Warning);
	//	return false;
	//}

	IAttemperEngine* pIAttemperSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngine);
	ASSERT(pIAttemperSink != NULL);
	m_QueueServiceEvent.SetQueueService((CQueueService*)pIAttemperSink->GetQueue());

	return true;
}

//���ܽӿ�
//////////////////////////////////////////////////////////////////////////
//�ر�����
bool CTCPSocketService::CloseSocket()
{
	//�ر�����
	bool bClose = (m_hSocket != INVALID_SOCKET);
	if (bClose)
		return m_QueueService.AddToQueue(QUEUE_SAFE_CLOSE, NULL, 0);

	//�ָ�����
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

//���ӵ�ַ
bool CTCPSocketService::Connect(DWORD dwServerIP, WORD wPort)
{
	if (m_bSuccess && ntohl(dwServerIP) == m_dwTempServerIP)//��¼��������BUG -- 
		m_dwServerIP = ntohl(dwServerIP);
	else m_dwServerIP = dwServerIP;

	m_wPort = wPort;
	m_dwTempServerIP = m_dwServerIP;

	//���ò���
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
		throw TEXT("���Ӳ���δ֪�쳣����");
	}

	return false;
}

//���ӵ�ַ
bool CTCPSocketService::Connect(LPCTSTR szServerIP, WORD wPort)
{
	//Ч������
	ASSERT(wPort != 0);
	ASSERT(szServerIP != NULL);
	if ((szServerIP == NULL) || (wPort == 0)) return false;
	return Connect(TranslateAddr(szServerIP), wPort);
}

//���ͺ���
bool CTCPSocketService::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч��״̬
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	//��������
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head), sizeof(cbDataBuffer));

	//��������
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//֪ͨ�ص�
void CTCPSocketService::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)
{
	switch (wIdentifier)
	{
		case QUEUE_SEND_REQUEST:
		{
			ASSERT(pBuffer);
			ASSERT(wDataSize > 0);
			if (!pBuffer || wDataSize <= 0) return;

			//��������
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
			//Ч��״̬
			if (m_dwServerIP == INADDR_NONE) throw TEXT("Ŀ���������ַ��ʽ����ȷ��������ٴγ��ԣ�");
			if (m_wPort == 0) throw TEXT("Ŀ���������ַ��ʽ����ȷ��������ٴγ��ԣ�");

			//���� SOCKET
			m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_hSocket == INVALID_SOCKET) throw TEXT("SOCKET ����ʧ��");

			//Ч�����
			ASSERT(m_hSocket != INVALID_SOCKET);
			ASSERT(m_cbSocketStatus == SOCKET_STATUS_IDLE);

			int iErrorCode = WSAAsyncSelect(m_hSocket, m_hWnd, WM_SOCKET_NOTIFY, FD_READ | FD_CONNECT | FD_CLOSE);
			if (iErrorCode == SOCKET_ERROR) throw TEXT("���ڲ����ڴ���");

			//��д��������ַ
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

			//�ָ�����
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

//���ͺ���
bool CTCPSocketService::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_hSocket == INVALID_SOCKET) return false;
//	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//Ч���С
	ASSERT(wDataSize <= SOCKET_TCP_BUFFER);
	if (wDataSize > SOCKET_TCP_BUFFER) return false;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0)
	{
		ASSERT(pData != NULL);
		CopyMemory(pHead + 1, pData, wDataSize);
	}

	//��������
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize, sizeof(cbDataBuffer));

	//��������
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//��������
bool CTCPSocketService::SendDataBuffer(void * pBuffer, WORD wSendSize)
{
	//Ч�����
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);
	if (NULL == pBuffer || 0 == wSendSize) return false;

	return m_QueueService.AddToQueue(QUEUE_SEND_REQUEST, pBuffer, wSendSize);
}

//�ر�����
VOID CTCPSocketService::CloseSocket(BYTE cbShutReason)
{
	//�ر�����
	m_QueueServiceEvent.PostTCPSocketShutEvent(m_wServiceID, cbShutReason);
	CloseSocket();
}

//���ͷ�������ַ
DWORD CTCPSocketService::TranslateAddr(LPCTSTR pszServerAddr)
{
	//ת����ַ
	DWORD dwServerIP = inet_addr((LPCSTR)CT2A(pszServerAddr));
	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname((LPCSTR)CT2A(pszServerAddr));
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}

//��������
WORD CTCPSocketService::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	int i = 0;
	//Ч�����
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(wBufferSize >= (wDataSize + 2*sizeof(DWORD)));
	ASSERT(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_BUFFER));

	//��������
	WORD wEncryptSize = wDataSize - sizeof(TCP_Command), wSnapCount = 0;
	if ((wEncryptSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wEncryptSize % sizeof(DWORD);
		memset(pcbDataBuffer + sizeof(TCP_Info) + wEncryptSize, 0, wSnapCount);
	}

	//Ч�������ֽ�ӳ��
	BYTE cbCheckCode = 0;
	for (WORD i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		cbCheckCode += pcbDataBuffer[i];
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
	}

	//��д��Ϣͷ
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.cbCheckCode = ~cbCheckCode + 1;
	pHead->TCPInfo.wPacketSize = wDataSize;
	//pHead->TCPInfo.cbVersion = SOCKET_TCP_VER;
	pHead->TCPInfo.cbDataKind = SOCKET_TCP_VER;
	//������Կ
	DWORD dwXorKey = m_dwSendXorKey;
	if (m_dwSendPacketCount == 0)
	{
		//���ɵ�һ���������
		GUID Guid;
		CoCreateGuid(&Guid);
		dwXorKey = GetTickCount() * GetTickCount();
		dwXorKey ^= Guid.Data1;
		dwXorKey ^= Guid.Data2;
		dwXorKey ^= Guid.Data3;
		dwXorKey ^= *((DWORD *)Guid.Data4);

		//���ӳ������
		dwXorKey = SeedRandMap((WORD)dwXorKey);
		dwXorKey |= ((DWORD)SeedRandMap((WORD)(dwXorKey >> 16))) << 16;
		dwXorKey ^= g_dwPacketKey;
		m_dwSendXorKey = dwXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//��������
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

	//������Կ
	if (m_dwSendPacketCount == 0)
	{
		MoveMemory(pcbDataBuffer + sizeof(TCP_Head) + sizeof(DWORD), pcbDataBuffer + sizeof(TCP_Head), wDataSize);
		*((DWORD *)(pcbDataBuffer + sizeof(TCP_Head))) = m_dwSendXorKey;
		pHead->TCPInfo.wPacketSize += sizeof(DWORD);
		wDataSize += sizeof(DWORD);
	}

	//���ñ���
	m_dwSendPacketCount++;
	m_dwSendXorKey = dwXorKey;

	return wDataSize;
}

//��������
WORD CTCPSocketService::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	//Ч�����
	ASSERT(m_dwSendPacketCount > 0);
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	//��������
	WORD wSnapCount = 0;
	if ((wDataSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wDataSize % sizeof(DWORD);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	//��������
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

	//Ч�������ֽ�ӳ��
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	BYTE cbCheckCode = pHead->TCPInfo.cbCheckCode;
	for (int i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode += pcbDataBuffer[i];
	}
	if (cbCheckCode != 0) throw TEXT("���ݰ�Ч�������");

	return wDataSize;
}

//���ӳ��
WORD CTCPSocketService::SeedRandMap(WORD wSeed)
{
	DWORD dwHold = wSeed;
	return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//ӳ�䷢������
BYTE CTCPSocketService::MapSendByte(BYTE const cbData)
{
	BYTE cbMap = g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}

//ӳ���������
BYTE CTCPSocketService::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}

//��������
LRESULT CTCPSocketService::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//�ж�״̬
	int iErrorCode = WSAGETSELECTERROR(lParam);
	if (iErrorCode == 0)
	{
		m_cbSocketStatus = SOCKET_STATUS_CONNECT;
		m_bSuccess = true;
		//����֪ͨ
		m_QueueServiceEvent.PostTCPSocketLinkEvent(m_wServiceID, iErrorCode);
	}
	else CloseSocket(SHUT_REASON_TIME_OUT);

	return 1;
}

//�����ȡ
LRESULT CTCPSocketService::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//��ȡ����
		int iRetCode = recv(m_hSocket, (char *)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) throw TEXT("�������ӹرգ���ȡ����ʧ��");
		ASSERT(m_dwSendPacketCount > 0);
		m_wRecvSize += iRetCode;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		//��������
		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_TCP_BUFFER+sizeof(TCP_Head)];
		TCP_Head * pHead = (TCP_Head *)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(TCP_Head))
		{
			//Ч�����
			wPacketSize = pHead->TCPInfo.wPacketSize;
			ASSERT(pHead->TCPInfo.cbDataKind == SOCKET_TCP_VER);
			//ASSERT(pHead->TCPInfo.cbVersion == SOCKET_TCP_VER);
			ASSERT(wPacketSize <= (SOCKET_TCP_BUFFER + sizeof(TCP_Head)));
			//if (pHead->TCPInfo.cbVersion != SOCKET_TCP_VER) throw TEXT("���ݰ��汾����");
			if (pHead->TCPInfo.cbDataKind != SOCKET_TCP_VER) throw TEXT("���ݰ��汾����");
			if (wPacketSize > (SOCKET_TCP_BUFFER + sizeof(TCP_Head))) throw TEXT("���ݰ�̫��");
			if (m_wRecvSize < wPacketSize) return 1;

			//��������
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer, m_cbRecvBuf, wPacketSize);
			m_wRecvSize -= wPacketSize;
			MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);

			//��������
			WORD wRealySize = CrevasseBuffer(cbDataBuffer, wPacketSize);
			ASSERT(wRealySize >= sizeof(TCP_Head));

			//��������
			WORD wDataSize = wRealySize - sizeof(TCP_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
			TCP_Command Command = ((TCP_Head *)cbDataBuffer)->CommandInfo;

			//�ں�����
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
					case SUB_KN_DETECT_SOCKET:	//������
					{
						//��������
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
						break;
					}
				}
				continue;
			}

			//��������
			bool bSuccess = m_QueueServiceEvent.PostTCPSocketReadEvent(m_wServiceID, Command, pDataBuffer, wDataSize);
			if (bSuccess == false) throw TEXT("�������ݰ�����ʧ��");
		};
	}
	catch (...)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
	}

	return 1;
}

//����ر�
LRESULT CTCPSocketService::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	CloseSocket(SHUT_REASON_REMOTE);
	return 1;
}

//SOCKET ��Ϣ�������
LRESULT	CTCPSocketService::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_CONNECT:	//��������
		{
			return OnSocketNotifyConnect(wParam, lParam);
		}
		case FD_READ:		//���ݶ�ȡ
		{
			return OnSocketNotifyRead(wParam, lParam);
		}
		case FD_CLOSE:		//����ر�
		{
			return OnSocketNotifyClose(wParam, lParam);
		}
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////////
//����������
extern "C" __declspec(dllexport) void * CreateTCPSocketService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CTCPSocketService * pTCPSocketService = NULL;
	try
	{
		pTCPSocketService = new CTCPSocketService();
		if (pTCPSocketService == NULL) throw TEXT("����ʧ��");
		void * pObject = pTCPSocketService->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTCPSocketService);
	return NULL;
}