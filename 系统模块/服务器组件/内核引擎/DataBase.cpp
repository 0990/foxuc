#include "StdAfx.h"
#include "Math.h"
#include "TraceService.h"
#include "DataBase.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
_COM_SMARTPTR_TYPEDEF(IADORecordBinding, __uuidof(IADORecordBinding));

//Ч������
#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

//////////////////////////////////////////////////////////////////////////

//���캯��
CADOError::CADOError()
{
	m_enErrorType = SQLException_None;
}

//��������
CADOError::~CADOError()
{
}

//�ӿڲ�ѯ
void * CADOError::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseException, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseException, Guid, dwQueryVer);
	return NULL;
}

//���ô���
void CADOError::SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe)
{
	//���ô���
	m_enErrorType = enErrorType;
	m_strErrorDescribe = pszDescribe;

	//�׳�����
	throw QUERY_ME_INTERFACE(IDataBaseException);

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBase::CDataBase() : m_dwResumeConnectCount(30L), m_dwResumeConnectTime(30L)
{
	//״̬����
	m_dwConnectCount = 0;
	m_dwConnectErrorTime = 0L;

	//��������
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//Ч������
	ASSERT(m_DBCommand != NULL);
	ASSERT(m_DBRecordset != NULL);
	ASSERT(m_DBConnection != NULL);
	if (m_DBCommand == NULL) throw TEXT("���ݿ�������󴴽�ʧ��");
	if (m_DBRecordset == NULL) throw TEXT("���ݿ��¼�����󴴽�ʧ��");
	if (m_DBConnection == NULL) throw TEXT("���ݿ����Ӷ��󴴽�ʧ��");

	//���ñ���
	m_DBCommand->CommandType = adCmdStoredProc;

	return;
}

//��������
CDataBase::~CDataBase()
{
	//�ر�����
	CloseConnection();

	//�ͷŶ���
	m_DBCommand.Release();
	m_DBRecordset.Release();
	m_DBConnection.Release();

	return;
}

//�ӿڲ�ѯ
void * CDataBase::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBase, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBase, Guid, dwQueryVer);
	return NULL;
}

//������
VOID CDataBase::OpenConnection()
{
	//�������ݿ�
	try
	{
		//�ر�����
		CloseConnection();

		//�������ݿ�
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_strConnect), L"", L"", adConnectUnspecified));
		m_DBConnection->CursorLocation = adUseClient;
		m_DBCommand->ActiveConnection = m_DBConnection;

		//���ñ���
		m_dwConnectCount = 0L;
		m_dwConnectErrorTime = 0L;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//�رռ�¼
VOID CDataBase::CloseRecordset()
{
	try
	{
		if (IsRecordsetOpened()) EfficacyResult(m_DBRecordset->Close());
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//�ر�����
VOID CDataBase::CloseConnection()
{
	try
	{
		CloseRecordset();
		if ((m_DBConnection != NULL) && (m_DBConnection->GetState() != adStateClosed))
		{
			EfficacyResult(m_DBConnection->Close());
		}
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//��������
bool CDataBase::TryConnectAgain(bool bFocusConnect, CComError * pComError)
{
	try
	{
		//�ж�����
		bool bReConnect = bFocusConnect;
		if (bReConnect == false)
		{
			DWORD dwNowTime = (DWORD)time(NULL);
			if ((m_dwConnectErrorTime + m_dwResumeConnectTime) > dwNowTime) bReConnect = true;
		}
		if ((bReConnect == false) && (m_dwConnectCount > m_dwResumeConnectCount)) bReConnect = true;

		//���ñ���
		m_dwConnectCount++;
		m_dwConnectErrorTime = (DWORD)time(NULL);
		if (bReConnect == false)
		{
			if (pComError != NULL) SetErrorInfo(SQLException_Connect, GetComErrorDescribe(*pComError));
			return false;
		}

		//��������
		OpenConnection();
		return true;
	}
	catch (IDataBaseException * pIDataBaseException)
	{
		//�������Ӵ���
		if (pComError != NULL) SetErrorInfo(SQLException_Connect, GetComErrorDescribe(*pComError));
		else throw pIDataBaseException;
	}

	return false;
}

//������Ϣ
bool CDataBase::SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//Ч�����
	ASSERT(dwDBAddr != 0);
	ASSERT(szDBName != NULL);
	ASSERT(szUser != NULL);
	ASSERT(szPassword != NULL);

	BYTE a = (BYTE)((dwDBAddr >> 24) & 0xFF);
	BYTE b = (BYTE)((dwDBAddr >> 16) & 0xFF);
	BYTE c = (BYTE)((dwDBAddr >> 8) & 0xFF);
	BYTE d = (BYTE)(dwDBAddr & 0xFF);
	try
	{
		//���������ַ���
		m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%d.%d.%d.%d;Data Source=%s,%ld;"),
		                    szPassword, szUser, szDBName, a, b, c, d, wPort);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
	return true;
}

//�л���¼
VOID CDataBase::NextRecordset()
{
	try
	{
		VARIANT lngRec;
		m_DBRecordset->NextRecordset(&lngRec);
		return;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

}

//������Ϣ
bool CDataBase::SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//Ч�����
	ASSERT(szDBAddr != NULL);
	ASSERT(szDBName != NULL);
	ASSERT(szUser != NULL);
	ASSERT(szPassword != NULL);
	try
	{
		//���������ַ���
		m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		                    szPassword, szUser, szDBName, szDBAddr, wPort);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
	return true;
}

//�Ƿ����Ӵ���
bool CDataBase::IsConnectError()
{
	try
	{
		//״̬�ж�
		if (m_DBConnection == NULL) return true;
		if (m_DBConnection->GetState() == adStateClosed) return true;

		//�����ж�
		long lErrorCount = m_DBConnection->Errors->Count;
		if (lErrorCount > 0L)
		{
			ErrorPtr pError = NULL;
			for (long i = 0; i < lErrorCount; i++)
			{
				pError = m_DBConnection->Errors->GetItem(i);
				if (pError->Number == 0x80004005) return true;
			}
		}

		return false;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//�Ƿ��
bool CDataBase::IsRecordsetOpened()
{
	if (m_DBRecordset == NULL) return false;
	if (m_DBRecordset->GetState() == adStateClosed) return false;
	return true;
}

//�����ƶ�
void CDataBase::MoveToNext()
{
	try
	{
		m_DBRecordset->MoveNext();
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return;
}

//�Ƶ���ͷ
void CDataBase::MoveToFirst()
{
	try
	{
		m_DBRecordset->MoveFirst();
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return;
}

//�Ƿ����
bool CDataBase::IsRecordsetEnd()
{
	try
	{
		return (m_DBRecordset->EndOfFile == VARIANT_TRUE);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return true;
}

//��ȡ��Ŀ
long CDataBase::GetRecordCount()
{
	try
	{
		if (m_DBRecordset == NULL) return 0;
		return m_DBRecordset->GetRecordCount();
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return 0;
}

//��ȡ��С
long CDataBase::GetActualSize(LPCTSTR pszParamName)
{
	ASSERT(pszParamName != NULL);
	try
	{
		return m_DBRecordset->Fields->Item[pszParamName]->ActualSize;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return -1;
}

//�󶨶���
VOID CDataBase::BindToRecordset(CADORecordBinding * pBind)
{
	ASSERT(pBind != NULL);
	try
	{
		IADORecordBindingPtr pIBind(m_DBRecordset);
		pIBind->BindToRecordset(pBind);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//��ȡ����
VOID CDataBase::GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	ASSERT(pszItem != NULL);
	try
	{
		DBVarValue = m_DBRecordset->Fields->GetItem(pszItem)->Value;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue)
{
	try
	{
		bValue = 0;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_BOOL:
			{
				bValue = (vtFld.boolVal != 0) ? 2 : 0;
				break;
			}
			case VT_I2:
			case VT_UI1:
			{
				bValue = (BYTE)vtFld.iVal;
				break;
			}
			case VT_NULL:
			case VT_EMPTY:
			{
				bValue = 0;
				break;
			}
			default:
				bValue = (BYTE)vtFld.iVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue)
{
	try
	{
		ulValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) ulValue = vtFld.lVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue)
{
	try
	{
		dbValue = 0.0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_R4:
			{
				dbValue = vtFld.fltVal;
				break;
			}
			case VT_R8:
			{
				dbValue = vtFld.dblVal;
				break;
			}
			case VT_DECIMAL:
			{
				dbValue = vtFld.decVal.Lo32;
				dbValue *= (vtFld.decVal.sign == 128) ? -1 : 1;
				dbValue /= pow((DOUBLE)10.0, (DOUBLE)(vtFld.decVal.scale));
				break;
			}
			case VT_UI1:
			{
				dbValue = vtFld.iVal;
				break;
			}
			case VT_I2:
			case VT_I4:
			{
				dbValue = vtFld.lVal;
				break;
			}
			case VT_NULL:
			case VT_EMPTY:
			{
				dbValue = 0.0L;
				break;
			}
			default:
				dbValue = vtFld.dblVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, LONG & lValue)
{
	try
	{
		lValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) lValue = vtFld.lVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue)
{
	try
	{
		ulValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) ulValue = vtFld.ulVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, INT & nValue)
{
	try
	{
		nValue = 0;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_BOOL:
			{
				nValue = vtFld.boolVal;
				break;
			}
			case VT_I2:
			case VT_UI1:
			{
				nValue = vtFld.iVal;
				break;
			}
			case VT_NULL:
			case VT_EMPTY:
			{
				nValue = 0;
				break;
			}
			default:
				nValue = vtFld.iVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue)
{
	try
	{
		llValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
		case VT_BOOL:
			{
				llValue = vtFld.boolVal;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				llValue = vtFld.iVal;
				break;
			}
		case VT_I4:
			{
				llValue = vtFld.lVal;
				break;
			}
		case VT_DECIMAL:
			{
				llValue = vtFld.decVal.Lo64;
				llValue *= (vtFld.decVal.sign == 128) ? -1 : 1;
				llValue = (__int64)(llValue / pow((DOUBLE)10.0, (DOUBLE)(vtFld.decVal.scale)));
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				llValue = 0;
				break;
			}
		default:
			llValue = vtFld.llVal;
			break;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, LPSTR szBuffer, UINT uSize)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if (vtFld.vt == VT_BSTR)
		{
			lstrcpyA(szBuffer, (LPCSTR)_bstr_t(vtFld));
			return true;
		}
		return false;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, LPWSTR szBuffer, UINT uSize)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if (vtFld.vt == VT_BSTR)
		{
			lstrcpyW(szBuffer, (LPCWSTR)_bstr_t(vtFld));
			return true;
		}
		return false;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}


//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, WORD & wValue)
{
	try
	{
		wValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) wValue = (WORD)vtFld.ulVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_DATE:
			{
				COleDateTime TempTime(vtFld);
				Time = TempTime;
				break;
			}
			case VT_EMPTY:
			case VT_NULL:
			{
				Time.SetStatus(COleDateTime::null);
				break;
			}
			default:
				return false;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, bool & bValue)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_BOOL:
			{
				bValue = (vtFld.boolVal == 0) ? false : true;
				break;
			}
			case VT_EMPTY:
			case VT_NULL:
			{
				bValue = false;
				break;
			}
			default:
				return false;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ������ֵ
long CDataBase::GetReturnValue()
{
	try
	{
		_ParameterPtr Parameter;
		long lParameterCount = m_DBCommand->Parameters->Count;
		for (long i = 0; i < lParameterCount; i++)
		{
			Parameter = m_DBCommand->Parameters->Item[i];
			if (Parameter->Direction == adParamReturnValue) return Parameter->Value.lVal;
		}
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return 0;
}

//ɾ������
void CDataBase::ClearParameters()
{
	try
	{
		long lParameterCount = m_DBCommand->Parameters->Count;
		if (lParameterCount > 0L)
		{
			for (long i = lParameterCount; i > 0; i--)
			{
				m_DBCommand->Parameters->Delete(i - 1);
			}
		}
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return;
}


//��ò���
void CDataBase::GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue)
{
	//Ч�����
	ASSERT(pszParamName != NULL);

	//��ȡ����
	try
	{
		DBVarValue.Clear();
		DBVarValue = m_DBCommand->Parameters->Item[pszParamName]->Value;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return;
}

//�������
void CDataBase::AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue)
{
	ASSERT(pszName != NULL);
	try
	{
		_ParameterPtr Parameter = m_DBCommand->CreateParameter(pszName, Type, Direction, lSize, DBVarValue);
		m_DBCommand->Parameters->Append(Parameter);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//ִ�����
VOID CDataBase::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	ASSERT(pszSPName != NULL);
	try
	{
		//�رռ�¼��
		CloseRecordset();

		m_DBCommand->CommandText = pszSPName;

		//ִ������
		if (bRecordset == true)
		{
			m_DBRecordset->PutRefSource(m_DBCommand);
			m_DBRecordset->CursorLocation = adUseClient;
			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified));
		}
		else
		{
			m_DBConnection->CursorLocation = adUseClient;
			EfficacyResult(m_DBCommand->Execute(NULL, NULL, adExecuteNoRecords));
		}
	}
	catch (CComError & ComError)
	{
		if (IsConnectError() == true)	TryConnectAgain(false, &ComError);
		else SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//ִ������
VOID CDataBase::ExecuteSentence(LPCTSTR pszCommand, bool bRecordset)
{
	ASSERT(pszCommand != NULL);
	try
	{
		m_DBConnection->CursorLocation = adUseClient;
		m_DBConnection->Execute(pszCommand, NULL, adExecuteNoRecords);
	}
	catch (CComError & ComError)
	{
		if (IsConnectError() == true)	TryConnectAgain(false, &ComError);
		else SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//��ȡ����
LPCTSTR CDataBase::GetComErrorDescribe(CComError & ComError)
{
	_bstr_t bstrDescribe(ComError.Description());
	m_strErrorDescribe.Format(TEXT("ADO ����0x%8x��%s"), ComError.Error(), (LPCTSTR)bstrDescribe);
	return m_strErrorDescribe;
}

//���ô���
void CDataBase::SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe)
{
	ASSERT(FALSE);
	m_ADOError.SetErrorInfo(enErrorType, pszDescribe);
	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngine::CDataBaseEngine(void)
{
	//���ñ���
	m_bService = false;
	m_pIDataBaseEngineSinks.clear();

	return;
}

//��������
CDataBaseEngine::~CDataBaseEngine(void)
{
}

//�ӿڲ�ѯ
void * CDataBaseEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngine, Guid, dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngine, Guid, dwQueryVer);
	return NULL;
}
/*
//ע��ӿ�
bool CDataBaseEngine::SetDataBaseEngineSink(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx != NULL);
	ASSERT(m_pIDataBaseEngineSink == NULL);
	if (pIUnknownEx == NULL) return false;
	if (m_pIDataBaseEngineSink != NULL) return false;

	//��ѯ�ӿ�
	m_pIDataBaseEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBaseEngineSink);
	if (m_pIDataBaseEngineSink == NULL)
	{
		CTraceService::TraceString(TEXT("���ݿ�������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"), TraceLevel_Exception);
		return false;
	}

	return true;
}

//ע��ӿ�
bool CDataBaseEngine::SetDataBaseEngineSink(IUnknownEx * pIUnknownEx[], WORD wSinkCount)
{

	//Ч�����
	ASSERT(pIUnknownEx != NULL);
	if (pIUnknownEx == NULL) return false;

	for (WORD i=0;i<wSinkCount;i++)
	{
		if (SetDataBaseEngineSink(pIUnknownEx[i])==false)
		{
			return false;
		}
	}
	return true;
}
*/
//ע��ӿ�
bool  CDataBaseEngine::SetDataBaseEngineSink(IUnknownEx * pIUnknownEx)
{
	/*
	//Ч�����
	ASSERT(pIUnknownEx != NULL);
	ASSERT(m_pIDataBaseEngineSink == NULL);
	if (pIUnknownEx == NULL) return false;
	if (m_pIDataBaseEngineSink != NULL) return false;

	//��ѯ�ӿ�
	m_pIDataBaseEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBaseEngineSink);
	if (m_pIDataBaseEngineSink == NULL)
	{
	CTraceService::TraceString(TEXT("���ݿ�������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"), TraceLevel_Exception);
	return false;
	}
	*/
	SetDataBaseEngineSink(&pIUnknownEx, 1);
	return true;
}

bool  CDataBaseEngine::SetDataBaseEngineSink(IUnknownEx * pIUnknownExs[], WORD wSinkCount)
{
	m_pIDataBaseEngineSinks.clear();
	wSinkCount = 1;
	for (WORD i = 0; i < wSinkCount; i++)
	{
		IUnknownEx *pIUnknownEx = pIUnknownExs[i];
		//Ч�����
		ASSERT(pIUnknownEx != NULL);
		if (pIUnknownEx == NULL) return false;

		//��ѯ�ӿ�
		IDataBaseEngineSink * pIDataBaseEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBaseEngineSink);
		if (pIDataBaseEngineSink == NULL)
		{
			CTraceService::TraceString(TEXT("���ݿ�������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"), TraceLevel_Exception);
			return false;
		}
		m_pIDataBaseEngineSinks.push_back(pIDataBaseEngineSink);
	}
	return true;
}
//��������
bool CDataBaseEngine::StartService()
{
	//�ж�״̬
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("���ݿ������ظ�������������������"), TraceLevel_Warning);
		return true;
	}

	//��ҽӿ�
	if (m_pIDataBaseEngineSinks.empty())
	{
		CTraceService::TraceString(TEXT("���ݿ�������ҷ��񲻴���"), TraceLevel_Exception);
		return false;
	}

	////���ö���
	//if (m_QueueServiceEvent.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	//{
	//	CTraceService::TraceString(TEXT("���ݿ���������з����ʧ��"), TraceLevel_Exception);
	//	return false;
	//}

	//�������
	for (WORD i = 0; i < m_pIDataBaseEngineSinks.size(); i++)
	{
		//�������
		if (m_pIDataBaseEngineSinks[i]->OnDataBaseEngineStart(QUERY_ME_INTERFACE(IUnknownEx)) == false)
		{
			CTraceService::TraceString(TEXT("���ݿ�������ҷ�������ʧ��"), TraceLevel_Exception);
			return false;
		}
	}

	m_QueueServiceEvent.SetQueueService(&m_QueueService);
	//���ö���
	if (m_QueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		CTraceService::TraceString(TEXT("������������з����ʧ��"), TraceLevel_Exception);
		return false;
	}

	//��������
	if (m_QueueService.StartService() == false)
	{
		CTraceService::TraceString(TEXT("����������з�������ʧ��"), TraceLevel_Exception);
		return false;
	}
	//���ñ���
	m_bService = true;

	return true;
}

//ֹͣ����
bool CDataBaseEngine::ConcludeService()
{
	//���ñ���
	m_bService = false;

	//ֹͣ�������
	//m_QueueServiceEvent.SetQueueServiceSink(NULL);
	//ֹͣ�������
	m_QueueService.ConcludeService();

	//ֹͣ���
	for (WORD i = 0; i < m_pIDataBaseEngineSinks.size(); i++)
	{
		//ֹͣ���
		if (m_pIDataBaseEngineSinks[i] != NULL)
		{
			m_pIDataBaseEngineSinks[i]->OnDataBaseEngineConclude(QUERY_ME_INTERFACE(IUnknownEx));
		}
	}
	m_pIDataBaseEngineSinks.clear();


	//m_QueueServiceEvent.SetQueueServiceSink(NULL);

	return true;
}

//���渺��
bool CDataBaseEngine::GetBurthenInfo( tagBurthenInfo & BurthenInfo )
{
	ASSERT(TRUE);ASSERT(FALSE);
	return true;
}

//���нӿ�
void CDataBaseEngine::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)
{
	//�ж�״̬
	if (m_bService == false) return;

	//������
	switch (wIdentifier)
	{
		case EVENT_DATABASE:
		{
			//Ч�����
			ASSERT(pBuffer != NULL);
			ASSERT(wDataSize >= sizeof(NTY_DataBaseEvent));
			if (wDataSize < sizeof(NTY_DataBaseEvent)) return;

			//��������
			NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)pBuffer;
			WORD wHandleBuffer = wDataSize - sizeof(NTY_DataBaseEvent);

			//��������
			for (WORD i = 0; i < m_pIDataBaseEngineSinks.size(); i++)
			{
				ASSERT(m_pIDataBaseEngineSinks[i] != NULL);
				m_pIDataBaseEngineSinks[i]->OnDataBaseEngineRequest(pDataBaseEvent->wRequestID, pDataBaseEvent->dwContextID, pDataBaseEvent + 1, wHandleBuffer);
			}

			return;
		}
		case EVENT_TIMER://ʱ���¼�
		{
			//Ч�����
			ASSERT(pBuffer != NULL);
			ASSERT(wDataSize >= sizeof(NTY_TimerEvent));
			if (wDataSize < sizeof(NTY_TimerEvent)) return;

			//��������
			NTY_TimerEvent * pDataBaseEvent = (NTY_TimerEvent *)pBuffer;
			WORD wHandleBuffer = wDataSize - sizeof(NTY_TimerEvent);

			//��������
			for (WORD i = 0; i < m_pIDataBaseEngineSinks.size(); i++)
			{
				ASSERT(m_pIDataBaseEngineSinks[i] != NULL);
				m_pIDataBaseEngineSinks[i]->OnDataBaseEngineTimer(pDataBaseEvent->dwTimerID, pDataBaseEvent->dwBindParameter);
			}
			return;
		}
		case EVENT_CONTROL://�����¼�
		{
			//Ч�����
			ASSERT(pBuffer != NULL);
			ASSERT(wDataSize >= sizeof(NTY_ControlEvent));
			if (wDataSize < sizeof(NTY_ControlEvent)) return;

			//��������
			NTY_ControlEvent * pDataBaseEvent = (NTY_ControlEvent *)pBuffer;
			WORD wHandleBuffer = wDataSize - sizeof(NTY_ControlEvent);

			//��������
			for (WORD i = 0; i < m_pIDataBaseEngineSinks.size(); i++)
			{
				ASSERT(m_pIDataBaseEngineSinks[i] != NULL);
				m_pIDataBaseEngineSinks[i]->OnDataBaseEngineControl(pDataBaseEvent->wControlID, pDataBaseEvent + 1, wHandleBuffer);
			}
			return;
		}
		default:
		{
			ASSERT(false);
		}
	}

	return;
}

//�����¼�
//////////////////////////////////////////////////////////////////////////
//�����¼�
bool CDataBaseEngine::PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return m_QueueServiceEvent.PostControlEvent(wControlID, pData, wDataSize);
}

//�����¼�
bool CDataBaseEngine::PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (m_bService == false) return false;
	return m_QueueServiceEvent.PostDataBaseEvent(wRequestID, dwContextID, pData, wDataSize);
}

//��������
bool CDataBaseEngine::DeferDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(FALSE);
	return false;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * CreateDataBaseEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDataBaseEngine * pDataBaseEngine = NULL;
	try
	{
		pDataBaseEngine = new CDataBaseEngine();
		if (pDataBaseEngine == NULL) throw TEXT("����ʧ��");
		void * pObject = pDataBaseEngine->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pDataBaseEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * CreateDataBase(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDataBase * pDataBase = NULL;
	try
	{
		pDataBase = new CDataBase();
		if (pDataBase == NULL) throw TEXT("����ʧ��");
		void * pObject = pDataBase->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pDataBase);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
