#include "StdAfx.h"
#include "ProcessDataControl.h"

//////////////////////////////////////////////////////////////////////////////////

//处理类型
#define IDT_IMMEDIATE				0x0101								//同步数据
#define IDT_ASYNCHRONISM			0x0201								//异步数据

//////////////////////////////////////////////////////////////////////////////////

//函数定义
CProcessSendData::CProcessSendData(HWND hWndServer, HWND hWndClient)
{
	//设置变量
	m_hWndServer=hWndServer;
	m_hWndClient=hWndClient;

	return;
}

//析构函数
CProcessSendData::~CProcessSendData()
{
}

//是否连接
bool CProcessSendData::IsConnect()
{
	return IsWindow(m_hWndServer)?true:false;
}

//获取窗口
HWND CProcessSendData::GetClientHwnd()
{
	return m_hWndClient;
}

//获取窗口
HWND CProcessSendData::GetServerHwnd()
{
	return m_hWndServer;
}

//设置窗口
bool CProcessSendData::SetClientHwnd(HWND hWndClient)
{
	//设置变量
	m_hWndClient=NULL;

	//窗口效验
	if ((hWndClient!=NULL)&&(IsWindow(hWndClient)))
	{
		m_hWndClient=hWndClient;
		return true;
	}

	return false;
}

//设置窗口
bool CProcessSendData::SetServerHwnd(HWND hWndServer)
{
	//设置变量
	m_hWndServer=NULL;

	//窗口效验
	if ((hWndServer!=NULL)&&(IsWindow(hWndServer)))
	{
		m_hWndServer=hWndServer;
		return true;
	}

	return false;
}

//设置窗口
bool CProcessSendData::SetServerHwnd(LPCTSTR pszClassName)
{
	//效验参数
	ASSERT(pszClassName!=NULL);
	if (pszClassName==NULL) return false;

	//设置变量
	m_hWndServer=FindWindow(pszClassName,NULL);

	return (m_hWndServer!=NULL);
}

//发送数据
bool CProcessSendData::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验参数
	ASSERT(IsWindow(m_hWndServer));
	if (IsWindow(m_hWndServer)==FALSE) return false;

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
	::SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//发送数据
bool CProcessSendData::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(IsWindow(m_hWndServer));
	if (IsWindow(m_hWndServer)==FALSE) return false;

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
	::SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//执行命令
bool CProcessSendData::TransferCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(IsWindow(m_hWndServer));
	if (IsWindow(m_hWndServer)==FALSE) return false;

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
	::SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CProcessRecvData::CProcessRecvData()
{
	//设置变量
	m_pIIPCRecvDataSink=NULL;

	return;
}

//析构函数
CProcessRecvData::~CProcessRecvData()
{
}

//设置接口
bool CProcessRecvData::SetProcessTradeSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IIPCRecvDataSink)!=NULL);
		m_pIIPCRecvDataSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IIPCRecvDataSink);

		//成功判断
		if (m_pIIPCRecvDataSink==NULL) return false;
	}
	else m_pIIPCRecvDataSink=NULL;

	return true;
}

//消息处理
bool CProcessRecvData::OnCopyData(HWND hWndSend, COPYDATASTRUCT * pCopyDataStruct)
{
	//效验接口
	ASSERT(m_pIIPCRecvDataSink!=NULL);
	if (m_pIIPCRecvDataSink==NULL) return false;

	//效验参数
	ASSERT(pCopyDataStruct->cbData>=sizeof(IPC_Head));
	if (pCopyDataStruct->cbData<sizeof(IPC_Head)) return false;

	//效验数据
	IPC_Head * pHead=(IPC_Head *)pCopyDataStruct->lpData;
	ASSERT((pHead->wVersion==IPC_VER)&&(pHead->wPacketSize==pCopyDataStruct->cbData));
	if ((pHead->wVersion!=IPC_VER)||(pHead->wPacketSize!=pCopyDataStruct->cbData)) return false;

	//数据处理
	switch (pCopyDataStruct->dwData)
	{
	case IDT_IMMEDIATE:			//同步数据
		{
			//数据处理
			WORD wDataSize=pHead->wPacketSize-sizeof(IPC_Head);
			m_pIIPCRecvDataSink->OnImmediateData(pHead->wMainCmdID,pHead->wSubCmdID,pHead+1,wDataSize,hWndSend);

			return true;
		}
	case IDT_ASYNCHRONISM:		//异步数据
		{
			//数据处理
			WORD wDataSize=pHead->wPacketSize-sizeof(IPC_Head);
			m_pIIPCRecvDataSink->OnAsynchronismData(pHead->wMainCmdID,pHead->wSubCmdID,pHead+1,wDataSize,hWndSend);

			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
