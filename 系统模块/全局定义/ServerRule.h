#ifndef SERVER_RULE_HEAD_FILE
#define SERVER_RULE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//�������
#define SR_FORFEND_GAME_CHAT		0x00000001							//��ֹ����
#define SR_FORFEND_ROOM_CHAT		0x00000002							//��ֹ����
#define SR_FORFEND_WISPER_CHAT		0x00000004							//��ֹ˽��
#define SR_FORFEND_WISPER_ON_GAME	0x00000008							//��ֹ˽��

//�߼�����
#define SR_ALLOW_DYNAMIC_JOIN		0x00000010							//��̬����
#define SR_ALLOW_OFFLINE_TRUSTEE	0x00000020							//���ߴ���
#define SR_ALLOW_AVERT_CHEAT_MODE	0x00000040							//������Ϣ

//��Ϸ����
#define SR_RECORD_GAME_SCORE		0x00000100							//��¼����
#define SR_RECORD_GAME_TRACK		0x00000200							//��¼����
#define SR_DYNAMIC_CELL_SCORE		0x00000400							//��̬�׷�
#define SR_IMMEDIATE_WRITE_SCORE	0x00000800							//��ʱд��

//�������
#define SR_FORFEND_ROOM_ENTER		0x00001000							//��ֹ����
#define SR_FORFEND_GAME_ENTER		0x00002000							//��ֹ����
#define SR_FORFEND_GAME_LOOKON		0x00004000							//��ֹ�Թ�

//���й���
#define SR_FORFEND_TAKE_IN_ROOM		0x00010000							//��ֹȡ��
#define SR_FORFEND_TAKE_IN_GAME		0x00020000							//��ֹȡ��
#define SR_FORFEND_SAVE_IN_ROOM		0x00040000							//��ֹ��Ǯ
#define SR_FORFEND_SAVE_IN_GAME		0x00080000							//��ֹ���

//��������
#define SR_FORFEND_GAME_RULE		0x00100000							//��ֹ����
#define SR_FORFEND_LOCK_TABLE		0x00200000							//��ֹ����
#define SR_ALLOW_ANDROID_ATTEND		0x00400000							//��������
#define SR_ALLOW_ANDROID_SIMULATE	0x00800000							//����ռλ

//////////////////////////////////////////////////////////////////////////////////

//�������
class CServerRule
{
	//�������
public:
	//��ֹ����
	static bool IsForfendGameChat(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_CHAT)!=0; }
	//��ֹ����
	static bool IsForfendRoomChat(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_ROOM_CHAT)!=0; }
	//��ֹ˽��
	static bool IsForfendWisperChat(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_WISPER_CHAT)!=0; }
	//��ֹ˽��
	static bool IsForfendWisperOnGame(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_WISPER_ON_GAME)!=0; }

	//ģʽ����
public:
	//��̬����
	static bool IsAllowDynamicJoin(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_DYNAMIC_JOIN)!=0; }
	//���ߴ���
	//static bool IsAllowOffLineTrustee(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_OFFLINE_TRUSTEE) != 0; }
	//����
	static bool IsTestMakeCard(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_OFFLINE_TRUSTEE) != 0; }
	//������Ϣ
	static bool IsAllowAvertCheatMode(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_AVERT_CHEAT_MODE)!=0; }

	//��Ϸ����
public:
	//��¼����
	static bool IsRecordGameScore(DWORD dwServerRule) { return (dwServerRule&SR_RECORD_GAME_SCORE)!=0; }
	//��¼����
	static bool IsRecordGameTrack(DWORD dwServerRule) { return (dwServerRule&SR_RECORD_GAME_TRACK)!=0; }
	//��̬�׷�
	//static bool IsDynamicCellScore(DWORD dwServerRule) { return (dwServerRule&SR_DYNAMIC_CELL_SCORE) != 0; }
	static bool IsTestReplay(DWORD dwServerRule) { return (dwServerRule&SR_DYNAMIC_CELL_SCORE) != 0; }
	//��ʱд��
	static bool IsImmediateWriteScore(DWORD dwServerRule) { return (dwServerRule&SR_IMMEDIATE_WRITE_SCORE)!=0; }

	//�������
public:
	//��ֹ����
	static bool IsForfendRoomEnter(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_ROOM_ENTER)!=0; }
	//��ֹ����
	static bool IsForfendGameEnter(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_ENTER)!=0; }
	//��ֹ�Թ�
	static bool IsForfendGameLookon(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_LOOKON)!=0; }

	//���й���
public:
	//��ֹȡ��
	static bool IsForfendTakeInRoom(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_TAKE_IN_ROOM)!=0; }
	//��ֹȡ��
	static bool IsForfendTakeInGame(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_TAKE_IN_GAME)!=0; }
	//��ֹ��Ǯ
	static bool IsForfendSaveInRoom(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_SAVE_IN_ROOM)!=0; }
	//��ֹ��Ǯ
	static bool IsForfendSaveInGame(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_SAVE_IN_GAME)!=0; }

	//��������
public:
	//��ֹ����
	static bool IsForfendGameRule(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_RULE)!=0; }
	//��ֹ����
	static bool IsForfendLockTable(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_LOCK_TABLE)!=0; }
	//��������
	static bool IsAllowAndroidAttend(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_ANDROID_ATTEND)!=0; }
	//����ռλ
	static bool IsAllowAndroidSimulate(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_ANDROID_SIMULATE)!=0; }

	//�������
public:
	//��ֹ����
	static VOID SetForfendGameChat(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_CHAT:dwServerRule&=~SR_FORFEND_GAME_CHAT; }
	//��ֹ����
	static VOID SetForfendRoomChat(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_ROOM_CHAT:dwServerRule&=~SR_FORFEND_ROOM_CHAT; }
	//��ֹ˽��
	static VOID SetForfendWisperChat(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_WISPER_CHAT:dwServerRule&=~SR_FORFEND_WISPER_CHAT; }
	//��ֹ˽��
	static VOID SetForfendWisperOnGame(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_WISPER_ON_GAME:dwServerRule&=~SR_FORFEND_WISPER_ON_GAME; }

	//ģʽ����
public:
	//��̬����
	static VOID SetAllowDynamicJoin(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_DYNAMIC_JOIN:dwServerRule&=~SR_ALLOW_DYNAMIC_JOIN; }
	//���ߴ���
	static VOID SetAllowOffLineTrustee(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_OFFLINE_TRUSTEE:dwServerRule&=~SR_ALLOW_OFFLINE_TRUSTEE; }
	//������Ϣ
	static VOID SetAllowAvertCheatMode(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_AVERT_CHEAT_MODE:dwServerRule&=~SR_ALLOW_AVERT_CHEAT_MODE; }

	//��Ϸ����
public:
	//��¼����
	static VOID SetRecordGameScore(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_RECORD_GAME_SCORE:dwServerRule&=~SR_RECORD_GAME_SCORE; }
	//��¼����
	static VOID SetRecordGameTrack(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_RECORD_GAME_TRACK:dwServerRule&=~SR_RECORD_GAME_TRACK; }
	//��̬�׷�
	static VOID SetDynamicCellScore(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_DYNAMIC_CELL_SCORE:dwServerRule&=~SR_DYNAMIC_CELL_SCORE; }
	//��ʱд��
	static VOID SetImmediateWriteScore(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_IMMEDIATE_WRITE_SCORE:dwServerRule&=~SR_IMMEDIATE_WRITE_SCORE; }

	//�������
public:
	//��ֹ����
	static VOID SetForfendRoomEnter(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_ROOM_ENTER:dwServerRule&=~SR_FORFEND_ROOM_ENTER; }
	//��ֹ����
	static VOID SetForfendGameEnter(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_ENTER:dwServerRule&=~SR_FORFEND_GAME_ENTER; }
	//��ֹ�Թ�
	static VOID SetForfendGameLookon(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_LOOKON:dwServerRule&=~SR_FORFEND_GAME_LOOKON; }

	//���й���
public:
	//��ֹȡ��
	static VOID SetForfendTakeInRoom(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_TAKE_IN_ROOM:dwServerRule&=~SR_FORFEND_TAKE_IN_ROOM; }
	//��ֹȡ��
	static VOID SetForfendTakeInGame(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_TAKE_IN_GAME:dwServerRule&=~SR_FORFEND_TAKE_IN_GAME; }
	//��ֹ��Ǯ
	static VOID SetForfendSaveInRoom(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_SAVE_IN_ROOM:dwServerRule&=~SR_FORFEND_SAVE_IN_ROOM; }
	//��ֹ��Ǯ
	static VOID SetForfendSaveInGame(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_SAVE_IN_GAME:dwServerRule&=~SR_FORFEND_SAVE_IN_GAME; }

	//��������
public:
	//��ֹ����
	static VOID SetForfendGameRule(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_RULE:dwServerRule&=~SR_FORFEND_GAME_RULE; }
	//��ֹ����
	static VOID SetForfendLockTable(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_LOCK_TABLE:dwServerRule&=~SR_FORFEND_LOCK_TABLE; }
	//��������
	static VOID SetAllowAndroidAttend(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_ANDROID_ATTEND:dwServerRule&=~SR_ALLOW_ANDROID_ATTEND; }
	//����ռλ
	static VOID SetAllowAndroidSimulate(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_ANDROID_SIMULATE:dwServerRule&=~SR_ALLOW_ANDROID_SIMULATE; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif