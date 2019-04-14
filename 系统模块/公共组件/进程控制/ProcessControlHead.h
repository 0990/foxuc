#ifndef PROCESS_CONTROL_HEAD_HEAD_FILE
#define PROCESS_CONTROL_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//MFC 文件
#include <Afxmt.h>
#include <AtlBase.h>

//平台文件
#include "..\..\全局定义\Platform.h"

//组件文件
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef PROCESS_CONTROL_CLASS
	#ifdef  PROCESS_CONTROL_DLL
		#define PROCESS_CONTROL_CLASS _declspec(dllexport)
	#else
		#define PROCESS_CONTROL_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define PROCESS_CONTROL_DLL_NAME	TEXT("ProcessControl.dll")		//组件名字
#else
	#define PROCESS_CONTROL_DLL_NAME	TEXT("ProcessControlD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//处理类型
#define IDT_IMMEDIATE				0x0101								//同步数据
#define IDT_ASYNCHRONISM			0x0201								//异步数据

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IIPCRecvDataSink INTERFACE_VERSION(1,1)
	static const GUID IID_IIPCRecvDataSink={0xeaea7e0b,0x2bba,0x414e,0x91,0xb4,0x9c,0x7e,0x1e,0xb2,0xdf,0xb9};
#else
	#define VER_IIPCRecvDataSink INTERFACE_VERSION(1,1)
	static const GUID IID_IIPCRecvDataSink={0xe1b2bbd0,0xf92e,0x4898,0x89,0xdb,0x72,0xdd,0xc2,0x8c,0x70,0x70};
#endif

//数据接口
interface IIPCRecvDataSink : public IUnknownEx
{
	//同步数据
	virtual bool OnImmediateData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)=NULL;
	//异步数据
	virtual bool OnAsynchronismData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IProcessControl INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessControl={0x7675ae4c,0x38bb,0x4de6,0x94,0xb3,0xe0,0x06,0xfb,0xe8,0x30,0xca};
#else
	#define VER_IProcessControl INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessControl={0x71733503,0x9d41,0x429e,0x8a,0xf0,0x78,0x40,0x61,0x86,0x01,0xbe};
#endif

//进程控制
interface IProcessControl : public IUnknownEx
{
	//配置接口
public:
	//获取句柄
	virtual HWND GetProcessControlWnd()=NULL;
	//配置组件
	virtual bool SetProcessControlSink(IUnknownEx * pIUnknownEx)=NULL;

	//连接管理
public:
	//配置连接
	virtual bool InitializeLink()=NULL;
	//销毁通讯
	virtual bool UnInitializeLink()=NULL;

	//发送接口
public:
	//发送数据
	virtual bool SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//发送数据
	virtual bool SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)=NULL;
	//执行命令
	virtual bool SendCommand(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IProcessControlSink INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessControlSink={0xe2b7aac5,0x2e60,0x4fda,0x8a,0x5a,0xfa,0x5e,0x35,0x1e,0x75,0x1a};
#else
	#define VER_IProcessControlSink INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessControlSink={0xc770827b,0xdeb5,0x471c,0x8e,0xf5,0x42,0x34,0x63,0xe8,0x72,0x2e};
#endif

//进程数据
interface IProcessControlSink : public IUnknownEx
{
	//进程数据
	virtual bool OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#include "ProcessControl.h"
#include "ProcessDataControl.h"

//////////////////////////////////////////////////////////////////////////////////

//组件创建
DECLARE_MODULE_HELPER(ProcessControl,PROCESS_CONTROL_DLL_NAME,"CreateProcessControl")

//////////////////////////////////////////////////////////////////////////////////

#endif