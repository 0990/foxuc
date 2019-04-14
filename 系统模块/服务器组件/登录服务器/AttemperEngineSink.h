#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "DataBasePacket.h"
#include "list"
#include "cpprest/http_listener.h"
//////////////////////////////////////////////////////////////////////////////////
//���ݶ���

//��������
#define CLIENT_KIND_FALSH			1									//��ҳ����
#define CLIENT_KIND_MOBILE			2									//�ֻ�����
#define CLIENT_KIND_COMPUTER		3									//��������


#define ACCOUNT_TYPE_VISITOR        1     //�ο�
#define ACCOUNT_TYPE_WX             2     //΢�����
#define ACCOUNT_TYPE_ROBOT          3     //������
//�󶨲���
struct tagBindParameter {
    //�������
    DWORD							dwSocketID;							//�����ʶ
    DWORD							dwClientAddr;						//���ӵ�ַ
    DWORD							dwActiveTime;						//����ʱ��

    SCORE                           lUserID;
};

struct EntityInfo {
    int nAccountType;
    SCORE lUserID;
    int nEntityID;
    std::string name;
    std::string szHeadImgUrl;
    float x;
    float y;
    float rotation;
    int lastProcessedInput;
    int nBulletSequenceNumber;
    int hp;
    int score;
	int killCount;
    int nGameRoundSequenceNumber;
    DWORD dwSocketID;
    bool bDead;
  //  bool bGhostMode;
  //  bool bGhostDead;
    DWORD dwLastShootTime;
    DWORD dwCreateTime;
	bool bNew;
	//int nRank;
};

struct WXInfo {
    std::string szName;
    std::string szHeadImgUrl;
    SCORE lUserID;
};

struct UserInfo {
	int nAccountType;
	std::string name; 
	std::string szHeadImgUrl;
	std::string openID;
};

struct SortInfo {
	//SCORE lUserID;
    int nEntityID;
    int nScore;
	int nKillCount;
};

struct BulletInfo {
    int bulletID;
    int damage;
    float initCenterX;
    float initCenterY;
    float x;
    float y;
    float rotation;
    SCORE lCreatorUserID;
    int  nCreatorEntityID;
    long lCreateTime;
    bool bActive;
};

struct Position {
    int x;
    int y;
    Position(int x1, int y1) {
        x = x1;
        y = y1;
    }
};

//////////////////////////////////////////////////////////////////////////////////

//���ȹ���
class CAttemperEngineSink : public IAttemperEngineSink {
    //��Ԫ����
    friend class CServiceUnits;

    //��������
  protected:
    CInitParameter *				m_pInitParameter;					//���ò���
    tagBindParameter *				m_pBindParameter;					//��������

    int                             m_nBulletSequenceNumber;
    int                             m_nEntitySequenceNumber;
    int                             m_nRoundSequenceNumber;
    int                             m_nGameStartTime;
	int                             m_nRobotCount;
    std::map<SCORE, EntityInfo>     m_EntityMap;
    std::list<BulletInfo>           m_BulletList;
    std::map<std::string, WXInfo>   m_WXInfoMap;
	std::map<SCORE, UserInfo>       m_UserInfoMap;

	Json::FastWriter                m_FastWriter;
    //����ӿ�
  protected:
    ITimerEngine *					m_pITimerEngine;					//ʱ������
    IDataBaseEngine *				m_pIDataBaseEngine;					//��������
    ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
    ITCPSocketService *				m_pITCPSocketService;				//Э������

    //��������
  public:
    //���캯��
    CAttemperEngineSink();
    //��������
    virtual ~CAttemperEngineSink();

    //�����ӿ�
  public:
    //�ͷŶ���
    virtual VOID Release() {
        return;
    }
    //�ӿڲ�ѯ
    virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

    //�첽�ӿ�
  public:
    //�����¼�
    virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
    //ֹͣ�¼�
    virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

    //�¼��ӿ�
  public:
    //�����¼�
    virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
    //�Զ��¼�
    virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

    //�ں��¼�
  public:
    //ʱ���¼�
    virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
    //���ݿ��¼�
    virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

    //�����¼�
  public:
    //�����¼�
    virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
    //�ر��¼�
    virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
    //��ȡ�¼�
    virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

    //�����¼�
  public:
    //Ӧ���¼�
    virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
    //�ر��¼�
    virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
    //��ȡ�¼�
    virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);
    //����̨�¼�
    virtual bool OnEventTCPNetworkForeground(bool bForeground, DWORD dwSocketID);
    //���Ӵ���

    //�ֻ��¼�
  protected:
    //��¼����
    bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

    bool OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

    //�ֻ��¼�
  protected:

    bool CreateEntity(DWORD dwSocketID,SCORE lUserID,int nAccountType,std::string name="",std::string headImgUrl="");

	bool EntityMove(SCORE lUserID, float fPressTime, float fTargetRotation, int nInputNumber);

	bool EntityJump(SCORE lUserID);

    bool EntityShoot(SCORE lUserID);
    //�ֻ��¼�
  protected:
    //��¼�ɹ�
    bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //��¼ʧ��
    bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnDBLogonResult(DWORD dwContextID, SCORE lUserID,int nAccountType);
    //
    bool SortRank(Json::Value& rankJson);
    bool SendDeadMessage(SCORE lDeadUserID,SCORE lKillerUserID);
	SCORE CreateNewUserID();
    Position GetRandPosition();
    pplx::task<Json::Value>  RequestAccessToken(DWORD dwSocketID,std::string code);
    pplx::task<Json::Value>  RequestRefreshToken(DWORD dwSocketID,std::string code);

    pplx::task<Json::Value> RequestUserInfo(const std::string& access_token, const std::string& open_id, const std::string& refresh_token, int expires_in);

	pplx::task<Json::Value> HandleRestSdkException(DWORD dwSocketID, pplx::task<Json::Value>& t);

	pplx::task<Json::Value> HandleRestSdkJSException(DWORD dwSocketID, pplx::task<Json::Value>& t);

	pplx::task<Json::Value>  RequestSessionKey(DWORD dwSocketID, std::string code, std::string name,std::string avatarUrl);

};

//////////////////////////////////////////////////////////////////////////////////

#endif