#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GR_LOGON				1									//��¼��Ϣ

//��¼ģʽ
#define SUB_GR_LOGON_USERID			1									//I D ��¼
#define SUB_GR_LOGON_MOBILE			2									//�ֻ���¼
#define SUB_GR_LOGON_ACCOUNTS		3									//�ʻ���¼

//��¼���
#define SUB_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GR_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH			102									//��¼���
//������ʾ
#define SUB_GR_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_GR_LogonUserID {
    //�汾��Ϣ
    DWORD							dwPlazaVersion;						//�㳡�汾
    DWORD							dwFrameVersion;						//��ܰ汾
    DWORD							dwProcessVersion;					//���̰汾

    //��¼��Ϣ
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPassword[LEN_MD5];				//��¼����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
    WORD							wKindID;							//��������
};

//�ֻ���¼
struct CMD_GR_LogonMobile {
    //�汾��Ϣ
    WORD							wGameID;							//��Ϸ��ʶ
    DWORD							dwProcessVersion;					//���̰汾

    //��������
    BYTE                            cbDeviceType;                       //�豸����
    WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
    WORD                            wPageTableCount;                    //��ҳ����

    //��¼��Ϣ
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPassword[LEN_MD5];				//��¼����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct CMD_GR_LogonAccounts {
    //�汾��Ϣ
    DWORD							dwPlazaVersion;						//�㳡�汾
    DWORD							dwFrameVersion;						//��ܰ汾
    DWORD							dwProcessVersion;					//���̰汾

    //��¼��Ϣ
    TCHAR							szPassword[LEN_MD5];				//��¼����
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��¼�ɹ�
struct CMD_GR_LogonSuccess {
    DWORD							dwUserRight;						//�û�Ȩ��
    DWORD							dwMasterRight;						//����Ȩ��
};

//��¼ʧ��
struct CMD_GR_LogonFailure {
    LONG							lErrorCode;							//�������
    TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_GR_UpdateNotify {
    //������־
    BYTE							cbMustUpdatePlaza;					//ǿ������
    BYTE							cbMustUpdateClient;					//ǿ������
    BYTE							cbAdviceUpdateClient;				//��������

    //��ǰ�汾
    DWORD							dwCurrentPlazaVersion;				//��ǰ�汾
    DWORD							dwCurrentFrameVersion;				//��ǰ�汾
    DWORD							dwCurrentClientVersion;				//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_CONFIG				2									//������Ϣ

#define SUB_GR_CONFIG_COLUMN		100									//�б�����
#define SUB_GR_CONFIG_SERVER		101									//��������
#define SUB_GR_CONFIG_PROPERTY		102									//��������
#define SUB_GR_CONFIG_FINISH		103									//�������
#define SUB_GR_CONFIG_USER_RIGHT	104									//���Ȩ��

//////////////////////////////////////////////////////////////////////////////////

//�б�����
struct CMD_GR_ConfigColumn {
    BYTE							cbColumnCount;						//�б���Ŀ
    tagColumnItem					ColumnItem[MAX_COLUMN];				//�б�����
};

//��������
struct CMD_GR_ConfigServer {
    //��������
    WORD							wTableCount;						//������Ŀ
    WORD							wChairCount;						//������Ŀ

    //��������
    WORD							wServerType;						//��������
    DWORD							dwServerRule;						//�������
};

//��������
struct CMD_GR_ConfigProperty {
    BYTE							cbPropertyCount;					//������Ŀ
    tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//��������
};

//���Ȩ��
struct CMD_GR_ConfigUserRight {
    DWORD							dwUserRight;						//���Ȩ��
};
//////////////////////////////////////////////////////////////////////////////////
//�û�����

#define MDM_GR_USER					3									//�û���Ϣ

//�û�����
#define SUB_GR_USER_RULE			1									//�û�����
#define SUB_GR_USER_LOOKON			2									//�Թ�����
#define SUB_GR_USER_SITDOWN			3									//��������
#define SUB_GR_USER_STANDUP			4									//��������
#define SUB_GR_USER_INVITE			5									//�û�����
#define SUB_GR_USER_INVITE_REQ		6									//��������
#define SUB_GR_USER_REPULSE_SIT  	7									//�ܾ��������
#define SUB_GR_USER_KICK_USER       8                                   //�߳��û�
#define SUB_GR_USER_INFO_REQ        9                                   //�����û���Ϣ
#define SUB_GR_USER_CHAIR_REQ       10                                  //�������λ��
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //���������û���Ϣ
#define SUB_GR_USER_WAIT_DISTRIBUTE 12									//�ȴ�����
#define SUB_GR_USER_FOREGROUND 13									//��Һ�̨�л���Ϣ
#define SUB_GR_USER_LOCATION   14                                    //��ַ��Ϣ

//�û�״̬
#define SUB_GR_USER_ENTER			100									//�û�����
#define SUB_GR_USER_SCORE			101									//�û�����
#define SUB_GR_USER_STATUS			102									//�û�״̬
#define SUB_GR_REQUEST_FAILURE		103									//����ʧ��
#define SUB_GR_USER_AWARD			104									//�û�����
#define SUB_GR_USER_GOLD			105									//��ұ仯

//��������
#define SUB_GR_USER_CHAT			201									//������Ϣ
#define SUB_GR_USER_EXPRESSION		202									//������Ϣ
#define SUB_GR_WISPER_CHAT			203									//˽����Ϣ
#define SUB_GR_WISPER_EXPRESSION	204									//˽�ı���
#define SUB_GR_COLLOQUY_CHAT		205									//�Ự��Ϣ
#define SUB_GR_COLLOQUY_EXPRESSION	206									//�Ự����

//��������
#define SUB_GR_PROPERTY_BUY			300									//�������
#define SUB_GR_PROPERTY_SUCCESS		301									//���߳ɹ�
#define SUB_GR_PROPERTY_FAILURE		302									//����ʧ��
#define SUB_GR_PROPERTY_MESSAGE     303                                 //������Ϣ
#define SUB_GR_PROPERTY_EFFECT      304                                 //����ЧӦ
#define SUB_GR_PROPERTY_TRUMPET		305                                 //������Ϣ

//////////////////////////////////////////////////////////////////////////////////

//�Թ�����
struct CMD_GR_UserLookon {
    WORD							wTableID;							//����λ��
    WORD							wChairID;							//����λ��
};

//��������
struct CMD_GR_UserSitDown {
    WORD							wTableID;							//����λ��
    WORD							wChairID;							//����λ��
    TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//��������
struct CMD_GR_UserStandUp {
    WORD							wTableID;							//����λ��
    WORD							wChairID;							//����λ��
    bool 							bForceLeave;						//ǿ���뿪
};

//�����û�
struct CMD_GR_UserInvite {
    WORD							wTableID;							//���Ӻ���
    DWORD							dwUserID;							//�û� I D
};

//�����û�����
struct CMD_GR_UserInviteReq {
    WORD							wTableID;							//���Ӻ���
    DWORD							dwUserID;							//�û� I D
};

//�û�����
struct CMD_GR_UserScore {
    DWORD							dwUserID;							//�û���ʶ
    tagUserScore					UserScore;							//������Ϣ
};

//�û�����
struct CMD_GR_MobileUserScore {
    DWORD							dwUserID;							//�û���ʶ
    tagMobileUserScore				UserScore;							//������Ϣ
};

//�û�����
struct CMD_GR_Award {
    DWORD							dwUserID;							//�û���ʶ
    DWORD							dwExperience;						//�û�����
    DWORD							dwGold;								//��Ϸ��
    DWORD							dwMedal;							//�û�����
    BYTE							cbPropType;							//��������
    DWORD							dwPropCount;						//���߽���
};

//�û�״̬
struct CMD_GR_UserStatus {
    DWORD							dwUserID;							//�û���ʶ
    tagUserStatus					UserStatus;							//�û�״̬
};

//����ʧ��
struct CMD_GR_RequestFailure {
    LONG							lErrorCode;							//�������
    TCHAR							szDescribeString[256];				//������Ϣ
};


//�û�����
struct CMD_GR_C_UserChat {
    WORD							wChatLength;						//��Ϣ����
    DWORD							dwChatColor;						//��Ϣ��ɫ
    DWORD							dwTargetUserID;						//Ŀ���û�
    TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_S_UserChat {
    WORD							wChatLength;						//��Ϣ����
    DWORD							dwChatColor;						//��Ϣ��ɫ
    DWORD							dwSendUserID;						//�����û�
    DWORD							dwTargetUserID;						//Ŀ���û�
    TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_C_UserExpression {
    WORD							wItemIndex;							//��������
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GR_S_UserExpression {
    WORD							wItemIndex;							//��������
    DWORD							dwSendUserID;						//�����û�
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�˽��
struct CMD_GR_C_WisperChat {
    WORD							wChatLength;						//��Ϣ����
    DWORD							dwChatColor;						//��Ϣ��ɫ
    DWORD							dwTargetUserID;						//Ŀ���û�
    TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�˽��
struct CMD_GR_S_WisperChat {
    WORD							wChatLength;						//��Ϣ����
    DWORD							dwChatColor;						//��Ϣ��ɫ
    DWORD							dwSendUserID;						//�����û�
    DWORD							dwTargetUserID;						//Ŀ���û�
    TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//˽�ı���
struct CMD_GR_C_WisperExpression {
    WORD							wItemIndex;							//��������
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//˽�ı���
struct CMD_GR_S_WisperExpression {
    WORD							wItemIndex;							//��������
    DWORD							dwSendUserID;						//�����û�
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û��Ự
struct CMD_GR_ColloquyChat {
    WORD							wChatLength;						//��Ϣ����
    DWORD							dwChatColor;						//��Ϣ��ɫ
    DWORD							dwSendUserID;						//�����û�
    DWORD							dwConversationID;					//�Ự��ʶ
    DWORD							dwTargetUserID[16];					//Ŀ���û�
    TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�����û�
struct CMD_GR_C_InviteUser {
    WORD							wTableID;							//���Ӻ���
    DWORD							dwSendUserID;						//�����û�
};

//�����û�
struct CMD_GR_S_InviteUser {
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//�������
struct CMD_GR_C_PropertyBuy {
    BYTE                            cbRequestArea;						//����Χ
    BYTE							cbConsumeScore;						//��������
    WORD							wItemCount;							//������Ŀ
    WORD							wPropertyIndex;						//��������
    DWORD							dwTargetUserID;						//ʹ�ö���
};

//���߳ɹ�
struct CMD_GR_S_PropertySuccess {
    BYTE                            cbRequestArea;						//ʹ�û���
    WORD							wItemCount;							//������Ŀ
    WORD							wPropertyIndex;						//��������
    DWORD							dwSourceUserID;						//Ŀ�����
    DWORD							dwTargetUserID;						//ʹ�ö���
};

//����ʧ��
struct CMD_GR_PropertyFailure {
    WORD                            wRequestArea;                       //��������
    LONG							lErrorCode;							//�������
    TCHAR							szDescribeString[256];				//������Ϣ
};

//������Ϣ
struct CMD_GR_S_PropertyMessage {
    //������Ϣ
    WORD                            wPropertyIndex;                     //��������
    WORD                            wPropertyCount;                     //������Ŀ
    DWORD                           dwSourceUserID;                     //Ŀ�����
    DWORD                           dwTargerUserID;                     //ʹ�ö���
};


//����ЧӦ
struct CMD_GR_S_PropertyEffect {
    DWORD                           wUserID;					        //�� ��I D
    BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//��������
struct CMD_GR_C_SendTrumpet {
    BYTE                           cbRequestArea;                        //����Χ
    WORD                           wPropertyIndex;                      //��������
    DWORD                          TrumpetColor;                        //������ɫ
    TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//��������
struct CMD_GR_S_SendTrumpet {
    WORD                           wPropertyIndex;                      //��������
    DWORD                          dwSendUserID;                         //�û� I D
    DWORD                          TrumpetColor;                        //������ɫ
    TCHAR                          szSendNickName[32];				    //����ǳ�
    TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};


//�û��ܾ�����������
struct CMD_GR_UserRepulseSit {
    WORD							wTableID;							//���Ӻ���
    WORD							wChairID;							//����λ��
    DWORD							dwUserID;							//�û� I D
    DWORD							dwRepulseUserID;					//�û� I D
};

//////////////////////////////////////////////////////////////////////////////////

//�����־
#define UR_LIMIT_SAME_IP			0x01								//���Ƶ�ַ
#define UR_LIMIT_WIN_RATE			0x02								//����ʤ��
#define UR_LIMIT_FLEE_RATE			0x04								//��������
#define UR_LIMIT_GAME_SCORE			0x08								//���ƻ���

//�û�����
struct CMD_GR_UserRule {
    BYTE							cbRuleMask;							//��������
    WORD							wMinWinRate;						//���ʤ��
    WORD							wMaxFleeRate;						//�������
    LONG							lMaxGameScore;						//��߷���
    LONG							lMinGameScore;						//��ͷ���
};

//�����û���Ϣ
struct CMD_GR_UserInfoReq {
    DWORD                           dwUserIDReq;                        //�����û�
    WORD							wTablePos;							//����λ��
};

//�����û���Ϣ
struct CMD_GR_ChairUserInfoReq {
    WORD							wTableID;							//���Ӻ���
    WORD							wChairID;							//����λ��
};
//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define MDM_GR_STATUS				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO			100									//������Ϣ
#define SUB_GR_TABLE_STATUS			101									//����״̬

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct CMD_GR_TableInfo {
    WORD							wTableCount;						//������Ŀ
    tagTableStatus					TableStatusArray[512];				//����״̬
};

//����״̬
struct CMD_GR_TableStatus {
    WORD							wTableID;							//���Ӻ���
    tagTableStatus					TableStatus;						//����״̬
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_INSURE				5									//�û���Ϣ

//��������
#define SUB_GR_QUERY_INSURE_INFO	1									//��ѯ����
#define SUB_GR_SAVE_SCORE_REQUEST	2									//������
#define SUB_GR_TAKE_SCORE_REQUEST	3									//ȡ�����
#define SUB_GR_TRANSFER_SCORE_REQUEST	4								//ȡ�����
#define SUB_GR_QUERY_USER_INFO_REQUEST	5								//��ѯ�û�

#define SUB_GR_USER_INSURE_INFO		100									//��������
#define SUB_GR_USER_INSURE_SUCCESS	101									//���гɹ�
#define SUB_GR_USER_INSURE_FAILURE	102									//����ʧ��
#define SUB_GR_USER_TRANSFER_USER_INFO	103								//�û�����

//////////////////////////////////////////////////////////////////////////////////

//��ѯ����
struct CMD_GR_C_QueryInsureInfoRequest {
    BYTE                            cbActivityGame;                     //��Ϸ����
    TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//�������
struct CMD_GR_C_SaveScoreRequest {
    BYTE                            cbActivityGame;                     //��Ϸ����
    SCORE							lSaveScore;							//�����Ŀ
};

//ȡ������
struct CMD_GR_C_TakeScoreRequest {
    BYTE                            cbActivityGame;                     //��Ϸ����
    SCORE							lTakeScore;							//ȡ����Ŀ
    TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//ת�˽��
struct CMD_GP_C_TransferScoreRequest {
    BYTE                            cbActivityGame;                     //��Ϸ����
    BYTE                            cbByNickName;                       //�ǳ�����
    SCORE							lTransferScore;						//ת�˽��
    TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
    TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//��ѯ�û�
struct CMD_GR_C_QueryUserInfoRequest {
    BYTE                            cbActivityGame;                     //��Ϸ����
    BYTE                            cbByNickName;                       //�ǳ�����
    TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//��������
struct CMD_GR_S_UserInsureInfo {
    BYTE                            cbActivityGame;                     //��Ϸ����
    WORD							wRevenueTake;						//˰�ձ���
    WORD							wRevenueTransfer;					//˰�ձ���
    WORD							wServerID;							//�����ʶ
    SCORE							lUserScore;							//�û����
    SCORE							lUserInsure;						//���н��
    SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct CMD_GR_S_UserInsureSuccess {
    BYTE                            cbActivityGame;                     //��Ϸ����
    SCORE							lUserScore;							//���Ͻ��
    SCORE							lUserInsure;						//���н��
    TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GR_S_UserInsureFailure {
    BYTE                            cbActivityGame;                     //��Ϸ����
    LONG							lErrorCode;							//�������
    TCHAR							szDescribeString[128];				//������Ϣ
};

//�û���Ϣ
struct CMD_GR_S_UserTransferUserInfo {
    BYTE                            cbActivityGame;                     //��Ϸ����
    DWORD							dwTargetGameID;						//Ŀ���û�
    TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};
//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_MANAGE				6									//��������

#define SUB_GR_SEND_WARNING			1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KILL_USER			4									//�߳��û�
#define SUB_GR_LIMIT_ACCOUNS		5									//�����ʻ�
#define SUB_GR_SET_USER_RIGHT		6									//Ȩ������

//��������
#define SUB_GR_QUERY_OPTION			7									//��ѯ����
#define SUB_GR_OPTION_SERVER		8									//��������
#define SUB_GR_OPTION_CURRENT		9									//��ǰ����

#define SUB_GR_LIMIT_USER_CHAT		10									//��������

#define SUB_GR_KICK_ALL_USER		11									//�߳��û�
#define SUB_GR_DISMISSGAME		    12									//��ɢ��Ϸ

//////////////////////////////////////////////////////////////////////////////////

//���;���
struct CMD_GR_SendWarning {
    WORD							wChatLength;						//��Ϣ����
    DWORD							dwTargetUserID;						//Ŀ���û�
    TCHAR							szWarningMessage[LEN_USER_CHAT];	//������Ϣ
};

////ϵͳ��Ϣ
//struct CMD_GR_SendMessage {
//    BYTE							cbGame;								//��Ϸ��Ϣ
//    BYTE							cbRoom;								//��Ϸ��Ϣ
//    BYTE							cbAllRoom;							//��Ϸ��Ϣ
//    BYTE                            cbLoop;                             //ѭ����־
//    DWORD                           dwTimeRate;                         //ѭ�����
//    __time64_t                      tConcludeTime;                      //����ʱ��
//    WORD							wChatLength;						//��Ϣ����
//    TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
//};

//�鿴��ַ
struct CMD_GR_LookUserIP {
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//�߳��û�
struct CMD_GR_KickUser {
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ʻ�
struct CMD_GR_LimitAccounts {
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//Ȩ������
struct CMD_GR_SetUserRight {
    //Ŀ���û�
    DWORD							dwTargetUserID;						//Ŀ���û�

    //�󶨱���
    BYTE							cbGameRight;						//�ʺ�Ȩ��
    BYTE							cbAccountsRight;					//�ʺ�Ȩ��

    //Ȩ�ޱ仯
    BYTE							cbLimitRoomChat;					//��������
    BYTE							cbLimitGameChat;					//��Ϸ����
    BYTE							cbLimitPlayGame;					//��ϷȨ��
    BYTE							cbLimitSendWisper;					//������Ϣ
    BYTE							cbLimitLookonGame;					//�Թ�Ȩ��
};

//��������
struct CMD_GR_OptionCurrent {
    DWORD							dwRuleMask;							//��������
    tagServerOptionInfo				ServerOptionInfo;					//��������
};

//��������
struct CMD_GR_ServerOption {
    tagServerOptionInfo				ServerOptionInfo;					//��������
};

//�߳������û�
struct CMD_GR_KickAllUser {
    TCHAR							szKickMessage[LEN_USER_CHAT];		//�߳���ʾ
};

//��ɢ��Ϸ
struct CMD_GR_DismissGame {
    WORD							wDismissTableNum;		            //��ɢ����
};
//////////////////////////////////////////////////////////////////////////////////

//���ñ�־
#define OSF_ROOM_CHAT				1									//��������
#define OSF_GAME_CHAT				2									//��Ϸ����
#define OSF_ROOM_WISPER				3									//����˽��
#define OSF_ENTER_TABLE				4									//������Ϸ
#define OSF_ENTER_SERVER			5									//���뷿��
#define OSF_SEND_BUGLE				12									//��������

//��������
struct CMD_GR_OptionServer {
    BYTE							cbOptionFlags;						//���ñ�־
    BYTE							cbOptionValue;						//���ñ�־
};

//��������
struct CMD_GR_LimitUserChat {
    DWORD							dwTargetUserID;						//Ŀ���û�
    BYTE							cbLimitFlags;						//���Ʊ�־
    BYTE							cbLimitValue;						//�������
};
//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_MATCH				7									//��������

#define SUB_GR_MATCH_FEE			400									//��������
#define SUB_GR_MATCH_NUM			401									//�ȴ�����
#define SUB_GR_LEAVE_MATCH			402									//�˳�����
#define SUB_GR_MATCH_INFO			403									//������Ϣ
#define SUB_GR_MATCH_WAIT_TIP		404									//�ȴ���ʾ
#define SUB_GR_MATCH_RESULT			405									//�������
#define SUB_GR_MATCH_STATUS			406									//����״̬
#define SUB_GR_USER_MATCH_STATUS	407									//����״̬
#define SUB_GR_MATCH_DESC			408									//��������
#define SUB_GR_MATCH_TIME			409									//����ʱ��
#define SUB_GR_REWARD_INFO			412									//������Ϣ
#define SUB_GR_MATCH_RANK			413									//��������
#define SUB_GR_MATCH_USER_OUT		414									//�û���̭
#define SUB_GR_CALL_MATCH           415                                 //����С���
////////////////�Ķ�����ʱ �뽫��Ϸ����CMD_GAME.H��ͬʱ�Ķ�////////////////////////////
#define SUB_GR_MATCH_INFO_ER_SPARROWS	410									//������Ϣ(2���齫)
//������Ϣ��2���齫���У�
struct CMD_GR_Match_ER_Sparrows {
    WORD							wALLGameRound;						//�ܹ���Ϸ��
    WORD							wALLGameRoundFan;					//�ܹ���Ϸ��
    WORD							wCurGameRound;						//��ǰ��Ϸ��
    WORD							wCurGameRoundFan;					//��ǰ��Ϸ��
    WORD							wLocalGameFan;						//������Ϸ��
    WORD							wOtherGameFan;						//�Լ���Ϸ��
    WORD							wNextGameRoundFan;					//������Ϸ��
};

//��������
struct CMD_GR_Match_Num {
    DWORD							dwWaitting;							//�ȴ�����
    DWORD							dwTotal;							//��������
};

//����ʱ��
struct CMD_GR_Match_Time {
    DWORD							dwSysTime;							//ϵͳʱ��
    DWORD							dwStartTime;						//����ʱ��
};

//������Ϣ
struct CMD_GR_Match_Info {
    TCHAR							szTitle[4][64];						//��Ϣ����
    WORD							wGameCount;							//��Ϸ����
};

//������Ϣ
struct CMD_MB_MatchInfo {
    int								nOutScore;							//��̭����
    BYTE							cbGameCount;						//��Ϸ����
    BYTE							cbTotalCount;						//�ܾ���
    BYTE							cbTotalRount;						//������
    BYTE							cbCurrentRount;						//��ǰ����
};

//��������
struct tagMatchReward {
    DWORD								dwGold;							//��ҽ���
    DWORD								dwMedal;						//���ƽ���
    DWORD								dwExperience;					//���齱��
    BYTE								cbPropType;						//��������
    DWORD								dwPropCount;					//���߽���
    TCHAR                               szExtraAward[128];                   //���⽱��
};

//��������
struct CMD_GR_Match_Rank {
    WORD							wCurRank;							//��ǰ����
    WORD							wUserCount;							//������
};

//��ʾ��Ϣ
struct CMD_GR_Match_Wait_Tip {
    SCORE							lScore;								//��ǰ����
    WORD							wRank;								//��ǰ����
    WORD							wCurTableRank;						//��������
    WORD							wUserCount;							//��ǰ����
    WORD							wPlayingTable;						//��Ϸ����
    TCHAR							szMatchName[LEN_SERVER];			//��������
};

//�������
struct CMD_GR_MatchResult {
    tagMatchReward					MatchReward;						//��������
    WORD							wLength;							//��������
    TCHAR							szDescribe[256];					//�ý�����
};

//�������
struct CMD_GR_MatchUserOut {
    DWORD							wLength;							//��������
    TCHAR							szDescribe[256];					//�ý�����
};

#define MAX_MATCH_DESC				4									//�������
//��������
struct CMD_GR_MatchDesc {
    TCHAR							szTitle[MAX_MATCH_DESC][16];		//��Ϣ����
    TCHAR							szDescribe[MAX_MATCH_DESC][64];		//��������
    COLORREF						crTitleColor;						//������ɫ
    COLORREF						crDescribeColor;					//������ɫ
};

struct CMD_GR_MatchCall {
    BOOL                            bEnabled;
    WORD							wInterval;
};

//////////////////////////////////////////////////////////////////////////////////
//�������

#define MDM_GF_FRAME				100									//�������

//////////////////////////////////////////////////////////////////////////////////
//�������

//�û�����
#define SUB_GF_GAME_OPTION			1									//��Ϸ����
#define SUB_GF_USER_READY			2									//�û�׼��
#define SUB_GF_LOOKON_CONFIG		3									//�Թ�����
#define SUB_GF_VOTE_DISMISS			4									//ͶƱ��ɢ
//��������
#define SUB_GF_USER_CHAT			10									//�û�����
#define SUB_GF_USER_EXPRESSION		11									//�û�����
#define SUB_GF_USER_VOICE			12									//�������

//��Ϸ��Ϣ
#define SUB_GF_GAME_STATUS			100									//��Ϸ״̬
#define SUB_GF_GAME_SCENE			101									//��Ϸ����
#define SUB_GF_LOOKON_STATUS		102									//�Թ�״̬
#define SUB_GF_PRIVATE_OVER         103                                 //��������
#define SUB_GF_TASK_INFO			104									//������Ϣ
#define SUB_GF_TASK_STATUS			105									//����״̬
#define SUB_GF_PRIVATE_INFO         106									//����������Ϣ								
#define SUB_GF_PRIVATE_START        107									//�������俪ʼ��Ϣ								
#define SUB_GF_GAME_RECORD          108                                 //ÿ����Ϸ��¼								
#define SUB_GF_PRIVATE_RECORD		109                                 //������Ϸ��¼								
#define SUB_GF_CONFIRM_ENTER		110                                 //������Ϸ��¼								
#define SUB_GF_READY_WARN           111                                 //��;�������׼������ʱ
#define SUB_GF_WATCH_WARN           112                                 //��ҳ�ʱ�䲻������ʾ
#define SUB_GF_WATCH_CANCEL         113                                 //��ҳ�ʱ�䲻������ʾ
//ϵͳ��Ϣ
#define SUB_GF_SYSTEM_MESSAGE		200									//ϵͳ��Ϣ
#define SUB_GF_ACTION_MESSAGE		201									//������Ϣ

//////////////////////////////////////////////////////////////////////////////////
struct CMD_GF_TotalScore {
    SCORE lScore[MAX_CHAIR];
};
//��Ϸ����
struct CMD_GF_GameOption {
    BYTE							cbAllowLookon;						//�Թ۱�־
    DWORD							dwFrameVersion;						//��ܰ汾
    DWORD							dwClientVersion;					//��Ϸ�汾
};

//�Թ�����
struct CMD_GF_LookonConfig {
    DWORD							dwUserID;							//�û���ʶ
    BYTE							cbAllowLookon;						//�����Թ�
};

//�Թ�״̬
struct CMD_GF_LookonStatus {
    BYTE							cbAllowLookon;						//�����Թ�
};

//��Ϸ����
struct CMD_GF_GameStatus {
    BYTE							cbGameStatus;						//��Ϸ״̬
    BYTE							cbAllowLookon;						//�Թ۱�־
};

//�û�����
struct CMD_GF_C_UserChat {
    WORD							wChatLength;						//��Ϣ����
    DWORD							dwChatColor;						//��Ϣ��ɫ
    DWORD							dwTargetUserID;						//Ŀ���û�
    TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_S_UserChat {
    WORD							wChatLength;						//��Ϣ����
    DWORD							dwChatColor;						//��Ϣ��ɫ
    DWORD							dwSendUserID;						//�����û�
    DWORD							dwTargetUserID;						//Ŀ���û�
    TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_C_UserExpression {
    WORD							wItemIndex;							//��������
    DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GF_S_UserExpression {
    WORD							wItemIndex;							//��������
    DWORD							dwSendUserID;						//�����û�
    DWORD							dwTargetUserID;						//Ŀ���û�
};
//������Ϣ
struct CMD_GF_UserTaskInfo {
    BYTE							cbTaskCount;						//������Ŀ
    tagSendTaskInfoItem				GameTaskInfo[MAX_TASK_COUNT];		//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����

#define MDM_GF_GAME					200									//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ

//������Ϣ
#define DTP_GR_TABLE_PASSWORD		1									//��������

//�û�����
#define DTP_GR_NICK_NAME			10									//�û��ǳ�
#define DTP_GR_GROUP_NAME			11									//��������
#define DTP_GR_UNDER_WRITE			12									//����ǩ��

//������Ϣ
#define DTP_GR_USER_NOTE			20									//�û���ע
#define DTP_GR_CUSTOM_FACE			21									//�Զ�ͷ��

//////////////////////////////////////////////////////////////////////////////////

//�������
#define REQUEST_FAILURE_NORMAL		0									//����ԭ��
#define REQUEST_FAILURE_NOGOLD		1									//��Ҳ���
#define REQUEST_FAILURE_NOSCORE		2									//���ֲ���
#define REQUEST_FAILURE_PASSWORD	3									//�������

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif