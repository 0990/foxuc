#include "StdAfx.h"
#include "WHCommandLine.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWHCommandLine::CWHCommandLine()
{
}

//��������
CWHCommandLine::~CWHCommandLine()
{
}

//��ѯ����
bool CWHCommandLine::SearchCommandItem(LPCTSTR pszCommandLine, LPCTSTR pszCommand, TCHAR szParameter[], WORD wParameterLen)
{
	//Ч�����
	ASSERT((pszCommandLine!=NULL)&&(pszCommand!=NULL));
	if ((pszCommandLine==NULL)||(pszCommand==NULL)) return false;

	//��������
	if (pszCommandLine[0]!=0)
	{
		//��������
		UINT nCommandLen=lstrlen(pszCommand);
		LPCTSTR lpszBeginString=pszCommandLine;

		//��ȡ����
		while (true)
		{
			//��������
			LPCTSTR lpszEndString=_tcschr(lpszBeginString,TEXT(' '));
			UINT nStringLength=(lpszEndString==NULL)?lstrlen(lpszBeginString):(UINT)(lpszEndString-lpszBeginString);

			//�������
			if ((nStringLength>=nCommandLen)&&(memcmp(lpszBeginString,pszCommand,nCommandLen*sizeof(TCHAR))==0))
			{
				//����Ч��
				ASSERT(wParameterLen>(nStringLength-nCommandLen));
				if ((wParameterLen<=(nStringLength-nCommandLen))) return false;

				//��ȡ����
				szParameter[nStringLength-nCommandLen]=0;
				CopyMemory(szParameter,lpszBeginString+nCommandLen,(nStringLength-nCommandLen)*sizeof(TCHAR));

				return true;
			}

			//���ñ���
			if (lpszEndString==NULL) break;
			lpszBeginString=(lpszEndString+1);
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
