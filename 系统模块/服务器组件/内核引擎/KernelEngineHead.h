#ifndef KERNEL_ENGINE_HEAD_HEAD_FILE
#define KERNEL_ENGINE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#include <Afxmt.h>
#include <Comutil.h>
#include <ICrsint.h>
#include <Process.h>
#include <Wininet.h>
#include <WinSock2.h>

//ƽ̨�ļ�
#include "..\..\..\ϵͳģ��\ȫ�ֶ���\Platform.h"
#include "..\..\..\ϵͳģ��\�������\�������\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//ADO ����

#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

typedef _com_error						CComError;						//COM ����
typedef _variant_t						CDBVarValue;					//���ݿ���ֵ

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef KERNEL_ENGINE_CLASS
	#ifdef  KERNEL_ENGINE_DLL
		#define KERNEL_ENGINE_CLASS _declspec(dllexport)
	#else
		#define KERNEL_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define KERNEL_ENGINE_DLL_NAME	TEXT("KernelEngine.dll")			//��� DLL ����
#else
	#define KERNEL_ENGINE_DLL_NAME	TEXT("KernelEngineD.dll")			//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

//��������
#define TIME_CELL					200									//ʱ�䵥Ԫ
#define TIMES_INFINITY				DWORD(-1)							//���޴���
//#define MAX_ASYNCHRONISM_DATA		16384								//�첽����
#define MAX_ASYNCHRONISM_DATA		32768								//�첽����
//////////////////////////////////////////////////////////////////////////////////
//���綨��

//���Ӵ���
#define CONNECT_SUCCESS				0									//���ӳɹ�
#define CONNECT_FAILURE				1									//����ʧ��
#define CONNECT_EXCEPTION			2									//�����쳣

//����״̬
#define SOCKET_STATUS_IDLE			0									//����״̬
#define SOCKET_STATUS_WAIT			1									//�ȴ�״̬
#define SOCKET_STATUS_CONNECT		2									//����״̬

//�ر�ԭ��
#define SHUT_REASON_INSIDE			0									//�ڲ�ԭ��
#define SHUT_REASON_NORMAL			1									//�����ر�
#define SHUT_REASON_REMOTE			2									//Զ�̹ر�
#define SHUT_REASON_TIME_OUT		3									//���糬ʱ
#define SHUT_REASON_EXCEPTION		4									//�쳣�ر�

//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����ȼ�
enum enTraceLevel
{
	TraceLevel_Info					=0,									//��Ϣ��Ϣ
	TraceLevel_Normal				=1,									//��ͨ��Ϣ
	TraceLevel_Warning				=2,									//������Ϣ
	TraceLevel_Exception			=3,									//�쳣��Ϣ
	TraceLevel_Debug				=4,									//������Ϣ
};

//SQL �쳣����
enum enSQLException
{
	SQLException_None				=0,									//û���쳣
	SQLException_Connect			=1,									//���Ӵ���
	SQLException_Syntax				=2,									//�﷨����
};

//////////////////////////////////////////////////////////////////////////////////
//�¼�����

//�¼���ʶ
#define EVENT_TIMER					0x0001								//ʱ���¼�
#define EVENT_CONTROL				0x0002								//�����¼�
#define EVENT_DATABASE				0x0003								//���ݿ��¼�

//�����¼�
#define EVENT_TCP_SOCKET_READ		0x0004								//��ȡ�¼�
#define EVENT_TCP_SOCKET_SHUT		0x0005								//�ر��¼�
#define EVENT_TCP_SOCKET_LINK		0x0006								//�����¼�

//�����¼�
#define EVENT_TCP_NETWORK_ACCEPT	0x0007								//Ӧ���¼�
#define EVENT_TCP_NETWORK_READ		0x0008								//��ȡ�¼�
#define EVENT_TCP_NETWORK_SHUT		0x0009								//�ر��¼�
#define EVENT_TCP_NETWORK_FOREGROUND	0x000a								//�ر��¼�

//�¼�����
#define EVENT_MASK_KERNEL			0x00FF								//�ں��¼�
#define EVENT_MASK_CUSTOM			0xFF00								//�Զ����¼�

//////////////////////////////////////////////////////////////////////////////////

//�����¼�
struct NTY_ControlEvent
{
	WORD							wControlID;							//���Ʊ�ʶ
};

//��ʱ���¼�
struct NTY_TimerEvent
{
	DWORD							dwTimerID;							//ʱ���ʶ
	WPARAM							dwBindParameter;					//�󶨲���
};

//���ݿ��¼�
struct NTY_DataBaseEvent
{
	WORD							wRequestID;							//�����ʶ
	DWORD							dwContextID;						//�����ʶ
};

//��ȡ�¼�
struct NTY_TCPSocketReadEvent
{
	WORD							wDataSize;							//���ݴ�С
	WORD							wServiceID;							//�����ʶ
	TCP_Command						Command;							//������Ϣ
};

//�ر��¼�
struct NTY_TCPSocketShutEvent
{
	WORD							wServiceID;							//�����ʶ
	BYTE							cbShutReason;						//�ر�ԭ��
};

//�����¼�
struct NTY_TCPSocketLinkEvent
{
	INT								nErrorCode;							//�������
	WORD							wServiceID;							//�����ʶ
};

//Ӧ���¼�
struct NTY_TCPNetworkAcceptEvent
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ȡ�¼�
struct NTY_TCPNetworkReadEvent
{
	WORD							wDataSize;							//���ݴ�С
	DWORD							dwSocketID;							//�����ʶ
	TCP_Command						Command;							//������Ϣ
};

//�ر��¼�
struct NTY_TCPNetworkShutEvent
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��
};
struct NTY_TCPNetworkForegroundEvent
{
	DWORD							dwSocketID;							//�����ʶ
	bool                            bForeground;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IDataBaseException INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBaseException={0x008be9d3,0x2305,0x40da,0x00ae,0xd1,0x61,0x7a,0xd2,0x2a,0x47,0xfc};
#else
	#define VER_IDataBaseException INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBaseException={0x428361ed,0x9dfa,0x43d7,0x008f,0x26,0x17,0x06,0x47,0x6b,0x2a,0x51};
#endif

//���ݿ��쳣
interface IDataBaseException : public IUnknownEx
{
	//�쳣����
	virtual HRESULT GetExceptionResult()=NULL;
	//�쳣����
	virtual LPCTSTR GetExceptionDescribe()=NULL;
	//�쳣����
	virtual enSQLException GetExceptionType()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITraceService INTERFACE_VERSION(1,1)
	static const GUID IID_ITraceService={0xe4096162,0x8134,0x4d2c,0x00b6,0x4f,0x09,0x5d,0xcc,0xca,0xe0,0x81};
#else
	#define VER_ITraceService INTERFACE_VERSION(1,1)
	static const GUID IID_ITraceService={0xe5f636c6,0xabb5,0x4752,0x00bb,0xc8,0xcd,0xb1,0x76,0x58,0xf5,0x2d};
#endif

//�¼����
interface ITraceService : public IUnknownEx
{
	//�ִ����
	virtual bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITraceServiceManager INTERFACE_VERSION(1,1)
	static const GUID IID_ITraceServiceManager={0x6d14efe6,0x892a,0x4a48,0x0092,0xc9,0xdb,0xea,0x92,0xdd,0xd5,0x13};
#else
	#define VER_ITraceServiceManager INTERFACE_VERSION(1,1)
	static const GUID IID_ITraceServiceManager={0x8bfc36db,0x5ba2,0x42ba,0x0081,0xb0,0x87,0xb0,0x1c,0x9e,0xaf,0xfe};
#endif

//�������
interface ITraceServiceManager : public IUnknownEx
{
	//״̬����
public:
	//׷��״̬
	virtual bool IsEnableTrace(enTraceLevel TraceLevel)=NULL;
	//׷�ٿ���
	virtual bool EnableTrace(enTraceLevel TraceLevel, bool bEnableTrace)=NULL;

	//��������
public:
	//���ýӿ�
	virtual bool SetTraceService(IUnknownEx * pIUnknownEx)=NULL;
	//��ȡ�ӿ�
	virtual VOID * GetTraceService(REFGUID Guid, DWORD dwQueryVer)=NULL;

	//����ӿ�
public:
	//�ִ����
	virtual bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServiceModule INTERFACE_VERSION(1,1)
	static const GUID IID_IServiceModule={0x49084dea,0x4420,0x4bea,0x0080,0x64,0xfa,0x37,0xe3,0x42,0xf3,0x1c};
#else
	#define VER_IServiceModule INTERFACE_VERSION(1,1)
	static const GUID IID_IServiceModule={0x05980504,0xa2f2,0x4b0f,0x009b,0x54,0x51,0x54,0x1e,0x05,0x5c,0xff};
#endif

//����ģ��
interface IServiceModule : public IUnknownEx
{
	//��������
	virtual bool StartService()=NULL;
	//ֹͣ����
	virtual bool ConcludeService()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAsynchronismEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IAsynchronismEngine={0xe03ad33d,0xb285,0x48ea,0x86,0x70,0x0a,0x95,0x55,0x92,0x07,0xe1};
#else
	#define VER_IAsynchronismEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IAsynchronismEngine={0xc7a13074,0x75c5,0x4b8e,0xb5,0x4b,0xee,0x0e,0xec,0xfe,0xb9,0xeb};
#endif

//�첽����
interface IAsynchronismEngine : public IServiceModule
{
	//���ýӿ�
public:
	//���и���
	virtual bool GetBurthenInfo(tagBurthenInfo & BurthenInfo)=NULL;
	//����ģ��
	virtual bool SetAsynchronismSink(IUnknownEx * pIUnknownEx)=NULL;

	//Ͷ�ݽӿ�
public:
	//Ͷ������
	virtual bool PostAsynchronismData(WORD wIdentifier, VOID * pData, WORD wDataSize)=NULL;
	//Ͷ������
	virtual bool PostAsynchronismData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAsynchronismEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAsynchronismEngineSink={0x55215681,0x858a,0x46f6,0x0084,0xec,0x84,0x9e,0xc8,0x7d,0x82,0x35};
#else
	#define VER_IAsynchronismEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAsynchronismEngineSink={0x2edf5c9e,0x2cac,0x461d,0x00a7,0x82,0x2e,0x2f,0xe1,0x91,0x80,0xf8};
#endif

//�첽����
interface IAsynchronismEngineSink : public IUnknownEx
{
	//�����¼�
	virtual bool OnAsynchronismEngineStart()=NULL;
	//ֹͣ�¼�
	virtual bool OnAsynchronismEngineConclude()=NULL;
	//�첽����
	virtual bool OnAsynchronismEngineData(WORD wIdentifier, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IDataBase INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBase={0x7fbbbffa,0xedf4,0x43d2,0x008a,0xb7,0x28,0x87,0x3b,0xd0,0xf5,0x3f};
#else
	#define VER_IDataBase INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBase={0xa2e38a78,0x1e4f,0x4de4,0x00a5,0xd1,0xb9,0x19,0x9b,0xce,0x41,0xae};
#endif

//���ݿ�ӿ�
interface IDataBase : public IUnknownEx
{
	//���ӽӿ�
public:
	//������
	virtual VOID OpenConnection()=NULL;
	//�ر�����
	virtual VOID CloseConnection()=NULL;
	//������Ϣ
	virtual bool SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)=NULL;
	//������Ϣ
	virtual bool SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)=NULL;

	//�����ӿ�
public:
	//�������
	virtual VOID ClearParameters()=NULL;
	//��ȡ����
	virtual VOID GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue)=NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue)=NULL;

	//���ƽӿ�
public:
	//�л���¼
	virtual VOID NextRecordset()=NULL;
	//�رռ�¼
	virtual VOID CloseRecordset()=NULL;
	//�󶨶���
	virtual VOID BindToRecordset(CADORecordBinding * pBind)=NULL;

	//��¼�ӿ�
public:
	//�����ƶ�
	virtual VOID MoveToNext()=NULL;
	//�Ƶ���ͷ
	virtual VOID MoveToFirst()=NULL;
	//�Ƿ����
	virtual bool IsRecordsetEnd()=NULL;
	//��ȡ��Ŀ
	virtual LONG GetRecordCount()=NULL;
	//������ֵ
	virtual LONG GetReturnValue()=NULL;
	//��ȡ����
	virtual VOID GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)=NULL;

	//���ƽӿ�
public:
	//�洢����
	virtual VOID ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)=NULL;
	//ִ�����
	virtual VOID ExecuteSentence(LPCTSTR pszCommand, bool bRecordset)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IDataBaseEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBaseEngine={0x3c56956d,0x1690,0x4c22,0x88,0x34,0x9e,0xb9,0xd0,0x93,0x66,0x4c};
#else
	#define VER_IDataBaseEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBaseEngine={0x47b5a119,0x1676,0x49a3,0xbe,0xae,0xca,0x27,0xeb,0x59,0x97,0x22};
#endif

//���ݿ�����
interface IDataBaseEngine : public IServiceModule
{
	//��Ϣ�ӿ�
public:
	//���渺��
	virtual bool GetBurthenInfo(tagBurthenInfo & BurthenInfo)=NULL;

	//���ýӿ�
public:
	//����ģ��
	virtual bool SetDataBaseEngineSink(IUnknownEx * pIUnknownEx)=NULL;
	//����ģ��
	virtual bool SetDataBaseEngineSink(IUnknownEx * pIUnknownEx[], WORD wSinkCount)=NULL;

	//�������
public:
	//�����¼�
	virtual bool PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize)=NULL;
	//�����¼�
	virtual bool PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool DeferDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IDataBaseEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBaseEngineSink={0x295c258b,0xfad0,0x41ac,0x0099,0x19,0xc5,0x64,0x54,0xec,0x3a,0xf9};
#else
	#define VER_IDataBaseEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBaseEngineSink={0x0ed26ed6,0x69d7,0x4f5b,0x00b0,0xca,0x17,0xae,0xab,0xba,0x06,0xdf};
#endif

//���ݿ⹳��
interface IDataBaseEngineSink : public IUnknownEx
{
	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)=NULL;
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)=NULL;

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)=NULL;
	//�����¼�
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)=NULL;
	//�����¼�
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUDPNetworkEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IUDPNetworkEngine={0xbbbd895a,0x0a97,0x4fae,0x8e,0x38,0x10,0x76,0xa7,0xc7,0x4d,0x7f};
#else
	#define VER_IUDPNetworkEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IUDPNetworkEngine={0x8d138a9b,0xa97d,0x4d51,0x9d,0x6c,0xd8,0x6e,0xa1,0x84,0x45,0x2e};
#endif

//��������
interface IUDPNetworkEngine : public IServiceModule
{
	//��Ϣ�ӿ�
public:
	//���ö˿�
	virtual WORD GetServicePort()=NULL;
	//��ǰ�˿�
	virtual WORD GetCurrentPort()=NULL;

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetUDPNetworkEngineEvent(IUnknownEx * pIUnknownEx)=NULL;
	//���ò���
	virtual bool SetServiceParameter(WORD wServicePort, WORD wMaxConnect)=NULL;
	
	//���ͽӿ�
public:
	//���ͺ���
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//���ƽӿ�
public:
	//�ر�����
	virtual bool CloseSocket(DWORD dwSocketID)=NULL;
	//���ùر�
	virtual bool ShutDownSocket(DWORD dwSocketID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITCPNetworkEngine INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPNetworkEngine={0x9aa6931f,0x417f,0x43a7,0x86,0xab,0x56,0x10,0xe4,0x34,0x1c,0x17};
#else
	#define VER_ITCPNetworkEngine INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPNetworkEngine={0x7747f683,0xc0da,0x4588,0x89,0xcc,0x15,0x93,0xac,0xc0,0x44,0xc8};
#endif

//��������
interface ITCPNetworkEngine : public IServiceModule
{
	//��Ϣ�ӿ�
public:
	//���ö˿�
	virtual WORD GetServicePort()=NULL;
	//��ǰ�˿�
	virtual WORD GetCurrentPort()=NULL;

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetTCPNetworkEngineEvent(IUnknownEx * pIUnknownEx)=NULL;
	//���ò���
	virtual bool SetServiceParameter(WORD wServicePort, WORD wMaxConnect, LPCTSTR  pszCompilation)=NULL;
	
	//���ͽӿ�
public:
	//���ͺ���
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, BYTE cbBatchMask)=NULL;

	//���ƽӿ�
public:
	//�ر�����
	virtual bool CloseSocket(DWORD dwSocketID)=NULL;
	//���ùر�
	virtual bool ShutDownSocket(DWORD dwSocketID)=NULL;
	//����Ⱥ��
	virtual bool AllowBatchSend(DWORD dwSocketID, bool bAllowBatch, BYTE cbBatchMask)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITCPSocketService INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocketService={0x0f8a5c14,0xab92,0x467c,0xb6,0x7b,0x6d,0x8a,0xcf,0x64,0x52,0xd7};
#else
	#define VER_ITCPSocketService INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocketService={0x709a4449,0xad77,0x4b3d,0xb4,0xd6,0x8d,0x0b,0x28,0x65,0xec,0xae};
#endif

//����ӿ�
interface ITCPSocketService : public IServiceModule
{
	//���ýӿ�
public:
	//���ú���
	virtual bool SetServiceID(WORD wServiceID)=NULL;
	//���ýӿ�
	virtual bool SetTCPSocketEvent(IUnknownEx * pIUnknownEx)=NULL;

	//���ܽӿ�
public:
	//�ر�����
	virtual bool CloseSocket()=NULL;
	//���ӵ�ַ
	virtual bool Connect(DWORD dwServerIP, WORD wPort)=NULL;
	//���ӵ�ַ
	virtual bool Connect(LPCTSTR szServerIP, WORD wPort)=NULL;
	//���ͺ���
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IWEBSocketService INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketService={0x5502d590,0xc446,0x440d,0xa1,0x95,0x3e,0xda,0x11,0xf8,0x0d,0x64};
#else
	#define VER_IWEBSocketService INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketService={0x91052ff2,0xeb29,0x40b9,0xb2,0xa2,0x8a,0xa2,0x98,0x36,0x2c,0x84};
#endif

//����ӿ�
interface IWEBSocketService : public IServiceModule
{
	//���ýӿ�
public:
	//���ú���
	virtual bool SetServiceID(WORD wServiceID)=NULL;
	//���ýӿ�
	virtual bool SetWEBSocketEvent(IUnknownEx * pIUnknownEx)=NULL;

	//���ܽӿ�
public:
	//�ر�����
	virtual bool CloseSocket(BYTE cbShutReason)=NULL;
	//���Ӳ���
	virtual bool ConnectServer(LPCTSTR pszURL, WORD wPort)=NULL;
	//��������
	virtual bool SendRequestData(VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITimerEngine INTERFACE_VERSION(1,1)
	static const GUID IID_ITimerEngine={0xc90cfc81,0xee01,0x4654,0x008c,0x9a,0x58,0xa9,0x4b,0x78,0x76,0xb6};
#else
	#define VER_ITimerEngine INTERFACE_VERSION(1,1)
	static const GUID IID_ITimerEngine={0x496401ae,0x6fb0,0x4e9f,0x0090,0x98,0x44,0x9d,0x9c,0xb2,0xbd,0x97};
#endif

//��ʱ������
interface ITimerEngine : public IServiceModule
{
	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetTimerEngineEvent(IUnknownEx * pIUnknownEx)=NULL;

	//���ܽӿ�
public:
	//���ö�ʱ��
	virtual bool SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)=NULL; 
	//ɾ����ʱ��
	virtual bool KillTimer(DWORD dwTimerID)=NULL;
	//ɾ����ʱ��
	virtual bool KillAllTimer()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITimerEngineEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ITimerEngineEvent={0x995c4868,0x81b8,0x4c52,0x00a2,0x15,0x71,0x97,0x0d,0x16,0xaf,0xb7};
#else
	#define VER_ITimerEngineEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ITimerEngineEvent={0xeb78a125,0x62fc,0x4811,0x00b6,0xf2,0x59,0x26,0x88,0x04,0xc3,0x02};
#endif

//��ʱ���¼�
interface ITimerEngineEvent : public IUnknownEx
{
	//�ӿڶ���
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IDataBaseEngineEvent INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBaseEngineEvent={0x344ea7f2,0x633b,0x4580,0x0080,0xa9,0xa9,0x4a,0x46,0x12,0xce,0x73};
#else
	#define VER_IDataBaseEngineEvent INTERFACE_VERSION(1,1)
	static const GUID IID_IDataBaseEngineEvent={0xc29c7131,0xe84b,0x4553,0x00a8,0x38,0x12,0xee,0x07,0xdd,0x0e,0xa3};
#endif

//���ݿ��¼�
interface IDataBaseEngineEvent : public IUnknownEx
{
	//�ӿڶ���
public:
	//���ݿ���
	virtual bool OnEventDataBaseResult(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITCPSocketEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocketEvent={0x0400c2de,0x69b1,0x4136,0x00af,0x30,0x2d,0x2d,0x1e,0x35,0x51,0x24};
#else
	#define VER_ITCPSocketEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocketEvent={0x6f5bdb91,0xf72a,0x425d,0x0087,0x03,0x39,0xbc,0xf7,0x1e,0x0b,0x03};
#endif

//�����¼�
interface ITCPSocketEvent : public IUnknownEx
{
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)=NULL;
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)=NULL;
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IWEBSocketEvent INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketEvent={0x9f64bb2b,0xb60b,0x4ef9,0xa2,0xb1,0x49,0xca,0x87,0xc9,0x19,0xe4};
#else
	#define VER_IWEBSocketEvent INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketEvent={0xabb2a528,0xcc16,0x4b67,0xbd,0x7b,0x28,0xa6,0xce,0x88,0x8a,0x33};
#endif

//�����¼�
interface IWEBSocketEvent : public IUnknownEx
{
	//״̬�ӿ�
public:
	//������Ϣ
	virtual bool OnEventWEBSocketLink(WORD wServiceID, WORD wRequestID, INT nErrorCode)=NULL;
	//�ر���Ϣ
	virtual bool OnEventWEBSocketShut(WORD wServiceID, WORD wRequestID, BYTE cbShutReason)=NULL;

	//���ݽӿ�
public:
	//���ݰ���
	virtual bool OnEventWEBSocketMain(WORD wServiceID, WORD wRequestID, VOID * pcbMailData, WORD wStreamSize)=NULL;
	//���ݰ�ͷ
	virtual bool OnEventWEBSocketHead(WORD wServiceID, WORD wRequestID, VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITCPNetworkEngineEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPNetworkEngineEvent={0x9759ffb3,0x5bec,0x4199,0x0091,0xef,0x49,0x5b,0xca,0xdc,0x00,0x98};
#else
	#define VER_ITCPNetworkEngineEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPNetworkEngineEvent={0xb7e6da53,0xfca5,0x4d90,0x0085,0x48,0xfe,0x05,0xf6,0xb4,0xc0,0xef};
#endif

//�����¼�
interface ITCPNetworkEngineEvent : public IUnknownEx
{
	//�ӿڶ���
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientAddr)=NULL;
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientAddr, DWORD dwActiveTime)=NULL;
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(DWORD dwSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAttemperEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IAttemperEngine={0x4d5d2424,0x40fd,0x4747,0x86,0xd8,0x8f,0xca,0x6b,0x96,0xea,0x0b};
#else
	#define VER_IAttemperEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IAttemperEngine={0x0b070b2c,0x9d72,0x42d2,0xa5,0x70,0xba,0x2c,0xbf,0x6f,0xbb,0x1c};
#endif

//��������
interface IAttemperEngine : public IServiceModule
{
	//���ýӿ�
public:
	//����ӿ�
	virtual bool SetNetworkEngine(IUnknownEx * pIUnknownEx)=NULL;
	//�ص��ӿ�
	virtual bool SetAttemperEngineSink(IUnknownEx * pIUnknownEx)=NULL;

	//�����¼�
public:
	//�Զ��¼�
	virtual bool OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize)=NULL;
	//�����¼�
	virtual bool OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize)=NULL;

	virtual void* GetQueue()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAttemperEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAttemperEngineSink={0x133d1f30,0x54ce,0x4360,0x0084,0x50,0x87,0x29,0xe0,0x95,0xaa,0xbb};
#else
	#define VER_IAttemperEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAttemperEngineSink={0x831b9001,0x4450,0x45dd,0x0091,0x37,0x0d,0x26,0x16,0xe3,0x75,0x32};
#endif

//���ȹ���
interface IAttemperEngineSink : public IUnknownEx
{
	//�첽�ӿ�
public:
	//�����¼�
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx)=NULL;
	//ֹͣ�¼�
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)=NULL;

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)=NULL;
	//�Զ��¼�
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)=NULL;

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)=NULL;
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)=NULL;

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)=NULL;
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)=NULL;
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)=NULL;

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)=NULL;
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)=NULL;
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID) = NULL;

	virtual bool OnEventTCPNetworkForeground(bool bForeground, DWORD dwSocketID) = NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//���������

DECLARE_MODULE_HELPER(DataBase,KERNEL_ENGINE_DLL_NAME,"CreateDataBase")
DECLARE_MODULE_HELPER(TimerEngine,KERNEL_ENGINE_DLL_NAME,"CreateTimerEngine")
DECLARE_MODULE_HELPER(DataBaseEngine,KERNEL_ENGINE_DLL_NAME,"CreateDataBaseEngine")
DECLARE_MODULE_HELPER(AttemperEngine,KERNEL_ENGINE_DLL_NAME,"CreateAttemperEngine")
DECLARE_MODULE_HELPER(TCPSocketService,KERNEL_ENGINE_DLL_NAME,"CreateTCPSocketService")
DECLARE_MODULE_HELPER(WEBSocketService,KERNEL_ENGINE_DLL_NAME,"CreateWEBSocketService")
DECLARE_MODULE_HELPER(TCPNetworkEngine,KERNEL_ENGINE_DLL_NAME,"CreateTCPNetworkEngine")
DECLARE_MODULE_HELPER(UDPNetworkEngine,KERNEL_ENGINE_DLL_NAME,"CreateUDPNetworkEngine")
DECLARE_MODULE_HELPER(AsynchronismEngine,KERNEL_ENGINE_DLL_NAME,"CreateAsynchronismEngine")
DECLARE_MODULE_HELPER(TraceServiceManager,KERNEL_ENGINE_DLL_NAME,"CreateTraceServiceManager")

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#ifndef KERNEL_ENGINE_DLL
	#include "DataBaseAide.h"
	#include "TraceService.h"
	#include "ProcessException.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif