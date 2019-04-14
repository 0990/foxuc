#ifndef CMD_COMMOM_HEAD_FILE
#define CMD_COMMOM_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

#define MDM_CM_SYSTEM				1000								//ϵͳ����

#define SUB_CM_SYSTEM_MESSAGE		1									//ϵͳ��Ϣ
#define SUB_CM_ACTION_MESSAGE		2									//������Ϣ
#define SUB_CM_DOWN_LOAD_MODULE		3									//������Ϣ

//////////////////////////////////////////////////////////////////////////////////

//��������
#define SMT_CHAT					0x0001								//������Ϣ
#define SMT_EJECT					0x0002								//������Ϣ
#define SMT_GLOBAL					0x0004								//ȫ����Ϣ
#define SMT_PROMPT					0x0008								//��ʾ��Ϣ
#define SMT_TABLE_ROLL				0x0010								//������Ϣ

//��������
#define SMT_CLOSE_ROOM				0x0100								//�رշ���
#define SMT_CLOSE_GAME				0x0200								//�ر���Ϸ
#define SMT_CLOSE_LINK				0x0400								//�ж�����

//ϵͳ��Ϣ
struct CMD_CM_SystemMessage
{
	WORD							wType;								//��Ϣ����
	WORD							wLength;							//��Ϣ����
	TCHAR							szString[1024];						//��Ϣ����
};
//ϵͳ��Ϣ
struct CMD_GR_SendMessage {
	BYTE							cbGame;								//��Ϸ��Ϣ
	BYTE							cbRoom;								//��Ϸ��Ϣ
	BYTE							cbAllRoom;							//��Ϸ��Ϣ
	BYTE                            cbLoop;                             //ѭ����־
	DWORD                           dwTimeRate;                         //ѭ�����
	__time64_t                      tConcludeTime;                      //����ʱ��
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};
//////////////////////////////////////////////////////////////////////////////////

//��������
#define ACT_BROWSE					1									//�������
#define ACT_DOWN_LOAD				2									//���ض���

//������Ϣ
struct tagActionHead
{
	UINT							uResponseID;						//��Ӧ��ʶ
	WORD							wAppendSize;						//���Ӵ�С
	BYTE							cbActionType;						//��������
};

//�������
#define BRT_IE						0x01								//I E ���
#define BRT_PLAZA					0x02								//�������
#define BRT_WINDOWS					0x04								//�������

//�������
struct tagActionBrowse
{
	BYTE							cbBrowseType;						//�������
	TCHAR							szBrowseUrl[256];					//�����ַ
};

//��������
#define DLT_IE						1									//I E ����
#define DLT_MODULE					2									//����ģ��

//���ض���
struct tagActionDownLoad
{
	BYTE							cbDownLoadMode;						//���ط�ʽ
	TCHAR							szDownLoadUrl[256];					//���ص�ַ
};

//������Ϣ
struct CMD_CM_ActionMessage
{
	WORD							wType;								//��Ϣ����
	WORD							wLength;							//��Ϣ����
	UINT							nButtonType;						//��ť����
	TCHAR							szString[1024];						//��Ϣ����
};

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct CMD_CM_DownLoadModule
{
	BYTE							cbShowUI;							//��ʾ����
	BYTE							cbAutoInstall;						//�Զ���װ
	WORD							wFileNameSize;						//���ֳ���
	WORD							wDescribeSize;						//��������
	WORD							wDownLoadUrlSize;					//��ַ����
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif