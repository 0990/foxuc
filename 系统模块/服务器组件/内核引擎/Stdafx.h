#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxDisp.h>
#include <AfxDtctl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <AfxCmn.h>
#endif

//////////////////////////////////////////////////////////////////////////////////
//链接代码
#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
//#pragma comment (lib,"../../外加链接库/Release/cryptlib.lib")
#pragma comment	(lib,"../../外加链接库/Release/lib_json.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
//#pragma comment (lib,"../../外加链接库/Debug/cryptlib.lib")
#pragma comment	(lib,"../../外加链接库/Debug/lib_json.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////