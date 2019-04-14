#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CServiceUnits *			CServiceUnits::g_pServiceUnits=NULL;			//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServiceUnits, CWnd)
	ON_MESSAGE(WM_UICONTROL_REQUEST,OnUIControlRequest)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServiceUnits::CServiceUnits()
{
	//���ñ���
	m_ServiceStatus=ServiceStatus_Stop;

	//���ýӿ�
	m_pIServiceUnitsSink=NULL;

	//���ö���
	ASSERT(g_pServiceUnits==NULL);
	if (g_pServiceUnits==NULL) g_pServiceUnits=this;

	return;
}

//��������
CServiceUnits::~CServiceUnits()
{
	ConcludeService();
}

//��������
bool CServiceUnits::StartService()
{
	//Ч��״̬
	ASSERT(m_ServiceStatus==ServiceStatus_Stop);
	if (m_ServiceStatus!=ServiceStatus_Stop) return false;

	//����״̬
	SetServiceStatus(ServiceStatus_Config);

	//��������
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,AfxGetMainWnd(),100);
	}

	//���÷���
	if (InitializeService()==false)
	{
		ConcludeService();
		return false;
	}

	//�����ں�
	if (StartKernelService()==false)
	{
		ConcludeService();
		return false;
	}

	return true;
}

//ֹͣ����
bool CServiceUnits::ConcludeService()
{
	//���ñ���
	SetServiceStatus(ServiceStatus_Stop);

	//ֹͣ����
	if (m_TimerEngine.GetInterface()!=NULL) m_TimerEngine->ConcludeService();
	if (m_AttemperEngine.GetInterface()!=NULL) m_AttemperEngine->ConcludeService();
	if (m_DataBaseEngine.GetInterface()!=NULL) m_DataBaseEngine->ConcludeService();
	if (m_TCPNetworkEngine.GetInterface()!=NULL) m_TCPNetworkEngine->ConcludeService();
	if (m_TCPSocketService.GetInterface()!=NULL) m_TCPSocketService->ConcludeService();

	return true;
}

//���ýӿ�
bool CServiceUnits::SetServiceUnitsSink(IServiceUnitsSink * pIServiceUnitsSink)
{
	//���ñ���
	m_pIServiceUnitsSink=pIServiceUnitsSink;

	return true;
}

//Ͷ������
bool CServiceUnits::PostControlRequest(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	//״̬�ж�
	ASSERT(IsWindow(m_hWnd));
	if (IsWindow(m_hWnd)==FALSE) return false;

	//�������
	CWHDataLocker DataLocker(m_CriticalSection);
	if (m_DataQueue.InsertData(wIdentifier,pData,wDataSize)==false) return false;

	//������Ϣ
	PostMessage(WM_UICONTROL_REQUEST,wIdentifier,wDataSize);

	return true;
}

//�������
bool CServiceUnits::InitializeService()
{
	//���ز���
	m_InitParameter.LoadInitParameter();

	//�������
	if ((m_TimerEngine.GetInterface()==NULL)&&(m_TimerEngine.CreateInstance()==false)) return false;
	if ((m_AttemperEngine.GetInterface()==NULL)&&(m_AttemperEngine.CreateInstance()==false)) return false;
	if ((m_DataBaseEngine.GetInterface()==NULL)&&(m_DataBaseEngine.CreateInstance()==false)) return false;
	if ((m_TCPNetworkEngine.GetInterface()==NULL)&&(m_TCPNetworkEngine.CreateInstance()==false)) return false;
	if ((m_TCPSocketService.GetInterface()==NULL)&&(m_TCPSocketService.CreateInstance()==false)) return false;

	//����ӿ�
	IUnknownEx * pIAttemperEngine=m_AttemperEngine.GetInterface();
	IUnknownEx * pITCPNetworkEngine=m_TCPNetworkEngine.GetInterface();
	IUnknownEx * pIAttemperEngineSink=QUERY_OBJECT_INTERFACE(m_AttemperEngineSink,IUnknownEx);

	//��������
	IUnknownEx * pIDataBaseEngineSink[CountArray(m_DataBaseEngineSink)];
	for (WORD i=0;i<CountArray(pIDataBaseEngineSink);i++) pIDataBaseEngineSink[i]=QUERY_OBJECT_INTERFACE(m_DataBaseEngineSink[i],IUnknownEx);

	//�ں����
	if (m_TimerEngine->SetTimerEngineEvent(pIAttemperEngine)==false) return false;
	if (m_AttemperEngine->SetNetworkEngine(pITCPNetworkEngine)==false) return false;
	if (m_AttemperEngine->SetAttemperEngineSink(pIAttemperEngineSink)==false) return false;
	if (m_TCPNetworkEngine->SetTCPNetworkEngineEvent(pIAttemperEngine)==false) return false;
	if (m_DataBaseEngine->SetDataBaseEngineSink(pIDataBaseEngineSink,CountArray(pIDataBaseEngineSink))==false) return false;

	//Э������
	if (m_TCPSocketService->SetServiceID(NETWORK_CORRESPOND)==false) return false;
	if (m_TCPSocketService->SetTCPSocketEvent(pIAttemperEngine)==false) return false;

	//���Ȼص�
	m_AttemperEngineSink.m_pInitParameter=&m_InitParameter;
	m_AttemperEngineSink.m_pITimerEngine=m_TimerEngine.GetInterface();
	m_AttemperEngineSink.m_pIDataBaseEngine=m_DataBaseEngine.GetInterface();
	m_AttemperEngineSink.m_pITCPNetworkEngine=m_TCPNetworkEngine.GetInterface();
	m_AttemperEngineSink.m_pITCPSocketService=m_TCPSocketService.GetInterface();

	//���ݿ�ص�
	for (INT i=0;i<CountArray(m_DataBaseEngineSink);i++)
	{
		m_DataBaseEngineSink[i].m_pInitParameter=&m_InitParameter;
		m_DataBaseEngineSink[i].m_pIDataBaseEngineEvent=QUERY_OBJECT_PTR_INTERFACE(pIAttemperEngine,IDataBaseEngineEvent);
	}

	//��������
	WORD wMaxConnect=m_InitParameter.m_wMaxConnect;
	WORD wServicePort=m_InitParameter.m_wServicePort;
	if (m_TCPNetworkEngine->SetServiceParameter(wServicePort,wMaxConnect,szCompilation)==false) return false;

	return true;
}

//�����ں�
bool CServiceUnits::StartKernelService()
{
	//ʱ������
	if (m_TimerEngine->StartService()==false)
	{
		CTraceService::TraceString(TEXT("��ʱ����������ʧ��"), TraceLevel_Warning);
		//ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_AttemperEngine->StartService()==false)
	{
		CTraceService::TraceString(TEXT("������������ʧ��"), TraceLevel_Warning);
		//ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_DataBaseEngine->StartService()==false)
	{
		CTraceService::TraceString(TEXT("���ݿ���������ʧ��"), TraceLevel_Warning);
		//ASSERT(FALSE);
		return false;
	}

	//Э������
	if (m_TCPSocketService->StartService()==false)
	{
		CTraceService::TraceString(TEXT("Э��TCPSocket��������ʧ��"), TraceLevel_Warning);
		//ASSERT(FALSE);
		return false;
	}

	return true;
}

//��������
bool CServiceUnits::StartNetworkService()
{
	//��������
	if (m_TCPNetworkEngine->StartService()==false)
	{
		CTraceService::TraceString(TEXT("������������ʧ�ܣ�"), TraceLevel_Warning);
		//ASSERT(FALSE);
		return false;
	}

	return true;
}

//����״̬
bool CServiceUnits::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//״̬�ж�
	if (m_ServiceStatus!=ServiceStatus)
	{
		//����֪ͨ
		if ((m_ServiceStatus!=ServiceStatus_Service)&&(ServiceStatus==ServiceStatus_Stop))
		{
			LPCTSTR pszString=TEXT("��������ʧ��");
			CTraceService::TraceString(pszString,TraceLevel_Exception);
		}

		//���ñ���
		m_ServiceStatus=ServiceStatus;

		//״̬֪ͨ
		ASSERT(m_pIServiceUnitsSink!=NULL);
		if (m_pIServiceUnitsSink!=NULL) m_pIServiceUnitsSink->OnServiceUnitsStatus(m_ServiceStatus);
	}

	return true;
}

//���Ϳ���
bool CServiceUnits::SendControlPacket(WORD wControlID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_AttemperEngine.GetInterface()!=NULL);
	if (m_AttemperEngine.GetInterface()==NULL) return false;

	//���Ϳ���
	m_AttemperEngine->OnEventControl(wControlID,pData,wDataSize);

	return true;
}

//������Ϣ
LRESULT CServiceUnits::OnUIControlRequest(WPARAM wParam, LPARAM lParam)
{
	//��������
	tagDataHead DataHead;
	BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

	//��ȡ����
	CWHDataLocker DataLocker(m_CriticalSection);
	if (m_DataQueue.DistillData(DataHead,cbBuffer,sizeof(cbBuffer))==false)
	{
		ASSERT(FALSE);
		return NULL;
	}

	//���ݴ���
	switch (DataHead.wIdentifier)
	{
	case UI_LOAD_DB_LIST_RESULT:	//�б���
		{
			//Ч����Ϣ
			ASSERT(DataHead.wDataSize==sizeof(CP_ControlResult));
			if (DataHead.wDataSize!=sizeof(CP_ControlResult)) return 0;

			//��������
			CP_ControlResult * pControlResult=(CP_ControlResult *)cbBuffer;

			//ʧ�ܴ���
			if ((m_ServiceStatus!=ServiceStatus_Service)&&(pControlResult->cbSuccess==ER_FAILURE))
			{
				ConcludeService();
				return 0;
			}

			//�ɹ�����
			if ((m_ServiceStatus!=ServiceStatus_Service)&&(pControlResult->cbSuccess==ER_SUCCESS))
			{
				//����Э��
				SendControlPacket(CT_CONNECT_CORRESPOND,NULL,0);
			}

			return 0;
		}
	case UI_CORRESPOND_RESULT:		//Э�����
		{
			//Ч����Ϣ
			ASSERT(DataHead.wDataSize==sizeof(CP_ControlResult));
			if (DataHead.wDataSize!=sizeof(CP_ControlResult)) return 0;

			//��������
			CP_ControlResult * pControlResult=(CP_ControlResult *)cbBuffer;

			//ʧ�ܴ���
			if ((m_ServiceStatus!=ServiceStatus_Service)&&(pControlResult->cbSuccess==ER_FAILURE))
			{
				ConcludeService();
				return 0;
			}

			//�ɹ�����
			if ((m_ServiceStatus!=ServiceStatus_Service)&&(pControlResult->cbSuccess==ER_SUCCESS))
			{
				//��������
				if (StartNetworkService()==false)
				{
					ConcludeService();
					return 0;
				}

				//����״̬
				SetServiceStatus(ServiceStatus_Service);
			}

			return 0;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
