#include "StdAfx.h"
#include "WHDataLocker.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWHDataLocker::CWHDataLocker(CCriticalSection & CriticalSection, bool bLockAtOnce) 
	: m_CriticalSection(CriticalSection)
{
	//���ñ���
	m_nLockCount=0;

	//��������
	if (bLockAtOnce==true)
	{
		Lock();
	}

	return;
}

//��������
CWHDataLocker::~CWHDataLocker()
{
	//�������
	while (m_nLockCount>0)
	{
		UnLock();
	}

	return;
}

//��������
VOID CWHDataLocker::Lock()
{
	//��������
	m_CriticalSection.Lock();

	//���ñ���
	m_nLockCount++;

	return;
}

//��������
VOID CWHDataLocker::UnLock()
{
	//Ч��״̬
	ASSERT(m_nLockCount>0);
	if (m_nLockCount==0) return;

	//���ñ���
	m_nLockCount--;

	//�������
	m_CriticalSection.Unlock();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
