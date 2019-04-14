#include "StdAfx.h"
#include "WHEncrypt.h"

//////////////////////////////////////////////////////////////////////////////////
//常量定义

//加密常量
#define S11							7									//数据常量
#define S12							12									//数据常量
#define S13							17									//数据常量
#define S14							22									//数据常量
#define S21							5									//数据常量
#define S22							9									//数据常量
#define S23 						14									//数据常量
#define S24 						20									//数据常量
#define S31 						4									//数据常量
#define S32 						11									//数据常量
#define S33 						16									//数据常量
#define S34 						23									//数据常量
#define S41 						6									//数据常量
#define S42 						10									//数据常量
#define S43 						15									//数据常量
#define S44 						21									//数据常量

//密钥长度
#define ENCRYPT_KEY_LEN				8									//密钥长度

//////////////////////////////////////////////////////////////////////////////////

//函数定义
#define H(x,y,z) ((x)^(y)^(z))
#define I(x,y,z) ((y)^((x)|(~z)))
#define F(x,y,z) (((x)&(y))|((~x)&(z)))
#define G(x,y,z) (((x)&(z))|((y)&(~z)))
#define ROTATE_LEFT(x,n) (((x)<<(n))|((x)>>(32-(n))))

//函数定义
#define FF(a,b,c,d,x,s,ac)												\
{																		\
	(a)+=F((b),(c),(d))+(x)+(ULONG)(ac);								\
	(a)=ROTATE_LEFT((a),(s));											\
	(a)+=(b);															\
}

//函数定义
#define GG(a,b,c,d,x,s,ac)												\
{																		\
	(a)+=G((b),(c),(d))+(x)+(ULONG)(ac);								\
	(a)=ROTATE_LEFT ((a),(s));											\
	(a)+=(b);															\
}

//函数定义
#define HH(a,b,c,d,x,s,ac)												\
{																		\
	(a)+=H((b),(c),(d))+(x)+(ULONG)(ac);								\
	(a)=ROTATE_LEFT((a),(s));											\
	(a)+=(b);															\
}

//函数定义
#define II(a,b,c,d,x,s,ac)												\
{																		\
	(a)+=I((b),(c),(d))+(x)+(ULONG)(ac);								\
	(a)=ROTATE_LEFT((a),(s));											\
	(a)+=(b);															\
}

//////////////////////////////////////////////////////////////////////////////////

//加密映射
const BYTE g_cbEncryptMap[256]=
{
	0x70,0x2F,0x40,0x5F,0x44,0x8E,0x6E,0x45,0x7E,0xAB,0x2C,0x1F,0xB4,0xAC,0x9D,0x91,
	0x0D,0x36,0x9B,0x0B,0xD4,0xC4,0x39,0x74,0xBF,0x23,0x16,0x14,0x06,0xEB,0x04,0x3E,
	0x12,0x5C,0x8B,0xBC,0x61,0x63,0xF6,0xA5,0xE1,0x65,0xD8,0xF5,0x5A,0x07,0xF0,0x13,
	0xF2,0x20,0x6B,0x4A,0x24,0x59,0x89,0x64,0xD7,0x42,0x6A,0x5E,0x3D,0x0A,0x77,0xE0,
	0x80,0x27,0xB8,0xC5,0x8C,0x0E,0xFA,0x8A,0xD5,0x29,0x56,0x57,0x6C,0x53,0x67,0x41,
	0xE8,0x00,0x1A,0xCE,0x86,0x83,0xB0,0x22,0x28,0x4D,0x3F,0x26,0x46,0x4F,0x6F,0x2B,
	0x72,0x3A,0xF1,0x8D,0x97,0x95,0x49,0x84,0xE5,0xE3,0x79,0x8F,0x51,0x10,0xA8,0x82,
	0xC6,0xDD,0xFF,0xFC,0xE4,0xCF,0xB3,0x09,0x5D,0xEA,0x9C,0x34,0xF9,0x17,0x9F,0xDA,
	0x87,0xF8,0x15,0x05,0x3C,0xD3,0xA4,0x85,0x2E,0xFB,0xEE,0x47,0x3B,0xEF,0x37,0x7F,
	0x93,0xAF,0x69,0x0C,0x71,0x31,0xDE,0x21,0x75,0xA0,0xAA,0xBA,0x7C,0x38,0x02,0xB7,
	0x81,0x01,0xFD,0xE7,0x1D,0xCC,0xCD,0xBD,0x1B,0x7A,0x2A,0xAD,0x66,0xBE,0x55,0x33,
	0x03,0xDB,0x88,0xB2,0x1E,0x4E,0xB9,0xE6,0xC2,0xF7,0xCB,0x7D,0xC9,0x62,0xC3,0xA6,
	0xDC,0xA7,0x50,0xB5,0x4B,0x94,0xC0,0x92,0x4C,0x11,0x5B,0x78,0xD9,0xB1,0xED,0x19,
	0xE9,0xA1,0x1C,0xB6,0x32,0x99,0xA3,0x76,0x9E,0x7B,0x6D,0x9A,0x30,0xD6,0xA9,0x25,
	0xC7,0xAE,0x96,0x35,0xD0,0xBB,0xD2,0xC8,0xA2,0x08,0xF3,0xD1,0x73,0xF4,0x48,0x2D,
	0x90,0xCA,0xE2,0x58,0xC1,0x18,0x52,0xFE,0xDF,0x68,0x98,0x54,0xEC,0x60,0x43,0x0F
};

//解密映射
const BYTE g_cbCrevasseMap[256]=
{
	0x51,0xA1,0x9E,0xB0,0x1E,0x83,0x1C,0x2D,0xE9,0x77,0x3D,0x13,0x93,0x10,0x45,0xFF,
	0x6D,0xC9,0x20,0x2F,0x1B,0x82,0x1A,0x7D,0xF5,0xCF,0x52,0xA8,0xD2,0xA4,0xB4,0x0B,
	0x31,0x97,0x57,0x19,0x34,0xDF,0x5B,0x41,0x58,0x49,0xAA,0x5F,0x0A,0xEF,0x88,0x01,
	0xDC,0x95,0xD4,0xAF,0x7B,0xE3,0x11,0x8E,0x9D,0x16,0x61,0x8C,0x84,0x3C,0x1F,0x5A,
	0x02,0x4F,0x39,0xFE,0x04,0x07,0x5C,0x8B,0xEE,0x66,0x33,0xC4,0xC8,0x59,0xB5,0x5D,
	0xC2,0x6C,0xF6,0x4D,0xFB,0xAE,0x4A,0x4B,0xF3,0x35,0x2C,0xCA,0x21,0x78,0x3B,0x03,
	0xFD,0x24,0xBD,0x25,0x37,0x29,0xAC,0x4E,0xF9,0x92,0x3A,0x32,0x4C,0xDA,0x06,0x5E,
	0x00,0x94,0x60,0xEC,0x17,0x98,0xD7,0x3E,0xCB,0x6A,0xA9,0xD9,0x9C,0xBB,0x08,0x8F,
	0x40,0xA0,0x6F,0x55,0x67,0x87,0x54,0x80,0xB2,0x36,0x47,0x22,0x44,0x63,0x05,0x6B,
	0xF0,0x0F,0xC7,0x90,0xC5,0x65,0xE2,0x64,0xFA,0xD5,0xDB,0x12,0x7A,0x0E,0xD8,0x7E,
	0x99,0xD1,0xE8,0xD6,0x86,0x27,0xBF,0xC1,0x6E,0xDE,0x9A,0x09,0x0D,0xAB,0xE1,0x91,
	0x56,0xCD,0xB3,0x76,0x0C,0xC3,0xD3,0x9F,0x42,0xB6,0x9B,0xE5,0x23,0xA7,0xAD,0x18,
	0xC6,0xF4,0xB8,0xBE,0x15,0x43,0x70,0xE0,0xE7,0xBC,0xF1,0xBA,0xA5,0xA6,0x53,0x75,
	0xE4,0xEB,0xE6,0x85,0x14,0x48,0xDD,0x38,0x2A,0xCC,0x7F,0xB1,0xC0,0x71,0x96,0xF8,
	0x3F,0x28,0xF2,0x69,0x74,0x68,0xB7,0xA3,0x50,0xD0,0x79,0x1D,0xFC,0xCE,0x8A,0x8D,
	0x2E,0x62,0x30,0xEA,0xED,0x2B,0x26,0xB9,0x81,0x7C,0x46,0x89,0x73,0xA2,0xF7,0x72
};

//////////////////////////////////////////////////////////////////////////////////

//MD5 加密
class CMD5Aide
{
	//变量定义
private:
	ULONG							m_lCount[2];						//加密变量
	ULONG							m_lState[4];						//加密变量
	UCHAR							m_cbBuffer[64];						//加密变量
	UCHAR							m_cbPadding[64];					//加密变量

	//函数定义
public:
	//构造函数
	CMD5Aide() { MD5Init(); }

	//功能函数
public:
	//最终结果
	VOID MD5Final(BYTE digest[16]);
	//设置数值
	VOID MD5Update(BYTE * pcbInput, UINT inputLen);

	//内部函数
private:
	//初始化
	VOID MD5Init();
	//置位函数
	VOID MD5Memset(BYTE * pcbOutput, INT nValue, UINT nLen);
	//拷贝函数
	VOID MD5Memcpy(BYTE * pcbOutput, BYTE * pcbInput, UINT nLen);
	//转换函数
	VOID MD5Transform(ULONG lState[4], BYTE cbBlock[64]);

	//编码函数
private:
	//编码函数
	VOID Encode(BYTE * pcbOutput, ULONG * pcbInput, UINT nLen);
	//解码函数
	VOID Decode(ULONG * pcbOutput, BYTE * pcbInput, UINT nLen);
};

//////////////////////////////////////////////////////////////////////////////////

//初始化
VOID CMD5Aide::MD5Init()
{
	m_lCount[0]=0;
	m_lCount[1]=0;
	m_lState[0]=0x67452301;
	m_lState[1]=0xefcdab89;
	m_lState[2]=0x98badcfe;
	m_lState[3]=0x10325476;
	MD5Memset(m_cbPadding,0,sizeof(m_cbPadding));
	*m_cbPadding=0x80;

	return;
}

//更新函数
VOID CMD5Aide::MD5Update (BYTE * pcbInput, UINT inputLen)
{
	UINT i,nIndex,nPartLen;
	nIndex=(UINT)((this->m_lCount[0]>>3)&0x3F);
	if ((m_lCount[0]+=((ULONG)inputLen<<3))<((ULONG)inputLen<<3)) m_lCount[1]++;
	m_lCount[1]+=((ULONG)inputLen>>29);
	nPartLen=64-nIndex;
	if (inputLen>=nPartLen) 
	{
		MD5Memcpy((BYTE*)&m_cbBuffer[nIndex],(BYTE *)pcbInput,nPartLen);
		MD5Transform(m_lState,m_cbBuffer);
		for (i=nPartLen;i+63<inputLen;i+=64) MD5Transform(m_lState,&pcbInput[i]);
		nIndex=0;
	}
	else i=0;
	MD5Memcpy((BYTE*)&m_cbBuffer[nIndex],(BYTE *)&pcbInput[i],inputLen-i);

	return;
}

//最终结果
VOID CMD5Aide::MD5Final(BYTE digest[16])
{
	BYTE cbBits[8];
	UINT nIndex,nPartLen;
	Encode(cbBits,m_lCount,8);
	nIndex=(UINT)((m_lCount[0]>>3)&0x3f);
	nPartLen=(nIndex<56)?(56-nIndex):(120-nIndex);
	MD5Update( m_cbPadding,nPartLen);
	MD5Update(cbBits,8);
	Encode(digest,m_lState,16);
	MD5Memset((BYTE*)this,0,sizeof (*this));
	MD5Init();

	return;
}

//转换函数
VOID CMD5Aide::MD5Transform(ULONG lState[4], BYTE cbBlock[64])
{
	ULONG a=lState[0],b=lState[1],c=lState[2],d=lState[3],x[16];
	Decode(x,cbBlock,64);

	FF(a,b,c,d,x[ 0],S11,0xd76aa478); /* 1 */
	FF(d,a,b,c,x[ 1],S12,0xe8c7b756); /* 2 */
	FF(c,d,a,b,x[ 2],S13,0x242070db); /* 3 */
	FF(b,c,d,a,x[ 3],S14,0xc1bdceee); /* 4 */
	FF(a,b,c,d,x[ 4],S11,0xf57c0faf); /* 5 */
	FF(d,a,b,c,x[ 5],S12,0x4787c62a); /* 6 */
	FF(c,d,a,b,x[ 6],S13,0xa8304613); /* 7 */
	FF(b,c,d,a,x[ 7],S14,0xfd469501); /* 8 */
	FF(a,b,c,d,x[ 8],S11,0x698098d8); /* 9 */
	FF(d,a,b,c,x[ 9],S12,0x8b44f7af); /* 10 */
	FF(c,d,a,b,x[10],S13,0xffff5bb1); /* 11 */
	FF(b,c,d,a,x[11],S14,0x895cd7be); /* 12 */
	FF(a,b,c,d,x[12],S11,0x6b901122); /* 13 */
	FF(d,a,b,c,x[13],S12,0xfd987193); /* 14 */
	FF(c,d,a,b,x[14],S13,0xa679438e); /* 15 */
	FF(b,c,d,a,x[15],S14,0x49b40821); /* 16 */

	GG(a,b,c,d,x[ 1],S21,0xf61e2562); /* 17 */
	GG(d,a,b,c,x[ 6],S22,0xc040b340); /* 18 */
	GG(c,d,a,b,x[11],S23,0x265e5a51); /* 19 */
	GG(b,c,d,a,x[ 0],S24,0xe9b6c7aa); /* 20 */
	GG(a,b,c,d,x[ 5],S21,0xd62f105d); /* 21 */
	GG(d,a,b,c,x[10],S22,0x2441453);  /* 22 */
	GG(c,d,a,b,x[15],S23,0xd8a1e681); /* 23 */
	GG(b,c,d,a,x[ 4],S24,0xe7d3fbc8); /* 24 */
	GG(a,b,c,d,x[ 9],S21,0x21e1cde6); /* 25 */
	GG(d,a,b,c,x[14],S22,0xc33707d6); /* 26 */
	GG(c,d,a,b,x[ 3],S23,0xf4d50d87); /* 27 */
	GG(b,c,d,a,x[ 8],S24,0x455a14ed); /* 28 */
	GG(a,b,c,d,x[13],S21,0xa9e3e905); /* 29 */
	GG(d,a,b,c,x[ 2],S22,0xfcefa3f8); /* 30 */
	GG(c,d,a,b,x[ 7],S23,0x676f02d9); /* 31 */
	GG(b,c,d,a,x[12],S24,0x8d2a4c8a); /* 32 */

	HH(a,b,c,d,x[ 5],S31,0xfffa3942); /* 33 */
	HH(d,a,b,c,x[ 8],S32,0x8771f681); /* 34 */
	HH(c,d,a,b,x[11],S33,0x6d9d6122); /* 35 */
	HH(b,c,d,a,x[14],S34,0xfde5380c); /* 36 */
	HH(a,b,c,d,x[ 1],S31,0xa4beea44); /* 37 */
	HH(d,a,b,c,x[ 4],S32,0x4bdecfa9); /* 38 */
	HH(c,d,a,b,x[ 7],S33,0xf6bb4b60); /* 39 */
	HH(b,c,d,a,x[10],S34,0xbebfbc70); /* 40 */
	HH(a,b,c,d,x[13],S31,0x289b7ec6); /* 41 */
	HH(d,a,b,c,x[ 0],S32,0xeaa127fa); /* 42 */
	HH(c,d,a,b,x[ 3],S33,0xd4ef3085); /* 43 */
	HH(b,c,d,a,x[ 6],S34,0x4881d05);  /* 44 */
	HH(a,b,c,d,x[ 9],S31,0xd9d4d039); /* 45 */
	HH(d,a,b,c,x[12],S32,0xe6db99e5); /* 46 */
	HH(c,d,a,b,x[15],S33,0x1fa27cf8); /* 47 */
	HH(b,c,d,a,x[ 2],S34,0xc4ac5665); /* 48 */

	II(a,b,c,d,x[ 0],S41,0xf4292244); /* 49 */
	II(d,a,b,c,x[ 7],S42,0x432aff97); /* 50 */
	II(c,d,a,b,x[14],S43,0xab9423a7); /* 51 */
	II(b,c,d,a,x[ 5],S44,0xfc93a039); /* 52 */
	II(a,b,c,d,x[12],S41,0x655b59c3); /* 53 */
	II(d,a,b,c,x[ 3],S42,0x8f0ccc92); /* 54 */
	II(c,d,a,b,x[10],S43,0xffeff47d); /* 55 */
	II(b,c,d,a,x[ 1],S44,0x85845dd1); /* 56 */
	II(a,b,c,d,x[ 8],S41,0x6fa87e4f); /* 57 */
	II(d,a,b,c,x[15],S42,0xfe2ce6e0); /* 58 */
	II(c,d,a,b,x[ 6],S43,0xa3014314); /* 59 */
	II(b,c,d,a,x[13],S44,0x4e0811a1); /* 60 */
	II(a,b,c,d,x[ 4],S41,0xf7537e82); /* 61 */
	II(d,a,b,c,x[11],S42,0xbd3af235); /* 62 */
	II(c,d,a,b,x[ 2],S43,0x2ad7d2bb); /* 63 */
	II(b,c,d,a,x[ 9],S44,0xeb86d391); /* 64 */

	lState[0]+=a;
	lState[1]+=b;
	lState[2]+=c;
	lState[3]+=d;

	MD5Memset((BYTE *)x,0,sizeof(x));

	return;
}

//编码函数
VOID CMD5Aide::Encode(BYTE * pcbOutput, ULONG * pcbInput,UINT nLen)
{
	UINT i, j;
	for (i=0,j=0;j<nLen;i++,j+=4)
	{
		pcbOutput[j]=(BYTE)(pcbInput[i]&0xff);
		pcbOutput[j+1]=(BYTE)((pcbInput[i]>>8)&0xff);
		pcbOutput[j+2]=(BYTE)((pcbInput[i]>>16)&0xff);
		pcbOutput[j+3]=(BYTE)((pcbInput[i]>>24)&0xff);
	}
	return;
}

//解码函数
VOID CMD5Aide::Decode(ULONG * pcbOutput, BYTE * pcbInput, UINT nLen)
{
	UINT i,j;
	for (i=0,j=0;j<nLen;i++,j+=4)
	{
		pcbOutput[i]=((ULONG)pcbInput[j])|(((ULONG)pcbInput[j+1])<<8)|
			(((ULONG)pcbInput[j+2])<<16)|(((ULONG)pcbInput[j+3])<< 24);
	}
	return;
}

//拷贝函数
VOID CMD5Aide::MD5Memcpy(BYTE * pcbOutput, BYTE * pcbInput,UINT nLen)
{
	for (UINT i=0;i<nLen;i++) pcbOutput[i]=pcbInput[i];
}

//置位函数
VOID CMD5Aide::MD5Memset (BYTE * pcbOutput, INT nValue, UINT nLen)
{
	for (UINT i=0;i<nLen;i++) ((char *)pcbOutput)[i]=(char)nValue;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWHEncrypt::CWHEncrypt()
{
}

//析构函数
CWHEncrypt::~CWHEncrypt()
{
}

//生成密文
bool CWHEncrypt::MD5Encrypt(LPCTSTR pszSourceData, TCHAR szMD5Result[LEN_MD5])
{
	//变量定义
	CMD5Aide MD5Aide;
	UCHAR szResult[16];
	CT2CA strSrcData(pszSourceData);

	//加密密文
	MD5Aide.MD5Update((BYTE *)((LPCSTR)strSrcData),lstrlenA(strSrcData)*sizeof(CHAR));
	MD5Aide.MD5Final(szResult);

	//输出结果
	szMD5Result[0]=0;
	for (INT i=0;i<16;i++) _stprintf(&szMD5Result[i*2],TEXT("%02X"),szResult[i]);

	return true;
}

//生成密文
bool CWHEncrypt::XorEncrypt(LPCTSTR pszSourceData, LPTSTR pszEncrypData, WORD wMaxCount)
{
	//变量定义
	CT2CW strSrcData(pszSourceData);
	WCHAR szEncrypData[MAX_ENCRYPT_LEN+1]=L"";

	//生成密钥
	WORD wRandKey[ENCRYPT_KEY_LEN];
	wRandKey[0]=lstrlenW(strSrcData);
	for (WORD i=1;i<CountArray(wRandKey);i++) wRandKey[i]=rand()%0xFFFF;

	//步骤准备
	WORD wTempCode=0;
	WORD wTimes=((wRandKey[0]+ENCRYPT_KEY_LEN-1)/ENCRYPT_KEY_LEN)*ENCRYPT_KEY_LEN;

	//参数效验
	ASSERT((wTimes*8+1)<=wMaxCount);
	if ((wTimes*8+1)>wMaxCount) return false;

	//生成密文
	for (WORD i=0;i<wTimes;i++)
	{
		if (i<wRandKey[0]) wTempCode=strSrcData[i]^wRandKey[i%ENCRYPT_KEY_LEN];
		else wTempCode=wRandKey[i%ENCRYPT_KEY_LEN]^(WORD)(rand()%0xFFFF);
		swprintf(szEncrypData+i*8,L"%04X%04X",wRandKey[i%ENCRYPT_KEY_LEN],wTempCode);
	}

	//字符转换
	CW2CT strEncrypData(szEncrypData);
	lstrcpyn(pszEncrypData,strEncrypData,wMaxCount);

	return true;
}

//解开密文
bool CWHEncrypt::XorCrevasse(LPCTSTR pszEncrypData, LPTSTR pszSourceData, WORD wMaxCount)
{
	//设置结果
	pszSourceData[0]=0;

	//变量定义
	CT2CW strEncrypData(pszEncrypData);
	WCHAR szSrcData[MAX_SOURCE_LEN]=L"";

	//效验长度
	WORD wEncrypPassLen=lstrlenW(strEncrypData);
	if (wEncrypPassLen<ENCRYPT_KEY_LEN*8) return false;

	//提取长度
	WCHAR szTempBuffer[5]=L"";
	szTempBuffer[CountArray(szTempBuffer)-1]=0;
	CopyMemory(szTempBuffer,(LPCWSTR)strEncrypData,sizeof(WCHAR)*4);

	//获取长度
	WCHAR * pEnd=NULL;
	WORD wSoureLength=(WORD)wcstol(szTempBuffer,&pEnd,16);

	//长度效验
	ASSERT(wEncrypPassLen==(((wSoureLength+ENCRYPT_KEY_LEN-1)/ENCRYPT_KEY_LEN)*ENCRYPT_KEY_LEN*8));
	if (wEncrypPassLen!=(((wSoureLength+ENCRYPT_KEY_LEN-1)/ENCRYPT_KEY_LEN)*ENCRYPT_KEY_LEN*8)) return false;

	//长度效验
	ASSERT((wSoureLength+1)<=wMaxCount);
	if ((wSoureLength+1)>wMaxCount) return false;

	//解开密码
	for (INT i=0;i<wSoureLength;i++)
	{
		//获取密钥
		WCHAR szKeyBuffer[5];
		szKeyBuffer[CountArray(szKeyBuffer)-1]=0;
		szTempBuffer[CountArray(szTempBuffer)-1]=0;
		CopyMemory(szKeyBuffer,(LPCWSTR)(strEncrypData+i*8),sizeof(WCHAR)*4);
		CopyMemory(szTempBuffer,(LPCWSTR)strEncrypData+i*8+4,sizeof(WCHAR)*4);

		//提取密钥
		WCHAR wKey=(WCHAR)wcstol(szKeyBuffer,&pEnd,16);
		WCHAR wEncrypt=(WCHAR)wcstol(szTempBuffer,&pEnd,16);

		//生成原文
		szSrcData[i]=(WCHAR)((WCHAR)wKey^(WCHAR)wEncrypt);
	}

	//终止字符
	szSrcData[wSoureLength]=0;

	//字符转换
	CW2CT strSrcData(szSrcData);
	lstrcpyn(pszSourceData,strSrcData,wMaxCount);

	return true;
}

//生成密文
bool CWHEncrypt::MapEncrypt(LPCTSTR pszSourceData, LPTSTR pszEncrypData, WORD wMaxCount)
{
	//效验参数
	ASSERT(wMaxCount>lstrlen(pszEncrypData));
	ASSERT((pszEncrypData!=NULL)&&(pszSourceData!=NULL));

	//变量定义
	INT nLength=lstrlen(pszSourceData);
	BYTE * pcbEncrypData=(BYTE *)pszEncrypData;
	BYTE * pcbSourceData=(BYTE *)pszSourceData;

	//解密数据
	for (UINT i=0;i<nLength*sizeof(TCHAR);i++)
	{
		BYTE cbIndex=pcbSourceData[i];
		pcbEncrypData[i]=g_cbEncryptMap[cbIndex];
	}

	//设置结果
	pszEncrypData[nLength]=0;

	return true;
}

//解开密文
bool CWHEncrypt::MapCrevasse(LPCTSTR pszEncrypData, LPTSTR pszSourceData, WORD wMaxCount)
{
	//效验参数
	ASSERT(wMaxCount>lstrlen(pszEncrypData));
	ASSERT((pszEncrypData!=NULL)&&(pszSourceData!=NULL));

	//变量定义
	INT nLength=lstrlen(pszEncrypData);
	BYTE * pcbEncrypData=(BYTE *)pszEncrypData;
	BYTE * pcbSourceData=(BYTE *)pszSourceData;

	//解密数据
	for (UINT i=0;i<nLength*sizeof(TCHAR);i++)
	{
		BYTE cbIndex=pcbEncrypData[i];
		pcbSourceData[i]=g_cbCrevasseMap[cbIndex];
	}

	//设置结果
	pszSourceData[nLength]=0;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
