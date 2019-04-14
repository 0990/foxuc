#include "StdAfx.h"
#include "ProcessControl.h"

//////////////////////////////////////////////////////////////////////////////////
//宏定义

//标识定义
#define IDC_PROCESS_CONTROL			100									//窗口标识

//消息定义
#define WM_ASYNCHRONISM_DATA		(WM_USER+100)						//异步数据

//////////////////////////////////////////////////////////////////////////////////

//异步数据
struct tagAsynchronismData
{
	HWND							hWndSend;							//发送窗口
	WORD							wDataSize;							//数据大小
	WORD							wSubCmdID;							//子命令码
	WORD							wMainCmdID;							//主命令码
	BYTE							cbBuffer[IPC_PACKET];				//数据缓冲
};

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CProcessControl, CWnd)
	ON_WM_COPYDATA()
	ON_MESSAGE(WM_ASYNCHRONISM_DATA,OnMessageAsynchronismData)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CProcessControl::CProcessControl()
{
	//设置变量
	m_pIProcessControlSink=NULL;

	return;
}

//析构函数
CProcessControl::~CProcessControl()
{
}

//接口查询
VOID * CProcessControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IProcessControl,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IProcessControl,Guid,dwQueryVer);
	return NULL;
}

//获取句柄
HWND CProcessControl::GetProcessControlWnd()
{
	return m_hWnd;
}

//创建通讯
bool CProcessControl::SetProcessControlSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessControlSink)!=NULL);
		m_pIProcessControlSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessControlSink);

		//成功判断
		if (m_pIProcessControlSink==NULL) return false;
	}
	else m_pIProcessControlSink=NULL;

	return true;
}

//配置连接
bool CProcessControl::InitializeLink()
{
	//效验参数
	ASSERT(m_pIProcessControlSink!=NULL);
	if (m_pIProcessControlSink==NULL) return false;

	//创建窗口
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),IDC_PROCESS_CONTROL);
	}

	return true;
}

//销毁通讯
bool CProcessControl::UnInitializeLink()
{
	//清理队列
	m_DataQueue.RemoveData(false);

	//关闭窗口
	if (m_hWnd!=NULL) DestroyWindow();

	return true;
}

//发送数据
bool CProcessControl::SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID)
{
	//效验参数
	if (IsWindow(m_hWnd)==FALSE) return false;
	if (IsWindow(hWndServer)==FALSE) return false;

	//构造数据
	IPC_Head IPCHead;
	IPCHead.wVersion=IPC_VER;
	IPCHead.wSubCmdID=wSubCmdID;
	IPCHead.wMainCmdID=wMainCmdID;
	IPCHead.wPacketSize=sizeof(IPC_Head);

	//发送数据
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCHead;
	CopyDataStruct.dwData=IDT_ASYNCHRONISM;
	CopyDataStruct.cbData=IPCHead.wPacketSize;
	::SendMessage(hWndServer,WM_COPYDATA,(LPARAM)m_hWnd,(WPARAM)&CopyDataStruct);

	return true;
}

//发送数据
bool CProcessControl::SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)
{
	//效验参数
	if (IsWindow(m_hWnd)==FALSE) return false;
	if (IsWindow(hWndServer)==FALSE) return false;

	//构造数据
	IPC_Buffer IPCBuffer;
	IPCBuffer.Head.wVersion=IPC_VER;
	IPCBuffer.Head.wSubCmdID=wSubCmdID;
	IPCBuffer.Head.wMainCmdID=wMainCmdID;
	IPCBuffer.Head.wPacketSize=sizeof(IPCBuffer.Head);

	//附加数据
	if (pData!=NULL)
	{
		//效验长度
		ASSERT(wDataSize<=sizeof(IPCBuffer.cbBuffer));
		if (wDataSize>sizeof(IPCBuffer.cbBuffer)) return false;

		//拷贝数据
		IPCBuffer.Head.wPacketSize+=wDataSize;
		CopyMemory(IPCBuffer.cbBuffer,pData,wDataSize);
	}

	//发送数据
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCBuffer;
	CopyDataStruct.dwData=IDT_ASYNCHRONISM;
	CopyDataStruct.cbData=IPCBuffer.Head.wPacketSize;
	::SendMessage(hWndServer,WM_COPYDATA,(LPARAM)m_hWnd,(WPARAM)&CopyDataStruct);

	return true;
}

//执行命令
bool CProcessControl::SendCommand(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)
{
	//效验参数
	if (IsWindow(m_hWnd)==FALSE) return false;
	if (IsWindow(hWndServer)==FALSE) return false;

	//构造数据
	IPC_Buffer IPCBuffer;
	IPCBuffer.Head.wVersion=IPC_VER;
	IPCBuffer.Head.wSubCmdID=wSubCmdID;
	IPCBuffer.Head.wMainCmdID=wMainCmdID;
	IPCBuffer.Head.wPacketSize=sizeof(IPCBuffer.Head);

	//附加数据
	if (pData!=NULL)
	{
		//效验长度
		ASSERT(wDataSize<=sizeof(IPCBuffer.cbBuffer));
		if (wDataSize>sizeof(IPCBuffer.cbBuffer)) return false;

		//拷贝数据
		IPCBuffer.Head.wPacketSize+=wDataSize;
		CopyMemory(IPCBuffer.cbBuffer,pData,wDataSize);
	}

	//发送数据
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCBuffer;
	CopyDataStruct.dwData=IDT_IMMEDIATE;
	CopyDataStruct.cbData=IPCBuffer.Head.wPacketSize;
	::SendMessage(hWndServer,WM_COPYDATA,(LPARAM)m_hWnd,(WPARAM)&CopyDataStruct);

	return true;
}

//IPC 消息
BOOL CProcessControl::OnCopyData(CWnd * pWnd, COPYDATASTRUCT * pCopyDataStruct)
{
	//效验接口
	ASSERT(m_pIProcessControlSink!=NULL);
	if (m_pIProcessControlSink==NULL) return FALSE;

	//效验参数
	ASSERT(pCopyDataStruct->cbData>=sizeof(IPC_Head));
	if (pCopyDataStruct->cbData<sizeof(IPC_Head)) return FALSE;

	//效验数据
	IPC_Head * pHead=(IPC_Head *)pCopyDataStruct->lpData;
	ASSERT((pHead->wVersion==IPC_VER)&&(pHead->wPacketSize==pCopyDataStruct->cbData));
	if ((pHead->wVersion!=IPC_VER)||(pHead->wPacketSize!=pCopyDataStruct->cbData)) return FALSE;

	//数据处理
	switch (pCopyDataStruct->dwData)
	{
	case IDT_IMMEDIATE:			//同步数据
		{
			//数据处理
			WORD wDataSize=pHead->wPacketSize-sizeof(IPC_Head);
			m_pIProcessControlSink->OnProcessControlData(pHead->wMainCmdID,pHead->wSubCmdID,pHead+1,wDataSize,pWnd->m_hWnd);

			return true;
		}
	case IDT_ASYNCHRONISM:		//异步数据
		{
			//构造数据
			tagAsynchronismData AsynchronismData;
			AsynchronismData.wSubCmdID=pHead->wSubCmdID;
			AsynchronismData.wMainCmdID=pHead->wMainCmdID;
			AsynchronismData.hWndSend=pWnd->GetSafeHwnd();
			AsynchronismData.wDataSize=pHead->wPacketSize-sizeof(IPC_Head);
			CopyMemory(&AsynchronismData.cbBuffer,pHead+1,pHead->wPacketSize-sizeof(IPC_Head));

			//插入数据
			WORD wHeadSize=sizeof(AsynchronismData)-sizeof(AsynchronismData.cbBuffer);
			m_DataQueue.InsertData(IDT_ASYNCHRONISM,&AsynchronismData,wHeadSize+AsynchronismData.wDataSize);

			//投递消息
			ASSERT(pWnd!=NULL);
			if (pWnd!=NULL) PostMessage(WM_ASYNCHRONISM_DATA,(WPARAM)(pWnd->m_hWnd),(LPARAM)(pHead->wPacketSize));

			return true;
		}
	}

	return TRUE;
}

//通知消息
LRESULT	CProcessControl::OnMessageAsynchronismData(WPARAM wParam, LPARAM lParam)
{
	//提取数据
	tagDataHead DataHead;
	tagAsynchronismData AsynchronismData;
	if (m_DataQueue.DistillData(DataHead,&AsynchronismData,sizeof(AsynchronismData))==false)
	{
		ASSERT(FALSE);
		return 0;
	}

	//状态效验
	ASSERT(m_pIProcessControlSink!=NULL);
	if (m_pIProcessControlSink==NULL) return 0;

	//数据处理
	HWND hWndClient=AsynchronismData.hWndSend;
	WORD wSubCmdID=AsynchronismData.wSubCmdID;
	WORD wMainCmdID=AsynchronismData.wMainCmdID;
	m_pIProcessControlSink->OnProcessControlData(wMainCmdID,wSubCmdID,AsynchronismData.cbBuffer,AsynchronismData.wDataSize,hWndClient);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(ProcessControl);

//////////////////////////////////////////////////////////////////////////////////
