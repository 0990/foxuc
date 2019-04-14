#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink {
    //友元定义
    friend class CServiceUnits;

    //用户数据库
  protected:
    CDataBaseAide					m_AccountsDBAide;					//用户数据库
    CDataBaseHelper					m_AccountsDBModule;					//用户数据库

    //游戏币数据库
  protected:
    CDataBaseAide					m_TreasureDBAide;					//游戏币数据库
    CDataBaseHelper					m_TreasureDBModule;					//游戏币数据库

    //平台数据库
  protected:
    CDataBaseAide					m_PlatformDBAide;					//平台数据库
    CDataBaseHelper					m_PlatformDBModule;					//平台数据库

    //组件变量
  protected:
    CInitParameter *				m_pInitParameter;					//配置参数
    IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件
    //函数定义
  public:
    //构造函数
    CDataBaseEngineSink();
    //析构函数
    virtual ~CDataBaseEngineSink();

    //基础接口
  public:
    //释放对象
    virtual VOID Release() {
        return;
    }
    //接口查询
    virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

    //系统事件
  public:
    //启动事件
    virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
    //停止事件
    virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

    //内核事件
  public:
    //时间事件
    virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
    //控制事件
    virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
    //请求事件
    virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

    //登录服务
  protected:
    //I D 登录
    bool OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //帐号登录
    bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //帐号注册
    bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

    //手机登录
  protected:
    //I D 登录
    bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //帐号登录
    bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //帐号注册
    bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnMobileLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnMobileLogonWX(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //账号服务
  protected:
    //修改机器
    bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //修改密码
    bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //修改密码
    bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //修改签名
    bool OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //修改头像
    bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //修改头像
    bool OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //修改资料
    bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

    //银行服务
  protected:
    //存入游戏币
    bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //提取游戏币
    bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //转账游戏币
    bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //救济金服务
    bool OnRequestAlmsInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnRequestRecordInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnRequestExchangeRoomCard(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //检测房卡数
    bool OnRequestPrivateCreate(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnRequestPrivateJoinCheck(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//回到房间
	bool OnRequestPrivateBackRoom(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //赠送房卡
	bool OnRequestTransferRoomCard(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置银行密码
	bool OnSetInsurePassword(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//碎钻兑换房卡
	bool OnFragmentExchangRate(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//搜索昵称
	bool OnSeekNickname(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//搜索财富
	bool OnSeekUserTreaure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//大厅任务
	bool OnTaskComplete(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//加载玩家任务
	bool OnLoadUserTask(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//领取奖励
	bool OnReceiveAward(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //加载任务信息
	bool OnLoadTaskInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//签到记录
	bool OnLoadSignInRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//加载系统消息
	bool OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//加载配置
	bool OnRequestLoadLogonConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//加载房卡房记录
	bool OnRequestGetPrivateRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//领取奖励金
	bool OnRequestReceiveAlms(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//绑定推广
	bool OnRequestUserBindPromote(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//信息查询
  protected:
    //查询资料
    bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //查询银行
    bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //查询用户
    bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

    //系统功能
  protected:
    //加载列表
    bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //在线信息
    bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

    //结果处理
  protected:
    //登录结果
    VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
    //银行结果
    VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
    //操作结果
    VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);

    VOID CreateRandPassword(TCHAR* szPassword);
	//获取签到碎钻数量
	WORD GetDaysInMonth(WORD year, WORD month);
	WORD getFragmentCardCount(int days);
	WORD GetSignInFreshDay(SYSTEMTIME& sys,int flag);
};

//////////////////////////////////////////////////////////////////////////////////

#endif