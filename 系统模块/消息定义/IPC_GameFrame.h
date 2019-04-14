#ifndef IPC_GAME_FRAME_HEAD_FILE
#define IPC_GAME_FRAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ

#define IPC_CMD_GF_SOCKET			1									//������Ϣ

#define IPC_SUB_GF_SOCKET_SEND		1									//���緢��
#define IPC_SUB_GF_SOCKET_RECV		2									//�������

//���緢��
struct IPC_GF_SocketSend
{
	TCP_Command						CommandInfo;						//������Ϣ
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//���ݻ���
};

//�������
struct IPC_GF_SocketRecv
{
	TCP_Command						CommandInfo;						//������Ϣ
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//���ݻ���
};

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ

#define IPC_CMD_GF_CONTROL			2									//������Ϣ

#define IPC_SUB_GF_CLIENT_READY		1									//׼������
#define IPC_SUB_GF_CLIENT_CLOSE		2									//���̹ر�

#define IPC_SUB_GF_CLOSE_PROCESS	100									//�رս���
#define IPC_SUB_GF_ACTIVE_PROCESS	101									//�������

#define IPC_SUB_GF_BOSS_COME		200									//�ϰ�����
#define IPC_SUB_GF_BOSS_LEFT		201									//�ϰ�����

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ

#define IPC_CMD_GF_CONFIG			3									//������Ϣ

#define IPC_SUB_GF_LEVEL_INFO		100									//�ȼ���Ϣ
#define IPC_SUB_GF_COLUMN_INFO		101									//�б���Ϣ
#define IPC_SUB_GF_SERVER_INFO		102									//������Ϣ
#define IPC_SUB_GF_PROPERTY_INFO	103									//������Ϣ
#define IPC_SUB_GF_CONFIG_FINISH	104									//�������
#define IPC_SUB_GF_USER_RIGHT		107									//���Ȩ��

//������Ϣ
struct IPC_GF_ServerInfo
{
	//�û���Ϣ
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D

	//�û�Ȩ��
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��

	//������Ϣ
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	WORD							wServerType;						//��������
	DWORD							dwServerRule;						//�������
	TCHAR							szServerName[LEN_SERVER];			//��������

	//��Ƶ����
	WORD							wAVServerPort;						//����˿�
	DWORD							dwAVServerAddr;						//�����ַ
};

//�ȼ���Ϣ
struct IPC_GF_LevelInfo
{
	BYTE							cbItemCount;						//�б���Ŀ
	tagLevelItem					LevelItem[64];						//�ȼ�����
};

//�б�����
struct IPC_GF_ColumnInfo
{
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItem[MAX_COLUMN];				//�б�����
};

//��������
struct IPC_GF_PropertyInfo
{
	BYTE							cbPropertyCount;					//������Ŀ
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//��������
};

//���Ȩ��
struct IPC_GF_UserRight
{
	DWORD							dwUserRight;						//���Ȩ��
};
//////////////////////////////////////////////////////////////////////////////////
//�û���Ϣ

#define IPC_CMD_GF_USER_INFO		4									//�û���Ϣ

#define IPC_SUB_GF_USER_ENTER		100									//�û�����
#define IPC_SUB_GF_USER_SCORE		101									//�û�����
#define IPC_SUB_GF_USER_STATUS		102									//�û�״̬
#define IPC_SUB_GF_USER_ATTRIB		103									//�û�����
#define IPC_SUB_GF_CUSTOM_FACE		104									//�Զ�ͷ��
#define IPC_SUB_GF_KICK_USER        105                                 //�û��߳�
#define IPC_SUB_GF_QUICK_TRANSPOS   106                                 //�û���λ

//�û���Ϣ
struct IPC_GF_UserInfo
{
	BYTE							cbCompanion;						//�û���ϵ
	tagUserInfoHead					UserInfoHead;						//�û���Ϣ
};

//�û�����
struct IPC_GF_UserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserScore					UserScore;							//�û�����
};

//�û�״̬
struct IPC_GF_UserStatus
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserStatus					UserStatus;							//�û�״̬
};

//�û�����
struct IPC_GF_UserAttrib
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserAttrib					UserAttrib;							//�û�����
};

//�û�ͷ��
struct IPC_GF_CustomFace
{
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	tagCustomFaceInfo				CustomFaceInfo;						//�Զ�ͷ��
};

//�û��߳�
struct IPC_GF_KickUser
{
	DWORD                          dwTargetUserID;                      //Ŀ���û�
};

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ
#define IPC_CMD_GF_PROPERTY_INFO		5								//������Ϣ

#define IPC_SUB_GF_BUY_PROPERTY 		100								//�������
#define IPC_SUB_GF_PROPERTY_SUCCESS		101								//���߳ɹ�
#define IPC_SUB_GF_PROPERTY_FAILURE     102                             //����ʧ�� 
#define IPC_SUB_GR_PROPERTY_MESSAGE     103                             //������Ϣ
#define IPC_SUB_GR_PROPERTY_TRUMPET     104                             //��������

//�������
struct IPC_GF_BuyProperty
{
	WORD							wItemCount;                         //������Ŀ  
	WORD							wPropertyIndex;                     //�������� 
	TCHAR							szNickName[LEN_NICKNAME];           //�Է��ǳ�
};

//ʹ�õ���
struct IPC_GR_PropertySuccess
{
	WORD							wPropertyIndex;                     //��������
	WORD                            wPropertyCount;                     //������Ŀ 
	DWORD                           dwSourceUserID;                     //�û� ID
	DWORD                           dwTargetUserID;                     //�û� ID
	TCHAR                           szSourceNickName[LEN_NICKNAME];     //�û��ǳ�
};

//������Ϣ
struct IPC_GR_PropertyMessage
{
	WORD                           wPropertyIndex;                     //��������
	WORD                           wPropertyCount;                     //������Ŀ 
	TCHAR                          szSourceNickName[LEN_NICKNAME];     //�û��ǳ�
	TCHAR                          szTargerNickName[LEN_NICKNAME];     //�û��ǳ�
};

//����ʧ��
struct IPC_GR_PropertyFailure
{
	LONG						   lErrorCode;							//�������
	TCHAR						   szDescribeString[256];				//������Ϣ
};


//��������
struct IPC_GR_PropertyTrumpet
{
	WORD                           wPropertyIndex;                      //�������� 
	DWORD						   dwSendUserID;                        //�û� I D
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szSendNickName[32];				    //����ǳ� 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ
#define IPC_CMD_GF_MATCH_INFO		6								//������Ϣ

#define IPC_SUB_GF_EXIT_MATCH 		100								//�뿪����
#endif