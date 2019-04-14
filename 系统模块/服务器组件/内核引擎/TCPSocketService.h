#ifndef _TCP_SOCKET_SERVICE_H_
#define _TCP_SOCKET_SERVICE_H_

#include "KernelEngineHead.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"

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
//

//动作定义
#define QUEUE_SEND_REQUEST			1									//发送标识
#define QUEUE_CONNECT_REQUEST		2									//发送标识
#define QUEUE_SAFE_CLOSE			3									//安全关闭
#define QUEUE_DETECT_SOCKET			4									//检测连接


class CTCPSocketService : public CWnd, public ITCPSocketService, public IQueueServiceSink
{
private:
	bool							m_bService;							//服务标志
	CQueueServiceEvent				m_QueueServiceEvent;				//通知组件
	CQueueService					m_QueueService;						//队列对象

	//加密数据
protected:
	BYTE							m_cbSendRound;						//字节映射
	BYTE							m_cbRecvRound;						//字节映射
	DWORD							m_dwSendXorKey;						//发送密钥
	DWORD							m_dwRecvXorKey;						//接收密钥

private:
	bool							m_bSuccess;							//连接成功
	DWORD							m_dwTempServerIP;					//连接的IP

private:
	DWORD							m_dwServerIP;						//连接的IP
	WORD							m_wPort;							//链接端口

	//内核变量
protected:
	SOCKET							m_hSocket;							//连接句柄
	WORD							m_wServiceID;						//服务ID
	BYTE							m_cbSocketStatus;					//连接状态

	//计数变量
protected:
	DWORD							m_dwSendTickCount;					//发送时间
	DWORD							m_dwRecvTickCount;					//接收时间
	DWORD							m_dwSendPacketCount;				//发送计数
	DWORD							m_dwRecvPacketCount;				//接受计数

	//接收变量
protected:
	WORD							m_wRecvSize;						//接收长度
	BYTE							m_cbRecvBuf[SOCKET_TCP_BUFFER*10];	//接收缓冲

public:
	CTCPSocketService(void);
	~CTCPSocketService(void);


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
		return AfxIsValidAddress(this, sizeof(CTCPSocketService)) ? true : false;
	}
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//服务接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();

	//队列接口
public:
	//通知回调函数
	virtual void OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize);

	//配置接口
public:
	//配置函数
	virtual bool SetServiceID(WORD wServiceID);
	//设置接口
	virtual bool SetTCPSocketEvent(IUnknownEx * pIUnknownEx);


	//功能接口
public:
	//关闭连接
	virtual bool CloseSocket();
	//连接地址
	virtual bool Connect(DWORD dwServerIP, WORD wPort);
	//连接地址
	virtual bool Connect(LPCTSTR szServerIP, WORD wPort);
	//发送函数
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);


	//辅助函数
protected:
	//地址解释
	DWORD TranslateAddr(LPCTSTR pszServerAddr);
	//发送数据
	bool SendDataBuffer(void * pBuffer, WORD wSendSize);
	//关闭连接
	VOID CloseSocket(BYTE cbShutReason);

	//加密函数
protected:
	//解密数据
	WORD CrevasseBuffer(BYTE cbDataBuffer[], WORD wDataSize);
	//加密数据
	WORD EncryptBuffer(BYTE cbDataBuffer[], WORD wDataSize, WORD wBufferSize);

	//内联函数
private:
	//字节映射
	inline WORD SeedRandMap(WORD wSeed);
	//发送映射
	inline BYTE MapSendByte(BYTE cbData);
	//接收映射
	inline BYTE MapRecvByte(BYTE cbData);

	//消息映射
protected:
	//网络消息
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	//消息函数
protected:
	//网络读取
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//网络发送
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam) {};
	//网络关闭
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//网络连接
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};

#endif //_TCP_SOCKET_SERVICE_H_