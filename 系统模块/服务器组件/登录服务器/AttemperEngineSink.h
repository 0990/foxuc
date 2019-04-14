#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "DataBasePacket.h"
#include "list"
#include "cpprest/http_listener.h"
//////////////////////////////////////////////////////////////////////////////////
//数据定义

//连接类型
#define CLIENT_KIND_FALSH			1									//网页类型
#define CLIENT_KIND_MOBILE			2									//手机类型
#define CLIENT_KIND_COMPUTER		3									//电脑类型


#define ACCOUNT_TYPE_VISITOR        1     //游客
#define ACCOUNT_TYPE_WX             2     //微信玩家
#define ACCOUNT_TYPE_ROBOT          3     //机器人
//绑定参数
struct tagBindParameter {
    //网络参数
    DWORD							dwSocketID;							//网络标识
    DWORD							dwClientAddr;						//连接地址
    DWORD							dwActiveTime;						//激活时间

    SCORE                           lUserID;
};

struct EntityInfo {
    int nAccountType;
    SCORE lUserID;
    int nEntityID;
    std::string name;
    std::string szHeadImgUrl;
    float x;
    float y;
    float rotation;
    int lastProcessedInput;
    int nBulletSequenceNumber;
    int hp;
    int score;
	int killCount;
    int nGameRoundSequenceNumber;
    DWORD dwSocketID;
    bool bDead;
  //  bool bGhostMode;
  //  bool bGhostDead;
    DWORD dwLastShootTime;
    DWORD dwCreateTime;
	bool bNew;
	//int nRank;
};

struct WXInfo {
    std::string szName;
    std::string szHeadImgUrl;
    SCORE lUserID;
};

struct UserInfo {
	int nAccountType;
	std::string name; 
	std::string szHeadImgUrl;
	std::string openID;
};

struct SortInfo {
	//SCORE lUserID;
    int nEntityID;
    int nScore;
	int nKillCount;
};

struct BulletInfo {
    int bulletID;
    int damage;
    float initCenterX;
    float initCenterY;
    float x;
    float y;
    float rotation;
    SCORE lCreatorUserID;
    int  nCreatorEntityID;
    long lCreateTime;
    bool bActive;
};

struct Position {
    int x;
    int y;
    Position(int x1, int y1) {
        x = x1;
        y = y1;
    }
};

//////////////////////////////////////////////////////////////////////////////////

//调度钩子
class CAttemperEngineSink : public IAttemperEngineSink {
    //友元定义
    friend class CServiceUnits;

    //变量定义
  protected:
    CInitParameter *				m_pInitParameter;					//配置参数
    tagBindParameter *				m_pBindParameter;					//辅助数组

    int                             m_nBulletSequenceNumber;
    int                             m_nEntitySequenceNumber;
    int                             m_nRoundSequenceNumber;
    int                             m_nGameStartTime;
	int                             m_nRobotCount;
    std::map<SCORE, EntityInfo>     m_EntityMap;
    std::list<BulletInfo>           m_BulletList;
    std::map<std::string, WXInfo>   m_WXInfoMap;
	std::map<SCORE, UserInfo>       m_UserInfoMap;

	Json::FastWriter                m_FastWriter;
    //组件接口
  protected:
    ITimerEngine *					m_pITimerEngine;					//时间引擎
    IDataBaseEngine *				m_pIDataBaseEngine;					//数据引擎
    ITCPNetworkEngine *				m_pITCPNetworkEngine;				//网络引擎
    ITCPSocketService *				m_pITCPSocketService;				//协调服务

    //函数定义
  public:
    //构造函数
    CAttemperEngineSink();
    //析构函数
    virtual ~CAttemperEngineSink();

    //基础接口
  public:
    //释放对象
    virtual VOID Release() {
        return;
    }
    //接口查询
    virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

    //异步接口
  public:
    //启动事件
    virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
    //停止事件
    virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

    //事件接口
  public:
    //控制事件
    virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
    //自定事件
    virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

    //内核事件
  public:
    //时间事件
    virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
    //数据库事件
    virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

    //连接事件
  public:
    //连接事件
    virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
    //关闭事件
    virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
    //读取事件
    virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

    //网络事件
  public:
    //应答事件
    virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
    //关闭事件
    virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
    //读取事件
    virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);
    //进后台事件
    virtual bool OnEventTCPNetworkForeground(bool bForeground, DWORD dwSocketID);
    //连接处理

    //手机事件
  protected:
    //登录处理
    bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

    bool OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

    //手机事件
  protected:

    bool CreateEntity(DWORD dwSocketID,SCORE lUserID,int nAccountType,std::string name="",std::string headImgUrl="");

	bool EntityMove(SCORE lUserID, float fPressTime, float fTargetRotation, int nInputNumber);

	bool EntityJump(SCORE lUserID);

    bool EntityShoot(SCORE lUserID);
    //手机事件
  protected:
    //登录成功
    bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //登录失败
    bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnDBLogonResult(DWORD dwContextID, SCORE lUserID,int nAccountType);
    //
    bool SortRank(Json::Value& rankJson);
    bool SendDeadMessage(SCORE lDeadUserID,SCORE lKillerUserID);
	SCORE CreateNewUserID();
    Position GetRandPosition();
    pplx::task<Json::Value>  RequestAccessToken(DWORD dwSocketID,std::string code);
    pplx::task<Json::Value>  RequestRefreshToken(DWORD dwSocketID,std::string code);

    pplx::task<Json::Value> RequestUserInfo(const std::string& access_token, const std::string& open_id, const std::string& refresh_token, int expires_in);

	pplx::task<Json::Value> HandleRestSdkException(DWORD dwSocketID, pplx::task<Json::Value>& t);

	pplx::task<Json::Value> HandleRestSdkJSException(DWORD dwSocketID, pplx::task<Json::Value>& t);

	pplx::task<Json::Value>  RequestSessionKey(DWORD dwSocketID, std::string code, std::string name,std::string avatarUrl);

};

//////////////////////////////////////////////////////////////////////////////////

#endif