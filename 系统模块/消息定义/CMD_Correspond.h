#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//ע������

#define MDM_CS_REGISTER				1									//����ע��

//����ע��
#define SUB_CS_C_REGISTER_PLAZA		100									//ע��㳡
#define SUB_CS_C_REGISTER_SERVER	101									//ע�᷿��

//ע����
#define SUB_CS_S_REGISTER_FAILURE	200									//ע��ʧ��

//////////////////////////////////////////////////////////////////////////////////

//ע��㳡
struct CMD_CS_C_RegisterPlaza {
    TCHAR							szServerAddr[32];					//�����ַ
    TCHAR							szServerName[LEN_SERVER];			//��������
};

//ע����Ϸ
struct CMD_CS_C_RegisterServer {
    WORD							wKindID;							//��������
    WORD							wNodeID;							//�ڵ�����
    WORD							wSortID;							//��������
    WORD							wServerID;							//��������
    WORD							wServerPort;						//����˿�
    WORD                            wServerType;                        //��������
    DWORD							dwOnLineCount;						//��������
    DWORD							dwFullCount;						//��Ա����
    TCHAR							szServerAddr[32];					//�����ַ
    TCHAR							szServerName[LEN_SERVER];			//��������
	bool							bAcceptNew;							//�Ƿ�����·���
};

//ע��ʧ��
struct CMD_CS_S_RegisterFailure {
    LONG							lErrorCode;							//�������
    TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ

#define MDM_CS_SERVICE_INFO			2									//������Ϣ

//������Ϣ
#define SUB_CS_C_SERVER_ONLINE		1									//��������
#define SUB_CS_C_SERVER_MODIFY		2									//�����޸�

//�㳡����
#define SUB_CS_S_PLAZA_INFO			100									//�㳡��Ϣ
#define SUB_CS_S_PLAZA_INSERT		101									//�㳡�б�
#define SUB_CS_S_PLAZA_REMOVE		103									//�㳡ɾ��
#define SUB_CS_S_PLAZA_FINISH		104									//�㳡���

//��������
#define SUB_CS_S_SERVER_INFO		110									//������Ϣ
#define SUB_CS_S_SERVER_ONLINE		111									//��������
#define SUB_CS_S_SERVER_INSERT		112									//�����б�
#define SUB_CS_S_SERVER_MODIFY		113									//�����޸�
#define SUB_CS_S_SERVER_REMOVE		114									//����ɾ��
#define SUB_CS_S_SERVER_FINISH		115									//�������

#define SUB_CS_S_TASK_FRESH         120                                 //5������ˢ����Ϣ

//////////////////////////////////////////////////////////////////////////////////
struct CMD_CS_S_TaskFresh{
	time_t tTaskFreshTime;
};
//��������
struct CMD_CS_C_ServerModify {
    WORD							wSortID;							//��������
    WORD							wKindID;							//��������
    WORD							wNodeID;							//�ڵ�����
    WORD							wServerPort;						//����˿�
    DWORD							dwOnLineCount;						//��������
    DWORD							dwFullCount;						//��Ա����
    TCHAR							szServerAddr[32];					//�����ַ
    TCHAR							szServerName[LEN_SERVER];			//��������
	bool							bAcceptNew;							//�Ƿ�����·���
};

//��������
struct CMD_CS_C_ServerOnLine {
    DWORD							dwOnLineCount;						//��������
};

//////////////////////////////////////////////////////////////////////////////////

//�㳡ɾ��
struct CMD_CS_S_PlazaRemove {
    DWORD							dwPlazaID;							//�㳡��ʶ
};

//��������
struct CMD_CS_S_ServerOnLine {
    WORD							wServerID;							//�����ʶ
    DWORD							dwOnLineCount;						//��������
};

//�����޸�
struct CMD_CS_S_ServerModify {
    WORD							wSortID;							//��������
    WORD							wKindID;							//��������
    WORD							wNodeID;							//�ڵ�����
    WORD							wServerID;							//�����ʶ
    WORD							wServerPort;						//����˿�
    DWORD							dwOnLineCount;						//��������
    DWORD							dwFullCount;						//��Ա����
    TCHAR							szServerAddr[32];					//�����ַ
    TCHAR							szServerName[LEN_SERVER];			//��������
	bool							bAcceptNew;							//�Ƿ�����·���

};

//����ɾ��
struct CMD_CS_S_ServerRemove {
    WORD							wServerID;							//�����ʶ
};

//////////////////////////////////////////////////////////////////////////////////
//�û�����

#define MDM_CS_USER_COLLECT			3									//�û�����

//�û�״̬
#define SUB_CS_C_USER_ENTER			1									//�û�����
#define SUB_CS_C_USER_LEAVE			2									//�û��뿪
#define SUB_CS_C_USER_FINISH		3									//�û����

//�û�״̬
#define SUB_CS_S_COLLECT_REQUEST	100									//��������

//////////////////////////////////////////////////////////////////////////////////

//�û�����
struct CMD_CS_C_UserEnter {
    //�û���Ϣ
    DWORD							dwUserID;							//�û���ʶ
    DWORD							dwGameID;							//��Ϸ��ʶ
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

    //������Ϣ
    BYTE							cbGender;							//�û��Ա�
    BYTE							cbMemberOrder;						//��Ա�ȼ�
    BYTE							cbMasterOrder;						//����ȼ�
};

//�û��뿪
struct CMD_CS_C_UserLeave {
    DWORD							dwUserID;							//�û���ʶ
};

//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���

#define MDM_CS_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_CS_C_SEARCH_DATABASE	1									//���ݲ���
#define SUB_CS_C_SEARCH_CORRESPOND	2									//Э������

//���ҽ��
#define SUB_CS_S_SEARCH_DATABASE	100									//���ݲ���
#define SUB_CS_S_SEARCH_CORRESPOND	101									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_CS_C_SearchCorrespond {
    //���Ӳ���
    DWORD							dwSocketID;							//�����ʶ
    DWORD							dwClientAddr;						//���ӵ�ַ

    //��������
    DWORD							dwGameID;							//��Ϸ��ʶ
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_CS_S_SearchCorrespond {
    //������Ϣ
    DWORD							dwSocketID;							//�����ʶ
    DWORD							dwClientAddr;						//���ӵ�ַ

    //������Ϣ
    WORD							wUserCount;							//�û���Ŀ
    tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//�������

#define MDM_CS_MANAGER_SERVICE		5									//�������

#define SUB_CS_C_SYSTEM_MESSAGE 	1									//ϵͳ��Ϣ
#define SUB_CS_C_PROPERTY_TRUMPET 	2									//������Ϣ

#define SUB_CS_S_SYSTEM_MESSAGE 	100									//ϵͳ��Ϣ
#define SUB_CS_S_PROPERTY_TRUMPET  	200									//������Ϣ


//��������
struct CMD_CS_S_SendTrumpet {
    WORD                           wPropertyIndex;                      //��������
    DWORD                          dwSendUserID;                         //�û� I D
    DWORD                          TrumpetColor;                        //������ɫ
    TCHAR                          szSendNickName[32];				    //����ǳ�
    TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};
//////////////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif