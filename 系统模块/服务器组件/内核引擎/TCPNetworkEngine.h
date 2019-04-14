#ifndef SOCKET_ENGINE_HEAD_FILE
#define SOCKET_ENGINE_HEAD_FILE

//组件头文件

#include "KernelEngineHead.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"
#include "zlreactor/ByteBuffer.h"
#include "zlreactor/net/http/HttpContext.h"
#include "zlreactor/WebSocket.h"
//#include "json/json.h"
#include <conio.h> 


//关闭原因
#define SHUT_REASON_INSIDE			0									//内部原因
#define SHUT_REASON_NORMAL			1									//正常关闭
#define SHUT_REASON_REMOTE			2									//远程关闭
#define SHUT_REASON_TIME_OUT		3									//网络超时
#define SHUT_REASON_EXCEPTION		4									//异常关闭

//////////////////////////////////////////////////////////////////////////
//公共定义

//网络状态
#define SOCKET_STATUS_IDLE			0									//空闲状态
#define SOCKET_STATUS_WAIT			1									//等待状态
#define SOCKET_STATUS_CONNECT		2									//连接状态

//连接错误
#define CONNECT_SUCCESS				0									//连接成功
#define CONNECT_FAILURE				1									//连接失败
#define CONNECT_EXCEPTION			2									//参数异常
#define CONNECT_PROXY_FAILURE		3									//连接成功
#define CONNECT_PROXY_USER_INVALID	4									//用户无效

//////////////////////////////////////////////////////////////////////////
//枚举定义

//控制类型
enum enOperationType
{
	OperationType_Send,				//发送类型
	OperationType_Recv,				//接收类型
};

//////////////////////////////////////////////////////////////////////////
//类说明

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
//接口定义

//连接对象回调接口
interface IServerSocketItemSink
{
	//应答消息
	virtual bool OnSocketAcceptEvent(CServerSocketItem * pServerSocketItem) = NULL;
	//读取消息
	virtual bool OnSocketReadEvent(TCP_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem) = NULL;
	//关闭消息
	virtual bool OnSocketCloseEvent(CServerSocketItem * pServerSocketItem) = NULL;
	//进后台消息
	virtual bool OnSocketForegroundEvent(CServerSocketItem* pServerSocketItem) = NULL;
};

//////////////////////////////////////////////////////////////////////////

//重叠结构类
class COverLapped
{
	//变量定义
public:
	WSABUF							m_WSABuffer;						//数据指针
	OVERLAPPED						m_OverLapped;						//重叠结构
	const enOperationType			m_OperationType;					//操作类型

	//函数定义
public:
	//构造函数
	COverLapped(enOperationType OperationType);
	//析构函数
	virtual ~COverLapped();

	//信息函数
public:
	//获取类型
	enOperationType GetOperationType()
	{
		return m_OperationType;
	}
};

//接收重叠结构
class COverLappedSend : public COverLapped
{
	//数据变量
public:
	BYTE							m_cbBuffer[SOCKET_TCP_BUFFER];			//数据缓冲

	//函数定义
public:
	//构造函数
	COverLappedSend();
	//析构函数
	virtual ~COverLappedSend();
};

//重叠结构模板
template <enOperationType OperationType> class CATLOverLapped : public COverLapped
{
	//函数定义
public:
	//构造函数
	CATLOverLapped() : COverLapped(OperationType) {}
	//析构函数
	virtual ~CATLOverLapped() {}
};

//////////////////////////////////////////////////////////////////////////

//TCP SOCKET 类
class CServerSocketItem
{
	enum ConnectType{
		CHECKING,
		HANDSHAKING,
		WINSOCKET,
		WEBSOCKET
	};
	//连接属性
protected:
	DWORD							m_dwClientAddr;						//连接地址
	DWORD							m_dwConnectTime;					//连接时间

	std::string                     m_strAesKey;
	//核心变量
protected:
	WORD							m_wRountID;							//循环计数
	SOCKET							m_hSocket;							//SOCKET 句柄
public:
	ConnectType                     m_connectType;
	//状态变量
protected:
	bool							m_bNotify;							//通知标志
	bool							m_bRecvIng;							//接收标志
	bool							m_bCloseIng;						//关闭标志
	bool							m_bAllowBatch;						//接受群发
	bool                            m_bForeground;                      //手机在前台标志

	WORD							m_wRecvSize;						//接收长度
	BYTE							m_cbRecvBuf[SOCKET_TCP_BUFFER*5];	//接收缓冲
	//计数变量
protected:
	DWORD							m_dwSendTickCount;					//发送时间
	DWORD							m_dwRecvTickCount;					//接受时间
	DWORD							m_dwSendPacketCount;				//发送计数
	DWORD							m_dwRecvPacketCount;				//接受计数
	DWORD                           m_dwKNRecvTickCount;

	//加密数据
protected:
	BYTE							m_cbSendRound;						//字节映射
	BYTE							m_cbRecvRound;						//字节映射
	DWORD							m_dwSendXorKey;						//发送密钥
	DWORD							m_dwRecvXorKey;						//接收密钥

	//内部变量
protected:
	const WORD						m_wIndex;							//连接索引
	CCriticalSection				m_CriticalSection;					//同步锁定
	COverLappedRecv					m_OverLappedRecv;					//重叠结构
	COverLappedSendPtr				m_OverLappedSendFree;				//重叠结构
	COverLappedSendPtr				m_OverLappedSendActive;				//重叠结构
	IServerSocketItemSink			* m_pIServerSocketItemSink;			//回调接口

	//函数定义
public:
	//构造函数
	CServerSocketItem(WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink);
	//析够函数
	virtual ~CServerSocketItem(void);

	//标识函数
public:
	//获取索引
	WORD GetIndex()
	{
		return m_wIndex;
	}
	//获取计数
	WORD GetRountID()
	{
		return m_wRountID;
	}
	//获取标识
	DWORD GetSocketID()
	{
		return MAKELONG(m_wIndex, m_wRountID);
	}

	//辅助函数
public:
	//获取地址
	DWORD GetClientAddr()
	{
		return m_dwClientAddr;
	}
	//连接时间
	DWORD GetConnectTime()
	{
		return m_dwConnectTime;
	}
	//发送时间
	DWORD GetSendTickCount()
	{
		return m_dwSendTickCount;
	}
	//接收时间
	DWORD GetRecvTickCount()
	{
		return m_dwRecvTickCount;
	}
	DWORD GetKNRecvTickCount(){
		return m_dwKNRecvTickCount;
	}
	//锁定对象
	CCriticalSection& GetSignedLock()
	{
		return m_CriticalSection;
	}
	//是准备好
	bool IsReadySend()
	{
		return m_dwRecvPacketCount > 0L;
	}
	//是否群发
	bool IsAllowBatch()
	{
		return m_bAllowBatch;
	}
	bool isForeground(){
		return m_bForeground;
	}
	//判断连接
	bool IsValidSocket()
	{
		return (m_hSocket != INVALID_SOCKET);
	}

	//功能函数
public:
	//绑定对象
	DWORD Attach(SOCKET hSocket, DWORD dwClientAddr);
	//发送函数
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//发送函数
	bool SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//接收操作
	bool RecvData();
	//关闭连接
	bool CloseSocket(WORD wRountID);
	//设置关闭
	bool ShutDownSocket(WORD wRountID);
	//允许群发
	bool AllowBatchSend(WORD wRountID, bool bAllowBatch);
	//重置变量
	void ResetSocketData();

	//通知接口
public:
	//发送完成通知
	bool OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred);
	//接收完成通知
	bool OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred);
	//关闭完成通知
	bool OnCloseCompleted();

	//内部函数
private:
	//加密数据
	WORD EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize);
	//解密数据
	WORD CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize);

	//内联函数
private:
	//随机映射
	inline WORD SeedRandMap(WORD wSeed);
	//映射发送数据
	inline BYTE MapSendByte(BYTE const cbData);
	//映射接收数据
	inline BYTE MapRecvByte(BYTE const cbData);
	//获取发送结构
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

//读写线程类
class CServerSocketRSThread : public CWHThread
{
	//变量定义
protected:
	HANDLE							m_hCompletionPort;					//完成端口

	//函数定义
public:
	//构造函数
	CServerSocketRSThread(void);
	//析构函数
	virtual ~CServerSocketRSThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(HANDLE hCompletionPort);

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//应答线程对象
class CSocketAcceptThread : public CWHThread
{
	//变量定义
protected:
	SOCKET							m_hListenSocket;					//监听连接
	HANDLE							m_hCompletionPort;					//完成端口
	CTCPNetworkEngine				* m_pTCPSocketManager;				//管理指针

	//函数定义
public:
	//构造函数
	CSocketAcceptThread(void);
	//析构函数
	virtual ~CSocketAcceptThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPNetworkEngine * pTCPSocketManager);

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//检测线程类
class CSocketDetectThread : public CWHThread
{
	//变量定义
protected:
	DWORD							m_dwTickCount;						//积累时间
	CTCPNetworkEngine				* m_pTCPSocketManager;				//管理指针

	//函数定义
public:
	//构造函数
	CSocketDetectThread(void);
	//析构函数
	virtual ~CSocketDetectThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(CTCPNetworkEngine * pTCPSocketManager);

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//类说明
typedef CWHArray<CServerSocketItem *> CServerSocketItemPtrArray;
typedef CWHArray<CServerSocketRSThread *> CServerSocketRSThreadPtrArray;

//////////////////////////////////////////////////////////////////////////
//网络引擎
class CTCPNetworkEngine : public ITCPNetworkEngine, public IServerSocketItemSink, public IQueueServiceSink
{
	friend class CServerSocketRSThread;
	friend class CSocketAcceptThread;

	//变量定义
protected:
	CCriticalSection				m_CriticalSection;
	CServerSocketItemPtrArray		m_FreeSocketItem;					//空闲连接
	CServerSocketItemPtrArray		m_ActiveSocketItem;					//活动连接
	CServerSocketItemPtrArray		m_StorageSocketItem;				//存储连接

	//辅助变量
protected:
	DWORD							m_dwLastDetect;						//检测时间
	CServerSocketItemPtrArray		m_TempSocketItem;					//临时连接

	//配置变量
protected:
	WORD							m_wListenPort;						//监听端口
	WORD							m_wMaxSocketItem;					//最大连接

protected:
	CQueueService					m_SendQueueService;					//队列对象
	CQueueServiceEvent				m_QueueServiceEvent;				//通知组件

	//内核变量
protected:
	bool							m_bService;							//服务标志
	SOCKET							m_hServerSocket;					//连接句柄
	HANDLE							m_hCompletionPort;					//完成端口
	CSocketDetectThread				m_SocketDetectThread;				//检测线程
	CSocketAcceptThread				m_SocketAcceptThread;				//应答线程
	CServerSocketRSThreadPtrArray	m_SocketRSThreadArray;				//读写线程

	//函数定义
public:
	//构造函数
	CTCPNetworkEngine(void);
	//析构函数
	virtual ~CTCPNetworkEngine(void);

	//基础接口
public:
	//释放对象
	virtual VOID Release()
	{
		if (IsValid()) delete this;
	}
	//是否有效
	virtual bool IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CTCPNetworkEngine)) ? true : false;
	}
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//服务接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();

	//信息接口
public:
	//配置端口
	virtual WORD GetServicePort();
	//当前端口
	virtual WORD GetCurrentPort();

	//配置接口
public:
	//设置接口
	virtual bool SetTCPNetworkEngineEvent(IUnknownEx * pIUnknownEx);
	//设置参数
	virtual bool SetServiceParameter(WORD wServicePort, WORD wMaxConnect, LPCTSTR  pszCompilation);

	//队列接口
public:
	//通知回调函数
	virtual void OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize);

	//网络接口
public:
	//检测连接
	virtual bool DetectSocket();
	//发送函数
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//批量发送
	virtual bool SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize, BYTE cbBatchMask);
	//关闭连接
	virtual bool CloseSocket(DWORD dwSocketID);
	//设置关闭
	virtual bool ShutDownSocket(DWORD dwSocketID);
	//允许群发
	virtual bool AllowBatchSend(DWORD dwSocketID, bool bAllow, BYTE cbBatchMask);

	//通知接口
public:
	//应答消息
	virtual bool OnSocketAcceptEvent(CServerSocketItem * pServerSocketItem);
	//读取消息
	virtual bool OnSocketReadEvent(TCP_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem);
	//关闭消息
	virtual bool OnSocketCloseEvent(CServerSocketItem * pServerSocketItem);

	virtual bool OnSocketForegroundEvent(CServerSocketItem* pServerSocketItem);

	//内部函数
protected:
	//激活空闲对象
	CServerSocketItem * ActiveSocketItem();
	//获取对象
	CServerSocketItem * EnumSocketItem(WORD wIndex);
	//释放连接对象
	bool FreeSocketItem(CServerSocketItem * pServerSocketItem);

};


#endif