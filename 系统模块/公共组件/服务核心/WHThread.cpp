#include "StdAfx.h"
#include "WHThread.h"

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//启动参数
struct tagThreadParameter
{
	bool							bSuccess;							//是否错误
	HANDLE							hEventFinish;						//事件句柄
	CWHThread	*				pServiceThread;						//线程指针
};

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWHThread::CWHThread()
{
	//设置变量
	m_bRun=false;
	m_uThreadID=0;
	m_hThreadHandle=NULL;

	return;
}

//析构函数
CWHThread::~CWHThread()
{
	//停止线程
	ConcludeThread(INFINITE);

	return;
}

//状态判断
bool CWHThread::IsRuning()
{
	//运行检测
	if (m_hThreadHandle==NULL) return false;
	if (WaitForSingleObject(m_hThreadHandle,0)!=WAIT_TIMEOUT) return false;

	return true;
}

//启动线程
bool CWHThread::StartThread()
{
	//效验状态
	ASSERT(IsRuning()==false);
	if (IsRuning()==true) return false;

	//清理变量
	if (m_hThreadHandle!=NULL) 
	{
		//关闭句柄
		CloseHandle(m_hThreadHandle);

		//设置变量
		m_uThreadID=0;
		m_hThreadHandle=NULL;
	}

	//变量定义
	tagThreadParameter ThreadParameter;
	ZeroMemory(&ThreadParameter,sizeof(ThreadParameter));

	//设置变量
	ThreadParameter.bSuccess=false;
	ThreadParameter.pServiceThread=this;
	ThreadParameter.hEventFinish=CreateEvent(NULL,FALSE,FALSE,NULL);

	//效验状态
	ASSERT(ThreadParameter.hEventFinish!=NULL);
	if (ThreadParameter.hEventFinish==NULL) return false;

	//启动线程
	m_bRun=true;
	m_hThreadHandle=(HANDLE)::_beginthreadex(NULL,0,ThreadFunction,&ThreadParameter,0,&m_uThreadID);

	//错误判断
	if (m_hThreadHandle==INVALID_HANDLE_VALUE)
	{
		CloseHandle(ThreadParameter.hEventFinish);
		return false;
	}

	//等待事件
	WaitForSingleObject(ThreadParameter.hEventFinish,INFINITE);
	CloseHandle(ThreadParameter.hEventFinish);

	//判断错误
	if (ThreadParameter.bSuccess==false)
	{
		ConcludeThread(INFINITE);
		return false;
	}

	return true;
}

//停止线程
bool CWHThread::ConcludeThread(DWORD dwMillSeconds)
{
	//停止线程
	if (IsRuning()==true)
	{
		//设置变量
		m_bRun=false;
		//DWORD dwRet = 0;
		//MSG msg;
		//while (TRUE)
		//{
		//	//wait for m_hThread to be over，and wait for  
		//	//QS_ALLINPUT（Any message is in the queue）  
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
		//停止等待 debug by xujialong 主进程使用waitforsingleobject，子线程sendMessage会卡死
		if (WaitForSingleObject(m_hThreadHandle,dwMillSeconds)==WAIT_TIMEOUT)
		{
			return false;
		}
	}

	//设置变量
	if (m_hThreadHandle!=NULL)
	{
		//关闭句柄
		CloseHandle(m_hThreadHandle);

		//设置变量
		m_uThreadID=0;
		m_hThreadHandle=NULL;
	}

	return true;
}

//投递消息
LRESULT CWHThread::PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//状态效验
	ASSERT((m_uThreadID!=0)&&(m_hThreadHandle!=NULL));
	if ((m_uThreadID==0)||(m_hThreadHandle==NULL)) return false;

	//投递消息
	if (::PostThreadMessage(m_uThreadID,uMessage,wParam,lParam)==FALSE)
	{
		DWORD dwLastError=GetLastError();
		return dwLastError;
	}

	return 0L;
}

//线程函数
unsigned __stdcall CWHThread::ThreadFunction(LPVOID pThreadData)
{
	//随机种子
	srand((DWORD)time(NULL));

	//变量定义
	tagThreadParameter * pThreadParameter=(tagThreadParameter *)pThreadData;
	CWHThread * pServiceThread=pThreadParameter->pServiceThread;

	//启动通知
	try
	{
		pThreadParameter->bSuccess=pServiceThread->OnEventThreadStrat(); 
	} 
	catch (...)
	{
		//设置变量
		ASSERT(FALSE);
		pThreadParameter->bSuccess=false;
	}

	//设置事件
	bool bSuccess=pThreadParameter->bSuccess;
	ASSERT(pThreadParameter->hEventFinish!=NULL);
	if (pThreadParameter->hEventFinish!=NULL) SetEvent(pThreadParameter->hEventFinish);

	//线程处理
	if (bSuccess==true)
	{
		//线程运行
		while (pServiceThread->m_bRun)
		{
#ifndef _DEBUG
			//运行版本
			try
			{
				if (pServiceThread->OnEventThreadRun()==false)
				{
					break;
				}
			}
			catch (...)	{ }
#else
			//调试版本
			if (pServiceThread->OnEventThreadRun()==false)
			{
				break;
			}
#endif
		}

		//停止通知
		try
		{ 
			pServiceThread->OnEventThreadConclude();
		} 
		catch (...)	{ ASSERT(FALSE); }
	}

	//中止线程
	_endthreadex(0L);

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
