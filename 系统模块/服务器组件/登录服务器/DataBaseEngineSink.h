#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink {
    //��Ԫ����
    friend class CServiceUnits;

    //�û����ݿ�
  protected:
    CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
    CDataBaseHelper					m_AccountsDBModule;					//�û����ݿ�

    //��Ϸ�����ݿ�
  protected:
    CDataBaseAide					m_TreasureDBAide;					//��Ϸ�����ݿ�
    CDataBaseHelper					m_TreasureDBModule;					//��Ϸ�����ݿ�

    //ƽ̨���ݿ�
  protected:
    CDataBaseAide					m_PlatformDBAide;					//ƽ̨���ݿ�
    CDataBaseHelper					m_PlatformDBModule;					//ƽ̨���ݿ�

    //�������
  protected:
    CInitParameter *				m_pInitParameter;					//���ò���
    IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�
    //��������
  public:
    //���캯��
    CDataBaseEngineSink();
    //��������
    virtual ~CDataBaseEngineSink();

    //�����ӿ�
  public:
    //�ͷŶ���
    virtual VOID Release() {
        return;
    }
    //�ӿڲ�ѯ
    virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

    //ϵͳ�¼�
  public:
    //�����¼�
    virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
    //ֹͣ�¼�
    virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

    //�ں��¼�
  public:
    //ʱ���¼�
    virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
    //�����¼�
    virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
    //�����¼�
    virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

    //��¼����
  protected:
    //I D ��¼
    bool OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�ʺŵ�¼
    bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�ʺ�ע��
    bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

    //�ֻ���¼
  protected:
    //I D ��¼
    bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�ʺŵ�¼
    bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�ʺ�ע��
    bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnMobileLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnMobileLogonWX(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�˺ŷ���
  protected:
    //�޸Ļ���
    bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�޸�����
    bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�޸�����
    bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�޸�ǩ��
    bool OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�޸�ͷ��
    bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�޸�ͷ��
    bool OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�޸�����
    bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

    //���з���
  protected:
    //������Ϸ��
    bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //��ȡ��Ϸ��
    bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //ת����Ϸ��
    bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //�ȼý����
    bool OnRequestAlmsInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnRequestRecordInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnRequestExchangeRoomCard(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //��ⷿ����
    bool OnRequestPrivateCreate(DWORD dwContextID, VOID * pData, WORD wDataSize);
    bool OnRequestPrivateJoinCheck(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ص�����
	bool OnRequestPrivateBackRoom(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //���ͷ���
	bool OnRequestTransferRoomCard(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������������
	bool OnSetInsurePassword(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����һ�����
	bool OnFragmentExchangRate(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ǳ�
	bool OnSeekNickname(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����Ƹ�
	bool OnSeekUserTreaure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnTaskComplete(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����������
	bool OnLoadUserTask(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ����
	bool OnReceiveAward(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //����������Ϣ
	bool OnLoadTaskInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ����¼
	bool OnLoadSignInRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ϵͳ��Ϣ
	bool OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnRequestLoadLogonConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ط�������¼
	bool OnRequestGetPrivateRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ������
	bool OnRequestReceiveAlms(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ƹ�
	bool OnRequestUserBindPromote(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϣ��ѯ
  protected:
    //��ѯ����
    bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //��ѯ����
    bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //��ѯ�û�
    bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

    //ϵͳ����
  protected:
    //�����б�
    bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //������Ϣ
    bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

    //�������
  protected:
    //��¼���
    VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
    //���н��
    VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
    //�������
    VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);

    VOID CreateRandPassword(TCHAR* szPassword);
	//��ȡǩ����������
	WORD GetDaysInMonth(WORD year, WORD month);
	WORD getFragmentCardCount(int days);
	WORD GetSignInFreshDay(SYSTEMTIME& sys,int flag);
};

//////////////////////////////////////////////////////////////////////////////////

#endif