#ifndef CMD_COMMOM_HEAD_FILE
#define CMD_COMMOM_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

#define MDM_CM_SYSTEM				1000								//系统命令

#define SUB_CM_SYSTEM_MESSAGE		1									//系统消息
#define SUB_CM_ACTION_MESSAGE		2									//动作消息
#define SUB_CM_DOWN_LOAD_MODULE		3									//下载消息

//////////////////////////////////////////////////////////////////////////////////

//类型掩码
#define SMT_CHAT					0x0001								//聊天消息
#define SMT_EJECT					0x0002								//弹出消息
#define SMT_GLOBAL					0x0004								//全局消息
#define SMT_PROMPT					0x0008								//提示消息
#define SMT_TABLE_ROLL				0x0010								//滚动消息

//控制掩码
#define SMT_CLOSE_ROOM				0x0100								//关闭房间
#define SMT_CLOSE_GAME				0x0200								//关闭游戏
#define SMT_CLOSE_LINK				0x0400								//中断连接

//系统消息
struct CMD_CM_SystemMessage
{
	WORD							wType;								//消息类型
	WORD							wLength;							//消息长度
	TCHAR							szString[1024];						//消息内容
};
//系统消息
struct CMD_GR_SendMessage {
	BYTE							cbGame;								//游戏消息
	BYTE							cbRoom;								//游戏消息
	BYTE							cbAllRoom;							//游戏消息
	BYTE                            cbLoop;                             //循环标志
	DWORD                           dwTimeRate;                         //循环间隔
	__time64_t                      tConcludeTime;                      //结束时间
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};
//////////////////////////////////////////////////////////////////////////////////

//动作类型
#define ACT_BROWSE					1									//浏览动作
#define ACT_DOWN_LOAD				2									//下载动作

//动作信息
struct tagActionHead
{
	UINT							uResponseID;						//响应标识
	WORD							wAppendSize;						//附加大小
	BYTE							cbActionType;						//动作类型
};

//浏览类型
#define BRT_IE						0x01								//I E 浏览
#define BRT_PLAZA					0x02								//大厅浏览
#define BRT_WINDOWS					0x04								//窗口浏览

//浏览动作
struct tagActionBrowse
{
	BYTE							cbBrowseType;						//浏览类型
	TCHAR							szBrowseUrl[256];					//浏览地址
};

//下载类型
#define DLT_IE						1									//I E 下载
#define DLT_MODULE					2									//下载模块

//下载动作
struct tagActionDownLoad
{
	BYTE							cbDownLoadMode;						//下载方式
	TCHAR							szDownLoadUrl[256];					//下载地址
};

//动作消息
struct CMD_CM_ActionMessage
{
	WORD							wType;								//消息类型
	WORD							wLength;							//消息长度
	UINT							nButtonType;						//按钮类型
	TCHAR							szString[1024];						//消息内容
};

//////////////////////////////////////////////////////////////////////////////////

//下载信息
struct CMD_CM_DownLoadModule
{
	BYTE							cbShowUI;							//显示界面
	BYTE							cbAutoInstall;						//自动安装
	WORD							wFileNameSize;						//名字长度
	WORD							wDescribeSize;						//描述长度
	WORD							wDownLoadUrlSize;					//地址长度
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif