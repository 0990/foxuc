#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID			1									//I D 登录
#define SUB_GP_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_MB_WX_LOGON_FIRST       4                                   //微信第一次登录
#define SUB_MB_WX_LOGON_SECOND      5                                   //微信后期登录
#define SUB_MB_LOGON_VISITOR        6                                   //游客登录
#define SUB_MB_LOGON_WX_OPENID      7                                   //微信openID登录
#define SUB_MB_LOGON_WX_GAME        8
#define SUB_MB_JOIN_GAME            9                                   //加入游戏
#define SUB_MB_LOGON_WX_TEMP        10


//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号

   

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败
#define SUB_MB_JOIN_GAME_SUCCESS    102                                 //加入游戏成功
#define SUB_MB_JOIN_GAME_FAILURE    103                                 //加入游戏失败


//登录成功
struct CMD_MB_LogonSuccess {
    WORD							wFaceID;							//头像标识
    BYTE							cbGender;							//用户性别
    DWORD							dwUserID;							//用户 I D
    DWORD							dwGameID;							//游戏 I D
    DWORD							dwExperience;						//经验数值
    DWORD							dwLoveLiness;						//用户魅力
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//登录失败
struct CMD_MB_LogonFailure {
    LONG							lResultCode;						//错误代码
    TCHAR							szDescribeString[128];				//描述消息
};


#define MDM_GF_GAME					200									//游戏命令

#define SUB_MB_MOVE                 1                                   //移动
#define SUB_MB_SHOOT                2	                                //射击
#define SUB_MB_WORLD_STATE          3
#define SUB_MB_GAME_END             4                                   //游戏结束
#define SUB_MB_RANK                 5                                   //排名
#define SUB_MB_DESTROY_BULLET       6             
#define SUB_MB_NEW_ENTITY           7                                  
#define SUB_MB_DESTROY_ENTITY       8      
#define SUB_MB_CALCULATE_RESULT     9
#define SUB_MB_JUMP 10
#define SUB_MB_GAME_SCENE 11
#pragma pack()

#endif