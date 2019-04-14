#ifndef WH_DATA_LOCKER_HEAD_FILE
#define WH_DATA_LOCKER_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////

//数据锁定
class SERVICE_CORE_CLASS CWHDataLocker
{
	//变量定义
private:
	INT								m_nLockCount;					//锁定计数
	CCriticalSection &				m_CriticalSection;				//锁定对象

	//函数定义
public:
	//构造函数
	CWHDataLocker(CCriticalSection & CriticalSection, bool bLockAtOnce=true);
	//析构函数
	virtual ~CWHDataLocker();

	//操作函数
public:
	//锁定函数
	VOID Lock();
	//解锁函数 
	VOID UnLock();

	//状态函数
public:
	//锁定次数
	inline INT GetLockCount() { return m_nLockCount; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif