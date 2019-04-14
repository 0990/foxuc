#include "StdAfx.h"
#include "ProcessControl.h"

//////////////////////////////////////////////////////////////////////////////////
//�궨��

//��ʶ����
#define IDC_PROCESS_CONTROL			100									//���ڱ�ʶ

//��Ϣ����
#define WM_ASYNCHRONISM_DATA		(WM_USER+100)						//�첽����

//////////////////////////////////////////////////////////////////////////////////

//�첽����
struct tagAsynchronismData
{
	HWND							hWndSend;							//���ʹ���
	WORD							wDataSize;							//���ݴ�С
	WORD							wSubCmdID;							//��������
	WORD							wMainCmdID;							//��������
	BYTE							cbBuffer[IPC_PACKET];				//���ݻ���
};

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CProcessControl, CWnd)
	ON_WM_COPYDATA()
	ON_MESSAGE(WM_ASYNCHRONISM_DATA,OnMessageAsynchronismData)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CProcessControl::CProcessControl()
{
	//���ñ���
	m_pIProcessControlSink=NULL;

	return;
}

//��������
CProcessControl::~CProcessControl()
{
}

//�ӿڲ�ѯ
VOID * CProcessControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IProcessControl,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IProcessControl,Guid,dwQueryVer);
	return NULL;
}

//��ȡ���
HWND CProcessControl::GetProcessControlWnd()
{
	return m_hWnd;
}

//����ͨѶ
bool CProcessControl::SetProcessControlSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessControlSink)!=NULL);
		m_pIProcessControlSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessControlSink);

		//�ɹ��ж�
		if (m_pIProcessControlSink==NULL) return false;
	}
	else m_pIProcessControlSink=NULL;

	return true;
}

//��������
bool CProcessControl::InitializeLink()
{
	//Ч�����
	ASSERT(m_pIProcessControlSink!=NULL);
	if (m_pIProcessControlSink==NULL) return false;

	//��������
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),IDC_PROCESS_CONTROL);
	}

	return true;
}

//����ͨѶ
bool CProcessControl::UnInitializeLink()
{
	//�������
	m_DataQueue.RemoveData(false);

	//�رմ���
	if (m_hWnd!=NULL) DestroyWindow();

	return true;
}

//��������
bool CProcessControl::SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч�����
	if (IsWindow(m_hWnd)==FALSE) return false;
	if (IsWindow(hWndServer)==FALSE) return false;

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
	::SendMessage(hWndServer,WM_COPYDATA,(LPARAM)m_hWnd,(WPARAM)&CopyDataStruct);

	return true;
}

//��������
bool CProcessControl::SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)
{
	//Ч�����
	if (IsWindow(m_hWnd)==FALSE) return false;
	if (IsWindow(hWndServer)==FALSE) return false;

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
	::SendMessage(hWndServer,WM_COPYDATA,(LPARAM)m_hWnd,(WPARAM)&CopyDataStruct);

	return true;
}

//ִ������
bool CProcessControl::SendCommand(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)
{
	//Ч�����
	if (IsWindow(m_hWnd)==FALSE) return false;
	if (IsWindow(hWndServer)==FALSE) return false;

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
	::SendMessage(hWndServer,WM_COPYDATA,(LPARAM)m_hWnd,(WPARAM)&CopyDataStruct);

	return true;
}

//IPC ��Ϣ
BOOL CProcessControl::OnCopyData(CWnd * pWnd, COPYDATASTRUCT * pCopyDataStruct)
{
	//Ч��ӿ�
	ASSERT(m_pIProcessControlSink!=NULL);
	if (m_pIProcessControlSink==NULL) return FALSE;

	//Ч�����
	ASSERT(pCopyDataStruct->cbData>=sizeof(IPC_Head));
	if (pCopyDataStruct->cbData<sizeof(IPC_Head)) return FALSE;

	//Ч������
	IPC_Head * pHead=(IPC_Head *)pCopyDataStruct->lpData;
	ASSERT((pHead->wVersion==IPC_VER)&&(pHead->wPacketSize==pCopyDataStruct->cbData));
	if ((pHead->wVersion!=IPC_VER)||(pHead->wPacketSize!=pCopyDataStruct->cbData)) return FALSE;

	//���ݴ���
	switch (pCopyDataStruct->dwData)
	{
	case IDT_IMMEDIATE:			//ͬ������
		{
			//���ݴ���
			WORD wDataSize=pHead->wPacketSize-sizeof(IPC_Head);
			m_pIProcessControlSink->OnProcessControlData(pHead->wMainCmdID,pHead->wSubCmdID,pHead+1,wDataSize,pWnd->m_hWnd);

			return true;
		}
	case IDT_ASYNCHRONISM:		//�첽����
		{
			//��������
			tagAsynchronismData AsynchronismData;
			AsynchronismData.wSubCmdID=pHead->wSubCmdID;
			AsynchronismData.wMainCmdID=pHead->wMainCmdID;
			AsynchronismData.hWndSend=pWnd->GetSafeHwnd();
			AsynchronismData.wDataSize=pHead->wPacketSize-sizeof(IPC_Head);
			CopyMemory(&AsynchronismData.cbBuffer,pHead+1,pHead->wPacketSize-sizeof(IPC_Head));

			//��������
			WORD wHeadSize=sizeof(AsynchronismData)-sizeof(AsynchronismData.cbBuffer);
			m_DataQueue.InsertData(IDT_ASYNCHRONISM,&AsynchronismData,wHeadSize+AsynchronismData.wDataSize);

			//Ͷ����Ϣ
			ASSERT(pWnd!=NULL);
			if (pWnd!=NULL) PostMessage(WM_ASYNCHRONISM_DATA,(WPARAM)(pWnd->m_hWnd),(LPARAM)(pHead->wPacketSize));

			return true;
		}
	}

	return TRUE;
}

//֪ͨ��Ϣ
LRESULT	CProcessControl::OnMessageAsynchronismData(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	tagDataHead DataHead;
	tagAsynchronismData AsynchronismData;
	if (m_DataQueue.DistillData(DataHead,&AsynchronismData,sizeof(AsynchronismData))==false)
	{
		ASSERT(FALSE);
		return 0;
	}

	//״̬Ч��
	ASSERT(m_pIProcessControlSink!=NULL);
	if (m_pIProcessControlSink==NULL) return 0;

	//���ݴ���
	HWND hWndClient=AsynchronismData.hWndSend;
	WORD wSubCmdID=AsynchronismData.wSubCmdID;
	WORD wMainCmdID=AsynchronismData.wMainCmdID;
	m_pIProcessControlSink->OnProcessControlData(wMainCmdID,wSubCmdID,AsynchronismData.cbBuffer,AsynchronismData.wDataSize,hWndClient);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(ProcessControl);

//////////////////////////////////////////////////////////////////////////////////
