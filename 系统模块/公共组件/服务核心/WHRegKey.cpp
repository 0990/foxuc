#include "StdAfx.h"
#include "WHRegKey.h"
#include "WHEncrypt.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

TCHAR		CWHRegKey::m_szMainKeyName[64]=TEXT("");					//��������

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWHRegKey::CWHRegKey()
{
	//�������
	m_hRegKey=NULL;

	//��������
	if (m_szMainKeyName[0]==0)
	{
		CWinApp * pWinApp=AfxGetApp();
		lstrcpyn(m_szMainKeyName,pWinApp->m_pszAppName,CountArray(m_szMainKeyName));
	}

	return;
}

//��������
CWHRegKey::~CWHRegKey()
{
	//�رռ���
	if (m_hRegKey!=NULL) CloseRegKey();

	return;
}

//��������
VOID CWHRegKey::SetMainKeyName(LPCTSTR pszMainKeyName)
{
	//���ñ���
	ASSERT(pszMainKeyName!=NULL);
	lstrcpyn(m_szMainKeyName,pszMainKeyName,CountArray(m_szMainKeyName));

	return;
}

//�Ӵ���
HKEY CWHRegKey::Detach()
{
	//�������
	HKEY hRegKey=m_hRegKey;

	//���ñ���
	m_hRegKey=NULL;

	return hRegKey;
}

//�󶨼���
HKEY CWHRegKey::Attach(HKEY hRegKey)
{
	//Ч�����
	ASSERT(hRegKey!=NULL);

	//���ñ���
	m_hRegKey=hRegKey;

	return m_hRegKey;
}

//�رռ���
bool CWHRegKey::CloseRegKey()
{
	//�رռ���
	RegCloseKey(m_hRegKey);

	//���ñ���
	m_hRegKey=NULL;

	return true;
}

//�򿪼���
bool CWHRegKey::OpenRegKey(LPCTSTR pszKeyName, bool bCreate)
{
	//Ч�����
	ASSERT(pszKeyName!=NULL);

	//�رռ���
	if (m_hRegKey!=NULL) RegCloseKey(m_hRegKey);

	//���ɼ���
	TCHAR szFullKeyName[256]=TEXT("");
	_sntprintf(szFullKeyName,CountArray(szFullKeyName),TEXT("Software\\%s\\%s\\%s"),szProductKey,m_szMainKeyName,pszKeyName);

	//�򿪼���
	m_hRegKey=NULL;
	LONG lResult=RegOpenKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);
	if ((lResult!=ERROR_SUCCESS)&&(bCreate==true)) lResult=RegCreateKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);

	return (lResult==ERROR_SUCCESS);
}

//�򿪼���
bool CWHRegKey::OpenRegKey(LPCTSTR pszMainKeyName, LPCTSTR pszKeyName, bool bCreate)
{
	//Ч�����
	ASSERT(pszKeyName!=NULL);
	ASSERT(pszMainKeyName!=NULL);

	//�رռ���
	if (m_hRegKey!=NULL) RegCloseKey(m_hRegKey);

	//���ɼ���
	TCHAR szFullKeyName[256]=TEXT("");
	_sntprintf(szFullKeyName,CountArray(szFullKeyName),TEXT("Software\\%s\\%s\\%s"),szProductKey,pszMainKeyName,pszKeyName);

	//�򿪼���
	m_hRegKey=NULL;
	LONG lResult=RegOpenKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);
	if ((lResult!=ERROR_SUCCESS)&&(bCreate==true)) lResult=RegCreateKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);

	return (lResult==ERROR_SUCCESS);
}

//�򿪼���
HKEY CWHRegKey::OpenItemKey(LPCTSTR pszSubKeyName)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);
	ASSERT(pszSubKeyName!=NULL);

	//�򿪼���
	HKEY hRegKey=NULL;
	RegOpenKey(m_hRegKey,pszSubKeyName,&hRegKey);

	return hRegKey;
}

//��������
HKEY CWHRegKey::CreateItemKey(LPCTSTR pszSubKeyName)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);
	ASSERT(pszSubKeyName!=NULL);

	//��������
	HKEY hRegKey=NULL;
	RegCreateKey(m_hRegKey,pszSubKeyName,&hRegKey);

	return hRegKey;
}

//ö�ټ���
bool CWHRegKey::EnumItemString(DWORD dwIndex, LPTSTR pszString, DWORD dwSize)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);

	//���ñ���
	pszString[0]=0;

	//ö�ټ���
	LONG lResult=RegEnumKey(m_hRegKey,dwIndex,pszString,dwSize);

	return (lResult==ERROR_SUCCESS);
}

//д����ֵ
bool CWHRegKey::WriteValue(LPCTSTR pszName, DWORD dwValue)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);

	//д����ֵ
	LONG lResult=RegSetValueEx(m_hRegKey,pszName,0,REG_DWORD,(LPBYTE)(&dwValue),sizeof(DWORD));

	return (lResult==ERROR_SUCCESS);
}

//д���ַ�
bool CWHRegKey::WriteString(LPCTSTR pszName, LPCTSTR pszString)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);

	//д����ֵ
	DWORD dwDataSize=CountStringBuffer(pszString);
	LONG lResult=RegSetValueEx(m_hRegKey,pszName,0,REG_SZ,(LPBYTE)pszString,dwDataSize);

	return (lResult==ERROR_SUCCESS);
}

//д���ַ�
bool CWHRegKey::WriteEncryptString(LPCTSTR pszName, LPCTSTR pszString)
{
	//��������
	TCHAR szRegString[MAX_ENCRYPT_LEN+1]=TEXT("");
	CWHEncrypt::XorEncrypt(pszString,szRegString,CountArray(szRegString));

	//д���ַ�
	return WriteString(pszName,szRegString);
}

//д������
bool CWHRegKey::WriteBinary(LPCTSTR pszName, const VOID * pData, DWORD dwSize)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);

	//д����ֵ
	LONG lResult=RegSetValueEx(m_hRegKey,pszName,0,REG_BINARY,(LPBYTE)(pData),dwSize);

	return (lResult==ERROR_SUCCESS);
}

//��ȡ��ֵ
DWORD CWHRegKey::GetValue(LPCTSTR pszName, DWORD dwDefValue)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);

	//��ȡ��ֵ
	DWORD dwType=REG_NONE,dwResultSize=sizeof(DWORD),dwValue=0;
	LONG lResult=RegQueryValueEx(m_hRegKey,pszName,0,&dwType,(BYTE *)(&dwValue),&dwResultSize);
	if ((lResult!=ERROR_SUCCESS)||(dwType!=REG_DWORD)) return dwDefValue;

	return dwValue;
}

//��ȡ����
DWORD CWHRegKey::GetBinary(LPCTSTR pszName, VOID * pData, DWORD dwBufferSize)
{
	//Ч�����
	ASSERT(pData!=NULL);
	ASSERT(m_hRegKey!=NULL);

	//��ȡ��ֵ
	DWORD dwType=REG_NONE,dwResultSize=dwBufferSize;
	LONG lResult=RegQueryValueEx(m_hRegKey,pszName,0,&dwType,(BYTE *)(pData),&dwResultSize);
	if ((lResult!=ERROR_SUCCESS)||(dwType!=REG_BINARY)) return 0;

	return dwResultSize;
}

//��ȡ�ַ�
DWORD CWHRegKey::GetString(LPCTSTR pszName, LPTSTR pszString, UINT uMaxCount)
{
	//Ч�����
	ASSERT(pszString!=NULL);
	ASSERT(m_hRegKey!=NULL);

	//��ȡ��ֵ
	DWORD dwType=REG_NONE,dwResultSize=uMaxCount*sizeof(TCHAR);
	LONG lResult=RegQueryValueEx(m_hRegKey,pszName,0,&dwType,(BYTE *)(pszString),&dwResultSize);
	if ((lResult!=ERROR_SUCCESS)||(dwType!=REG_SZ)) return 0;

	return dwResultSize/sizeof(TCHAR);
}

//��ȡ�ַ�
DWORD CWHRegKey::GetEncryptString(LPCTSTR pszName, LPTSTR pszString, UINT uMaxCount)
{
	//Ч�����
	ASSERT(pszString!=NULL);
	ASSERT(m_hRegKey!=NULL);

	//��ȡ�ַ�
	TCHAR szRegString[MAX_ENCRYPT_LEN+1]=TEXT("");
	GetString(pszName,szRegString,CountArray(szRegString));

	//��������
	CWHEncrypt::XorCrevasse(szRegString,pszString,uMaxCount);

	return lstrlen(pszString);
}

//ɾ����ֵ
bool CWHRegKey::DeleteValue(LPCTSTR pszName)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);

	//ɾ����ֵ
	LONG lResult=RegDeleteValue(m_hRegKey,pszName);

	return (lResult==ERROR_SUCCESS);
}

//ɾ������
bool CWHRegKey::RecurseDeleteKey(LPCTSTR pszSubKeyName)
{
	//Ч�����
	ASSERT(m_hRegKey!=NULL);
	ASSERT(pszSubKeyName!=NULL);

	//ɾ������
	LONG lResult=RecurseDeleteKey(m_hRegKey,pszSubKeyName);

	return (lResult==ERROR_SUCCESS);
}

//ɾ������
LONG CWHRegKey::RecurseDeleteKey(HKEY hParentKey, LPCTSTR pszSubKeyName)
{
	//Ч�����
	ASSERT(hParentKey!=NULL);
	ASSERT(pszSubKeyName!=NULL);

	//�򿪼���
	HKEY hSubKey=NULL;
	LONG lResult=RegOpenKey(hParentKey,pszSubKeyName,&hSubKey);
	if (lResult!=ERROR_SUCCESS) return lResult;

	//�ݹ�ɾ��
	TCHAR szKeyName[256];
	while (RegEnumKey(hSubKey,0,szKeyName,sizeof(szKeyName))==ERROR_SUCCESS)
	{
		//ɾ������
		lResult=RecurseDeleteKey(hSubKey,szKeyName);
		if (lResult!=ERROR_SUCCESS)
		{
			RegCloseKey(hSubKey);
			return lResult;
		}
	}

	//�رռ���
	RegCloseKey(hSubKey);

	//ɾ������
	lResult=RegDeleteKey(hParentKey,pszSubKeyName);

	return lResult;
}
//�޸���װĿ¼
void CWHRegKey::ResetInstallPath(LPCTSTR pszString)
{
	TCHAR szFullKeyName[256]=TEXT("");
	_sntprintf(szFullKeyName,CountArray(szFullKeyName),TEXT("Software\\%s\\%s\\%s"),szProductKey,_T("Plaza"),_T(""));

	//�򿪼���
	m_hRegKey=NULL;
	LONG lResult=RegOpenKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);
	if ((lResult!=ERROR_SUCCESS)) lResult=RegCreateKey(HKEY_CURRENT_USER,szFullKeyName,&m_hRegKey);
	//д����ֵ
	DWORD dwDataSize=CountStringBuffer(pszString);
	RegSetValueEx(m_hRegKey,_T("Path"),0,REG_SZ,(LPBYTE)pszString,dwDataSize);
}

//////////////////////////////////////////////////////////////////////////////////
