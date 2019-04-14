#ifndef PROCESS_TRADE_HEAD_FILE
#define PROCESS_TRADE_HEAD_FILE

#pragma once

#include "ProcessControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//进程通讯
class PROCESS_CONTROL_CLASS CProcessControl : public CWnd, public IProcessControl
{
	//组件变量
protected:
	CWHDataQueue					m_DataQueue;						//数据队列
	IProcessControlSink *			m_pIProcessControlSink;				//回调接口

	//函数定义
public:
	//构造函数
	CProcessControl();
	//析构函数
	virtual ~CProcessControl();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//获取句柄
	virtual HWND GetProcessControlWnd();
	//创建通讯
	virtual bool SetProcessControlSink(IUnknownEx * pIUnknownEx);

	//连接管理
public:
	//配置连接
	virtual bool InitializeLink();
	//销毁通讯
	virtual bool UnInitializeLink();

	//发送接口
public:
	//发送数据
	virtual bool SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID);
	//发送数据
	virtual bool SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);
	//执行命令
	virtual bool SendCommand(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	//消息映射
protected:
	//IPC 消息
	BOOL OnCopyData(CWnd * pWnd, COPYDATASTRUCT * pCopyDataStruct);
	//异步数据
	LRESULT	OnMessageAsynchronismData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif