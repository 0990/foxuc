#ifndef RIGHT_DEFINE_HEAD_FILE
#define RIGHT_DEFINE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//�û�Ȩ��
#define UR_CANNOT_PLAY					0x00000001L				//���ܽ�����Ϸ
#define UR_CANNOT_LOOKON				0x00000002L				//�����Թ���Ϸ
#define UR_CANNOT_WISPER				0x00000004L				//���ܷ���˽��
#define UR_CANNOT_ROOM_CHAT				0x00000008L				//���ܴ�������
#define UR_CANNOT_GAME_CHAT				0x00000010L				//������Ϸ����
#define UR_CANNOT_BUGLE					0x00000020L				//���ܷ�������

//��ԱȨ��
#define UR_GAME_DOUBLE_SCORE			0x00000100L				//��Ϸ˫������
#define UR_GAME_KICK_OUT_USER			0x00000200L				//��Ϸ�߳��û�
#define UR_GAME_ENTER_VIP_ROOM          0x00000400L             //����VIP���� 

//�û����
#define UR_GAME_MATCH_USER				0x10000000L				//��Ϸ�����û�
#define UR_GAME_CHEAT_USER				0x20000000L				//��Ϸ�����û�

//////////////////////////////////////////////////////////////////////////////////

//��ͨ����
#define UR_CAN_LIMIT_PLAY				0x00000001L				//�����ֹ��Ϸ
#define UR_CAN_LIMIT_LOOKON				0x00000002L				//�����ֹ�Թ�
#define UR_CAN_LIMIT_WISPER				0x00000004L				//�����ֹ˽��
#define UR_CAN_LIMIT_ROOM_CHAT			0x00000008L				//�����ֹ����
#define UR_CAN_LIMIT_GAME_CHAT			0x00000010L				//�����ֹ����

//�û�����
#define UR_CAN_KILL_USER				0x00000100L				//�����߳��û�
#define UR_CAN_SEE_USER_IP				0x00000200L				//����鿴��ַ
#define UR_CAN_DISMISS_GAME				0x00000400L				//�����ɢ��Ϸ
#define UR_CAN_LIMIT_USER_CHAT			0x00000800L				//�����ֹ�������

//�߼�����
#define UR_CAN_CONFINE_IP				0x00001000L				//�����ֹ��ַ
#define UR_CAN_CONFINE_MAC				0x00002000L				//�����ֹ����
#define UR_CAN_SEND_WARNING				0x00004000L				//�����;���
#define UR_CAN_MODIFY_SCORE				0x00008000L				//�����޸Ļ���
#define UR_CAN_FORBID_ACCOUNTS			0x00010000L				//��������ʺ�

//�󶨹���
#define UR_CAN_BIND_GAME				0x00100000L				//������Ϸ��
#define UR_CAN_BIND_GLOBAL				0x00200000L				//����ȫ�ְ�

//���ù���
#define UR_CAN_ISSUE_MESSAGE			0x01000000L				//��������Ϣ
#define UR_CAN_MANAGER_SERVER			0x02000000L				//���������
#define UR_CAN_MANAGER_OPTION			0x04000000L				//�����������
#define UR_CAN_MANAGER_ANDROID			0x08000000L				//����������

//////////////////////////////////////////////////////////////////////////////////

//�û�Ȩ��
class CUserRight
{
	//���Ȩ��
public:
	//��ϷȨ��
	static bool CanPlay(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_PLAY)==0; }
	//�Թ�Ȩ��
	static bool CanLookon(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_LOOKON)==0; }
	//˽��Ȩ��
	static bool CanWisper(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_WISPER)==0; }
	//��������
	static bool CanRoomChat(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_ROOM_CHAT)==0; }
	//��Ϸ����
	static bool CanGameChat(DWORD dwUserRight) { return (dwUserRight&UR_CANNOT_GAME_CHAT)==0; }
	//����VIP��
	static bool CanEnterVipRoom(DWORD dwUserRight) { return (dwUserRight&UR_GAME_ENTER_VIP_ROOM)==0; }

	//��ԱȨ��
public:
	//˫������
	static bool CanDoubleScore(DWORD dwUserRight) { return (dwUserRight&UR_GAME_DOUBLE_SCORE)!=0; }
	//�߳��û�
	static bool CanKillOutUser(DWORD dwUserRight) { return (dwUserRight&UR_GAME_KICK_OUT_USER)!=0; }

	//����Ȩ��
public:
	//�����û�
	static bool IsGameMatchUser(DWORD dwUserRight) { return (dwUserRight&UR_GAME_MATCH_USER)!=0; }
	//�����û�
	static bool IsGameCheatUser(DWORD dwUserRight) { return (dwUserRight&UR_GAME_CHEAT_USER)!=0; }
};

//////////////////////////////////////////////////////////////////////////////////

//����Ȩ��
class CMasterRight
{
	//��ͨ����
public:
	//��ֹ��Ϸ
	static bool CanLimitPlay(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_PLAY)!=0); }
	//��ֹ�Թ�
	static bool CanLimitLookon(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_LOOKON)!=0); }
	//��ֹ˽��
	static bool CanLimitWisper(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_WISPER)!=0); }
	//��ֹ����
	static bool CanLimitRoomChat(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_ROOM_CHAT)!=0); }
	//��ֹ����
	static bool CanLimitGameChat(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_GAME_CHAT)!=0); }

	//�û�����
public:
	//�߳��û�
	static bool CanKillUser(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_KILL_USER)!=0); }
	//�鿴��ַ
	static bool CanSeeUserIP(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_SEE_USER_IP)!=0); }
	//��ɢ��Ϸ
	static bool CanDismissGame(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_DISMISS_GAME)!=0); }
	//��ֹ�������
	static bool CanLimitUserChat(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_LIMIT_USER_CHAT)!=0); }

	//�߼�����
public:
	//��ֹ��ַ
	static bool CanConfineIP(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_CONFINE_IP)!=0); }
	//��ֹ����
	static bool CanConfineMac(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_CONFINE_MAC)!=0); }
	//���;���
	static bool CanSendWarning(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_SEND_WARNING)!=0); }
	//�޸Ļ���
	static bool CanModifyScore(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_MODIFY_SCORE)!=0); }
	//�����ʺ�
	static bool CanForbidAccounts(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_FORBID_ACCOUNTS)!=0); }

	//�󶨹���
public:
	//��Ϸ��
	static bool CanBindGame(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_BIND_GAME)!=0); }
	//ȫ�ְ�
	static bool CanBindGlobal(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_BIND_GLOBAL)!=0); }

	//���ù���
public:
	//������Ϣ
	static bool CanIssueMessage(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_ISSUE_MESSAGE)!=0); }
	//������
	static bool CanManagerServer(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_MANAGER_SERVER)!=0); }
	//�������
	static bool CanManagerAndroid(DWORD dwMasterRight) { return ((dwMasterRight&UR_CAN_MANAGER_ANDROID)!=0); }
};

//////////////////////////////////////////////////////////////////////////////////

#endif
