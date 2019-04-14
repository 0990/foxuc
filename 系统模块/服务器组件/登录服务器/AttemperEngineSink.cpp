#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include "cpprest\http_client.h"
#include <codecvt>
#include <math.h>


using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace http::client;
#define IDI_CALCULATE                  1
#define TIME_CALCULATE                 50

#define IDI_SEND_WORLD_STATE        2
#define TIME_SEND_WORLD_STATE       100

#define IDI_SEND_RANK   3
#define TIME_SEND_RANK  2000

#define IDI_ONE_SECOND  4
#define TIME_ONE_SECOND 1000

#define PI 3.1415927

#define ENTITY_SPEED 150
#define ENTITY_RADIUS 50
#define BULLET_SPEED 400
#define BULLET_LIVE_TIME 1500
#define ENITTY_PROTECT_TIME 2000
#define GAME_ROUND_TIME 60 //秒

#define ROTATION_DELTA 180
static utility::string_t s_kBaseUrl = L"https://api.weixin.qq.com";
static utility::string_t s_kAppID = L"wx303f99fd7d21038c";
static utility::string_t s_kAppSecret = L"XXXXXX";


#define OVER_REASON_KILLED 0
#define OVER_REASON_NORMAL 1

#define WORLD_WIDTH 2560
#define WORLD_HEIGHT 1440
//////////////////////////////////////////////////////////////////////////////////
std::wstring StringToWString(const std::string& str) {
    int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (num <= 0) return std::wstring(L"");
    wchar_t *wide = new wchar_t[num];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
    std::wstring w_str(wide);
    delete[] wide;
    return w_str;
}

SCORE CAttemperEngineSink::CreateNewUserID() {
	static int nUserCount = 0;
	nUserCount++;
	struct timeval tv;
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tv.tv_sec = clock;
	tv.tv_usec = wtm.wMilliseconds * 1000;
	unsigned long long x =  ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
	return (x * 100 + nUserCount % 100);
}
//构造函数
CAttemperEngineSink::CAttemperEngineSink() {
    //状态变量
    m_pInitParameter=NULL;
    m_pBindParameter=NULL;

    //组件变量
    m_pITimerEngine=NULL;
    m_pIDataBaseEngine=NULL;
    m_pITCPNetworkEngine=NULL;
    m_pITCPSocketService=NULL;
    return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink() {
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer) {
    QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
    return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx) {
    //绑定参数
    m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
    ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

    //设置时间
    ASSERT(m_pITimerEngine!=NULL);

    m_pITimerEngine->SetTimer(IDI_CALCULATE, TIME_CALCULATE, TIMES_INFINITY, 0);
    m_pITimerEngine->SetTimer(IDI_SEND_WORLD_STATE, TIME_SEND_WORLD_STATE, TIMES_INFINITY, 0);
   // m_pITimerEngine->SetTimer(IDI_SEND_RANK, TIME_SEND_RANK, TIMES_INFINITY, 0);
    m_pITimerEngine->SetTimer(IDI_ONE_SECOND, TIME_ONE_SECOND,TIMES_INFINITY,0);

    CP_ControlResult ControlResult;
    ControlResult.cbSuccess = ER_SUCCESS;
    CServiceUnits * pServiceUnits = CServiceUnits::g_pServiceUnits;
    pServiceUnits->PostControlRequest(UI_CORRESPOND_RESULT, &ControlResult, sizeof(ControlResult));
    m_nEntitySequenceNumber = 0;
    m_nBulletSequenceNumber = 0;
    m_nRoundSequenceNumber = 0;
	m_nRobotCount = 0;
	m_nGameStartTime = (DWORD)time(NULL);
    return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx) {
    //组件变量
    m_pITimerEngine=NULL;
    m_pIDataBaseEngine=NULL;
    m_pITCPNetworkEngine=NULL;
    m_pITCPSocketService=NULL;

    //删除数据
    SafeDeleteArray(m_pBindParameter);
    m_EntityMap.clear();
    m_BulletList.clear();
	m_nRobotCount = 0;
    return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize) {
    return false;
}

//调度事件
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize) {
    return false;
}

//时间事件
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam) {
    switch (dwTimerID) {
    case IDI_CALCULATE: {
        DWORD now = GetTickCount();
        //创建机器人
        if (rand() % 150 < 3) {
			m_nRobotCount++;
			ostringstream oss;
			oss << "robot" << m_nRobotCount;
            CreateEntity(INVALID_SOCKET, CreateNewUserID(), ACCOUNT_TYPE_ROBOT, oss.str());
        }

		//DWORD now = GetTickCount();
		//for (auto iterA = m_EntityMap.begin(); iterA != m_EntityMap.end(); iterA++) {
		//	EntityInfo& entity = iterA->second;
		//	if (entity.bDead) continue;
		//	if (now - entity.dwCreateTime > ENITTY_PROTECT_TIME) {
		//		entity.bNew = false;
		//	}
		//}
        for (auto iterA = m_EntityMap.begin(); iterA != m_EntityMap.end(); iterA++) {
            EntityInfo& entity = iterA->second;
            if (entity.bDead) continue;
			if (now - entity.dwCreateTime > ENITTY_PROTECT_TIME) {
				entity.bNew = false;
			}
            if (entity.nAccountType == ACCOUNT_TYPE_ROBOT) {
                float fTargetRotation = entity.rotation;
                int randInt = rand() % 100;
                if (randInt < 10) {
                    fTargetRotation = rand() % 180;
                } else if(randInt<20) {
                    fTargetRotation = -rand() % 180;
                } else {
                    if (entity.x < 640) {
                        if (entity.y < 360) {
                            fTargetRotation = 45;
                        } else if (entity.y > 1080) {
                            fTargetRotation = -45;
                        } else {
                            fTargetRotation = 0;
                        }
                    } else  if (entity.x > 1920) {
                        if (entity.y < 360) {
                            fTargetRotation = 135;
                        } else if (entity.y > 1080) {
                            fTargetRotation = -135;
                        } else {
                            fTargetRotation = 180;
                        }
                    }
                }
                EntityMove(iterA->first, 0.05, fTargetRotation,0);
                if (now - entity.dwLastShootTime > 700) {
                    EntityShoot(iterA->first);
                }
            }
        }

        std::vector<int> deleteVector;
        std::vector<int> diedEntity;
        std::vector<SCORE> statusChangeEntity;
        for (auto bulletIter = m_BulletList.begin(); bulletIter != m_BulletList.end();) {

            BulletInfo& bullet = *bulletIter;
            if (now - bullet.lCreateTime > BULLET_LIVE_TIME) {
                bulletIter = m_BulletList.erase(bulletIter);
                continue;
            }

            bool bErased = false;
            float rotation = bullet.rotation*PI / 180;
            //bullet.y += ((now - bullet.lCreateTime)*BULLET_SPEED/1000)*sin(rotation);
            //bullet.x += ((now - bullet.lCreateTime)*BULLET_SPEED/1000)*cos(rotation);
            bullet.y =bullet.initCenterY +((now - bullet.lCreateTime)*BULLET_SPEED / 1000+ENTITY_RADIUS)*sin(rotation);
            bullet.x =bullet.initCenterX+ ((now - bullet.lCreateTime)*BULLET_SPEED / 1000+ENTITY_RADIUS)*cos(rotation);
            for (auto iter = m_EntityMap.begin(); iter != m_EntityMap.end(); iter++) {
                EntityInfo& entity = iter->second;
                if (entity.bDead) continue;
                if (bullet.lCreatorUserID == iter->first) continue;
                float entityX = entity.x;
                float entityY = entity.y;
                float distance = sqrt(pow(bullet.x - entityX, 2)+pow(bullet.y - entityY, 2));
                if (distance < ENTITY_RADIUS) {
                    if (entity.nAccountType == ACCOUNT_TYPE_VISITOR) {
                        bErased = true;
                    }
                    bErased = true;
                    deleteVector.push_back(bullet.bulletID);
                    if (now - entity.dwCreateTime > ENITTY_PROTECT_TIME) {
                        entity.hp -= bullet.damage;
                        if (entity.hp <= 0) {
                            entity.bDead = true;
                            if (!m_EntityMap[bullet.lCreatorUserID].bDead) {
								m_EntityMap[bullet.lCreatorUserID].killCount ++;
								if (entity.nAccountType == ACCOUNT_TYPE_ROBOT) {
									m_EntityMap[bullet.lCreatorUserID].score ++;
								}
								else {
									m_EntityMap[bullet.lCreatorUserID].score += 5;
								}
                            }
                            diedEntity.push_back(entity.nEntityID);
                            SendDeadMessage(iter->first, bullet.lCreatorUserID);
                        }
                        statusChangeEntity.push_back(iter->first);
                        statusChangeEntity.push_back(bullet.lCreatorUserID);
                    }
                    bulletIter = m_BulletList.erase(bulletIter);
                    break;
                }
            }

            if (bErased == false) {
                bulletIter++;
            }
        }

        for (auto iterA = m_EntityMap.begin(); iterA != m_EntityMap.end(); iterA++) {
            EntityInfo& entityA = iterA->second;
			if (entityA.bDead) continue;
            //if (entityA.bGhostDead) continue;
            //if (entityA.bDead&&entityA.bGhostMode == false) continue;
            if (now - entityA.dwCreateTime < ENITTY_PROTECT_TIME) continue;

            bool bADied = false;
            bool bBDied = false;
            for (auto iterB = iterA; iterB != m_EntityMap.end(); iterB++) {
                EntityInfo& entityB = iterB->second;
				if (entityB.bDead) continue;

                //if (entityB.bGhostDead) continue;
                //if (entityB.bDead&&entityB.bGhostMode == false) continue;
                //if (entityA.bGhostMode&&entityB.bGhostMode) continue;
                if (iterA->first == iterB->first) continue;
                if (now - entityB.dwCreateTime < ENITTY_PROTECT_TIME) continue;
                float distance = sqrt(pow(entityA.x - entityB.x, 2) + pow(entityA.y - entityB.y, 2));
                if (distance < 2*ENTITY_RADIUS) {
                    //if (entityA.bGhostMode) {
                    //    bADied = true;
                    //} else if (entityB.bGhostMode) {
                    //    bBDied = true;
                    //} else {
                        int damage = min(entityA.hp, entityB.hp);
                        entityA.hp -= damage;
                        entityB.hp -= damage;
                        if (entityA.hp <= 0) {
                            bADied = true;
							entityB.killCount++;
							if (entityA.nAccountType == ACCOUNT_TYPE_ROBOT) {
								entityB.score++;
							}
							else {
								entityB.score+=5;
							}
                        }
                        if (entityB.hp <= 0) {
                            bBDied = true;
							entityA.killCount++;
							if (entityB.nAccountType == ACCOUNT_TYPE_ROBOT) {
								entityA.score++;
							}
							else {
								entityA.score += 5;
							}
                        }
                    //}

                    statusChangeEntity.push_back(iterA->first);
                    statusChangeEntity.push_back(iterB->first);
                    if (bADied) {
                        //if (entityA.bGhostMode) {
                        //    entityA.bGhostDead = true;
                        //} else {
                            entityA.bDead = true;
                       // }
                        diedEntity.push_back(entityA.nEntityID);
                        SendDeadMessage(iterA->first, iterB->first);
                    }

                    if (bBDied) {
                        //if (entityB.bGhostMode) {
                        //    entityB.bGhostDead = true;
                        //} else {
                            entityB.bDead = true;
                       // }
                        diedEntity.push_back(entityB.nEntityID);
                        SendDeadMessage(iterB->first, iterB->first);
                        break;
                    }
                }
            }
            if (bADied) {
                break;
            }
        }
        bool bNeedSend = false;;
        Json::Value bulletDeleteList;
        if (deleteVector.size() > 0) {
            bNeedSend = true;
            Json::Value sendRoot;
            for (auto iter = deleteVector.begin(); iter != deleteVector.end(); iter++) {
                bulletDeleteList.append(*iter);
            }
            // std::string sendData = sendm_FastWriter.write(root);
            // m_pITCPNetworkEngine->SendDataBatch(MDM_GF_GAME, SUB_MB_DESTROY_BULLET, (void*)sendData.c_str(), sendData.size(), BG_MOBILE);
        }
        Json::Value entityDeleteList;
        if(diedEntity.size()>0) {
            bNeedSend = true;
            for (auto iter = diedEntity.begin(); iter != diedEntity.end(); iter++) {
                entityDeleteList.append(*iter);
            }
            // std::string sendData = sendm_FastWriter.write(root);
            //  m_pITCPNetworkEngine->SendDataBatch(MDM_GF_GAME, SUB_MB_DESTROY_ENTITY, (void*)sendData.c_str(), sendData.size(), BG_MOBILE);
        }

        Json::Value entityChangeList;
        if (statusChangeEntity.size() > 0) {
            bNeedSend = true;
            for (auto iter = statusChangeEntity.begin(); iter != statusChangeEntity.end(); iter++) {
                if (m_EntityMap[*iter].bDead) continue;
                Json::Value obj;
                obj["entityID"] = m_EntityMap[*iter].nEntityID;
                obj["hp"] = m_EntityMap[*iter].hp;
				obj["score"] = m_EntityMap[*iter].score;
				obj["killCount"] = m_EntityMap[*iter].killCount;
                if (m_EntityMap[*iter].nAccountType == ACCOUNT_TYPE_VISITOR) {

                }
                entityChangeList.append(obj);
            }
        }

        if (bNeedSend) {
            Json::Value root;
            root["bulletDeleteList"] = bulletDeleteList;
            root["entityDeleteList"] = entityDeleteList;
            root["entityChangeList"] = entityChangeList;
            std::string data = m_FastWriter.write(root);
            m_pITCPNetworkEngine->SendDataBatch(MDM_GF_GAME, SUB_MB_CALCULATE_RESULT, (void*)data.c_str(), data.size(), BG_MOBILE);
        }

        return true;
    }
    case IDI_SEND_WORLD_STATE: {
        Json::Value root;
        for (auto iter = m_EntityMap.begin(); iter != m_EntityMap.end(); iter++) {
            EntityInfo& entity = iter->second;
			if (entity.bDead) continue;
			//if (entity.bGhostDead) continue;
   //         if (entity.bDead&&entity.bGhostMode == false) continue;
            Json::Value obj;
            obj["lastProcessedInput"] = iter->second.lastProcessedInput;
            obj["x"] = iter->second.x;
            obj["y"] = iter->second.y;
            obj["rotation"] = iter->second.rotation;
            //obj["hp"] = iter->second.hp;
            obj["entityID"] = entity.nEntityID;
            //obj["ghostMode"] = iter->second.bGhostMode;
            // obj["score"] = entity.score;
            root["entities"].append(obj);
        }

        //for (auto iter = m_BulletList.begin(); iter != m_BulletList.end(); iter++) {
        //    BulletInfo& bullet = *iter;
        //    Json::Value obj;
        //    obj["x"] = bullet.x;
        //    obj["y"] = bullet.y;
        //    obj["rotation"] = bullet.rotation;
        //    obj["bulletID"] = bullet.bulletID;
        //    root["bullets"].append(obj);
        //}
        std::string data = m_FastWriter.write(root);
        m_pITCPNetworkEngine->SendDataBatch(MDM_GF_GAME, SUB_MB_WORLD_STATE, (void*)data.c_str(), data.size(), BG_MOBILE);
        return true;
    }
    case IDI_SEND_RANK: {
        //      Json::Value root;
        //      Json::Value rankJson;
        //      std::vector<SortInfo> sortVector;
        //      for (auto iter = m_EntityMap.begin(); iter != m_EntityMap.end(); iter++) {
        //          EntityInfo& entity = iter->second;
        //          if (entity.bDead) continue;
        //          SortInfo sortInfo;
        //          sortInfo.lUserID = iter->first;
        //          sortInfo.nScore = entity.score;
        //          sortVector.push_back(sortInfo);
        //      }
        //      std::sort(sortVector.begin(), sortVector.end(), compare);
        //      int nRank = 0;
        //      for (auto iter = sortVector.begin(); iter != sortVector.end(); iter++) {
        //          if(nRank >= 3) break;
        //          Json::Value obj;
        //          obj["entityID"] = m_EntityMap[iter->lUserID].nEntityID;
        //          obj["score"] = iter->nScore;
        //          obj["rank"] = nRank++;
        //          rankJson.append(obj);

        //      }
        //      root["rankInfo"] = rankJson;
        //long now = GetTickCount();
        //root["leftTime"] = (int)(now - m_nGameStartTime);
        //      nRank = 0;
        //      for (auto iter = sortVector.begin(); iter != sortVector.end(); iter++) {
        //	nRank++;
        //          if (m_EntityMap[iter->lUserID].nAccountType != ACCOUNT_TYPE_ROBOT) {
        //              DWORD dwSocketID = m_EntityMap[iter->lUserID].dwSocketID;
        //              if ((m_pBindParameter + LOWORD(dwSocketID))->dwSocketID != dwSocketID) continue;
        //		root["rank"] = nRank;
        //		root["score"] = iter->nScore;
        //		std::string data = m_FastWriter.write(root);
        //		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GF_GAME, SUB_MB_RANK, (void*)data.c_str(), data.size());
        //          }
        //      }
        return true;
    }
    case IDI_ONE_SECOND: {
		DWORD nowTime = (DWORD)time(NULL);
		if (nowTime - m_nGameStartTime < GAME_ROUND_TIME)  return true;
        Json::Value root;
        root["overReason"] = OVER_REASON_NORMAL;
        Json::Value rankJson;
        SortRank(rankJson);
        root["rankInfo"] = rankJson;
        //发送游戏结束消息
        std::string data = m_FastWriter.write(root);
        //登录成功
        m_pITCPNetworkEngine->SendDataBatch(MDM_GF_GAME, SUB_MB_GAME_END, (void*)data.c_str(), data.size(), BG_MOBILE);
        m_EntityMap.clear();
        m_BulletList.clear();
        m_nGameStartTime = nowTime;
        m_nEntitySequenceNumber = 0;
		m_nRobotCount = 0;
        return true;
    }
    }

    return false;
}

bool CAttemperEngineSink::SendDeadMessage(SCORE lDeadUserID,SCORE lKillerUserID) {
    EntityInfo entity = m_EntityMap[lDeadUserID];
    DWORD dwSocketID = entity.dwSocketID;
    if (dwSocketID == INVALID_SOCKET) return true;
    if((m_pBindParameter + LOWORD(dwSocketID))->dwSocketID != dwSocketID) return true;

    Json::Value root;
    root["overReason"] = OVER_REASON_KILLED;
	Json::Value killerInfo;
   // root["killedBy"] = nKilledByEntityID;
	killerInfo["accountType"] = m_EntityMap[lKillerUserID].nAccountType;
	killerInfo["name"] = m_EntityMap[lKillerUserID].name;
	killerInfo["headImgUrl"] = m_EntityMap[lKillerUserID].szHeadImgUrl;
	killerInfo["hp"] = 6;

    root["dead"] = entity.bDead;
	root["killerInfo"] = killerInfo;
    //root["ghostDead"] = entity.bGhostDead;
	root["leftTime"] = GAME_ROUND_TIME + m_nGameStartTime - (int)time(NULL);

    std::string data = m_FastWriter.write(root);
    m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GF_GAME, SUB_MB_GAME_END, (void*)data.c_str(), data.size());
	return true;
}
bool compare(const SortInfo &a, const SortInfo &b) {
    if (a.nScore > b.nScore) {
        return true;
    } else if (a.nScore < b.nScore) {
        return false;
    } else {
        return a.nEntityID < b.nEntityID;
    }
}
bool CAttemperEngineSink::SortRank(Json::Value& rankJson) {
    std::vector<SortInfo> sortVector;
    for (auto iter = m_EntityMap.begin(); iter != m_EntityMap.end(); iter++) {
        EntityInfo& entity = iter->second;
        if (entity.bDead) continue;
        SortInfo sortInfo;
        sortInfo.nEntityID = entity.nEntityID;
        sortInfo.nScore = entity.score;
		sortInfo.nKillCount = entity.killCount;
        sortVector.push_back(sortInfo);
    }
    std::sort(sortVector.begin(), sortVector.end(), compare);
    int nRank = 1;
    for (auto iter = sortVector.begin(); iter != sortVector.end(); iter++) {
        Json::Value obj;
        obj["entityID"] = iter->nEntityID;
        obj["score"] = iter->nScore;
        obj["rank"] = nRank++;
		obj["killCount"] = iter->nKillCount;
        rankJson.append(obj);
    }
    return true;
}

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID) {
//获取索引
    ASSERT(LOWORD(dwSocketID) < m_pInitParameter->m_wMaxConnect);
    if (LOWORD(dwSocketID) >= m_pInitParameter->m_wMaxConnect) return false;

//变量定义
    WORD wBindIndex = LOWORD(dwSocketID);
    tagBindParameter * pBindParameter = (m_pBindParameter + wBindIndex);

//设置变量
    pBindParameter->dwSocketID = dwSocketID;
    pBindParameter->dwClientAddr = dwClientAddr;
    pBindParameter->dwActiveTime = (DWORD)time(NULL);

    return true;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID) {
    //清除信息
    WORD wBindIndex = LOWORD(dwSocketID);
    ZeroMemory((m_pBindParameter + wBindIndex), sizeof(tagBindParameter));

    return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID) {
    switch (Command.wMainCmdID) {
    case MDM_GF_GAME: {			//游戏命令
        return OnTCPNetworkMainGame(Command.wSubCmdID, pData, wDataSize, dwSocketID);
    }
    case MDM_MB_LOGON: {		//登录命令
        return OnTCPNetworkMainMBLogon(Command.wSubCmdID, pData, wDataSize, dwSocketID);
    }
    }

    return false;
}
bool CAttemperEngineSink::OnEventTCPNetworkForeground(bool bForeground, DWORD dwSocketID) {
    //CTraceService::TraceString(TEXT("内核检测到玩家有进出后台"), TraceLevel_Warning);
    return true;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize) {
    switch (wRequestID) {
    case DBO_GP_WXLOGON: {		//登录成功
        return OnDBMBLogonSuccess(dwContextID, pData, wDataSize);
    }
    case DBO_MB_LOGON_FAILURE: {		//登录失败
        return OnDBMBLogonFailure(dwContextID, pData, wDataSize);
    }
    }

    return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason) {
    return false;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode) {
    return true;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize) {
    return true;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID) {
    //解析数据
    std::string data((char*)pData, wDataSize);
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(data, root)) {
        return false;
    }

    switch (wSubCmdID) {
    case SUB_MB_LOGON_VISITOR: {
        SCORE lUserID = root["userID"].asLargestInt();
        if (lUserID == 0) {
            lUserID = CreateNewUserID();
		}

		if (m_UserInfoMap.find(lUserID) == m_UserInfoMap.end()) {
			UserInfo userInfo;
			userInfo.nAccountType = ACCOUNT_TYPE_VISITOR;
			userInfo.name = "visitor";
			m_UserInfoMap[lUserID] = userInfo;
		}

		OnDBLogonResult(dwSocketID, lUserID, ACCOUNT_TYPE_VISITOR);
		return true;
    }
	case SUB_MB_LOGON_WX_TEMP: {
		SCORE lUserID = CreateNewUserID();
		if (m_UserInfoMap.find(lUserID) == m_UserInfoMap.end()) {
			UserInfo userInfo;
			userInfo.nAccountType = ACCOUNT_TYPE_WX;
			userInfo.name = root["name"].asString();
			userInfo.szHeadImgUrl = root["avatarUrl"].asString();
			m_UserInfoMap[lUserID] = userInfo;
		}
		OnDBLogonResult(dwSocketID, lUserID, ACCOUNT_TYPE_WX);
		return true;
	}
    case SUB_MB_WX_LOGON_FIRST: {
        string code = root["code"].asString();
        RequestAccessToken(dwSocketID, code);
        return true;
    }
	case SUB_MB_LOGON_WX_GAME: {
		string code = root["code"].asString();
		string name = root["name"].asString();
		string avatarUrl = root["avatarUrl"].asString();
		RequestSessionKey(dwSocketID, code,name,avatarUrl);
		return true;
	}
    case SUB_MB_WX_LOGON_SECOND: {
        string refresh_token = root["refreshToken"].asString();
        RequestRefreshToken(dwSocketID, refresh_token);
        return true;
    }
    case SUB_MB_LOGON_WX_OPENID: {
        string openID = root["openID"].asString();
        if (m_WXInfoMap.find(openID) != m_WXInfoMap.end()) {
            CreateEntity(dwSocketID, m_WXInfoMap[openID].lUserID, ACCOUNT_TYPE_WX, m_WXInfoMap[openID].szName, m_WXInfoMap[openID].szHeadImgUrl);
        } else {
            Json::Value sendRoot;
            sendRoot["reason"] = "notexisted";
            sendRoot["accountType"] = ACCOUNT_TYPE_WX;
            std::string sendData = m_FastWriter.write(sendRoot);
            m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON, SUB_MB_LOGON_FAILURE, (void*)sendData.c_str(), sendData.size());
        }
        return true;
    }
	case SUB_MB_JOIN_GAME: {
		SCORE lUserID = root["userID"].asLargestInt();
		if (m_UserInfoMap.find(lUserID) == m_UserInfoMap.end()) {
			Json::Value sendRoot;
			sendRoot["reason"] = "neverlogon";
			std::string sendData = m_FastWriter.write(sendRoot);
			m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON, SUB_MB_LOGON_FAILURE,(void*)sendData.c_str(), sendData.size());
		}
		else {
			std::string name = m_UserInfoMap[lUserID].name;
			if (m_UserInfoMap[lUserID].nAccountType == ACCOUNT_TYPE_VISITOR) {
				if (!root["name"].isNull()) {
					m_UserInfoMap[lUserID].name = root["name"].asString();
				}
			}
			CreateEntity(dwSocketID, lUserID, m_UserInfoMap[lUserID].nAccountType, m_UserInfoMap[lUserID].name, m_UserInfoMap[lUserID].szHeadImgUrl);
		}
		return true;
	}
    }

    return false;
}
bool CAttemperEngineSink::OnDBLogonResult(DWORD dwContextID, SCORE lUserID, int nAccountType) {
	bool bDead = false;
	if (m_EntityMap.find(lUserID) != m_EntityMap.end()) {
		bDead = m_EntityMap[lUserID].bDead;
	}


	Json::Value root;
	root["accountType"] = nAccountType;
	root["leftTime"] = GAME_ROUND_TIME + m_nGameStartTime - (int)time(NULL);

	root["userID"] = lUserID;
	root["dead"] = bDead;
	std::string sendData = m_FastWriter.write(root);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_MB_LOGON, SUB_MB_LOGON_SUCCESS, (void*)sendData.c_str(), sendData.size());

	if (m_EntityMap.find(lUserID) != m_EntityMap.end()) {
		if (!m_EntityMap[lUserID].bDead) {
			CreateEntity(dwContextID, lUserID, nAccountType, m_EntityMap[lUserID].name);
		}
	}

	return true;
}
//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID) {
    //解析数据
    std::string data((char*)pData, wDataSize);
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(data, root)) {
        return false;
    }

    WORD wBindIndex = LOWORD(dwSocketID);
    tagBindParameter * pBindParameter = (m_pBindParameter + wBindIndex);
    SCORE lUserID = pBindParameter->lUserID;

    switch (wSubCmdID) {
    case SUB_MB_MOVE: {
        float fPressTime = root["pressTime"].asFloat();
        float fTargetRotation = root["targetRotation"].asFloat();
        int nlastProcessedInput = root["inputSequenceNumber"].asInt();
        return EntityMove(lUserID, fPressTime, fTargetRotation, nlastProcessedInput);
    }
    case SUB_MB_SHOOT: {
        return EntityShoot(lUserID);
    }
    case SUB_MB_JUMP: {
        return EntityJump(lUserID);
    }
    case SUB_MB_GAME_SCENE: {
        Json::Value entitiesInfo;
        for (auto iter = m_EntityMap.begin(); iter != m_EntityMap.end(); iter++) {
            EntityInfo& entity = iter->second;
			if (entity.bDead) continue;
			//if (entity.bGhostDead) continue;
   //         if (entity.bDead&&entity.bGhostMode == false) continue;
            Json::Value item;
          //  item["ghostMode"] = entity.bGhostMode;
            item["accountType"] = entity.nAccountType;
            item["dead"] = entity.bDead;
            item["name"] = entity.name;
            if (entity.nAccountType == ACCOUNT_TYPE_WX) {
                item["headImgUrl"] = entity.szHeadImgUrl;
            }
            item["entityID"] = entity.nEntityID;
            item["hp"] = entity.hp;
            item["score"] = entity.score;
			item["new"] = entity.bNew;
            entitiesInfo.append(item);
        }
        Json::Value sendRoot;
        sendRoot["entities"] = entitiesInfo;
		sendRoot["leftTime"] = GAME_ROUND_TIME + m_nGameStartTime - (int)time(NULL);

        std::string sendData = m_FastWriter.write(sendRoot);

        m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GF_GAME, SUB_MB_GAME_SCENE, (void*)sendData.c_str(), sendData.size());
        return true;
    }
    }

    return false;
}

bool CAttemperEngineSink::CreateEntity(DWORD dwSocketID,SCORE lUserID,int nAccountType, std::string name,std::string headImgUrl) {
    int leftTime = GAME_ROUND_TIME + m_nGameStartTime - (int)time(NULL);

    int nEntityID = 0;
    if (m_EntityMap.find(lUserID) == m_EntityMap.end()) {
        nEntityID = m_nEntitySequenceNumber++;
        EntityInfo info;
        Position pos = GetRandPosition();
        info.nAccountType = nAccountType;
        info.nEntityID = nEntityID;
        info.x = pos.x;
        info.y = pos.y;
        info.rotation = 0;
        info.lastProcessedInput = -1;
        info.nBulletSequenceNumber = 0;
        info.hp = 6;
        info.score = 0;
		info.killCount = 0;
        info.name = name;
        info.szHeadImgUrl = headImgUrl;
        info.bDead = false;
       // info.bGhostMode = false;
       // info.bGhostDead = false;
        info.dwCreateTime = GetTickCount();
        info.lUserID = lUserID;
		info.dwLastShootTime = GetTickCount();
		info.bNew = true;
        m_EntityMap[lUserID] = info;
    } else {
        nEntityID = m_EntityMap[lUserID].nEntityID;
        //if (m_EntityMap[lUserID].bGhostDead == false) {
        //    m_EntityMap[lUserID].lastProcessedInput = -1;
        //    if (m_EntityMap[lUserID].bDead) {
        //        Position pos = GetRandPosition();
        //        m_EntityMap[lUserID].bGhostMode = true;
        //        m_EntityMap[lUserID].x = pos.x;
        //        m_EntityMap[lUserID].y = pos.y;
        //    }
        //} else {
        if (m_EntityMap[lUserID].bDead) {
            Json::Value sendRoot;
            sendRoot["reason"] = "dead";
            sendRoot["leftTime"] = leftTime;
            sendRoot["accountType"] = nAccountType;
            std::string sendData = m_FastWriter.write(sendRoot);
            if (dwSocketID != INVALID_SOCKET) {
                m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON, SUB_MB_JOIN_GAME_FAILURE, (void*)sendData.c_str(), sendData.size());
            }
            //登录成功
            return true;
        }
    }


    if (dwSocketID != INVALID_SOCKET) {
        //绑定socket和unionID
        WORD wBindIndex = LOWORD(dwSocketID);
        tagBindParameter * pBindParameter = (m_pBindParameter + wBindIndex);
        pBindParameter->lUserID = lUserID;
    }
    m_EntityMap[lUserID].dwSocketID = dwSocketID;

    Json::Value userInfo;
    userInfo["userID"] = lUserID;
    userInfo["entityID"] = nEntityID;
    userInfo["dead"] = m_EntityMap[lUserID].bDead;
    userInfo["name"] = m_EntityMap[lUserID].name;
    //if (nAccountType == ACCOUNT_TYPE_WX) {
    //    userInfo["openID"] = openID;
    //}

    //TCHAR szClientAddr[16] = TEXT("");
    //DWORD dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
    //BYTE * pClientAddr = (BYTE *)&dwClientAddr;
    //_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);
    //entityInfo["clientAddr"] = Util::StringConvert::UnicodeToUtf8(szClientAddr);

    Json::Value config;
    config["bulletLiveTime"] = BULLET_LIVE_TIME;
    config["rotationDelta"] = ROTATION_DELTA;
    config["entitySpeed"] = ENTITY_SPEED;
    config["bulletSpeed"] = BULLET_SPEED;
    config["sendWorldTime"] = TIME_SEND_WORLD_STATE;
    config["protectTime"] = ENITTY_PROTECT_TIME;
    config["entityRadius"] = ENTITY_RADIUS;
    config["gameStartTime"] = m_nGameStartTime;

    Json::Value sendRoot;
    sendRoot["userInfo"] = userInfo;
    sendRoot["config"] = config;
    sendRoot["leftTime"] = leftTime;
    sendRoot["entityID"] = nEntityID;
    std::string sendData = m_FastWriter.write(sendRoot);

    //登录成功发消息
    if (dwSocketID != INVALID_SOCKET) {
        m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON, SUB_MB_JOIN_GAME_SUCCESS, (void*)sendData.c_str(), sendData.size());
    }

    Json::Value global;
    global["entityID"] = m_EntityMap[lUserID].nEntityID;
    global["accountType"] = m_EntityMap[lUserID].nAccountType;
    if (m_EntityMap[lUserID].nAccountType == ACCOUNT_TYPE_WX) {
        global["headImgUrl"] = headImgUrl;
    }
    global["x"] = m_EntityMap[lUserID].x;
    global["y"] = m_EntityMap[lUserID].y;
    global["rotation"] = m_EntityMap[lUserID].rotation;
    global["name"] = m_EntityMap[lUserID].name;
   // global["ghostMode"] = m_EntityMap[lUserID].bGhostMode;
    global["hp"] = m_EntityMap[lUserID].hp;
    global["score"] = m_EntityMap[lUserID].score;
    global["dead"] = m_EntityMap[lUserID].bDead;
	global["new"] = m_EntityMap[lUserID].bNew;
    std::string globalData = global.toStyledString();
    m_pITCPNetworkEngine->SendDataBatch(MDM_GF_GAME, SUB_MB_NEW_ENTITY, (void*)globalData.c_str(), globalData.size(), BG_MOBILE);
	return true;
}

bool CAttemperEngineSink::EntityMove(SCORE lUserID, float fPressTime, float fTargetRotation,int nInputNumber) {
    if (m_EntityMap.find(lUserID) == m_EntityMap.end()) return true;
    float lastRotation = m_EntityMap[lUserID].rotation;
    float newRotation = lastRotation;
    float rotationDelta = abs(fTargetRotation - lastRotation);
    if (rotationDelta >= 180) {
        rotationDelta = abs(360 - rotationDelta);
    }
    if (rotationDelta <= fPressTime*ROTATION_DELTA) {
        newRotation = fTargetRotation;
    } else {
        float rotationChange = 0;
        if (lastRotation >= 0) {
            if (lastRotation - 180 < fTargetRotation && fTargetRotation < lastRotation) {
                rotationChange = -fPressTime * ROTATION_DELTA;
            } else {
                rotationChange = fPressTime * ROTATION_DELTA;
            }
        } else {
            if (lastRotation < fTargetRotation && fTargetRotation < lastRotation + 180) {
                rotationChange = fPressTime * ROTATION_DELTA;
            } else {
                rotationChange = -fPressTime * ROTATION_DELTA;
            }
        }
        newRotation = lastRotation + rotationChange;
    }
    if (newRotation > 180) newRotation = newRotation - 360;
    if (newRotation < -180) newRotation = 360 + newRotation;
    ASSERT(-180 <= newRotation&&newRotation <= 180);
    m_EntityMap[lUserID].y += fPressTime * ENTITY_SPEED*sin(lastRotation*PI / 180);
    m_EntityMap[lUserID].x += fPressTime * ENTITY_SPEED*cos(lastRotation*PI / 180);
    m_EntityMap[lUserID].rotation = newRotation;
    m_EntityMap[lUserID].lastProcessedInput = nInputNumber;

    if (m_EntityMap[lUserID].x < ENTITY_RADIUS) {
        m_EntityMap[lUserID].x = ENTITY_RADIUS;
    }
    if (m_EntityMap[lUserID].x >WORLD_WIDTH - ENTITY_RADIUS) {
        m_EntityMap[lUserID].x = WORLD_WIDTH - ENTITY_RADIUS;
    }

    if (m_EntityMap[lUserID].y < ENTITY_RADIUS) {
        m_EntityMap[lUserID].y = ENTITY_RADIUS;
    }

    if (m_EntityMap[lUserID].y >WORLD_HEIGHT - ENTITY_RADIUS) {
        m_EntityMap[lUserID].y = WORLD_HEIGHT - ENTITY_RADIUS;
    }
    return true;
}

bool CAttemperEngineSink::EntityJump(SCORE lUserID) {
    if (m_EntityMap.find(lUserID) == m_EntityMap.end()) return true;

    float lastRotation = m_EntityMap[lUserID].rotation;
    m_EntityMap[lUserID].y += 1.0 * ENTITY_SPEED*sin(lastRotation*PI / 180);
    m_EntityMap[lUserID].x += 1.0 * ENTITY_SPEED*cos(lastRotation*PI / 180);
    //m_EntityMap[lUserID].rotation = newRotation;

    if (m_EntityMap[lUserID].x < ENTITY_RADIUS) {
        m_EntityMap[lUserID].x = ENTITY_RADIUS;
    }
    if (m_EntityMap[lUserID].x >WORLD_WIDTH - ENTITY_RADIUS) {
        m_EntityMap[lUserID].x = WORLD_WIDTH - ENTITY_RADIUS;
    }

    if (m_EntityMap[lUserID].y < ENTITY_RADIUS) {
        m_EntityMap[lUserID].y = ENTITY_RADIUS;
    }

    if (m_EntityMap[lUserID].y >WORLD_HEIGHT - ENTITY_RADIUS) {
        m_EntityMap[lUserID].y = WORLD_HEIGHT - ENTITY_RADIUS;
    }
    return true;
}

bool CAttemperEngineSink::EntityShoot(SCORE lUserID) {
    if (m_EntityMap.find(lUserID) == m_EntityMap.end()) return true;
    DWORD now = GetTickCount();
    if (now - m_EntityMap[lUserID].dwLastShootTime < 500) {
        return true;
    }

 //   if (m_EntityMap[lUserID].bGhostDead) return true;
	//if (m_EntityMap[lUserID].bDead&&m_EntityMap[lUserID].bGhostMode == false) return true;
	if (m_EntityMap[lUserID].bDead) return true;
    m_EntityMap[lUserID].dwLastShootTime = now;

    BulletInfo bulletInfo;
    bulletInfo.rotation = m_EntityMap[lUserID].rotation;
    float rotation = bulletInfo.rotation  *PI / 180;
    bulletInfo.y = m_EntityMap[lUserID].y + ENTITY_RADIUS*sin(rotation);
    bulletInfo.x = m_EntityMap[lUserID].x + ENTITY_RADIUS*cos(rotation);
    bulletInfo.initCenterX = m_EntityMap[lUserID].x;
    bulletInfo.initCenterY = m_EntityMap[lUserID].y;

    bulletInfo.lCreatorUserID = lUserID;
    bulletInfo.nCreatorEntityID = m_EntityMap[lUserID].nEntityID;
    bulletInfo.bulletID = m_nBulletSequenceNumber++;
    bulletInfo.lCreateTime = now;
    bulletInfo.bActive = true;
    //if (m_EntityMap[lUserID].bGhostMode) {
    //    bulletInfo.damage = 1;
    //} else {
    bulletInfo.damage = 2;
    //}
    m_BulletList.push_back(bulletInfo);

    //这里发送当前所有玩家的状态
    Json::Value sendRoot;
    sendRoot["x"] = bulletInfo.x;
    sendRoot["y"] = bulletInfo.y;
    sendRoot["rotation"] = bulletInfo.rotation;
    sendRoot["bulletID"] = bulletInfo.bulletID;
    sendRoot["creatorID"] = bulletInfo.nCreatorEntityID;
    std::string sendData = m_FastWriter.write(sendRoot);
    m_pITCPNetworkEngine->SendDataBatch(MDM_GF_GAME, SUB_MB_SHOOT, (void*)sendData.c_str(), sendData.size(), BG_MOBILE);

	return true;
}
//登录成功
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //判断在线
    ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
    if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;
    DBO_MB_LogonWX* pLogonWX = (DBO_MB_LogonWX*)pData;
    std::string openID = std::string(pLogonWX->szOpenID);

    if (m_WXInfoMap.find(openID) == m_WXInfoMap.end()) {
        WXInfo wxInfo;
        wxInfo.lUserID = CreateNewUserID();
        wxInfo.szName = Util::StringConvert::UnicodeToUtf8(pLogonWX->szNickName);
        wxInfo.szHeadImgUrl = std::string(pLogonWX->szHeadImgUrl);
        m_WXInfoMap[openID] = wxInfo;

		UserInfo userInfo;
		userInfo.nAccountType = ACCOUNT_TYPE_WX;
		userInfo.openID = openID;
		userInfo.name = wxInfo.szName;
		userInfo.szHeadImgUrl = wxInfo.szHeadImgUrl;
		m_UserInfoMap[wxInfo.lUserID] = userInfo;
    } else {
        m_WXInfoMap[openID].szName = Util::StringConvert::UnicodeToUtf8(pLogonWX->szNickName);
        m_WXInfoMap[openID].szHeadImgUrl = std::string(pLogonWX->szHeadImgUrl);
		m_UserInfoMap[m_WXInfoMap[openID].lUserID].name = m_WXInfoMap[openID].szName;
		m_UserInfoMap[m_WXInfoMap[openID].lUserID].szHeadImgUrl = m_WXInfoMap[openID].szHeadImgUrl;
    }

	OnDBLogonResult(dwContextID, m_WXInfoMap[openID].lUserID, ACCOUNT_TYPE_WX);
    //CreateEntity(dwContextID, m_WXInfoMap[openID].lUserID,ACCOUNT_TYPE_WX, m_WXInfoMap[openID].szName, m_WXInfoMap[openID].szHeadImgUrl, openID);
    return true;
}

//登录失败
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize) {
    //判断在线
    ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
    if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

    //变量定义
    CMD_MB_LogonFailure LogonFailure;
    ZeroMemory(&LogonFailure,sizeof(LogonFailure));
    DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

    Json::Value root;
    root["accountType"] = 0;
    root["resultCode"] = pLogonFailure->lResultCode;
    root["describe"] = Util::StringConvert::UnicodeToUtf8(pLogonFailure->szDescribeString);
    std::string data = m_FastWriter.write(root);

    //构造数据
    //LogonFailure.lResultCode=pLogonFailure->lResultCode;
    //lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

    //发送数据
    //WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
    //WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
    //m_pITCPNetworkEngine->SendData(dwContextID, MDM_MB_LOGON, SUB_MB_LOGON_FAILURE, &LogonFailure, wSendSize);
    m_pITCPNetworkEngine->SendData(dwContextID, MDM_MB_LOGON, SUB_MB_LOGON_FAILURE,(void*)data.c_str(),data.size());

    //关闭连接
    m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

    return true;
}

Position CAttemperEngineSink::GetRandPosition() {
    bool bNextRand = false;
    int x = 0;
    int y = 0;
    do {
        bNextRand = false;
        //  x = rand() % 640;
        // y = rand() % 320;
        // x = rand() % 2 == 0 ? x : -x;
        //y = rand() % 2 == 0 ? y : -y;
        x = rand() % (WORLD_WIDTH- 2*ENTITY_RADIUS)+ENTITY_RADIUS;
        y = rand() % (WORLD_HEIGHT- 2*ENTITY_RADIUS)+ ENTITY_RADIUS;
        for (auto iter = m_EntityMap.begin(); iter != m_EntityMap.end(); iter++) {
            EntityInfo& entity = iter->second;
			if (entity.bDead) continue;
			//if (entity.bGhostDead) continue;
            //if (entity.bDead&&entity.bGhostMode == false) continue;
            float distance = sqrt(pow(entity.x - x, 2) + pow(entity.y - y, 2));
            if (distance < 5*ENTITY_RADIUS) {
                bNextRand = true;
                break;
            }
        }
    } while (bNextRand);
    return Position(x, y);
}


pplx::task<Json::Value> CAttemperEngineSink::RequestUserInfo(const std::string& access_token, const std::string& open_id, const std::string& refresh_token, int expires_in) {
    uri_builder builder;
    builder.append_path(L"sns/userinfo");
    builder.append_query(L"access_token", StringToWString(access_token));
    builder.append_query(L"openid", StringToWString(open_id));

    http_client client(s_kBaseUrl);
    int nStart = GetTickCount();
    return client.request(methods::GET, builder.to_string()).then([&](http_response resp) {
        return resp.extract_utf8string(true);
    }).then([access_token, refresh_token, open_id, expires_in](string result) {
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(result,root) == false) {
            cout << "parse error" << endl;
            throw exception("parse json error");
        }
        root["access_token"] = access_token;
        root["refresh_token"] = refresh_token;
        root["openid"] = open_id;
        root["expires_in"] = expires_in;
        return root;
    });
}
pplx::task<Json::Value>  CAttemperEngineSink::RequestAccessToken(DWORD dwSocketID,std::string code) {
    //构造地址
    uri_builder builder;
    builder.append_path(L"sns/oauth2/access_token");
    builder.append_query(L"appid", s_kAppID);
    builder.append_query(L"secret", s_kAppSecret);
    builder.append_query(L"code", StringToWString(code));
    builder.append_query(L"grant_type", L"authorization_code");
    http_client client(s_kBaseUrl);
    int nStart = GetTickCount();
    return client.request(methods::GET, builder.to_string()).then([&](http_response resp) {
        // CTraceService::TraceStringEx(TraceLevel_Info, TEXT("needTime:%d"), (int)(GetTickCount() - nStart));
        return resp.extract_utf8string(true);
    }).then([&](string result) {
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(result, root) == false) {
            throw exception("parse result error");
        }
        if (root["errcode"].isNull() == false) {
            throw exception(root["errmsg"].asCString());
        }
        string access_token = root["access_token"].asString();
        int nExpires_in = root["expires_in"].asInt();
        string refresh_token = root["refresh_token"].asString();
        string openid = root["openid"].asString();
        string scope = root["scope"].asString();
        if (root["unionid"].isNull() == false) {
            string unionid = root["unionid"].asString();
        }
        return RequestUserInfo(access_token, openid, refresh_token, nExpires_in);
    }).then([=](pplx::task<Json::Value> t) {
        return HandleRestSdkException(dwSocketID,t);
    });

}
pplx::task<Json::Value>  CAttemperEngineSink::RequestRefreshToken(DWORD dwSocketID, std::string refresh_token) {
    //构造地址
    uri_builder builder;
    builder.append_path(L"sns/oauth2/refresh_token");
    builder.append_query(L"appid", s_kAppID);
    builder.append_query(L"refresh_token", StringToWString(refresh_token));
    builder.append_query(L"grant_type", L"refresh_token");
    http_client client(s_kBaseUrl);
    int nStart = GetTickCount();
    return client.request(methods::GET, builder.to_string()).then([&](http_response resp) {
        //CTraceService::TraceStringEx(TraceLevel_Info, TEXT("needTime:%d"), (int)(GetTickCount() - nStart));
        return resp.extract_utf8string(true);
    }).then([&](string result) {
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(result.c_str(), root) == false) {
            throw exception("parse result error");
        }
        if (root["errcode"].isNull() == false) {
            throw exception(root["errmsg"].asCString());
        }
        string access_token = root["access_token"].asString();
        int nExpires_in = root["expires_in"].asInt();
        string refresh_token = root["refresh_token"].asString();
        string openid = root["openid"].asString();
        string scope = root["scope"].asString();
        if (root["unionid"].isNull() == false) {
            string unionid = root["unionid"].asString();
        }

        return RequestUserInfo(access_token, openid, refresh_token, nExpires_in);
    }).then([=](pplx::task<Json::Value> t) {
        return HandleRestSdkException(dwSocketID, t);
    });

}
pplx::task<Json::Value> CAttemperEngineSink::HandleRestSdkException(DWORD dwSocketID, pplx::task<Json::Value>& t) {

    bool bNoException = true;
    string errMsg;
    try {
        t.get();
    } catch (const std::exception& ex) {
        bNoException = false;
        errMsg = ex.what();
    }

    //如果有异常，直接返回给玩家微信登录失败消息
    if (bNoException == false) {
        //CTraceService::TraceStringEx(TraceLevel_Warning, TEXT("微信登录失败，原因:%s"), CString(errMsg.c_str()));
        Json::Value error_json;
        error_json["describe"] = errMsg;
        error_json["accountType"] = ACCOUNT_TYPE_WX;
        std::string data = error_json.toStyledString();
        m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON, SUB_MB_LOGON_FAILURE, (void*)data.c_str(), data.size());
        return pplx::task_from_result<Json::Value>(error_json);
    }
    Json::Value root = t.get();
    if (root["errcode"].isNull() && bNoException == true) {
        //成功
        cout << "success" << endl;
        string openid = root["openid"].asString();
        string nickname = root["nickname"].asString();
        int nSex = root["sex"].asInt();
        string province = root["province"].asString();
        string city = root["city"].asString();
        string country = root["country"].asString();
        string headimgurl = root["headimgurl"].asString();
        string unionid = root["unionid"].asString();
        int nExpiresIn = root["expires_in"].asInt();
        string access_token = root["access_token"].asString();
        string refresh_token = root["refresh_token"].asString();

        DBR_MB_LogonWX LogonWX;
        ZeroMemory(&LogonWX, sizeof(LogonWX));

        //附加信息
        LogonWX.pBindParameter = (m_pBindParameter + LOWORD(dwSocketID));
        LogonWX.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
        LogonWX.nSex = nSex;
        LogonWX.nExpiresIn = nExpiresIn;
        strcpy(LogonWX.szOpenID, openid.c_str());
        strcpy(LogonWX.szUnionID, unionid.c_str());
        Util::StringConvert::Utf8ToUnicode(LogonWX.szNickName, CountArray(LogonWX.szNickName), nickname.c_str());

        //strcpy(LogonWX.szNickName, nickname.c_str());
        strcpy(LogonWX.szHeadImgUrl, headimgurl.c_str());
        strcpy(LogonWX.szProvince, root["province"].asCString());
        strcpy(LogonWX.szCity, root["city"].asCString());
        strcpy(LogonWX.szCountry, root["country"].asCString());
        //strcpy(LogonWX.szPrivilege, root["privilege"].asCString());
        std::string privilege = "";
        strcpy(LogonWX.szPrivilege, privilege.c_str());
        strcpy(LogonWX.szAccessToken, access_token.c_str());
        strcpy(LogonWX.szRefreshToken, refresh_token.c_str());
        m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_WX, dwSocketID, &LogonWX, sizeof(LogonWX));
    }

    return t;
}

pplx::task<Json::Value>  CAttemperEngineSink::RequestSessionKey(DWORD dwSocketID, std::string code, std::string name, std::string avatarUrl) {
	//构造地址
	uri_builder builder;
	builder.append_path(L"sns/jscode2session");
	builder.append_query(L"appid", s_kAppID);
	builder.append_query(L"secret", s_kAppSecret);
	builder.append_query(L"js_code", StringToWString(code));
	builder.append_query(L"grant_type", L"authorization_code");
	http_client client(s_kBaseUrl);
	int nStart = GetTickCount();
	return client.request(methods::GET, builder.to_string()).then([&](http_response resp) {
		// CTraceService::TraceStringEx(TraceLevel_Info, TEXT("needTime:%d"), (int)(GetTickCount() - nStart));
		return resp.extract_utf8string(true);
	}).then([&](string result) {
		Json::Value root;
		Json::Reader reader;
		if (reader.parse(result, root) == false) {
			throw exception("parse result error");
		}
		if (root["errcode"].isNull() == false) {
			throw exception(root["errmsg"].asCString());
		}

		root["headImgUrl"] = avatarUrl;
		root["nickname"] = name;
		return root;
	}).then([=](pplx::task<Json::Value> t) {
		return HandleRestSdkJSException(dwSocketID, t);
	});

}

pplx::task<Json::Value> CAttemperEngineSink::HandleRestSdkJSException(DWORD dwSocketID, pplx::task<Json::Value>& t) {

	bool bNoException = true;
	string errMsg;
	try {
		t.get();
	}
	catch (const std::exception& ex) {
		bNoException = false;
		errMsg = ex.what();
	}

	//如果有异常，直接返回给玩家微信登录失败消息
	if (bNoException == false) {
		//CTraceService::TraceStringEx(TraceLevel_Warning, TEXT("微信登录失败，原因:%s"), CString(errMsg.c_str()));
		Json::Value error_json;
		error_json["describe"] = errMsg;
		error_json["accountType"] = 2;
		std::string data = error_json.toStyledString();
		m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON, SUB_MB_LOGON_FAILURE, (void*)data.c_str(), data.size());
		return pplx::task_from_result<Json::Value>(error_json);
	}
	Json::Value root = t.get();
	if (root["errcode"].isNull() && bNoException == true) {
		//成功
		cout << "success" << endl;
		string openid = root["openid"].asString();
		string nickname = root["nickname"].asString();
		string headimgurl = root["headimgurl"].asString();
		//int nSex = root["sex"].asInt();
		//string province = root["province"].asString();
		//string city = root["city"].asString();
		//string country = root["country"].asString();
		//string unionid = root["unionid"].asString();

		DBR_MB_LogonWX LogonWX;
		ZeroMemory(&LogonWX, sizeof(LogonWX));

		//附加信息
		LogonWX.pBindParameter = (m_pBindParameter + LOWORD(dwSocketID));
		LogonWX.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
		//LogonWX.nSex = nSex;
		strcpy(LogonWX.szOpenID, openid.c_str());
		//strcpy(LogonWX.szUnionID, unionid.c_str());
		Util::StringConvert::Utf8ToUnicode(LogonWX.szNickName, CountArray(LogonWX.szNickName), nickname.c_str());
		strcpy(LogonWX.szHeadImgUrl, headimgurl.c_str());
		//strcpy(LogonWX.szProvince, root["province"].asCString());
		//strcpy(LogonWX.szCity, root["city"].asCString());
		//strcpy(LogonWX.szCountry, root["country"].asCString());
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_WX, dwSocketID, &LogonWX, sizeof(LogonWX));
	}

	return t;
}