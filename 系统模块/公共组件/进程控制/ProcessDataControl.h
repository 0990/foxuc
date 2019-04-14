#ifndef PROCESS_DATA_CONTROL_HEAD_FILE
#define PROCESS_DATA_CONTROL_HEAD_FILE

#pragma once

#include "ProcessControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//���ݷ���
class PROCESS_CONTROL_CLASS CProcessSendData
{
	//��������
protected:
	HWND							m_hWndServer;						//���մ���
	HWND							m_hWndClient;						//���ʹ���

	//��������
public:
	//���캯��
	CProcessSendData(HWND hWndServer=NULL, HWND hWndClient=NULL);
	//��������
	virtual ~CProcessSendData();

	//��Ϣ����
public:
	//�Ƿ�����
	bool IsConnect();
	//��ȡ����
	HWND GetClientHwnd();
	//��ȡ����
	HWND GetServerHwnd();

	//����ӿ�
public:
	//���ô���
	bool SetClientHwnd(HWND hWndClient);
	//���ô���
	bool SetServerHwnd(HWND hWndServer);
	//���ô���
	bool SetServerHwnd(LPCTSTR pszClassName);

	//���ͺ���
public:
	//��������
	bool SendData(WORD wMainCmdID, WORD wSubCmdID);
	//��������
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//ִ������
	bool TransferCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

//���ݽ���
class PROCESS_CONTROL_CLASS CProcessRecvData
{
	//��������
protected:
	IIPCRecvDataSink *				m_pIIPCRecvDataSink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CProcessRecvData();
	//��������
	virtual ~CProcessRecvData();

	//�ӿں���
public:
	//���ýӿ�
	bool SetProcessTradeSink(IUnknownEx * pIUnknownEx);
	//��Ϣ����
	bool OnCopyData(HWND hWndSend, COPYDATASTRUCT * pCopyDataStruct);
};

//////////////////////////////////////////////////////////////////////////////////

#endif
