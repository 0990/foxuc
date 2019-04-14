#ifndef WH_COMMAND_LINE_HEAD_FILE
#define WH_COMMAND_LINE_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////

//命令解释
class SERVICE_CORE_CLASS CWHCommandLine
{
	//函数定义
public:
	//构造函数
	CWHCommandLine();
	//析构函数
	virtual ~CWHCommandLine();

	//功能函数
public:
	//查询命令
	bool SearchCommandItem(LPCTSTR pszCommandLine, LPCTSTR pszCommand, TCHAR szParameter[], WORD wParameterLen);
};

//////////////////////////////////////////////////////////////////////////////////

#endif