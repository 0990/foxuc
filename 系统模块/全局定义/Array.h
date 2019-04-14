#ifndef ARRAY_HEAD_FILE
#define ARRAY_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//����ģ����
template <class TYPE, class ARG_TYPE=const TYPE &> class CWHArray
{
	//��������
protected:
	TYPE *							m_pData;							//����ָ��
	INT_PTR							m_nMaxCount;						//������Ŀ
	INT_PTR							m_nGrowCount;						//������Ŀ
	INT_PTR							m_nElementCount;					//Ԫ����Ŀ

	//��������
public:
	//���캯��
	CWHArray();
	//��������
	virtual ~CWHArray();

	//��Ϣ����
public:
	//�Ƿ����
	bool IsEmpty() const;
	//��ȡ��Ŀ
	INT_PTR GetCount() const;

	//���ܺ���
public:
	//��ȡ����
	TYPE * GetData();
	//��ȡ����
	const TYPE * GetData() const;
	//����Ԫ��
	INT_PTR Add(ARG_TYPE newElement);
	//��������
	VOID Copy(const CWHArray & Src);
	//׷������
	INT_PTR Append(const CWHArray & Src);
	//��ȡԪ��
	TYPE & GetAt(INT_PTR nIndex);
	//��ȡԪ��
	const TYPE & GetAt(INT_PTR nIndex) const;
	//��ȡԪ��
	TYPE & ElementAt(INT_PTR nIndex);
	//��ȡԪ��
	const TYPE & ElementAt(INT_PTR nIndex) const;

	//��������
public:
	//���ô�С
	VOID SetSize(INT_PTR nNewSize);
	//����Ԫ��
	VOID SetAt(INT_PTR nIndex, ARG_TYPE newElement);
	//����Ԫ��
	VOID SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement);
	//��������
	VOID InsertAt(INT_PTR nIndex, const CWHArray & Src);
	//��������
	VOID InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount=1);
	//ɾ������
	VOID RemoveAt(INT_PTR nIndex, INT_PTR nCount=1);
	//ɾ��Ԫ��
	VOID RemoveAll();

	//��������
public:
	//��������
	TYPE & operator[](INT_PTR nIndex);
	//��������
	const TYPE & operator[](INT_PTR nIndex) const;

	//�ڴ溯��
public:
	//�ͷ��ڴ�
	VOID FreeMemory();
	//�����ڴ�
	VOID AllocMemory(INT_PTR nNewCount);
};

//////////////////////////////////////////////////////////////////////////////////
// CWHArray<TYPE, ARG_TYPE> ��������

//�Ƿ����
template<class TYPE, class ARG_TYPE> 
AFX_INLINE bool CWHArray<TYPE, ARG_TYPE>::IsEmpty() const
{
	return (m_nElementCount==0);
}

//��ȡ��Ŀ
template<class TYPE, class ARG_TYPE>
AFX_INLINE INT_PTR CWHArray<TYPE, ARG_TYPE>::GetCount() const
{
	return m_nElementCount;
}

//����Ԫ��
template<class TYPE, class ARG_TYPE>
AFX_INLINE INT_PTR CWHArray<TYPE,ARG_TYPE>::Add(ARG_TYPE newElement)
{
	INT_PTR nIndex=m_nElementCount;
	SetAtGrow(nIndex,newElement);
	return nIndex;
}

//��������
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE & CWHArray<TYPE, ARG_TYPE>::operator[](INT_PTR nIndex)
{ 
	return ElementAt(nIndex);
}

//��������
template<class TYPE, class ARG_TYPE>
AFX_INLINE const TYPE & CWHArray<TYPE, ARG_TYPE>::operator[](INT_PTR nIndex) const
{ 
	return GetAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////////////
// CWHArray<TYPE, ARG_TYPE> ��������

//���캯��
template<class TYPE, class ARG_TYPE> 
CWHArray<TYPE, ARG_TYPE>::CWHArray()
{
	m_pData=NULL;
	m_nMaxCount=0;
	m_nGrowCount=0;
	m_nElementCount=0;

	return;
}

//���캯��
template<class TYPE, class ARG_TYPE> 
CWHArray<TYPE,ARG_TYPE>::~CWHArray()
{
	if (m_pData!=NULL)
	{
		for (INT_PTR i=0;i<m_nElementCount;i++)	(m_pData+i)->~TYPE();
		delete [] (BYTE *)m_pData;
		m_pData=NULL;
	}

	return;
}

//��ȡ����
template<class TYPE, class ARG_TYPE> 
TYPE * CWHArray<TYPE,ARG_TYPE>::GetData()
{
	return m_pData;
}

//��ȡ����
template<class TYPE, class ARG_TYPE> 
const TYPE * CWHArray<TYPE,ARG_TYPE>::GetData() const
{
	return m_pData;
}

//��������
template<class TYPE, class ARG_TYPE> 
VOID CWHArray<TYPE,ARG_TYPE>::Copy(const CWHArray & Src)
{
	//Ч�����
	ASSERT(this!=&Src);
	if (this==&Src) return;

	//��������
	AllocMemory(Src.m_nElementCount);
	if (m_nElementCount>0)
	{
		for (INT_PTR i=0;i<m_nElementCount;i++) (m_pData+i)->~TYPE();
		memset(m_pData,0,m_nElementCount*sizeof(TYPE));
	}
	for (INT_PTR i=0;i<Src.m_nElementCount;i++)	m_pData[i]=Src.m_pData[i];
	m_nElementCount=Src.m_nElementCount;

	return;
}

//׷������
template<class TYPE, class ARG_TYPE> 
INT_PTR CWHArray<TYPE,ARG_TYPE>::Append(const CWHArray & Src)
{
	//Ч�����
	ASSERT(this!=&Src);
	if (this==&Src) AfxThrowInvalidArgException();

	//��������
	if (Src.m_nElementCount>0)
	{
		INT_PTR nOldCount=m_nElementCount;
		AllocMemory(m_nElementCount+Src.m_nElementCount);
		for (INT_PTR i=0;i<Src.m_nElementCount;i++)	m_pData[m_nElementCount+i]=Src.m_pData[i];
		m_nElementCount+=Src.m_nElementCount;
	}

	return m_nElementCount;
}

//��ȡԪ��
template<class TYPE, class ARG_TYPE> 
TYPE & CWHArray<TYPE,ARG_TYPE>::GetAt(INT_PTR nIndex)
{
	ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	if ((nIndex<0)||(nIndex>=m_nElementCount)) AfxThrowInvalidArgException();
	
	return m_pData[nIndex];
}

//��ȡԪ��
template<class TYPE, class ARG_TYPE> 
const TYPE & CWHArray<TYPE,ARG_TYPE>::GetAt(INT_PTR nIndex) const
{
	ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	if ((nIndex<0)||(nIndex>=m_nElementCount)) AfxThrowInvalidArgException();
	
	return m_pData[nIndex];
}

//��ȡԪ��
template<class TYPE, class ARG_TYPE> 
TYPE & CWHArray<TYPE,ARG_TYPE>::ElementAt(INT_PTR nIndex)
{
	ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	if ((nIndex<0)&&(nIndex>=m_nElementCount)) AfxThrowInvalidArgException();
	
	return m_pData[nIndex];
}

//��ȡԪ��
template<class TYPE, class ARG_TYPE> 
const TYPE & CWHArray<TYPE,ARG_TYPE>::ElementAt(INT_PTR nIndex) const
{
	ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	if ((nIndex<0)&&(nIndex>=m_nElementCount)) AfxThrowInvalidArgException();

	return m_pData[nIndex];
}

//���ô�С
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::SetSize(INT_PTR nNewSize)
{
	//Ч�����
	ASSERT(nNewSize>=0);
	if (nNewSize<0)	AfxThrowInvalidArgException();
	
	//���ô�С
	AllocMemory(nNewSize);
	if (nNewSize>m_nElementCount)
	{
		for (INT_PTR i=m_nElementCount;i<nNewSize;i++) new ((VOID *)(m_pData+i)) TYPE;
	}
	else if (nNewSize<m_nElementCount)
	{
		for (INT_PTR i=nNewSize;i<m_nElementCount;i++) (m_pData+i)->~TYPE();
		memset(m_pData+nNewSize,0,(m_nElementCount-nNewSize)*sizeof(TYPE));
	}
	m_nElementCount=nNewSize;

	return;
}

//����Ԫ��
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::SetAt(INT_PTR nIndex, ARG_TYPE newElement)
{
	ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	if ((nIndex>=0)&&(nIndex<m_nElementCount)) m_pData[nIndex]=newElement;
	else AfxThrowInvalidArgException();

	return;
}

//����Ԫ��
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement)
{
	//Ч�����
	ASSERT(nIndex>=0);
	if (nIndex<0) AfxThrowInvalidArgException();

	//����Ԫ��
	if (nIndex>=m_nElementCount) SetSize(m_nElementCount+1);
	m_pData[nIndex]=newElement;

	return;
}

//��������
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::InsertAt(INT_PTR nIndex, const CWHArray & Src)
{
	//Ч�����
	ASSERT(nStartIndex>=0);
	if (nStartIndex<0) AfxThrowInvalidArgException();

	if (Src.m_nElementCount>0)
	{
		//��������
		if (nIndex<m_nElementCount)
		{
			INT_PTR nOldCount=m_nElementCount;
			SetSize(m_nElementCount+Src.m_nElementCount);
			for (INT_PTR i=0;i<nCount;i++) (m_pData+nOldCount+i)->~TYPE();
			memmove(m_pData+nIndex+nCount,m_pData+nIndex,(nOldCount-nIndex)*sizeof(TYPE));
			memset(m_pData+nIndex,0,Src.m_nElementCount*sizeof(TYPE));
			for (INT_PTR i=0;i<Src.m_nElementCount;i++) new (m_pData+nIndex+i) TYPE();
		}
		else SetSize(nIndex+nCount);

		//��������
		ASSERT((nIndex+Src.m_nElementCount)<=m_nElementCount);
		while (nCount--) m_pData[nIndex++]=newElement;
	}

	return;
}

//��������
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount)
{
	//Ч�����
	ASSERT(nIndex>=0);
	ASSERT(nCount>0);
	if ((nIndex<0)||(nCount<=0)) AfxThrowInvalidArgException();

	//��������
	if (nIndex<m_nElementCount)
	{
		INT_PTR nOldCount=m_nElementCount;
		SetSize(m_nElementCount+nCount);
		for (INT_PTR i=0;i<nCount;i++) (m_pData+nOldCount+i)->~TYPE();
		memmove(m_pData+nIndex+nCount,m_pData+nIndex,(nOldCount-nIndex)*sizeof(TYPE));
		memset(m_pData+nIndex,0,nCount*sizeof(TYPE));
		for (INT_PTR i=0;i<nCount;i++) new (m_pData+nIndex+i) TYPE();
	}
	else SetSize(nIndex+nCount);

	//��������
	ASSERT((nIndex+nCount)<=m_nElementCount);
	while (nCount--) m_pData[nIndex++]=newElement;

	return;
}

//ɾ������
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::RemoveAt(INT_PTR nIndex, INT_PTR nCount)
{
	//Ч�����
	ASSERT(nIndex>=0);
	ASSERT(nCount>=0);
	ASSERT(nIndex+nCount<=m_nElementCount);
	if ((nIndex<0)||(nCount<0)||((nIndex+nCount>m_nElementCount))) AfxThrowInvalidArgException();

	//ɾ������
	INT_PTR nMoveCount=m_nElementCount-(nIndex+nCount);
	for (INT_PTR i=0;i<nCount;i++) (m_pData+nIndex+i)->~TYPE();
	if (nMoveCount>0) memmove(m_pData+nIndex,m_pData+nIndex+nCount,nMoveCount*sizeof(TYPE));
	m_nElementCount-=nCount;

	return;
}

//ɾ��Ԫ��
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::RemoveAll()
{
	if (m_nElementCount>0)
	{
		for (INT_PTR i=0;i<m_nElementCount;i++) (m_pData+i)->~TYPE();
		memset(m_pData,0,m_nElementCount*sizeof(TYPE));
		m_nElementCount=0;
	}

	return;
}

//�ͷ��ڴ�
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::FreeMemory()
{
	if (m_nElementCount!=m_nMaxCount)
	{
		TYPE * pNewData=NULL;
		if (m_nElementCount!=0)
		{
			pNewData=(TYPE *) new BYTE[m_nElementCount*sizeof(TYPE)];
			memcpy(pNewData,m_pData,m_nElementCount*sizeof(TYPE));
		}
		delete [] (BYTE *)m_pData;
		m_pData=pNewData;
		m_nMaxCount=m_nElementCount;
	}

	return;
}

//�����ڴ�
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE,ARG_TYPE>::AllocMemory(INT_PTR nNewCount)
{
	//Ч�����
	ASSERT(nNewCount>=0);

	if (nNewCount>m_nMaxCount)
	{
		//������Ŀ
		INT_PTR nGrowCount=m_nGrowCount;
		if (nGrowCount==0)
		{
			nGrowCount=m_nElementCount/8;
			nGrowCount=(nGrowCount<4)?4:((nGrowCount>1024)?1024:nGrowCount);
		}
		nNewCount+=nGrowCount;

		//�����ڴ�
		TYPE * pNewData=(TYPE *) new BYTE[nNewCount*sizeof(TYPE)];
		memcpy(pNewData,m_pData,m_nElementCount*sizeof(TYPE));
		memset(pNewData+m_nElementCount,0,(nNewCount-m_nElementCount)*sizeof(TYPE));
		delete [] (BYTE *)m_pData;

		//���ñ���
		m_pData=pNewData;
		m_nMaxCount=nNewCount;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

#endif