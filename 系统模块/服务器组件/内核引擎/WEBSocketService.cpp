#include "StdAfx.h"
#include "TraceService.h"
#include "WEBSocketService.h"


//宏定义
#define ID_SOCKET_WND				10								//SOCKET 窗口 ID

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWEBSocketService, CWnd)
END_MESSAGE_MAP()

//函数定义
//////////////////////////////////////////////////////////////////////////
//构造函数
CWEBSocketService::CWEBSocketService(void)
{

}

//析构函数
CWEBSocketService::~CWEBSocketService(void)
{

}

//基础接口
//////////////////////////////////////////////////////////////////////////
//接口查询
void * CWEBSocketService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWEBSocketService, Guid, dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWEBSocketService, Guid, dwQueryVer);
	return NULL;
}

//服务接口
//////////////////////////////////////////////////////////////////////////
//启动服务
bool CWEBSocketService::StartService()
{
	return true;
}

//停止服务
bool CWEBSocketService::ConcludeService()
{
	return true;
}

//队列接口
//////////////////////////////////////////////////////////////////////////
//通知回调
void CWEBSocketService::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)
{

}

//配置接口
//////////////////////////////////////////////////////////////////////////
//配置函数
bool CWEBSocketService::SetServiceID(WORD wServiceID)
{
	return true;
}

//设置接口
bool CWEBSocketService::SetWEBSocketEvent(IUnknownEx * pIUnknownEx)
{
	return true;
}

//功能接口
//////////////////////////////////////////////////////////////////////////
//关闭连接
bool CWEBSocketService::CloseSocket(BYTE cbShutReason)
{
	return true;
}

//连接地址
bool CWEBSocketService::ConnectServer(LPCTSTR pszURL, WORD wPort)
{
	return true;
}

//连接地址
bool CWEBSocketService::SendRequestData(VOID * pData, WORD wDataSize)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
//建立对象函数
extern "C" __declspec(dllexport) void * CreateWEBSocketService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//建立对象
	CWEBSocketService * pWEBSocketService = NULL;
	try
	{
		pWEBSocketService = new CWEBSocketService();
		if (pWEBSocketService == NULL) throw TEXT("创建失败");
		void * pObject = pWEBSocketService->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pWEBSocketService);
	return NULL;
}