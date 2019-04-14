#include "StdAfx.h"
#include "WHDataQueue.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWHDataQueue::CWHDataQueue()
{
	//��ѯ����
	m_dwInsertPos=0L;
	m_dwTerminalPos=0L;
	m_dwDataQueryPos=0L;

	//���ݱ���
	m_dwDataSize=0L;
	m_dwDataPacketCount=0L;

	//�������
	m_dwBufferSize=0L;
	m_pDataQueueBuffer=NULL;

	return;
}

//��������
CWHDataQueue::~CWHDataQueue()
{
	SafeDeleteArray(m_pDataQueueBuffer);
	return;
}

//������Ϣ
VOID CWHDataQueue::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	//���ñ���
	BurthenInfo.dwDataSize=m_dwDataSize;
	BurthenInfo.dwBufferSize=m_dwBufferSize;
	BurthenInfo.dwDataPacketCount=m_dwDataPacketCount;

	return;
}

//��������
bool CWHDataQueue::InsertData(WORD wIdentifier, VOID * pBuffer, WORD wDataSize)
{
	//��������
	tagDataHead DataHead;
	ZeroMemory(&DataHead,sizeof(DataHead));

	//���ñ���
	DataHead.wDataSize=wDataSize;
	DataHead.wIdentifier=wIdentifier;

	//�����洢
	if (RectifyBuffer(sizeof(DataHead)+DataHead.wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	try
	{
		//��������
		CopyMemory(m_pDataQueueBuffer+m_dwInsertPos,&DataHead,sizeof(DataHead));

		//��������
		if (wDataSize>0) 
		{
			ASSERT(pBuffer!=NULL);
			CopyMemory(m_pDataQueueBuffer+m_dwInsertPos+sizeof(DataHead),pBuffer,wDataSize);
		}

		//��������
		m_dwDataPacketCount++;
		m_dwDataSize+=sizeof(DataHead)+wDataSize;
		m_dwInsertPos+=sizeof(DataHead)+wDataSize;
		m_dwTerminalPos=__max(m_dwTerminalPos,m_dwInsertPos);

		return true;
	}
	catch (...)
	{
		ASSERT(FALSE);
		return false;
	}

	return false;
}

//��������
bool CWHDataQueue::InsertData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount)
{
	//��������
	tagDataHead DataHead;
	ZeroMemory(&DataHead,sizeof(DataHead));

	//���ñ���
	DataHead.wDataSize=0;
	DataHead.wIdentifier=wIdentifier;

	//�ۼƴ�С
	for (WORD i=0;i<wDataCount;i++)
	{
		if (DataBuffer[i].wDataSize>0)
		{
			DataHead.wDataSize+=DataBuffer[i].wDataSize;
		}
	}

	//�����洢
	if (RectifyBuffer(sizeof(DataHead)+DataHead.wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	try
	{
		//��������
		CopyMemory(m_pDataQueueBuffer+m_dwInsertPos,&DataHead,sizeof(DataHead));

		//��������
		if (DataHead.wDataSize>0)
		{
			//��������
			WORD wExcursion=sizeof(DataHead);

			//��������
			for (WORD i=0;i<wDataCount;i++)
			{
				if (DataBuffer[i].wDataSize>0)
				{
					//Ч��״̬
					ASSERT(DataBuffer[i].pDataBuffer!=NULL);

					//��������
					CopyMemory(m_pDataQueueBuffer+m_dwInsertPos+wExcursion,DataBuffer[i].pDataBuffer,DataBuffer[i].wDataSize);

					//���ñ���
					wExcursion+=DataBuffer[i].wDataSize;
				}
			}
		}

		//��������
		m_dwDataPacketCount++;
		m_dwDataSize+=sizeof(DataHead)+DataHead.wDataSize;
		m_dwInsertPos+=sizeof(DataHead)+DataHead.wDataSize;
		m_dwTerminalPos=__max(m_dwTerminalPos,m_dwInsertPos);

		return true;
	}
	catch (...)
	{
		ASSERT(FALSE);
		return false;
	}

	return false;
}

//��ȡ����
bool CWHDataQueue::DistillData(tagDataHead & DataHead, VOID * pBuffer, WORD wBufferSize)
{
	//Ч�����
	ASSERT(m_dwDataSize>0L);
	ASSERT(m_dwDataPacketCount>0);
	ASSERT(m_pDataQueueBuffer!=NULL);

	//Ч�����
	if (m_dwDataSize==0L) return false;
	if (m_dwDataPacketCount==0L) return false;

	//��������
	if (m_dwDataQueryPos==m_dwTerminalPos)
	{
		m_dwDataQueryPos=0L;
		m_dwTerminalPos=m_dwInsertPos;
	}

	//��ȡָ��
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+sizeof(tagDataHead)));
	tagDataHead * pDataHead=(tagDataHead *)(m_pDataQueueBuffer+m_dwDataQueryPos);
	ASSERT(wBufferSize>=pDataHead->wDataSize);

	//��ȡ��С
	WORD wPacketSize=sizeof(DataHead)+pDataHead->wDataSize;
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+wPacketSize));

	//�жϻ���
	WORD wCopySize=0;
	ASSERT(wBufferSize>=pDataHead->wDataSize);
	if (wBufferSize>=pDataHead->wDataSize) wCopySize=pDataHead->wDataSize;

	//��������
	DataHead=*pDataHead;
	if (DataHead.wDataSize>0)
	{
		if (wBufferSize<pDataHead->wDataSize) DataHead.wDataSize=0;
		else CopyMemory(pBuffer,pDataHead+1,DataHead.wDataSize);
	}

	//Ч�����
	ASSERT(wPacketSize<=m_dwDataSize);
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+wPacketSize));

	//���ñ���
	m_dwDataPacketCount--;
	m_dwDataSize-=wPacketSize;
	m_dwDataQueryPos+=wPacketSize;

	return true;
}

//ɾ������
VOID CWHDataQueue::RemoveData(bool bFreeMemroy)
{
	//���ñ���
	m_dwDataSize=0L;
	m_dwInsertPos=0L;
	m_dwTerminalPos=0L;
	m_dwDataQueryPos=0L;
	m_dwDataPacketCount=0L;

	//ɾ���ڴ�
	if (bFreeMemroy==true) 
	{
		m_dwBufferSize=0;
		SafeDeleteArray(m_pDataQueueBuffer);
	}

	return;
}

//�����洢
bool CWHDataQueue::RectifyBuffer(DWORD dwNeedSize)
{
	try
	{
		//�����ж�
		if ((m_dwDataSize+dwNeedSize)>m_dwBufferSize) throw 0;

		//���¿�ʼ
		if ((m_dwInsertPos==m_dwTerminalPos)&&((m_dwInsertPos+dwNeedSize)>m_dwBufferSize)) 
		{
			if (m_dwDataQueryPos>=dwNeedSize) m_dwInsertPos=0;
			else throw 0;
		}

		//�����ж�
		if ((m_dwInsertPos<m_dwTerminalPos)&&((m_dwInsertPos+dwNeedSize)>m_dwDataQueryPos)) throw 0;

		//�޸�����bug
		if (m_dwInsertPos + dwNeedSize > m_dwDataQueryPos && m_dwDataQueryPos >= m_dwInsertPos)
		{
			//β׷��ͷ
			if (m_dwDataSize > 0){
				//ASSERT(false);
				throw 0;
			}
		}
	}
	catch (...)
	{
		try
		{
			//�����ڴ�
			DWORD dwRiseSize=__max(m_dwBufferSize/2L,dwNeedSize*10L);
			LPBYTE pNewQueueServiceBuffer=new BYTE [m_dwBufferSize+dwRiseSize];

			//�����ж�
			ASSERT(pNewQueueServiceBuffer!=NULL);
			if (pNewQueueServiceBuffer==NULL) return false;

			//��������
			if (m_pDataQueueBuffer!=NULL) 
			{
				//Ч��״̬
				ASSERT(m_dwTerminalPos>=m_dwDataSize);
				ASSERT(m_dwTerminalPos>=m_dwDataQueryPos);

				//��������
				DWORD dwPartOneSize=m_dwTerminalPos-m_dwDataQueryPos;
				if (dwPartOneSize>0L) CopyMemory(pNewQueueServiceBuffer,m_pDataQueueBuffer+m_dwDataQueryPos,dwPartOneSize);
				if (m_dwDataSize>dwPartOneSize)
				{
					ASSERT((m_dwInsertPos+dwPartOneSize)==m_dwDataSize);
					CopyMemory(pNewQueueServiceBuffer+dwPartOneSize,m_pDataQueueBuffer,m_dwInsertPos);
				}
			}

			//���ñ���
			m_dwDataQueryPos=0L;
			m_dwInsertPos=m_dwDataSize;
			m_dwTerminalPos=m_dwDataSize;
			m_dwBufferSize=m_dwBufferSize+dwRiseSize;

			//���û���
			SafeDeleteArray(m_pDataQueueBuffer);
			m_pDataQueueBuffer=pNewQueueServiceBuffer;
		}
		catch (...) { return false; }
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
