#include "StdAfx.h"
#include "TraceService.h"
#include "WEBSocketService.h"


//�궨��
#define ID_SOCKET_WND				10								//SOCKET ���� ID

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWEBSocketService, CWnd)
END_MESSAGE_MAP()

//��������
//////////////////////////////////////////////////////////////////////////
//���캯��
CWEBSocketService::CWEBSocketService(void)
{

}

//��������
CWEBSocketService::~CWEBSocketService(void)
{

}

//�����ӿ�
//////////////////////////////////////////////////////////////////////////
//�ӿڲ�ѯ
void * CWEBSocketService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWEBSocketService, Guid, dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWEBSocketService, Guid, dwQueryVer);
	return NULL;
}

//����ӿ�
//////////////////////////////////////////////////////////////////////////
//��������
bool CWEBSocketService::StartService()
{
	return true;
}

//ֹͣ����
bool CWEBSocketService::ConcludeService()
{
	return true;
}

//���нӿ�
//////////////////////////////////////////////////////////////////////////
//֪ͨ�ص�
void CWEBSocketService::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)
{

}

//���ýӿ�
//////////////////////////////////////////////////////////////////////////
//���ú���
bool CWEBSocketService::SetServiceID(WORD wServiceID)
{
	return true;
}

//���ýӿ�
bool CWEBSocketService::SetWEBSocketEvent(IUnknownEx * pIUnknownEx)
{
	return true;
}

//���ܽӿ�
//////////////////////////////////////////////////////////////////////////
//�ر�����
bool CWEBSocketService::CloseSocket(BYTE cbShutReason)
{
	return true;
}

//���ӵ�ַ
bool CWEBSocketService::ConnectServer(LPCTSTR pszURL, WORD wPort)
{
	return true;
}

//���ӵ�ַ
bool CWEBSocketService::SendRequestData(VOID * pData, WORD wDataSize)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
//����������
extern "C" __declspec(dllexport) void * CreateWEBSocketService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CWEBSocketService * pWEBSocketService = NULL;
	try
	{
		pWEBSocketService = new CWEBSocketService();
		if (pWEBSocketService == NULL) throw TEXT("����ʧ��");
		void * pObject = pWEBSocketService->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pWEBSocketService);
	return NULL;
}