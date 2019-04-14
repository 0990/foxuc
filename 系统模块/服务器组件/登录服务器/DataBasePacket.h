#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//登录命令
#define	DBR_GP_LOGON_GAMEID			1									//I D 登录
#define	DBR_GP_LOGON_ACCOUNTS		2									//帐号登录
#define DBR_GP_REGISTER_ACCOUNTS	3									//注册帐号

//账号服务
#define DBR_GP_MODIFY_MACHINE		10									//修改机器
#define DBR_GP_MODIFY_LOGON_PASS	11									//修改密码
#define DBR_GP_MODIFY_INSURE_PASS	12									//修改密码
#define DBR_GP_MODIFY_UNDER_WRITE	13									//修改签名
#define DBR_GP_MODIFY_INDIVIDUAL	14									//修改资料

//头像命令
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//修改头像
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//修改头像

//银行命令
#define DBR_GP_USER_SAVE_SCORE		30									//存入游戏币
#define DBR_GP_USER_TAKE_SCORE		31									//提取游戏币
#define DBR_GP_USER_TRANSFER_SCORE	32									//转账游戏币

//查询命令
#define DBR_GP_QUERY_INDIVIDUAL		40									//查询资料
#define DBR_GP_QUERY_INSURE_INFO	41									//查询银行
#define DBR_GP_QUERY_USER_INFO	    42									//查询用户
#define DBR_GP_ALMS_INFO            43                                  //救济金操作
#define DBR_MB_RECORD_INFO          44                                  //救济金操作
#define DBR_MB_EXCHANGE_ROOM_CARD   45                                  //救济金操作
#define DBR_MB_ROOM_CARD_INFO		46									//赠送房卡
#define DBR_MB_INSURE_PASSWORD_INFO 47									//设置银行密码
#define DBR_MB_FRAGMENT_EXCHANGE_RATE	48								//碎钻兑换房卡
#define DBR_MB_SEEK_NICKNAME		49									//搜索昵称
#define DBR_MB_SEEK_USER_TREASURE	60									//搜索玩家财富
#define DBR_MB_TASK_COMPLETED		61									//大厅任务
#define DBR_MB_LOAD_USER_TASK		62									//加载玩家任务
#define DBR_MB_ACCEPT_TASK_AWARD	63									//领取奖励
#define DBR_MB_LOAD_TASK_INFO		64									//加载任务信息
#define DBR_MB_LOAD_SIGN_IN_RECORD	65									//签到记录
#define DBR_MB_LOAD_SYSTEM_MESSAGE	66									//加载系统信息
#define DBR_MB_LOAD_LOGON_CONFIG	67									//加载配置
#define DBR_MB_GET_PRIVATE_RECORD	68									//房卡房记录
#define DBR_MB_RECEIVE_ALMS			69									//领取救济金
#define DBR_MB_USER_BIND_PROMOTE	70									//绑定推广

//系统命令
#define DBR_GP_LOAD_GAME_LIST		50									//加载列表
#define DBR_GP_ONLINE_COUNT_INFO	51									//在线信息

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_GP_LogonGameID {
    //登录信息
    DWORD							dwGameID;							//用户 I D
    TCHAR							szPassword[LEN_MD5];				//登录密码
    BYTE							cbNeeValidateMBCard;				//密保校验

    //附加信息
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

    //连接信息
    LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_GP_LogonAccounts {
    //登录信息
    TCHAR							szPassword[LEN_MD5];				//登录密码
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
    BYTE							cbNeeValidateMBCard;				//密保校验

    //附加信息
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

    //连接信息
    LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_GP_RegisterAccounts {
    //注册信息
    WORD							wFaceID;							//头像标识
    BYTE							cbGender;							//用户性别
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
    TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
    TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
    TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
    TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

    //密码变量
    TCHAR							szLogonPass[LEN_MD5];				//登录密码
    TCHAR							szInsurePass[LEN_MD5];				//银行密码

    //附加信息
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

    //连接信息
    LPVOID							pBindParameter;						//绑定参数
};

//修改机器
struct DBR_GP_ModifyMachine {
    BYTE							cbBind;								//绑定标志
    DWORD							dwUserID;							//用户标识
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改密码
struct DBR_GP_ModifyLogonPass {
    DWORD							dwUserID;							//用户 I D
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
    TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct DBR_GP_ModifyInsurePass {
    DWORD							dwUserID;							//用户 I D
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
    TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改签名
struct DBR_GP_ModifyUnderWrite {
    DWORD							dwUserID;							//用户 I D
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//修改头像
struct DBR_GP_ModifySystemFace {
    //用户信息
    WORD							wFaceID;							//头像标识
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_MD5];				//登录密码

    //机器信息
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct DBR_GP_ModifyCustomFace {
    //用户信息
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_MD5];				//登录密码
    DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息

    //机器信息
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改资料
struct DBR_GP_ModifyIndividual {
    //验证资料
    DWORD							dwUserID;							//用户 I D
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码

    //帐号资料
    BYTE							cbGender;							//用户性别
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

    //用户信息
    TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
    TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

    //电话号码
    TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

    //联系资料
    TCHAR							szQQ[LEN_QQ];						//Q Q 号码
    TCHAR							szEMail[LEN_EMAIL];					//电子邮件
    TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//查询资料
struct DBR_GP_QueryIndividual {
    DWORD							dwUserID;							//用户 I D
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szPassword[LEN_PASSWORD];			//银行密码
};

//存入游戏币
struct DBR_GP_UserSaveScore {
    DWORD							dwUserID;							//用户 I D
    SCORE							lSaveScore;							//存入游戏币
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出游戏币
struct DBR_GP_UserTakeScore {
    DWORD							dwUserID;							//用户 I D
    SCORE							lTakeScore;							//提取游戏币
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szPassword[LEN_PASSWORD];			//银行密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账游戏币
struct DBR_GP_UserTransferScore {
    DWORD							dwUserID;							//用户 I D
    DWORD							dwClientAddr;						//连接地址
    BYTE                            cbByNickName;                       //昵称赠送
    SCORE							lTransferScore;						//转账游戏币
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
    TCHAR							szPassword[LEN_PASSWORD];			//银行密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct DBR_GP_QueryInsureInfo {
    DWORD							dwUserID;							//用户 I D
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//查询用户
struct DBR_GP_QueryInsureUserInfo {
    BYTE                            cbByNickName;                       //昵称赠送
    TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//救济金操作
struct DBR_GP_AlmsInfo {
    DWORD                           dwUserID;                           //用户ID
    bool                            bGetAlms;                           //是否获取救济金
};
struct DBR_MB_RecordInfo {
    DWORD       dwUserID;
    int         nCurrRecordID;
    TCHAR		szPassword[LEN_PASSWORD];			//用户密码
};
struct DBR_MB_ExchangeRoomCard {
    DWORD dwUserID;
    DWORD dwExchangeGameID;
    char szCardNum[LEN_PASSWORD];
};
//赠送房卡
struct DBR_MB_RoomCardInfo {
    DWORD		dwUserID;							//用户ID
    DWORD		dwGameID;							//游戏ID
    DWORD		dwRoomCardCount;					//房卡数量
    DWORD		dwRedDiamondCount;					//红钻数量
    TCHAR		szPassword[LEN_PASSWORD];			//用户密码
};
//设置银行密码
struct DBR_MB_InsurePasswdInfo {
    DWORD		dwUserID;							//用户ID
    TCHAR		szLogPasswd[LEN_PASSWORD];			//用户密码
    TCHAR		szPassword[LEN_PASSWORD];			//银行密码
};
//碎钻兑换房卡
struct DBR_MB_FragmentExchangRate {
    DWORD		dwUserID;							//用户ID
    TCHAR		szLogPasswd[LEN_PASSWORD];			//用户密码
    WORD		wDiamondType;						//兑换类型
};
//搜索昵称
struct DBR_MB_SeekNickname {
    DWORD		dwGameID;							//游戏ID
};
//搜索财富
struct DBR_MB_SeekUserTreasure {
    DWORD		dwUserID;							//用户ID
};
//大厅任务
struct DBR_MB_TaskComplete {
    DWORD		dwUserID;							//用户ID
    DWORD		dwTaskID;							//任务ID
    TCHAR		szPasswd[LEN_PASSWORD];			//用户密码
};
//签到记录
struct DBR_MB_SignInRecord {
    DWORD		dwUserID;							//用户ID
};
//加载玩家任务
struct DBR_MB_LoadUserTask {
    DWORD		dwUserID;							//用户ID
    int			dwKindID;							//游戏类型
};
//领取奖励
struct DBR_MB_AcceptTaskAward {
    DWORD		dwUserID;							//用户ID
    TCHAR		szLogPasswd[LEN_PASSWORD];			//用户密码
    DWORD		dwTaskID;							//任务ID
};
//房卡房记录
struct DBR_MB_PrivateRecord {
    DWORD		dwOrderNumber;							//录像ID
    DWORD		dwPrivateID;
    DWORD		dwRoundCount;
};
//领取救济金
struct DBR_MB_ReceiveAlms {
    int			nUserID;
    TCHAR		szLogPasswd[LEN_PASSWORD];			//用户密码
};
//绑定推广
struct DBR_MB_UserBindPromote {
    DWORD			dwGameID;						//玩家ID
    DWORD			dwPromoteGameID;				//推广员ID
};
//用户资料
struct DBO_GP_UserTransferUserInfo {
    DWORD							dwGameID;							//用户 I D
    TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
};
//系统消息
struct DBR_GR_SystemMessage {
    BYTE                            cbMessageID;                        //消息ID
    BYTE							cbMessageType;						//消息类型
    DWORD							dwTimeRate;						    //时间频率
    __time64_t                      tConcludeTime;                      //结束时间
    TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};
//在线信息
struct DBR_GP_OnLineCountInfo {
    WORD							wKindCount;							//类型数目
    DWORD							dwOnLineCountSum;					//总在线数
    tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//类型在线
};

//////////////////////////////////////////////////////////////////////////////////
//输出信息

//登录结果
#define DBO_GP_LOGON_SUCCESS		100									//登录成功
#define DBO_GP_LOGON_FAILURE		101									//登录失败
#define DBR_GP_VALIDATE_MBCARD		102									//登录失败
#define DBO_GP_WXLOGON              103                                 //微信登录


//操作结果
#define DBO_GP_USER_FACE_INFO		110									//用户头像
#define DBO_GP_USER_INDIVIDUAL		111									//用户资料

//银行命令
#define DBO_GP_USER_INSURE_INFO					120						//银行资料
#define DBO_GP_USER_INSURE_SUCCESS				121						//银行成功
#define DBO_GP_USER_INSURE_FAILURE				122						//银行失败
#define DBO_GP_USER_INSURE_USER_INFO			123						//用户资料
#define DBO_GP_USER_ALMS_SUCCESS				124                     //救济金成功
#define DBO_GP_USER_ALMS_FAILURE				125                     //查询救失败
#define DBO_MB_RECORD_INFO						126                     //查询游戏记录
#define DBO_MB_EXCHANGE_ROOM_CARD				127                     //查询游戏记录
#define DBO_MB_ROOM_CARD_INFO					128						//查询房卡记录
#define DBO_MB_INSURE_PASSWORD_INFO				129						//设置银行密码
#define DBO_MB_FRAGMENT_EXCHANGE_RATE			140						//碎钻兑换房卡
#define DBO_MB_SEEK_NICKNAME					141						//搜索昵称
#define DBO_MB_SEEK_USER_TREASURE				142						//搜索财富
#define DBO_MB_TASK_COMPLETE					143						//大厅任务
#define DBO_MB_LOAD_USER_TASK					144						//加载玩家任务
#define DBO_MB_ACCEPT_TASK_AWARD				145						//领取奖励
#define DBO_MB_LOAD_TASK_INFO					146						//加载任务信息
#define DBO_MB_LOAD_SIGN_IN_RECORD				147						//签到记录
#define	DBO_MB_LOAD_SYSTEM_MESSAGE				148						//加载系统消息
#define DBO_MB_LOAD_LOGON_CONFIG				149						//加载配置
#define DBO_MB_RECEIVE_ALMS						150						//领取救济金
#define DBO_MB_USER_BIND_PROMOTE				151						//绑定推广
//列表结果
#define DBO_GP_GAME_TYPE_ITEM		130									//种类信息
#define DBO_GP_GAME_KIND_ITEM		131									//类型信息
#define DBO_GP_GAME_NODE_ITEM		132									//节点信息
#define DBO_GP_GAME_PAGE_ITEM		133									//定制信息
#define DBO_GP_GAME_LIST_RESULT		134									//加载结果

//服务结果
#define DBO_GP_OPERATE_SUCCESS		500									//操作成功
#define DBO_GP_OPERATE_FAILURE		501									//操作失败

struct DBO_MB_LogonWX {
    TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
    char                            szHeadImgUrl[256];
	char                            szRefreshToken[256];
	char                            szUnionID[256];
	char                            szOpenID[256];
};

//登录成功
struct DBO_GP_LogonSuccess {
    //属性资料
    WORD							wFaceID;							//头像标识
    DWORD							dwUserID;							//用户标识
    DWORD							dwGameID;							//游戏标识
    DWORD							dwGroupID;							//社团标识
    DWORD							dwCustomID;							//自定索引
    DWORD							dwUserMedal;						//用户奖牌
    DWORD							dwExperience;						//经验数值
    DWORD							dwLoveLiness;						//用户魅力
    TCHAR							szPassword[LEN_MD5];				//登录密码
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
    TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

    //用户成绩
    SCORE							lUserScore;							//用户游戏币
    SCORE							lUserInsure;						//用户银行

    //用户资料
    BYTE							cbGender;							//用户性别
    BYTE							cbMoorMachine;						//锁定机器
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

    //会员资料
    BYTE							cbMemberOrder;						//会员等级
    SYSTEMTIME						MemberOverDate;						//到期时间

    //描述信息
    TCHAR							szDescribeString[128];				//描述消息
};

//登录失败
struct DBO_GP_LogonFailure {
    LONG							lResultCode;						//错误代码
    TCHAR							szDescribeString[128];				//描述消息
};

//登录失败
struct DBR_GP_ValidateMBCard {
    UINT							uMBCardID;						//机器序列
};

//头像信息
struct DBO_GP_UserFaceInfo {
    WORD							wFaceID;							//头像标识
    DWORD							dwCustomID;							//自定索引
};

//个人资料
struct DBO_GP_UserIndividual {
    //用户信息
    DWORD							dwUserID;							//用户 I D
    TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
    TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

    //电话号码
    TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

    //联系资料
    TCHAR							szQQ[LEN_QQ];						//Q Q 号码
    TCHAR							szEMail[LEN_EMAIL];					//电子邮件
    TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//银行资料
struct DBO_GP_UserInsureInfo {
    WORD							wRevenueTake;						//税收比例
    WORD							wRevenueTransfer;					//税收比例
    WORD							wServerID;							//房间标识
    SCORE							lUserScore;							//用户游戏币
    SCORE							lUserInsure;						//银行游戏币
    SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct DBO_GP_UserInsureSuccess {
    DWORD							dwUserID;							//用户 I D
    SCORE							lSourceScore;						//原来游戏币
    SCORE							lSourceInsure;						//原来游戏币
    SCORE							lInsureRevenue;						//银行税收
    SCORE							lVariationScore;					//游戏币变化
    SCORE							lVariationInsure;					//银行变化
    TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct  DBO_GP_UserInsureFailure {
    LONG							lResultCode;						//操作代码
    TCHAR							szDescribeString[128];				//描述消息
};

struct DBO_GP_AlmsFailure {
    LONG							lResultCode;						//操作代码
    TCHAR							szDescribeString[128];				//描述消息
};

//救济金反馈
struct DBO_GP_AlmsSuccess {
    DWORD							dwUserID;							//用户ID
    SCORE							lUserScore;							//用户元宝
    SCORE							lUserInsure;						//银行元宝
    INT                             nUseCount;                          //使用次数
    bool							bGetAlms;				            //是否获取救济金
    INT                             nMaxUseCount;                       //每日领取救济次数
    SCORE                           lAlmsScore;                         //每次领取救济金量
    SCORE							lAlmsThreshold;                     //领取救济阈值
    TCHAR							szDescribeString[128];		//描述消息
};

//操作失败
struct DBO_GP_OperateFailure {
    LONG							lResultCode;						//操作代码
    TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct DBO_GP_OperateSuccess {
    LONG							lResultCode;						//操作代码
    TCHAR							szDescribeString[128];				//成功消息
};

//////////////////////////////////////////////////////////////////////////////////

//游戏类型
struct DBO_GP_GameType {
    WORD							wJoinID;							//挂接索引
    WORD							wSortID;							//排序索引
    WORD							wTypeID;							//类型索引
    TCHAR							szTypeName[LEN_TYPE];				//种类名字
};

//游戏种类
struct DBO_GP_GameKind {
    WORD							wTypeID;							//类型索引
    WORD							wJoinID;							//挂接索引
    WORD							wSortID;							//排序索引
    WORD							wKindID;							//类型索引
    WORD							wGameID;							//模块索引
    TCHAR							szKindName[LEN_KIND];				//游戏名字
    TCHAR							szProcessName[LEN_PROCESS];			//进程名字
    TCHAR                           szGameRuleURL[MAX_PATH];            //游戏规则网址
    TCHAR                           szDownLoadURL[MAX_PATH];            //游戏下载网址
};
struct PlayerInfo {
    DWORD   dwUserID;
    WORD    wChairID;
    TCHAR	szNickName[LEN_NICKNAME];				//游戏名字
};

struct DBO_MB_GameRecord {
    int nRecordID;//游戏记录ID
    WORD wKindID;//游戏
    WORD wServerID;//游戏房间
    char szRecordData[MAX_RECORD_BYTES];//记录数据
    __time64_t	tRecordTime;	//游戏记录时间
};

struct DBO_MB_ExchangeRoomCard {
    bool bSuccess;
    DOUBLE dRoomCardCount;
    TCHAR							szDescribeString[128];				//消息
};
//赠送结果
struct DBO_MB_RoomCardInfo {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//消息
    DOUBLE							dRoomCardCount;					//房卡数量
    DOUBLE							dRedDiamondCount;					//红钻数量
};

//银行密码设置结果
struct DBO_MB_InsurePasswdInfo {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//消息
};
//碎钻兑换房卡结果
struct DBO_MB_FragmentExchangRate {
    bool							bSuccess;
    DOUBLE							dRoomCardCount;					//房卡数量
    DWORD							dwFragmentCardCount;				//碎钻数量
    DOUBLE							dRedDiamondCount;					//房卡数量
    TCHAR							szDescribeString[128];				//消息
    WORD							wDiamondType;						//兑换类型
};
//搜索昵称
struct DBO_MB_SeekNickname {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//消息
    TCHAR							szNickName[LEN_NICKNAME];			//游戏名字
};
//搜索财富
struct DBO_MB_SeekUserTreasure {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//消息
    DOUBLE							dRoomCardCount;					//房卡数量
    DOUBLE							dRedDiamondCount;					//红钻数量
    DWORD							dwFragmentCardCount;				//碎钻数量
    SCORE							lUserScore;							//玩家分数
};
//大厅任务
struct DBO_MB_TaskComplete {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//消息
    DWORD							dwDays;								//签到天数
    DWORD							dwNowFragmentCardCount;				//本次签到碎钻数量
    DWORD							dwFragmentCardCount;				//碎钻数量
    DWORD							dwTaskID;							//任务ID
};
struct UserTaskInfo {
    DWORD							dwTaskID;							//任务ID
    DWORD							dwFinishCount;						//完成次数
    bool							bAccept;							//奖励领取标志
    bool							bFinish;							//任务完成标志
    time_t							tLastTime;							//完成时间
};
//玩家任务信息
struct DBO_MB_UserTaskInfo {
    bool							bSuccess;
    DWORD							dwTaskCount;						//任务数量
    UserTaskInfo					TaskInfo[MAX_TASK_COUNT];			//玩家任务
};
struct TaskInfo {
    DWORD							dwTaskID;						//任务ID
    TCHAR							szTaskName[LEN_NICKNAME];		//任务名称
    WORD							wTaskType;						//任务类型
    DWORD							dwCondition;					//完成条件
    DWORD							dwAmount;						//完成次数
    WORD							wAwardType;						//奖励类型
    DWORD							dwAwardCount;					//奖励数量
    DWORD							dwKindID;						//游戏类型
};
//加载任务信息
struct DBO_MB_TaskInfo {
    TaskInfo						taskInfo[MAX_TASK_COUNT];		//任务信息
    DWORD							dwTaskCount;						//任务数量
};
//签到记录
struct tagUserSignInRecordInfo {
    WORD							wFragmentCardCounts;				//碎钻数量
    int								tSignInDate;						//签到时间
};
struct DBO_MB_SignInRecord {
    WORD							wSignInDays;						//签到总天数
    WORD							wFragmentCardCounts[31];			//将要获得的碎钻数量
    tagUserSignInRecordInfo			SignInRecord[31];					//签到记录
    WORD							wDays;								//本月签到天数
    WORD							wToDay;								//当前日期
    WORD							wMonth;								//当前月份
    WORD							wDaysInMonth;						//当月天数
};
//领取奖励
struct DBO_MB_AcceptTaskAward {
    bool							bSuccess;
    DWORD							dwTaskID;						//任务ID
    DWORD							dwAwardCount;					//奖励数量
    SCORE							lScore;						//总分数
    DWORD							dwFragmentCardCount;			//碎钻数量
    TCHAR							szDescribeString[128];			//消息
};
//加载配置
//服务器状态
struct tagServerStatus {
    bool							bServerStatus;						//服务器状态
    TCHAR							szDescrubeString[128];				//消息
};
struct DBO_MB_LoadLogonConfig {
    bool							bSuccess;
    TCHAR							cbVersion[20];					//版本号
    tagServerStatus					pServerStatus;					//服务器状态
    bool							bAdsShow;						//广告弹窗
    TCHAR							szDescribeString[128];			//消息
};
//绑定推广
struct DBO_MB_UserBindPromote {
    int								nRoomCardCount;
    bool							bSuccess;
    TCHAR							szDescribeString[128];			//消息
};
//游戏节点
struct DBO_GP_GameNode {
    WORD							wKindID;							//名称索引
    WORD							wJoinID;							//挂接索引
    WORD							wSortID;							//排序索引
    WORD							wNodeID;							//节点索引
    TCHAR							szNodeName[LEN_NODE];				//节点名称
};

//定制类型
struct DBO_GP_GamePage {
    WORD							wKindID;							//名称索引
    WORD							wNodeID;							//节点索引
    WORD							wSortID;							//排序索引
    WORD							wPageID;							//定制索引
    WORD							wOperateType;						//控制类型
    TCHAR							szDisplayName[LEN_PAGE];			//显示名称
};

//加载结果
struct DBO_GP_GameListResult {
    BYTE							cbSuccess;							//成功标志
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//手机数据包

//登录命令
#define	DBR_MB_LOGON_GAMEID			500									//I D 登录
#define	DBR_MB_LOGON_ACCOUNTS		501									//帐号登录
#define DBR_MB_REGISTER_ACCOUNTS	502									//注册帐号
#define DBR_MB_LOGON_VISITOR		505									//游客登录
#define DBR_MB_LOGON_WX             506                                 //微信登录
#define DBR_MB_PRIVATE_CREATE	    503									//检测房卡是否充足
#define DBR_MB_PRIVATE_JOIN_CHECK	504									//检测房卡是否充足
#define DBR_MB_PRIVATE_BACK_ROOM	507									//回到房间
//登录结果
#define DBO_MB_LOGON_SUCCESS		600									//登录成功
#define DBO_MB_LOGON_FAILURE		601									//登录失败

#define DBO_MB_PRIVATE_CREATE_RESULT 603                                //数据库房卡创建结果
#define DBO_MB_PRIVATE_JOIN_CHECK_RESULT	604							//检测房卡是否充足
#define DBO_MB_PRIVATE_BACK_ROOM_RESULT		605							//回到房间结果

#define DBO_MB_GET_PRIVATE_RECORD		606							    //房卡房结果
//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_MB_LogonGameID {
    //登录信息
    DWORD							dwGameID;							//用户 I D
    TCHAR							szPassword[LEN_MD5];				//登录密码

    //附加信息
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

    //连接信息
    LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_MB_LogonAccounts {
    //登录信息
    TCHAR							szPassword[LEN_MD5];				//登录密码
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

    //附加信息
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

    //连接信息
    LPVOID							pBindParameter;						//绑定参数
};
//游客登录
struct DBR_MB_LogonVisitor {
    //bool                            bCreateNew;
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szPassword[LEN_MD5];				//登录密码
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

    LPVOID							pBindParameter;						//绑定参数
};
struct DBR_MB_LogonWX {
    char szOpenID[128];
    char szUnionID[128];
    TCHAR szNickName[128];
    int  nSex;
    char szProvince[128];
    char szCity[128];
    char szCountry[128];
    char szHeadImgUrl[256];
    char szPrivilege[256];
    char szAccessToken[256];
    char szRefreshToken[256];
    int nExpiresIn;
    DWORD							dwClientAddr;						//连接地址
    LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_MB_RegisterAccounts {
    //注册信息
    WORD							wFaceID;							//头像标识
    BYTE							cbGender;							//用户性别
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
    TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称

    //密码变量
    TCHAR							szLogonPass[LEN_MD5];				//登录密码
    TCHAR							szInsurePass[LEN_MD5];				//银行密码

    //附加信息
    DWORD							dwClientAddr;						//连接地址
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

    //连接信息
    LPVOID							pBindParameter;						//绑定参数
};

struct DBR_MB_PrivateCreate {
    int   nCreateWay;  //0代表房主开房，1代表AA，2代表非房主开房
    DWORD dwUserID;
    TCHAR szPassword[LEN_MD5];				//登录密码
    DWORD dwServerID;
    WORD  wKindID;
    int	  nPlayerCount;
    int   nRoundCount;
    int   nPlayRule;
    int   nExtraRule;
    int   nTimes;							//倍率
    bool  bDynamicEnter;
    bool  bAllowSameIP;
    bool  bAllowRepeat;
    char  szOtherConfig[256];
};
struct DBR_MB_PrivateJoinCheck {
    DWORD dwUserID;
    TCHAR szPassword[LEN_MD5];				//登录密码
    int nRoomID;
};
struct DBO_MB_PrivateJoinCheckResult {
    bool bSuccess;
    DWORD dwCreatorUserID;
    int nRoomID;
    int nServerID;
    int nCreateWay;
    LONG  lResultCode;
    TCHAR szDescribeString[128];										//描述消息
};

struct DBO_MB_PrivateCreateResult {
    bool  bSuccess;
    LONG  lResultCode;													//错误代码
    TCHAR szDescribeString[128];										//描述消息
    DWORD dwCreatorUserID;
    int nRoomID;
    int nCreateWay;
    DWORD dwServerID;
    WORD  wKindID;
    DOUBLE	dNeedCardCount;
    DOUBLE	dRoomCardCount;
    DOUBLE	dRedDiamondCount;
    int	  nPlayerCount;
    int   nRoundCount;
    int   nPlayRule;
    int   nExtraRule;
    int   nTimes;							//倍率
    bool  bDynamicEnter;
    bool  bAllowSameIP;
    bool  bAllowRepeat;
    char  szOtherConfig[256];

};

struct DBO_MB_PrivateRecord {
    bool  bSuccess;
    LONG  lResultCode;
    char  szRecordData[MAX_RECORD_BYTES];
    TCHAR szNickName[6][128];
    //错误代码
    TCHAR szDescribeString[128];										//描述消息
    DWORD dwCreatorUserID;
    int nPrivateID;
    int nRoomID;
    int nCreateWay;
    DWORD dwServerID;
    DOUBLE	dNeedCardCount;
    int	  nPlayerCount;
    int   nRoundCount;
    int   nPlayRule;
    int   nExtraRule;
    int   nTimes;							//倍率
    bool  bDynamicEnter;
    bool  bAllowSameIP;
    char  szOtherConfig[256];

    int nFinishCount;
    int nKindID;
    int nRecordNumber;
};
//领取救济金
struct DBO_MB_ReceiveAlms {
    bool  bSuccess;
    int	  nFragmentCardCount;
    TCHAR szDescribeString[128];										//描述消息
};
//回到房间
struct DBR_MB_PrivateBackRoom {
    DWORD dwUserID;
    TCHAR szPassword[LEN_MD5];				//登录密码
};
//回到房间
struct DBO_MB_PrivateBackRoomResult {
    bool  bSuccess;
    TCHAR szDescribeString[128];										//描述消息
    int nServerID;
};
//登录成功
struct DBO_MB_LogonSuccess {
    //用户属性
    WORD							wFaceID;							//头像标识
    BYTE							cbGender;							//用户性别
    DWORD							dwUserID;							//用户 I D
    DWORD							dwGameID;							//游戏 I D
    DWORD							dwExperience;						//经验数值
    DWORD							dwLoveLiness;						//用户魅力
    SCORE							lUserScore;
    TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
    DOUBLE                          dRoomCardCount;                    //房卡数量
    DOUBLE                          dRedDiamondCount;						//红钻数量
    DWORD                           dwFragmentCardCount;                //碎片数量
    DWORD                           dwLockServerID;
    TCHAR							szPassword[LEN_MD5];				//登录密码
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
    bool                            bHaveInsurePass;
    BYTE							cbMemberOrder;						//会员等级
    bool							bReceiveAlms;

    char                            szHeadImgUrl[256];
    char                            szRefreshToken[256];
    BYTE                            cbAccountType;
    //描述信息
    TCHAR							szDescribeString[128];				//描述消息
};


//登录失败
struct DBO_MB_LogonFailure {
    LONG							lResultCode;						//错误代码
    TCHAR							szDescribeString[128];				//描述消息
};

//////////////////////////////////////////////////////////////////////////////////

#endif