#ifndef WH_COMMAND_LINE_HEAD_FILE
#define WH_COMMAND_LINE_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
class SERVICE_CORE_CLASS CWHCommandLine
{
	//��������
public:
	//���캯��
	CWHCommandLine();
	//��������
	virtual ~CWHCommandLine();

	//���ܺ���
public:
	//��ѯ����
	bool SearchCommandItem(LPCTSTR pszCommandLine, LPCTSTR pszCommand, TCHAR szParameter[], WORD wParameterLen);
};

//////////////////////////////////////////////////////////////////////////////////

#endif