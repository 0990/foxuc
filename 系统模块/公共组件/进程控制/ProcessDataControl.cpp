#include "StdAfx.h"
#include "ProcessDataControl.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define IDT_IMMEDIATE				0x0101								//ͬ������
#define IDT_ASYNCHRONISM			0x0201								//�첽����

//////////////////////////////////////////////////////////////////////////////////

//��������
CProcessSendData::CProcessSendData(HWND hWndServer, HWND hWndClient)
{
	//���ñ���
	m_hWndServer=hWndServer;
	m_hWndClient=hWndClient;

	return;
}

//��������
CProcessSendData::~CProcessSendData()
{
}

//�Ƿ�����
bool CProcessSendData::IsConnect()
{
	return IsWindow(m_hWndServer)?true:false;
}

//��ȡ����
HWND CProcessSendData::GetClientHwnd()
{
	return m_hWndClient;
}

//��ȡ����
HWND CProcessSendData::GetServerHwnd()
{
	return m_hWndServer;
}

//���ô���
bool CProcessSendData::SetClientHwnd(HWND hWndClient)
{
	//���ñ���
	m_hWndClient=NULL;

	//����Ч��
	if ((hWndClient!=NULL)&&(IsWindow(hWndClient)))
	{
		m_hWndClient=hWndClient;
		return true;
	}

	return false;
}

//���ô���
bool CProcessSendData::SetServerHwnd(HWND hWndServer)
{
	//���ñ���
	m_hWndServer=NULL;

	//����Ч��
	if ((hWndServer!=NULL)&&(IsWindow(hWndServer)))
	{
		m_hWndServer=hWndServer;
		return true;
	}

	return false;
}

//���ô���
bool CProcessSendData::SetServerHwnd(LPCTSTR pszClassName)
{
	//Ч�����
	ASSERT(pszClassName!=NULL);
	if (pszClassName==NULL) return false;

	//���ñ���
	m_hWndServer=FindWindow(pszClassName,NULL);

	return (m_hWndServer!=NULL);
}

//��������
bool CProcessSendData::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч�����
	ASSERT(IsWindow(m_hWndServer));
	if (IsWindow(m_hWndServer)==FALSE) return false;

	//��������
	IPC_Head IPCHead;
	IPCHead.wVersion=IPC_VER;
	IPCHead.wSubCmdID=wSubCmdID;
	IPCHead.wMainCmdID=wMainCmdID;
	IPCHead.wPacketSize=sizeof(IPC_Head);

	//��������
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCHead;
	CopyDataStruct.dwData=IDT_ASYNCHRONISM;
	CopyDataStruct.cbData=IPCHead.wPacketSize;
	::SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//��������
bool CProcessSendData::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(IsWindow(m_hWndServer));
	if (IsWindow(m_hWndServer)==FALSE) return false;

	//��������
	IPC_Buffer IPCBuffer;
	IPCBuffer.Head.wVersion=IPC_VER;
	IPCBuffer.Head.wSubCmdID=wSubCmdID;
	IPCBuffer.Head.wMainCmdID=wMainCmdID;
	IPCBuffer.Head.wPacketSize=sizeof(IPCBuffer.Head);

	//��������
	if (pData!=NULL)
	{
		//Ч�鳤��
		ASSERT(wDataSize<=sizeof(IPCBuffer.cbBuffer));
		if (wDataSize>sizeof(IPCBuffer.cbBuffer)) return false;

		//��������
		IPCBuffer.Head.wPacketSize+=wDataSize;
		CopyMemory(IPCBuffer.cbBuffer,pData,wDataSize);
	}

	//��������
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCBuffer;
	CopyDataStruct.dwData=IDT_ASYNCHRONISM;
	CopyDataStruct.cbData=IPCBuffer.Head.wPacketSize;
	::SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//ִ������
bool CProcessSendData::TransferCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(IsWindow(m_hWndServer));
	if (IsWindow(m_hWndServer)==FALSE) return false;

	//��������
	IPC_Buffer IPCBuffer;
	IPCBuffer.Head.wVersion=IPC_VER;
	IPCBuffer.Head.wSubCmdID=wSubCmdID;
	IPCBuffer.Head.wMainCmdID=wMainCmdID;
	IPCBuffer.Head.wPacketSize=sizeof(IPCBuffer.Head);

	//��������
	if (pData!=NULL)
	{
		//Ч�鳤��
		ASSERT(wDataSize<=sizeof(IPCBuffer.cbBuffer));
		if (wDataSize>sizeof(IPCBuffer.cbBuffer)) return false;

		//��������
		IPCBuffer.Head.wPacketSize+=wDataSize;
		CopyMemory(IPCBuffer.cbBuffer,pData,wDataSize);
	}

	//��������
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCBuffer;
	CopyDataStruct.dwData=IDT_IMMEDIATE;
	CopyDataStruct.cbData=IPCBuffer.Head.wPacketSize;
	::SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CProcessRecvData::CProcessRecvData()
{
	//���ñ���
	m_pIIPCRecvDataSink=NULL;

	return;
}

//��������
CProcessRecvData::~CProcessRecvData()
{
}

//���ýӿ�
bool CProcessRecvData::SetProcessTradeSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IIPCRecvDataSink)!=NULL);
		m_pIIPCRecvDataSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IIPCRecvDataSink);

		//�ɹ��ж�
		if (m_pIIPCRecvDataSink==NULL) return false;
	}
	else m_pIIPCRecvDataSink=NULL;

	return true;
}

//��Ϣ����
bool CProcessRecvData::OnCopyData(HWND hWndSend, COPYDATASTRUCT * pCopyDataStruct)
{
	//Ч��ӿ�
	ASSERT(m_pIIPCRecvDataSink!=NULL);
	if (m_pIIPCRecvDataSink==NULL) return false;

	//Ч�����
	ASSERT(pCopyDataStruct->cbData>=sizeof(IPC_Head));
	if (pCopyDataStruct->cbData<sizeof(IPC_Head)) return false;

	//Ч������
	IPC_Head * pHead=(IPC_Head *)pCopyDataStruct->lpData;
	ASSERT((pHead->wVersion==IPC_VER)&&(pHead->wPacketSize==pCopyDataStruct->cbData));
	if ((pHead->wVersion!=IPC_VER)||(pHead->wPacketSize!=pCopyDataStruct->cbData)) return false;

	//���ݴ���
	switch (pCopyDataStruct->dwData)
	{
	case IDT_IMMEDIATE:			//ͬ������
		{
			//���ݴ���
			WORD wDataSize=pHead->wPacketSize-sizeof(IPC_Head);
			m_pIIPCRecvDataSink->OnImmediateData(pHead->wMainCmdID,pHead->wSubCmdID,pHead+1,wDataSize,hWndSend);

			return true;
		}
	case IDT_ASYNCHRONISM:		//�첽����
		{
			//���ݴ���
			WORD wDataSize=pHead->wPacketSize-sizeof(IPC_Head);
			m_pIIPCRecvDataSink->OnAsynchronismData(pHead->wMainCmdID,pHead->wSubCmdID,pHead+1,wDataSize,hWndSend);

			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
