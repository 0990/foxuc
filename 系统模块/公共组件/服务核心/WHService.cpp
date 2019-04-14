#include "StdAfx.h"
#include "WHService.h"
#include "WHEncrypt.h"

//////////////////////////////////////////////////////////////////////////////////

//压缩文件
#define ZLIB_WINAPI
#include "Compress\ZLib.h"
#pragma comment(lib,"Version")
#pragma comment(lib,"Compress\\ZLib.lib")

//////////////////////////////////////////////////////////////////////////////////

//状态信息
struct tagAstatInfo
{
	ADAPTER_STATUS					AdapterStatus;						//网卡状态
	NAME_BUFFER						NameBuff[16];						//名字缓冲
};

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWHService::CWHService()
{
}

//拷贝字符
bool CWHService::SetClipboardString(LPCTSTR pszString)
{
	//变量定义
	HANDLE hData=NULL;
	BOOL bOpenClopboard=FALSE;

	//执行逻辑
	__try
	{
		//打开拷贝
		bOpenClopboard=OpenClipboard(AfxGetMainWnd()->m_hWnd);
		if (bOpenClopboard==FALSE) __leave;

		//清空拷贝
		if (EmptyClipboard()==FALSE) __leave;

		//申请内存
		HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,CountStringBuffer(pszString));
		if (hData==NULL) __leave;

		//复制数据
		lstrcpy((LPTSTR)GlobalLock(hData),pszString);
		GlobalUnlock(hData);

		//设置数据
		#ifndef _UNICODE
			::SetClipboardData(CF_TEXT,hData);
		#else
			::SetClipboardData(CF_UNICODETEXT,hData);
		#endif
	}

	//终止程序
	__finally
	{
		//释放内存
		if (hData!=NULL) GlobalUnlock(hData);

		//关闭拷贝
		if (bOpenClopboard==TRUE) CloseClipboard();

		//错误判断
		if (AbnormalTermination()==TRUE)
		{
			ASSERT(FALSE);
		}
	}

	return true;
}

//机器标识
bool CWHService::GetMachineID(TCHAR szMachineID[LEN_MACHINE_ID])
{
	//变量定义
	TCHAR szMACAddress[LEN_NETWORK_ID]=TEXT("");

	//网卡标识
	GetMACAddress(szMACAddress);

	//转换信息
	ASSERT(LEN_MACHINE_ID>=LEN_MD5);
	CWHEncrypt::MD5Encrypt(szMACAddress,szMachineID);

	return true;
}

//机器标识
bool CWHService::GetMachineIDEx(TCHAR szMachineID[LEN_MACHINE_ID])
{
	//变量定义
	TCHAR szMACAddress[LEN_NETWORK_ID]=TEXT("");

	//网卡标识
	WORD wMacSize=GetMACAddress(szMACAddress);

	//硬盘标识
	DWORD *pbuf=(DWORD*)szMACAddress;
	WORD wIndex=(wMacSize+sizeof(DWORD)-1)/sizeof(DWORD);
	LPCTSTR pszHardDisk[]={TEXT("C:\\"),TEXT("D:\\"),TEXT("E:\\")};
	for (WORD i=wIndex;i<CountArray(pszHardDisk);i++)
	{
		ASSERT(CountArray(pszHardDisk)>(i-wIndex));
		GetVolumeInformation(pszHardDisk[i-wIndex],NULL,0,pbuf+i,NULL,NULL,0,NULL);
	}

	//转换信息
	ASSERT(LEN_MACHINE_ID>=LEN_MD5);
	CWHEncrypt::MD5Encrypt(szMACAddress,szMachineID);

	return true;
}

//网卡地址
bool CWHService::GetMACAddress(TCHAR szMACAddress[LEN_NETWORK_ID])
{
	//变量定义
	HINSTANCE hInstance=NULL;

	//执行逻辑
	__try
	{
		//加载 DLL
		hInstance=LoadLibrary(TEXT("NetApi32.dll"));
		if (hInstance==NULL) __leave;

		//获取函数
		typedef BYTE __stdcall NetBiosProc(NCB * Ncb);
		NetBiosProc * pNetBiosProc=(NetBiosProc *)GetProcAddress(hInstance,"Netbios");
		if (pNetBiosProc==NULL) __leave;

		//变量定义
		NCB Ncb;
		LANA_ENUM LanaEnum;
		ZeroMemory(&Ncb,sizeof(Ncb));
		ZeroMemory(&LanaEnum,sizeof(LanaEnum));

		//枚举网卡
		Ncb.ncb_command=NCBENUM;
		Ncb.ncb_length=sizeof(LanaEnum);
		Ncb.ncb_buffer=(BYTE *)&LanaEnum;
		if ((pNetBiosProc(&Ncb)!=NRC_GOODRET)||(LanaEnum.length==0)) __leave;

		//获取地址
		if (LanaEnum.length>0)
		{
			//变量定义
			tagAstatInfo Adapter;
			ZeroMemory(&Adapter,sizeof(Adapter));

			//重置网卡
			Ncb.ncb_command=NCBRESET;
			Ncb.ncb_lana_num=LanaEnum.lana[0];
			if (pNetBiosProc(&Ncb)!=NRC_GOODRET) __leave;

			//获取状态
			Ncb.ncb_command=NCBASTAT;
			Ncb.ncb_length=sizeof(Adapter);
			Ncb.ncb_buffer=(BYTE *)&Adapter;
			Ncb.ncb_lana_num=LanaEnum.lana[0];
			strcpy((char *)Ncb.ncb_callname,"*");
			if (pNetBiosProc(&Ncb)!=NRC_GOODRET) __leave;

			//获取地址
			for (INT i=0;i<6;i++)
			{
				ASSERT((i*2)<LEN_NETWORK_ID);
				_stprintf(&szMACAddress[i*2],TEXT("%02X"),Adapter.AdapterStatus.adapter_address[i]);
			}
		}
	}

	//结束清理
	__finally
	{
		//释放资源
		if (hInstance!=NULL)
		{
			FreeLibrary(hInstance);
			hInstance=NULL;
		}

		//错误断言
		if (AbnormalTermination()==TRUE)
		{
			ASSERT(FALSE);
		}
	}

	return true;
}

//注销热键
bool CWHService::UnRegisterHotKey(HWND hWnd, UINT uKeyID)
{
	//注销热键
	BOOL bSuccess=::UnregisterHotKey(hWnd,uKeyID);

	return (bSuccess==TRUE)?true:false;
}

//注册热键
bool CWHService::RegisterHotKey(HWND hWnd, UINT uKeyID, WORD wHotKey)
{
	//变量定义
	BYTE cbModifiers=0;
	if (HIBYTE(wHotKey)&HOTKEYF_ALT) cbModifiers|=MOD_ALT;
	if (HIBYTE(wHotKey)&HOTKEYF_SHIFT) cbModifiers|=MOD_SHIFT;
	if (HIBYTE(wHotKey)&HOTKEYF_CONTROL) cbModifiers|=MOD_CONTROL;

	//注册热键
	BOOL bSuccess=::RegisterHotKey(hWnd,uKeyID,cbModifiers,LOBYTE(wHotKey));

	return (bSuccess==TRUE)?true:false;
}

//进程目录
bool CWHService::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	//模块路径
	TCHAR szModulePath[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szModulePath,CountArray(szModulePath));

	//分析文件
	for (INT i=lstrlen(szModulePath);i>=0;i--)
	{
		if (szModulePath[i]==TEXT('\\'))
		{
			szModulePath[i]=0;
			break;
		}
	}

	//设置结果
	ASSERT(szModulePath[0]!=0);
	lstrcpyn(szWorkDirectory,szModulePath,wBufferCount);

	return true;
}

//文件版本
bool CWHService::GetModuleVersion(LPCTSTR pszModuleName, DWORD & dwVersionInfo)
{
	//设置结果
	dwVersionInfo=0L;

	//接收缓冲
	BYTE cbInfoBuffer[1024];
	ZeroMemory(cbInfoBuffer,sizeof(cbInfoBuffer));

	//模块信息
    DWORD dwFileHandle=NULL;
	if (GetFileVersionInfo(pszModuleName,dwFileHandle,sizeof(cbInfoBuffer),cbInfoBuffer)==FALSE) return false;

	//获取信息
	UINT uQuerySize=0;
	VS_FIXEDFILEINFO * pFixedFileInfo=NULL;
    if (VerQueryValue(cbInfoBuffer,TEXT("\\"),(VOID * *)&pFixedFileInfo,&uQuerySize)==FALSE) return false;

	//设置结果
	if ((pFixedFileInfo!=NULL)&&(uQuerySize==sizeof(VS_FIXEDFILEINFO)))
	{
		//设置版本
		WORD wVersion1=HIWORD(pFixedFileInfo->dwFileVersionMS);
		WORD wVersion2=LOWORD(pFixedFileInfo->dwFileVersionMS);
		WORD wVersion3=HIWORD(pFixedFileInfo->dwFileVersionLS);
		WORD wVersion4=LOWORD(pFixedFileInfo->dwFileVersionLS);
		dwVersionInfo=MAKELONG(MAKEWORD(wVersion4,wVersion3),MAKEWORD(wVersion2,wVersion1));

		return true;
	}

	return false;
}

//压缩数据
ULONG CWHService::CompressData(LPBYTE pcbSourceData, ULONG lSourceSize, BYTE cbResultData[], ULONG lResultSize)
{
	//压缩数据
	if (compress(cbResultData,&lResultSize,pcbSourceData,lSourceSize)==0L)
	{
		return lResultSize;
	}

	return 0L;
}

//解压数据
ULONG CWHService::UnCompressData(LPBYTE pcbSourceData, ULONG lSourceSize, BYTE cbResultData[], ULONG lResultSize)
{
	//解压数据
	if (uncompress(cbResultData,&lResultSize,pcbSourceData,lSourceSize)==0L)
	{
		return lResultSize;
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
