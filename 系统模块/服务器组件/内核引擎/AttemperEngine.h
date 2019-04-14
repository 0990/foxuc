#ifndef ATTEMPER_ENGINE_HEAD_FILE
#define ATTEMPER_ENGINE_HEAD_FILE

#pragma once

//���ͷ�ļ�

#include "KernelEngineHead.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"

//////////////////////////////////////////////////////////////////////////

//���ȹ�����
class CAttemperEngine : public IAttemperEngine, public IQueueServiceSink,
		public IDataBaseEngineEvent, public ITCPNetworkEngineEvent
{
	//���ı���
protected:
	bool							m_bService;							//���б�־
	CQueueService					m_QueueService;						//���ж���
	CQueueServiceEvent              m_QueueServiceEvent;
	CCriticalSection				m_CriticalSection;					//ͬ������

	//�ӿڱ���
protected:
	ITCPSocketService				* m_pITCPSocketService;				//��������
	ITCPNetworkEngine				* m_pTCPNetworkEngine;				//��������
	IAttemperEngineSink				* m_pIAttemperEngineSink;			//�ҽӽӿ�

	//����������ά��
protected:
	CWHArray<DWORD>					m_AndroidUserSocketIDArray;			//����������

	//��������
public:
	//���캯��
	CAttemperEngine(void);
	//��������
	virtual ~CAttemperEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release()
	{
		if (IsValid()) delete this;
	}
	//�Ƿ���Ч
	virtual bool IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CAttemperEngine)) ? true : false;
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
public:
	//��������
	virtual bool SetNetworkEngine(IUnknownEx * pIUnknownEx);
	//ע�ṳ��
	virtual bool SetAttemperEngineSink(IUnknownEx * pIUnknownEx);
	//�Զ��¼�
	virtual bool OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize);

	//���нӿ�
public:
	//�����ӿ�
	virtual void OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize/*, DWORD dwInsertTime*/);

	//virtual void BindQueueService(CQueueServiceEvent* queueServiceEvent);
	//�ӿڶ���
public:
	//���ݿ���
	virtual bool OnEventDataBaseResult(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//����ģ��ӿ�
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientAddr);
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientAddr, DWORD dwActiveTime);
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(DWORD dwSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

public:
	//��������
	virtual bool SetSocketEngine(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual void * GetQueueService(const IID & Guid, DWORD dwQueryVer);

	virtual void * GetQueue();
};

//////////////////////////////////////////////////////////////////////////

#endif