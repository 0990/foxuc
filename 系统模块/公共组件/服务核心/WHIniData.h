#ifndef WH_INI_DATA_HEAD_FILE
#define WH_INI_DATA_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////

//配置数据
class SERVICE_CORE_CLASS CWHIniData
{
	//变量定义
protected:
	TCHAR							m_szIniFile[MAX_PATH];				//文件路径

	//函数定义
public:
	//构造函数
	CWHIniData();
	//析构函数
	virtual ~CWHIniData();

	//路径函数
public:
	//设置路径
	VOID SetIniFilePath(LPCTSTR pszIniFile);
	//获取路径
	LPCTSTR GetIniFilePath() { return m_szIniFile; }

	//数据读取
public:
	//读取矩形
	bool ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem);
	//读取尺寸
	bool ReadSize(SIZE & ValueSize, LPCTSTR pszItem, LPCTSTR pszSubItem);
	//读取坐标
	bool ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem);
	//读取颜色
	bool ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem);

	//常规读取
public:
	//读取数值
	UINT ReadInt(LPCTSTR pszItem, LPCTSTR pszSubItem, INT nDefault);
	//读取字符
	LPCTSTR ReadString(LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszDefault, LPTSTR pszString, WORD wMaxCount);
	//读取字符
	LPCTSTR ReadEncryptString(LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszDefault, LPTSTR pszString, WORD wMaxCount);

	//内部函数
protected:
	//转换数值
	LONG SwitchStringToValue(LPCTSTR & pszSring);
};

//////////////////////////////////////////////////////////////////////////////////

#endif