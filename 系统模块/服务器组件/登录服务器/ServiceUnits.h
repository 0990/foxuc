#ifndef SERVICE_UNITS_HEAD_FILE
#define SERVICE_UNITS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "AttemperEngineSink.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//网络标示
#define NETWORK_CORRESPOND			1									//登录连接

//消息定义
#define WM_UICONTROL_REQUEST		(WM_USER+100)						//控制请求

//////////////////////////////////////////////////////////////////////////////////
//枚举定义

//服务状态
enum enServiceStatus
{
	ServiceStatus_Stop,				//停止状态
	ServiceStatus_Config,			//配置状态
	ServiceStatus_Service,			//服务状态
};

//////////////////////////////////////////////////////////////////////////////////

//状态接口
interface IServiceUnitsSink
{
	//接口定义
public:
	//服务状态
	virtual VOID OnServiceUnitsStatus(enServiceStatus ServiceStatus)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//服务单元
class CServiceUnits : public CWnd
{
	//友元定义
	friend class CAttemperEngineSink;
	friend class CDataBaseEngineSink;

	//状态变量
protected:
	enServiceStatus					m_ServiceStatus;					//运行状态

	//配置组件
public:
	CInitParameter					m_InitParameter;					//配置参数

	//组件变量
private:
	CWHDataQueue					m_DataQueue;						//数据队列
	CCriticalSection				m_CriticalSection;					//同步对象

	//服务组件
protected:
	CAttemperEngineSink				m_AttemperEngineSink;				//调度钩子
	CDataBaseEngineSink				m_DataBaseEngineSink[4];			//数据钩子

	//内核组件
protected:
	CTimerEngineHelper				m_TimerEngine;						//时间引擎
	CAttemperEngineHelper			m_AttemperEngine;					//调度引擎
	CDataBaseEngineHelper			m_DataBaseEngine;					//数据引擎
	CTCPNetworkEngineHelper			m_TCPNetworkEngine;					//网络引擎
	CTCPSocketServiceHelper			m_TCPSocketService;					//协调服务

	//接口变量
protected:
	IServiceUnitsSink *				m_pIServiceUnitsSink;				//状态接口

	//静态变量
public:
	static CServiceUnits *			g_pServiceUnits;					//对象指针

	//函数定义
public:
	//构造函数
	CServiceUnits();
	//析构函数
	virtual ~CServiceUnits();

	//信息函数
public:
	//获取状态
	enServiceStatus GetServiceStatus() { return m_ServiceStatus; }

	//服务控制
public:
	//启动服务
	bool StartService();
	//停止服务
	bool ConcludeService();
	//设置接口
	bool SetServiceUnitsSink(IServiceUnitsSink * pIServiceUnitsSink);
	//投递请求
	bool PostControlRequest(WORD wIdentifier, VOID * pData, WORD wDataSize);

	//辅助函数
protected:
	//配置组件
	bool InitializeService();
	//启动内核
	bool StartKernelService();
	//启动网络
	bool StartNetworkService();

	//内部函数
private:
	//设置状态
	bool SetServiceStatus(enServiceStatus ServiceStatus);
	//发送控制
	bool SendControlPacket(WORD wControlID, VOID * pData, WORD wDataSize);

	//消息映射
protected:
	//控制消息
	LRESULT OnUIControlRequest(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif