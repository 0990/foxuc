#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseEngineSink::CDataBaseEngineSink() {
    //组件变量
    m_pInitParameter=NULL;
    m_pIDataBaseEngineEvent=NULL;

    return;
}

//析构函数
CDataBaseEngineSink::~CDataBaseEngineSink() {
}

//接口查询
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer) {
    QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
    return NULL;
}

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx) {
	//这里暂时不用数据库服务
	return true;
    //创建对象
    if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false)) {
        ASSERT(FALSE);
        return false;
    }

    //创建对象
    if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false)) {
        ASSERT(FALSE);
        return false;
    }

    //创建对象
    if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false)) {
        ASSERT(FALSE);
        return false;
    }

    try {
        //连接信息
        tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
        tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
        tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

        //设置连接
        m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
                                              pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass);
        m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
                                              pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass);
        m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
                                              pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass);

        //发起连接
        m_AccountsDBModule->OpenConnection();
        m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

        //发起连接
        m_TreasureDBModule->OpenConnection();
        m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

        //发起连接
        m_PlatformDBModule->OpenConnection();
        m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
        CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

        return false;
    }

    return true;
}

//停止事件
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx) {
    //设置对象
    m_AccountsDBAide.SetDataBase(NULL);
    m_TreasureDBAide.SetDataBase(NULL);
    m_PlatformDBAide.SetDataBase(NULL);

    //关闭连接
    if (m_AccountsDBModule.GetInterface()!=NULL) {
        m_AccountsDBModule->CloseConnection();
        m_AccountsDBModule.CloseInstance();
    }

    //关闭连接
    if (m_TreasureDBModule.GetInterface()!=NULL) {
        m_TreasureDBModule->CloseConnection();
        m_TreasureDBModule.CloseInstance();
    }

    //关闭连接
    if (m_PlatformDBModule.GetInterface()!=NULL) {
        m_PlatformDBModule->CloseConnection();
        m_PlatformDBModule.CloseInstance();
    }

    //组件变量
    m_pInitParameter=NULL;
    m_pIDataBaseEngineEvent=NULL;

    return true;
}

//时间事件
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter) {
    return false;
}

//控制事件
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize) {
    return false;
}

//请求事件
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize) {
    switch (wRequestID) {
    case DBR_GP_LOGON_GAMEID: {		//标识登录
        return OnRequestLogonGameID(dwContextID, pData, wDataSize);
    }
    case DBR_GP_LOGON_ACCOUNTS: {		//帐号登录
        return OnRequestLogonAccounts(dwContextID, pData, wDataSize);
    }
    case DBR_GP_REGISTER_ACCOUNTS: {	//注册帐号
        return OnRequestRegisterAccounts(dwContextID, pData, wDataSize);
    }
    case DBR_GP_MODIFY_MACHINE: {		//修改机器
        return OnRequestModifyMachine(dwContextID, pData, wDataSize);
    }
    case DBR_GP_MODIFY_LOGON_PASS: {	//修改密码
        return OnRequestModifyLogonPass(dwContextID, pData, wDataSize);
    }
    case DBR_GP_MODIFY_INSURE_PASS: {	//修改密码
        return OnRequestModifyInsurePass(dwContextID, pData, wDataSize);
    }
    case DBR_GP_MODIFY_UNDER_WRITE: {	//修改签名
        return OnRequestModifyUnderWrite(dwContextID, pData, wDataSize);
    }
    case DBR_GP_MODIFY_SYSTEM_FACE: {	//修改头像
        return OnRequestModifySystemFace(dwContextID, pData, wDataSize);
    }
    case DBR_GP_MODIFY_CUSTOM_FACE: {	//修改头像
        return OnRequestModifyCustomFace(dwContextID, pData, wDataSize);
    }
    case DBR_GP_MODIFY_INDIVIDUAL: {	//修改资料
        return OnRequestModifyIndividual(dwContextID, pData, wDataSize);
    }
    case DBR_GP_USER_SAVE_SCORE: {	//存入游戏币
        return OnRequestUserSaveScore(dwContextID, pData, wDataSize);
    }
    case DBR_GP_USER_TAKE_SCORE: {	//提取游戏币
        return OnRequestUserTakeScore(dwContextID, pData, wDataSize);
    }
    case DBR_GP_USER_TRANSFER_SCORE: {	//转账游戏币
        return OnRequestUserTransferScore(dwContextID, pData, wDataSize);
    }
    case DBR_GP_QUERY_INDIVIDUAL: {	//查询资料
        return OnRequestQueryIndividual(dwContextID, pData, wDataSize);
    }
    case DBR_GP_QUERY_INSURE_INFO: {	//查询银行
        return OnRequestQueryInsureInfo(dwContextID, pData, wDataSize);
    }
    case DBR_GP_QUERY_USER_INFO: {	//查询用户
        return OnRequestQueryTransferUserInfo(dwContextID, pData, wDataSize);
    }
    case DBR_GP_LOAD_GAME_LIST: {		//加载列表
        return OnRequestLoadGameList(dwContextID, pData, wDataSize);
    }
    case DBR_GP_ONLINE_COUNT_INFO: {	//在线信息
        return OnRequestOnLineCountInfo(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOGON_GAMEID: {		//标识登录
        return OnMobileLogonGameID(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOGON_ACCOUNTS: {		//帐号登录
        return OnMobileLogonAccounts(dwContextID, pData, wDataSize);
    }
    case DBR_MB_REGISTER_ACCOUNTS: {	//注册帐号
        return OnMobileRegisterAccounts(dwContextID, pData, wDataSize);
    }
    case DBR_GP_ALMS_INFO: {	//注册帐号
        return OnRequestAlmsInfo(dwContextID, pData, wDataSize);
    }
    case DBR_MB_RECORD_INFO: {
        return OnRequestRecordInfo(dwContextID, pData, wDataSize);
    }
    case DBR_MB_EXCHANGE_ROOM_CARD: {
        return OnRequestExchangeRoomCard(dwContextID, pData, wDataSize);
    }
    case DBR_MB_PRIVATE_CREATE: {
        return OnRequestPrivateCreate(dwContextID, pData, wDataSize);
    }
    case DBR_MB_PRIVATE_JOIN_CHECK: {
        return OnRequestPrivateJoinCheck(dwContextID, pData, wDataSize);
    }
    case DBR_MB_PRIVATE_BACK_ROOM: {		//回到房间
        return OnRequestPrivateBackRoom(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOGON_VISITOR: {
        return OnMobileLogonVisitor(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOGON_WX: {
        return OnMobileLogonWX(dwContextID, pData, wDataSize);
    }
    case DBR_MB_ROOM_CARD_INFO: {			//赠送房卡
        return OnRequestTransferRoomCard(dwContextID, pData, wDataSize);
    }
    case DBR_MB_INSURE_PASSWORD_INFO: {			//设置银行密码
        return OnSetInsurePassword(dwContextID, pData, wDataSize);
    }
    case DBR_MB_FRAGMENT_EXCHANGE_RATE: {				//碎钻兑换房卡
        return OnFragmentExchangRate(dwContextID, pData, wDataSize);
    }
    case DBR_MB_SEEK_NICKNAME: {				//搜索昵称
        return OnSeekNickname(dwContextID, pData, wDataSize);
    }
    case DBR_MB_SEEK_USER_TREASURE: {			//搜索财富
        return OnSeekUserTreaure(dwContextID, pData, wDataSize);
    }
    case DBR_MB_TASK_COMPLETED: {				//大厅任务
        return OnTaskComplete(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOAD_USER_TASK: {			//加载玩家任务
        return OnLoadUserTask(dwContextID, pData, wDataSize);
    }
    case DBR_MB_ACCEPT_TASK_AWARD: {				//领取奖励
        return OnReceiveAward(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOAD_TASK_INFO: {				//加载任务信息
        return OnLoadTaskInfo(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOAD_SIGN_IN_RECORD: {		//签到记录
        return OnLoadSignInRecord(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOAD_SYSTEM_MESSAGE: {		//系统消息
        return OnRequestLoadSystemMessage(dwContextID, pData, wDataSize);
    }
    case DBR_MB_LOAD_LOGON_CONFIG: {		//加载版本号
        return OnRequestLoadLogonConfig(dwContextID, pData, wDataSize);
    }
    case DBR_MB_GET_PRIVATE_RECORD: {
        return OnRequestGetPrivateRecord(dwContextID, pData, wDataSize);
    }
    case DBR_MB_RECEIVE_ALMS: {
        return OnRequestReceiveAlms(dwContextID, pData, wDataSize);
    }
    case DBR_MB_USER_BIND_PROMOTE: {
        return OnRequestUserBindPromote(dwContextID, pData, wDataSize);
    }
    }

    return false;
}

//I D 登录
bool CDataBaseEngineSink::OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_LogonGameID));
        if (wDataSize!=sizeof(DBR_GP_LogonGameID)) return false;

        //执行查询
        DBR_GP_LogonGameID * pLogonGameID=(DBR_GP_LogonGameID *)pData;

        //变量定义
        tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
        if (pBindParameter->dwSocketID!=dwContextID) return true;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
        m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonGameID->cbNeeValidateMBCard);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

        return false;
    }

    return true;
}

//帐号登录
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_LogonAccounts));
        if (wDataSize!=sizeof(DBR_GP_LogonAccounts)) return false;

        //请求处理
        DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

        //执行判断
        tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
        if (pBindParameter->dwSocketID!=dwContextID) return true;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
        m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

        return false;
    }

    return true;
}

//注册处理
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
        if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

        //请求处理
        DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

        //执行判断
        tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
        if (pBindParameter->dwSocketID!=dwContextID) return true;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
        m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
        m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
        m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
        m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
        m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
        m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
        m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
        m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

        return false;
    }

    return true;
}

//I D 登录
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_MB_LogonGameID));
        if (wDataSize!=sizeof(DBR_MB_LogonGameID)) return false;

        //执行查询
        DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

        //变量定义
        tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
        if (pBindParameter->dwSocketID!=dwContextID) return true;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
        m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

        return false;
    }

    return true;
}

//帐号登录
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_MB_LogonAccounts));
        if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

        //请求处理
        DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

        //执行判断
        tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
        if (pBindParameter->dwSocketID!=dwContextID) return true;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
        m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyAccounts"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

        return false;
    }

    return true;
}

//查询救济金
bool CDataBaseEngineSink::OnRequestAlmsInfo(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_AlmsInfo));
        if (wDataSize!=sizeof(DBR_GP_AlmsInfo)) return false;

        //请求处理
        DBR_GP_AlmsInfo * pAlmsInfo=(DBR_GP_AlmsInfo *)pData;


        //构造参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pAlmsInfo->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@bGetAlms"),pAlmsInfo->bGetAlms);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //结果处理
        if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryAlmsInfo"),true)==DB_SUCCESS) {
            //变量定义
            DBO_GP_AlmsSuccess UserAlmsSuccess;
            ZeroMemory(&UserAlmsSuccess,sizeof(UserAlmsSuccess));

            //银行信息
            UserAlmsSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
            UserAlmsSuccess.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
            UserAlmsSuccess.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
            UserAlmsSuccess.nUseCount=m_TreasureDBAide.GetValue_DWORD(TEXT("UseCount"));
            UserAlmsSuccess.bGetAlms=pAlmsInfo->bGetAlms;
            UserAlmsSuccess.nMaxUseCount=m_TreasureDBAide.GetValue_DWORD(TEXT("MaxUseCount"));
            UserAlmsSuccess.lAlmsScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("AlmsScore"));
            UserAlmsSuccess.lAlmsThreshold=m_TreasureDBAide.GetValue_LONGLONG(TEXT("AlmsThreshold"));

            CDBVarValue DBVarValue;
            m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
            lstrcpyn(UserAlmsSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(UserAlmsSuccess.szDescribeString));

            //发送结果
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_ALMS_SUCCESS,dwContextID,&UserAlmsSuccess,sizeof(UserAlmsSuccess));
        } else {
            CDBVarValue DBVarValue;
            m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

            DBO_GP_AlmsFailure UserAlmsFaulure;
            ZeroMemory(&UserAlmsFaulure,sizeof(UserAlmsFaulure));
            UserAlmsFaulure.lResultCode = m_TreasureDBModule->GetReturnValue();
            lstrcpyn(UserAlmsFaulure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(UserAlmsFaulure.szDescribeString));

            //发送结果
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_ALMS_FAILURE,dwContextID,&UserAlmsFaulure,sizeof(UserAlmsFaulure));
        }

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        DBR_GP_AlmsInfo * pAlmsInfo=(DBR_GP_AlmsInfo *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("GSP_GR_QueryAlmsInfo-@dwUserID:%d-@bGetAlms:%d"),pAlmsInfo->dwUserID,(INT)pAlmsInfo->bGetAlms);
        CTraceService::TraceString(szBuffer,TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//查询游戏记录
bool CDataBaseEngineSink::OnRequestRecordInfo(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //效验参数
    ASSERT(wDataSize == sizeof(DBR_MB_RecordInfo));
    if (wDataSize != sizeof(DBR_MB_RecordInfo)) return false;

    //请求处理
    DBR_MB_RecordInfo * pRecordInfo = (DBR_MB_RecordInfo *)pData;
    try {

        //读取类型
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pRecordInfo->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@dwCurrRecordID"), pRecordInfo->nCurrRecordID);

        //发送类型
        WORD wPacketSize = 0;
        DBO_MB_GameRecord  GameRecord;
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_LoadGameRecord"), true) == DB_SUCCESS) {
            GameRecord.nRecordID = m_AccountsDBAide.GetValue_DWORD(TEXT("DrawID"));
            GameRecord.wKindID = m_AccountsDBAide.GetValue_DWORD(TEXT("KindID"));
            GameRecord.wServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("ServerID"));
            //读取时间
            SYSTEMTIME systime;
            ZeroMemory(&systime, sizeof(systime));
            m_AccountsDBAide.GetValue_SystemTime(TEXT("InsertTime"), systime);

            //转换时间
            CTime time(systime);
            GameRecord.tRecordTime = time.GetTime();

            m_AccountsDBAide.GetValue_String(TEXT("RecordData"), GameRecord.szRecordData, CountArray(GameRecord.szRecordData));
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_RECORD_INFO, dwContextID, &GameRecord, sizeof(GameRecord));
        }

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_RecordInfo * pRecordInfo = (DBR_MB_RecordInfo *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_LoadGameRecord-@dwUserID:%d"),(int)pRecordInfo->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return true;
}

//查询游戏记录
bool CDataBaseEngineSink::OnRequestExchangeRoomCard(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //效验参数
    ASSERT(wDataSize == sizeof(DBR_MB_ExchangeRoomCard));
    if (wDataSize != sizeof(DBR_MB_ExchangeRoomCard)) return false;

    //请求处理
    DBR_MB_ExchangeRoomCard * pExchange = (DBR_MB_ExchangeRoomCard *)pData;
    try {

        //读取类型
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pExchange->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@dwExchangeGameID"), pExchange->dwExchangeGameID);
        m_AccountsDBAide.AddParameter(TEXT("@strCardNum"), pExchange->szCardNum);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        DBO_MB_ExchangeRoomCard  ExchangeRoomCard;
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_ExchangeRoomCard"), true) == DB_SUCCESS) {
            ExchangeRoomCard.bSuccess = true;
            ExchangeRoomCard.dRoomCardCount = m_AccountsDBAide.GetValue_DOUBLE(TEXT("RoomCardCount"));
        } else {
            ExchangeRoomCard.bSuccess = false;
        }

        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(ExchangeRoomCard.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(ExchangeRoomCard.szDescribeString));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_EXCHANGE_ROOM_CARD, dwContextID, &ExchangeRoomCard, sizeof(ExchangeRoomCard));

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_RecordInfo * pRecordInfo = (DBR_MB_RecordInfo *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_ExchangeRoomCard-@dwUserID:%d"), (int)pRecordInfo->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return true;
}

//赠送房卡
bool CDataBaseEngineSink::OnRequestTransferRoomCard(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_RoomCardInfo));
    if (wDataSize != sizeof(DBR_MB_RoomCardInfo))	return false;

    DBR_MB_RoomCardInfo * roomCardInfo = (DBR_MB_RoomCardInfo *)pData;
    try {
        m_TreasureDBAide.ResetParameter();
        //输入参数
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), roomCardInfo->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@dwGameID"), roomCardInfo->dwGameID);
        m_TreasureDBAide.AddParameter(TEXT("@szPassword"), roomCardInfo->szPassword);
        m_TreasureDBAide.AddParameter(TEXT("@dwRoomCardCount"), roomCardInfo->dwRoomCardCount);
        m_TreasureDBAide.AddParameter(TEXT("@dwRedDiamondCount"), roomCardInfo->dwRedDiamondCount);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

        DBO_MB_RoomCardInfo  roomCard;
        if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_MB_UserTransferRoomCard"), true) == DB_SUCCESS) {
            roomCard.bSuccess = true;
            roomCard.dRoomCardCount = m_TreasureDBAide.GetValue_DOUBLE(TEXT("roomCardCount"));
            roomCard.dRedDiamondCount = m_TreasureDBAide.GetValue_DOUBLE(TEXT("redDiamondCount"));
        } else {
            roomCard.bSuccess = false;
        }

        CDBVarValue DBVarValue;
        m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(roomCard.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(roomCard.szDescribeString));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_ROOM_CARD_INFO, dwContextID, &roomCard, sizeof(roomCard));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_RoomCardInfo * pRoomCardInfo = (DBR_MB_RoomCardInfo *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_UserTransferRoomCard-@dwUserID:%d"), (int)pRoomCardInfo->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }
    return false;
}

//设置银行密码
bool CDataBaseEngineSink::OnSetInsurePassword(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_InsurePasswdInfo));
    if (wDataSize != sizeof(DBR_MB_InsurePasswdInfo))	return false;

    DBR_MB_InsurePasswdInfo *insurePasswdInfo = (DBR_MB_InsurePasswdInfo *)pData;
    try {
        m_AccountsDBAide.ResetParameter();
        //输入参数
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), insurePasswdInfo->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@szLogPassword"), insurePasswdInfo->szLogPasswd);
        m_AccountsDBAide.AddParameter(TEXT("@szInsurePassword"), insurePasswdInfo->szPassword);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

        DBO_MB_InsurePasswdInfo  insurePasswd;
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_SetInsurePassword"), true) == DB_SUCCESS) {
            insurePasswd.bSuccess = true;
        } else {
            insurePasswd.bSuccess = false;
        }

        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(insurePasswd.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(insurePasswd.szDescribeString));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_INSURE_PASSWORD_INFO, dwContextID, &insurePasswd, sizeof(insurePasswd));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_InsurePasswdInfo *pInsurePasswdInfo = (DBR_MB_InsurePasswdInfo *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_SetInsurePassword-@dwUserID:%d"), (int)pInsurePasswdInfo->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }
    return false;
}
//碎钻兑换房卡
bool CDataBaseEngineSink::OnFragmentExchangRate(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_FragmentExchangRate));
    if (wDataSize != sizeof(DBR_MB_FragmentExchangRate))	return false;

    DBR_MB_FragmentExchangRate * fragmentExchangeRateInfo = (DBR_MB_FragmentExchangRate *)pData;
    try {
        //输入参数
        m_TreasureDBAide.ResetParameter();

        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), fragmentExchangeRateInfo->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@szLogPassword"), fragmentExchangeRateInfo->szLogPasswd);
        m_TreasureDBAide.AddParameter(TEXT("@wDiamondType"), fragmentExchangeRateInfo->wDiamondType);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        DBO_MB_FragmentExchangRate  fragmentExchangRate;
        if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_MB_FragmentExchangRate"), true) == DB_SUCCESS) {
            fragmentExchangRate.bSuccess = true;
            fragmentExchangRate.dRoomCardCount = m_TreasureDBAide.GetValue_DOUBLE(TEXT("RoomCardCount"));
            fragmentExchangRate.dwFragmentCardCount = m_TreasureDBAide.GetValue_DWORD(TEXT("FragmentCardCount"));
            fragmentExchangRate.dRedDiamondCount = m_TreasureDBAide.GetValue_DOUBLE(TEXT("RedDiamondCount"));
            fragmentExchangRate.wDiamondType = fragmentExchangeRateInfo->wDiamondType;
        } else {
            fragmentExchangRate.bSuccess = false;
        }

        CDBVarValue DBVarValue;
        m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(fragmentExchangRate.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(fragmentExchangRate.szDescribeString));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_FRAGMENT_EXCHANGE_RATE, dwContextID, &fragmentExchangRate, sizeof(fragmentExchangRate));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_FragmentExchangRate * pFragmentExchangRateInfo = (DBR_MB_FragmentExchangRate *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_FragmentExchangRate-@dwUserID:%d"), (int)pFragmentExchangRateInfo->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }
    return false;
}
//搜索昵称
bool CDataBaseEngineSink::OnSeekNickname(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_SeekNickname));
    if (wDataSize != sizeof(DBR_MB_SeekNickname))	return false;

    DBR_MB_SeekNickname *seekNicknameInfo = (DBR_MB_SeekNickname *)pData;
    try {
        //输入参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwGameID"), seekNicknameInfo->dwGameID);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        DBO_MB_SeekNickname seekNickname;
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_SeekNickname"), true) == DB_SUCCESS) {
            seekNickname.bSuccess = true;
            m_AccountsDBAide.GetValue_String(TEXT("UserNickname"), seekNickname.szNickName, CountArray(seekNickname.szNickName));
        } else {
            seekNickname.bSuccess = false;
        }

        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(seekNickname.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(seekNickname.szDescribeString));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_SEEK_NICKNAME, dwContextID, &seekNickname, sizeof(seekNickname));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_SeekNickname * pSeekNicknameInfo = (DBR_MB_SeekNickname *)pData;
        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_SeekNickname-@dwGameID:%d"), (int)pSeekNicknameInfo->dwGameID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);
        return false;
    }
    return false;
}
//搜索财富
bool CDataBaseEngineSink::OnSeekUserTreaure(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_SeekUserTreasure));
    if (wDataSize != sizeof(DBR_MB_SeekUserTreasure))	return false;

    DBR_MB_SeekUserTreasure * seekUserTreasureInfo = (DBR_MB_SeekUserTreasure *)pData;
    try {
        //输入参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), seekUserTreasureInfo->dwUserID);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        DBO_MB_SeekUserTreasure seekUserTreasure;
        if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_MB_SeekUserTreasure"), true) == DB_SUCCESS) {
            seekUserTreasure.bSuccess = true;
            seekUserTreasure.lUserScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("userScore"));
            seekUserTreasure.dRoomCardCount = m_TreasureDBAide.GetValue_DOUBLE(TEXT("roomCardCount"));
            seekUserTreasure.dRedDiamondCount = m_TreasureDBAide.GetValue_DOUBLE(TEXT("redDiamondCount"));
            seekUserTreasure.dwFragmentCardCount = m_TreasureDBAide.GetValue_DWORD(TEXT("fragmentCardCount"));
        } else {
            seekUserTreasure.bSuccess = false;
        }

        CDBVarValue DBVarValue;
        m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(seekUserTreasure.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(seekUserTreasure.szDescribeString));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_SEEK_USER_TREASURE, dwContextID, &seekUserTreasure, sizeof(seekUserTreasure));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_SeekUserTreasure *pSeekUserTreasureInfo = (DBR_MB_SeekUserTreasure *)pData;
        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_SeekUserTreasure-@dwUserID:%d"), (int)pSeekUserTreasureInfo->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);
        return false;
    }
    return false;
}
//大厅任务
bool CDataBaseEngineSink::OnTaskComplete(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_TaskComplete));
    if (wDataSize != sizeof(DBR_MB_TaskComplete))	return false;

    DBR_MB_TaskComplete * taskComplete = (DBR_MB_TaskComplete *)pData;
    try {
        //输入参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), taskComplete->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@dwTaskID"), taskComplete->dwTaskID);
        m_AccountsDBAide.AddParameter(TEXT("@szPasswd"), taskComplete->szPasswd);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        DBO_MB_TaskComplete taskCompleteInfo;
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_TaskComplete"), true) == DB_SUCCESS) {
            taskCompleteInfo.bSuccess = true;
            if (taskComplete->dwTaskID == TASKID_SIGN_IN) {
                taskCompleteInfo.dwDays = m_AccountsDBAide.GetValue_DWORD(TEXT("DaysCount"));
                taskCompleteInfo.dwNowFragmentCardCount = m_AccountsDBAide.GetValue_DWORD(TEXT("nowFragmentCardCount"));
                taskCompleteInfo.dwFragmentCardCount = m_AccountsDBAide.GetValue_DWORD(TEXT("FragmentCardCount"));
            }
        } else {
            taskCompleteInfo.bSuccess = false;
        }
        taskCompleteInfo.dwTaskID = taskComplete->dwTaskID;
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(taskCompleteInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(taskCompleteInfo.szDescribeString));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_TASK_COMPLETE, dwContextID, &taskCompleteInfo, sizeof(taskCompleteInfo));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_TaskComplete * pTaskComplete = (DBR_MB_TaskComplete *)pData;
        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_TaskComplete-@dwUserID:%d"), (int)pTaskComplete->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);
        return false;
    }
    return false;

}
//加载玩家任务
bool CDataBaseEngineSink::OnLoadUserTask(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_LoadUserTask));
    if (wDataSize != sizeof(DBR_MB_LoadUserTask))	return false;

    DBR_MB_LoadUserTask * loadUserTask = (DBR_MB_LoadUserTask *)pData;
    try {
        //输入参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), loadUserTask->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@dwKindID"), loadUserTask->dwKindID);
        //输出参数
        DBO_MB_UserTaskInfo userTaskInfo;
        ZeroMemory(&userTaskInfo, sizeof(userTaskInfo));
        SYSTEMTIME stime;
        if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_MB_LoadUserTask"), true) == DB_SUCCESS) {
            for (WORD i = 0; i < MAX_TASK_COUNT; i++) {
                //结束判断
                if (m_TreasureDBModule->IsRecordsetEnd() == true) break;
                //溢出效验
                //ASSERT(userTaskInfo.dwTaskCount<CountArray(userTaskInfo.TaskInfo));
                //if (userTaskInfo.dwTaskCount >= CountArray(userTaskInfo.TaskInfo)) break;
                ASSERT(userTaskInfo.dwTaskCount<MAX_TASK_COUNT);
                userTaskInfo.dwTaskCount++;
                userTaskInfo.TaskInfo[i].dwTaskID = m_TreasureDBAide.GetValue_DWORD(TEXT("TaskID"));
                userTaskInfo.TaskInfo[i].dwFinishCount = m_TreasureDBAide.GetValue_DWORD(TEXT("FinishCount"));
                userTaskInfo.TaskInfo[i].bAccept = (bool)m_TreasureDBAide.GetValue_BYTE(TEXT("Accept"));
                userTaskInfo.TaskInfo[i].bFinish = (bool)m_TreasureDBAide.GetValue_BYTE(TEXT("Finish"));
                m_TreasureDBAide.GetValue_SystemTime(TEXT("LastTime"), stime);
                CTime	time(stime);
                userTaskInfo.TaskInfo[i].tLastTime = time.GetTime();

                //移动记录
                m_TreasureDBModule->MoveToNext();
            }
            userTaskInfo.bSuccess = true;

        } else {
            userTaskInfo.bSuccess = false;
        }
        WORD wHeadSize = sizeof(userTaskInfo)-sizeof(userTaskInfo.TaskInfo);
        WORD wDataSize = userTaskInfo.dwTaskCount*sizeof(userTaskInfo.TaskInfo[0]);
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOAD_USER_TASK, dwContextID, &userTaskInfo, wHeadSize + wDataSize);
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_LoadUserTask * pUserTaskInfo = (DBR_MB_LoadUserTask *)pData;
        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_LoadUserTask-@dwUserID:%d"), (int)pUserTaskInfo->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);
        return false;
    }
    return false;
}
//领取奖励
bool CDataBaseEngineSink::OnReceiveAward(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_AcceptTaskAward));
    if (wDataSize != sizeof(DBR_MB_AcceptTaskAward))	return false;

    DBR_MB_AcceptTaskAward * acceptTaskAward = (DBR_MB_AcceptTaskAward *)pData;
    try {
        //输入参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), acceptTaskAward->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@dwTaskID"), acceptTaskAward->dwTaskID);
        m_TreasureDBAide.AddParameter(TEXT("@szLogPass"), acceptTaskAward->szLogPasswd);

        //输出参数
        DBO_MB_AcceptTaskAward awardInfo;
        TCHAR szDescribeString[128] = TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_MB_ReceiveAward"), true) == DB_SUCCESS) {
            awardInfo.bSuccess = true;
            awardInfo.dwTaskID = m_TreasureDBAide.GetValue_DWORD(TEXT("TaskID"));
            awardInfo.dwAwardCount = m_TreasureDBAide.GetValue_DWORD(TEXT("AwardCount"));
            awardInfo.lScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
            awardInfo.dwFragmentCardCount = m_TreasureDBAide.GetValue_DWORD(TEXT("FragmentCardCount"));
        } else {
            awardInfo.bSuccess = false;
        }

        CDBVarValue DBVarValue;
        m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(awardInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(awardInfo.szDescribeString));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_ACCEPT_TASK_AWARD, dwContextID, &awardInfo, sizeof(awardInfo));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_AcceptTaskAward * pAcceptTaskAward = (DBR_MB_AcceptTaskAward *)pData;
        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_ReceiveAward-@dwUserID:%d"), (int)pAcceptTaskAward->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);
        return false;
    }
    return false;
}
//加载任务信息
bool CDataBaseEngineSink::OnLoadTaskInfo(DWORD dwContextID, VOID * pData, WORD wDataSize) {

    try {
        //输出参数
        DBO_MB_TaskInfo loadTaskInfo;
        ZeroMemory(&loadTaskInfo, sizeof(loadTaskInfo));
        if (m_PlatformDBAide.ExecuteProcess(TEXT("GSP_MB_LoadTaskInfo"), true) == DB_SUCCESS) {
            for (int i = 0; i < MAX_TASK_COUNT; i++) {
                if (m_PlatformDBModule->IsRecordsetEnd() == true)	break;

                loadTaskInfo.taskInfo[i].dwTaskID = m_PlatformDBAide.GetValue_DWORD(TEXT("TaskID"));
                m_PlatformDBAide.GetValue_String(TEXT("TaskName"), loadTaskInfo.taskInfo[i].szTaskName, CountArray(loadTaskInfo.taskInfo[i].szTaskName));
                loadTaskInfo.taskInfo[i].wTaskType = m_PlatformDBAide.GetValue_WORD(TEXT("TaskType"));
                loadTaskInfo.taskInfo[i].dwCondition = m_PlatformDBAide.GetValue_DWORD(TEXT("Condition"));
                loadTaskInfo.taskInfo[i].dwAmount = m_PlatformDBAide.GetValue_DWORD(TEXT("Amount"));
                loadTaskInfo.taskInfo[i].wAwardType = m_PlatformDBAide.GetValue_WORD(TEXT("AwardType"));
                loadTaskInfo.taskInfo[i].dwAwardCount = m_PlatformDBAide.GetValue_DWORD(TEXT("AwardCount"));
                loadTaskInfo.taskInfo[i].dwKindID = m_PlatformDBAide.GetValue_DWORD(TEXT("kindID"));
                //移动记录
                m_PlatformDBModule->MoveToNext();
                loadTaskInfo.dwTaskCount++;
            }
        }

        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOAD_TASK_INFO, dwContextID, &loadTaskInfo, sizeof(loadTaskInfo));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_LoadTaskInfo"));
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);
        return false;
    }
    return false;
}
//签到记录
bool CDataBaseEngineSink::OnLoadSignInRecord(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //校验参数
    ASSERT(wDataSize == sizeof(DBR_MB_SignInRecord));
    if (wDataSize != sizeof(DBR_MB_SignInRecord))	return false;

    DBR_MB_SignInRecord * SignIn = (DBR_MB_SignInRecord *)pData;
    try {

        //输入参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), SignIn->dwUserID);
        //输出参数
        DBO_MB_SignInRecord signInRecord;
        ZeroMemory(&signInRecord, sizeof(signInRecord));
        SYSTEMTIME stime;
        GetLocalTime(&stime);
        signInRecord.wToDay = GetSignInFreshDay(stime, 1);
        signInRecord.wMonth = GetSignInFreshDay(stime, 2);
        signInRecord.wDaysInMonth = GetSignInFreshDay(stime, 3);
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_SignInRecord"), true) == DB_SUCCESS) {
            for (int i = 0; i < 31; i++) {
                if (m_AccountsDBModule->IsRecordsetEnd() == true)	break;
                signInRecord.SignInRecord[i].wFragmentCardCounts = m_AccountsDBAide.GetValue_DWORD(TEXT("FragmentCardCount"));
                m_AccountsDBAide.GetValue_SystemTime(TEXT("InsertTime"), stime);
                // CTime	time(stime);
                signInRecord.SignInRecord[i].tSignInDate = GetSignInFreshDay(stime,1);
                signInRecord.wDays++;

                //移动记录
                m_AccountsDBModule->MoveToNext();
            }
        }
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_SignInDays"), true) == DB_SUCCESS) {

            signInRecord.wSignInDays = m_AccountsDBAide.GetValue_DWORD(TEXT("DaysCount"));
            int days = signInRecord.wSignInDays;
            if (days == 0 || signInRecord.wToDay - signInRecord.SignInRecord[0].tSignInDate > 1) {
                days = 0;
                for (int i = 0; i < (signInRecord.wDaysInMonth - signInRecord.wToDay + 1); i++) {
                    signInRecord.wFragmentCardCounts[i] = getFragmentCardCount(++days);
                }
            } else if (signInRecord.SignInRecord[0].tSignInDate == signInRecord.wToDay) {
                for (int i = 0; i < (signInRecord.wDaysInMonth - signInRecord.wToDay); i++) {
                    signInRecord.wFragmentCardCounts[i] = getFragmentCardCount(++days);
                }
            } else {
                for (int i = 0; i < (signInRecord.wDaysInMonth - signInRecord.wToDay+1); i++) {
                    signInRecord.wFragmentCardCounts[i] = getFragmentCardCount(++days);
                }
            }

        }

        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOAD_SIGN_IN_RECORD, dwContextID, &signInRecord, sizeof(signInRecord));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_SignInRecord-GSP_MB_SignInDays-%d"), (int)SignIn->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);
        return false;
    }
    return false;
}
//系统消息
bool CDataBaseEngineSink::OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //构造参数
        m_TreasureDBAide.ResetParameter();

        //执行查询
        LONG lReturnValue = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_MB_LoadSystemMessage"), true);

        //结果处理
        if (lReturnValue == 0) {
            while (true) {
                //结束判断
                if (m_TreasureDBModule->IsRecordsetEnd() == true) break;

                //发送消息
                //定义变量
                DBR_GR_SystemMessage SystemMessage;
                ZeroMemory(&SystemMessage, sizeof(SystemMessage));

                //读取消息
                SystemMessage.cbMessageID = m_TreasureDBAide.GetValue_BYTE(TEXT("ID"));
                SystemMessage.cbMessageType = m_TreasureDBAide.GetValue_BYTE(TEXT("MessageType"));
                SystemMessage.dwTimeRate = m_TreasureDBAide.GetValue_DWORD(TEXT("TimeRate"));
                m_TreasureDBAide.GetValue_String(TEXT("MessageString"), SystemMessage.szSystemMessage, CountArray(SystemMessage.szSystemMessage));

                //读取时间
                SYSTEMTIME systime;
                ZeroMemory(&systime, sizeof(systime));
                m_TreasureDBAide.GetValue_SystemTime(TEXT("ConcludeTime"), systime);

                //转换时间
                CTime time(systime);
                SystemMessage.tConcludeTime = time.GetTime();

                //发送结果
                m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOAD_SYSTEM_MESSAGE, dwContextID, &SystemMessage, sizeof(SystemMessage));


                //下一条
                m_TreasureDBModule->MoveToNext();
            }
        }

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        //错误处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return true;
}
//加载版本号
bool CDataBaseEngineSink::OnRequestLoadLogonConfig(DWORD dwContextID, VOID * pData, WORD wDataSize) {

    try {

        //输出参数
        DBO_MB_LoadLogonConfig Config;
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_LoadLogonConfig"), true) == DB_SUCCESS) {
            Config.bSuccess = true;
            m_AccountsDBAide.GetValue_String(TEXT("Version"), Config.cbVersion, CountArray(Config.cbVersion));
            Config.pServerStatus.bServerStatus = (bool)m_AccountsDBAide.GetValue_BYTE(TEXT("ServerStatus"));
            m_AccountsDBAide.GetValue_String(TEXT("StrDescribeString"), Config.pServerStatus.szDescrubeString, CountArray(Config.pServerStatus.szDescrubeString));
            Config.bAdsShow = (bool)m_AccountsDBAide.GetValue_BYTE(TEXT("AdsShow"));
        } else {
            Config.bSuccess = false;
            CDBVarValue DBVarValue;
            m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
            lstrcpyn(Config.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(Config.szDescribeString));
        }

        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOAD_LOGON_CONFIG, dwContextID, &Config, sizeof(Config));
        return true;
    } catch (IDataBaseException * pIException) {
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);
        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_ReceiveAward"));
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);
        return false;
    }
    return false;
}
//加载房卡房记录
bool CDataBaseEngineSink::OnRequestGetPrivateRecord(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize == sizeof(DBR_MB_PrivateRecord));
        if (wDataSize != sizeof(DBR_MB_PrivateRecord)) return false;

        //请求处理
        DBR_MB_PrivateRecord * pCreate = (DBR_MB_PrivateRecord *)pData;
        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwPrivateID"), pCreate->dwPrivateID);
        m_AccountsDBAide.AddParameter(TEXT("@dwRoundCount"), pCreate->dwRoundCount);

        //输出参数
        //  TCHAR szDescribeString[128] = TEXT("");
        //  m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

        //变量定义
        DBO_MB_PrivateRecord PrivateRecord;
        ZeroMemory(&PrivateRecord, sizeof(PrivateRecord));

        PrivateRecord.nFinishCount = pCreate->dwRoundCount;
        PrivateRecord.nRecordNumber = pCreate->dwOrderNumber;
        PrivateRecord.nPrivateID = pCreate->dwPrivateID;

        //结果处理
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_GetPrivateRecord"), true) == DB_SUCCESS) {
            PrivateRecord.bSuccess = true;
            m_AccountsDBAide.GetValue_String(TEXT("RecordData"), PrivateRecord.szRecordData, CountArray(PrivateRecord.szRecordData));
            for (int i = 0; i < 6; i++) {
                CString paraString = TEXT("");
                paraString.Format(TEXT("NickName%d"), i);
                m_AccountsDBAide.GetValue_String(paraString,PrivateRecord.szNickName[i], CountArray(PrivateRecord.szNickName[i]));
            }
            PrivateRecord.nKindID = m_AccountsDBAide.GetValue_DWORD(TEXT("KindID"));
            m_AccountsDBAide.ResetParameter();
            m_AccountsDBAide.AddParameter(TEXT("@dwPrivateID"), pCreate->dwPrivateID);
            if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_GetPrivateInfo"), true) == DB_SUCCESS) {
                PrivateRecord.nRoomID = m_AccountsDBAide.GetValue_DWORD(TEXT("RoomID"));
                PrivateRecord.dNeedCardCount = m_AccountsDBAide.GetValue_DOUBLE(TEXT("NeedCardCount"));
                PrivateRecord.nPlayerCount = m_AccountsDBAide.GetValue_DWORD(TEXT("PlayerCount"));
                PrivateRecord.nRoundCount = m_AccountsDBAide.GetValue_DWORD(TEXT("RoundCount"));
                PrivateRecord.nPlayRule = m_AccountsDBAide.GetValue_DWORD(TEXT("PlayRule"));
                PrivateRecord.nExtraRule = m_AccountsDBAide.GetValue_DWORD(TEXT("ExtraRule"));
                m_AccountsDBAide.GetValue_String(TEXT("OtherConfig"), PrivateRecord.szOtherConfig, CountArray(PrivateRecord.szOtherConfig));
                PrivateRecord.nPrivateID = m_AccountsDBAide.GetValue_DWORD(TEXT("PrivateID"));
                PrivateRecord.nCreateWay = m_AccountsDBAide.GetValue_DWORD(TEXT("CreateWay"));
                PrivateRecord.dwCreatorUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("CreatorUserID"));
                PrivateRecord.nTimes = m_AccountsDBAide.GetValue_DWORD(TEXT("Times"));

                PrivateRecord.bDynamicEnter = (bool)m_AccountsDBAide.GetValue_BYTE(TEXT("DynamicEnter"));
                PrivateRecord.bAllowSameIP = (bool)m_AccountsDBAide.GetValue_BYTE(TEXT("AllowSameIP"));
            } else {
                PrivateRecord.bSuccess = false;
            }

        } else {
            PrivateRecord.bSuccess = false;
        }
        // CDBVarValue DBVarValue;
        // m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        //  CreateResult.lResultCode = m_AccountsDBModule->GetReturnValue();
        // lstrcpyn(CreateResult.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(CreateResult.szDescribeString));
        //发送结果
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_GET_PRIVATE_RECORD, dwContextID, &PrivateRecord,sizeof(PrivateRecord));

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_PrivateRecord * pCheck = (DBR_MB_PrivateRecord *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_GetPrivateRecord-@dwPrivateID:%d"), pCheck->dwPrivateID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return true;
}
//领取救济金
bool CDataBaseEngineSink::OnRequestReceiveAlms(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize == sizeof(DBR_MB_ReceiveAlms));
        if (wDataSize != sizeof(DBR_MB_ReceiveAlms)) return false;

        //请求处理
        DBR_MB_ReceiveAlms * pReceiveAlms = (DBR_MB_ReceiveAlms *)pData;
        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@UserID"), pReceiveAlms->nUserID);
        m_AccountsDBAide.AddParameter(TEXT("@szLogPasswd"), pReceiveAlms->szLogPasswd);

        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

        DBO_MB_ReceiveAlms ReceiveAlms;
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_ReceiveAlms"), true) == DB_SUCCESS) {
            ReceiveAlms.bSuccess = true;
            ReceiveAlms.nFragmentCardCount = (int)m_AccountsDBAide.GetValue_DWORD(TEXT("FragmentCardCount"));
        } else {
            ReceiveAlms.bSuccess = false;
        }
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(ReceiveAlms.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(ReceiveAlms.szDescribeString));

        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_RECEIVE_ALMS, dwContextID, &ReceiveAlms, sizeof(ReceiveAlms));
        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_ReceiveAlms * pCheck = (DBR_MB_ReceiveAlms *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_ReceiveAlms-@nUserID:%d"), pCheck->nUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return false;
}
//绑定推广
bool CDataBaseEngineSink::OnRequestUserBindPromote(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize == sizeof(DBR_MB_UserBindPromote));
        if (wDataSize != sizeof(DBR_MB_UserBindPromote)) return false;

        //请求处理
        DBR_MB_UserBindPromote * pBindPromote = (DBR_MB_UserBindPromote *)pData;
        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwPromoteGameID"), pBindPromote->dwPromoteGameID);
        m_AccountsDBAide.AddParameter(TEXT("@dwGameID"), pBindPromote->dwGameID);

        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

        DBO_MB_UserBindPromote BindPromote;
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_UserBindPromote"), true) == DB_SUCCESS) {
            BindPromote.bSuccess = true;
            BindPromote.nRoomCardCount = (int)m_AccountsDBAide.GetValue_DWORD(TEXT("RoomCardCount"));
        } else {
            BindPromote.bSuccess = false;
        }
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(BindPromote.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(BindPromote.szDescribeString));

        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_USER_BIND_PROMOTE, dwContextID, &BindPromote, sizeof(BindPromote));
        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_UserBindPromote * pCheck = (DBR_MB_UserBindPromote *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_UserBindPromote-@dwGameID:%d"), pCheck->dwGameID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return false;
}
//查询房卡
bool CDataBaseEngineSink::OnRequestPrivateCreate(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize == sizeof(DBR_MB_PrivateCreate));
        if (wDataSize != sizeof(DBR_MB_PrivateCreate)) return false;

        //请求处理
        DBR_MB_PrivateCreate * pCreate = (DBR_MB_PrivateCreate *)pData;
        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pCreate->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"), pCreate->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@dwCreateWay"), pCreate->nCreateWay);
        m_AccountsDBAide.AddParameter(TEXT("@wKindID"), pCreate->wKindID);
        m_AccountsDBAide.AddParameter(TEXT("@dwServerID"), pCreate->dwServerID);
        m_AccountsDBAide.AddParameter(TEXT("@dwPlayerCount"), pCreate->nPlayerCount);
        m_AccountsDBAide.AddParameter(TEXT("@dwRoundCount"), pCreate->nRoundCount);
        m_AccountsDBAide.AddParameter(TEXT("@dwPlayRule"), pCreate->nPlayRule);
        m_AccountsDBAide.AddParameter(TEXT("@dwExtraRule"), pCreate->nExtraRule);
        m_AccountsDBAide.AddParameter(TEXT("@dwTimes"), pCreate->nTimes);
        m_AccountsDBAide.AddParameter(TEXT("@strOtherConfig"), pCreate->szOtherConfig);
        m_AccountsDBAide.AddParameter(TEXT("@bDynamicEnter"), pCreate->bDynamicEnter);
        m_AccountsDBAide.AddParameter(TEXT("@bAllowSameIP"), pCreate->bAllowSameIP);
        m_AccountsDBAide.AddParameter(TEXT("@bAllowRepeat"), pCreate->bAllowRepeat);

        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

        //变量定义
        DBO_MB_PrivateCreateResult CreateResult;
        ZeroMemory(&CreateResult, sizeof(CreateResult));
        //结果处理
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_CreateRoomCard"), true) == DB_SUCCESS) {
            CreateResult.bSuccess = true;
            CreateResult.dwCreatorUserID = pCreate->dwUserID;
            CreateResult.nRoomID = m_AccountsDBAide.GetValue_DWORD(TEXT("RoomID"));
            CreateResult.dRoomCardCount = m_AccountsDBAide.GetValue_DOUBLE(TEXT("RoomCardCount"));
            CreateResult.dRedDiamondCount= m_AccountsDBAide.GetValue_DOUBLE(TEXT("RedDiamondCount"));
            CreateResult.dwServerID = pCreate->dwServerID;
            CreateResult.wKindID = pCreate->wKindID;
            CreateResult.nCreateWay = pCreate->nCreateWay;
            CreateResult.nPlayerCount = pCreate->nPlayerCount;
            CreateResult.nPlayRule = pCreate->nPlayRule;
            CreateResult.nExtraRule = pCreate->nExtraRule;
            CreateResult.nRoundCount = pCreate->nRoundCount;
            CreateResult.nTimes = pCreate->nTimes;
            CreateResult.bDynamicEnter = pCreate->bDynamicEnter;
            CreateResult.bAllowSameIP = pCreate->bAllowSameIP;
            CreateResult.bAllowRepeat = pCreate->bAllowRepeat;
            strcpy(CreateResult.szOtherConfig, pCreate->szOtherConfig);
        } else {
            CreateResult.bSuccess = false;
        }
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        CreateResult.lResultCode = m_AccountsDBModule->GetReturnValue();
        lstrcpyn(CreateResult.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(CreateResult.szDescribeString));
        //发送结果
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_PRIVATE_CREATE_RESULT, dwContextID, &CreateResult, sizeof(CreateResult));

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_PrivateCreate * pCheck = (DBR_MB_PrivateCreate *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_GR_CreateRoomCard-@dwUserID:%d"), pCheck->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return true;
}

//查询房卡
bool CDataBaseEngineSink::OnRequestPrivateJoinCheck(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize == sizeof(DBR_MB_PrivateJoinCheck));
        if (wDataSize != sizeof(DBR_MB_PrivateJoinCheck)) return false;

        //请求处理
        DBR_MB_PrivateJoinCheck * pJoinCheck = (DBR_MB_PrivateJoinCheck *)pData;
        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pJoinCheck->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"), pJoinCheck->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@dwRoomID"), pJoinCheck->nRoomID);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        //变量定义
        DBO_MB_PrivateJoinCheckResult JoinCheckResult;
        ZeroMemory(&JoinCheckResult, sizeof(JoinCheckResult));
        //结果处理
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_CheckRoomCard"), true) == DB_SUCCESS) {
            JoinCheckResult.bSuccess = true;
            JoinCheckResult.dwCreatorUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("CreatorUserID"));
            JoinCheckResult.nRoomID = pJoinCheck->nRoomID;
            JoinCheckResult.nServerID =  m_AccountsDBAide.GetValue_DWORD(TEXT("ServerID"));
            JoinCheckResult.nCreateWay = m_AccountsDBAide.GetValue_DWORD(TEXT("CreateWay"));
        } else {
            JoinCheckResult.bSuccess = false;
        }
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        JoinCheckResult.lResultCode = m_AccountsDBModule->GetReturnValue();
        lstrcpyn(JoinCheckResult.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(JoinCheckResult.szDescribeString));
        //发送结果
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_PRIVATE_JOIN_CHECK_RESULT, dwContextID, &JoinCheckResult, sizeof(JoinCheckResult));

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_PrivateJoinCheck * pCheck = (DBR_MB_PrivateJoinCheck *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_GR_CheckRoomCard-@dwUserID:%d"), pCheck->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return true;
}
//回到房间
bool CDataBaseEngineSink::OnRequestPrivateBackRoom(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize == sizeof(DBR_MB_PrivateBackRoom));
        if (wDataSize != sizeof(DBR_MB_PrivateBackRoom)) return false;

        //请求处理
        DBR_MB_PrivateBackRoom * pBackRoom = (DBR_MB_PrivateBackRoom *)pData;
        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pBackRoom->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"), pBackRoom->szPassword);
        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);
        //变量定义
        DBO_MB_PrivateBackRoomResult BackRoomResult;
        ZeroMemory(&BackRoomResult, sizeof(BackRoomResult));
        //结果处理
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_BackRoom"), true) == DB_SUCCESS) {
            BackRoomResult.bSuccess = true;
            BackRoomResult.nServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("ServerID"));
        } else {
            BackRoomResult.bSuccess = false;
        }
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        lstrcpyn(BackRoomResult.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(BackRoomResult.szDescribeString));
        //发送结果
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_PRIVATE_BACK_ROOM_RESULT, dwContextID, &BackRoomResult, sizeof(BackRoomResult));

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        DBR_MB_PrivateBackRoom * pBackRoom = (DBR_MB_PrivateBackRoom *)pData;

        //输出详细信息
        TCHAR szBuffer[1024] = TEXT("");
        _sntprintf(szBuffer, CountArray(szBuffer), TEXT("GSP_MB_BackRoom-@dwUserID:%d"), pBackRoom->dwUserID);
        CTraceService::TraceString(szBuffer, TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

        return false;
    }

    return true;
}
//帐号注册
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_MB_RegisterAccounts));
        if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

        //请求处理
        DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

        //执行判断
        tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
        if (pBindParameter->dwSocketID!=dwContextID) return true;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
        // m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
        //  m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pRegisterAccounts->szLogonPass);
        //  m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
        // m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
        // m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        // m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
        // m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_SimpleRegisterAccounts"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

        return false;
    }

    return true;
}
//帐号注册
bool CDataBaseEngineSink::OnMobileLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize == sizeof(DBR_MB_LogonVisitor));
        if (wDataSize != sizeof(DBR_MB_LogonVisitor)) return false;

        //请求处理
        DBR_MB_LogonVisitor * pLogonVisitor = (DBR_MB_LogonVisitor *)pData;

        //执行判断
        tagBindParameter * pBindParameter = (tagBindParameter *)pLogonVisitor->pBindParameter;
        if (pBindParameter->dwSocketID != dwContextID) return true;

        //转化地址
        TCHAR szClientAddr[16] = TEXT("");
        BYTE * pClientAddr = (BYTE *)&pLogonVisitor->dwClientAddr;
        _sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);
        //TCHAR szPassword[LEN_MD5]=TEXT("");
        //CreateRandPassword(szPassword);
        //CTraceService::TraceStringEx(TraceLevel_Info, TEXT("rand password:%s"), szPassword);
        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@strAccounts"), pLogonVisitor->szAccounts);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"), pLogonVisitor->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

        //输出参数
        TCHAR szDescribeString[128] = TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

        //执行查询
        LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_LogonVisitor"), true);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        //错误处理
        OnLogonDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"), true);

        return false;
    }

    return true;
}
//微信登录
bool CDataBaseEngineSink::OnMobileLogonWX(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize == sizeof(DBR_MB_LogonWX));
        if (wDataSize != sizeof(DBR_MB_LogonWX)) return false;

        //请求处理
        DBR_MB_LogonWX * pLogonWX = (DBR_MB_LogonWX *)pData;

        //执行判断
        tagBindParameter * pBindParameter = (tagBindParameter *)pLogonWX->pBindParameter;
        if (pBindParameter->dwSocketID != dwContextID) return true;

        DBO_MB_LogonWX logonWX;
        strcpy(logonWX.szHeadImgUrl, pLogonWX->szHeadImgUrl);
        strcpy(logonWX.szRefreshToken, pLogonWX->szRefreshToken);
		strcpy(logonWX.szUnionID, pLogonWX->szUnionID);
		strcpy(logonWX.szOpenID, pLogonWX->szOpenID);
        lstrcpyn(logonWX.szNickName, pLogonWX->szNickName, CountArray(pLogonWX->szNickName));
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_WXLOGON, dwContextID, &logonWX, sizeof(logonWX));

        ////转化地址
        //TCHAR szClientAddr[16] = TEXT("");
        //BYTE * pClientAddr = (BYTE *)&pLogonWX->dwClientAddr;
        //_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);
        //TCHAR szPassword[LEN_MD5];
        //CreateRandPassword(szPassword);
        ////构造参数
        //m_AccountsDBAide.ResetParameter();
        //m_AccountsDBAide.AddParameter(TEXT("@strOpenID"), pLogonWX->szOpenID);
        //m_AccountsDBAide.AddParameter(TEXT("@strNickName"), pLogonWX->szNickName);
        //m_AccountsDBAide.AddParameter(TEXT("@cbSex"), pLogonWX->nSex);
        //m_AccountsDBAide.AddParameter(TEXT("@strProvince"), pLogonWX->szProvince);
        //m_AccountsDBAide.AddParameter(TEXT("@strCity"), pLogonWX->szCity);
        //m_AccountsDBAide.AddParameter(TEXT("@strCountry"), pLogonWX->szCountry);
        //m_AccountsDBAide.AddParameter(TEXT("@strHeadImgUrl"), pLogonWX->szHeadImgUrl);
        //m_AccountsDBAide.AddParameter(TEXT("@strPrivilege"), pLogonWX->szPrivilege);
        //m_AccountsDBAide.AddParameter(TEXT("@strUnionID"), pLogonWX->szUnionID);

        //m_AccountsDBAide.AddParameter(TEXT("@strPassword"), szPassword);
        //m_AccountsDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

        //m_AccountsDBAide.AddParameter(TEXT("@strAccessToken"), pLogonWX->szAccessToken);
        //m_AccountsDBAide.AddParameter(TEXT("@strRefreshToken"), pLogonWX->szRefreshToken);
        //m_AccountsDBAide.AddParameter(TEXT("@wExpiresIn"), pLogonWX->nExpiresIn);

        ////输出参数
        //TCHAR szDescribeString[128] = TEXT("");
        //m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

        ////执行查询
        //LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_LogonWX"), true);

        ////结果处理
        //CDBVarValue DBVarValue;
        //m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
        //OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

        //错误处理
        OnLogonDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"), true);

        return false;
    }

    return true;
}
//修改机器
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_ModifyMachine));
        if (wDataSize!=sizeof(DBR_GP_ModifyMachine)) return false;

        //请求处理
        DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

        //输出变量
        WCHAR szDescribe[128]=L"";
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

        //绑定操作
        if (pModifyMachine->cbBind==TRUE) {
            m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
        } else {
            m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
        }

        //结果处理
        if (m_AccountsDBAide.GetReturnValue()==DB_SUCCESS) {
            //变量定义
            DBO_GP_OperateSuccess OperateSuccess;
            ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

            //获取信息
            CDBVarValue DBVarValue;
            m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

            //构造变量
            OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
            lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

            //发送结果
            WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
            WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
        } else {
            //变量定义
            DBO_GP_OperateFailure OperateFailure;
            ZeroMemory(&OperateFailure,sizeof(OperateFailure));

            //获取信息
            CDBVarValue DBVarValue;
            m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

            //构造变量
            OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
            lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

            //发送结果
            WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
            WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
        }

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//修改头像
bool CDataBaseEngineSink::OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_ModifySystemFace));
        if (wDataSize!=sizeof(DBR_GP_ModifySystemFace)) return false;

        //请求处理
        DBR_GP_ModifySystemFace * pModifySystemFace=(DBR_GP_ModifySystemFace *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pModifySystemFace->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifySystemFace->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifySystemFace->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pModifySystemFace->wFaceID);

        //机器信息
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifySystemFace->szMachineID);

        //输出变量
        WCHAR szDescribe[128]=L"";
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

        //结果处理
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SystemFaceInsert"),true)==DB_SUCCESS) {
            //变量定义
            DBO_GP_UserFaceInfo UserFaceInfo;
            ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

            //读取信息
            UserFaceInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));

            //发送结果
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
        } else {
            //获取参数
            CDBVarValue DBVarValue;
            m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

            //结果处理
            OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
        }
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//修改头像
bool CDataBaseEngineSink::OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_ModifyCustomFace));
        if (wDataSize!=sizeof(DBR_GP_ModifyCustomFace)) return false;

        //请求处理
        DBR_GP_ModifyCustomFace * pModifyCustomFace=(DBR_GP_ModifyCustomFace *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pModifyCustomFace->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //变量定义
        SAFEARRAYBOUND SafeArrayBound;
        ZeroMemory(&SafeArrayBound,sizeof(SafeArrayBound));

        //设置变量
        SafeArrayBound.lLbound=0L;
        SafeArrayBound.cElements=sizeof(pModifyCustomFace->dwCustomFace);

        //变量定义
        BYTE * pcbCustomFace=(BYTE *)pModifyCustomFace->dwCustomFace;
        SAFEARRAY * pSafeArray=SafeArrayCreate(VT_UI1,1,&SafeArrayBound);

        //创建数组
        for (LONG nIndex=0; nIndex<sizeof(pModifyCustomFace->dwCustomFace); nIndex++) {
            SafeArrayPutElement(pSafeArray,&nIndex,pcbCustomFace+nIndex);
        }

        //设置变量
        VARIANT VarChunk;
        VariantClear(&VarChunk);
        VarChunk.parray=pSafeArray;
        VarChunk.vt=VT_ARRAY|VT_UI1;

        //获取对象
        ASSERT(m_AccountsDBModule.GetInterface()!=NULL);
        IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyCustomFace->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyCustomFace->szPassword);

        //头像信息
        pIDataBase->AddParameter(TEXT("@dwCustomFace"),adLongVarBinary,adParamInput,sizeof(pModifyCustomFace->dwCustomFace),CDBVarValue(VarChunk));

        //机器信息
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyCustomFace->szMachineID);

        //输出变量
        WCHAR szDescribe[128]=L"";
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

        //释放资源
        SafeArrayDestroy(pSafeArray);

        //结果处理
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CustomFaceInsert"),true)==DB_SUCCESS) {
            //变量定义
            DBO_GP_UserFaceInfo UserFaceInfo;
            ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

            //读取信息
            UserFaceInfo.wFaceID=INVALID_WORD;
            UserFaceInfo.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));

            //发送结果
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
        } else {
            //获取参数
            CDBVarValue DBVarValue;
            m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

            //结果处理
            OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
        }
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//修改密码
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
        if (wDataSize!=sizeof(DBR_GP_ModifyLogonPass)) return false;

        //请求处理
        DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyLogonPass->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

        //输出变量
        WCHAR szDescribe[128]=L"";
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyLogonPassword"),false);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//修改密码
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
        if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

        //请求处理
        DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

        //输出变量
        WCHAR szDescribe[128]=L"";
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

        //结果处理
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//修改签名
bool CDataBaseEngineSink::OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_ModifyUnderWrite));
        if (wDataSize!=sizeof(DBR_GP_ModifyUnderWrite)) return false;

        //请求处理
        DBR_GP_ModifyUnderWrite * pModifyUnderWrite=(DBR_GP_ModifyUnderWrite *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pModifyUnderWrite->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyUnderWrite->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyUnderWrite->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyUnderWrite->szUnderWrite);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

        //输出变量
        WCHAR szDescribe[128]=L"";
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUnderWrite"),false);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//修改资料
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_ModifyIndividual));
        if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

        //请求处理
        DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pModifyIndividual->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyIndividual->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pModifyIndividual->cbGender);
        m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pModifyIndividual->szNickName);
        m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyIndividual->szUnderWrite);
        m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pModifyIndividual->szCompellation);
        m_AccountsDBAide.AddParameter(TEXT("@strQQ"),pModifyIndividual->szQQ);
        m_AccountsDBAide.AddParameter(TEXT("@strEMail"),pModifyIndividual->szEMail);
        m_AccountsDBAide.AddParameter(TEXT("@strSeatPhone"),pModifyIndividual->szSeatPhone);
        m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pModifyIndividual->szMobilePhone);
        m_AccountsDBAide.AddParameter(TEXT("@strDwellingPlace"),pModifyIndividual->szDwellingPlace);
        m_AccountsDBAide.AddParameter(TEXT("@strUserNote"),pModifyIndividual->szUserNote);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

        //输出变量
        WCHAR szDescribe[128]=L"";
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

        //执行查询
        LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserIndividual"),false);

        //结果处理
        CDBVarValue DBVarValue;
        m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//存入游戏币
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_UserSaveScore));
        if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

        //请求处理
        DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
        m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
        m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
        m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//提取游戏币
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_UserTakeScore));
        if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

        //请求处理
        DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
        m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
        m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
        m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
        m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//转账游戏币
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_UserTransferScore));
        if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

        //请求处理
        DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pUserTransferScore->cbByNickName);
        m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
        m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
        m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szNickName);
        m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
        m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
        m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
        m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //执行查询
        LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

        //结果处理
        CDBVarValue DBVarValue;
        m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
        OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //错误信息
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //错误处理
        OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//查询资料
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_QueryIndividual));
        if (wDataSize!=sizeof(DBR_GP_QueryIndividual)) return false;

        //请求处理
        DBR_GP_QueryIndividual * pQueryIndividual=(DBR_GP_QueryIndividual *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pQueryIndividual->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryIndividual->dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pQueryIndividual->szPassword);
        m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //结果处理
        if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserIndividual"),true)==DB_SUCCESS) {
            //变量定义
            DBO_GP_UserIndividual UserIndividual;
            ZeroMemory(&UserIndividual,sizeof(UserIndividual));

            //用户信息
            UserIndividual.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
            m_AccountsDBAide.GetValue_String(TEXT("UserNote"),UserIndividual.szUserNote,CountArray(UserIndividual.szUserNote));
            m_AccountsDBAide.GetValue_String(TEXT("Compellation"),UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));

            //电话号码
            m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),UserIndividual.szSeatPhone,CountArray(UserIndividual.szSeatPhone));
            m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),UserIndividual.szMobilePhone,CountArray(UserIndividual.szMobilePhone));

            //联系资料
            m_AccountsDBAide.GetValue_String(TEXT("QQ"),UserIndividual.szQQ,CountArray(UserIndividual.szQQ));
            m_AccountsDBAide.GetValue_String(TEXT("EMail"),UserIndividual.szEMail,CountArray(UserIndividual.szEMail));
            m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),UserIndividual.szDwellingPlace,CountArray(UserIndividual.szDwellingPlace));

            //发送结果
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
        } else {
            //获取参数
            CDBVarValue DBVarValue;
            m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

            //错误处理
            OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
        }

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//查询银行
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //效验参数
        ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
        if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

        //请求处理
        DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

        //转化地址
        TCHAR szClientAddr[16]=TEXT("");
        BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
        _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

        //构造参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
        m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pQueryInsureInfo->szPassword);
        m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //结果处理
        if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS) {
            //变量定义
            DBO_GP_UserInsureInfo UserInsureInfo;
            ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

            //银行信息
            UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
            UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
            UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
            UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
            UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
            UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

            //发送结果
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
        } else {
            //获取参数
            CDBVarValue DBVarValue;
            m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

            //错误处理
            OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
        }

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }

    return true;
}

//查询用户
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //效验参数
    ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureUserInfo));
    if (wDataSize!=sizeof(DBR_GP_QueryInsureUserInfo)) return false;

    //请求处理
    DBR_GP_QueryInsureUserInfo * pQueryTransferUserInfo=(DBR_GP_QueryInsureUserInfo *)pData;

    try {
        //构造参数
        m_TreasureDBAide.ResetParameter();
        m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
        m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pQueryTransferUserInfo->szNickName);

        //输出参数
        TCHAR szDescribeString[128]=TEXT("");
        m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

        //结果处理
        if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS) {
            //变量定义
            DBO_GP_UserTransferUserInfo TransferUserInfo;
            ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

            //银行信息
            TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
            m_TreasureDBAide.GetValue_String(TEXT("NickName"), TransferUserInfo.szNickName, CountArray(TransferUserInfo.szNickName));

            //发送结果
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
        } else {
            //获取参数
            CDBVarValue DBVarValue;
            m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

            //错误处理
            OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
        }

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        //结果处理
        OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

        return false;
    }
}

//加载列表
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //变量定义
        WORD wPacketSize=0;
        BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

        //加载类型
        m_PlatformDBAide.ResetParameter();
        m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

        //发送种类
        wPacketSize=0;
        DBO_GP_GameType * pGameType=NULL;
        while (m_PlatformDBModule->IsRecordsetEnd()==false) {
            //发送信息
            if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer)) {
                m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
                wPacketSize=0;
            }

            //读取信息
            pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
            pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
            pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
            pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
            m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

            //设置位移
            wPacketSize+=sizeof(DBO_GP_GameType);

            //移动记录
            m_PlatformDBModule->MoveToNext();
        }
        if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

        //读取类型
        m_PlatformDBAide.ResetParameter();
        m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

        //发送类型
        wPacketSize=0;
        DBO_GP_GameKind * pGameKind=NULL;
        while (m_PlatformDBModule->IsRecordsetEnd()==false) {
            //发送信息
            if ((wPacketSize+sizeof(DBO_GP_GameKind))>sizeof(cbBuffer)) {
                m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
                wPacketSize=0;
            }

            //读取信息
            pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
            pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
            pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
            pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
            pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
            pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
            m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
            m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));
            m_PlatformDBAide.GetValue_String(TEXT("GameRuleUrl"),pGameKind->szGameRuleURL,CountArray(pGameKind->szGameRuleURL));//游戏规则网址
            m_PlatformDBAide.GetValue_String(TEXT("DownLoadUrl"),pGameKind->szDownLoadURL,CountArray(pGameKind->szDownLoadURL));//游戏下载网址

            //设置位移
            wPacketSize+=sizeof(DBO_GP_GameKind);

            //移动记录
            m_PlatformDBModule->MoveToNext();
        }
        if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

        //读取节点
        m_PlatformDBAide.ResetParameter();
        m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

        //发送节点
        wPacketSize=0;
        DBO_GP_GameNode * pGameNode=NULL;
        while (m_PlatformDBModule->IsRecordsetEnd()==false) {
            //发送信息
            if ((wPacketSize+sizeof(DBO_GP_GameNode))>sizeof(cbBuffer)) {
                m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
                wPacketSize=0;
            }

            //读取信息
            pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
            pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
            pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
            pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
            pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
            m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));

            //设置位移
            wPacketSize+=sizeof(DBO_GP_GameNode);

            //移动记录
            m_PlatformDBModule->MoveToNext();
        }
        if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

        //读取定制
        m_PlatformDBAide.ResetParameter();
        m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

        //发送定制
        wPacketSize=0;
        DBO_GP_GamePage * pGamePage=NULL;
        while (m_PlatformDBModule->IsRecordsetEnd()==false) {
            //发送信息
            if ((wPacketSize+sizeof(DBO_GP_GamePage))>sizeof(cbBuffer)) {
                m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
                wPacketSize=0;
            }

            //读取信息
            pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
            pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
            pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
            pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
            pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
            pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
            m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));

            //设置位移
            wPacketSize+=sizeof(DBO_GP_GamePage);

            //移动记录
            m_PlatformDBModule->MoveToNext();
        }
        if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

        //变量定义
        DBO_GP_GameListResult GameListResult;
        ZeroMemory(&GameListResult,sizeof(GameListResult));

        //设置变量
        GameListResult.cbSuccess=TRUE;

        //发送消息
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
        CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

        //变量定义
        DBO_GP_GameListResult GameListResult;
        ZeroMemory(&GameListResult,sizeof(GameListResult));

        //设置变量
        GameListResult.cbSuccess=FALSE;

        //发送消息
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

        return false;
    }

    return true;
}

//在线信息
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    try {
        //变量定义
        DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
        WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

        //效验数据
        ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind))));
        if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

        //机器标识
        TCHAR szMachineID[LEN_MACHINE_ID];
        CWHService::GetMachineID(szMachineID);

        //构造信息
        TCHAR szOnLineCountKind[2048]=TEXT("");
        for (WORD i=0; i<pOnLineCountInfo->wKindCount; i++) {
            INT nLength=lstrlen(szOnLineCountKind);
            _sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,
                       pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
        }

        //构造参数
        m_PlatformDBAide.ResetParameter();
        m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
        m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
        m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
        m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);

        //执行命令
        m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

        return true;
    } catch (IDataBaseException * pIException) {
        //输出错误
        CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

        return false;
    }

    return true;
}
//登录成功
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient) {
    if (bMobileClient==false) {
        if (dwErrorCode==DB_SUCCESS) {
            //变量定义
            DBO_GP_LogonSuccess LogonSuccess;
            ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

            //属性资料
            LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
            LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
            LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
            LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
            LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
            LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
            LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
            m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
            m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

            //用户成绩
            LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
            LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

            //用户资料
            LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
            LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
            m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

            //会员资料
            LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
            m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

            //获取信息
            lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

            //发送结果
            WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
            WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
        } else if(dwErrorCode == DB_NEEDMB) {
            //登录成功
            DBR_GP_ValidateMBCard ValidateMBCard;
            ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

            //读取变量
            ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

            //投递结果
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
        } else {
            //变量定义
            DBO_GP_LogonFailure LogonFailure;
            ZeroMemory(&LogonFailure,sizeof(LogonFailure));

            //构造数据
            LogonFailure.lResultCode=dwErrorCode;
            lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

            //发送结果
            WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
            WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
        }
    } else {
        if (dwErrorCode==DB_SUCCESS) {
            //变量定义
            DBO_MB_LogonSuccess LogonSuccess;
            ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

            //属性资料
            LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
            LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
            LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
            LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
            LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
            LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
            LogonSuccess.lUserScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
            LogonSuccess.dRoomCardCount = m_AccountsDBAide.GetValue_DOUBLE(TEXT("RoomCardCount"));
            LogonSuccess.dRedDiamondCount = m_AccountsDBAide.GetValue_DOUBLE(TEXT("RedDiamondCount"));
            LogonSuccess.dwFragmentCardCount = m_AccountsDBAide.GetValue_DWORD(TEXT("FragmentCardCount"));
            LogonSuccess.dwLockServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("LockServerID"));
            LogonSuccess.bHaveInsurePass = m_AccountsDBAide.GetValue_BYTE(TEXT("HaveInsurePass"));
            LogonSuccess.cbMemberOrder = m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
            LogonSuccess.cbAccountType = m_AccountsDBAide.GetValue_BYTE(TEXT("AccountType"));
            m_AccountsDBAide.GetValue_String(TEXT("NickName"), LogonSuccess.szNickName, CountArray(LogonSuccess.szNickName));
            m_AccountsDBAide.GetValue_String(TEXT("Accounts"), LogonSuccess.szAccounts, CountArray(LogonSuccess.szAccounts));
            m_AccountsDBAide.GetValue_String(TEXT("Password"), LogonSuccess.szPassword, CountArray(LogonSuccess.szPassword));
            m_AccountsDBAide.GetValue_String(TEXT("HeadImgUrl"), LogonSuccess.szHeadImgUrl, CountArray(LogonSuccess.szHeadImgUrl));
            m_AccountsDBAide.GetValue_String(TEXT("RefreshToken"), LogonSuccess.szRefreshToken, CountArray(LogonSuccess.szRefreshToken));

            //获取信息
            lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

            //发送结果
            WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
            WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
        } else {
            //变量定义
            DBO_MB_LogonFailure LogonFailure;
            ZeroMemory(&LogonFailure,sizeof(LogonFailure));

            //构造数据
            LogonFailure.lResultCode=dwErrorCode;
            lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

            //发送结果
            WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
            WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
            m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
        }
    }

    return;
}

//银行结果
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient) {
    if (dwErrorCode==DB_SUCCESS) {
        //变量定义
        DBO_GP_UserInsureSuccess UserInsureSuccess;
        ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

        //构造变量
        UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
        UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
        UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
        UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
        UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationScore"));
        UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationInsure"));
        lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

        //发送结果
        WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
        WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
    } else {
        //变量定义
        DBO_GP_UserInsureFailure UserInsureFailure;
        ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

        //构造变量
        UserInsureFailure.lResultCode=dwErrorCode;
        lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

        //发送结果
        WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
        WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
    }

    return;
}

//操作结果
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient) {
    if (dwErrorCode==DB_SUCCESS) {
        //变量定义
        DBO_GP_OperateSuccess OperateSuccess;
        ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

        //构造变量
        OperateSuccess.lResultCode=dwErrorCode;
        lstrcpyn(OperateSuccess.szDescribeString,pszErrorString,CountArray(OperateSuccess.szDescribeString));

        //发送结果
        WORD wDataSize=CountStringBuffer(OperateSuccess.szDescribeString);
        WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wHeadSize+wDataSize);
    } else {
        //变量定义
        DBO_GP_OperateFailure OperateFailure;
        ZeroMemory(&OperateFailure,sizeof(OperateFailure));

        //构造变量
        OperateFailure.lResultCode=dwErrorCode;
        lstrcpyn(OperateFailure.szDescribeString,pszErrorString,CountArray(OperateFailure.szDescribeString));

        //发送结果
        WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
        WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wHeadSize+wDataSize);
    }

    return;
}
VOID CDataBaseEngineSink::CreateRandPassword(TCHAR* szPassword) {
    srand(time(NULL));
    int i;
    for (i = 0; i<32; i++) {
        switch ((rand() % 3)) {
        case 1:
            szPassword[i] = 'A' + rand() % 26;
            break;
        case 2:
            szPassword[i] = 'a' + rand() % 26;
            break;
        default:
            szPassword[i] = '0' + rand() % 10;
            break;
        }
    }
    szPassword[i] = '\0';
}
//获取月份和天数
WORD CDataBaseEngineSink::GetSignInFreshDay(SYSTEMTIME& sys,int flag) {
    CTime tFreshTime = CTime(sys.wYear, sys.wMonth, sys.wDay, 5, 0, 0, 0);
    if (sys.wHour < 5) {
        CTimeSpan span(1, 0, 0, 0);
        tFreshTime -= span;
    }
    WORD days = 0;
    days = GetDaysInMonth(tFreshTime.GetYear(), tFreshTime.GetMonth());
    switch (flag) {
    case 1:
        return tFreshTime.GetDay();
    case 2:
        return tFreshTime.GetMonth();
    case 3:
        return days;
    default:
        break;
    }
    return 0;
}
WORD CDataBaseEngineSink::GetDaysInMonth(WORD year, WORD month) {
    WORD d;
    WORD day[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (2 == month) {
        d = (((0 == year % 4) && (0 != year % 100) || (0 == year % 400)) ? 29 : 28);
    } else {
        d = day[month - 1];

    }
    return d;
}
//获取碎钻数量
WORD CDataBaseEngineSink::getFragmentCardCount(int days) {
    WORD FragmentCardCount = 0;
    if (days <= 7) {
        switch (days) {
        case 1: {
            FragmentCardCount = 2;
            break;
        }
        case 2: {
            FragmentCardCount = 4;
            break;
        }
        case 3: {
            FragmentCardCount = 6;
            break;
        }
        case 4: {
            FragmentCardCount = 8;
            break;
        }
        case 5: {
            FragmentCardCount = 12;
            break;
        }
        case 6: {
            FragmentCardCount = 16;
            break;
        }
        case 7: {
            FragmentCardCount = 20;
            break;
        }
        default:
            break;
        }
    } else if (days >= 507) {
        FragmentCardCount = 520;
    } else {
        int tmp = days;
        if (tmp % 10 < 7) {
            tmp = tmp / 10 - 1;
        } else {
            tmp = tmp / 10;
        }
        FragmentCardCount = tmp * 10 + 20;
    }
    return FragmentCardCount;
}
//////////////////////////////////////////////////////////////////////////////////
