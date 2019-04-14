#ifndef DEFINE_HEAD_FILE
#define DEFINE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//��ֵ����

//ͷ���С
#define FACE_CX						48									//ͷ����
#define FACE_CY						48									//ͷ��߶�

//���ȶ���
#define LEN_LESS_ACCOUNTS			6									//����ʺ�
#define LEN_LESS_NICKNAME			6									//����ǳ�
#define LEN_LESS_PASSWORD			6									//�������

//��������
#define MAX_CHAIR					100									//�������
#define MAX_TABLE					512									//�������
#define MAX_COLUMN					32									//����б�
#define MAX_ANDROID					256									//������
#define MAX_PROPERTY				128									//������
#define MAX_WHISPER_USER			16									//���˽��
#define MAX_TASK_COUNT				100									//���������
#define MAX_RECORD_COUNT			10									//��Ϸ��¼�������
#define MAX_RECORD_BYTES            20000                               //��Ϸ��¼����ֽ���
#define MIN_RECORD_BYTES            8000                                //��Ϸ��¼��С�ֽ���

//�б���
#define MAX_KIND					128									//�������
#define MAX_SERVER					1024								//��󷿼�

//��������
#define INVALID_CHAIR				0xFFFF								//��Ч����
#define INVALID_TABLE				0xFFFF								//��Ч����

//˰�ն���
#define REVENUE_BENCHMARK			0L								    //˰�����
#define REVENUE_DENOMINATOR			1000L								//˰�շ�ĸ

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

//��������
#define SCORE						LONGLONG							//��������
#define SCORE_STRING				TEXT("%I64d")						//��������

//��Ϸ״̬
#define GAME_STATUS_FREE			0									//����״̬
#define GAME_STATUS_PLAY			100									//��Ϸ״̬
#define GAME_STATUS_WAIT			200									//�ȴ�״̬

//ϵͳ����
#define LEN_USER_CHAT				128									//���쳤��
#define TIME_USER_CHAT				1L									//������
#define TRUMPET_MAX_CHAR            128									//���ȳ���

//////////////////////////////////////////////////////////////////////////////////
//��������

//�б�����
#define PRIME_TYPE					11L									//������Ŀ
#define PRIME_KIND					53L									//������Ŀ
#define PRIME_NODE					101L								//�ڵ���Ŀ
#define PRIME_PAGE					53L									//�Զ���Ŀ
#define PRIME_SERVER				1009L								//������Ŀ

//��������
#define PRIME_SERVER_USER			503L								//��������
#define PRIME_ANDROID_USER			503L								//��������
#define PRIME_PLATFORM_USER			100003L								//ƽ̨����

//////////////////////////////////////////////////////////////////////////////////
//���ݳ���

//��������
#define LEN_MD5						33									//��������
#define LEN_USERNOTE				32									//��ע����
#define LEN_ACCOUNTS				32									//�ʺų���
#define LEN_NICKNAME				32									//�ǳƳ���
#define LEN_PASSWORD				33									//���볤��
#define LEN_GROUP_NAME				32									//��������
#define LEN_UNDER_WRITE				32									//����ǩ��

//���ݳ���
#define LEN_QQ						16									//Q Q ����
#define LEN_EMAIL					33									//�����ʼ�
#define LEN_USER_NOTE				256									//�û���ע
#define LEN_SEAT_PHONE				33									//�̶��绰
#define LEN_MOBILE_PHONE			12									//�ƶ��绰
#define LEN_PASS_PORT_ID			19									//֤������
#define LEN_COMPELLATION			16									//��ʵ����
#define LEN_DWELLING_PLACE			128									//��ϵ��ַ

#define LEN_HEAD_IMG_URL            256

//������ʶ
#define LEN_NETWORK_ID				13									//��������
#define LEN_MACHINE_ID				33									//���г���

//�б�����
#define LEN_TYPE					32									//���೤��
#define LEN_KIND					32									//���ͳ���
#define LEN_NODE					32									//�ڵ㳤��
#define LEN_PAGE					32									//���Ƴ���
#define LEN_SERVER					32									//���䳤��
#define LEN_PROCESS					32									//���̳���

//////////////////////////////////////////////////////////////////////////////////

//�û���ϵ
#define	CP_NORMAL					0									//δ֪��ϵ
#define	CP_FRIEND					1									//���ѹ�ϵ
#define	CP_DETEST					2									//����ϵ
#define CP_SHIELD					3									//��������

//////////////////////////////////////////////////////////////////////////////////

//�Ա���
#define GENDER_FEMALE				2									//Ů���Ա�
#define GENDER_MALE					1									//�����Ա�

//////////////////////////////////////////////////////////////////////////////////

//��Ϸģʽ
#define GAME_GENRE_GOLD				0x0001								//�������
#define GAME_GENRE_SCORE			0x0002								//��ֵ����
#define GAME_GENRE_MATCH			0x0004								//��������
#define GAME_GENRE_EDUCATE			0x0008								//ѵ������
#define GAME_GENRE_PRIVATE          0x0010                              //��������

//����ģʽ
#define SCORE_GENRE_NORMAL			0x0100								//��ͨģʽ
#define SCORE_GENRE_POSITIVE		0x0200								//�Ǹ�ģʽ

//////////////////////////////////////////////////////////////////////////////////

//�û�״̬
#define US_NULL						0x00								//û��״̬
#define US_FREE						0x01								//վ��״̬
#define US_SIT						0x02								//����״̬
#define US_READY					0x03								//ͬ��״̬
#define US_LOOKON					0x04								//�Թ�״̬
#define US_PLAYING					0x05								//��Ϸ״̬
#define US_OFFLINE					0x06								//����״̬

//�Թ�״̬
#define LOOKON_NULL                 0//���Թ�״̬
#define LOOKON_ONLY_ME                  1//ֻ�ܿ����ҵ���
#define LOOKON_ALL                      2//�ܿ�������ҵ���

//////////////////////////////////////////////////////////////////////////////////

//����״̬
#define MS_NULL						0x00								//û��״̬
#define MS_SIGNUP					0x01								//����״̬
#define MS_HAVE_SIGNED				0x02								//�ѱ���״̬
#define MS_MATCHING					0x03								//����״̬
#define MS_OUT						0x04								//��̭״̬

//////////////////////////////////////////////////////////////////////////////////

//�������
#define SRL_LOOKON					0x00000001							//�Թ۱�־
#define SRL_OFFLINE					0x00000002							//���߱�־
#define SRL_SAME_IP					0x00000004							//ͬ����־

//�������
#define SRL_ROOM_CHAT				0x00000100							//�����־
#define SRL_GAME_CHAT				0x00000200							//�����־
#define SRL_WISPER_CHAT				0x00000400							//˽�ı�־
#define SRL_HIDE_USER_INFO			0x00000800							//���ر�־

//////////////////////////////////////////////////////////////////////////////////
//�б�����

//��Ч����
#define UD_NULL						0									//��Ч����
#define UD_IMAGE					100									//ͼ������
#define UD_CUSTOM					200									//�Զ�����

//��������
#define UD_GAME_ID					1									//��Ϸ��ʶ
#define UD_USER_ID					2									//�û���ʶ
#define	UD_NICKNAME					3									//�û��ǳ�

//��չ����
#define UD_GENDER					10									//�û��Ա�
#define UD_GROUP_NAME				11									//��������
#define UD_UNDER_WRITE				12									//����ǩ��

//״̬��Ϣ
#define UD_TABLE					20									//��Ϸ����
#define UD_CHAIR					21									//���Ӻ���

//������Ϣ
#define UD_SCORE					30									//�û�����
#define UD_GRADE					31									//�û��ɼ�
#define UD_USER_MEDAL				32									//�û�����
#define UD_EXPERIENCE				33									//�û�����
#define UD_LOVELINESS				34									//�û�����
#define UD_WIN_COUNT				35									//ʤ������
#define UD_LOST_COUNT				36									//�������
#define UD_DRAW_COUNT				37									//�;�����
#define UD_FLEE_COUNT				38									//�Ӿ�����
#define UD_PLAY_COUNT				39									//�ܾ�����

//���ֱ���
#define UD_WIN_RATE					40									//�û�ʤ��
#define UD_LOST_RATE				41									//�û�����
#define UD_DRAW_RATE				42									//�û�����
#define UD_FLEE_RATE				43									//�û�����
#define UD_GAME_LEVEL				44									//��Ϸ�ȼ�

//��չ��Ϣ
#define UD_NOTE_INFO				50									//�û���ע
#define UD_LOOKON_USER				51									//�Թ��û�

//ͼ���б�
#define UD_IMAGE_FLAG				(UD_IMAGE+1)						//�û���־
#define UD_IMAGE_GENDER				(UD_IMAGE+2)						//�û��Ա�
#define UD_IMAGE_STATUS				(UD_IMAGE+3)						//�û�״̬

//////////////////////////////////////////////////////////////////////////////////
//���ݿⶨ��

#define DB_ERROR 					-1  								//����ʧ��
#define DB_SUCCESS 					0  									//����ɹ�
#define DB_NEEDMB 					18 									//����ʧ��

//////////////////////////////////////////////////////////////////////////////////
//���߱�ʾ
#define PT_USE_MARK_DOUBLE_SCORE    0x0001								//˫������
#define PT_USE_MARK_FOURE_SCORE     0x0002								//�ı�����
#define PT_USE_MARK_GUARDKICK_CARD  0x0010								//���ߵ���
#define PT_USE_MARK_POSSESS         0x0020								//������� 

#define MAX_PT_MARK                 4                                   //��ʶ��Ŀ

//��Ч��Χ
#define VALID_TIME_DOUBLE_SCORE     3600                                //��Чʱ��
#define VALID_TIME_FOUR_SCORE       3600                                //��Чʱ��
#define VALID_TIME_GUARDKICK_CARD   3600                                //����ʱ��
#define VALID_TIME_POSSESS          3600                                //����ʱ��
#define VALID_TIME_KICK_BY_MANAGER  3600                                //��Ϸʱ��

//��������
#define TASK_TYPE_NULL				0									//��Ч����
#define TASK_TYPE_DAILY				1									//ÿ������
#define TASK_TYPE_REPEAT			2								//�ظ�����

//��������
#define TASK_COND_OTHER				0									//������
#define TASK_COND_PLAY				1									//��Ϸ����
#define TASK_COND_WIN				2									//��ϷӮ��

//��������
#define AWARD_TYPE_NULL				0									//��Ч����
#define AWARD_TYPE_GOLD				1									//��ҽ���
#define AWARD_TYPE_EXPR				2									//���齱��
#define ARARD_TYPE_MEDAL			3									//���ƽ���
#define AWARD_TYPE_PROP				4									//���߽���
#define AWARD_TYPE_FRAGMENT         5                                   //������Ƭ

//����;��
#define AWARD_WAY_NULL				0									//��Ч;��
#define AWARD_WAY_MATCH				1									//��������
#define AWARD_WAY_TASK				2									//������

#define TASKID_MING_GANG              1
#define TASKID_AN_GANG                2
#define TASKID_MANGUAN_ONE            3
#define TASKID_MANGUAN_THREE          4
#define TASKID_WIN_THREE			  5
#define TASKID_QIANG_GANG_HU          6
#define TASKID_OPEN_ROOM_ONE          20
#define TASKID_OPEN_ROOM_THREE        21
#define TASKID_SIGN_IN                50
#define TASKID_WX_SHARE               51
#define TASKID_FINISH_ALL             99

#define TASKID_NINECARDS_KILL_ALL     30 //ͨ��
#define TASKID_NINECARDS_THREE_SAME   31 //�������õ���һ��������ͬ��
#define TASKID_NINECARDS_ONE_LUCK     32 //���������һ�γ�ϲ
#define TASKID_NINECARDS_TEN_LUCK	  33 //ʮ�γ�ϲ
#define TASKID_NINECARDS_FOUR_SAME    34 //�������õ���һ��������ͬ��
#define TASKID_NINECARDS_COLOR_SAME   35 //�������õ���ȫ���ȫ������
#define TASKID_NINECARDS_FIRST_STRAIGHT_FLUSH 38 //��������ɵ�һ��˳��һ��
//////////////////////////////////////////////////////////////////////////////////
//�豸����
#define DEVICE_TYPE_PC              0x00                                //PC
#define DEVICE_TYPE_ANDROID         0x10                                //Android
#define DEVICE_TYPE_ITOUCH          0x20                                //iTouch
#define DEVICE_TYPE_IPHONE          0x40                                //iPhone
#define DEVICE_TYPE_IPAD            0x80                                //iPad

#define PRIVATE_CREATE_WAY_ROOM_OWNER 0
#define PRIVATE_CREATE_WAY_AA         1
#define PRIVATE_CREATE_WAY_FOR_OTHER  2


#define OVER_REASON_NULL 0 //��Ч
#define OVER_REASON_NORMAL 1 //�������
#define OVER_REASON_DB_DISMISS 2
#define OVER_REASON_SERVER_DISMISS 3
#define OVER_REASON_PLAYER_DISMISS 4
/////////////////////////////////////////////////////////////////////////////////
//�ֻ�����

//��ͼģʽ
//#define	VIEW_MODE_ALL				0x0001								//ȫ������
//#define	VIEW_MODE_PART				0x0002								//���ֿ���

//��Ϣģʽ
//#define VIEW_INFO_LEVEL_1			0x0010								//������Ϣ
//#define VIEW_INFO_LEVEL_2			0x0020								//������Ϣ
//#define VIEW_INFO_LEVEL_3			0x0040								//������Ϣ
//#define VIEW_INFO_LEVEL_4			0x0080								//������Ϣ

//��������
#define RECVICE_GAME_CHAT			0x0100								//��������
#define RECVICE_ROOM_CHAT			0x0200								//��������
#define RECVICE_ROOM_WHISPER		0x0400								//����˽��

//��Ϊ��ʶ
#define BEHAVIOR_LOGON_NORMAL       0x0000                              //��ͨ��¼
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000                              //������¼

/////////////////////////////////////////////////////////////////////////////////
//������
#define RESULT_ERROR 					-1  								//�������
#define RESULT_SUCCESS 					0  									//����ɹ�
#define RESULT_FAIL 					1  									//����ʧ��

/////////////////////////////////////////////////////////////////////////////////
//�仯ԭ��
#define SCORE_REASON_WRITE              0                                   //д�ֱ仯
#define SCORE_REASON_INSURE             1                                   //���б仯
#define SCORE_REASON_PROPERTY           2                                   //���߱仯
#define SCORE_REASON_MATCH_FEE          3                                   //��������
#define SCORE_REASON_MATCH_QUIT         4                                   //��������

/////////////////////////////////////////////////////////////////////////////////

//��¼����ʧ��ԭ��
#define LOGON_FAIL_SERVER_INVALIDATION  200                                 //����ʧЧ

////////////////////////////////////////////////////////////////////////////////
#endif