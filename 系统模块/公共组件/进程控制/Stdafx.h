#pragma once

//////////////////////////////////////////////////////////////////////////////////

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
#define _WIN32_IE 0x0400
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC �ļ�
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////
