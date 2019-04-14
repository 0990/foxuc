#ifndef SOCKET_ENGINE_HEAD_FILE
#define SOCKET_ENGINE_HEAD_FILE

//���ͷ�ļ�

#include "KernelEngineHead.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"
#include "zlreactor/ByteBuffer.h"
#include "zlreactor/net/http/HttpContext.h"
#include "zlreactor/WebSocket.h"
//#include "json/json.h"
#include <conio.h> 


//�ر�ԭ��
#define SHUT_REASON_INSIDE			0									//�ڲ�ԭ��
#define SHUT_REASON_NORMAL			1									//�����ر�
#define SHUT_REASON_REMOTE			2									//Զ�̹ر�
#define SHUT_REASON_TIME_OUT		3									//���糬ʱ
#define SHUT_REASON_EXCEPTION		4									//�쳣�ر�

//////////////////////////////////////////////////////////////////////////
//��������

//����״̬
#define SOCKET_STATUS_IDLE			0									//����״̬
#define SOCKET_STATUS_WAIT			1									//�ȴ�״̬
#define SOCKET_STATUS_CONNECT		2									//����״̬

//���Ӵ���
#define CONNECT_SUCCESS				0									//���ӳɹ�
#define CONNECT_FAILURE				1									//����ʧ��
#define CONNECT_EXCEPTION			2									//�����쳣
#define CONNECT_PROXY_FAILURE		3									//���ӳɹ�
#define CONNECT_PROXY_USER_INVALID	4									//�û���Ч

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��������
enum enOperationType
{
	OperationType_Send,				//��������
	OperationType_Recv,				//��������
};

//////////////////////////////////////////////////////////////////////////
//��˵��

class COverLapped;
class CServerSocketRSThread;
class COverLappedSend;
class CTCPNetworkEngine;
class CServerSocketItem;
class CSocketAcceptThread;
template <enOperationType OperationType> class CATLOverLapped;

typedef class CATLOverLapped<OperationType_Recv> COverLappedRecv;
typedef CWHArray<COverLappedSend *> COverLappedSendPtr;
typedef CWHArray<COverLappedRecv *> COverLappedRecvPtr;


//////////////////////////////////////////////////////////////////////////
//�ӿڶ���

//���Ӷ���ص��ӿ�
interface IServerSocketItemSink
{
	//Ӧ����Ϣ
	virtual bool OnSocketAcceptEvent(CServerSocketItem * pServerSocketItem) = NULL;
	//��ȡ��Ϣ
	virtual bool OnSocketReadEvent(TCP_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem) = NULL;
	//�ر���Ϣ
	virtual bool OnSocketCloseEvent(CServerSocketItem * pServerSocketItem) = NULL;
	//����̨��Ϣ
	virtual bool OnSocketForegroundEvent(CServerSocketItem* pServerSocketItem) = NULL;
};

//////////////////////////////////////////////////////////////////////////

//�ص��ṹ��
class COverLapped
{
	//��������
public:
	WSABUF							m_WSABuffer;						//����ָ��
	OVERLAPPED						m_OverLapped;						//�ص��ṹ
	const enOperationType			m_OperationType;					//��������

	//��������
public:
	//���캯��
	COverLapped(enOperationType OperationType);
	//��������
	virtual ~COverLapped();

	//��Ϣ����
public:
	//��ȡ����
	enOperationType GetOperationType()
	{
		return m_OperationType;
	}
};

//�����ص��ṹ
class COverLappedSend : public COverLapped
{
	//���ݱ���
public:
	BYTE							m_cbBuffer[SOCKET_TCP_BUFFER];			//���ݻ���

	//��������
public:
	//���캯��
	COverLappedSend();
	//��������
	virtual ~COverLappedSend();
};

//�ص��ṹģ��
template <enOperationType OperationType> class CATLOverLapped : public COverLapped
{
	//��������
public:
	//���캯��
	CATLOverLapped() : COverLapped(OperationType) {}
	//��������
	virtual ~CATLOverLapped() {}
};

//////////////////////////////////////////////////////////////////////////

//TCP SOCKET ��
class CServerSocketItem
{
	enum ConnectType{
		CHECKING,
		HANDSHAKING,
		WINSOCKET,
		WEBSOCKET
	};
	//��������
protected:
	DWORD							m_dwClientAddr;						//���ӵ�ַ
	DWORD							m_dwConnectTime;					//����ʱ��

	std::string                     m_strAesKey;
	//���ı���
protected:
	WORD							m_wRountID;							//ѭ������
	SOCKET							m_hSocket;							//SOCKET ���
public:
	ConnectType                     m_connectType;
	//״̬����
protected:
	bool							m_bNotify;							//֪ͨ��־
	bool							m_bRecvIng;							//���ձ�־
	bool							m_bCloseIng;						//�رձ�־
	bool							m_bAllowBatch;						//����Ⱥ��
	bool                            m_bForeground;                      //�ֻ���ǰ̨��־

	WORD							m_wRecvSize;						//���ճ���
	BYTE							m_cbRecvBuf[SOCKET_TCP_BUFFER*5];	//���ջ���
	//��������
protected:
	DWORD							m_dwSendTickCount;					//����ʱ��
	DWORD							m_dwRecvTickCount;					//����ʱ��
	DWORD							m_dwSendPacketCount;				//���ͼ���
	DWORD							m_dwRecvPacketCount;				//���ܼ���
	DWORD                           m_dwKNRecvTickCount;

	//��������
protected:
	BYTE							m_cbSendRound;						//�ֽ�ӳ��
	BYTE							m_cbRecvRound;						//�ֽ�ӳ��
	DWORD							m_dwSendXorKey;						//������Կ
	DWORD							m_dwRecvXorKey;						//������Կ

	//�ڲ�����
protected:
	const WORD						m_wIndex;							//��������
	CCriticalSection				m_CriticalSection;					//ͬ������
	COverLappedRecv					m_OverLappedRecv;					//�ص��ṹ
	COverLappedSendPtr				m_OverLappedSendFree;				//�ص��ṹ
	COverLappedSendPtr				m_OverLappedSendActive;				//�ص��ṹ
	IServerSocketItemSink			* m_pIServerSocketItemSink;			//�ص��ӿ�

	//��������
public:
	//���캯��
	CServerSocketItem(WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink);
	//��������
	virtual ~CServerSocketItem(void);

	//��ʶ����
public:
	//��ȡ����
	WORD GetIndex()
	{
		return m_wIndex;
	}
	//��ȡ����
	WORD GetRountID()
	{
		return m_wRountID;
	}
	//��ȡ��ʶ
	DWORD GetSocketID()
	{
		return MAKELONG(m_wIndex, m_wRountID);
	}

	//��������
public:
	//��ȡ��ַ
	DWORD GetClientAddr()
	{
		return m_dwClientAddr;
	}
	//����ʱ��
	DWORD GetConnectTime()
	{
		return m_dwConnectTime;
	}
	//����ʱ��
	DWORD GetSendTickCount()
	{
		return m_dwSendTickCount;
	}
	//����ʱ��
	DWORD GetRecvTickCount()
	{
		return m_dwRecvTickCount;
	}
	DWORD GetKNRecvTickCount(){
		return m_dwKNRecvTickCount;
	}
	//��������
	CCriticalSection& GetSignedLock()
	{
		return m_CriticalSection;
	}
	//��׼����
	bool IsReadySend()
	{
		return m_dwRecvPacketCount > 0L;
	}
	//�Ƿ�Ⱥ��
	bool IsAllowBatch()
	{
		return m_bAllowBatch;
	}
	bool isForeground(){
		return m_bForeground;
	}
	//�ж�����
	bool IsValidSocket()
	{
		return (m_hSocket != INVALID_SOCKET);
	}

	//���ܺ���
public:
	//�󶨶���
	DWORD Attach(SOCKET hSocket, DWORD dwClientAddr);
	//���ͺ���
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//���ͺ���
	bool SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//���ղ���
	bool RecvData();
	//�ر�����
	bool CloseSocket(WORD wRountID);
	//���ùر�
	bool ShutDownSocket(WORD wRountID);
	//����Ⱥ��
	bool AllowBatchSend(WORD wRountID, bool bAllowBatch);
	//���ñ���
	void ResetSocketData();

	//֪ͨ�ӿ�
public:
	//�������֪ͨ
	bool OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred);
	//�������֪ͨ
	bool OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred);
	//�ر����֪ͨ
	bool OnCloseCompleted();

	//�ڲ�����
private:
	//��������
	WORD EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize);
	//��������
	WORD CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize);

	//��������
private:
	//���ӳ��
	inline WORD SeedRandMap(WORD wSeed);
	//ӳ�䷢������
	inline BYTE MapSendByte(BYTE const cbData);
	//ӳ���������
	inline BYTE MapRecvByte(BYTE const cbData);
	//��ȡ���ͽṹ
	inline COverLappedSend * GetSendOverLapped();
private:
	//return true mean websocket,false not
	bool handshake(zl::net::ByteBuffer* byteBuffer);
	bool SendRawData(const char* data,int len);
	bool CheckIfWinSocket();
	bool HandleWebsocketRecv();
public:
	bool isWebSocket();
	bool isWinSocket();
	bool sendPingFrame();
	bool setForeground(bool foreground);
};


//////////////////////////////////////////////////////////////////////////

//��д�߳���
class CServerSocketRSThread : public CWHThread
{
	//��������
protected:
	HANDLE							m_hCompletionPort;					//��ɶ˿�

	//��������
public:
	//���캯��
	CServerSocketRSThread(void);
	//��������
	virtual ~CServerSocketRSThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(HANDLE hCompletionPort);

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//Ӧ���̶߳���
class CSocketAcceptThread : public CWHThread
{
	//��������
protected:
	SOCKET							m_hListenSocket;					//��������
	HANDLE							m_hCompletionPort;					//��ɶ˿�
	CTCPNetworkEngine				* m_pTCPSocketManager;				//����ָ��

	//��������
public:
	//���캯��
	CSocketAcceptThread(void);
	//��������
	virtual ~CSocketAcceptThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPNetworkEngine * pTCPSocketManager);

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//����߳���
class CSocketDetectThread : public CWHThread
{
	//��������
protected:
	DWORD							m_dwTickCount;						//����ʱ��
	CTCPNetworkEngine				* m_pTCPSocketManager;				//����ָ��

	//��������
public:
	//���캯��
	CSocketDetectThread(void);
	//��������
	virtual ~CSocketDetectThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(CTCPNetworkEngine * pTCPSocketManager);

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//��˵��
typedef CWHArray<CServerSocketItem *> CServerSocketItemPtrArray;
typedef CWHArray<CServerSocketRSThread *> CServerSocketRSThreadPtrArray;

//////////////////////////////////////////////////////////////////////////
//��������
class CTCPNetworkEngine : public ITCPNetworkEngine, public IServerSocketItemSink, public IQueueServiceSink
{
	friend class CServerSocketRSThread;
	friend class CSocketAcceptThread;

	//��������
protected:
	CCriticalSection				m_CriticalSection;
	CServerSocketItemPtrArray		m_FreeSocketItem;					//��������
	CServerSocketItemPtrArray		m_ActiveSocketItem;					//�����
	CServerSocketItemPtrArray		m_StorageSocketItem;				//�洢����

	//��������
protected:
	DWORD							m_dwLastDetect;						//���ʱ��
	CServerSocketItemPtrArray		m_TempSocketItem;					//��ʱ����

	//���ñ���
protected:
	WORD							m_wListenPort;						//�����˿�
	WORD							m_wMaxSocketItem;					//�������

protected:
	CQueueService					m_SendQueueService;					//���ж���
	CQueueServiceEvent				m_QueueServiceEvent;				//֪ͨ���

	//�ں˱���
protected:
	bool							m_bService;							//�����־
	SOCKET							m_hServerSocket;					//���Ӿ��
	HANDLE							m_hCompletionPort;					//��ɶ˿�
	CSocketDetectThread				m_SocketDetectThread;				//����߳�
	CSocketAcceptThread				m_SocketAcceptThread;				//Ӧ���߳�
	CServerSocketRSThreadPtrArray	m_SocketRSThreadArray;				//��д�߳�

	//��������
public:
	//���캯��
	CTCPNetworkEngine(void);
	//��������
	virtual ~CTCPNetworkEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release()
	{
		if (IsValid()) delete this;
	}
	//�Ƿ���Ч
	virtual bool IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CTCPNetworkEngine)) ? true : false;
	}
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool StartService();
	//ֹͣ����
	virtual bool ConcludeService();

	//��Ϣ�ӿ�
public:
	//���ö˿�
	virtual WORD GetServicePort();
	//��ǰ�˿�
	virtual WORD GetCurrentPort();

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetTCPNetworkEngineEvent(IUnknownEx * pIUnknownEx);
	//���ò���
	virtual bool SetServiceParameter(WORD wServicePort, WORD wMaxConnect, LPCTSTR  pszCompilation);

	//���нӿ�
public:
	//֪ͨ�ص�����
	virtual void OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize);

	//����ӿ�
public:
	//�������
	virtual bool DetectSocket();
	//���ͺ���
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//��������
	virtual bool SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize, BYTE cbBatchMask);
	//�ر�����
	virtual bool CloseSocket(DWORD dwSocketID);
	//���ùر�
	virtual bool ShutDownSocket(DWORD dwSocketID);
	//����Ⱥ��
	virtual bool AllowBatchSend(DWORD dwSocketID, bool bAllow, BYTE cbBatchMask);

	//֪ͨ�ӿ�
public:
	//Ӧ����Ϣ
	virtual bool OnSocketAcceptEvent(CServerSocketItem * pServerSocketItem);
	//��ȡ��Ϣ
	virtual bool OnSocketReadEvent(TCP_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem);
	//�ر���Ϣ
	virtual bool OnSocketCloseEvent(CServerSocketItem * pServerSocketItem);

	virtual bool OnSocketForegroundEvent(CServerSocketItem* pServerSocketItem);

	//�ڲ�����
protected:
	//������ж���
	CServerSocketItem * ActiveSocketItem();
	//��ȡ����
	CServerSocketItem * EnumSocketItem(WORD wIndex);
	//�ͷ����Ӷ���
	bool FreeSocketItem(CServerSocketItem * pServerSocketItem);

};


#endif