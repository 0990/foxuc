#include "StdAfx.h"
#include "TraceService.h" //
#include "TCPNetworkEngine.h"
#include "Log.h"
//#include "caes.h"
//////////////////////////////////////////////////////////////////////////
//�궨��

#define TIME_BREAK_READY			9000L								//�ж�ʱ��
#define TIME_BREAK_CONNECT			9000L								//�ж�ʱ��
#define TIME_DETECT_SOCKET			30000L								//���ʱ��

//////////////////////////////////////////////////////////////////////////

//��������
#define QUEUE_SEND_REQUEST			1									//���ͱ�ʶ
#define QUEUE_SAFE_CLOSE			2									//��ȫ�ر�
#define QUEUE_ALLOW_BATCH			3									//����Ⱥ��
#define QUEUE_DETECT_SOCKET			4									//�������

//��������ṹ
struct tagSendDataRequest {
    WORD							wMainCmdID;							//��������
    WORD							wSubCmdID;							//��������
    DWORD							dwSocketID;							//��������
    WORD							wDataSize;							//���ݴ�С
    BYTE                            cbBatchMask;
    BYTE							cbSendBuf[SOCKET_TCP_BUFFER];		//���ͻ���
};

//����Ⱥ��
struct tagAllowBatchSend {
    DWORD							dwSocketID;							//��������
    BYTE							cbAllow;							//�����־
};

//��ȫ�ر�
struct tagSafeCloseSocket {
    DWORD							dwSocketID;							//��������
};

//���ṹ��Ϣ
struct TCP_KN_DetectSocket {
    DWORD								dwSendTickCount;					//����ʱ��
    DWORD								dwRecvTickCount;					//����ʱ��
};

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLapped::COverLapped (enOperationType OperationType) : m_OperationType (OperationType) {
    memset (&m_WSABuffer, 0, sizeof (m_WSABuffer));
    memset (&m_OverLapped, 0, sizeof (m_OverLapped));
}

//��������
COverLapped::~COverLapped() {
}

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLappedSend::COverLappedSend() : COverLapped (OperationType_Send) {
    m_WSABuffer.len = 0;
    m_WSABuffer.buf = (char *) m_cbBuffer;
}

//��������
COverLappedSend::~COverLappedSend() {
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketItem::CServerSocketItem (WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink)
    : m_wIndex (wIndex), m_pIServerSocketItemSink (pIServerSocketItemSink) {
    m_wRountID = 1;
    m_wRecvSize = 0;
    m_cbSendRound = 0;
    m_cbRecvRound = 0;
    m_bNotify = true;
    m_bRecvIng = false;
    m_bCloseIng = false;
    m_bAllowBatch = true;
    m_bForeground = true;
    m_dwSendXorKey = 0;
    m_dwRecvXorKey = 0;
    m_dwClientAddr = 0;
    m_dwConnectTime = 0;
    m_dwSendTickCount = 0;
    m_dwRecvTickCount = 0;
    m_dwSendPacketCount = 0;
    m_dwRecvPacketCount = 0;
    m_dwKNRecvTickCount = 0;
    m_hSocket = INVALID_SOCKET;
    m_connectType = CHECKING;
}

//��������
CServerSocketItem::~CServerSocketItem (void) {
    //ɾ�������ص� IO
    INT_PTR iCount = m_OverLappedSendFree.GetCount();

    for (INT_PTR i = 0; i < iCount; i++) {
        delete m_OverLappedSendFree[i];
    }

    m_OverLappedSendFree.RemoveAll();
    //ɾ����ص� IO
    iCount = m_OverLappedSendActive.GetCount();

    for (INT_PTR i = 0; i < iCount; i++) {
        delete m_OverLappedSendActive[i];
    }

    m_OverLappedSendActive.RemoveAll();
    return;
}

//���ӳ��
WORD CServerSocketItem::SeedRandMap (WORD wSeed) {
    DWORD dwHold = wSeed;
    return (WORD) ( (dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//ӳ�䷢������
BYTE CServerSocketItem::MapSendByte (BYTE const cbData) {
    BYTE cbMap = g_SendByteMap[ (BYTE) (cbData + m_cbSendRound)];
    m_cbSendRound += 3;
    return cbMap;
}

//ӳ���������
BYTE CServerSocketItem::MapRecvByte (BYTE const cbData) {
    BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
    m_cbRecvRound += 3;
    return cbMap;
}

//��ȡ���ͽṹ
COverLappedSend * CServerSocketItem::GetSendOverLapped() {
    //Ѱ�ҿ��нṹ
    COverLappedSend * pOverLappedSend = NULL;
    INT_PTR nFreeCount = m_OverLappedSendFree.GetCount();

    if (nFreeCount > 0) {
        pOverLappedSend = m_OverLappedSendFree[nFreeCount - 1];
        ASSERT (pOverLappedSend != NULL);
        m_OverLappedSendFree.RemoveAt (nFreeCount - 1);
        m_OverLappedSendActive.Add (pOverLappedSend);
        return pOverLappedSend;
    }

    //�½����ͽṹ
    try {
        pOverLappedSend = new COverLappedSend;
        ASSERT (pOverLappedSend != NULL);
        m_OverLappedSendActive.Add (pOverLappedSend);
        return pOverLappedSend;
    }

    catch (...) {}

    return NULL;
}

//�󶨶���
DWORD CServerSocketItem::Attach (SOCKET hSocket, DWORD dwClientAddr) {
    //Ч������
    ASSERT (dwClientAddr != 0);
    ASSERT (m_bRecvIng == false);
    ASSERT (IsValidSocket() == false);
    ASSERT (hSocket != INVALID_SOCKET);
    //���ñ���
    m_bNotify = false;
    m_bRecvIng = false;
    m_bCloseIng = false;
    m_bForeground = true;
    m_hSocket = hSocket;
    m_dwClientAddr = dwClientAddr;
    m_dwRecvTickCount = GetTickCount();
    m_dwKNRecvTickCount = GetTickCount();
    m_dwConnectTime = (DWORD) time (NULL);
    //����֪ͨ
    m_pIServerSocketItemSink->OnSocketAcceptEvent (this);
    return GetSocketID();
}

//���ͺ���
bool CServerSocketItem::SendData (WORD wMainCmdID, WORD wSubCmdID, WORD wRountID) {
    if (m_connectType == WEBSOCKET) {
        ASSERT(m_bCloseIng == false);
        ASSERT(m_wRountID == wRountID);
        ASSERT(m_dwRecvPacketCount != 0);
        ASSERT(m_dwRecvTickCount != 0);
        ASSERT(IsValidSocket() == true);

        //Ч��״̬
        if (m_bCloseIng == true) {
            return false;
        }

        if (m_wRountID != wRountID) {
            return false;
        }

        if (m_dwRecvPacketCount == 0) {
            return false;
        }

        if (IsValidSocket() == false) {
            return false;
        }

        Json::Value root;
        root["mainID"] = wMainCmdID;
        root["subID"] = wSubCmdID;
        std::string data = root.toStyledString();
		std::string sendData = data;
        //std::string sendData = CAES::Encrypt(data, m_strAesKey, m_strAesKey);
        char cbSendData[SOCKET_TCP_BUFFER];
        int encodesize = zl::net::ws::encodeFrame (zl::net::ws::WsFrameType::WS_TEXT_FRAME, sendData.c_str(), sendData.size(), cbSendData, SOCKET_TCP_BUFFER);
        SendRawData (cbSendData, encodesize);
    } else {
        //Ч��״̬
        if (m_bCloseIng == true) {
            return false;
        }

        if (m_wRountID != wRountID) {
            return false;
        }

        if (m_dwRecvPacketCount == 0) {
            return false;
        }

        if (IsValidSocket() == false) {
            return false;
        }

        //Ѱ�ҷ��ͽṹ
        COverLappedSend * pOverLappedSend = GetSendOverLapped();
        ASSERT (pOverLappedSend != NULL);

        if (pOverLappedSend == NULL) {
            return false;
        }

        //��������
        TCP_Head * pHead = (TCP_Head *) pOverLappedSend->m_cbBuffer;
        pHead->CommandInfo.wMainCmdID = wMainCmdID;
        pHead->CommandInfo.wSubCmdID = wSubCmdID;
        WORD wSendSize = EncryptBuffer (pOverLappedSend->m_cbBuffer, sizeof (TCP_Head), sizeof (pOverLappedSend->m_cbBuffer));
        pOverLappedSend->m_WSABuffer.len = wSendSize;

        //��������
        if (m_OverLappedSendActive.GetCount() == 1) {
            DWORD dwThancferred = 0;
            int iRetCode = WSASend (m_hSocket, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL);

            if ( (iRetCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING)) {
                OnSendCompleted (pOverLappedSend, 0L);
                return false;
            }
        }
    }

    return true;
}

//���ͺ���
bool CServerSocketItem::SendData (void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID) {
    //Ч�����
    ASSERT(wDataSize <= SOCKET_TCP_BUFFER);

    //Ч��״̬
    if (m_bCloseIng == true) {
        return false;
    }

    if (m_wRountID != wRountID) {
        return false;
    }

    if (m_dwRecvPacketCount == 0) {
        return false;
    }

    if (IsValidSocket() == false) {
        return false;
    }

    if (wDataSize > SOCKET_TCP_BUFFER) {
        // LOG_WARN(_T("SendData,wDataSize%d > SOCKET_TCP_BUFFER32768,mainID:%d,subID:%d"), (int)wDataSize, (int)wMainCmdID, (int)wSubCmdID);
        return false;
    }

    if (m_connectType == WEBSOCKET) {
        if (wDataSize == 0) {
            return SendData (wMainCmdID, wSubCmdID, wRountID);
        }

        std::string data ( (char*) pData, wDataSize);
        Json::Reader reader;
        Json::Value jsonData;
        Json::Value root;

        if (!reader.parse (data, jsonData)) {
            // LOG_WARN(_T("SendData json parse error,mainID:%d,subID:%d,%s"), (int)wMainCmdID, (int)wSubCmdID, data.c_str());
            return false;
        }

        root["mainID"] = wMainCmdID;
        root["subID"] = wSubCmdID;
        root["data"] = jsonData;
        Json::FastWriter writer;
        std::string sendDataTemp = writer.write(root);
        //std::string sendDataTemp = root.toStyledString();
		//std::string sendData = CAES::Encrypt(sendDataTemp, m_strAesKey, m_strAesKey);
		std::string sendData = sendDataTemp;
        char cbSendData[SOCKET_TCP_BUFFER];
        int encodesize = zl::net::ws::encodeFrame (zl::net::ws::WsFrameType::WS_TEXT_FRAME, sendData.c_str(), sendData.size(), cbSendData, SOCKET_TCP_BUFFER);

        if (encodesize >= SOCKET_TCP_BUFFER) {
            // LOG_WARN(_T("SendData,encodesize%d > SOCKET_TCP_BUFFER32768,mainID:%d,subID:%d"), (int)encodesize, (int)wMainCmdID, (int)wSubCmdID);
            return false;
        }
        SendRawData (cbSendData, encodesize);
    } else {
        //Ѱ�ҷ��ͽṹ
        COverLappedSend * pOverLappedSend = GetSendOverLapped();
        ASSERT (pOverLappedSend != NULL);

        if (pOverLappedSend == NULL) {
            return false;
        }

        //��������
        TCP_Head * pHead = (TCP_Head *) pOverLappedSend->m_cbBuffer;
        pHead->CommandInfo.wMainCmdID = wMainCmdID;
        pHead->CommandInfo.wSubCmdID = wSubCmdID;

        if (wDataSize > 0) {
            ASSERT (pData != NULL);
            memcpy (pHead + 1, pData, wDataSize);
        }

        WORD wSendSize = EncryptBuffer (pOverLappedSend->m_cbBuffer, sizeof (TCP_Head) + wDataSize, sizeof (pOverLappedSend->m_cbBuffer));
        pOverLappedSend->m_WSABuffer.len = wSendSize;

        //��������
        if (m_OverLappedSendActive.GetCount() == 1) {
            DWORD dwThancferred = 0;
            int iRetCode = WSASend (m_hSocket, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL);

            if ( (iRetCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING)) {
                OnSendCompleted (pOverLappedSend, 0L);
                return false;
            }
        }
    }

    return true;
}

//Ͷ�ݽ���
bool CServerSocketItem::RecvData() {
    //Ч�����
    ASSERT (m_bRecvIng == false);
    ASSERT (m_hSocket != INVALID_SOCKET);

    //�жϹر�
    if (m_bCloseIng == true) {
        if (m_OverLappedSendActive.GetCount() == 0) {
            CloseSocket (m_wRountID);
        }

        return false;
    }

    //��������
    m_bRecvIng = true;
    DWORD dwThancferred = 0, dwFlags = 0;
    int iRetCode = WSARecv (m_hSocket, &m_OverLappedRecv.m_WSABuffer, 1, &dwThancferred, &dwFlags, &m_OverLappedRecv.m_OverLapped, NULL);

    if ( (iRetCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING)) {
        m_bRecvIng = false;
        CloseSocket (m_wRountID);
        return false;
    }

    return true;
}

//�ر�����
bool CServerSocketItem::CloseSocket (WORD wRountID) {
    //״̬�ж�
    if (m_wRountID != wRountID) {
        return false;
    }

    //�ر�����
    if (m_hSocket != INVALID_SOCKET) {
        closesocket (m_hSocket);
        m_hSocket = INVALID_SOCKET;
    }

    //�жϹر�
    if ( (m_bRecvIng == false) && (m_OverLappedSendActive.GetCount() == 0)) {
        OnCloseCompleted();
    }

    return true;
}

//���ùر�
bool CServerSocketItem::ShutDownSocket (WORD wRountID) {
    //edit by xujialong
    return true;

    //״̬�ж�
    if (m_wRountID != wRountID) {
        return false;
    }

    if (m_hSocket == INVALID_SOCKET) {
        return false;
    }

    //���ñ���
    if (m_bCloseIng == false) {
        m_bCloseIng = true;

        //if (m_OverLappedSendActive.GetCount() == 0) CloseSocket(wRountID);
        //edit by xujialong
        if (m_OverLappedSendActive.GetCount() == 0) {
            CloseSocket (wRountID);
        }
    }

    return true;
}

//����Ⱥ��
bool CServerSocketItem::AllowBatchSend (WORD wRountID, bool bAllowBatch) {
    //״̬�ж�
    if (m_wRountID != wRountID) {
        return false;
    }

    if (m_hSocket == INVALID_SOCKET) {
        return false;
    }

    //���ñ���
    m_bAllowBatch = bAllowBatch;
    return true;
}

//���ñ���
void CServerSocketItem::ResetSocketData() {
    //Ч��״̬
    ASSERT (m_hSocket == INVALID_SOCKET);
    //��������
    m_wRountID++;
    m_wRecvSize = 0;
    m_cbSendRound = 0;
    m_cbRecvRound = 0;
    m_dwSendXorKey = 0;
    m_dwRecvXorKey = 0;
    m_dwClientAddr = 0;
    m_dwConnectTime = 0;
    m_dwSendTickCount = 0;
    m_dwRecvTickCount = 0;
    m_dwSendPacketCount = 0;
    m_dwRecvPacketCount = 0;
    m_dwKNRecvTickCount = 0;
    //״̬����
    m_bNotify = true;
    m_bRecvIng = false;
    m_bCloseIng = false;
    m_bAllowBatch = true;
    m_bForeground = true;
    m_connectType = CHECKING;
    m_OverLappedSendFree.Append (m_OverLappedSendActive);
    m_OverLappedSendActive.RemoveAll();
    return;
}

//������ɺ���
bool CServerSocketItem::OnSendCompleted (COverLappedSend * pOverLappedSend, DWORD dwThancferred) {
    //Ч�����
    ASSERT (pOverLappedSend != NULL);
    ASSERT (m_OverLappedSendActive.GetCount() > 0);
    ASSERT (pOverLappedSend == m_OverLappedSendActive[0]);
    //�ͷŷ��ͽṹ
    m_OverLappedSendActive.RemoveAt (0);
    m_OverLappedSendFree.Add (pOverLappedSend);

    //���ñ���
    if (dwThancferred != 0) {
        m_dwSendTickCount = GetTickCount();
    }

    //�жϹر�
    if (m_hSocket == INVALID_SOCKET) {
        m_OverLappedSendFree.Append (m_OverLappedSendActive);
        m_OverLappedSendActive.RemoveAll();
        CloseSocket (m_wRountID);
        return true;
    }

    //������������
    if (m_OverLappedSendActive.GetCount() > 0) {
        DWORD dwThancferred = 0;
        pOverLappedSend = m_OverLappedSendActive[0];
        ASSERT (pOverLappedSend != NULL);
        int iRetCode = WSASend (m_hSocket, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL);

        if ( (iRetCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING)) {
            m_OverLappedSendFree.Append (m_OverLappedSendActive);
            m_OverLappedSendActive.RemoveAll();
            CloseSocket (m_wRountID);
            return false;
        }

        return true;
    }

    //�жϹر�
    if (m_bCloseIng == true) {
        // LOG_WARN(_T("closeIng = true,close socket"));
        CloseSocket (m_wRountID);
    }

    return true;
}

//������ɺ���
bool CServerSocketItem::OnRecvCompleted (COverLappedRecv * pOverLappedRecv, DWORD dwThancferred) {
    //Ч������
    ASSERT (m_bRecvIng == true);
    //���ñ���
    m_bRecvIng = false;
    m_dwRecvTickCount = GetTickCount();

    //�жϹر�
    if (m_hSocket == INVALID_SOCKET) {
        CloseSocket (m_wRountID);
        return true;
    }

    //��������
    int iRetCode = recv (m_hSocket, (char *) m_cbRecvBuf + m_wRecvSize, sizeof (m_cbRecvBuf) - m_wRecvSize, 0);

    if (iRetCode <= 0) {
        CloseSocket (m_wRountID);
        return true;
    }

    //�������
    m_wRecvSize += iRetCode;

    if (m_connectType == CHECKING) {
        if (CheckIfWinSocket() == true) {
            m_connectType = WINSOCKET;
            return RecvData();
        } else {
            m_connectType = HANDSHAKING;
        }
    }

    if (m_connectType == HANDSHAKING) {
        //try to handshake for websocket
        const char kDoubleCRLF[] = "\r\n\r\n";
        char* recvChar = (char*)m_cbRecvBuf;
        const char* pos = std::search(recvChar, recvChar + m_wRecvSize, kDoubleCRLF, kDoubleCRLF + 4);

        if (pos == recvChar + m_wRecvSize) {
            std::string data((char*)m_cbRecvBuf, m_wRecvSize);
            //FILE_LOG("handshake request data is not ready,data:%s",data.c_str());
            return RecvData();
        }
        //std::string data((char*)m_cbRecvBuf, m_wRecvSize);
        //LOG_PRINT("handshake request:\n%s", data.c_str());
        zl::net::ByteBuffer handshakeBuffer;
        handshakeBuffer.write(recvChar, m_wRecvSize);
        bool success = handshake(&handshakeBuffer);

        if (success == true) {
            m_connectType = WEBSOCKET;
            m_wRecvSize = 0;
            return RecvData();
        } else {
            CloseSocket(m_wRountID);
            return false;
        }
    }

    switch (m_connectType) {
    case WINSOCKET: {
        BYTE cbBuffer[SOCKET_TCP_BUFFER];
        TCP_Head * pHead = (TCP_Head *) m_cbRecvBuf;

        //��������
        try {
            while (m_wRecvSize >= sizeof (TCP_Head)) {
                //Ч������
                WORD wPacketSize = pHead->TCPInfo.wPacketSize;

                if (wPacketSize > SOCKET_TCP_BUFFER) {
                    throw TEXT ("���ݰ�����");
                }

                if (wPacketSize < sizeof (TCP_Head)) {
                    throw TEXT ("���ݰ��Ƿ�");
                }

                //if (pHead->TCPInfo.cbVersion != SOCKET_TCP_VER) throw TEXT("���ݰ��汾����");
                if (pHead->TCPInfo.cbDataKind != SOCKET_TCP_VER) {
                    throw TEXT ("���ݰ��汾����");
                }

                if (m_wRecvSize < wPacketSize) {
                    break;
                }

                //��ȡ����
                CopyMemory (cbBuffer, m_cbRecvBuf, wPacketSize);
                WORD wRealySize = CrevasseBuffer (cbBuffer, wPacketSize);
                ASSERT (wRealySize >= sizeof (TCP_Head));
                m_dwRecvPacketCount++;
                //��������
                WORD wDataSize = wRealySize - sizeof (TCP_Head);
                void * pDataBuffer = cbBuffer + sizeof (TCP_Head);
                TCP_Command Command = ( (TCP_Head *) cbBuffer)->CommandInfo;

                //�ں�����
                if (Command.wMainCmdID == MDM_KN_COMMAND) {
                    switch (Command.wSubCmdID) {
                    case SUB_KN_DETECT_SOCKET: {	//������
                        break;
                    }

                    default: {
                        throw TEXT ("�Ƿ�������");
                    }
                    }
                }

                else {
                    //��Ϣ����
                    m_pIServerSocketItemSink->OnSocketReadEvent (Command, pDataBuffer, wDataSize, this);
                }

                //ɾ����������
                m_wRecvSize -= wPacketSize;
                MoveMemory (m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);
            }
        }

        catch (...) {
            CloseSocket (m_wRountID);
            return false;
        }

        break;
    }

    case WEBSOCKET: {
        if (HandleWebsocketRecv() == false) {
            CloseSocket(m_wRountID);
            return false;
        }

        break;
    }
    }

    return RecvData();
}
bool CServerSocketItem::isWebSocket() {
    return m_connectType == WEBSOCKET;
}
bool CServerSocketItem::isWinSocket() {
    return m_connectType == WINSOCKET;
}
bool CServerSocketItem::HandleWebsocketRecv() {
    using namespace zl::net::ws;

    if (m_wRecvSize > SOCKET_TCP_BUFFER) {
        //CloseSocket(m_wRountID);
        //LOG_WARN(_T("error,receive data len > SOCKET_TCP_BUFFER,%d>32768"), (int)m_wRecvSize);
        return false;
    }

    try {
        while (m_wRecvSize > 0) {
            char outbuf[SOCKET_TCP_BUFFER];
            int outlen = 0;
            int frameSize = 0;
            WsFrameType type = decodeFrame ( (char*) m_cbRecvBuf, m_wRecvSize, outbuf, &outlen, &frameSize);

            switch (type) {
            case WS_INCOMPLETE_TEXT_FRAME:
            case WS_INCOMPLETE_BINARY_FRAME:
            case WS_INCOMPLETE_FRAME: {
                return true;
            }

            case WS_TEXT_FRAME:
            case WS_BINARY_FRAME: {
                m_dwRecvPacketCount++;
                TCP_Command Command;
                //std::string data(outbuf, outlen);
                std::string outData(outbuf, outlen);
				//std::string data = CAES::Decrypt(outData, m_strAesKey, m_strAesKey);
				std::string data = outData;
                Json::Reader reader;
                Json::Value root;

                if (reader.parse (data, root)) {
                    Command.wMainCmdID = root["mainID"].asInt();
                    Command.wSubCmdID = root["subID"].asInt();
                } else {
                    //LOG_WARN(_T("receive's data parse json error"));
                    throw TEXT ("parse error");
                }
                //��ҽ���̨��״̬����
                m_dwKNRecvTickCount = GetTickCount();
                if (m_bForeground == false) {
                    setForeground(true);
                }
                //�ں�����
                if (Command.wMainCmdID == MDM_KN_COMMAND) {
                    switch (Command.wSubCmdID) {
                    case SUB_KN_DETECT_SOCKET: {	//������
                        SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, m_wRountID);
                        break;
                    }
                    default: {
                        throw TEXT ("�Ƿ�������");
                    }
                    }
                } else {
                    //��Ϣ����
                    m_pIServerSocketItemSink->OnSocketReadEvent(Command, (void*)data.c_str(), data.size(), this);
                }

                break;
            }

            case WS_PING_FRAME: {
                //printf ("receive ping frame,framesize:%d\n", frameSize);
                char cbSendData[SOCKET_TCP_BUFFER];
                int encodesize = zl::net::ws::encodeFrame (zl::net::ws::WsFrameType::WS_PONG_FRAME, NULL, 0, cbSendData, SOCKET_TCP_BUFFER);
                SendRawData (cbSendData, encodesize);
                break;
            }

            case WS_PONG_FRAME: {
                // printf ("receive pong frame,framesize:%d\n", frameSize);
                break;
            }

            case WS_CLOSE_FRAME: {
                //printf ("receive close frame\n");
                //CloseSocket (m_wRountID);
                return false;
            }

            default: {
                //LOG_WARN(_T("receive unknow frame,close socket,type:%d"), (int)type);
                //CloseSocket (m_wRountID);
                return false;
            }
            }

            //ɾ����������
            m_wRecvSize -= frameSize;
            MoveMemory (m_cbRecvBuf, m_cbRecvBuf + frameSize, m_wRecvSize);
        }
    } catch (...) {
        //LOG_WARN(_T("parse json error or could not find mainID,subID,close socket"));
        //CloseSocket (m_wRountID);
        return false;
    }

    return true;
}

bool CServerSocketItem::sendPingFrame() {
    char cbSendData[SOCKET_TCP_BUFFER];
    int encodesize = zl::net::ws::encodeFrame(zl::net::ws::WsFrameType::WS_PING_FRAME, NULL, 0, cbSendData, SOCKET_TCP_BUFFER);
    return SendRawData(cbSendData, encodesize);
}

bool CServerSocketItem::setForeground(bool foreground) {
    m_bForeground = foreground;
    m_pIServerSocketItemSink->OnSocketForegroundEvent(this);
    return true;
}
bool CServerSocketItem::CheckIfWinSocket() {
    BYTE cbBuffer[SOCKET_TCP_BUFFER];
    TCP_Head * pHead = (TCP_Head *) m_cbRecvBuf;

    //��������
    try {
        while (m_wRecvSize >= sizeof (TCP_Head)) {
            //Ч������
            WORD wPacketSize = pHead->TCPInfo.wPacketSize;

            if (wPacketSize > SOCKET_TCP_BUFFER) {
                throw TEXT ("���ݰ�����");
            }

            if (wPacketSize < sizeof (TCP_Head)) {
                throw TEXT ("���ݰ��Ƿ�");
            }

            //if (pHead->TCPInfo.cbVersion != SOCKET_TCP_VER) throw TEXT("���ݰ��汾����");
            if (pHead->TCPInfo.cbDataKind != SOCKET_TCP_VER) {
                throw TEXT ("���ݰ��汾����");
            }

            if (m_wRecvSize < wPacketSize) {
                break;
            }

            //��ȡ����
            CopyMemory (cbBuffer, m_cbRecvBuf, wPacketSize);
            WORD wRealySize = CrevasseBuffer (cbBuffer, wPacketSize);
            ASSERT (wRealySize >= sizeof (TCP_Head));
            m_dwRecvPacketCount++;
            //��������
            WORD wDataSize = wRealySize - sizeof (TCP_Head);
            void * pDataBuffer = cbBuffer + sizeof (TCP_Head);
            TCP_Command Command = ( (TCP_Head *) cbBuffer)->CommandInfo;

            //�ں�����
            if (Command.wMainCmdID == MDM_KN_COMMAND) {
                switch (Command.wSubCmdID) {
                case SUB_KN_DETECT_SOCKET: {	//������
                    break;
                }

                default: {
                    throw TEXT ("�Ƿ�������");
                }
                }
            }

            else {
                //��Ϣ����
                m_pIServerSocketItemSink->OnSocketReadEvent (Command, pDataBuffer, wDataSize, this);
            }

            //ɾ����������
            m_wRecvSize -= wPacketSize;
            MoveMemory (m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);
        }
    }

    catch (...) {
        return false;
    }

    return true;
}
bool CServerSocketItem::handshake (zl::net::ByteBuffer* byteBuffer) {
    zl::net::HttpContext context;

    if (!context.parseRequest (byteBuffer) || context.request().method() != zl::net::HttpGet) { // ����ʧ�� ���� ����Get����
        //LOG_WARN(_T("parse handshake error,send close header"));
        std::string msg = "HTTP/1.1 400 Bad Request\r\n\r\n";
        SendRawData (msg.c_str(), msg.size());
        return false;
    }

    assert (context.gotAll());
    zl::net::HttpRequest& req = context.request();
    std::string query = req.query();
    if (query.empty() == false) {
        query = query.substr(1);
    }

    //if (CAES::Check(query)==false) {
    //    //LOG_WARN(_T("parse handshake error,have params,send close header"));
    //    std::string msg = "HTTP/1.1 400 Bad Request\r\n\r\n";
    //    SendRawData(msg.c_str(), msg.size());
    //    return false;
    //} else {
    //    m_strAesKey = query;
    //}

    std::string key = req.getHeader (zl::net::ws::kSecWebSocketKeyHeader);
    std::string ip = req.getHeader("X-Forwarded-For");
    if (ip.empty() == false) {
        //LOG_PRINT("real ip",ip.c_str());
        m_dwClientAddr = inet_addr(ip.c_str());
        //Attach(m_hSocket, inet_addr(ip.c_str()));
        m_pIServerSocketItemSink->OnSocketAcceptEvent(this);
    }
    std::string answer = zl::net::ws::makeHandshakeResponse (key.c_str(), req.getHeader (zl::net::ws::kSecWebSocketProtocolHeader));
    //printf ("response handshake:\n %s\n", answer.c_str());
    SendRawData (answer.c_str(), answer.size());
    return true;
}
bool CServerSocketItem::SendRawData (const char* data, int len) {
    //Ѱ�ҷ��ͽṹ
    COverLappedSend * pOverLappedSend = GetSendOverLapped();
    ASSERT (pOverLappedSend != NULL);

    if (pOverLappedSend == NULL) {
        return false;
    }

    for (int i = 0; i < len; i++) {
        pOverLappedSend->m_cbBuffer[i] = data[i];
    }

    pOverLappedSend->m_WSABuffer.len = len;

    //��������
    if (m_OverLappedSendActive.GetCount() == 1) {
        DWORD dwThancferred = 0;
        int iRetCode = WSASend (m_hSocket, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL);

        if ( (iRetCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING)) {
            OnSendCompleted (pOverLappedSend, 0L);
            return false;
        }
    }

    return true;
}
//�ر����֪ͨ
bool CServerSocketItem::OnCloseCompleted() {
    //Ч��״̬
    ASSERT (m_hSocket == INVALID_SOCKET);
    ASSERT (m_OverLappedSendActive.GetCount() == 0);
    //�ر��¼�
    ASSERT (m_bNotify == false);

    if (m_bNotify == false) {
        m_bNotify = true;
        m_pIServerSocketItemSink->OnSocketCloseEvent (this);
    }

    //״̬����
    ResetSocketData();
    return true;
}

//��������
WORD CServerSocketItem::EncryptBuffer (BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize) {
    WORD i = 0;
    //Ч�����
    ASSERT (wDataSize >= sizeof (TCP_Head));
    ASSERT (wDataSize <= (sizeof (TCP_Head) + SOCKET_TCP_BUFFER));
    ASSERT (wBufferSize >= (wDataSize + 2 * sizeof (DWORD)));
    //��������
    WORD wEncryptSize = wDataSize - sizeof (TCP_Info), wSnapCount = 0;

    if ( (wEncryptSize % sizeof (DWORD)) != 0) {
        wSnapCount = sizeof (DWORD) - wEncryptSize % sizeof (DWORD);
        memset (pcbDataBuffer + sizeof (TCP_Info) + wEncryptSize, 0, wSnapCount);
    }

    //Ч�������ֽ�ӳ��
    BYTE cbCheckCode = 0;

    for (i = sizeof (TCP_Info); i < wDataSize; i++) {
        cbCheckCode += pcbDataBuffer[i];
        pcbDataBuffer[i] = MapSendByte (pcbDataBuffer[i]);
    }

    //��д��Ϣͷ
    TCP_Head * pHead = (TCP_Head *) pcbDataBuffer;
    pHead->TCPInfo.cbCheckCode = ~cbCheckCode + 1;
    pHead->TCPInfo.wPacketSize = wDataSize;
    //pHead->TCPInfo.cbVersion = SOCKET_TCP_VER;
    pHead->TCPInfo.cbDataKind = SOCKET_TCP_VER;
    //��������
    DWORD dwXorKey = m_dwSendXorKey;
    WORD * pwSeed = (WORD *) (pcbDataBuffer + sizeof (TCP_Info));
    DWORD * pdwXor = (DWORD *) (pcbDataBuffer + sizeof (TCP_Info));
    WORD wEncrypCount = (wEncryptSize + wSnapCount) / sizeof (DWORD);

    for (i = 0; i < wEncrypCount; i++) {
        *pdwXor++ ^= dwXorKey;
        dwXorKey = SeedRandMap (*pwSeed++);
        dwXorKey |= ( (DWORD) SeedRandMap (*pwSeed++)) << 16;
        dwXorKey ^= g_dwPacketKey;
    }

    //���ñ���
    m_dwSendPacketCount++;
    m_dwSendXorKey = dwXorKey;
    return wDataSize;
}

//��������
WORD CServerSocketItem::CrevasseBuffer (BYTE pcbDataBuffer[], WORD wDataSize) {
    WORD i = 0;
    //Ч�����
    ASSERT (wDataSize >= sizeof (TCP_Head));
    ASSERT ( ( (TCP_Head *) pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);
    //��������
    WORD wSnapCount = 0;

    if ( (wDataSize % sizeof (DWORD)) != 0) {
        wSnapCount = sizeof (DWORD) - wDataSize % sizeof (DWORD);
        memset (pcbDataBuffer + wDataSize, 0, wSnapCount);
    }

    //��ȡ��Կ
    if (m_dwRecvPacketCount == 0) {
        ASSERT (wDataSize >= (sizeof (TCP_Head) + sizeof (DWORD)));

        if (wDataSize < (sizeof (TCP_Head) + sizeof (DWORD))) {
            throw TEXT ("���ݰ����ܳ��ȴ���");
        }

        m_dwRecvXorKey = * (DWORD *) (pcbDataBuffer + sizeof (TCP_Head));
        m_dwSendXorKey = m_dwRecvXorKey;
        MoveMemory (pcbDataBuffer + sizeof (TCP_Head), pcbDataBuffer + sizeof (TCP_Head) + sizeof (DWORD),
                    wDataSize - sizeof (TCP_Head) - sizeof (DWORD));
        wDataSize -= sizeof (DWORD);
        ( (TCP_Head *) pcbDataBuffer)->TCPInfo.wPacketSize -= sizeof (DWORD);
    }

    //��������
    DWORD dwXorKey = m_dwRecvXorKey;
    DWORD * pdwXor = (DWORD *) (pcbDataBuffer + sizeof (TCP_Info));
    WORD  * pwSeed = (WORD *) (pcbDataBuffer + sizeof (TCP_Info));
    WORD wEncrypCount = (wDataSize + wSnapCount - sizeof (TCP_Info)) / 4;

    for (i = 0; i < wEncrypCount; i++) {
        if ( (i == (wEncrypCount - 1)) && (wSnapCount > 0)) {
            BYTE * pcbKey = ( (BYTE *) & m_dwRecvXorKey) + sizeof (DWORD) - wSnapCount;
            CopyMemory (pcbDataBuffer + wDataSize, pcbKey, wSnapCount);
        }

        dwXorKey = SeedRandMap (*pwSeed++);
        dwXorKey |= ( (DWORD) SeedRandMap (*pwSeed++)) << 16;
        dwXorKey ^= g_dwPacketKey;
        *pdwXor++ ^= m_dwRecvXorKey;
        m_dwRecvXorKey = dwXorKey;
    }

    //Ч�������ֽ�ӳ��
    TCP_Head * pHead = (TCP_Head *) pcbDataBuffer;
    BYTE cbCheckCode = pHead->TCPInfo.cbCheckCode;;

    for (i = sizeof (TCP_Info); i < wDataSize; i++) {
        pcbDataBuffer[i] = MapRecvByte (pcbDataBuffer[i]);
        cbCheckCode += pcbDataBuffer[i];
    }

    if (cbCheckCode != 0) {
        throw TEXT ("���ݰ�Ч�������");
    }

    return wDataSize;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketRSThread::CServerSocketRSThread (void) {
    m_hCompletionPort = NULL;
}

//��������
CServerSocketRSThread::~CServerSocketRSThread (void) {
}

//���ú���
bool CServerSocketRSThread::InitThread (HANDLE hCompletionPort) {
    ASSERT (hCompletionPort != NULL);
    m_hCompletionPort = hCompletionPort;
    return true;
}

//���к���
bool CServerSocketRSThread::OnEventThreadRun() {
    //Ч�����
    ASSERT (m_hCompletionPort != NULL);
    //��������
    DWORD dwThancferred = 0;
    OVERLAPPED * pOverLapped = NULL;
    COverLapped * pSocketLapped = NULL;
    CServerSocketItem * pServerSocketItem = NULL;
    //�ȴ���ɶ˿�
    BOOL bSuccess = GetQueuedCompletionStatus (m_hCompletionPort, &dwThancferred, (PULONG_PTR) & pServerSocketItem, &pOverLapped, INFINITE);

    if ( (bSuccess == FALSE) && (GetLastError() != ERROR_NETNAME_DELETED)) {
        return false;
    }

    if ( (pServerSocketItem == NULL) && (pOverLapped == NULL)) {
        return false;
    }

    //�������
    ASSERT (pOverLapped != NULL);
    ASSERT (pServerSocketItem != NULL);
    pSocketLapped = CONTAINING_RECORD (pOverLapped, COverLapped, m_OverLapped);

    switch (pSocketLapped->GetOperationType()) {
    case OperationType_Recv: {	//SOCKET ���ݶ�ȡ
        COverLappedRecv * pOverLappedRecv = (COverLappedRecv *) pSocketLapped;
        CWHDataLocker lock (pServerSocketItem->GetSignedLock());
        pServerSocketItem->OnRecvCompleted (pOverLappedRecv, dwThancferred);
        break;
    }

    case OperationType_Send: {	//SOCKET ���ݷ���
        COverLappedSend * pOverLappedSend = (COverLappedSend *) pSocketLapped;
        CWHDataLocker lock (pServerSocketItem->GetSignedLock());
        pServerSocketItem->OnSendCompleted (pOverLappedSend, dwThancferred);
        break;
    }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketAcceptThread::CSocketAcceptThread (void) {
    m_hCompletionPort = NULL;
    m_pTCPSocketManager = NULL;
    m_hListenSocket = INVALID_SOCKET;
}

//��������
CSocketAcceptThread::~CSocketAcceptThread (void) {
}

//���ú���
bool CSocketAcceptThread::InitThread (HANDLE hCompletionPort, SOCKET hListenSocket, CTCPNetworkEngine * pTCPSocketManager) {
    ASSERT (hCompletionPort != NULL);
    ASSERT (pTCPSocketManager != NULL);
    ASSERT (hListenSocket != INVALID_SOCKET);
    m_hListenSocket = hListenSocket;
    m_hCompletionPort = hCompletionPort;
    m_pTCPSocketManager = pTCPSocketManager;
    return true;
}

//���к���
bool CSocketAcceptThread::OnEventThreadRun() {
    //Ч�����
    ASSERT (m_hCompletionPort != NULL);
    ASSERT (m_pTCPSocketManager != NULL);
    //���ñ���
    SOCKADDR_IN	SocketAddr;
    CServerSocketItem * pServerSocketItem = NULL;
    SOCKET hConnectSocket = INVALID_SOCKET;
    int nBufferSize = sizeof (SocketAddr);

    try {
        //��������
        hConnectSocket = WSAAccept (m_hListenSocket, (SOCKADDR *) & SocketAddr, &nBufferSize, NULL, NULL);

        if (hConnectSocket == INVALID_SOCKET) {
            return false;
        }

        //��ȡ����
        pServerSocketItem = m_pTCPSocketManager->ActiveSocketItem();

        if (pServerSocketItem == NULL) {
            throw TEXT ("�������Ӷ���ʧ��");
        }

        //�������
        CWHDataLocker lock (pServerSocketItem->GetSignedLock());
        pServerSocketItem->Attach (hConnectSocket, SocketAddr.sin_addr.S_un.S_addr);
        CreateIoCompletionPort ( (HANDLE) hConnectSocket, m_hCompletionPort, (ULONG_PTR) pServerSocketItem, 0);
        pServerSocketItem->RecvData();
    }

    catch (...) {
        //�������
        ASSERT (pServerSocketItem == NULL);

        if (hConnectSocket != INVALID_SOCKET)	{
            closesocket (hConnectSocket);
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketDetectThread::CSocketDetectThread (void) {
    m_dwTickCount = 0;;
    m_pTCPSocketManager = NULL;
}

//��������
CSocketDetectThread::~CSocketDetectThread (void) {
}

//���ú���
bool CSocketDetectThread::InitThread (CTCPNetworkEngine * pTCPSocketManager) {
    //Ч�����
    ASSERT (pTCPSocketManager != NULL);
    //���ñ���
    m_dwTickCount = 0L;
    m_pTCPSocketManager = pTCPSocketManager;
    return true;
}

//���к���
bool CSocketDetectThread::OnEventThreadRun() {
    //Ч�����
    ASSERT (m_pTCPSocketManager != NULL);
    //���ü��
    Sleep (500);
    m_dwTickCount += 500L;

    //�������
    if (m_dwTickCount > TIME_DETECT_SOCKET) {
        m_dwTickCount = 0L;
        m_pTCPSocketManager->DetectSocket();
    }

    return true;
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPNetworkEngine::CTCPNetworkEngine (void) {
    m_bService = false;
    m_wListenPort = 0;
    m_dwLastDetect = 0;
    m_wMaxSocketItem = 0;
    m_hCompletionPort = NULL;
    m_hServerSocket = INVALID_SOCKET;
    //AllocConsole();
    //freopen ("CONOUT$", "a+", stdout);
    return;
}

//��������
CTCPNetworkEngine::~CTCPNetworkEngine (void) {
    //ֹͣ����
    ConcludeService();
    //�ͷŴ洢����
    CServerSocketItem * pSocketItem = NULL;

    for (INT_PTR i = 0; i < m_StorageSocketItem.GetCount(); i++) {
        pSocketItem = m_StorageSocketItem[i];
        ASSERT (pSocketItem != NULL);
        SafeDelete (pSocketItem);
    }

    m_StorageSocketItem.RemoveAll();
    return;
}

//�ӿڲ�ѯ
void * CTCPNetworkEngine::QueryInterface (const IID & Guid, DWORD dwQueryVer) {
    QUERYINTERFACE (ITCPNetworkEngine, Guid, dwQueryVer);
    QUERYINTERFACE (IQueueServiceSink, Guid, dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX (ITCPNetworkEngine, Guid, dwQueryVer);
    return NULL;
}

//���ö˿�
WORD CTCPNetworkEngine::GetServicePort() {
    return m_wListenPort;
}

//��ǰ�˿�
WORD CTCPNetworkEngine::GetCurrentPort() {
    return m_wListenPort;
}

//���ýӿ�
bool CTCPNetworkEngine::SetTCPNetworkEngineEvent (IUnknownEx * pIUnknownEx) {
    //״̬�ж�
    if (m_bService == true) {
        CTraceService::TraceString (TEXT ("�������洦�ڷ���״̬���󶨲�������"), TraceLevel_Exception);
        return false;
    }

    IAttemperEngine* pIAttemperSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngine);
    ASSERT(pIAttemperSink != NULL);
    m_QueueServiceEvent.SetQueueService((CQueueService*)pIAttemperSink->GetQueue());

    ////���ýӿ�
    //if (m_QueueServiceEvent.SetQueueServiceSink (pIUnknownEx) == false)
    //{
    //    CTraceService::TraceString (TEXT ("���������봥�������ʧ��"), TraceLevel_Exception);
    //    return false;
    //}

    return true;
}

//���ò���
bool CTCPNetworkEngine::SetServiceParameter (WORD wServicePort, WORD wMaxConnect, LPCTSTR  pszCompilation) {
    //Ч��״̬
    if (m_bService == true) {
        CTraceService::TraceString (TEXT ("�������洦�ڷ���״̬���˿ڰ󶨲�������"), TraceLevel_Exception);
        return false;
    }

    //�жϲ���
    if (wServicePort == 0) {
        CTraceService::TraceString (TEXT ("δָ������˿ڣ����������"), TraceLevel_Normal);
    }

    //���ñ���
    m_wListenPort = wServicePort;
    m_wMaxSocketItem = wMaxConnect;
    return true;
}

//��������
bool CTCPNetworkEngine::StartService() {
    DWORD i = 0;

    //Ч��״̬
    if (m_bService == true) {
        CTraceService::TraceString (TEXT ("���������ظ�������������������"), TraceLevel_Warning);
        return true;
    }

    //�󶨶���
    if (m_SendQueueService.SetQueueServiceSink (QUERY_ME_INTERFACE (IUnknownEx)) == false) {
        CTraceService::TraceString (TEXT ("���������봥�������ʧ��"), TraceLevel_Exception);
        return false;
    }

    try {
        //��ȡϵͳ��Ϣ
        SYSTEM_INFO SystemInfo;
        GetSystemInfo (&SystemInfo);
        DWORD dwThreadCount = SystemInfo.dwNumberOfProcessors;
        //������ɶ˿�
        m_hCompletionPort = CreateIoCompletionPort (INVALID_HANDLE_VALUE, NULL, NULL, SystemInfo.dwNumberOfProcessors);

        if (m_hCompletionPort == NULL) {
            throw TEXT ("����������ɶ˿ڴ���ʧ��");
        }

        //��������SOCKET
        struct sockaddr_in SocketAddr;
        memset (&SocketAddr, 0, sizeof (SocketAddr));
        SocketAddr.sin_addr.s_addr = INADDR_ANY;
        SocketAddr.sin_family = AF_INET;
        SocketAddr.sin_port = htons (m_wListenPort);
        m_hServerSocket = WSASocket (AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

        if (m_hServerSocket == INVALID_SOCKET) {
            throw TEXT ("����������� SOCKET ����ʧ��");
        }

        int iErrorCode = bind (m_hServerSocket, (SOCKADDR*) & SocketAddr, sizeof (SocketAddr));

        if (iErrorCode == SOCKET_ERROR) {
            throw TEXT ("������������˿ڱ�ռ�ã��˿ڰ�ʧ��");
        }

        iErrorCode = listen (m_hServerSocket, 200);

        if (iErrorCode == SOCKET_ERROR) {
            throw TEXT ("������������˿ڱ�ռ�ã��˿ڼ���ʧ��");
        }

        //��ȡ�˿�
        int iLen = sizeof (SocketAddr);
        getsockname (m_hServerSocket, (SOCKADDR*) & SocketAddr, &iLen);
        m_wListenPort = htons (SocketAddr.sin_port);
        //�������Ͷ���
        bool bSuccess = m_SendQueueService.StartService();

        if (bSuccess == false) {
            throw TEXT ("�������淢�Ͷ��з�������ʧ��");
        }

        //������д�߳�
        for (i = 0; i < dwThreadCount; i++) {
            CServerSocketRSThread * pServerSocketRSThread = new CServerSocketRSThread();

            if (pServerSocketRSThread == NULL) {
                throw TEXT ("���������д�̷߳��񴴽�ʧ��");
            }

            bSuccess = pServerSocketRSThread->InitThread (m_hCompletionPort);

            if (bSuccess == false) {
                throw TEXT ("���������д�̷߳�������ʧ��");
            }

            m_SocketRSThreadArray.Add (pServerSocketRSThread);
        }

        //����Ӧ���߳�
        bSuccess = m_SocketAcceptThread.InitThread (m_hCompletionPort, m_hServerSocket, this);

        if (bSuccess == false) {
            throw TEXT ("����������������̷߳�������");
        }

        //���ж�д�߳�
        for (i = 0; i < dwThreadCount; i++) {
            CServerSocketRSThread * pServerSocketRSThread = m_SocketRSThreadArray[i];
            ASSERT (pServerSocketRSThread != NULL);
            bSuccess = pServerSocketRSThread->StartThread();

            if (bSuccess == false) {
                throw TEXT ("���������д�̷߳�������ʧ��");
            }
        }

        //�������߳�
        m_SocketDetectThread.InitThread (this);
        bSuccess = m_SocketDetectThread.StartThread();

        if (bSuccess == false) {
            throw TEXT ("����������̷߳�������ʧ��");
        }

        //����Ӧ���߳�
        bSuccess = m_SocketAcceptThread.StartThread();

        if (bSuccess == false) {
            throw TEXT ("������������̷߳�������ʧ��");
        }

        //���ñ���
        m_bService = true;
    }

    catch (LPCTSTR pszError) {
        CTraceService::TraceString (pszError, TraceLevel_Exception);
        return false;
    }

    return true;
}

//ֹͣ����
bool CTCPNetworkEngine::ConcludeService() {
    //���ñ���
    m_bService = false;
    m_dwLastDetect = 0L;
    //ֹͣ����߳�
    m_SocketDetectThread.ConcludeThread (INFINITE);

    //��ֹӦ���߳�
    if (m_hServerSocket != INVALID_SOCKET) {
        closesocket (m_hServerSocket);
        m_hServerSocket = INVALID_SOCKET;
    }

    m_SocketAcceptThread.ConcludeThread (INFINITE);
    //ֹͣ���Ͷ���
    m_SendQueueService.ConcludeService();
    //�ͷŶ�д�߳�
    INT_PTR nCount = m_SocketRSThreadArray.GetCount(), i = 0;

    if (m_hCompletionPort != NULL) {
        for (i = 0; i < nCount; i++) {
            PostQueuedCompletionStatus (m_hCompletionPort, 0, NULL, NULL);
        }
    }

    for (i = 0; i < nCount; i++) {
        CServerSocketRSThread * pSocketThread = m_SocketRSThreadArray[i];
        ASSERT (pSocketThread != NULL);
        pSocketThread->ConcludeThread (INFINITE);
        SafeDelete (pSocketThread);
    }

    m_SocketRSThreadArray.RemoveAll();

    //�ر���ɶ˿�
    if (m_hCompletionPort != NULL) {
        CloseHandle (m_hCompletionPort);
        m_hCompletionPort = NULL;
    }

    //�ر�����
    CServerSocketItem * pServerSocketItem = NULL;

    for (i = 0; i < m_ActiveSocketItem.GetCount(); i++) {
        pServerSocketItem = m_ActiveSocketItem[i];
        ASSERT (pServerSocketItem != NULL);
        pServerSocketItem->CloseSocket (pServerSocketItem->GetRountID());
        pServerSocketItem->ResetSocketData();
    }

    m_FreeSocketItem.Append (m_ActiveSocketItem);
    m_ActiveSocketItem.RemoveAll();
    // m_QueueServiceEvent.SetQueueServiceSink (NULL);
    return true;
}

//Ӧ����Ϣ
bool CTCPNetworkEngine::OnSocketAcceptEvent (CServerSocketItem * pServerSocketItem) {
    //Ч������
    ASSERT (pServerSocketItem != NULL);

    if (NULL == pServerSocketItem) {
        return false;
    }

    //Ͷ����Ϣ
    if (m_bService == false) {
        return false;
    }

    m_QueueServiceEvent.PostNetworkAcceptEvent (pServerSocketItem->GetSocketID(), pServerSocketItem->GetClientAddr());
    return true;
}

//�����ȡ��Ϣ
bool CTCPNetworkEngine::OnSocketReadEvent (TCP_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem) {
    //Ч������
    ASSERT (pServerSocketItem != NULL);

    if (NULL == pServerSocketItem) {
        return false;
    }

    //Ч��״̬
    if (m_bService == false) {
        return false;
    }

    m_QueueServiceEvent.PostNetworkReadEvent (pServerSocketItem->GetSocketID(), Command, pBuffer, wDataSize);
    return true;
}

//����ر���Ϣ
bool CTCPNetworkEngine::OnSocketCloseEvent (CServerSocketItem * pServerSocketItem) {
    //Ч�����
    ASSERT (pServerSocketItem != NULL);

    if (NULL == pServerSocketItem) {
        return false;
    }

    try {
        //Ч��״̬
        if (m_bService == false) {
            return false;
        }

        //����ʱ��
        WORD wIndex = pServerSocketItem->GetIndex();
        WORD wRountID = pServerSocketItem->GetRountID();
        DWORD dwClientAddr = pServerSocketItem->GetClientAddr();
        DWORD dwConnectTime = pServerSocketItem->GetConnectTime();
        //////////////////////////////////////////////////////////////////////////,����Ҫ����
        m_QueueServiceEvent.PostNetworkShutEvent (pServerSocketItem->GetSocketID(), dwClientAddr, dwConnectTime);
        //�ͷ�����
        FreeSocketItem (pServerSocketItem);
    }

    catch (...) {}

    return true;
}

//Ӧ����Ϣ
bool CTCPNetworkEngine::OnSocketForegroundEvent(CServerSocketItem * pServerSocketItem) {
    //Ч������
    ASSERT(pServerSocketItem != NULL);

    if (NULL == pServerSocketItem) {
        return false;
    }

    //Ͷ����Ϣ
    if (m_bService == false) {
        return false;
    }

    m_QueueServiceEvent.PostNetworkForegroundEvent(pServerSocketItem->GetSocketID(), pServerSocketItem->isForeground());
    return true;
}

//֪ͨ�ص����������Ͷ����̵߳��ã�
void CTCPNetworkEngine::OnQueueServiceSink (WORD wIdentifier, void * pBuffer, WORD wDataSize) {
    switch (wIdentifier) {
    case QUEUE_SEND_REQUEST: {	//��������
        //Ч������
        tagSendDataRequest * pSendDataRequest = (tagSendDataRequest *) pBuffer;
        ASSERT (wDataSize >= (sizeof (tagSendDataRequest) - sizeof (pSendDataRequest->cbSendBuf)));
        ASSERT (wDataSize == (pSendDataRequest->wDataSize + sizeof (tagSendDataRequest) - sizeof (pSendDataRequest->cbSendBuf)));

        //Ⱥ������
        if (pSendDataRequest->cbBatchMask == BG_COMPUTER) {
            //��ȡ���
            {
                CWHDataLocker lcok (m_CriticalSection);
                m_TempSocketItem.RemoveAll();
                m_TempSocketItem.Copy (m_ActiveSocketItem);
            }
            //ѭ����������
            CServerSocketItem * pServerSocketItem = NULL;

            for (INT_PTR i = 0; i < m_TempSocketItem.GetCount(); i++) {
                pServerSocketItem = m_TempSocketItem[i];
                ASSERT (pServerSocketItem != NULL);
                //if (pServerSocketItem->isWinSocket() == false) continue;
                CWHDataLocker lock (pServerSocketItem->GetSignedLock());

                if (pServerSocketItem->IsAllowBatch() && pServerSocketItem->isWinSocket()) { //��һ�β�������
                    pServerSocketItem->SendData (pSendDataRequest->cbSendBuf, pSendDataRequest->wDataSize, pSendDataRequest->wMainCmdID,
                                                 pSendDataRequest->wSubCmdID, pServerSocketItem->GetRountID());
                }
            }
        }

        else if (pSendDataRequest->cbBatchMask == BG_MOBILE) {
            //��ȡ���
            {
                CWHDataLocker lcok (m_CriticalSection);
                m_TempSocketItem.RemoveAll();
                m_TempSocketItem.Copy (m_ActiveSocketItem);
            }
            //ѭ����������
            CServerSocketItem * pServerSocketItem = NULL;

            for (INT_PTR i = 0; i < m_TempSocketItem.GetCount(); i++) {
                pServerSocketItem = m_TempSocketItem[i];
                ASSERT (pServerSocketItem != NULL);
                //if (pServerSocketItem->isWebSocket() == false) continue;
                CWHDataLocker lock (pServerSocketItem->GetSignedLock());

                if (pServerSocketItem->IsAllowBatch() && pServerSocketItem->isWebSocket()) { //��һ�β�������
                    pServerSocketItem->SendData (pSendDataRequest->cbSendBuf, pSendDataRequest->wDataSize, pSendDataRequest->wMainCmdID,
                                                 pSendDataRequest->wSubCmdID, pServerSocketItem->GetRountID());
                }
            }
        }

        else {
            //�����
            CServerSocketItem * pServerSocketItem = EnumSocketItem (LOWORD (pSendDataRequest->dwSocketID));
            CWHDataLocker lock (pServerSocketItem->GetSignedLock());
            pServerSocketItem->SendData (pSendDataRequest->cbSendBuf, pSendDataRequest->wDataSize, pSendDataRequest->wMainCmdID,
                                         pSendDataRequest->wSubCmdID, HIWORD (pSendDataRequest->dwSocketID));
        }

        break;
    }

    case QUEUE_SAFE_CLOSE: {	//��ȫ�ر�
        //Ч������
        ASSERT (wDataSize == sizeof (tagSafeCloseSocket));
        tagSafeCloseSocket * pSafeCloseSocket = (tagSafeCloseSocket *) pBuffer;

        //��ȫ�ر�
        try {
            CServerSocketItem * pServerSocketItem = EnumSocketItem (LOWORD (pSafeCloseSocket->dwSocketID));
            CWHDataLocker lock (pServerSocketItem->GetSignedLock());
            pServerSocketItem->ShutDownSocket (HIWORD (pSafeCloseSocket->dwSocketID));
        }

        catch (...) {
        }

        break;
    }

    case QUEUE_ALLOW_BATCH: {	//����Ⱥ��
        //Ч������
        ASSERT (wDataSize == sizeof (tagAllowBatchSend));
        tagAllowBatchSend * pAllowBatchSend = (tagAllowBatchSend *) pBuffer;
        //����Ⱥ��
        CServerSocketItem * pServerSocketItem = EnumSocketItem (LOWORD (pAllowBatchSend->dwSocketID));
        CWHDataLocker lock (pServerSocketItem->GetSignedLock());
        pServerSocketItem->AllowBatchSend (HIWORD (pAllowBatchSend->dwSocketID), pAllowBatchSend->cbAllow ? true : false);
        break;
    }

    case QUEUE_DETECT_SOCKET: {	//�������
        //��ȡ���
        {
            CWHDataLocker lcok (m_CriticalSection);
            m_TempSocketItem.RemoveAll();
            m_TempSocketItem.Copy (m_ActiveSocketItem);
        }
        //��������
        TCP_KN_DetectSocket DetectSocket;
        ZeroMemory (&DetectSocket, sizeof (DetectSocket));
        //��������
        WORD wRountID = 0;
        DWORD dwNowTickCount = GetTickCount();
        DWORD dwBreakTickCount = __max (dwNowTickCount - m_dwLastDetect, TIME_BREAK_READY);
        //ƻ������̨ûping��Ӧ������ӳ����ʱ��,Ϊ2����
        //DWORD dwWebSocketBreakTickCount = 120000;
        //���ñ���
        m_dwLastDetect = GetTickCount();

        //�������
        for (INT_PTR i = 0; i < m_TempSocketItem.GetCount(); i++) {
            //��������
            CServerSocketItem * pServerSocketItem = m_TempSocketItem[i];

            //edit by xujialong
            // if (!pServerSocketItem->isWinSocket()) { continue; }

            DWORD dwRecvTickCount = pServerSocketItem->GetRecvTickCount();
            CWHDataLocker lock (pServerSocketItem->GetSignedLock());

            //������
            if (dwRecvTickCount >= dwNowTickCount) {
                continue;
            }

            //�������
            if (pServerSocketItem->IsReadySend() == true) {
                //DWORD dwBreakTime = dwBreakTickCount;
                //����Ƿ�����̨
                if (pServerSocketItem->isWebSocket()) {
                    //dwBreakTime = dwWebSocketBreakTickCount;
                    DWORD dwKNInterval = dwNowTickCount - pServerSocketItem->GetKNRecvTickCount();
                    //1������δ���ܵ��ͻ�����������Ϣ�����ر�����
                    if (dwKNInterval > 60 * 1000) {
                        pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
                        continue;
                    }

                    bool setForeground = true;
                    if (dwKNInterval > 30 * 1000) {
                        setForeground = false;
                    } else {
                        setForeground = true;
                    }

                    if (pServerSocketItem->isForeground() == true) {
                        if (setForeground == false) {
                            pServerSocketItem->setForeground(false);
                        }
                    }
                } else {
                    if ((dwNowTickCount - dwRecvTickCount) > dwBreakTickCount) {
                        pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
                        continue;
                    }
                }
            } else {
                if ( (dwNowTickCount - dwRecvTickCount) > TIME_BREAK_CONNECT) {
                    pServerSocketItem->CloseSocket (pServerSocketItem->GetRountID());
                    continue;
                }
            }

            //��������
            if (pServerSocketItem->IsReadySend() == true) {
                wRountID = pServerSocketItem->GetRountID();
                if (pServerSocketItem->isWebSocket() == true) {
                    pServerSocketItem->sendPingFrame();
                    // pServerSocketItem->SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, wRountID);
                } else if(pServerSocketItem->isWinSocket()) {
                    DetectSocket.dwSendTickCount = GetTickCount();
                    pServerSocketItem->SendData(&DetectSocket, sizeof (DetectSocket), MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, wRountID);
                }
            }
        }

        break;
    }

    default: {
        ASSERT (FALSE);
    }
    }

    return;
}

//��ȡ���ж���
CServerSocketItem * CTCPNetworkEngine::ActiveSocketItem() {
    CWHDataLocker lock (m_CriticalSection);
    //��ȡ���ж���
    CServerSocketItem * pServerSocketItem = NULL;

    if (m_FreeSocketItem.GetCount() > 0) {
        INT_PTR nItemPostion = m_FreeSocketItem.GetCount() - 1;
        pServerSocketItem = m_FreeSocketItem[nItemPostion];
        ASSERT (pServerSocketItem != NULL);
        m_FreeSocketItem.RemoveAt (nItemPostion);
        m_ActiveSocketItem.Add (pServerSocketItem);
    }

    //�����¶���
    if (pServerSocketItem == NULL) {
        WORD wStorageCount = (WORD) m_StorageSocketItem.GetCount();

        if (wStorageCount < m_wMaxSocketItem) {
            try {
                pServerSocketItem = new CServerSocketItem (wStorageCount, this);

                if (pServerSocketItem == NULL) {
                    return NULL;
                }

                m_StorageSocketItem.Add (pServerSocketItem);
                m_ActiveSocketItem.Add (pServerSocketItem);
            }

            catch (...) {
                return NULL;
            }
        }
    }

    return pServerSocketItem;
}

//��ȡ���Ӷ���
CServerSocketItem * CTCPNetworkEngine::EnumSocketItem (WORD wIndex) {
    CWHDataLocker lock (m_CriticalSection);

    if (wIndex < m_StorageSocketItem.GetCount()) {
        CServerSocketItem * pServerSocketItem = m_StorageSocketItem[wIndex];
        ASSERT (pServerSocketItem != NULL);
        return pServerSocketItem;
    }

    return NULL;
}

//�ͷ����Ӷ���
bool CTCPNetworkEngine::FreeSocketItem (CServerSocketItem * pServerSocketItem) {
    //Ч�����
    ASSERT (pServerSocketItem != NULL);
    //�ͷŶ���
    CWHDataLocker lock (m_CriticalSection);
    INT_PTR nActiveCount = m_ActiveSocketItem.GetCount();

    for (int i = 0; i < nActiveCount; i++) {
        if (pServerSocketItem == m_ActiveSocketItem[i]) {
            m_ActiveSocketItem.RemoveAt (i);
            m_FreeSocketItem.Add (pServerSocketItem);
            return true;
        }
    }

    //�ͷ�ʧ��
    ASSERT (FALSE);
    return false;
}

//�������
bool CTCPNetworkEngine::DetectSocket() {
    return m_SendQueueService.AddToQueue (QUEUE_DETECT_SOCKET, NULL, 0);
}


//���ͺ���
bool CTCPNetworkEngine::SendData (DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID) {
    //Ч��״̬
    ASSERT (m_bService == true);

    if (m_bService == false) {
        return false;
    }

    //��������
    tagSendDataRequest SendRequest;
    SendRequest.wMainCmdID = wMainCmdID;
    SendRequest.wSubCmdID = wSubCmdID;
    SendRequest.dwSocketID = dwSocketID;
    SendRequest.wDataSize = 0;
    SendRequest.cbBatchMask = BG_NULL;
    //���뷢�Ͷ���
    WORD wSendSize = sizeof (SendRequest) - sizeof (SendRequest.cbSendBuf);
    return m_SendQueueService.AddToQueue (QUEUE_SEND_REQUEST, &SendRequest, wSendSize);
}

//���ͺ���
bool CTCPNetworkEngine::SendData (DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize) {
    //Ч��״̬
    ASSERT (m_bService == true);

    if (m_bService == false) {
        return false;
    }

    //Ч������
    ASSERT ( (wDataSize + sizeof (TCP_Head)) <= SOCKET_TCP_BUFFER);

    if ( (wDataSize + sizeof (TCP_Head)) > SOCKET_TCP_BUFFER) {
        return false;
    }

    //��������
    tagSendDataRequest SendRequest;
    SendRequest.wMainCmdID = wMainCmdID;
    SendRequest.wSubCmdID = wSubCmdID;
    SendRequest.dwSocketID = dwSocketID;
    SendRequest.wDataSize = wDataSize;
    SendRequest.cbBatchMask = BG_NULL;

    if (wDataSize > 0) {
        ASSERT (pData != NULL);
        CopyMemory (SendRequest.cbSendBuf, pData, wDataSize);
    }

    //���뷢�Ͷ���
    WORD wSendSize = sizeof (SendRequest) - sizeof (SendRequest.cbSendBuf) + wDataSize;
    return m_SendQueueService.AddToQueue (QUEUE_SEND_REQUEST, &SendRequest, wSendSize);
}

//��������
bool CTCPNetworkEngine::SendDataBatch (WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize, BYTE cbBatchMask) {
    //Ч��״̬
    if (m_bService == false) {
        return false;
    }

    //Ч������
    ASSERT ( (wDataSize + sizeof (TCP_Head)) <= SOCKET_TCP_BUFFER);

    if ( (wDataSize + sizeof (TCP_Head)) > SOCKET_TCP_BUFFER) {
        return false;
    }

    //��������
    tagSendDataRequest SendRequest;
    SendRequest.wMainCmdID = wMainCmdID;
    SendRequest.wSubCmdID = wSubCmdID;
    SendRequest.dwSocketID = 0;
    SendRequest.wDataSize = wDataSize;
    SendRequest.cbBatchMask = cbBatchMask;

    if (wDataSize > 0) {
        ASSERT (pData != NULL);
        CopyMemory (SendRequest.cbSendBuf, pData, wDataSize);
    }

    //���뷢�Ͷ���
    WORD wSendSize = sizeof (SendRequest) - sizeof (SendRequest.cbSendBuf) + wDataSize;
    return m_SendQueueService.AddToQueue (QUEUE_SEND_REQUEST, &SendRequest, wSendSize);
}

//�ر�����
bool CTCPNetworkEngine::CloseSocket (DWORD dwSocketID) {
    CServerSocketItem * pServerSocketItem = EnumSocketItem (LOWORD (dwSocketID));

    if (pServerSocketItem == NULL) {
        return false;
    }

    CWHDataLocker lock (pServerSocketItem->GetSignedLock());
    return pServerSocketItem->CloseSocket (HIWORD (dwSocketID));
}

//���ùر�
bool CTCPNetworkEngine::ShutDownSocket (DWORD dwSocketID) {
    tagSafeCloseSocket SafeCloseSocket;
    SafeCloseSocket.dwSocketID = dwSocketID;
    return m_SendQueueService.AddToQueue (QUEUE_SAFE_CLOSE, &SafeCloseSocket, sizeof (SafeCloseSocket));
}

//����Ⱥ��
bool CTCPNetworkEngine::AllowBatchSend (DWORD dwSocketID, bool bAllow, BYTE cbBatchMask) {
    tagAllowBatchSend AllowBatchSendNode;
    AllowBatchSendNode.dwSocketID = dwSocketID;
    AllowBatchSendNode.cbAllow = bAllow;
    return m_SendQueueService.AddToQueue (QUEUE_ALLOW_BATCH, &AllowBatchSendNode, sizeof (tagAllowBatchSend));
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec (dllexport) void * CreateTCPNetworkEngine (const GUID & Guid, DWORD dwInterfaceVer) {
    //��������
    CTCPNetworkEngine * pTCPSocketEngine = NULL;

    try {
        pTCPSocketEngine = new CTCPNetworkEngine();

        if (pTCPSocketEngine == NULL) {
            throw TEXT ("����ʧ��");
        }

        void * pObject = pTCPSocketEngine->QueryInterface (Guid, dwInterfaceVer);

        if (pObject == NULL) {
            throw TEXT ("�ӿڲ�ѯʧ��");
        }

        return pObject;
    }

    catch (...) {}

    //�������
    SafeDelete (pTCPSocketEngine);
    return NULL;
}



//////////////////////////////////////////////////////////////////////////
