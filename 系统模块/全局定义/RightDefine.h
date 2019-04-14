#ifndef RIGHT_DEFINE_HEAD_FILE
#define RIGHT_DEFINE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//用户权限
#define UR_CANNOT_PLAY					0x00000001L				//不能进行游戏
#define UR_CANNOT_LOOKON				0x00000002L				//不能旁观游戏
#define UR_CANNOT_WISPER				0x00000004L				//不能发送私聊
#define UR_CANNOT_ROOM_CHAT				0x00000008L				//不能大厅聊天
#define UR_CANNOT_GAME_CHAT				0x00000010L				//不能游戏聊天
#define UR_CANNOT_BUGLE					0x00000020L				//不能发送喇叭

//会员权限
#define UR_GAME_DOUBLE_SCORE			0x00000100L				//游戏双倍积分
#define UR_GAME_KICK_OUT_USER			0x00000200L				//游戏踢出用户
#define UR_GAME_ENTER_VIP_ROOM          0x00000400L             //进入VIP房间 

//用户身份
#define UR_GAME_MATCH_USER				0x10000000L				//游戏比赛用户
#define UR_GAME_CHEAT_USER				0x20000000L				//游戏作弊用户

//////////////////////////////////////////////////////////////////////////////////

//普通管理
#define UR_CAN_LIMIT_PLAY				0x00000001L				//允许禁止游戏
#define UR_CAN_LIMIT_LOOKON				0x00000002L				//允许禁止旁观
#define UR_CAN_LIMIT_WISPER				0x00000004L				//允许禁止私聊
#define UR_CAN_LIMIT_ROOM_CHAT			0x00000008L				//允许禁止聊天
#define UR_CAN_LIMIT_GAME_CHAT			0x00000010L				//允许禁止聊天

//用户管理
#define UR_CAN_KILL_USER				0x00000100L				//允许踢出用户
#define UR_CAN_SEE_USER_IP				0x00000200L				//允许查看地址
#define UR_CAN_DISMISS_GAME				0x00000400L				//允许解散游戏
#define UR_CAN_LIMIT_USER_CHAT			0x00000800L				//允许禁止玩家聊天

//高级管理
#define UR_CAN_CONFINE_IP				0x00001000L				//允许禁止地址
#define UR_CAN_CONFINE_MAC				0x00002000L				//允许禁止机器
#define UR_CAN_SEND_WARNING				0x00004000L				//允许发送警告
#define UR_CAN_MODIFY_SCORE				0x00008000L				//允许修改积分
#define UR_CAN_FORBID_ACCOUNTS			0x00010000L				//允许封锁帐号

//绑定管理
#define UR_CAN_BIND_GAME				0x00100000L				//允许游戏绑定
#define UR_CAN_BIND_GLOBAL				0x00200000L				//允许全局绑定

//配置管理
#define UR_CAN_ISSUE_MESSAGE			0x01000000L				//允许发布消息
#define UR_CAN_MANAGER_SERVER			0x02000000L				//允许管理房间
#define UR_CAN_MANAGER_OPTION			0x04000000L				//允许管理配置
#define UR_CAN_MANAGER_ANDROID			0x08000000L				//允许管理机器

//////////////////////////////////////////////////////////////////////////////////

//用户权限
class CUserRight
{
	//玩家权限
public:
	//游戏权限
	static bool CanPlay(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_PLAY)==0; }
	//旁观权限
	static bool CanLookon(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_LOOKON)==0; }
	//私聊权限
	static bool CanWisper(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_WISPER)==0; }
	//大厅聊天
	static bool CanRoomChat(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_ROOM_CHAT)==0; }
	//游戏聊天
	static bool CanGameChat(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_GAME_CHAT)==0; }
	//进入VIP房
	static bool CanEnterVipRoom(DWORD dwUserRight) { return (dwUserRight&UR_GAME_ENTER_VIP_ROOM)==0; }

	//会员权限
public:
	//双倍积分
	static bool CanDoubleScore(DWORD dwUserRight) { return (dwUserRight&UR_GAME_DOUBLE_SCORE)!=0; }
	//踢出用户
	static bool CanKillOutUser(DWORD dwUserRight) { return (dwUserRight&UR_GAME_KICK_OUT_USER)!=0; }

	//特殊权限
public:
	//比赛用户
	static bool IsGameMatchUser(DWORD dwUserRight) { return (dwUserRight&UR_GAME_MATCH_USER)!=0; }
	//作弊用户
	static bool IsGameCheatUser(DWORD dwUserRight) { return (dwUserRight&UR_GAME_CHEAT_USER)!=0; }
};

//////////////////////////////////////////////////////////////////////////////////

//管理权限
class CMasterRight
{
	//普通管理
public:
	//禁止游戏
	static bool CanLimitPlay(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_PLAY)!=0); }
	//禁止旁观
	static bool CanLimitLookon(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_LOOKON)!=0); }
	//禁止私聊
	static bool CanLimitWisper(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_WISPER)!=0); }
	//禁止聊天
	static bool CanLimitRoomChat(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_ROOM_CHAT)!=0); }
	//禁止聊天
	static bool CanLimitGameChat(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_GAME_CHAT)!=0); }

	//用户管理
public:
	//踢出用户
	static bool CanKillUser(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_KILL_USER)!=0); }
	//查看地址
	static bool CanSeeUserIP(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_SEE_USER_IP)!=0); }
	//解散游戏
	static bool CanDismissGame(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_DISMISS_GAME)!=0); }
	//禁止玩家聊天
	static bool CanLimitUserChat(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_USER_CHAT)!=0); }

	//高级管理
public:
	//禁止地址
	static bool CanConfineIP(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_CONFINE_IP)!=0); }
	//禁止机器
	static bool CanConfineMac(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_CONFINE_MAC)!=0); }
	//发送警告
	static bool CanSendWarning(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_SEND_WARNING)!=0); }
	//修改积分
	static bool CanModifyScore(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_MODIFY_SCORE)!=0); }
	//封锁帐号
	static bool CanForbidAccounts(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_FORBID_ACCOUNTS)!=0); }

	//绑定管理
public:
	//游戏绑定
	static bool CanBindGame(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_BIND_GAME)!=0); }
	//全局绑定
	static bool CanBindGlobal(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_BIND_GLOBAL)!=0); }

	//配置管理
public:
	//发布消息
	static bool CanIssueMessage(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_ISSUE_MESSAGE)!=0); }
	//管理房间
	static bool CanManagerServer(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_MANAGER_SERVER)!=0); }
	//管理机器
	static bool CanManagerAndroid(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_MANAGER_ANDROID)!=0); }
};

//////////////////////////////////////////////////////////////////////////////////

#endif
