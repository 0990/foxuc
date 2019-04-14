#ifndef SERVER_LIST_MANAGER_HEAD_FILE
#define SERVER_LIST_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"

//////////////////////////////////////////////////////////////////////////////////

//�б����
namespace NServerListManager
{

	//////////////////////////////////////////////////////////////////////////////////

	//��Ϸ����
	class CGameListItem
	{
		//��������
	public:
		bool							m_bDisuse;							//������־

		//��������
	protected:
		//���캯��
		CGameListItem() { m_bDisuse=false; }
	};

	//////////////////////////////////////////////////////////////////////////////////

	//����ṹ
	class CGameTypeItem : public CGameListItem
	{
		//��������
	public:
		tagGameType						m_GameType;							//������Ϣ

		//��������
	public:
		//���캯��
		CGameTypeItem();
	};

	//////////////////////////////////////////////////////////////////////////////////

	//���ͽṹ
	class CGameKindItem : public CGameListItem
	{
		//��������
	public:
		tagGameKind						m_GameKind;							//������Ϣ

		//��������
	public:
		//���캯��
		CGameKindItem();
	};

	//////////////////////////////////////////////////////////////////////////////////

	//�ڵ�ṹ
	class CGameNodeItem : public CGameListItem
	{
		//��������
	public:
		tagGameNode						m_GameNode;							//�ڵ���Ϣ

		//��������
	public:
		//���캯��
		CGameNodeItem();
	};

	//////////////////////////////////////////////////////////////////////////////////

	//���ƽṹ
	class CGamePageItem : public CGameListItem
	{
		//��������
	public:
		tagGamePage					m_GamePage;						//������Ϣ

		//��������
	public:
		//���캯��
		CGamePageItem();
	};

	//////////////////////////////////////////////////////////////////////////////////

	//����ṹ
	class CGameServerItem : public CGameListItem
	{
		//��������
	public:
		tagGameServer					m_GameServer;						//������Ϣ

		//��������
	public:
		//���캯��
		CGameServerItem();
	};

	//////////////////////////////////////////////////////////////////////////////////

	//����˵��
	typedef CWHArray<CGameTypeItem *> CGameTypeItemArray;
	typedef CWHArray<CGameKindItem *> CGameKindItemArray;
	typedef CWHArray<CGameNodeItem *> CGameNodeItemArray;
	typedef CWHArray<CGamePageItem *> CGamePageItemArray;
	typedef CWHArray<CGameServerItem *>	CGameServerItemArray;

	//����˵��
	typedef CMap<WORD,WORD,CGameTypeItem *,CGameTypeItem * &> CTypeItemMap;
	typedef CMap<WORD,WORD,CGameKindItem *,CGameKindItem * &> CKindItemMap;
	typedef CMap<WORD,WORD,CGameNodeItem *,CGameNodeItem * &> CNodeItemMap;
	typedef CMap<WORD,WORD,CGamePageItem *,CGamePageItem * &> CPageItemMap;
	typedef CMap<WORD,WORD,CGameServerItem *,CGameServerItem * &> CServerItemMap;

	//////////////////////////////////////////////////////////////////////////////////

	//�б����
	class CServerListManager
	{
		//��������
	protected:
		CTypeItemMap					m_TypeItemMap;						//��������
		CKindItemMap					m_KindItemMap;						//��������
		CNodeItemMap					m_NodeItemMap;						//�ڵ�����
		CPageItemMap					m_PageItemMap;						//��������
		CServerItemMap					m_ServerItemMap;					//��������

		//�洢����
	protected:
		CGameTypeItemArray				m_TypeItemBuffer;					//��������
		CGameKindItemArray				m_KindItemBuffer;					//��������
		CGameNodeItemArray				m_NodeItemBuffer;					//�ڵ�����
		CGamePageItemArray				m_PageItemBuffer;					//��������
		CGameServerItemArray			m_ServerItemBuffer;					//��������

		//��������
	public:
		//���캯��
		CServerListManager();
		//��������
		virtual ~CServerListManager();

		//����ӿ�
	public:
		//�����б�
		VOID ResetServerList();
		//�����ں�
		VOID CleanKernelItem();
		//������
		VOID CleanServerItem();
		//�����ں�
		VOID DisuseKernelItem();
		//��������
		VOID DisuseServerItem();

		//ͳ�ƽӿ�
	public:
		//ͳ������
		DWORD CollectOnlineInfo();
		//��������
		DWORD CollectOnlineInfo(WORD wKindID);

		//����ӿ�
	public:
		//��������
		bool InsertGameType(tagGameType * pGameType);
		//��������
		bool InsertGameKind(tagGameKind * pGameKind);
		//����ڵ�
		bool InsertGameNode(tagGameNode * pGameNode);
		//���붨��
		bool InsertGamePage(tagGamePage * pGamePage);
		//���뷿��
		bool InsertGameServer(tagGameServer * pGameServer);

		//ɾ���ӿ�
	public:
		//ɾ������
		bool DeleteGameType(WORD wTypeID);
		//ɾ������
		bool DeleteGameKind(WORD wKindID);
		//ɾ���ڵ�
		bool DeleteGameNode(WORD wNodeID);
		//ɾ������
		bool DeleteGamePage(WORD wPageID);
		//ɾ������
		bool DeleteGameServer(WORD wServerID);

		//ö�ٽӿ�
	public:
		//ö������
		CGameTypeItem * EmunGameTypeItem(POSITION & Position);
		//ö������
		CGameKindItem * EmunGameKindItem(POSITION & Position);
		//ö�ٽڵ�
		CGameNodeItem * EmunGameNodeItem(POSITION & Position);
		//ö�ٶ���
		CGamePageItem * EmunGamePageItem(POSITION & Position);
		//ö�ٷ���
		CGameServerItem * EmunGameServerItem(POSITION & Position);

		//���ҽӿ�
	public:
		//��������
		CGameTypeItem * SearchGameType(WORD wTypeID);
		//��������
		CGameKindItem * SearchGameKind(WORD wKindID);
		//���ҽڵ�
		CGameNodeItem * SearchGameNode(WORD wNodeID);
		//���Ҷ���
		CGamePageItem * SearchGamePage(WORD wPageID);
		//���ҷ���
		CGameServerItem * SearchGameServer(WORD wServerID);

		//��Ŀ�ӿ�
	public:
		//������Ŀ
		DWORD GetGameTypeCount() { return (DWORD)m_TypeItemMap.GetCount(); }
		//������Ŀ
		DWORD GetGameKindCount() { return (DWORD)m_KindItemMap.GetCount(); }
		//�ڵ���Ŀ
		DWORD GetGameNodeCount() { return (DWORD)m_NodeItemMap.GetCount(); }
		//������Ŀ
		DWORD GetGamePageCount() { return (DWORD)m_PageItemMap.GetCount(); }
		//������Ŀ
		DWORD GetGameServerCount() { return (DWORD)m_ServerItemMap.GetCount(); }
	};

	//////////////////////////////////////////////////////////////////////////////////

};

using namespace NServerListManager;

#endif