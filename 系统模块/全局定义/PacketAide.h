#ifndef PACKET_AIDE_HEAD_FILE
#define PACKET_AIDE_HEAD_FILE

#pragma once

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

//数据定义
#define DTP_NULL					0								//无效数据

//数据描述
struct tagDataDescribe
{
	WORD							wDataSize;						//数据大小
	WORD							wDataDescribe;					//数据描述
};

//////////////////////////////////////////////////////////////////////////////////

//发送辅助类
class CSendPacketHelper
{
	//变量定义
protected:
	WORD							m_wDataSize;					//数据大小
	WORD							m_wMaxBytes;					//缓冲大小
	LPBYTE							m_pcbBuffer;					//缓冲指针

	//函数定义
public:
	//构造函数
	inline CSendPacketHelper(VOID * pcbBuffer, WORD wMaxBytes);

	//功能函数
public:
	//清理数据
	inline VOID CleanData() { m_wDataSize=0; }
	//获取大小
	inline WORD GetDataSize() { return m_wDataSize; }
	//获取缓冲
	inline VOID * GetDataBuffer() { return m_pcbBuffer; }

	//功能函数
public:
	//插入字符
	inline bool AddPacket(LPCSTR pszString, WORD wDataType);
	//插入字符
	inline bool AddPacket(LPCWSTR pszString, WORD wDataType);
	//插入数据
	inline bool AddPacket(VOID * pData, WORD wDataSize, WORD wDataType);
};

//////////////////////////////////////////////////////////////////////////////////

//接收辅助类
class CRecvPacketHelper
{
	//变量定义
protected:
	WORD							m_wDataPos;						//数据点
	WORD							m_wDataSize;					//数据大小
	LPBYTE							m_pcbBuffer;					//缓冲指针

	//函数定义
public:
	//构造函数
	inline CRecvPacketHelper(LPCVOID pcbBuffer, WORD wDataSize);

	//功能函数
public:
	//获取数据
	inline VOID * GetData(tagDataDescribe & DataDescribe);
};

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSendPacketHelper::CSendPacketHelper(VOID * pcbBuffer, WORD wMaxBytes)
{
	//设置变量
	m_wDataSize=0;
	m_wMaxBytes=wMaxBytes;
	m_pcbBuffer=(BYTE *)pcbBuffer;

	return;
}

//插入字符
bool CSendPacketHelper::AddPacket(LPCSTR pszString, WORD wDataType)
{
	//参数判断
	ASSERT(pszString!=NULL);
	if (pszString[0]==0) return true;

	//插入数据
	return AddPacket((VOID *)pszString,CountStringBufferA(pszString),wDataType);
}

//插入字符
bool CSendPacketHelper::AddPacket(LPCWSTR pszString, WORD wDataType)
{
	//参数判断
	ASSERT(pszString!=NULL);
	if (pszString[0]==0) return true;

	//插入数据
	return AddPacket((VOID *)pszString,CountStringBufferW(pszString),wDataType);
}

//插入数据
bool CSendPacketHelper::AddPacket(VOID * pData, WORD wDataSize, WORD wDataType)
{
	//效验数据
	ASSERT(wDataType!=DTP_NULL);
	ASSERT((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)<=m_wMaxBytes);
	if ((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)>m_wMaxBytes) return false;

	//插入数据
	ASSERT(m_pcbBuffer!=NULL);
	tagDataDescribe * pDataDescribe=(tagDataDescribe *)(m_pcbBuffer+m_wDataSize);
	pDataDescribe->wDataSize=wDataSize;
	pDataDescribe->wDataDescribe=wDataType;

	//插入数据
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(pDataDescribe+1,pData,wDataSize);
	}

	//设置数据
	m_wDataSize+=sizeof(tagDataDescribe)+wDataSize;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CRecvPacketHelper::CRecvPacketHelper(LPCVOID pcbBuffer, WORD wDataSize)
{
	//设置变量
	m_wDataPos=0;
	m_wDataSize=wDataSize;
	m_pcbBuffer=(BYTE *)pcbBuffer;

	return;
}

//获取数据
VOID * CRecvPacketHelper::GetData(tagDataDescribe & DataDescribe)
{
	//效验数据
	if (m_wDataPos>=m_wDataSize) 
	{
		ASSERT(m_wDataPos==m_wDataSize);
		DataDescribe.wDataSize=0;
		DataDescribe.wDataDescribe=DTP_NULL;
		return NULL;
	}

	//获取数据
	ASSERT((m_wDataPos+sizeof(tagDataDescribe))<=m_wDataSize);
	CopyMemory(&DataDescribe,m_pcbBuffer+m_wDataPos,sizeof(tagDataDescribe));
	ASSERT((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)<=m_wDataSize);

	//效验数据
	if ((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)>m_wDataSize)
	{
		DataDescribe.wDataSize=0;
		DataDescribe.wDataDescribe=DTP_NULL;
		return NULL;
	}

	//设置数据
	VOID * pData=NULL;
	if (DataDescribe.wDataSize>0) pData=m_pcbBuffer+m_wDataPos+sizeof(tagDataDescribe);
	m_wDataPos+=sizeof(tagDataDescribe)+DataDescribe.wDataSize;

	return pData;
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif