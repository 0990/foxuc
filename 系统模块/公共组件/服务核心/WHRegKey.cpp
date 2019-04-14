#include "StdAfx.h"
#include "WHRegKey.h"
#include "WHEncrypt.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

TCHAR		CWHRegKey::m_szMainKeyName[64]=TEXT("");					//主键名字

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWHRegKey::CWHRegKey()
{
	//键项变量
	m_hRegKey=NULL;

	//键名变量
	if (m_szMainKeyName[0]==0)
	{
		CWinApp * pWinApp=AfxGetApp();
		lstrcpyn(m_szMainKeyName,pWinApp->m_pszAppName,CountArray(m_szMainKeyName));
	}

	return;
}

//析构函数
CWHRegKey::~CWHRegKey()
{
	//关闭键项
	if (m_hRegKey!=NULL) CloseRegKey();

	return;
}

//设置主键
VOID CWHRegKey::SetMainKeyName(LPCTSTR pszMainKeyName)
{
	//设置变量
	ASSERT(pszMainKeyName!=NULL);
	lstrcpyn(m_szMainKeyName,pszMainKeyName,CountArray(m_szMainKeyName));

	return;
}

//接触绑定
HKEY CWHRegKey::Detach()
{
	//保存变量
	HKEY hRegKey=m_hRegKey;

	//设置变量
	m_hRegKey=NULL;

	return hRegKey;
}

//绑定键项
HKEY CWHRegKey::Attach(HKEY hRegKey)
{
	//效验参数
	ASSERT(hRegKey!=NULL);

	//设置变量
	m_hRegKey=hRegKey;

	return m_hRegKey;
}

//关闭键项
bool CWHRegKey::CloseRegKey()
{
	//关闭键项
	RegCloseKey(m_hRegKey);

	//设置变量
	m_hRegKey=NULL;

	return true;
}

//打开键项
bool CWHRegKey::OpenRegKey(LPCTSTR pszKeyName, bool bCreate)
{
	//效验参数
	ASSERT(pszKeyName!=NULL);

	//关闭键项
	if (m_hRegKey!=NULL) RegCloseKey(m_hRegKey);

	//生成键名
	TCHAR szFullKeyName[256]=TEXT("");
	_sntprintf(szFullKeyName,CountArray(szFullKeyName),TEXT("Software\\%s\\%s\\%s"),szProductKey,m_szMainKeyName,pszKeyName);

	//打开键项
	m_hRegKey=NULL;
	LONG lResult=RegOpenKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);
	if ((lResult!=ERROR_SUCCESS)&&(bCreate==true)) lResult=RegCreateKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);

	return (lResult==ERROR_SUCCESS);
}

//打开键项
bool CWHRegKey::OpenRegKey(LPCTSTR pszMainKeyName, LPCTSTR pszKeyName, bool bCreate)
{
	//效验参数
	ASSERT(pszKeyName!=NULL);
	ASSERT(pszMainKeyName!=NULL);

	//关闭键项
	if (m_hRegKey!=NULL) RegCloseKey(m_hRegKey);

	//生成键名
	TCHAR szFullKeyName[256]=TEXT("");
	_sntprintf(szFullKeyName,CountArray(szFullKeyName),TEXT("Software\\%s\\%s\\%s"),szProductKey,pszMainKeyName,pszKeyName);

	//打开键项
	m_hRegKey=NULL;
	LONG lResult=RegOpenKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);
	if ((lResult!=ERROR_SUCCESS)&&(bCreate==true)) lResult=RegCreateKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);

	return (lResult==ERROR_SUCCESS);
}

//打开键项
HKEY CWHRegKey::OpenItemKey(LPCTSTR pszSubKeyName)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);
	ASSERT(pszSubKeyName!=NULL);

	//打开键项
	HKEY hRegKey=NULL;
	RegOpenKey(m_hRegKey,pszSubKeyName,&hRegKey);

	return hRegKey;
}

//创建键项
HKEY CWHRegKey::CreateItemKey(LPCTSTR pszSubKeyName)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);
	ASSERT(pszSubKeyName!=NULL);

	//创建键项
	HKEY hRegKey=NULL;
	RegCreateKey(m_hRegKey,pszSubKeyName,&hRegKey);

	return hRegKey;
}

//枚举键项
bool CWHRegKey::EnumItemString(DWORD dwIndex, LPTSTR pszString, DWORD dwSize)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);

	//设置变量
	pszString[0]=0;

	//枚举键项
	LONG lResult=RegEnumKey(m_hRegKey,dwIndex,pszString,dwSize);

	return (lResult==ERROR_SUCCESS);
}

//写入数值
bool CWHRegKey::WriteValue(LPCTSTR pszName, DWORD dwValue)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);

	//写入数值
	LONG lResult=RegSetValueEx(m_hRegKey,pszName,0,REG_DWORD,(LPBYTE)(&dwValue),sizeof(DWORD));

	return (lResult==ERROR_SUCCESS);
}

//写入字符
bool CWHRegKey::WriteString(LPCTSTR pszName, LPCTSTR pszString)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);

	//写入数值
	DWORD dwDataSize=CountStringBuffer(pszString);
	LONG lResult=RegSetValueEx(m_hRegKey,pszName,0,REG_SZ,(LPBYTE)pszString,dwDataSize);

	return (lResult==ERROR_SUCCESS);
}

//写入字符
bool CWHRegKey::WriteEncryptString(LPCTSTR pszName, LPCTSTR pszString)
{
	//加密数据
	TCHAR szRegString[MAX_ENCRYPT_LEN+1]=TEXT("");
	CWHEncrypt::XorEncrypt(pszString,szRegString,CountArray(szRegString));

	//写入字符
	return WriteString(pszName,szRegString);
}

//写入数据
bool CWHRegKey::WriteBinary(LPCTSTR pszName, const VOID * pData, DWORD dwSize)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);

	//写入数值
	LONG lResult=RegSetValueEx(m_hRegKey,pszName,0,REG_BINARY,(LPBYTE)(pData),dwSize);

	return (lResult==ERROR_SUCCESS);
}

//读取数值
DWORD CWHRegKey::GetValue(LPCTSTR pszName, DWORD dwDefValue)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);

	//读取数值
	DWORD dwType=REG_NONE,dwResultSize=sizeof(DWORD),dwValue=0;
	LONG lResult=RegQueryValueEx(m_hRegKey,pszName,0,&dwType,(BYTE *)(&dwValue),&dwResultSize);
	if ((lResult!=ERROR_SUCCESS)||(dwType!=REG_DWORD)) return dwDefValue;

	return dwValue;
}

//读取数据
DWORD CWHRegKey::GetBinary(LPCTSTR pszName, VOID * pData, DWORD dwBufferSize)
{
	//效验参数
	ASSERT(pData!=NULL);
	ASSERT(m_hRegKey!=NULL);

	//读取数值
	DWORD dwType=REG_NONE,dwResultSize=dwBufferSize;
	LONG lResult=RegQueryValueEx(m_hRegKey,pszName,0,&dwType,(BYTE *)(pData),&dwResultSize);
	if ((lResult!=ERROR_SUCCESS)||(dwType!=REG_BINARY)) return 0;

	return dwResultSize;
}

//读取字符
DWORD CWHRegKey::GetString(LPCTSTR pszName, LPTSTR pszString, UINT uMaxCount)
{
	//效验参数
	ASSERT(pszString!=NULL);
	ASSERT(m_hRegKey!=NULL);

	//读取数值
	DWORD dwType=REG_NONE,dwResultSize=uMaxCount*sizeof(TCHAR);
	LONG lResult=RegQueryValueEx(m_hRegKey,pszName,0,&dwType,(BYTE *)(pszString),&dwResultSize);
	if ((lResult!=ERROR_SUCCESS)||(dwType!=REG_SZ)) return 0;

	return dwResultSize/sizeof(TCHAR);
}

//读取字符
DWORD CWHRegKey::GetEncryptString(LPCTSTR pszName, LPTSTR pszString, UINT uMaxCount)
{
	//效验参数
	ASSERT(pszString!=NULL);
	ASSERT(m_hRegKey!=NULL);

	//读取字符
	TCHAR szRegString[MAX_ENCRYPT_LEN+1]=TEXT("");
	GetString(pszName,szRegString,CountArray(szRegString));

	//解密数据
	CWHEncrypt::XorCrevasse(szRegString,pszString,uMaxCount);

	return lstrlen(pszString);
}

//删除数值
bool CWHRegKey::DeleteValue(LPCTSTR pszName)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);

	//删除数值
	LONG lResult=RegDeleteValue(m_hRegKey,pszName);

	return (lResult==ERROR_SUCCESS);
}

//删除键项
bool CWHRegKey::RecurseDeleteKey(LPCTSTR pszSubKeyName)
{
	//效验参数
	ASSERT(m_hRegKey!=NULL);
	ASSERT(pszSubKeyName!=NULL);

	//删除键项
	LONG lResult=RecurseDeleteKey(m_hRegKey,pszSubKeyName);

	return (lResult==ERROR_SUCCESS);
}

//删除键项
LONG CWHRegKey::RecurseDeleteKey(HKEY hParentKey, LPCTSTR pszSubKeyName)
{
	//效验参数
	ASSERT(hParentKey!=NULL);
	ASSERT(pszSubKeyName!=NULL);

	//打开键项
	HKEY hSubKey=NULL;
	LONG lResult=RegOpenKey(hParentKey,pszSubKeyName,&hSubKey);
	if (lResult!=ERROR_SUCCESS) return lResult;

	//递归删除
	TCHAR szKeyName[256];
	while (RegEnumKey(hSubKey,0,szKeyName,sizeof(szKeyName))==ERROR_SUCCESS)
	{
		//删除键项
		lResult=RecurseDeleteKey(hSubKey,szKeyName);
		if (lResult!=ERROR_SUCCESS)
		{
			RegCloseKey(hSubKey);
			return lResult;
		}
	}

	//关闭键项
	RegCloseKey(hSubKey);

	//删除键项
	lResult=RegDeleteKey(hParentKey,pszSubKeyName);

	return lResult;
}
//修复安装目录
void CWHRegKey::ResetInstallPath(LPCTSTR pszString)
{
	TCHAR szFullKeyName[256]=TEXT("");
	_sntprintf(szFullKeyName,CountArray(szFullKeyName),TEXT("Software\\%s\\%s\\%s"),szProductKey,_T("Plaza"),_T(""));

	//打开键项
	m_hRegKey=NULL;
	LONG lResult=RegOpenKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);
	if ((lResult!=ERROR_SUCCESS)) lResult=RegCreateKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);
	//写入数值
	DWORD dwDataSize=CountStringBuffer(pszString);
	RegSetValueEx(m_hRegKey,_T("Path"),0,REG_SZ,(LPBYTE)pszString,dwDataSize);
}

//////////////////////////////////////////////////////////////////////////////////
