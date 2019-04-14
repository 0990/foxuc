#ifndef WH_THREAD_HEAD_FILE
#define WH_THREAD_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�̶߳���
class SERVICE_CORE_CLASS CWHThread
{
	//״̬����
private:
	volatile bool					m_bRun;								//���б�־

	//�̱߳���
private:
	UINT							m_uThreadID;						//�̱߳�ʶ
	HANDLE							m_hThreadHandle;					//�߳̾��

	//��������
protected:
	//���캯��
	CWHThread();
	//��������
	virtual ~CWHThread();

	//�ӿں���
public:
	//��ȡ״̬
	virtual bool IsRuning();
	//�����߳�
	virtual bool StartThread();
	//��ֹ�߳�
	virtual bool ConcludeThread(DWORD dwMillSeconds);

	//���ܺ���
public:
	//�̱߳�ʶ
	UINT GetThreadID() { return m_uThreadID; }
	//�߳̾��
	HANDLE GetThreadHandle() { return m_hThreadHandle; }
	//Ͷ����Ϣ
	LRESULT PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);

	//�¼�����
protected:
	//�����¼�
	virtual bool OnEventThreadRun() { return true; }
	//��ʼ�¼�
	virtual bool OnEventThreadStrat() { return true; }
	//��ֹ�¼�
	virtual bool OnEventThreadConclude() { return true; }

	//�ڲ�����
private:
	//�̺߳���
	static unsigned __stdcall ThreadFunction(LPVOID pThreadData);
};

//////////////////////////////////////////////////////////////////////////////////

#endif