#ifndef ATTEMPER_ENGINE_HEAD_FILE
#define ATTEMPER_ENGINE_HEAD_FILE

#pragma once

//组件头文件

#include "KernelEngineHead.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"

//////////////////////////////////////////////////////////////////////////

//调度管理类
class CAttemperEngine : public IAttemperEngine, public IQueueServiceSink,
		public IDataBaseEngineEvent, public ITCPNetworkEngineEvent
{
	//核心变量
protected:
	bool							m_bService;							//运行标志
	CQueueService					m_QueueService;						//队列对象
	CQueueServiceEvent              m_QueueServiceEvent;
	CCriticalSection				m_CriticalSection;					//同步锁定

	//接口变量
protected:
	ITCPSocketService				* m_pITCPSocketService;				//网络引擎
	ITCPNetworkEngine				* m_pTCPNetworkEngine;				//网络引擎
	IAttemperEngineSink				* m_pIAttemperEngineSink;			//挂接接口

	//机器人链接维护
protected:
	CWHArray<DWORD>					m_AndroidUserSocketIDArray;			//机器人连接

	//函数定义
public:
	//构造函数
	CAttemperEngine(void);
	//析构函数
	virtual ~CAttemperEngine(void);

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
		return AfxIsValidAddress(this, sizeof(CAttemperEngine)) ? true : false;
	}
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();

	//调度引擎
public:
	//设置网络
	virtual bool SetNetworkEngine(IUnknownEx * pIUnknownEx);
	//注册钩子
	virtual bool SetAttemperEngineSink(IUnknownEx * pIUnknownEx);
	//自定事件
	virtual bool OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize);
	//控制事件
	virtual bool OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize);

	//队列接口
public:
	//触发接口
	virtual void OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize/*, DWORD dwInsertTime*/);

	//virtual void BindQueueService(CQueueServiceEvent* queueServiceEvent);
	//接口定义
public:
	//数据库结果
	virtual bool OnEventDataBaseResult(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//机器模拟接口
public:
	//应答事件
	virtual bool OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientAddr);
	//关闭事件
	virtual bool OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientAddr, DWORD dwActiveTime);
	//读取事件
	virtual bool OnEventTCPNetworkRead(DWORD dwSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

public:
	//设置网络
	virtual bool SetSocketEngine(IUnknownEx * pIUnknownEx);
	//获取接口
	virtual void * GetQueueService(const IID & Guid, DWORD dwQueryVer);

	virtual void * GetQueue();
};

//////////////////////////////////////////////////////////////////////////

#endif