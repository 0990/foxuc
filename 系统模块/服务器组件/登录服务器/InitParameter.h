#ifndef INIR_PARAMETER_HEAD_FILE
#define INIR_PARAMETER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//���ò���
class CInitParameter
{
	//ϵͳ����
public:
	BYTE							m_cbDelayList;						//��ʱ�б�
	WORD							m_wMaxConnect;						//�������
	WORD							m_wServicePort;						//����˿�
	TCHAR							m_szServerName[LEN_SERVER];			//��������

	//���ʱ��
public:
	WORD							m_wConnectTime;						//����ʱ��
	WORD							m_wCollectTime;						//ͳ��ʱ��
	WORD							m_wLoadListTime;					//�б�ʱ��
	WORD							m_wReLoadListTime;					//�б�ʱ��

	//�ͻ�ʱ��
public:
	WORD							m_wIntermitTime;					//�ж�ʱ��
	WORD							m_wOnLineCountTime;					//����ʱ��

	//Э����Ϣ
public:
	WORD							m_wCorrespondPort;					//Э���˿�
	tagAddressInfo					m_CorrespondAddress;				//Э����ַ

	//�����ַ
public:
	tagAddressInfo					m_ServiceAddress;					//�����ַ
	tagDataBaseParameter			m_AccountsDBParameter;				//���ӵ�ַ
	tagDataBaseParameter			m_TreasureDBParameter;				//���ӵ�ַ
	tagDataBaseParameter			m_PlatformDBParameter;				//���ӵ�ַ

	//��������
public:
	//���캯��
	CInitParameter();
	//��������
	virtual ~CInitParameter();

	//���ܺ���
public:
	//��ʼ��
	VOID InitParameter();
	//��������
	VOID LoadInitParameter();
};

//////////////////////////////////////////////////////////////////////////////////

#endif
