#include "StdAfx.h"
#include "WHIniData.h"
#include "WHEncrypt.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWHIniData::CWHIniData()
{
	//���ñ���
	ZeroMemory(m_szIniFile,sizeof(m_szIniFile));

	return;
}

//��������
CWHIniData::~CWHIniData()
{
}

//����·��
VOID CWHIniData::SetIniFilePath(LPCTSTR pszIniFile)
{
	//���ñ���
	lstrcpyn(m_szIniFile,pszIniFile,CountArray(m_szIniFile));

	return;
}

//��ȡ��ֵ
UINT CWHIniData::ReadInt(LPCTSTR pszItem, LPCTSTR pszSubItem, INT nDefault)
{
	//Ч��״̬
	ASSERT(m_szIniFile[0]!=0);

	//��ȡ��ֵ
	UINT uReadData=GetPrivateProfileInt(pszItem,pszSubItem,nDefault,m_szIniFile);

	return uReadData;
}

//��ȡ�ַ�
LPCTSTR CWHIniData::ReadString(LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszDefault, LPTSTR pszString, WORD wMaxCount)
{
	//Ч��״̬
	ASSERT(m_szIniFile[0]!=0);

	//��ȡ�ַ�
	GetPrivateProfileString(pszItem,pszSubItem,pszDefault,pszString,wMaxCount,m_szIniFile);

	return pszString;
}

//��ȡ�ַ�
LPCTSTR CWHIniData::ReadEncryptString(LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszDefault, LPTSTR pszString, WORD wMaxCount)
{
	//Ч��״̬
	ASSERT(m_szIniFile[0]!=0);
	ASSERT(wMaxCount<=MAX_SOURCE_LEN);

	//���ý��
	if (wMaxCount>0) pszString[0]=0;

	//��ȡ�ַ�
	TCHAR szStringRead[MAX_ENCRYPT_LEN];
	DWORD dwReadCount=GetPrivateProfileString(pszItem,pszSubItem,NULL,szStringRead,MAX_ENCRYPT_LEN,m_szIniFile);

	//�����ַ�
	if ((dwReadCount>0)&&(dwReadCount<CountArray(szStringRead)))
	{
		CWHEncrypt WHEncrypt;
		WHEncrypt.XorCrevasse(szStringRead,pszString,wMaxCount);
	}

	//Ĭ�ϲ���
	if ((wMaxCount>0)&&(pszString[0]==0)) lstrcpyn(pszString,pszDefault,wMaxCount);

	return pszString;
}

//��ȡ����
bool CWHIniData::ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem)
{
	//Ч��״̬
	ASSERT(m_szIniFile[0]!=0);

	//���ñ���
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValueRect,sizeof(ValueRect));

	//��ȡ�ַ�
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),m_szIniFile);

	//���ݴ���
	if (szReadData[0]!=0)
	{
		//��ȡ����
		LPCTSTR pszString=szReadData;
		ValueRect.left=SwitchStringToValue(pszString);
		ValueRect.top=SwitchStringToValue(pszString);
		ValueRect.right=SwitchStringToValue(pszString);
		ValueRect.bottom=SwitchStringToValue(pszString);

		return true;
	}

	return false;
}

//��ȡ�ߴ�
bool CWHIniData::ReadSize(SIZE & ValueSize, LPCTSTR pszItem, LPCTSTR pszSubItem)
{
	//Ч��״̬
	ASSERT(m_szIniFile[0]!=0);

	//���ñ���
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValueSize,sizeof(ValueSize));

	//��ȡ�ַ�
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),m_szIniFile);

	//���ݴ���
	if (szReadData[0]!=0)
	{
		//��ȡ����
		LPCTSTR pszString=szReadData;
		ValueSize.cx=SwitchStringToValue(pszString);
		ValueSize.cy=SwitchStringToValue(pszString);

		return true;
	}

	return false;
}

//��ȡ����
bool CWHIniData::ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem)
{
	//Ч��״̬
	ASSERT(m_szIniFile[0]!=0);

	//���ñ���
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValuePoint,sizeof(ValuePoint));

	//��ȡ�ַ�
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),m_szIniFile);

	//���ݴ���
	if (szReadData[0]!=0)
	{
		//��ȡ����
		LPCTSTR pszString=szReadData;
		ValuePoint.x=SwitchStringToValue(pszString);
		ValuePoint.y=SwitchStringToValue(pszString);

		return true;
	}

	return false;
}

//��ȡ��ɫ
bool CWHIniData::ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem)
{
	//Ч��״̬
	ASSERT(m_szIniFile[0]!=0);

	//���ñ���
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValueColor,sizeof(ValueColor));

	//��ȡ�ַ�
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),m_szIniFile);

	//���ݴ���
	if (szReadData[0]!=0)
	{
		//��ȡ����
		LPCTSTR pszString=szReadData;
		ValueColor=RGB(SwitchStringToValue(pszString),SwitchStringToValue(pszString),SwitchStringToValue(pszString));

		return true;
	}

	return false;
}

//ת����ֵ
LONG CWHIniData::SwitchStringToValue(LPCTSTR & pszSring)
{
	//Ч�����
	ASSERT((pszSring!=NULL)&&(pszSring[0]!=0));
	if ((pszSring==NULL)||(pszSring[0]==0)) return 0L;

	//Ѱ�ҿ�ʼ
	while (((pszSring[0]>0)&&(pszSring[0]<TEXT('0')))||(pszSring[0]>TEXT('9'))) pszSring++;

	//��ȡ��ֵ
	LONG lValue=0L;
	while ((pszSring[0]>=TEXT('0'))&&(pszSring[0]<=TEXT('9')))
	{
		lValue=lValue*10L+pszSring[0]-TEXT('0');
		++pszSring;
	}

	return lValue;
}

//////////////////////////////////////////////////////////////////////////////////
