#include "StdAfx.h"
#include "WHCommandLine.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWHCommandLine::CWHCommandLine()
{
}

//析构函数
CWHCommandLine::~CWHCommandLine()
{
}

//查询命令
bool CWHCommandLine::SearchCommandItem(LPCTSTR pszCommandLine, LPCTSTR pszCommand, TCHAR szParameter[], WORD wParameterLen)
{
	//效验参数
	ASSERT((pszCommandLine!=NULL)&&(pszCommand!=NULL));
	if ((pszCommandLine==NULL)||(pszCommand==NULL)) return false;

	//参数处理
	if (pszCommandLine[0]!=0)
	{
		//变量定义
		UINT nCommandLen=lstrlen(pszCommand);
		LPCTSTR lpszBeginString=pszCommandLine;

		//提取参数
		while (true)
		{
			//参数分析
			LPCTSTR lpszEndString=_tcschr(lpszBeginString,TEXT(' '));
			UINT nStringLength=(lpszEndString==NULL)?lstrlen(lpszBeginString):(UINT)(lpszEndString-lpszBeginString);

			//命令分析
			if ((nStringLength>=nCommandLen)&&(memcmp(lpszBeginString,pszCommand,nCommandLen*sizeof(TCHAR))==0))
			{
				//长度效验
				ASSERT(wParameterLen>(nStringLength-nCommandLen));
				if ((wParameterLen<=(nStringLength-nCommandLen))) return false;

				//提取参数
				szParameter[nStringLength-nCommandLen]=0;
				CopyMemory(szParameter,lpszBeginString+nCommandLen,(nStringLength-nCommandLen)*sizeof(TCHAR));

				return true;
			}

			//设置变量
			if (lpszEndString==NULL) break;
			lpszBeginString=(lpszEndString+1);
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
