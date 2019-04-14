#ifndef DEFINE_HEAD_FILE
#define DEFINE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//数值定义

//头像大小
#define FACE_CX						48									//头像宽度
#define FACE_CY						48									//头像高度

//长度定义
#define LEN_LESS_ACCOUNTS			6									//最短帐号
#define LEN_LESS_NICKNAME			6									//最短昵称
#define LEN_LESS_PASSWORD			6									//最短密码

//人数定义
#define MAX_CHAIR					100									//最大椅子
#define MAX_TABLE					512									//最大桌子
#define MAX_COLUMN					32									//最大列表
#define MAX_ANDROID					256									//最大机器
#define MAX_PROPERTY				128									//最大道具
#define MAX_WHISPER_USER			16									//最大私聊
#define MAX_TASK_COUNT				100									//最多任务数
#define MAX_RECORD_COUNT			10									//游戏记录最大人数
#define MAX_RECORD_BYTES            20000                               //游戏记录最大字节数
#define MIN_RECORD_BYTES            8000                                //游戏记录最小字节数

//列表定义
#define MAX_KIND					128									//最大类型
#define MAX_SERVER					1024								//最大房间

//参数定义
#define INVALID_CHAIR				0xFFFF								//无效椅子
#define INVALID_TABLE				0xFFFF								//无效桌子

//税收定义
#define REVENUE_BENCHMARK			0L								    //税收起点
#define REVENUE_DENOMINATOR			1000L								//税收分母

//////////////////////////////////////////////////////////////////////////////////
//系统参数

//积分类型
#define SCORE						LONGLONG							//积分类型
#define SCORE_STRING				TEXT("%I64d")						//积分类型

//游戏状态
#define GAME_STATUS_FREE			0									//空闲状态
#define GAME_STATUS_PLAY			100									//游戏状态
#define GAME_STATUS_WAIT			200									//等待状态

//系统参数
#define LEN_USER_CHAT				128									//聊天长度
#define TIME_USER_CHAT				1L									//聊天间隔
#define TRUMPET_MAX_CHAR            128									//喇叭长度

//////////////////////////////////////////////////////////////////////////////////
//索引质数

//列表质数
#define PRIME_TYPE					11L									//种类数目
#define PRIME_KIND					53L									//类型数目
#define PRIME_NODE					101L								//节点数目
#define PRIME_PAGE					53L									//自定数目
#define PRIME_SERVER				1009L								//房间数目

//人数质数
#define PRIME_SERVER_USER			503L								//房间人数
#define PRIME_ANDROID_USER			503L								//机器人数
#define PRIME_PLATFORM_USER			100003L								//平台人数

//////////////////////////////////////////////////////////////////////////////////
//数据长度

//资料数据
#define LEN_MD5						33									//加密密码
#define LEN_USERNOTE				32									//备注长度
#define LEN_ACCOUNTS				32									//帐号长度
#define LEN_NICKNAME				32									//昵称长度
#define LEN_PASSWORD				33									//密码长度
#define LEN_GROUP_NAME				32									//社团名字
#define LEN_UNDER_WRITE				32									//个性签名

//数据长度
#define LEN_QQ						16									//Q Q 号码
#define LEN_EMAIL					33									//电子邮件
#define LEN_USER_NOTE				256									//用户备注
#define LEN_SEAT_PHONE				33									//固定电话
#define LEN_MOBILE_PHONE			12									//移动电话
#define LEN_PASS_PORT_ID			19									//证件号码
#define LEN_COMPELLATION			16									//真实名字
#define LEN_DWELLING_PLACE			128									//联系地址

#define LEN_HEAD_IMG_URL            256

//机器标识
#define LEN_NETWORK_ID				13									//网卡长度
#define LEN_MACHINE_ID				33									//序列长度

//列表数据
#define LEN_TYPE					32									//种类长度
#define LEN_KIND					32									//类型长度
#define LEN_NODE					32									//节点长度
#define LEN_PAGE					32									//定制长度
#define LEN_SERVER					32									//房间长度
#define LEN_PROCESS					32									//进程长度

//////////////////////////////////////////////////////////////////////////////////

//用户关系
#define	CP_NORMAL					0									//未知关系
#define	CP_FRIEND					1									//好友关系
#define	CP_DETEST					2									//厌恶关系
#define CP_SHIELD					3									//屏蔽聊天

//////////////////////////////////////////////////////////////////////////////////

//性别定义
#define GENDER_FEMALE				2									//女性性别
#define GENDER_MALE					1									//男性性别

//////////////////////////////////////////////////////////////////////////////////

//游戏模式
#define GAME_GENRE_GOLD				0x0001								//金币类型
#define GAME_GENRE_SCORE			0x0002								//点值类型
#define GAME_GENRE_MATCH			0x0004								//比赛类型
#define GAME_GENRE_EDUCATE			0x0008								//训练类型
#define GAME_GENRE_PRIVATE          0x0010                              //房卡类型

//分数模式
#define SCORE_GENRE_NORMAL			0x0100								//普通模式
#define SCORE_GENRE_POSITIVE		0x0200								//非负模式

//////////////////////////////////////////////////////////////////////////////////

//用户状态
#define US_NULL						0x00								//没有状态
#define US_FREE						0x01								//站立状态
#define US_SIT						0x02								//坐下状态
#define US_READY					0x03								//同意状态
#define US_LOOKON					0x04								//旁观状态
#define US_PLAYING					0x05								//游戏状态
#define US_OFFLINE					0x06								//断线状态

//旁观状态
#define LOOKON_NULL                 0//非旁观状态
#define LOOKON_ONLY_ME                  1//只能看到我的牌
#define LOOKON_ALL                      2//能看所有玩家的牌

//////////////////////////////////////////////////////////////////////////////////

//比赛状态
#define MS_NULL						0x00								//没有状态
#define MS_SIGNUP					0x01								//报名状态
#define MS_HAVE_SIGNED				0x02								//已报名状态
#define MS_MATCHING					0x03								//比赛状态
#define MS_OUT						0x04								//淘汰状态

//////////////////////////////////////////////////////////////////////////////////

//房间规则
#define SRL_LOOKON					0x00000001							//旁观标志
#define SRL_OFFLINE					0x00000002							//断线标志
#define SRL_SAME_IP					0x00000004							//同网标志

//房间规则
#define SRL_ROOM_CHAT				0x00000100							//聊天标志
#define SRL_GAME_CHAT				0x00000200							//聊天标志
#define SRL_WISPER_CHAT				0x00000400							//私聊标志
#define SRL_HIDE_USER_INFO			0x00000800							//隐藏标志

//////////////////////////////////////////////////////////////////////////////////
//列表数据

//无效属性
#define UD_NULL						0									//无效子项
#define UD_IMAGE					100									//图形子项
#define UD_CUSTOM					200									//自定子项

//基本属性
#define UD_GAME_ID					1									//游戏标识
#define UD_USER_ID					2									//用户标识
#define	UD_NICKNAME					3									//用户昵称

//扩展属性
#define UD_GENDER					10									//用户性别
#define UD_GROUP_NAME				11									//社团名字
#define UD_UNDER_WRITE				12									//个性签名

//状态信息
#define UD_TABLE					20									//游戏桌号
#define UD_CHAIR					21									//椅子号码

//积分信息
#define UD_SCORE					30									//用户分数
#define UD_GRADE					31									//用户成绩
#define UD_USER_MEDAL				32									//用户经验
#define UD_EXPERIENCE				33									//用户经验
#define UD_LOVELINESS				34									//用户魅力
#define UD_WIN_COUNT				35									//胜局盘数
#define UD_LOST_COUNT				36									//输局盘数
#define UD_DRAW_COUNT				37									//和局盘数
#define UD_FLEE_COUNT				38									//逃局盘数
#define UD_PLAY_COUNT				39									//总局盘数

//积分比率
#define UD_WIN_RATE					40									//用户胜率
#define UD_LOST_RATE				41									//用户输率
#define UD_DRAW_RATE				42									//用户和率
#define UD_FLEE_RATE				43									//用户逃率
#define UD_GAME_LEVEL				44									//游戏等级

//扩展信息
#define UD_NOTE_INFO				50									//用户备注
#define UD_LOOKON_USER				51									//旁观用户

//图像列表
#define UD_IMAGE_FLAG				(UD_IMAGE+1)						//用户标志
#define UD_IMAGE_GENDER				(UD_IMAGE+2)						//用户性别
#define UD_IMAGE_STATUS				(UD_IMAGE+3)						//用户状态

//////////////////////////////////////////////////////////////////////////////////
//数据库定义

#define DB_ERROR 					-1  								//处理失败
#define DB_SUCCESS 					0  									//处理成功
#define DB_NEEDMB 					18 									//处理失败

//////////////////////////////////////////////////////////////////////////////////
//道具标示
#define PT_USE_MARK_DOUBLE_SCORE    0x0001								//双倍积分
#define PT_USE_MARK_FOURE_SCORE     0x0002								//四倍积分
#define PT_USE_MARK_GUARDKICK_CARD  0x0010								//防踢道具
#define PT_USE_MARK_POSSESS         0x0020								//附身道具 

#define MAX_PT_MARK                 4                                   //标识数目

//有效范围
#define VALID_TIME_DOUBLE_SCORE     3600                                //有效时间
#define VALID_TIME_FOUR_SCORE       3600                                //有效时间
#define VALID_TIME_GUARDKICK_CARD   3600                                //防踢时间
#define VALID_TIME_POSSESS          3600                                //附身时间
#define VALID_TIME_KICK_BY_MANAGER  3600                                //游戏时间

//任务类型
#define TASK_TYPE_NULL				0									//无效任务
#define TASK_TYPE_DAILY				1									//每日任务
#define TASK_TYPE_REPEAT			2								//重复任务

//任务条件
#define TASK_COND_OTHER				0									//仅次数
#define TASK_COND_PLAY				1									//游戏局数
#define TASK_COND_WIN				2									//游戏赢局

//奖励类型
#define AWARD_TYPE_NULL				0									//无效类型
#define AWARD_TYPE_GOLD				1									//金币奖励
#define AWARD_TYPE_EXPR				2									//经验奖励
#define ARARD_TYPE_MEDAL			3									//奖牌奖励
#define AWARD_TYPE_PROP				4									//道具奖励
#define AWARD_TYPE_FRAGMENT         5                                   //房卡碎片

//奖励途径
#define AWARD_WAY_NULL				0									//无效途径
#define AWARD_WAY_MATCH				1									//比赛奖励
#define AWARD_WAY_TASK				2									//任务奖励

#define TASKID_MING_GANG              1
#define TASKID_AN_GANG                2
#define TASKID_MANGUAN_ONE            3
#define TASKID_MANGUAN_THREE          4
#define TASKID_WIN_THREE			  5
#define TASKID_QIANG_GANG_HU          6
#define TASKID_OPEN_ROOM_ONE          20
#define TASKID_OPEN_ROOM_THREE        21
#define TASKID_SIGN_IN                50
#define TASKID_WX_SHARE               51
#define TASKID_FINISH_ALL             99

#define TASKID_NINECARDS_KILL_ALL     30 //通锅
#define TASKID_NINECARDS_THREE_SAME   31 //九张牌拿到过一次三张相同牌
#define TASKID_NINECARDS_ONE_LUCK     32 //九张牌完成一次吃喜
#define TASKID_NINECARDS_TEN_LUCK	  33 //十次吃喜
#define TASKID_NINECARDS_FOUR_SAME    34 //九张牌拿到过一次四张相同牌
#define TASKID_NINECARDS_COLOR_SAME   35 //九张牌拿到过全红或全黑牌型
#define TASKID_NINECARDS_FIRST_STRAIGHT_FLUSH 38 //九张牌完成第一锅顺清一次
//////////////////////////////////////////////////////////////////////////////////
//设备类型
#define DEVICE_TYPE_PC              0x00                                //PC
#define DEVICE_TYPE_ANDROID         0x10                                //Android
#define DEVICE_TYPE_ITOUCH          0x20                                //iTouch
#define DEVICE_TYPE_IPHONE          0x40                                //iPhone
#define DEVICE_TYPE_IPAD            0x80                                //iPad

#define PRIVATE_CREATE_WAY_ROOM_OWNER 0
#define PRIVATE_CREATE_WAY_AA         1
#define PRIVATE_CREATE_WAY_FOR_OTHER  2


#define OVER_REASON_NULL 0 //无效
#define OVER_REASON_NORMAL 1 //正常情况
#define OVER_REASON_DB_DISMISS 2
#define OVER_REASON_SERVER_DISMISS 3
#define OVER_REASON_PLAYER_DISMISS 4
/////////////////////////////////////////////////////////////////////////////////
//手机定义

//视图模式
//#define	VIEW_MODE_ALL				0x0001								//全部可视
//#define	VIEW_MODE_PART				0x0002								//部分可视

//信息模式
//#define VIEW_INFO_LEVEL_1			0x0010								//部分信息
//#define VIEW_INFO_LEVEL_2			0x0020								//部分信息
//#define VIEW_INFO_LEVEL_3			0x0040								//部分信息
//#define VIEW_INFO_LEVEL_4			0x0080								//部分信息

//其他配置
#define RECVICE_GAME_CHAT			0x0100								//接收聊天
#define RECVICE_ROOM_CHAT			0x0200								//接收聊天
#define RECVICE_ROOM_WHISPER		0x0400								//接收私聊

//行为标识
#define BEHAVIOR_LOGON_NORMAL       0x0000                              //普通登录
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000                              //立即登录

/////////////////////////////////////////////////////////////////////////////////
//处理结果
#define RESULT_ERROR 					-1  								//处理错误
#define RESULT_SUCCESS 					0  									//处理成功
#define RESULT_FAIL 					1  									//处理失败

/////////////////////////////////////////////////////////////////////////////////
//变化原因
#define SCORE_REASON_WRITE              0                                   //写分变化
#define SCORE_REASON_INSURE             1                                   //银行变化
#define SCORE_REASON_PROPERTY           2                                   //道具变化
#define SCORE_REASON_MATCH_FEE          3                                   //比赛报名
#define SCORE_REASON_MATCH_QUIT         4                                   //比赛退赛

/////////////////////////////////////////////////////////////////////////////////

//登录房间失败原因
#define LOGON_FAIL_SERVER_INVALIDATION  200                                 //房间失效

////////////////////////////////////////////////////////////////////////////////
#endif