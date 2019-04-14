#include "Stdafx.h"
#include "AfxDllx.h"
#include "ProcessControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE ProcessControlDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(ProcessControlDLL, hInstance)) return 0;
		new CDynLinkLibrary(ProcessControlDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(ProcessControlDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
