#ifndef _TCP_SOCKET_SERVICE_H_
#define _TCP_SOCKET_SERVICE_H_

#include "KernelEngineHead.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"

//����״̬
#define SOCKET_STATUS_IDLE			0									//����״̬
#define SOCKET_STATUS_WAIT			1									//�ȴ�״̬
#define SOCKET_STATUS_CONNECT		2									//����״̬

//���Ӵ���
#define CONNECT_SUCCESS				0									//���ӳɹ�
#define CONNECT_FAILURE				1									//����ʧ��
#define CONNECT_EXCEPTION			2									//�����쳣
#define CONNECT_PROXY_FAILURE		3									//���ӳɹ�
#define CONNECT_PROXY_USER_INVALID	4									//�û���Ч
//

//��������
#define QUEUE_SEND_REQUEST			1									//���ͱ�ʶ
#define QUEUE_CONNECT_REQUEST		2									//���ͱ�ʶ
#define QUEUE_SAFE_CLOSE			3									//��ȫ�ر�
#define QUEUE_DETECT_SOCKET			4									//�������


class CTCPSocketService : public CWnd, public ITCPSocketService, public IQueueServiceSink
{
private:
	bool							m_bService;							//�����־
	CQueueServiceEvent				m_QueueServiceEvent;				//֪ͨ���
	CQueueService					m_QueueService;						//���ж���

	//��������
protected:
	BYTE							m_cbSendRound;						//�ֽ�ӳ��
	BYTE							m_cbRecvRound;						//�ֽ�ӳ��
	DWORD							m_dwSendXorKey;						//������Կ
	DWORD							m_dwRecvXorKey;						//������Կ

private:
	bool							m_bSuccess;							//���ӳɹ�
	DWORD							m_dwTempServerIP;					//���ӵ�IP

private:
	DWORD							m_dwServerIP;						//���ӵ�IP
	WORD							m_wPort;							//���Ӷ˿�

	//�ں˱���
protected:
	SOCKET							m_hSocket;							//���Ӿ��
	WORD							m_wServiceID;						//����ID
	BYTE							m_cbSocketStatus;					//����״̬

	//��������
protected:
	DWORD							m_dwSendTickCount;					//����ʱ��
	DWORD							m_dwRecvTickCount;					//����ʱ��
	DWORD							m_dwSendPacketCount;				//���ͼ���
	DWORD							m_dwRecvPacketCount;				//���ܼ���

	//���ձ���
protected:
	WORD							m_wRecvSize;						//���ճ���
	BYTE							m_cbRecvBuf[SOCKET_TCP_BUFFER*10];	//���ջ���

public:
	CTCPSocketService(void);
	~CTCPSocketService(void);


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
		return AfxIsValidAddress(this, sizeof(CTCPSocketService)) ? true : false;
	}
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
	virtual bool SetTCPSocketEvent(IUnknownEx * pIUnknownEx);


	//���ܽӿ�
public:
	//�ر�����
	virtual bool CloseSocket();
	//���ӵ�ַ
	virtual bool Connect(DWORD dwServerIP, WORD wPort);
	//���ӵ�ַ
	virtual bool Connect(LPCTSTR szServerIP, WORD wPort);
	//���ͺ���
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);


	//��������
protected:
	//��ַ����
	DWORD TranslateAddr(LPCTSTR pszServerAddr);
	//��������
	bool SendDataBuffer(void * pBuffer, WORD wSendSize);
	//�ر�����
	VOID CloseSocket(BYTE cbShutReason);

	//���ܺ���
protected:
	//��������
	WORD CrevasseBuffer(BYTE cbDataBuffer[], WORD wDataSize);
	//��������
	WORD EncryptBuffer(BYTE cbDataBuffer[], WORD wDataSize, WORD wBufferSize);

	//��������
private:
	//�ֽ�ӳ��
	inline WORD SeedRandMap(WORD wSeed);
	//����ӳ��
	inline BYTE MapSendByte(BYTE cbData);
	//����ӳ��
	inline BYTE MapRecvByte(BYTE cbData);

	//��Ϣӳ��
protected:
	//������Ϣ
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	//��Ϣ����
protected:
	//�����ȡ
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//���緢��
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam) {};
	//����ر�
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};

#endif //_TCP_SOCKET_SERVICE_H_