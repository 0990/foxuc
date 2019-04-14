
#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE


#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ�б�

//��Ϸ����
struct tagGameType {
    WORD							wJoinID;							//�ҽ�����
    WORD							wSortID;							//��������
    WORD							wTypeID;							//��������
    TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct tagGameKind {
    WORD							wTypeID;							//��������
    WORD							wJoinID;							//�ҽ�����
    WORD							wSortID;							//��������
    WORD							wKindID;							//��������
    WORD							wGameID;							//ģ������
    DWORD							dwOnLineCount;						//��������
    DWORD							dwFullCount;						//��Ա����
    TCHAR							szKindName[LEN_KIND];				//��Ϸ����
    TCHAR							szProcessName[LEN_PROCESS];			//��������
    TCHAR                           szGameRuleURL[MAX_PATH];            //��Ϸ������ַ����
    TCHAR                           szDownLoadURL[MAX_PATH];            //��Ϸ������ַ
};

//��Ϸ�ڵ�
struct tagGameNode {
    WORD							wKindID;							//��������
    WORD							wJoinID;							//�ҽ�����
    WORD							wSortID;							//��������
    WORD							wNodeID;							//�ڵ�����
    TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct tagGamePage {
    WORD							wPageID;							//ҳ������
    WORD							wKindID;							//��������
    WORD							wNodeID;							//�ڵ�����
    WORD							wSortID;							//��������
    WORD							wOperateType;						//��������
    TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//��Ϸ����
struct tagGameServer {
    WORD							wKindID;							//��������
    WORD							wNodeID;							//�ڵ�����
    WORD							wSortID;							//��������
    WORD							wServerID;							//��������
    WORD							wServerPort;						//����˿�
    WORD                            wServerType;                        //��������
    DWORD							dwOnLineCount;						//��������
    DWORD							dwFullCount;						//��Ա����
    bool                            bAcceptNew;                         //�Ƿ��ڽ����·���
    TCHAR							szServerAddr[32];					//��������
    TCHAR							szServerName[LEN_SERVER];			//��������
};

//��Ƶ����
struct tagAVServerOption {
    WORD							wAVServerPort;						//��Ƶ�˿�
    DWORD							dwAVServerAddr;						//��Ƶ��ַ
};

//������Ϣ
struct tagOnLineInfoKind {
    WORD							wKindID;							//���ͱ�ʶ
    DWORD							dwOnLineCount;						//��������
};

//������Ϣ
struct tagOnLineInfoServer {
    WORD							wServerID;							//�����ʶ
    DWORD							dwOnLineCount;						//��������
};

//////////////////////////////////////////////////////////////////////////////////
//�û���Ϣ

//����״̬
struct tagTableStatus {
    BYTE							cbTableLock;						//������־
    BYTE							cbPlayStatus;						//��Ϸ��־
};

//�û�״̬
struct tagUserStatus {
    WORD							wTableID;							//��������
    WORD							wChairID;							//����λ��
    BYTE							cbUserStatus;						//�û�״̬
};

//�û�����
struct tagUserAttrib {
    BYTE							cbCompanion;						//�û���ϵ
};

//�û�����
struct tagUserScore {
    //������Ϣ
    SCORE							lScore;								//�û�����
    SCORE							lGrade;								//�û��ɼ�
    SCORE							lInsure;							//�û�����

    //��Ӯ��Ϣ
    DWORD							dwWinCount;							//ʤ������
    DWORD							dwLostCount;						//ʧ������
    DWORD							dwDrawCount;						//�;�����
    DWORD							dwFleeCount;						//��������

    //ȫ����Ϣ
    DWORD							dwUserMedal;						//�û�����
    DWORD							dwExperience;						//�û�����
    LONG							lLoveLiness;						//�û�����
};

//�û�����
struct tagMobileUserScore {
    //������Ϣ
    SCORE							lScore;								//�û�����

    //��Ӯ��Ϣ
    DWORD							dwWinCount;							//ʤ������
    DWORD							dwLostCount;						//ʧ������
    DWORD							dwDrawCount;						//�;�����
    DWORD							dwFleeCount;						//��������

    //ȫ����Ϣ
    DWORD							dwExperience;						//�û�����
};


//����ʹ��
struct tagUsePropertyInfo {
    WORD                            wPropertyCount;                     //������Ŀ
    WORD                            dwValidNum;						    //��Ч����
    DWORD                           dwEffectTime;                       //��Чʱ��
};


//�û�����
struct tagUserProperty {
    WORD                            wPropertyUseMark;                   //���߱�ʾ
    tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//ʹ����Ϣ
};

//������Ϣ
struct tagUserTaskInfoItem {
    WORD							wTaskID;							//�����ʶ
    WORD							wFinishCount;						//��ɴ���
    bool                            bFinished;                          //���״̬
    bool                            bAccepted;                          //�������״̬
};
typedef std::map<int, tagUserTaskInfoItem> Map_Int_UserTaskInfoItem;
//�û�����   ���ݿ⽻��ʱʹ��
struct tagUserTaskInfo {
    BYTE							cbTaskCount;						//������Ŀ
    time_t							tloadTime;                          //�������ʱ��
    tagUserTaskInfoItem				UserTaskInfo[MAX_TASK_COUNT];		//�û�����
};

struct tagUserTaskInfoMapItem {
    void init() {
        tloadTime = 0;
        mapUserTaskInfo.clear();
    }
    time_t							tloadTime;
    Map_Int_UserTaskInfoItem		mapUserTaskInfo;
};

//���߰���
struct tagPropertyPackage {
    WORD                            wTrumpetCount;                     //С������
    WORD                            wTyphonCount;                      //��������
};

//ʱ����Ϣ
struct tagTimeInfo {
    DWORD						dwEnterTableTimer;						//��������ʱ��
    DWORD						dwLeaveTableTimer;						//�뿪����ʱ��
    DWORD						dwStartGameTimer;						//��ʼ��Ϸʱ��
    DWORD						dwEndGameTimer;							//�뿪��Ϸʱ��
};

//�û���Ϣ
struct tagUserInfo {
    //��������
    DWORD							dwUserID;							//�û� I D
    DWORD							dwGameID;							//��Ϸ I D
    DWORD							dwGroupID;							//���� I D
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
    TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
    char                            szHeadImgUrl[LEN_HEAD_IMG_URL];     //���ͷ��url
    TCHAR                           szLocation[LEN_DWELLING_PLACE];     //��Ҷ�λ

    int                             nAccuracy;                           //��λ����
	SCORE                            lLocateTime;                         //��λʱ��

    BYTE                            cbAccountType;						//�˻�����
    //ͷ����Ϣ
    WORD							wFaceID;							//ͷ������
    DWORD							dwCustomID;							//�Զ���ʶ

    //�û�����
    BYTE							cbGender;							//�û��Ա�
    BYTE							cbMemberOrder;						//��Ա�ȼ�
    BYTE							cbMasterOrder;						//����ȼ�

    //�û�״̬
    WORD							wTableID;							//��������
    WORD							wChairID;							//��������
    BYTE							cbUserStatus;						//�û�״̬
    bool                            bForeground;                        //����ֻ���ǰ̨��־
    bool                            bDynamicEnter;                      //��;�����־

    //������Ϣ
    SCORE							lScore;								//�û�����
    SCORE							lGrade;								//�û��ɼ�
    SCORE							lInsure;							//�û�����

    //��Ϸ��Ϣ
    DWORD							dwWinCount;							//ʤ������
    DWORD							dwLostCount;						//ʧ������
    DWORD							dwDrawCount;						//�;�����
    DWORD							dwFleeCount;						//��������
    DWORD							dwUserMedal;						//�û�����
    DWORD							dwExperience;						//�û�����
    LONG							lLoveLiness;						//�û�����

    //ʱ����Ϣ
    tagTimeInfo						TimerInfo;							//ʱ����Ϣ

    //������Ϣ
    BYTE							cbEnlistStatus;						//����״̬

    //��չ��ʶ
    LONG							lExpand;
    DWORD							dwExpand;
};

//�û���Ϣ
struct tagUserInfoHead {
    //�û�����
    DWORD							dwGameID;							//��Ϸ I D
    DWORD							dwUserID;							//�û� I D
    DWORD							dwGroupID;							//���� I D

    //ͷ����Ϣ
    WORD							wFaceID;							//ͷ������
    DWORD							dwCustomID;							//�Զ���ʶ

    //�û�����
    BYTE							cbGender;							//�û��Ա�
    BYTE							cbMemberOrder;						//��Ա�ȼ�
    BYTE							cbMasterOrder;						//����ȼ�

    //�û�״̬
    WORD							wTableID;							//��������
    WORD							wChairID;							//��������
    BYTE							cbUserStatus;						//�û�״̬

    //������Ϣ
    SCORE							lScore;								//�û�����
    SCORE							lGrade;								//�û��ɼ�
    SCORE							lInsure;							//�û�����

    //��Ϸ��Ϣ
    DWORD							dwWinCount;							//ʤ������
    DWORD							dwLostCount;						//ʧ������
    DWORD							dwDrawCount;						//�;�����
    DWORD							dwFleeCount;						//��������
    DWORD							dwUserMedal;						//�û�����
    DWORD							dwExperience;						//�û�����
    LONG							lLoveLiness;						//�û�����
};

//ͷ����Ϣ
struct tagCustomFaceInfo {
    DWORD							dwDataSize;							//���ݴ�С
    DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//�û���Ϣ
struct tagUserRemoteInfo {
    //�û���Ϣ
    DWORD							dwUserID;							//�û���ʶ
    DWORD							dwGameID;							//��Ϸ��ʶ
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

    //�ȼ���Ϣ
    BYTE							cbGender;							//�û��Ա�
    BYTE							cbMemberOrder;						//��Ա�ȼ�
    BYTE							cbMasterOrder;						//����ȼ�

    //λ����Ϣ
    WORD							wKindID;							//���ͱ�ʶ
    WORD							wServerID;							//�����ʶ
    TCHAR							szGameServer[LEN_SERVER];			//����λ��
};

//////////////////////////////////////////////////////////////////////////////////

//�㳡����
struct tagGamePlaza {
    WORD							wPlazaID;							//�㳡��ʶ
    TCHAR							szServerAddr[32];					//�����ַ
    TCHAR							szServerName[32];					//��������
};

//��������
struct tagLevelItem {
    LONG							lLevelScore;						//�������
    TCHAR							szLevelName[16];					//��������
};

//��Ա����
struct tagMemberItem {
    BYTE							cbMemberOrder;						//�ȼ���ʶ
    TCHAR							szMemberName[16];					//�ȼ�����
};

//��������
struct tagMasterItem {
    BYTE							cbMasterOrder;						//�ȼ���ʶ
    TCHAR							szMasterName[16];					//�ȼ�����
};

//�б�����
struct tagColumnItem {
    BYTE							cbColumnWidth;						//�б���
    BYTE							cbDataDescribe;						//�ֶ�����
    TCHAR							szColumnName[16];					//�б�����
};

//��ַ��Ϣ
struct tagAddressInfo {
    TCHAR							szAddress[32];						//�����ַ
};

//������Ϣ
struct tagDataBaseParameter {
    WORD							wDataBasePort;						//���ݿ�˿�
    TCHAR							szDataBaseAddr[32];					//���ݿ��ַ
    TCHAR							szDataBaseUser[32];					//���ݿ��û�
    TCHAR							szDataBasePass[32];					//���ݿ�����
    TCHAR							szDataBaseName[32];					//���ݿ�����
};

//��������
struct tagServerOptionInfo {
    //�ҽ�����
    WORD							wKindID;							//�ҽ�����
    WORD							wNodeID;							//�ҽӽڵ�
    WORD							wSortID;							//���б�ʶ

    //˰������
    WORD							wRevenueRatio;						//˰�ձ���
    SCORE							lServiceScore;						//�������

    //��������
    SCORE							lRestrictScore;						//���ƻ���
    SCORE							lMinTableScore;						//��ͻ���
    SCORE							lMinEnterScore;						//��ͻ���
    SCORE							lMaxEnterScore;						//��߻���

    //��Ա����
    BYTE							cbMinEnterMember;					//��ͻ�Ա
    BYTE							cbMaxEnterMember;					//��߻�Ա

    //��������
    DWORD							dwServerRule;						//�������
    TCHAR							szServerName[LEN_SERVER];			//��������
};

//�û���Ϣ
struct tagMobileUserInfoHead {
    //�û�����
    DWORD							dwGameID;							//��Ϸ I D
    DWORD							dwUserID;							//�û� I D

    //ͷ����Ϣ
    WORD							wFaceID;							//ͷ������
    DWORD							dwCustomID;							//�Զ���ʶ

    //�û�����
    BYTE							cbGender;							//�û��Ա�
    BYTE							cbMemberOrder;						//��Ա�ȼ�

    //�û�״̬
    WORD							wTableID;							//��������
    WORD							wChairID;							//��������
    BYTE							cbUserStatus;						//�û�״̬

    //������Ϣ
    SCORE							lScore;								//�û�����

    //��Ϸ��Ϣ
    DWORD							dwWinCount;							//ʤ������
    DWORD							dwLostCount;						//ʧ������
    DWORD							dwDrawCount;						//�;�����
    DWORD							dwFleeCount;						//��������
    DWORD							dwExperience;						//�û�����
};
//////////////////////////////////////////////////////////////////////////////////
//������Ϣ
struct tagGameTaskInfoItem {
    tagGameTaskInfoItem() {

        wTaskID = 0;
        cbTaskType = 0;
        cbCondition = 0;
        wAmount = 0;
        cbAwardType = 0;
        lAwardCount = 0;
    }
    WORD							wTaskID;							//�����ʶ
    BYTE							cbTaskType;							//��������
    BYTE							cbCondition;						//��������
    WORD							wAmount;							//������������
    BYTE							cbAwardType;						//��������
    LONG							lAwardCount;						//��������
    WORD							wKindID;							//��������
};
typedef std::map<int, tagGameTaskInfoItem> Map_Int_GameTaskInfoItem;
typedef Map_Int_GameTaskInfoItem* pMap_Int_GameTaskInfoItem;

//��������
struct tagSendTaskInfoItem: public tagGameTaskInfoItem {
    WORD							wFinishCount;						//��ɴ���
};

//��������
struct tagGameTaskInfo {
    BYTE							cbTaskCount;						//������Ŀ
    tagGameTaskInfoItem				GameTaskInfo[MAX_TASK_COUNT];		//��������
};
#pragma pack()

#endif