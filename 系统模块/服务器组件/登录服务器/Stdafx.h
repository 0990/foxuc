#pragma once

//////////////////////////////////////////////////////////////////////////////////
//MFC �ļ�

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
//�����ļ�

//ƽ̨����
#include "..\..\ȫ�ֶ���\Platform.h"
#include "..\..\��Ϣ����\CMD_Correspond.h"
#include "..\..\��Ϣ����\CMD_LogonServer.h"
#include "..\..\��Ϣ����\CMD_Commom.h"

//�������
#include "..\..\�������\�������\ServiceCoreHead.h"
#include "..\..\���������\�ں�����\KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
#pragma comment (lib,"../../���ӿ�/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../������ӿ�/Release/KernelEngine.lib")
#pragma comment	(lib,"../../������ӿ�/Release/lib_json.lib")
#pragma comment	(lib,"../../������ӿ�/Release/util.lib")
#pragma comment	(lib,"../../������ӿ�/Release/cpprest141_2_10.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
#pragma comment (lib,"../../���ӿ�/Ansi/KernelEngineD.lib")
#pragma comment	(lib,"../../���ӿ�/Ansi/lib_json.lib")
#pragma comment	(lib,"../../���ӿ�/Ansi/util.lib")
#else
#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
#pragma comment (lib,"../../���ӿ�/Unicode/KernelEngineD.lib")
#pragma comment	(lib,"../../������ӿ�/Debug/lib_json.lib")
#pragma comment	(lib,"../../������ӿ�/Debug/util.lib")
#pragma comment	(lib,"../../������ӿ�/Debug/cpprest141d_2_10.lib")

#endif
#endif

//////////////////////////////////////////////////////////////////////////////////
