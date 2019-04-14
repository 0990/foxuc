#ifndef PROCESS_EXCEPTION_HEAD_FILE
#define PROCESS_EXCEPTION_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�쳣����
class KERNEL_ENGINE_CLASS CExceptionHandle : public CDialog
{
	//��Ԫ����
	friend class CExceptionManager;

	//��������
protected:
	CString							m_strFileName;						//�ļ�����
	EXCEPTION_POINTERS *			m_lpExceptionInfo;					//�쳣��Ϣ

	//�ؼ�����
protected:
	CRichEditCtrl					m_RichEditCtrl;						//��Ϣ�ؼ�

	//��������
public:
	//���캯��
	CExceptionHandle();
	//��������
	virtual ~CExceptionHandle();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();

	//��ť��Ϣ
protected:
	//���԰�ť
	VOID OnBnClickedDebug();
	//���ͱ���
	VOID OnBnClickedSendReport();

	//��������
protected:
	//�����ִ�
	VOID InsertString(LPCTSTR pszString);
	//�����ļ�
	VOID SaveExceptionInfo(EXCEPTION_POINTERS * lpExceptionInfo);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�����쳣
class KERNEL_ENGINE_CLASS CExceptionManager
{
	//��������
protected:
	static CExceptionManager *		m_pExceptionManager;				//�쳣����

	//��������
public:
	//���캯��
	CExceptionManager();
	//��������
	virtual ~CExceptionManager();

	//ȫ�ֺ���
public:
	//�쳣����
	static LONG WINAPI ProcessExceptionFilter(EXCEPTION_POINTERS * lpExceptionInfo);
};

//////////////////////////////////////////////////////////////////////////////////

#endif