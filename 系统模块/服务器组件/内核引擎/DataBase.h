#ifndef DATA_BASE_ENGINE_HEAD_FILE
#define DATA_BASE_ENGINE_HEAD_FILE

#pragma once

//组件头文件
#include "KernelEngineHead.h"
#include "QueueService.h"
#include "TraceService.h"
#include "QueueServiceEvent.h"
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////////

//ADO 错误类
class CADOError : public IDataBaseException
{
	//变量定义
protected:
	enSQLException					m_enErrorType;						//错误代号
	CString							m_strErrorDescribe;					//错误信息

	//函数定义
public:
	//构造函数
	CADOError();
	//析构函数
	virtual ~CADOError();

	//基础接口
public:
	//释放对象
	virtual VOID Release()
	{
		return /*true*/;
	}
	//是否有效
	virtual bool IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CADOError)) ? true : false;
	}
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//功能接口
public:
	//错误类型
	virtual enSQLException GetExceptionType()
	{
		return m_enErrorType;
	}
	//错误描述
	virtual LPCTSTR GetExceptionDescribe()
	{
		return m_strErrorDescribe;
	}
	//异常代码
	virtual HRESULT GetExceptionResult()
	{
		return (HRESULT)0;    //
	}

	//功能函数
public:
	//设置错误
	void SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe);
};

//////////////////////////////////////////////////////////////////////////

//数据库对象
class CDataBase : public IDataBase
{
	//信息变量
protected:
	CADOError						m_ADOError;							//错误对象
	CString							m_strConnect;						//连接字符串
	CString							m_strErrorDescribe;					//错误信息

	//状态变量
protected:
	DWORD							m_dwConnectCount;					//重试次数
	DWORD							m_dwConnectErrorTime;				//错误时间
	const DWORD						m_dwResumeConnectCount;				//恢复次数
	const DWORD						m_dwResumeConnectTime;				//恢复时间

	//内核变量
protected:
	_CommandPtr						m_DBCommand;						//命令对象
	_RecordsetPtr					m_DBRecordset;						//记录集对象
	_ConnectionPtr					m_DBConnection;						//数据库对象

	//函数定义
public:
	//构造函数
	CDataBase();
	//析构函数
	virtual ~CDataBase();

	//基础接口
public:
	//释放对象
	virtual VOID Release()
	{
		if (IsValid()) delete this;
		return /*true*/;
	}
	//是否有效
	virtual bool IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CDataBase)) ? true : false;
	}
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//打开连接
	virtual VOID OpenConnection();
	//关闭记录
	virtual VOID CloseRecordset();
	//关闭连接
	virtual VOID CloseConnection();
	//重新连接
	virtual bool TryConnectAgain(bool bFocusConnect, CComError * pComError);
	//连接信息
	virtual bool SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//设置信息
	virtual bool SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//切换记录
	virtual VOID NextRecordset();

	//状态接口
public:
	//是否连接错误
	virtual bool IsConnectError();
	//是否打开
	virtual bool IsRecordsetOpened();

	//记录集接口
public:
	//往下移动
	virtual void MoveToNext();
	//移到开头
	virtual void MoveToFirst();
	//是否结束
	virtual bool IsRecordsetEnd();
	//获取数目
	virtual long GetRecordCount();
	//获取大小
	virtual long GetActualSize(LPCTSTR pszParamName);
	//绑定对象
	virtual VOID BindToRecordset(CADORecordBinding * pBind);
	//获取数据
	virtual VOID GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue);
	//字段接口
public:
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, INT & nValue);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, LPSTR szBuffer, UINT uSize);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, LPWSTR szBuffer, UINT uSize);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);
	//获取参数
	virtual bool GetFieldValue(LPCTSTR lpFieldName, bool & bValue);

	//命令对象接口
public:
	//插入参数
	virtual void AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue);
	//删除参数
	virtual void ClearParameters();
	//获得参数
	virtual void GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue);
	//获取返回数值
	virtual long GetReturnValue();

	//执行接口
public:
	//执行语句
	virtual VOID ExecuteProcess(LPCTSTR pszSPName, bool bRecordset);
	//执行命令
	virtual VOID ExecuteSentence(LPCTSTR pszCommand, bool bRecordset);

	//内部函数
private:
	//获取错误
	LPCTSTR GetComErrorDescribe(CComError & ComError);
	//设置错误
	void SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe);
};

//////////////////////////////////////////////////////////////////////////

//数据库管理类
class CDataBaseEngine : public IDataBaseEngine, public IQueueServiceSink
{
	//内核变量
protected:
	bool							m_bService;							//运行标志
	CQueueServiceEvent				m_QueueServiceEvent;				//队列对象
	CQueueService                   m_QueueService;
	//IDataBaseEngineSink				* m_pIDataBaseEngineSink;			//通知钩子

	vector<IDataBaseEngineSink *>   m_pIDataBaseEngineSinks;			//通知钩子
	//函数定义
public:
	//构造函数
	CDataBaseEngine(void);
	//析构函数
	virtual ~CDataBaseEngine(void);

	//基础接口
public:
	//释放对象
	virtual VOID Release()
	{
		if (IsValid()) delete this;
		return /*true*/;
	}
	//是否有效
	virtual bool IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CDataBaseEngine)) ? true : false;
	}
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//服务接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();

public:
	//引擎负荷
	virtual bool GetBurthenInfo(tagBurthenInfo & BurthenInfo);

	//配置接口
public:
	//配置模块
	virtual bool SetDataBaseEngineSink(IUnknownEx * pIUnknownEx);
	//配置模块
	virtual bool SetDataBaseEngineSink(IUnknownEx * pIUnknownEx[], WORD wSinkCount);

	//队列接口
public:
	//队列接口
	virtual void OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize);

	//控制事件
public:
	//控制事件
	virtual bool PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//延期请求
	virtual bool DeferDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif