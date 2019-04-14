#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��¼����
#define	DBR_GP_LOGON_GAMEID			1									//I D ��¼
#define	DBR_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�

//�˺ŷ���
#define DBR_GP_MODIFY_MACHINE		10									//�޸Ļ���
#define DBR_GP_MODIFY_LOGON_PASS	11									//�޸�����
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_UNDER_WRITE	13									//�޸�ǩ��
#define DBR_GP_MODIFY_INDIVIDUAL	14									//�޸�����

//ͷ������
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//�޸�ͷ��
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//�޸�ͷ��

//��������
#define DBR_GP_USER_SAVE_SCORE		30									//������Ϸ��
#define DBR_GP_USER_TAKE_SCORE		31									//��ȡ��Ϸ��
#define DBR_GP_USER_TRANSFER_SCORE	32									//ת����Ϸ��

//��ѯ����
#define DBR_GP_QUERY_INDIVIDUAL		40									//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	41									//��ѯ����
#define DBR_GP_QUERY_USER_INFO	    42									//��ѯ�û�
#define DBR_GP_ALMS_INFO            43                                  //�ȼý����
#define DBR_MB_RECORD_INFO          44                                  //�ȼý����
#define DBR_MB_EXCHANGE_ROOM_CARD   45                                  //�ȼý����
#define DBR_MB_ROOM_CARD_INFO		46									//���ͷ���
#define DBR_MB_INSURE_PASSWORD_INFO 47									//������������
#define DBR_MB_FRAGMENT_EXCHANGE_RATE	48								//����һ�����
#define DBR_MB_SEEK_NICKNAME		49									//�����ǳ�
#define DBR_MB_SEEK_USER_TREASURE	60									//������ҲƸ�
#define DBR_MB_TASK_COMPLETED		61									//��������
#define DBR_MB_LOAD_USER_TASK		62									//�����������
#define DBR_MB_ACCEPT_TASK_AWARD	63									//��ȡ����
#define DBR_MB_LOAD_TASK_INFO		64									//����������Ϣ
#define DBR_MB_LOAD_SIGN_IN_RECORD	65									//ǩ����¼
#define DBR_MB_LOAD_SYSTEM_MESSAGE	66									//����ϵͳ��Ϣ
#define DBR_MB_LOAD_LOGON_CONFIG	67									//��������
#define DBR_MB_GET_PRIVATE_RECORD	68									//��������¼
#define DBR_MB_RECEIVE_ALMS			69									//��ȡ�ȼý�
#define DBR_MB_USER_BIND_PROMOTE	70									//���ƹ�

//ϵͳ����
#define DBR_GP_LOAD_GAME_LIST		50									//�����б�
#define DBR_GP_ONLINE_COUNT_INFO	51									//������Ϣ

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GP_LogonGameID {
    //��¼��Ϣ
    DWORD							dwGameID;							//�û� I D
    TCHAR							szPassword[LEN_MD5];				//��¼����
    BYTE							cbNeeValidateMBCard;				//�ܱ�У��

    //������Ϣ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

    //������Ϣ
    LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonAccounts {
    //��¼��Ϣ
    TCHAR							szPassword[LEN_MD5];				//��¼����
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
    BYTE							cbNeeValidateMBCard;				//�ܱ�У��

    //������Ϣ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

    //������Ϣ
    LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts {
    //ע����Ϣ
    WORD							wFaceID;							//ͷ���ʶ
    BYTE							cbGender;							//�û��Ա�
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
    TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
    TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
    TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
    TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

    //�������
    TCHAR							szLogonPass[LEN_MD5];				//��¼����
    TCHAR							szInsurePass[LEN_MD5];				//��������

    //������Ϣ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

    //������Ϣ
    LPVOID							pBindParameter;						//�󶨲���
};

//�޸Ļ���
struct DBR_GP_ModifyMachine {
    BYTE							cbBind;								//�󶨱�־
    DWORD							dwUserID;							//�û���ʶ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szPassword[LEN_PASSWORD];			//�û�����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyLogonPass {
    DWORD							dwUserID;							//�û� I D
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
    TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyInsurePass {
    DWORD							dwUserID;							//�û� I D
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
    TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct DBR_GP_ModifyUnderWrite {
    DWORD							dwUserID;							//�û� I D
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szPassword[LEN_PASSWORD];			//�û�����
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//�޸�ͷ��
struct DBR_GP_ModifySystemFace {
    //�û���Ϣ
    WORD							wFaceID;							//ͷ���ʶ
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPassword[LEN_MD5];				//��¼����

    //������Ϣ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct DBR_GP_ModifyCustomFace {
    //�û���Ϣ
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPassword[LEN_MD5];				//��¼����
    DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ

    //������Ϣ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyIndividual {
    //��֤����
    DWORD							dwUserID;							//�û� I D
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szPassword[LEN_PASSWORD];			//�û�����

    //�ʺ�����
    BYTE							cbGender;							//�û��Ա�
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

    //�û���Ϣ
    TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
    TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

    //�绰����
    TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

    //��ϵ����
    TCHAR							szQQ[LEN_QQ];						//Q Q ����
    TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
    TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��ѯ����
struct DBR_GP_QueryIndividual {
    DWORD							dwUserID;							//�û� I D
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//������Ϸ��
struct DBR_GP_UserSaveScore {
    DWORD							dwUserID;							//�û� I D
    SCORE							lSaveScore;							//������Ϸ��
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GP_UserTakeScore {
    DWORD							dwUserID;							//�û� I D
    SCORE							lTakeScore;							//��ȡ��Ϸ��
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szPassword[LEN_PASSWORD];			//��������
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GP_UserTransferScore {
    DWORD							dwUserID;							//�û� I D
    DWORD							dwClientAddr;						//���ӵ�ַ
    BYTE                            cbByNickName;                       //�ǳ�����
    SCORE							lTransferScore;						//ת����Ϸ��
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
    TCHAR							szPassword[LEN_PASSWORD];			//��������
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GP_QueryInsureInfo {
    DWORD							dwUserID;							//�û� I D
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//��ѯ�û�
struct DBR_GP_QueryInsureUserInfo {
    BYTE                            cbByNickName;                       //�ǳ�����
    TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�ȼý����
struct DBR_GP_AlmsInfo {
    DWORD                           dwUserID;                           //�û�ID
    bool                            bGetAlms;                           //�Ƿ��ȡ�ȼý�
};
struct DBR_MB_RecordInfo {
    DWORD       dwUserID;
    int         nCurrRecordID;
    TCHAR		szPassword[LEN_PASSWORD];			//�û�����
};
struct DBR_MB_ExchangeRoomCard {
    DWORD dwUserID;
    DWORD dwExchangeGameID;
    char szCardNum[LEN_PASSWORD];
};
//���ͷ���
struct DBR_MB_RoomCardInfo {
    DWORD		dwUserID;							//�û�ID
    DWORD		dwGameID;							//��ϷID
    DWORD		dwRoomCardCount;					//��������
    DWORD		dwRedDiamondCount;					//��������
    TCHAR		szPassword[LEN_PASSWORD];			//�û�����
};
//������������
struct DBR_MB_InsurePasswdInfo {
    DWORD		dwUserID;							//�û�ID
    TCHAR		szLogPasswd[LEN_PASSWORD];			//�û�����
    TCHAR		szPassword[LEN_PASSWORD];			//��������
};
//����һ�����
struct DBR_MB_FragmentExchangRate {
    DWORD		dwUserID;							//�û�ID
    TCHAR		szLogPasswd[LEN_PASSWORD];			//�û�����
    WORD		wDiamondType;						//�һ�����
};
//�����ǳ�
struct DBR_MB_SeekNickname {
    DWORD		dwGameID;							//��ϷID
};
//�����Ƹ�
struct DBR_MB_SeekUserTreasure {
    DWORD		dwUserID;							//�û�ID
};
//��������
struct DBR_MB_TaskComplete {
    DWORD		dwUserID;							//�û�ID
    DWORD		dwTaskID;							//����ID
    TCHAR		szPasswd[LEN_PASSWORD];			//�û�����
};
//ǩ����¼
struct DBR_MB_SignInRecord {
    DWORD		dwUserID;							//�û�ID
};
//�����������
struct DBR_MB_LoadUserTask {
    DWORD		dwUserID;							//�û�ID
    int			dwKindID;							//��Ϸ����
};
//��ȡ����
struct DBR_MB_AcceptTaskAward {
    DWORD		dwUserID;							//�û�ID
    TCHAR		szLogPasswd[LEN_PASSWORD];			//�û�����
    DWORD		dwTaskID;							//����ID
};
//��������¼
struct DBR_MB_PrivateRecord {
    DWORD		dwOrderNumber;							//¼��ID
    DWORD		dwPrivateID;
    DWORD		dwRoundCount;
};
//��ȡ�ȼý�
struct DBR_MB_ReceiveAlms {
    int			nUserID;
    TCHAR		szLogPasswd[LEN_PASSWORD];			//�û�����
};
//���ƹ�
struct DBR_MB_UserBindPromote {
    DWORD			dwGameID;						//���ID
    DWORD			dwPromoteGameID;				//�ƹ�ԱID
};
//�û�����
struct DBO_GP_UserTransferUserInfo {
    DWORD							dwGameID;							//�û� I D
    TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};
//ϵͳ��Ϣ
struct DBR_GR_SystemMessage {
    BYTE                            cbMessageID;                        //��ϢID
    BYTE							cbMessageType;						//��Ϣ����
    DWORD							dwTimeRate;						    //ʱ��Ƶ��
    __time64_t                      tConcludeTime;                      //����ʱ��
    TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};
//������Ϣ
struct DBR_GP_OnLineCountInfo {
    WORD							wKindCount;							//������Ŀ
    DWORD							dwOnLineCountSum;					//��������
    tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//��������
};

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//��¼���
#define DBO_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GP_LOGON_FAILURE		101									//��¼ʧ��
#define DBR_GP_VALIDATE_MBCARD		102									//��¼ʧ��
#define DBO_GP_WXLOGON              103                                 //΢�ŵ�¼


//�������
#define DBO_GP_USER_FACE_INFO		110									//�û�ͷ��
#define DBO_GP_USER_INDIVIDUAL		111									//�û�����

//��������
#define DBO_GP_USER_INSURE_INFO					120						//��������
#define DBO_GP_USER_INSURE_SUCCESS				121						//���гɹ�
#define DBO_GP_USER_INSURE_FAILURE				122						//����ʧ��
#define DBO_GP_USER_INSURE_USER_INFO			123						//�û�����
#define DBO_GP_USER_ALMS_SUCCESS				124                     //�ȼý�ɹ�
#define DBO_GP_USER_ALMS_FAILURE				125                     //��ѯ��ʧ��
#define DBO_MB_RECORD_INFO						126                     //��ѯ��Ϸ��¼
#define DBO_MB_EXCHANGE_ROOM_CARD				127                     //��ѯ��Ϸ��¼
#define DBO_MB_ROOM_CARD_INFO					128						//��ѯ������¼
#define DBO_MB_INSURE_PASSWORD_INFO				129						//������������
#define DBO_MB_FRAGMENT_EXCHANGE_RATE			140						//����һ�����
#define DBO_MB_SEEK_NICKNAME					141						//�����ǳ�
#define DBO_MB_SEEK_USER_TREASURE				142						//�����Ƹ�
#define DBO_MB_TASK_COMPLETE					143						//��������
#define DBO_MB_LOAD_USER_TASK					144						//�����������
#define DBO_MB_ACCEPT_TASK_AWARD				145						//��ȡ����
#define DBO_MB_LOAD_TASK_INFO					146						//����������Ϣ
#define DBO_MB_LOAD_SIGN_IN_RECORD				147						//ǩ����¼
#define	DBO_MB_LOAD_SYSTEM_MESSAGE				148						//����ϵͳ��Ϣ
#define DBO_MB_LOAD_LOGON_CONFIG				149						//��������
#define DBO_MB_RECEIVE_ALMS						150						//��ȡ�ȼý�
#define DBO_MB_USER_BIND_PROMOTE				151						//���ƹ�
//�б���
#define DBO_GP_GAME_TYPE_ITEM		130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM		131									//������Ϣ
#define DBO_GP_GAME_NODE_ITEM		132									//�ڵ���Ϣ
#define DBO_GP_GAME_PAGE_ITEM		133									//������Ϣ
#define DBO_GP_GAME_LIST_RESULT		134									//���ؽ��

//������
#define DBO_GP_OPERATE_SUCCESS		500									//�����ɹ�
#define DBO_GP_OPERATE_FAILURE		501									//����ʧ��

struct DBO_MB_LogonWX {
    TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
    char                            szHeadImgUrl[256];
	char                            szRefreshToken[256];
	char                            szUnionID[256];
	char                            szOpenID[256];
};

//��¼�ɹ�
struct DBO_GP_LogonSuccess {
    //��������
    WORD							wFaceID;							//ͷ���ʶ
    DWORD							dwUserID;							//�û���ʶ
    DWORD							dwGameID;							//��Ϸ��ʶ
    DWORD							dwGroupID;							//���ű�ʶ
    DWORD							dwCustomID;							//�Զ�����
    DWORD							dwUserMedal;						//�û�����
    DWORD							dwExperience;						//������ֵ
    DWORD							dwLoveLiness;						//�û�����
    TCHAR							szPassword[LEN_MD5];				//��¼����
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
    TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

    //�û��ɼ�
    SCORE							lUserScore;							//�û���Ϸ��
    SCORE							lUserInsure;						//�û�����

    //�û�����
    BYTE							cbGender;							//�û��Ա�
    BYTE							cbMoorMachine;						//��������
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

    //��Ա����
    BYTE							cbMemberOrder;						//��Ա�ȼ�
    SYSTEMTIME						MemberOverDate;						//����ʱ��

    //������Ϣ
    TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GP_LogonFailure {
    LONG							lResultCode;						//�������
    TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBR_GP_ValidateMBCard {
    UINT							uMBCardID;						//��������
};

//ͷ����Ϣ
struct DBO_GP_UserFaceInfo {
    WORD							wFaceID;							//ͷ���ʶ
    DWORD							dwCustomID;							//�Զ�����
};

//��������
struct DBO_GP_UserIndividual {
    //�û���Ϣ
    DWORD							dwUserID;							//�û� I D
    TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
    TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

    //�绰����
    TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

    //��ϵ����
    TCHAR							szQQ[LEN_QQ];						//Q Q ����
    TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
    TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��������
struct DBO_GP_UserInsureInfo {
    WORD							wRevenueTake;						//˰�ձ���
    WORD							wRevenueTransfer;					//˰�ձ���
    WORD							wServerID;							//�����ʶ
    SCORE							lUserScore;							//�û���Ϸ��
    SCORE							lUserInsure;						//������Ϸ��
    SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GP_UserInsureSuccess {
    DWORD							dwUserID;							//�û� I D
    SCORE							lSourceScore;						//ԭ����Ϸ��
    SCORE							lSourceInsure;						//ԭ����Ϸ��
    SCORE							lInsureRevenue;						//����˰��
    SCORE							lVariationScore;					//��Ϸ�ұ仯
    SCORE							lVariationInsure;					//���б仯
    TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct  DBO_GP_UserInsureFailure {
    LONG							lResultCode;						//��������
    TCHAR							szDescribeString[128];				//������Ϣ
};

struct DBO_GP_AlmsFailure {
    LONG							lResultCode;						//��������
    TCHAR							szDescribeString[128];				//������Ϣ
};

//�ȼý���
struct DBO_GP_AlmsSuccess {
    DWORD							dwUserID;							//�û�ID
    SCORE							lUserScore;							//�û�Ԫ��
    SCORE							lUserInsure;						//����Ԫ��
    INT                             nUseCount;                          //ʹ�ô���
    bool							bGetAlms;				            //�Ƿ��ȡ�ȼý�
    INT                             nMaxUseCount;                       //ÿ����ȡ�ȼô���
    SCORE                           lAlmsScore;                         //ÿ����ȡ�ȼý���
    SCORE							lAlmsThreshold;                     //��ȡ�ȼ���ֵ
    TCHAR							szDescribeString[128];		//������Ϣ
};

//����ʧ��
struct DBO_GP_OperateFailure {
    LONG							lResultCode;						//��������
    TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct DBO_GP_OperateSuccess {
    LONG							lResultCode;						//��������
    TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct DBO_GP_GameType {
    WORD							wJoinID;							//�ҽ�����
    WORD							wSortID;							//��������
    WORD							wTypeID;							//��������
    TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct DBO_GP_GameKind {
    WORD							wTypeID;							//��������
    WORD							wJoinID;							//�ҽ�����
    WORD							wSortID;							//��������
    WORD							wKindID;							//��������
    WORD							wGameID;							//ģ������
    TCHAR							szKindName[LEN_KIND];				//��Ϸ����
    TCHAR							szProcessName[LEN_PROCESS];			//��������
    TCHAR                           szGameRuleURL[MAX_PATH];            //��Ϸ������ַ
    TCHAR                           szDownLoadURL[MAX_PATH];            //��Ϸ������ַ
};
struct PlayerInfo {
    DWORD   dwUserID;
    WORD    wChairID;
    TCHAR	szNickName[LEN_NICKNAME];				//��Ϸ����
};

struct DBO_MB_GameRecord {
    int nRecordID;//��Ϸ��¼ID
    WORD wKindID;//��Ϸ
    WORD wServerID;//��Ϸ����
    char szRecordData[MAX_RECORD_BYTES];//��¼����
    __time64_t	tRecordTime;	//��Ϸ��¼ʱ��
};

struct DBO_MB_ExchangeRoomCard {
    bool bSuccess;
    DOUBLE dRoomCardCount;
    TCHAR							szDescribeString[128];				//��Ϣ
};
//���ͽ��
struct DBO_MB_RoomCardInfo {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//��Ϣ
    DOUBLE							dRoomCardCount;					//��������
    DOUBLE							dRedDiamondCount;					//��������
};

//�����������ý��
struct DBO_MB_InsurePasswdInfo {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//��Ϣ
};
//����һ��������
struct DBO_MB_FragmentExchangRate {
    bool							bSuccess;
    DOUBLE							dRoomCardCount;					//��������
    DWORD							dwFragmentCardCount;				//��������
    DOUBLE							dRedDiamondCount;					//��������
    TCHAR							szDescribeString[128];				//��Ϣ
    WORD							wDiamondType;						//�һ�����
};
//�����ǳ�
struct DBO_MB_SeekNickname {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//��Ϣ
    TCHAR							szNickName[LEN_NICKNAME];			//��Ϸ����
};
//�����Ƹ�
struct DBO_MB_SeekUserTreasure {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//��Ϣ
    DOUBLE							dRoomCardCount;					//��������
    DOUBLE							dRedDiamondCount;					//��������
    DWORD							dwFragmentCardCount;				//��������
    SCORE							lUserScore;							//��ҷ���
};
//��������
struct DBO_MB_TaskComplete {
    bool							bSuccess;
    TCHAR							szDescribeString[128];				//��Ϣ
    DWORD							dwDays;								//ǩ������
    DWORD							dwNowFragmentCardCount;				//����ǩ����������
    DWORD							dwFragmentCardCount;				//��������
    DWORD							dwTaskID;							//����ID
};
struct UserTaskInfo {
    DWORD							dwTaskID;							//����ID
    DWORD							dwFinishCount;						//��ɴ���
    bool							bAccept;							//������ȡ��־
    bool							bFinish;							//������ɱ�־
    time_t							tLastTime;							//���ʱ��
};
//���������Ϣ
struct DBO_MB_UserTaskInfo {
    bool							bSuccess;
    DWORD							dwTaskCount;						//��������
    UserTaskInfo					TaskInfo[MAX_TASK_COUNT];			//�������
};
struct TaskInfo {
    DWORD							dwTaskID;						//����ID
    TCHAR							szTaskName[LEN_NICKNAME];		//��������
    WORD							wTaskType;						//��������
    DWORD							dwCondition;					//�������
    DWORD							dwAmount;						//��ɴ���
    WORD							wAwardType;						//��������
    DWORD							dwAwardCount;					//��������
    DWORD							dwKindID;						//��Ϸ����
};
//����������Ϣ
struct DBO_MB_TaskInfo {
    TaskInfo						taskInfo[MAX_TASK_COUNT];		//������Ϣ
    DWORD							dwTaskCount;						//��������
};
//ǩ����¼
struct tagUserSignInRecordInfo {
    WORD							wFragmentCardCounts;				//��������
    int								tSignInDate;						//ǩ��ʱ��
};
struct DBO_MB_SignInRecord {
    WORD							wSignInDays;						//ǩ��������
    WORD							wFragmentCardCounts[31];			//��Ҫ��õ���������
    tagUserSignInRecordInfo			SignInRecord[31];					//ǩ����¼
    WORD							wDays;								//����ǩ������
    WORD							wToDay;								//��ǰ����
    WORD							wMonth;								//��ǰ�·�
    WORD							wDaysInMonth;						//��������
};
//��ȡ����
struct DBO_MB_AcceptTaskAward {
    bool							bSuccess;
    DWORD							dwTaskID;						//����ID
    DWORD							dwAwardCount;					//��������
    SCORE							lScore;						//�ܷ���
    DWORD							dwFragmentCardCount;			//��������
    TCHAR							szDescribeString[128];			//��Ϣ
};
//��������
//������״̬
struct tagServerStatus {
    bool							bServerStatus;						//������״̬
    TCHAR							szDescrubeString[128];				//��Ϣ
};
struct DBO_MB_LoadLogonConfig {
    bool							bSuccess;
    TCHAR							cbVersion[20];					//�汾��
    tagServerStatus					pServerStatus;					//������״̬
    bool							bAdsShow;						//��浯��
    TCHAR							szDescribeString[128];			//��Ϣ
};
//���ƹ�
struct DBO_MB_UserBindPromote {
    int								nRoomCardCount;
    bool							bSuccess;
    TCHAR							szDescribeString[128];			//��Ϣ
};
//��Ϸ�ڵ�
struct DBO_GP_GameNode {
    WORD							wKindID;							//��������
    WORD							wJoinID;							//�ҽ�����
    WORD							wSortID;							//��������
    WORD							wNodeID;							//�ڵ�����
    TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct DBO_GP_GamePage {
    WORD							wKindID;							//��������
    WORD							wNodeID;							//�ڵ�����
    WORD							wSortID;							//��������
    WORD							wPageID;							//��������
    WORD							wOperateType;						//��������
    TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//���ؽ��
struct DBO_GP_GameListResult {
    BYTE							cbSuccess;							//�ɹ���־
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//�ֻ����ݰ�

//��¼����
#define	DBR_MB_LOGON_GAMEID			500									//I D ��¼
#define	DBR_MB_LOGON_ACCOUNTS		501									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS	502									//ע���ʺ�
#define DBR_MB_LOGON_VISITOR		505									//�ο͵�¼
#define DBR_MB_LOGON_WX             506                                 //΢�ŵ�¼
#define DBR_MB_PRIVATE_CREATE	    503									//��ⷿ���Ƿ����
#define DBR_MB_PRIVATE_JOIN_CHECK	504									//��ⷿ���Ƿ����
#define DBR_MB_PRIVATE_BACK_ROOM	507									//�ص�����
//��¼���
#define DBO_MB_LOGON_SUCCESS		600									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE		601									//��¼ʧ��

#define DBO_MB_PRIVATE_CREATE_RESULT 603                                //���ݿⷿ���������
#define DBO_MB_PRIVATE_JOIN_CHECK_RESULT	604							//��ⷿ���Ƿ����
#define DBO_MB_PRIVATE_BACK_ROOM_RESULT		605							//�ص�������

#define DBO_MB_GET_PRIVATE_RECORD		606							    //���������
//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_MB_LogonGameID {
    //��¼��Ϣ
    DWORD							dwGameID;							//�û� I D
    TCHAR							szPassword[LEN_MD5];				//��¼����

    //������Ϣ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

    //������Ϣ
    LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonAccounts {
    //��¼��Ϣ
    TCHAR							szPassword[LEN_MD5];				//��¼����
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

    //������Ϣ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

    //������Ϣ
    LPVOID							pBindParameter;						//�󶨲���
};
//�ο͵�¼
struct DBR_MB_LogonVisitor {
    //bool                            bCreateNew;
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szPassword[LEN_MD5];				//��¼����
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

    LPVOID							pBindParameter;						//�󶨲���
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
    DWORD							dwClientAddr;						//���ӵ�ַ
    LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_MB_RegisterAccounts {
    //ע����Ϣ
    WORD							wFaceID;							//ͷ���ʶ
    BYTE							cbGender;							//�û��Ա�
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
    TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

    //�������
    TCHAR							szLogonPass[LEN_MD5];				//��¼����
    TCHAR							szInsurePass[LEN_MD5];				//��������

    //������Ϣ
    DWORD							dwClientAddr;						//���ӵ�ַ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

    //������Ϣ
    LPVOID							pBindParameter;						//�󶨲���
};

struct DBR_MB_PrivateCreate {
    int   nCreateWay;  //0������������1����AA��2����Ƿ�������
    DWORD dwUserID;
    TCHAR szPassword[LEN_MD5];				//��¼����
    DWORD dwServerID;
    WORD  wKindID;
    int	  nPlayerCount;
    int   nRoundCount;
    int   nPlayRule;
    int   nExtraRule;
    int   nTimes;							//����
    bool  bDynamicEnter;
    bool  bAllowSameIP;
    bool  bAllowRepeat;
    char  szOtherConfig[256];
};
struct DBR_MB_PrivateJoinCheck {
    DWORD dwUserID;
    TCHAR szPassword[LEN_MD5];				//��¼����
    int nRoomID;
};
struct DBO_MB_PrivateJoinCheckResult {
    bool bSuccess;
    DWORD dwCreatorUserID;
    int nRoomID;
    int nServerID;
    int nCreateWay;
    LONG  lResultCode;
    TCHAR szDescribeString[128];										//������Ϣ
};

struct DBO_MB_PrivateCreateResult {
    bool  bSuccess;
    LONG  lResultCode;													//�������
    TCHAR szDescribeString[128];										//������Ϣ
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
    int   nTimes;							//����
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
    //�������
    TCHAR szDescribeString[128];										//������Ϣ
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
    int   nTimes;							//����
    bool  bDynamicEnter;
    bool  bAllowSameIP;
    char  szOtherConfig[256];

    int nFinishCount;
    int nKindID;
    int nRecordNumber;
};
//��ȡ�ȼý�
struct DBO_MB_ReceiveAlms {
    bool  bSuccess;
    int	  nFragmentCardCount;
    TCHAR szDescribeString[128];										//������Ϣ
};
//�ص�����
struct DBR_MB_PrivateBackRoom {
    DWORD dwUserID;
    TCHAR szPassword[LEN_MD5];				//��¼����
};
//�ص�����
struct DBO_MB_PrivateBackRoomResult {
    bool  bSuccess;
    TCHAR szDescribeString[128];										//������Ϣ
    int nServerID;
};
//��¼�ɹ�
struct DBO_MB_LogonSuccess {
    //�û�����
    WORD							wFaceID;							//ͷ���ʶ
    BYTE							cbGender;							//�û��Ա�
    DWORD							dwUserID;							//�û� I D
    DWORD							dwGameID;							//��Ϸ I D
    DWORD							dwExperience;						//������ֵ
    DWORD							dwLoveLiness;						//�û�����
    SCORE							lUserScore;
    TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
    DOUBLE                          dRoomCardCount;                    //��������
    DOUBLE                          dRedDiamondCount;						//��������
    DWORD                           dwFragmentCardCount;                //��Ƭ����
    DWORD                           dwLockServerID;
    TCHAR							szPassword[LEN_MD5];				//��¼����
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
    bool                            bHaveInsurePass;
    BYTE							cbMemberOrder;						//��Ա�ȼ�
    bool							bReceiveAlms;

    char                            szHeadImgUrl[256];
    char                            szRefreshToken[256];
    BYTE                            cbAccountType;
    //������Ϣ
    TCHAR							szDescribeString[128];				//������Ϣ
};


//��¼ʧ��
struct DBO_MB_LogonFailure {
    LONG							lResultCode;						//�������
    TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

#endif