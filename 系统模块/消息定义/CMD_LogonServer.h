#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID			1									//I D ��¼
#define SUB_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_MB_WX_LOGON_FIRST       4                                   //΢�ŵ�һ�ε�¼
#define SUB_MB_WX_LOGON_SECOND      5                                   //΢�ź��ڵ�¼
#define SUB_MB_LOGON_VISITOR        6                                   //�ο͵�¼
#define SUB_MB_LOGON_WX_OPENID      7                                   //΢��openID��¼
#define SUB_MB_LOGON_WX_GAME        8
#define SUB_MB_JOIN_GAME            9                                   //������Ϸ
#define SUB_MB_LOGON_WX_TEMP        10


//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�

   

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_MB_JOIN_GAME_SUCCESS    102                                 //������Ϸ�ɹ�
#define SUB_MB_JOIN_GAME_FAILURE    103                                 //������Ϸʧ��


//��¼�ɹ�
struct CMD_MB_LogonSuccess {
    WORD							wFaceID;							//ͷ���ʶ
    BYTE							cbGender;							//�û��Ա�
    DWORD							dwUserID;							//�û� I D
    DWORD							dwGameID;							//��Ϸ I D
    DWORD							dwExperience;						//������ֵ
    DWORD							dwLoveLiness;						//�û�����
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��¼ʧ��
struct CMD_MB_LogonFailure {
    LONG							lResultCode;						//�������
    TCHAR							szDescribeString[128];				//������Ϣ
};


#define MDM_GF_GAME					200									//��Ϸ����

#define SUB_MB_MOVE                 1                                   //�ƶ�
#define SUB_MB_SHOOT                2	                                //���
#define SUB_MB_WORLD_STATE          3
#define SUB_MB_GAME_END             4                                   //��Ϸ����
#define SUB_MB_RANK                 5                                   //����
#define SUB_MB_DESTROY_BULLET       6             
#define SUB_MB_NEW_ENTITY           7                                  
#define SUB_MB_DESTROY_ENTITY       8      
#define SUB_MB_CALCULATE_RESULT     9
#define SUB_MB_JUMP 10
#define SUB_MB_GAME_SCENE 11
#pragma pack()

#endif