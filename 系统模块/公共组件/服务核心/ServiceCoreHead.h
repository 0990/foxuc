#ifndef SERVICE_CORE_HEAD_FILE
#define SERVICE_CORE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <Nb30.h>
#include <Afxmt.h>
#include <AtlBase.h>

//ϵͳ�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef SERVICE_CORE_CLASS
	#ifdef  SERVICE_CORE_DLL
		#define SERVICE_CORE_CLASS _declspec(dllexport)
	#else
		#define SERVICE_CORE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define SERVICE_CORE_DLL_NAME	TEXT("ServiceCore.dll")			//�������
#else
	#define SERVICE_CORE_DLL_NAME	TEXT("ServiceCoreD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef SERVICE_CORE_DLL
	#include "WHThread.h"
	#include "WHRegKey.h"
	#include "WHEncrypt.h"
	#include "WHService.h"
	#include "WHIniData.h"
	#include "WHDataQueue.h"
	#include "WHDataLocker.h"
	#include "WHCommandLine.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif