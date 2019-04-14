#ifndef PROCESS_DATA_CONTROL_HEAD_FILE
#define PROCESS_DATA_CONTROL_HEAD_FILE

#pragma once

#include "ProcessControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//数据发送
class PROCESS_CONTROL_CLASS CProcessSendData
{
	//变量定义
protected:
	HWND							m_hWndServer;						//接收窗口
	HWND							m_hWndClient;						//发送窗口

	//函数定义
public:
	//构造函数
	CProcessSendData(HWND hWndServer=NULL, HWND hWndClient=NULL);
	//析构函数
	virtual ~CProcessSendData();

	//信息函数
public:
	//是否连接
	bool IsConnect();
	//获取窗口
	HWND GetClientHwnd();
	//获取窗口
	HWND GetServerHwnd();

	//管理接口
public:
	//设置窗口
	bool SetClientHwnd(HWND hWndClient);
	//设置窗口
	bool SetServerHwnd(HWND hWndServer);
	//设置窗口
	bool SetServerHwnd(LPCTSTR pszClassName);

	//发送函数
public:
	//发送数据
	bool SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送数据
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//执行命令
	bool TransferCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

//数据接收
class PROCESS_CONTROL_CLASS CProcessRecvData
{
	//变量定义
protected:
	IIPCRecvDataSink *				m_pIIPCRecvDataSink;				//回调接口

	//函数定义
public:
	//构造函数
	CProcessRecvData();
	//析构函数
	virtual ~CProcessRecvData();

	//接口函数
public:
	//设置接口
	bool SetProcessTradeSink(IUnknownEx * pIUnknownEx);
	//消息处理
	bool OnCopyData(HWND hWndSend, COPYDATASTRUCT * pCopyDataStruct);
};

//////////////////////////////////////////////////////////////////////////////////

#endif
