#ifndef PROCESS_TRADE_HEAD_FILE
#define PROCESS_TRADE_HEAD_FILE

#pragma once

#include "ProcessControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//����ͨѶ
class PROCESS_CONTROL_CLASS CProcessControl : public CWnd, public IProcessControl
{
	//�������
protected:
	CWHDataQueue					m_DataQueue;						//���ݶ���
	IProcessControlSink *			m_pIProcessControlSink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CProcessControl();
	//��������
	virtual ~CProcessControl();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��ȡ���
	virtual HWND GetProcessControlWnd();
	//����ͨѶ
	virtual bool SetProcessControlSink(IUnknownEx * pIUnknownEx);

	//���ӹ���
public:
	//��������
	virtual bool InitializeLink();
	//����ͨѶ
	virtual bool UnInitializeLink();

	//���ͽӿ�
public:
	//��������
	virtual bool SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID);
	//��������
	virtual bool SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);
	//ִ������
	virtual bool SendCommand(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	//��Ϣӳ��
protected:
	//IPC ��Ϣ
	BOOL OnCopyData(CWnd * pWnd, COPYDATASTRUCT * pCopyDataStruct);
	//�첽����
	LRESULT	OnMessageAsynchronismData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif