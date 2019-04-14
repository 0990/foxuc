#include "StdAfx.h"
#include "DataBaseAide.h"
#include "DataBase.h"

//构造函数
CDataBaseAide::CDataBaseAide(IUnknownEx * pIUnknownEx)
{
}
//析构函数
CDataBaseAide::~CDataBaseAide()
{
}

bool CDataBaseAide::SetDataBase(IUnknownEx *pIUnKnownEx)
{
	m_pIDataBase = (IDataBase *)pIUnKnownEx;
	if (m_pIDataBase != NULL)
		return true;
	else
		return false;
}

VOID *CDataBaseAide::GetDataBase(REFGUID Guid, DWORD dwQueryVer)
{
	if (m_pIDataBase != NULL)
		return m_pIDataBase;
	return NULL;
}
/////////////////////////////////
////取数据

INT CDataBaseAide::GetValue_INT(LPCTSTR pszItem)
{
	INT nValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, nValue);
		return nValue;
	}
	return 0;
}

UINT CDataBaseAide::GetValue_UINT(LPCTSTR pszItem)
{
	UINT ulValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, ulValue);
		return ulValue;

	}
	return 0;
}

DOUBLE CDataBaseAide::GetValue_DOUBLE(LPCTSTR pszItem)
{
	DOUBLE  dbValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, dbValue);
		return dbValue;

	}
	return 0;
}

BYTE CDataBaseAide::GetValue_BYTE(LPCTSTR pszItem)
{
	BYTE  bValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, bValue);
		return bValue;
	}

	return 0;
}

LONG CDataBaseAide::GetValue_LONG(LPCTSTR pszItem)
{
	LONG  lValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, lValue);
		return lValue;

	}
	return 0;

}

WORD CDataBaseAide::GetValue_WORD(LPCTSTR pszItem)
{
	WORD  wValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, wValue);
		return wValue;

	}
	return 0;

}

DWORD CDataBaseAide::GetValue_DWORD(LPCTSTR pszItem)
{
	DWORD  ulValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, ulValue);
		return ulValue;

	}
	return 0;
}

FLOAT CDataBaseAide::GetValue_FLOAT(LPCTSTR pszItem)
{
	return (FLOAT)GetValue_DOUBLE(pszItem);
}

LONGLONG CDataBaseAide::GetValue_LONGLONG(LPCTSTR pszItem)
{
	__int64 llValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, llValue);
		return llValue;

	}
	return 0;
}

VOID CDataBaseAide::GetValue_VarValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	ASSERT(FALSE);
	return;
}

VOID CDataBaseAide::GetValue_SystemTime(LPCTSTR pszItem, SYSTEMTIME & SystemTime)
{
	COleDateTime Time;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, Time);
		Time.GetAsSystemTime(SystemTime);
	}
	return;
}


//获取参数
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetParameter(pszItem, DBVarValue);
	}
}

//获取参数
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, LPSTR pszString, UINT uSize)
{
	ASSERT(pszString!=NULL);

	if (m_pIDataBase != NULL && pszString != NULL)
	{
		CDBVarValue DBVarValue;
		((CDataBase*)m_pIDataBase)->GetParameter(pszItem, DBVarValue);
		lstrcpynA(pszString, (LPCSTR)_bstr_t(DBVarValue), uSize);
	}
}

//获取参数
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, LPWSTR pszString, UINT uSize)
{
	ASSERT(pszString!=NULL);

	if (m_pIDataBase != NULL && pszString != NULL)
	{
		CDBVarValue DBVarValue;
		((CDataBase*)m_pIDataBase)->GetParameter(pszItem, DBVarValue);
		lstrcpynW(pszString, (LPCWSTR)_bstr_t(DBVarValue), uSize);
	}
}

//重置参数
VOID CDataBaseAide::ResetParameter()
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->ClearParameters();
		((CDataBase*)m_pIDataBase)->AddParameter(TEXT("RETURN_VALUE"), adInteger, adParamReturnValue, sizeof(long), _variant_t((long)0));
	}

}

//获取字符
VOID CDataBaseAide::GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, pszString, uMaxCount);
	}
}

//获取字符
VOID CDataBaseAide::GetValue_String(LPCTSTR pszItem, LPWSTR pszString, UINT uMaxCount)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, pszString, uMaxCount);
	}
}

//返回数值
LONG CDataBaseAide::GetReturnValue()
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetReturnValue();
	}
	return 0;
}

//存储过程
LONG CDataBaseAide::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->ExecuteProcess(pszSPName, bRecordset);
		return ((CDataBase*)m_pIDataBase)->GetReturnValue();
	}
	return -1;

}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, INT nValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(int), _variant_t((int)nValue));
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, UINT uValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adUnsignedInt, adParamInput, sizeof(unsigned int), _variant_t((unsigned int)uValue));
	}

}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LONG lValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)lValue));
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LONGLONG lValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adBigInt, adParamInput, sizeof(__int64), _variant_t((__int64)lValue));
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, BYTE cbValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adTinyInt, adParamInput, sizeof(BYTE), _variant_t((BYTE)cbValue));
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, WORD wValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adUnsignedSmallInt, adParamInput, sizeof(unsigned short), _variant_t((unsigned short)wValue));
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, DWORD dwValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adUnsignedInt, adParamInput, sizeof(unsigned long), _variant_t((unsigned long)dwValue));
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, FLOAT fValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adSingle, adParamInput, sizeof(float), _variant_t((float)fValue));
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, DOUBLE dValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adDouble, adParamInput, sizeof(double), _variant_t((double)dValue));
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		if (lstrlenA(pszString) != 0)
		{
			((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adChar, adParamInput, lstrlenA(pszString), _variant_t(pszString));
		}
		else
		{
			((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adChar, adParamInput, lstrlenA(pszString) + 1, _variant_t(pszString));
		}
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCWSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		if (lstrlenW(pszString) != 0)
		{
			((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adWChar, adParamInput, lstrlenW(pszString), _variant_t(pszString));
		}
		else
		{
			((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adWChar, adParamInput, lstrlenW(pszString) + 1, _variant_t(pszString));
		}
	}
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, SYSTEMTIME & SystemTime, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		DOUBLE dbValue = 0.0;
		SystemTimeToVariantTime(&SystemTime, &dbValue);
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adDBDate, adParamInput, sizeof(DATE), _variant_t((DATE)dbValue));
	}
}

#if 0  //
//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCWSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adParamInput, adVarChar, lstrlen(pszString), _variant_t(pszString));
	}
}
#endif

//插入参数
VOID CDataBaseAide::AddParameterOutput(LPCTSTR pszItem, LPSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adVarChar, ParameterDirection, uSize, _variant_t(pszString));
	}
}
//插入参数
VOID CDataBaseAide::AddParameterOutput(LPCTSTR pszItem, LPWSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adVarWChar, ParameterDirection, uSize, _variant_t(pszString));
	}
}












