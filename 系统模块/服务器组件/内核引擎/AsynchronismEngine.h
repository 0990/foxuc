#ifndef ASYNCHRONISM_ENGINE_HEAD_FILE
#define ASYNCHRONISM_ENGINE_HEAD_FILE

#pragma once


#include "KernelEngineHead.h"

//��˵��
class CAsynchronismEngine;
typedef class CWHArray<IAsynchronismEngineSink *> CAsynchronismEngineSinkArray;

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
	//��������
public:
	CAsynchronismEngine				* m_pAsynchronismEngine;			//�첽����

	//��������
public:
	//���캯��
	CControlWnd();
	//��������
	virtual ~CControlWnd();

	//��Ϣӳ��
protected:
	//������Ϣ
	HRESULT OnAsynRequest(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//��Ϣ�߳�
class CMessageThread : public CWHThread
{
	//��������
public:
	CAsynchronismEngine				* m_pAsynchronismEngine;			//�첽����

	//��������
public:
	//���캯��
	CMessageThread();
	//��������
	virtual ~CMessageThread();

	//���ܺ���
public:
	//ֹͣ�߳�
	virtual bool StopThread(DWORD dwWaitSeconds = INFINITE);
	//��ֹ�߳�
	virtual bool TerminateThread(DWORD dwExitCode);

	//�¼�����
private:
	//��ʼ�¼�
	virtual bool OnThreadStratEvent();
	//ֹͣ�¼�
	virtual bool OnThreadStopEvent();

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//�첽����
class CAsynchronismEngine : public IAsynchronismEngine
{
	friend class CControlWnd;
	friend class CMessageThread;

	//�ں˱���
protected:
	bool							m_bService;							//�����־
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];	//���ݻ���
	CAsynchronismEngineSinkArray	m_AsynchronismEngineSinkArray;		//�첽����

	//�������
protected:
	CControlWnd						m_ControlWnd;						//���ƴ���
	CCriticalSection				m_CriticalSection;					//�߳���
	CWHDataQueue					m_DataStorage;						//���ݴ洢
	CMessageThread					m_MessageThread;					//�߳����

	//��������
public:
	//���캯��
	CAsynchronismEngine(void);
	//��������
	virtual ~CAsynchronismEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release()
	{
		if (IsValid()) delete this;    //
	}
	//�Ƿ���Ч
	virtual bool IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CAsynchronismEngine)) ? true : false;
	}
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool StartService();
	//ֹͣ����
	virtual bool ConcludeService();
	//��������
	virtual bool InsertRequest(WORD wRequestID, void * const pBuffer, WORD wDataSize, IUnknownEx * pIUnknownEx);

	//���ܽӿ�
public:
	//���и���
	virtual bool GetBurthenInfo(tagBurthenInfo & BurthenInfo);
	//ע�ṳ��
	virtual bool SetAsynchronismSink(IUnknownEx * pIUnknownEx);
	//Ͷ������
	virtual bool PostAsynchronismData(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//Ͷ������
	virtual bool PostAsynchronismData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount);
	//ȡ��ע��
	virtual bool UnRegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx);

	//�̺߳���
protected:
	//�߳�����
	bool OnMessageThreadStart();
	//�߳�ֹͣ
	bool OnMessageThreadStop();
	//�첽����
	void OnAsynchronismRequest(WORD wRequestID, IAsynchronismEngineSink * pIAsynchronismEngineSink);
};

//////////////////////////////////////////////////////////////////////////

#endif