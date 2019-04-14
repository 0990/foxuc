#ifndef PROCESS_EXCEPTION_HEAD_FILE
#define PROCESS_EXCEPTION_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//异常报告
class KERNEL_ENGINE_CLASS CExceptionHandle : public CDialog
{
	//友元定义
	friend class CExceptionManager;

	//变量定义
protected:
	CString							m_strFileName;						//文件名字
	EXCEPTION_POINTERS *			m_lpExceptionInfo;					//异常信息

	//控件变量
protected:
	CRichEditCtrl					m_RichEditCtrl;						//信息控件

	//函数定义
public:
	//构造函数
	CExceptionHandle();
	//析构函数
	virtual ~CExceptionHandle();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//初始化函数
	virtual BOOL OnInitDialog();

	//按钮消息
protected:
	//调试按钮
	VOID OnBnClickedDebug();
	//发送报告
	VOID OnBnClickedSendReport();

	//辅助函数
protected:
	//插入字串
	VOID InsertString(LPCTSTR pszString);
	//保存文件
	VOID SaveExceptionInfo(EXCEPTION_POINTERS * lpExceptionInfo);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//进程异常
class KERNEL_ENGINE_CLASS CExceptionManager
{
	//变量定义
protected:
	static CExceptionManager *		m_pExceptionManager;				//异常对象

	//函数定义
public:
	//构造函数
	CExceptionManager();
	//析构函数
	virtual ~CExceptionManager();

	//全局函数
public:
	//异常函数
	static LONG WINAPI ProcessExceptionFilter(EXCEPTION_POINTERS * lpExceptionInfo);
};

//////////////////////////////////////////////////////////////////////////////////

#endif