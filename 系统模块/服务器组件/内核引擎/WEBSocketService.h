#ifndef _WEB_SOCKET_SERVICE_H_
#define _WEB_SOCKET_SERVICE_H_

#include "KernelEngineHead.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"


class CWEBSocketService : public CWnd, public IWEBSocketService, public IQueueServiceSink
{

public:
	CWEBSocketService(void);
	~CWEBSocketService(void);


	//基础接口
public:
	//释放对象
	virtual VOID Release()	{ if (IsValid()) delete this; }
	//是否有效
	virtual bool IsValid()	{ return AfxIsValidAddress(this, sizeof(CWEBSocketService)) ? true : false; }
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
	virtual bool SetWEBSocketEvent(IUnknownEx * pIUnknownEx);

	//功能接口
public:
	//关闭连接
	virtual bool CloseSocket(BYTE cbShutReason);
	//连接操作
	virtual bool ConnectServer(LPCTSTR pszURL, WORD wPort);
	//发送请求
	virtual bool SendRequestData(VOID * pData, WORD wDataSize);

	//消息映射
protected:
	DECLARE_MESSAGE_MAP()

};

#endif //_WEB_SOCKET_SERVICE_H_