
#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE


#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//游戏列表

//游戏类型
struct tagGameType {
    WORD							wJoinID;							//挂接索引
    WORD							wSortID;							//排序索引
    WORD							wTypeID;							//类型索引
    TCHAR							szTypeName[LEN_TYPE];				//种类名字
};

//游戏种类
struct tagGameKind {
    WORD							wTypeID;							//类型索引
    WORD							wJoinID;							//挂接索引
    WORD							wSortID;							//排序索引
    WORD							wKindID;							//类型索引
    WORD							wGameID;							//模块索引
    DWORD							dwOnLineCount;						//在线人数
    DWORD							dwFullCount;						//满员人数
    TCHAR							szKindName[LEN_KIND];				//游戏名字
    TCHAR							szProcessName[LEN_PROCESS];			//进程名字
    TCHAR                           szGameRuleURL[MAX_PATH];            //游戏规则网址　　
    TCHAR                           szDownLoadURL[MAX_PATH];            //游戏下载网址
};

//游戏节点
struct tagGameNode {
    WORD							wKindID;							//名称索引
    WORD							wJoinID;							//挂接索引
    WORD							wSortID;							//排序索引
    WORD							wNodeID;							//节点索引
    TCHAR							szNodeName[LEN_NODE];				//节点名称
};

//定制类型
struct tagGamePage {
    WORD							wPageID;							//页面索引
    WORD							wKindID;							//名称索引
    WORD							wNodeID;							//节点索引
    WORD							wSortID;							//排序索引
    WORD							wOperateType;						//控制类型
    TCHAR							szDisplayName[LEN_PAGE];			//显示名称
};

//游戏房间
struct tagGameServer {
    WORD							wKindID;							//名称索引
    WORD							wNodeID;							//节点索引
    WORD							wSortID;							//排序索引
    WORD							wServerID;							//房间索引
    WORD							wServerPort;						//房间端口
    WORD                            wServerType;                        //服务类型
    DWORD							dwOnLineCount;						//在线人数
    DWORD							dwFullCount;						//满员人数
    bool                            bAcceptNew;                         //是否在接受新服务
    TCHAR							szServerAddr[32];					//房间名称
    TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//视频配置
struct tagAVServerOption {
    WORD							wAVServerPort;						//视频端口
    DWORD							dwAVServerAddr;						//视频地址
};

//在线信息
struct tagOnLineInfoKind {
    WORD							wKindID;							//类型标识
    DWORD							dwOnLineCount;						//在线人数
};

//在线信息
struct tagOnLineInfoServer {
    WORD							wServerID;							//房间标识
    DWORD							dwOnLineCount;						//在线人数
};

//////////////////////////////////////////////////////////////////////////////////
//用户信息

//桌子状态
struct tagTableStatus {
    BYTE							cbTableLock;						//锁定标志
    BYTE							cbPlayStatus;						//游戏标志
};

//用户状态
struct tagUserStatus {
    WORD							wTableID;							//桌子索引
    WORD							wChairID;							//椅子位置
    BYTE							cbUserStatus;						//用户状态
};

//用户属性
struct tagUserAttrib {
    BYTE							cbCompanion;						//用户关系
};

//用户积分
struct tagUserScore {
    //积分信息
    SCORE							lScore;								//用户分数
    SCORE							lGrade;								//用户成绩
    SCORE							lInsure;							//用户银行

    //输赢信息
    DWORD							dwWinCount;							//胜利盘数
    DWORD							dwLostCount;						//失败盘数
    DWORD							dwDrawCount;						//和局盘数
    DWORD							dwFleeCount;						//逃跑盘数

    //全局信息
    DWORD							dwUserMedal;						//用户奖牌
    DWORD							dwExperience;						//用户经验
    LONG							lLoveLiness;						//用户魅力
};

//用户积分
struct tagMobileUserScore {
    //积分信息
    SCORE							lScore;								//用户分数

    //输赢信息
    DWORD							dwWinCount;							//胜利盘数
    DWORD							dwLostCount;						//失败盘数
    DWORD							dwDrawCount;						//和局盘数
    DWORD							dwFleeCount;						//逃跑盘数

    //全局信息
    DWORD							dwExperience;						//用户经验
};


//道具使用
struct tagUsePropertyInfo {
    WORD                            wPropertyCount;                     //道具数目
    WORD                            dwValidNum;						    //有效数字
    DWORD                           dwEffectTime;                       //生效时间
};


//用户道具
struct tagUserProperty {
    WORD                            wPropertyUseMark;                   //道具标示
    tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//使用信息
};

//任务信息
struct tagUserTaskInfoItem {
    WORD							wTaskID;							//任务标识
    WORD							wFinishCount;						//完成次数
    bool                            bFinished;                          //完成状态
    bool                            bAccepted;                          //任务接受状态
};
typedef std::map<int, tagUserTaskInfoItem> Map_Int_UserTaskInfoItem;
//用户任务   数据库交互时使用
struct tagUserTaskInfo {
    BYTE							cbTaskCount;						//任务数目
    time_t							tloadTime;                          //任务加载时间
    tagUserTaskInfoItem				UserTaskInfo[MAX_TASK_COUNT];		//用户任务
};

struct tagUserTaskInfoMapItem {
    void init() {
        tloadTime = 0;
        mapUserTaskInfo.clear();
    }
    time_t							tloadTime;
    Map_Int_UserTaskInfoItem		mapUserTaskInfo;
};

//道具包裹
struct tagPropertyPackage {
    WORD                            wTrumpetCount;                     //小喇叭数
    WORD                            wTyphonCount;                      //大喇叭数
};

//时间信息
struct tagTimeInfo {
    DWORD						dwEnterTableTimer;						//进出桌子时间
    DWORD						dwLeaveTableTimer;						//离开桌子时间
    DWORD						dwStartGameTimer;						//开始游戏时间
    DWORD						dwEndGameTimer;							//离开游戏时间
};

//用户信息
struct tagUserInfo {
    //基本属性
    DWORD							dwUserID;							//用户 I D
    DWORD							dwGameID;							//游戏 I D
    DWORD							dwGroupID;							//社团 I D
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
    TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
    char                            szHeadImgUrl[LEN_HEAD_IMG_URL];     //玩家头像url
    TCHAR                           szLocation[LEN_DWELLING_PLACE];     //玩家定位

    int                             nAccuracy;                           //定位精度
	SCORE                            lLocateTime;                         //定位时间

    BYTE                            cbAccountType;						//账户类型
    //头像信息
    WORD							wFaceID;							//头像索引
    DWORD							dwCustomID;							//自定标识

    //用户资料
    BYTE							cbGender;							//用户性别
    BYTE							cbMemberOrder;						//会员等级
    BYTE							cbMasterOrder;						//管理等级

    //用户状态
    WORD							wTableID;							//桌子索引
    WORD							wChairID;							//椅子索引
    BYTE							cbUserStatus;						//用户状态
    bool                            bForeground;                        //玩家手机在前台标志
    bool                            bDynamicEnter;                      //中途加入标志

    //积分信息
    SCORE							lScore;								//用户分数
    SCORE							lGrade;								//用户成绩
    SCORE							lInsure;							//用户银行

    //游戏信息
    DWORD							dwWinCount;							//胜利盘数
    DWORD							dwLostCount;						//失败盘数
    DWORD							dwDrawCount;						//和局盘数
    DWORD							dwFleeCount;						//逃跑盘数
    DWORD							dwUserMedal;						//用户奖牌
    DWORD							dwExperience;						//用户经验
    LONG							lLoveLiness;						//用户魅力

    //时间信息
    tagTimeInfo						TimerInfo;							//时间信息

    //比赛信息
    BYTE							cbEnlistStatus;						//报名状态

    //扩展标识
    LONG							lExpand;
    DWORD							dwExpand;
};

//用户信息
struct tagUserInfoHead {
    //用户属性
    DWORD							dwGameID;							//游戏 I D
    DWORD							dwUserID;							//用户 I D
    DWORD							dwGroupID;							//社团 I D

    //头像信息
    WORD							wFaceID;							//头像索引
    DWORD							dwCustomID;							//自定标识

    //用户属性
    BYTE							cbGender;							//用户性别
    BYTE							cbMemberOrder;						//会员等级
    BYTE							cbMasterOrder;						//管理等级

    //用户状态
    WORD							wTableID;							//桌子索引
    WORD							wChairID;							//椅子索引
    BYTE							cbUserStatus;						//用户状态

    //积分信息
    SCORE							lScore;								//用户分数
    SCORE							lGrade;								//用户成绩
    SCORE							lInsure;							//用户银行

    //游戏信息
    DWORD							dwWinCount;							//胜利盘数
    DWORD							dwLostCount;						//失败盘数
    DWORD							dwDrawCount;						//和局盘数
    DWORD							dwFleeCount;						//逃跑盘数
    DWORD							dwUserMedal;						//用户奖牌
    DWORD							dwExperience;						//用户经验
    LONG							lLoveLiness;						//用户魅力
};

//头像信息
struct tagCustomFaceInfo {
    DWORD							dwDataSize;							//数据大小
    DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//用户信息
struct tagUserRemoteInfo {
    //用户信息
    DWORD							dwUserID;							//用户标识
    DWORD							dwGameID;							//游戏标识
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

    //等级信息
    BYTE							cbGender;							//用户性别
    BYTE							cbMemberOrder;						//会员等级
    BYTE							cbMasterOrder;						//管理等级

    //位置信息
    WORD							wKindID;							//类型标识
    WORD							wServerID;							//房间标识
    TCHAR							szGameServer[LEN_SERVER];			//房间位置
};

//////////////////////////////////////////////////////////////////////////////////

//广场子项
struct tagGamePlaza {
    WORD							wPlazaID;							//广场标识
    TCHAR							szServerAddr[32];					//服务地址
    TCHAR							szServerName[32];					//服务器名
};

//级别子项
struct tagLevelItem {
    LONG							lLevelScore;						//级别积分
    TCHAR							szLevelName[16];					//级别描述
};

//会员子项
struct tagMemberItem {
    BYTE							cbMemberOrder;						//等级标识
    TCHAR							szMemberName[16];					//等级名字
};

//管理子项
struct tagMasterItem {
    BYTE							cbMasterOrder;						//等级标识
    TCHAR							szMasterName[16];					//等级名字
};

//列表子项
struct tagColumnItem {
    BYTE							cbColumnWidth;						//列表宽度
    BYTE							cbDataDescribe;						//字段类型
    TCHAR							szColumnName[16];					//列表名字
};

//地址信息
struct tagAddressInfo {
    TCHAR							szAddress[32];						//服务地址
};

//数据信息
struct tagDataBaseParameter {
    WORD							wDataBasePort;						//数据库端口
    TCHAR							szDataBaseAddr[32];					//数据库地址
    TCHAR							szDataBaseUser[32];					//数据库用户
    TCHAR							szDataBasePass[32];					//数据库密码
    TCHAR							szDataBaseName[32];					//数据库名字
};

//房间配置
struct tagServerOptionInfo {
    //挂接属性
    WORD							wKindID;							//挂接类型
    WORD							wNodeID;							//挂接节点
    WORD							wSortID;							//排列标识

    //税收配置
    WORD							wRevenueRatio;						//税收比例
    SCORE							lServiceScore;						//服务费用

    //房间配置
    SCORE							lRestrictScore;						//限制积分
    SCORE							lMinTableScore;						//最低积分
    SCORE							lMinEnterScore;						//最低积分
    SCORE							lMaxEnterScore;						//最高积分

    //会员限制
    BYTE							cbMinEnterMember;					//最低会员
    BYTE							cbMaxEnterMember;					//最高会员

    //房间属性
    DWORD							dwServerRule;						//房间规则
    TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//用户信息
struct tagMobileUserInfoHead {
    //用户属性
    DWORD							dwGameID;							//游戏 I D
    DWORD							dwUserID;							//用户 I D

    //头像信息
    WORD							wFaceID;							//头像索引
    DWORD							dwCustomID;							//自定标识

    //用户属性
    BYTE							cbGender;							//用户性别
    BYTE							cbMemberOrder;						//会员等级

    //用户状态
    WORD							wTableID;							//桌子索引
    WORD							wChairID;							//椅子索引
    BYTE							cbUserStatus;						//用户状态

    //积分信息
    SCORE							lScore;								//用户分数

    //游戏信息
    DWORD							dwWinCount;							//胜利盘数
    DWORD							dwLostCount;						//失败盘数
    DWORD							dwDrawCount;						//和局盘数
    DWORD							dwFleeCount;						//逃跑盘数
    DWORD							dwExperience;						//用户经验
};
//////////////////////////////////////////////////////////////////////////////////
//任务信息
struct tagGameTaskInfoItem {
    tagGameTaskInfoItem() {

        wTaskID = 0;
        cbTaskType = 0;
        cbCondition = 0;
        wAmount = 0;
        cbAwardType = 0;
        lAwardCount = 0;
    }
    WORD							wTaskID;							//任务标识
    BYTE							cbTaskType;							//任务类型
    BYTE							cbCondition;						//任务条件
    WORD							wAmount;							//任务满足数量
    BYTE							cbAwardType;						//奖励类型
    LONG							lAwardCount;						//奖励数量
    WORD							wKindID;							//房间类型
};
typedef std::map<int, tagGameTaskInfoItem> Map_Int_GameTaskInfoItem;
typedef Map_Int_GameTaskInfoItem* pMap_Int_GameTaskInfoItem;

//发送任务
struct tagSendTaskInfoItem: public tagGameTaskInfoItem {
    WORD							wFinishCount;						//完成次数
};

//任务配置
struct tagGameTaskInfo {
    BYTE							cbTaskCount;						//任务数目
    tagGameTaskInfoItem				GameTaskInfo[MAX_TASK_COUNT];		//任务描述
};
#pragma pack()

#endif