#ifndef PROCESS_CONTROL_HEAD_HEAD_FILE
#define PROCESS_CONTROL_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <Afxmt.h>
#include <AtlBase.h>

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//����ļ�
#include "..\..\�������\�������\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef PROCESS_CONTROL_CLASS
	#ifdef  PROCESS_CONTROL_DLL
		#define PROCESS_CONTROL_CLASS _declspec(dllexport)
	#else
		#define PROCESS_CONTROL_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define PROCESS_CONTROL_DLL_NAME	TEXT("ProcessControl.dll")		//�������
#else
	#define PROCESS_CONTROL_DLL_NAME	TEXT("ProcessControlD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//��������
#define IDT_IMMEDIATE				0x0101								//ͬ������
#define IDT_ASYNCHRONISM			0x0201								//�첽����

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IIPCRecvDataSink INTERFACE_VERSION(1,1)
	static const GUID IID_IIPCRecvDataSink={0xeaea7e0b,0x2bba,0x414e,0x91,0xb4,0x9c,0x7e,0x1e,0xb2,0xdf,0xb9};
#else
	#define VER_IIPCRecvDataSink INTERFACE_VERSION(1,1)
	static const GUID IID_IIPCRecvDataSink={0xe1b2bbd0,0xf92e,0x4898,0x89,0xdb,0x72,0xdd,0xc2,0x8c,0x70,0x70};
#endif

//���ݽӿ�
interface IIPCRecvDataSink : public IUnknownEx
{
	//ͬ������
	virtual bool OnImmediateData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)=NULL;
	//�첽����
	virtual bool OnAsynchronismData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IProcessControl INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessControl={0x7675ae4c,0x38bb,0x4de6,0x94,0xb3,0xe0,0x06,0xfb,0xe8,0x30,0xca};
#else
	#define VER_IProcessControl INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessControl={0x71733503,0x9d41,0x429e,0x8a,0xf0,0x78,0x40,0x61,0x86,0x01,0xbe};
#endif

//���̿���
interface IProcessControl : public IUnknownEx
{
	//���ýӿ�
public:
	//��ȡ���
	virtual HWND GetProcessControlWnd()=NULL;
	//�������
	virtual bool SetProcessControlSink(IUnknownEx * pIUnknownEx)=NULL;

	//���ӹ���
public:
	//��������
	virtual bool InitializeLink()=NULL;
	//����ͨѶ
	virtual bool UnInitializeLink()=NULL;

	//���ͽӿ�
public:
	//��������
	virtual bool SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool SendData(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)=NULL;
	//ִ������
	virtual bool SendCommand(HWND hWndServer, WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IProcessControlSink INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessControlSink={0xe2b7aac5,0x2e60,0x4fda,0x8a,0x5a,0xfa,0x5e,0x35,0x1e,0x75,0x1a};
#else
	#define VER_IProcessControlSink INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessControlSink={0xc770827b,0xdeb5,0x471c,0x8e,0xf5,0x42,0x34,0x63,0xe8,0x72,0x2e};
#endif

//��������
interface IProcessControlSink : public IUnknownEx
{
	//��������
	virtual bool OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#include "ProcessControl.h"
#include "ProcessDataControl.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
DECLARE_MODULE_HELPER(ProcessControl,PROCESS_CONTROL_DLL_NAME,"CreateProcessControl")

//////////////////////////////////////////////////////////////////////////////////

#endif