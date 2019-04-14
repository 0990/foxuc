#ifndef WH_REG_KEY_HEAD_FILE
#define WH_REG_KEY_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class SERVICE_CORE_CLASS CWHRegKey
{
	//�������
protected:
	HKEY							m_hRegKey;							//��ǰ����

	//��̬����
protected:
	static TCHAR					m_szMainKeyName[64];				//��������

	//��������
public:
	//���캯��
	CWHRegKey();
	//��������
	virtual ~CWHRegKey();

	//���ú���
public:
	//��������
	static VOID SetMainKeyName(LPCTSTR pszMainKeyName);
	//��ȡ����
	static LPCTSTR GetMainKeyName() { return m_szMainKeyName; }

	//�󶨺���
public:
	//�����
	HKEY Detach();
	//�󶨼���
	HKEY Attach(HKEY hRegKey);

	//������
public:
	//�رռ���
	bool CloseRegKey();
	//�򿪼���
	bool OpenRegKey(LPCTSTR pszKeyName, bool bCreate);
	//�򿪼���
	bool OpenRegKey(LPCTSTR pszMainKeyName, LPCTSTR pszKeyName, bool bCreate);

	//���ƺ���
public:
	//�򿪼���
	HKEY OpenItemKey(LPCTSTR pszSubKeyName);
	//��������
	HKEY CreateItemKey(LPCTSTR pszSubKeyName);
	//ö�ټ���
	bool EnumItemString(DWORD dwIndex, LPTSTR pszString, DWORD dwSize);

	//д�뺯��
public:
	//д����ֵ
	bool WriteValue(LPCTSTR pszName, DWORD dwValue);
	//д���ַ�
	bool WriteString(LPCTSTR pszName, LPCTSTR pszString);
	//д���ַ�
	bool WriteEncryptString(LPCTSTR pszName, LPCTSTR pszString);
	//д������
	bool WriteBinary(LPCTSTR pszName, const VOID * pData, DWORD dwSize);

	void ResetInstallPath(LPCTSTR pszString);

	//��ȡ����
public:
	//��ȡ��ֵ
	DWORD GetValue(LPCTSTR pszName, DWORD dwDefValue);
	//��ȡ����
	DWORD GetBinary(LPCTSTR pszName, VOID * pData, DWORD dwBufferSize);
	//��ȡ�ַ�
	DWORD GetString(LPCTSTR pszName, LPTSTR pszString, UINT uMaxCount);
	//��ȡ�ַ�
	DWORD GetEncryptString(LPCTSTR pszName, LPTSTR pszString, UINT uMaxCount);

	//ɾ������
public:
	//ɾ����ֵ
	bool DeleteValue(LPCTSTR pszName);
	//ɾ������
	bool RecurseDeleteKey(LPCTSTR pszSubKeyName);

	//��������
public:
	//��������
	operator HKEY() { return m_hRegKey; }
	//��ȡ����
	HKEY GetRegKey() { return m_hRegKey; }

	//�ڲ�����
private:
	//ɾ������
	LONG RecurseDeleteKey(HKEY hParentKey, LPCTSTR pszSubKeyName);
};

//////////////////////////////////////////////////////////////////////////////////

#endif