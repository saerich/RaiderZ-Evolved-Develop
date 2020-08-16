#include "stdafx.h"
#include "MNetServer.h"
#include "MDebug.h"
#include "MCommObject.h"
#include "MCommandStream.h"
#include "MCmdHandler_NetServer.h"
#include "MCommand.h"
#include "MCommandTable.h"

namespace minet {

void MNetServer::_IocpCallback(void* pCallbackContext, 
								MIOCP_OPERATION nIO, 
								DWORD nKey, 
								MPacketHeader* pPacket, 
								DWORD dwPacketLen)
{
	MNetServer* pNetServer = (MNetServer*)pCallbackContext;
	pNetServer->IocpCallback(nIO, nKey, pPacket, dwPacketLen);
}

void MNetServer::IocpCallback(MIOCP_OPERATION nIO, DWORD nKey, MPacketHeader* pPacket, DWORD dwPacketLen)
{
	switch (nIO)
	{
	case MIOCP_OP_ACCEPT:
		{
			OnAccept(nKey);
		}
		break;
	case MIOCP_OP_SEND:
		{
			OnSend(nKey, dwPacketLen);
		}
		break;
	case MIOCP_OP_RECV:
		{
			OnRecv(nKey, pPacket, dwPacketLen);
		}
		break;
	case MIOCP_OP_CONNECT:
		{

		}
		break;
	case MIOCP_OP_DISCONNECT:
		{
			OnDisconnect(nKey);
		}
		break;
	};
}

void MNetServer::OnAccept(DWORD nClientKey)
{
	MUID AllocUID = NewUID();

	// �Ҵ��� �� �ִ� UID ������ ����.
	if(AllocUID.IsInvalid())
	{
		mlog3("Communicator has not UID space to allocate your UID.");
		return;
	}

	char szIP[128] = "";
	int nPort = 0;
	m_IocpNet.GetAddress(nClientKey, szIP, sizeof(szIP), &nPort);

	MCommObject* pCommObj = new MCommObject(this);
	pCommObj->Init(GetUID(), AllocUID, nClientKey, szIP, nPort);
	m_pCommObjectManager->Add(pCommObj);

	// TODO: �켱 �׽�Ʈ�� - bird
	MCommand* pNewCommand = NewCommand(MC_LOCAL_NET_ONACCEPT, GetUID());
	pNewCommand->AddParameter(new MCommandParameterUID(AllocUID));
	Post(pNewCommand);

	mlog(MLOG_DEBUG, "MNetServer::OnAccept>Accept uid=(%I64u) , sd=%d, address=(%s:%d)\n", 
							AllocUID, nClientKey, szIP, nPort);
}

void MNetServer::OnRecv(DWORD nClientKey, MPacketHeader* pPacket, DWORD dwPacketLen)
{
	m_Traffic.CheckTraffic(&dwPacketLen, NULL);

	m_pCommObjectManager->Lock();
	MCommObject* pCommObj = m_pCommObjectManager->GetCommObjectByKey(nClientKey);

	if ((pCommObj) && (pCommObj->IsAllowed()))
	{
		// New Cmd Buffer ////////////////
		MCommandStream* pCmdStream = pCommObj->GetCommandStream();
		if (!pCmdStream->Read((char*)pPacket, dwPacketLen))
		{
			// ��Ŷ�� ����� �ȿ��� ���������.
			pCommObj->SetAllowed(false);
			Disconnect(pCommObj->GetUID());
	
			m_pCommObjectManager->Unlock();
			return;
		}

		while(MCommand* pCmd = pCmdStream->GetCommand()) 
		{
			Post(pCmd);
		}
	}
	m_pCommObjectManager->Unlock();
}

void MNetServer::OnSend(DWORD nClientKey, DWORD dwPacketLen)
{
	m_Traffic.CheckTraffic(NULL, &dwPacketLen);
}

void MNetServer::OnDisconnect(DWORD nClientKey)
{
	mlog(MLOG_DEBUG, "MNetServer::OnDisconnect> sd = %d\n", nClientKey);
	MUID uidComm = m_pCommObjectManager->GetCommObjectUID(nClientKey);

	m_pCommObjectManager->Delete(nClientKey);

	MCommand* pNewCommand = NewCommand(MC_LOCAL_NET_CLEAR, m_This);
	pNewCommand->AddParameter(new MCommandParameterUID(uidComm));
	Post(pNewCommand);
}

MNetServer::MNetServer() : MCommandCommunicator(), m_uidGenerator(0,0)
{
	m_pCommObjectManager = new MCommObjectManager();
	m_pCmdHandler = new MCmdHandler_NetServer(this);
}

MNetServer::~MNetServer()
{
	delete m_pCmdHandler;
	delete m_pCommObjectManager;
}

bool MNetServer::Create(int nPort, const bool bReuse)
{
	bool bResult = true;

	// ���� UID ���� - ���߿� �л� ������ �Ǹ� �� �������� UID�� ��� ������� ����غ��� �Ѵ�.
	MUID new_uid = NewUID();
	SetUID(new_uid);

	if(m_IocpNet.Create(nPort, bReuse)==false) 
	{
		mlog( "MNetServer::Create - m_IocpNet.Create(nPort)==false" );
		bResult = false;
	}

	m_IocpNet.SetCallback(MNetServer::_IocpCallback, this);

	m_pCommObjectManager->Create();
	return bResult;
}

void MNetServer::Destroy()
{
	m_pCommObjectManager->Destroy();

	m_IocpNet.Destroy();
}


void MNetServer::SendCommand(MCommand* pCommand)
{
	_ASSERT(pCommand->GetReceiverUID().Value);

	int nSize = pCommand->GetSize();
	if ((nSize <= 0) || (nSize >= MAX_PACKET_SIZE)) return;

	char CmdDataBuf[MAX_PACKET_SIZE];
	nSize = pCommand->GetData(CmdDataBuf, nSize);

	if (pCommand->HasAdditiveReceivers() == false)
	{
		SendCommandToCommObject(pCommand->m_Receiver, CmdDataBuf, nSize);

	}
	else
	{
		// receiver���� �켱 ����
		SendCommandToCommObject(pCommand->m_Receiver, CmdDataBuf, nSize);

		// �߰� receiver���� ����
		for (list<MUID>::iterator itor = pCommand->GetAdditiveReceiverListBegin(); 
				itor != pCommand->GetAdditiveReceiverListEnd(); ++itor)
		{
			_ASSERTE(pCommand->m_Receiver != *itor && "�̰� �߸� ��(birdkr)���� ������ּ���");
			SendCommandToCommObject((*itor), CmdDataBuf, nSize);
		}
	}
}

void MNetServer::SendCommandToCommObject(MUID uidReceiver, char* pData, int nDataSize)
{
	DWORD nClientKey = m_pCommObjectManager->GetCommObjectKey(uidReceiver);
	if (nClientKey == 0) return;

	int nPacketSize = nDataSize+sizeof(MPacketHeader);

	MCommandMsg* pMsg = (MCommandMsg*)malloc(nPacketSize);	// malloc�� �޸� �Ҵ�
	pMsg->Buffer[0] = 0;
	pMsg->nCheckSum = 0;	// CheckSum�� ���ԵǱ� ������ ���� �ʱ�ȭ 
	pMsg->nMsg = MSGID_COMMAND;		// �ϴ��� ��ȣȭ���� ����
	pMsg->nSize = nPacketSize;

	CopyMemory(pMsg->Buffer, pData, nDataSize);
	pMsg->nCheckSum = MBuildCheckSum(pMsg, nPacketSize);

	m_IocpNet.Send(nClientKey, pMsg, nPacketSize);
}


void MNetServer::Disconnect(MUID uidComm)
{
	DWORD nClientKey = m_pCommObjectManager->GetCommObjectKey(uidComm);
	if (nClientKey != 0)
	{
		m_IocpNet.Disconnect(nClientKey);
	}
}

MUID MNetServer::NewUID()
{
	MCriticalSection::Guard guard(m_csNewUID);
	m_uidGenerator.Increase();
	return m_uidGenerator;
}

bool MNetServer::Post(MCommand* pCommand)
{
	// ������ Post�� ������ Safely�ϰ� Post�Ѵ�.
	MCommandCommunicator::PostSafely(pCommand);
	return true;
}


void MNetServer::GetTraffic(int* noutRecvTraffic, int* noutSendTraffic)
{
	m_Traffic.GetTraffic(noutRecvTraffic, noutSendTraffic);
}


} // namespace minet