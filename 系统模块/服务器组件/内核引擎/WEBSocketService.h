#ifndef _WEB_SOCKET_SERVICE_H_
#define _WEB_SOCKET_SERVICE_H_

#include "KernelEngineHead.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"


class CWEBSocketService : public CWnd, public IWEBSocketService, public IQueueServiceSink
{

public:
	CWEBSocketService(void);
	~CWEBSocketService(void);


	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release()	{ if (IsValid()) delete this; }
	//�Ƿ���Ч
	virtual bool IsValid()	{ return AfxIsValidAddress(this, sizeof(CWEBSocketService)) ? true : false; }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool StartService();
	//ֹͣ����
	virtual bool ConcludeService();

	//���нӿ�
public:
	//֪ͨ�ص�����
	virtual void OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize);

	//���ýӿ�
public:
	//���ú���
	virtual bool SetServiceID(WORD wServiceID);
	//���ýӿ�
	virtual bool SetWEBSocketEvent(IUnknownEx * pIUnknownEx);

	//���ܽӿ�
public:
	//�ر�����
	virtual bool CloseSocket(BYTE cbShutReason);
	//���Ӳ���
	virtual bool ConnectServer(LPCTSTR pszURL, WORD wPort);
	//��������
	virtual bool SendRequestData(VOID * pData, WORD wDataSize);

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()

};

#endif //_WEB_SOCKET_SERVICE_H_