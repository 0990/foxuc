#ifndef PROPERTY_HEAD_FILE
#define PROPERTY_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

//���з�Χ
#define PT_ISSUE_AREA_WEB			0x01								//�̳ǵ���
#define PT_ISSUE_AREA_GAME			0x02								//��Ϸ����
#define PT_ISSUE_AREA_SERVER		0x04								//�������

//ʹ�÷�Χ
#define PT_SERVICE_AREA_MESELF		0x0001								//�Լ���Χ
#define PT_SERVICE_AREA_PLAYER		0x0002								//��ҷ�Χ
#define PT_SERVICE_AREA_LOOKON		0x0004								//�Թ۷�Χ

//��������
#define PT_TYPE_ERROR               0                                   //�������� 
#define PT_TYPE_PROPERTY            1	                                //��������  
#define PT_TYPE_PRESENT             2                                   //��������

//���߶���
#define PROPERTY_ID_CAR				1									//��������
#define PROPERTY_ID_EGG				2									//��������
#define PROPERTY_ID_CLAP			3									//��������
#define PROPERTY_ID_KISS			4									//��������
#define PROPERTY_ID_BEER			5									//ơ������
#define PROPERTY_ID_CAKE			6									//��������
#define PROPERTY_ID_RING			7									//�������
#define PROPERTY_ID_BEAT			8									//��������
#define PROPERTY_ID_BOMB			9									//ը������
#define PROPERTY_ID_SMOKE			10									//��������
#define PROPERTY_ID_VILLA			11									//��������
#define PROPERTY_ID_BRICK			12									//שͷ����
#define PROPERTY_ID_FLOWER			13									//�ʻ�����

#define PROPERTY_ID_TWO_CARD	    14				                    //���ֵ��� 
#define PROPERTY_ID_FOUR_CARD		15			                        //���ֵ���  
#define PROPERTY_ID_SCORE_CLEAR     16									//��������
#define PROPERTY_ID_ESCAPE_CLEAR    17									//��������
#define PROPERTY_ID_TRUMPET			18									//���ȵ���
#define PROPERTY_ID_TYPHON			19									//���ȵ���
#define PROPERTY_ID_GUARDKICK_CARD  20									//���ߵ���
#define PROPERTY_ID_POSSESS			21									//�������
#define PROPERTY_ID_BLUERING_CARD	22									//�������
#define PROPERTY_ID_YELLOWRING_CARD	23									//�������
#define PROPERTY_ID_WHITERING_CARD	24  								//������� 
#define PROPERTY_ID_REDRING_CARD	25									//�������
#define PROPERTY_ID_VIPROOM_CARD	26									//VIP������

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagPropertyInfo
{
	//������Ϣ
	WORD							wIndex;								//���߱�ʶ
	WORD							wDiscount;							//��Ա�ۿ�
	WORD							wIssueArea;							//������Χ

	//���ۼ۸�
	SCORE							lPropertyGold;						//���߽��
	DOUBLE							dPropertyCash;						//���߼۸�

	//��������
	SCORE							lSendLoveLiness;					//��������
	SCORE							lRecvLoveLiness;					//��������
};


//��������
struct tagPropertyAttrib
{
	WORD							wIndex;								//���߱�ʶ
	WORD                            wPropertyType;                      //��������
	WORD							wServiceArea;						//ʹ�÷�Χ
	TCHAR                           szMeasuringunit[8];					//������λ 
	TCHAR							szPropertyName[32];					//��������
	TCHAR							szRegulationsInfo[256];				//ʹ����Ϣ
};

//��������
struct tagPropertyItem
{
	tagPropertyInfo					PropertyInfo;						//������Ϣ
	tagPropertyAttrib				PropertyAttrib;						//��������
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif