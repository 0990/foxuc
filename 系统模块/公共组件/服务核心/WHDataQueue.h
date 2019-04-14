#ifndef WH_DATA_QUEUE_HEAD_FILE
#define WH_DATA_QUEUE_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//������ͷ
struct tagDataHead
{
	WORD							wDataSize;							//���ݴ�С
	WORD							wIdentifier;						//���ͱ�ʶ
};

//������Ϣ
struct tagBurthenInfo
{
	DWORD							dwDataSize;							//���ݴ�С
	DWORD							dwBufferSize;						//���峤��
	DWORD							dwDataPacketCount;					//���ݰ���
};

//������Ϣ
struct tagDataBuffer
{
	WORD							wDataSize;							//���ݴ�С
	LPVOID							pDataBuffer;						//����ָ��
};

//////////////////////////////////////////////////////////////////////////////////

//���ݶ���
class SERVICE_CORE_CLASS CWHDataQueue
{
	//��ѯ����
protected:
	DWORD							m_dwInsertPos;						//����λ��
	DWORD							m_dwTerminalPos;					//����λ��
	DWORD							m_dwDataQueryPos;					//��ѯλ��

	//���ݱ���
protected:
	DWORD							m_dwDataSize;						//���ݴ�С
	DWORD							m_dwDataPacketCount;				//���ݰ���

	//�������
protected:
	DWORD							m_dwBufferSize;						//���峤��
	LPBYTE							m_pDataQueueBuffer;					//����ָ��

	//��������
public:
	//���캯��
	CWHDataQueue();
	//��������
	virtual ~CWHDataQueue();

	//���ܺ���
public:
	//������Ϣ
	VOID GetBurthenInfo(tagBurthenInfo & BurthenInfo);
	//������Ϣ
	DWORD GetDataPacketCount() { return m_dwDataPacketCount; }

	//��������
public:
	//��������
	bool InsertData(WORD wIdentifier, VOID * pBuffer, WORD wDataSize);
	//��������
	bool InsertData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount);

	//���ݹ���
public:
	//ɾ������
	VOID RemoveData(bool bFreeMemroy);
	//��ȡ����
	bool DistillData(tagDataHead & DataHead, VOID * pBuffer, WORD wBufferSize);

	//�ڲ�����
private:
	//�����洢
	bool RectifyBuffer(DWORD dwNeedSize);
};

//////////////////////////////////////////////////////////////////////////////////

#endif