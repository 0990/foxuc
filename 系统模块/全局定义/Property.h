#ifndef PROPERTY_HEAD_FILE
#define PROPERTY_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

//发行范围
#define PT_ISSUE_AREA_WEB			0x01								//商城道具
#define PT_ISSUE_AREA_GAME			0x02								//游戏道具
#define PT_ISSUE_AREA_SERVER		0x04								//房间道具

//使用范围
#define PT_SERVICE_AREA_MESELF		0x0001								//自己范围
#define PT_SERVICE_AREA_PLAYER		0x0002								//玩家范围
#define PT_SERVICE_AREA_LOOKON		0x0004								//旁观范围

//道具类型
#define PT_TYPE_ERROR               0                                   //道具类型 
#define PT_TYPE_PROPERTY            1	                                //道具类型  
#define PT_TYPE_PRESENT             2                                   //道具类型

//道具定义
#define PROPERTY_ID_CAR				1									//汽车礼物
#define PROPERTY_ID_EGG				2									//臭蛋礼物
#define PROPERTY_ID_CLAP			3									//鼓掌礼物
#define PROPERTY_ID_KISS			4									//香吻礼物
#define PROPERTY_ID_BEER			5									//啤酒礼物
#define PROPERTY_ID_CAKE			6									//蛋糕礼物
#define PROPERTY_ID_RING			7									//钻戒礼物
#define PROPERTY_ID_BEAT			8									//暴打礼物
#define PROPERTY_ID_BOMB			9									//炸弹礼物
#define PROPERTY_ID_SMOKE			10									//香烟礼物
#define PROPERTY_ID_VILLA			11									//别墅礼物
#define PROPERTY_ID_BRICK			12									//砖头礼物
#define PROPERTY_ID_FLOWER			13									//鲜花礼物

#define PROPERTY_ID_TWO_CARD	    14				                    //积分道具 
#define PROPERTY_ID_FOUR_CARD		15			                        //积分道具  
#define PROPERTY_ID_SCORE_CLEAR     16									//负分清零
#define PROPERTY_ID_ESCAPE_CLEAR    17									//逃跑清零
#define PROPERTY_ID_TRUMPET			18									//喇叭道具
#define PROPERTY_ID_TYPHON			19									//喇叭道具
#define PROPERTY_ID_GUARDKICK_CARD  20									//防踢道具
#define PROPERTY_ID_POSSESS			21									//附身道具
#define PROPERTY_ID_BLUERING_CARD	22									//蓝钻道具
#define PROPERTY_ID_YELLOWRING_CARD	23									//黄钻道具
#define PROPERTY_ID_WHITERING_CARD	24  								//白钻道具 
#define PROPERTY_ID_REDRING_CARD	25									//红钻道具
#define PROPERTY_ID_VIPROOM_CARD	26									//VIP房道具

//////////////////////////////////////////////////////////////////////////////////

//道具信息
struct tagPropertyInfo
{
	//道具信息
	WORD							wIndex;								//道具标识
	WORD							wDiscount;							//会员折扣
	WORD							wIssueArea;							//发布范围

	//销售价格
	SCORE							lPropertyGold;						//道具金币
	DOUBLE							dPropertyCash;						//道具价格

	//赠送魅力
	SCORE							lSendLoveLiness;					//赠送魅力
	SCORE							lRecvLoveLiness;					//接受魅力
};


//道具属性
struct tagPropertyAttrib
{
	WORD							wIndex;								//道具标识
	WORD                            wPropertyType;                      //道具类型
	WORD							wServiceArea;						//使用范围
	TCHAR                           szMeasuringunit[8];					//计量单位 
	TCHAR							szPropertyName[32];					//道具名字
	TCHAR							szRegulationsInfo[256];				//使用信息
};

//道具子项
struct tagPropertyItem
{
	tagPropertyInfo					PropertyInfo;						//道具信息
	tagPropertyAttrib				PropertyAttrib;						//道具属性
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif