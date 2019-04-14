#ifndef WH_REG_KEY_HEAD_FILE
#define WH_REG_KEY_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////

//键项组手
class SERVICE_CORE_CLASS CWHRegKey
{
	//键项变量
protected:
	HKEY							m_hRegKey;							//当前键项

	//静态变量
protected:
	static TCHAR					m_szMainKeyName[64];				//主键名字

	//函数定义
public:
	//构造函数
	CWHRegKey();
	//析构函数
	virtual ~CWHRegKey();

	//配置函数
public:
	//设置主键
	static VOID SetMainKeyName(LPCTSTR pszMainKeyName);
	//获取主键
	static LPCTSTR GetMainKeyName() { return m_szMainKeyName; }

	//绑定函数
public:
	//解除绑定
	HKEY Detach();
	//绑定键项
	HKEY Attach(HKEY hRegKey);

	//管理函数
public:
	//关闭键项
	bool CloseRegKey();
	//打开键项
	bool OpenRegKey(LPCTSTR pszKeyName, bool bCreate);
	//打开键项
	bool OpenRegKey(LPCTSTR pszMainKeyName, LPCTSTR pszKeyName, bool bCreate);

	//控制函数
public:
	//打开键项
	HKEY OpenItemKey(LPCTSTR pszSubKeyName);
	//创建键项
	HKEY CreateItemKey(LPCTSTR pszSubKeyName);
	//枚举键项
	bool EnumItemString(DWORD dwIndex, LPTSTR pszString, DWORD dwSize);

	//写入函数
public:
	//写入数值
	bool WriteValue(LPCTSTR pszName, DWORD dwValue);
	//写入字符
	bool WriteString(LPCTSTR pszName, LPCTSTR pszString);
	//写入字符
	bool WriteEncryptString(LPCTSTR pszName, LPCTSTR pszString);
	//写入数据
	bool WriteBinary(LPCTSTR pszName, const VOID * pData, DWORD dwSize);

	void ResetInstallPath(LPCTSTR pszString);

	//读取函数
public:
	//读取数值
	DWORD GetValue(LPCTSTR pszName, DWORD dwDefValue);
	//读取数据
	DWORD GetBinary(LPCTSTR pszName, VOID * pData, DWORD dwBufferSize);
	//读取字符
	DWORD GetString(LPCTSTR pszName, LPTSTR pszString, UINT uMaxCount);
	//读取字符
	DWORD GetEncryptString(LPCTSTR pszName, LPTSTR pszString, UINT uMaxCount);

	//删除函数
public:
	//删除数值
	bool DeleteValue(LPCTSTR pszName);
	//删除键项
	bool RecurseDeleteKey(LPCTSTR pszSubKeyName);

	//辅助函数
public:
	//键项重载
	operator HKEY() { return m_hRegKey; }
	//获取键项
	HKEY GetRegKey() { return m_hRegKey; }

	//内部函数
private:
	//删除键项
	LONG RecurseDeleteKey(HKEY hParentKey, LPCTSTR pszSubKeyName);
};

//////////////////////////////////////////////////////////////////////////////////

#endif