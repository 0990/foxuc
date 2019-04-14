#include "StdAfx.h"
#include "WHDataQueue.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWHDataQueue::CWHDataQueue()
{
	//查询变量
	m_dwInsertPos=0L;
	m_dwTerminalPos=0L;
	m_dwDataQueryPos=0L;

	//数据变量
	m_dwDataSize=0L;
	m_dwDataPacketCount=0L;

	//缓冲变量
	m_dwBufferSize=0L;
	m_pDataQueueBuffer=NULL;

	return;
}

//析构函数
CWHDataQueue::~CWHDataQueue()
{
	SafeDeleteArray(m_pDataQueueBuffer);
	return;
}

//负荷信息
VOID CWHDataQueue::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	//设置变量
	BurthenInfo.dwDataSize=m_dwDataSize;
	BurthenInfo.dwBufferSize=m_dwBufferSize;
	BurthenInfo.dwDataPacketCount=m_dwDataPacketCount;

	return;
}

//插入数据
bool CWHDataQueue::InsertData(WORD wIdentifier, VOID * pBuffer, WORD wDataSize)
{
	//变量定义
	tagDataHead DataHead;
	ZeroMemory(&DataHead,sizeof(DataHead));

	//设置变量
	DataHead.wDataSize=wDataSize;
	DataHead.wIdentifier=wIdentifier;

	//调整存储
	if (RectifyBuffer(sizeof(DataHead)+DataHead.wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//插入数据
	try
	{
		//拷贝数据
		CopyMemory(m_pDataQueueBuffer+m_dwInsertPos,&DataHead,sizeof(DataHead));

		//附加数据
		if (wDataSize>0) 
		{
			ASSERT(pBuffer!=NULL);
			CopyMemory(m_pDataQueueBuffer+m_dwInsertPos+sizeof(DataHead),pBuffer,wDataSize);
		}

		//调整数据
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

//插入数据
bool CWHDataQueue::InsertData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount)
{
	//变量定义
	tagDataHead DataHead;
	ZeroMemory(&DataHead,sizeof(DataHead));

	//设置变量
	DataHead.wDataSize=0;
	DataHead.wIdentifier=wIdentifier;

	//累计大小
	for (WORD i=0;i<wDataCount;i++)
	{
		if (DataBuffer[i].wDataSize>0)
		{
			DataHead.wDataSize+=DataBuffer[i].wDataSize;
		}
	}

	//调整存储
	if (RectifyBuffer(sizeof(DataHead)+DataHead.wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//插入数据
	try
	{
		//拷贝数据
		CopyMemory(m_pDataQueueBuffer+m_dwInsertPos,&DataHead,sizeof(DataHead));

		//附加数据
		if (DataHead.wDataSize>0)
		{
			//变量定义
			WORD wExcursion=sizeof(DataHead);

			//插入数据
			for (WORD i=0;i<wDataCount;i++)
			{
				if (DataBuffer[i].wDataSize>0)
				{
					//效验状态
					ASSERT(DataBuffer[i].pDataBuffer!=NULL);

					//拷贝数据
					CopyMemory(m_pDataQueueBuffer+m_dwInsertPos+wExcursion,DataBuffer[i].pDataBuffer,DataBuffer[i].wDataSize);

					//设置变量
					wExcursion+=DataBuffer[i].wDataSize;
				}
			}
		}

		//调整数据
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

//获取数据
bool CWHDataQueue::DistillData(tagDataHead & DataHead, VOID * pBuffer, WORD wBufferSize)
{
	//效验变量
	ASSERT(m_dwDataSize>0L);
	ASSERT(m_dwDataPacketCount>0);
	ASSERT(m_pDataQueueBuffer!=NULL);

	//效验变量
	if (m_dwDataSize==0L) return false;
	if (m_dwDataPacketCount==0L) return false;

	//调整参数
	if (m_dwDataQueryPos==m_dwTerminalPos)
	{
		m_dwDataQueryPos=0L;
		m_dwTerminalPos=m_dwInsertPos;
	}

	//获取指针
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+sizeof(tagDataHead)));
	tagDataHead * pDataHead=(tagDataHead *)(m_pDataQueueBuffer+m_dwDataQueryPos);
	ASSERT(wBufferSize>=pDataHead->wDataSize);

	//获取大小
	WORD wPacketSize=sizeof(DataHead)+pDataHead->wDataSize;
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+wPacketSize));

	//判断缓冲
	WORD wCopySize=0;
	ASSERT(wBufferSize>=pDataHead->wDataSize);
	if (wBufferSize>=pDataHead->wDataSize) wCopySize=pDataHead->wDataSize;

	//拷贝数据
	DataHead=*pDataHead;
	if (DataHead.wDataSize>0)
	{
		if (wBufferSize<pDataHead->wDataSize) DataHead.wDataSize=0;
		else CopyMemory(pBuffer,pDataHead+1,DataHead.wDataSize);
	}

	//效验参数
	ASSERT(wPacketSize<=m_dwDataSize);
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+wPacketSize));

	//设置变量
	m_dwDataPacketCount--;
	m_dwDataSize-=wPacketSize;
	m_dwDataQueryPos+=wPacketSize;

	return true;
}

//删除数据
VOID CWHDataQueue::RemoveData(bool bFreeMemroy)
{
	//设置变量
	m_dwDataSize=0L;
	m_dwInsertPos=0L;
	m_dwTerminalPos=0L;
	m_dwDataQueryPos=0L;
	m_dwDataPacketCount=0L;

	//删除内存
	if (bFreeMemroy==true) 
	{
		m_dwBufferSize=0;
		SafeDeleteArray(m_pDataQueueBuffer);
	}

	return;
}

//调整存储
bool CWHDataQueue::RectifyBuffer(DWORD dwNeedSize)
{
	try
	{
		//缓冲判断
		if ((m_dwDataSize+dwNeedSize)>m_dwBufferSize) throw 0;

		//重新开始
		if ((m_dwInsertPos==m_dwTerminalPos)&&((m_dwInsertPos+dwNeedSize)>m_dwBufferSize)) 
		{
			if (m_dwDataQueryPos>=dwNeedSize) m_dwInsertPos=0;
			else throw 0;
		}

		//缓冲判断
		if ((m_dwInsertPos<m_dwTerminalPos)&&((m_dwInsertPos+dwNeedSize)>m_dwDataQueryPos)) throw 0;

		//修复网狐bug
		if (m_dwInsertPos + dwNeedSize > m_dwDataQueryPos && m_dwDataQueryPos >= m_dwInsertPos)
		{
			//尾追上头
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
			//申请内存
			DWORD dwRiseSize=__max(m_dwBufferSize/2L,dwNeedSize*10L);
			LPBYTE pNewQueueServiceBuffer=new BYTE [m_dwBufferSize+dwRiseSize];

			//错误判断
			ASSERT(pNewQueueServiceBuffer!=NULL);
			if (pNewQueueServiceBuffer==NULL) return false;

			//拷贝数据
			if (m_pDataQueueBuffer!=NULL) 
			{
				//效验状态
				ASSERT(m_dwTerminalPos>=m_dwDataSize);
				ASSERT(m_dwTerminalPos>=m_dwDataQueryPos);

				//拷贝数据
				DWORD dwPartOneSize=m_dwTerminalPos-m_dwDataQueryPos;
				if (dwPartOneSize>0L) CopyMemory(pNewQueueServiceBuffer,m_pDataQueueBuffer+m_dwDataQueryPos,dwPartOneSize);
				if (m_dwDataSize>dwPartOneSize)
				{
					ASSERT((m_dwInsertPos+dwPartOneSize)==m_dwDataSize);
					CopyMemory(pNewQueueServiceBuffer+dwPartOneSize,m_pDataQueueBuffer,m_dwInsertPos);
				}
			}

			//设置变量
			m_dwDataQueryPos=0L;
			m_dwInsertPos=m_dwDataSize;
			m_dwTerminalPos=m_dwDataSize;
			m_dwBufferSize=m_dwBufferSize+dwRiseSize;

			//设置缓冲
			SafeDeleteArray(m_pDataQueueBuffer);
			m_pDataQueueBuffer=pNewQueueServiceBuffer;
		}
		catch (...) { return false; }
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
