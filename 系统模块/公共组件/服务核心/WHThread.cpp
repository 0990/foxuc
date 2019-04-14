#include "StdAfx.h"
#include "WHThread.h"

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagThreadParameter
{
	bool							bSuccess;							//�Ƿ����
	HANDLE							hEventFinish;						//�¼����
	CWHThread	*				pServiceThread;						//�߳�ָ��
};

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWHThread::CWHThread()
{
	//���ñ���
	m_bRun=false;
	m_uThreadID=0;
	m_hThreadHandle=NULL;

	return;
}

//��������
CWHThread::~CWHThread()
{
	//ֹͣ�߳�
	ConcludeThread(INFINITE);

	return;
}

//״̬�ж�
bool CWHThread::IsRuning()
{
	//���м��
	if (m_hThreadHandle==NULL) return false;
	if (WaitForSingleObject(m_hThreadHandle,0)!=WAIT_TIMEOUT) return false;

	return true;
}

//�����߳�
bool CWHThread::StartThread()
{
	//Ч��״̬
	ASSERT(IsRuning()==false);
	if (IsRuning()==true) return false;

	//�������
	if (m_hThreadHandle!=NULL) 
	{
		//�رվ��
		CloseHandle(m_hThreadHandle);

		//���ñ���
		m_uThreadID=0;
		m_hThreadHandle=NULL;
	}

	//��������
	tagThreadParameter ThreadParameter;
	ZeroMemory(&ThreadParameter,sizeof(ThreadParameter));

	//���ñ���
	ThreadParameter.bSuccess=false;
	ThreadParameter.pServiceThread=this;
	ThreadParameter.hEventFinish=CreateEvent(NULL,FALSE,FALSE,NULL);

	//Ч��״̬
	ASSERT(ThreadParameter.hEventFinish!=NULL);
	if (ThreadParameter.hEventFinish==NULL) return false;

	//�����߳�
	m_bRun=true;
	m_hThreadHandle=(HANDLE)::_beginthreadex(NULL,0,ThreadFunction,&ThreadParameter,0,&m_uThreadID);

	//�����ж�
	if (m_hThreadHandle==INVALID_HANDLE_VALUE)
	{
		CloseHandle(ThreadParameter.hEventFinish);
		return false;
	}

	//�ȴ��¼�
	WaitForSingleObject(ThreadParameter.hEventFinish,INFINITE);
	CloseHandle(ThreadParameter.hEventFinish);

	//�жϴ���
	if (ThreadParameter.bSuccess==false)
	{
		ConcludeThread(INFINITE);
		return false;
	}

	return true;
}

//ֹͣ�߳�
bool CWHThread::ConcludeThread(DWORD dwMillSeconds)
{
	//ֹͣ�߳�
	if (IsRuning()==true)
	{
		//���ñ���
		m_bRun=false;
		//DWORD dwRet = 0;
		//MSG msg;
		//while (TRUE)
		//{
		//	//wait for m_hThread to be over��and wait for  
		//	//QS_ALLINPUT��Any message is in the queue��  
		//	dwRet = MsgWaitForMultipleObjects(1, &m_hThreadHandle, FALSE, dwMillSeconds, QS_ALLINPUT);
		//	switch (dwRet)
		//	{
		//	case WAIT_OBJECT_0:
		//		break; //break the loop  
		//	case WAIT_OBJECT_0 + 1:
		//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		//			TranslateMessage(&msg);
		//			DispatchMessage(&msg);
		//		}
		//		continue;
		//	case WAIT_TIMEOUT:
		//		return false;
		//	default:
		//		 return false;
		//	}
		//	break;
		//}
		//ֹͣ�ȴ� debug by xujialong ������ʹ��waitforsingleobject�����߳�sendMessage�Ῠ��
		if (WaitForSingleObject(m_hThreadHandle,dwMillSeconds)==WAIT_TIMEOUT)
		{
			return false;
		}
	}

	//���ñ���
	if (m_hThreadHandle!=NULL)
	{
		//�رվ��
		CloseHandle(m_hThreadHandle);

		//���ñ���
		m_uThreadID=0;
		m_hThreadHandle=NULL;
	}

	return true;
}

//Ͷ����Ϣ
LRESULT CWHThread::PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//״̬Ч��
	ASSERT((m_uThreadID!=0)&&(m_hThreadHandle!=NULL));
	if ((m_uThreadID==0)||(m_hThreadHandle==NULL)) return false;

	//Ͷ����Ϣ
	if (::PostThreadMessage(m_uThreadID,uMessage,wParam,lParam)==FALSE)
	{
		DWORD dwLastError=GetLastError();
		return dwLastError;
	}

	return 0L;
}

//�̺߳���
unsigned __stdcall CWHThread::ThreadFunction(LPVOID pThreadData)
{
	//�������
	srand((DWORD)time(NULL));

	//��������
	tagThreadParameter * pThreadParameter=(tagThreadParameter *)pThreadData;
	CWHThread * pServiceThread=pThreadParameter->pServiceThread;

	//����֪ͨ
	try
	{
		pThreadParameter->bSuccess=pServiceThread->OnEventThreadStrat(); 
	} 
	catch (...)
	{
		//���ñ���
		ASSERT(FALSE);
		pThreadParameter->bSuccess=false;
	}

	//�����¼�
	bool bSuccess=pThreadParameter->bSuccess;
	ASSERT(pThreadParameter->hEventFinish!=NULL);
	if (pThreadParameter->hEventFinish!=NULL) SetEvent(pThreadParameter->hEventFinish);

	//�̴߳���
	if (bSuccess==true)
	{
		//�߳�����
		while (pServiceThread->m_bRun)
		{
#ifndef _DEBUG
			//���а汾
			try
			{
				if (pServiceThread->OnEventThreadRun()==false)
				{
					break;
				}
			}
			catch (...)	{ }
#else
			//���԰汾
			if (pServiceThread->OnEventThreadRun()==false)
			{
				break;
			}
#endif
		}

		//ֹ֪ͣͨ
		try
		{ 
			pServiceThread->OnEventThreadConclude();
		} 
		catch (...)	{ ASSERT(FALSE); }
	}

	//��ֹ�߳�
	_endthreadex(0L);

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
