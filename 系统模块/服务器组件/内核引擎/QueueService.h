#ifndef QUEUE_SERVICE_HEAD_FILE
#define QUEUE_SERVICE_HEAD_FILE

#include "KernelEngineHead.h"


#define VER_IQueueServiceSink INTERFACE_VERSION(1,1)
static const GUID IID_IQueueServiceSink = {0x88b5bf59, 0x3a98, 0x43b1, 0xac, 0x85, 0xf1, 0x17, 0x5b, 0x29, 0x69, 0x40};

//数据队列类钩子接口
interface IQueueServiceSink : public IUnknownEx
{
	//通知回调函数
	virtual void OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize) = NULL;
	//virtual void BindQueueService(CQueueServiceEvent* queueServiceEvent) = NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IQueueService INTERFACE_VERSION(1,1)
static const GUID IID_IQueueService = {0xcc5310b5, 0x3709, 0x40aa, 0x85, 0x24, 0xd6, 0xc5, 0x87, 0xb0, 0x32, 0x22};

//队列接口
interface IQueueService : public IServiceModule
{
	//设置接口
	virtual bool SetQueueServiceSink(IUnknownEx * pIUnknownEx) = NULL;
	//负荷信息
	virtual bool GetBurthenInfo(tagBurthenInfo & BurthenInfo) = NULL;
	//加入数据
	virtual bool AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize) = NULL;
};

//////////////////////////////////////////////////////////////////////////

//队列线程类
class CQueueServiceThread : public CWHThread
{
	//友员定义
	friend class CQueueService;

	//配置定义
protected:
	HANDLE							m_hCompletionPort;					//完成端口

	//辅助变量
private:
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];		//接收缓冲

	//函数定义
protected:
	//构造函数
	CQueueServiceThread(void);
	//析构函数
	virtual ~CQueueServiceThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(HANDLE hCompletionPort);
	//取消配置
	bool UnInitThread();

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//数据队列类
class CQueueService : public IQueueService
{
	friend class CQueueServiceThread;

	//变量定义
protected:
	bool							m_bService;							//服务标志
	HANDLE							m_hCompletionPort;					//完成端口
	IQueueServiceSink				* m_pIQueueServiceSink;				//回调接口

	//组件变量
protected:
	CCriticalSection				m_CriticalSection;					//线程锁
	CWHDataQueue					m_DataQueue;						//数据存储
	CQueueServiceThread				m_QueueServiceThread;				//队列线程

	//函数定义
public:
	//构造函数
	CQueueService(void);
	//析构函数
	virtual ~CQueueService(void);

	//基础接口
public:
	//释放对象
	virtual VOID Release()
	{
		if (IsValid()) delete this;    //
	}
	//是否有效
	virtual bool IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CQueueService)) ? true : false;
	}
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);


	//管理接口
public:
	//开始服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();
	//线程句柄
	HANDLE GetThreadHandle();

	//队列接口
public:
	//加入数据
	virtual bool AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize);
	//设置接口
	virtual bool SetQueueServiceSink(IUnknownEx * pIUnknownEx);
	//负荷信息
	virtual bool GetBurthenInfo(tagBurthenInfo & BurthenInfo);

	//辅助函数
private:
	//提取数据
	bool GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize);
	//数据消息
	void OnQueueServiceThread(const tagDataHead & DataHead, void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif