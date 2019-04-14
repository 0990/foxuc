// MD5.h: interface for the CMD5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MD5_H__65D176D4_F34D_48DA_8596_2595F41C94DE__INCLUDED_)
#define AFX_MD5_H__65D176D4_F34D_48DA_8596_2595F41C94DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Define_Stdafx.h"
#include <Windows.h>
//MD5数据结构
typedef struct 
	{
		DWORD	state[4];				/*状态数组 (ABCD) */
		DWORD	count[2];				/* bits数, 以2^64 为模*/
		BYTE	buffer[64];				/* 输入缓冲器 */
	}MD5;

class CMD5
{
public:
    char* MD5_Algorithm(const char* sources);
    
public:
	CMD5();
	virtual ~CMD5();
    
private:
	void MD5Final (BYTE digest[16], MD5 *context);
    void MD5_memset (BYTE *output, int value, DWORD len);
	void MD5_memcpy (BYTE * output, BYTE * input, DWORD len);
	void Decode (DWORD *output, BYTE *input, DWORD len);
	void Encode (BYTE *output, DWORD *input, DWORD len);
	void MD5Update (MD5 *context, BYTE *input, DWORD inputLen);
	void MD5Init (MD5 *context);
	void MD5Transform (DWORD state[4], BYTE block[64]);	
};




#endif // !defined(AFX_MD5_H__65D176D4_F34D_48DA_8596_2595F41C94DE__INCLUDED_)
