#include "stdafx.h"
#include "MCommandServer.h"
#include "MCommandManager.h"
#include "MLink.h"
#include "MCmdHandler_NetServer.h"
#include "MAcceptor.h"
#include "MDisconnector.h"
#include "MSender.h"
#include "MReceiver.h"
#include "MCommandTable.h"
#include "MPacket.h"

namespace minet {

class MCommandServerAcceptListener : public MAcceptListener
{
private:
	MNetServer& m_CommandServer;
public:
	MCommandServerAcceptListener(MNetServer& server) : m_CommandServer(server) {}
	virtual void OnAccept(MServerTcpSocket& tcp_socket)
	{
		MUID AllocUID = m_CommandServer.NewUID();

		MLink* pNewLink = new MLink(m_CommandServer.GetUID(), 
									AllocUID, 
									tcp_socket, 
									m_CommandServer.GetCommandManager());

		m_CommandServer.m_LinkMap.Add(pNewLink);

		minet::MCommand* pNewCommand = m_CommandServer.NewCommand(minet::MC_LOCAL_NET_ONACCEPT, m_CommandServer.GetUID());
		pNewCommand->AddParameter(new minet::MCommandParameterUID(AllocUID));
		m_CommandServer.Post(pNewCommand);
	}
};

class MCommandServerDisconnectListner : public MDisconnectListener
{
private:
	MNetServer& m_CommandServer;
public:
	MCommandServerDisconnectListner(MNetServer& server) : m_CommandServer(server) {}
	virtual void OnDisconnect(MServerTcpSocket& tcp_socket)
	{
		mlog(MLOG_DEBUG, "MNetServer::OnDisconnect> sd = %d\n", socket);

		MUID uidLink = m_CommandServer.m_LinkMap.GetLinkUID(tcp_socket.GetSocket());
		m_CommandServer.m_LinkMap.Delete(tcp_socket.GetSocket());

		minet::MCommand* pNewCommand = m_CommandServer.NewCommand(minet::MC_LOCAL_NET_CLEAR, m_CommandServer.GetUID());
		pNewCommand->AddParameter(new minet::MCommandParameterUID(uidLink));
		m_CommandServer.Post(pNewCommand);
	}
};

class MCommandServerSendListener : public MSendListener
{
private:
	MNetServer& m_CommandServer;
public:
	MCommandServerSendListener(MNetServer& server) : m_CommandServer(server) {}
	virtual void OnSend()
	{
	}
};

class MCommandServerRecvListener : public MRecvListener
{
private:
	MNetServer& m_CommandServer;
public:
	MCommandServerRecvListener(MNetServer& server) : m_CommandServer(server) {}
	virtual void OnRecv(MServerTcpSocket& tcpsocket, BYTE* pData, DWORD dwPacketLen)
	{
		cml2::MCriticalSection::Guard guard(m_CommandServer.m_LinkMap.m_csLock);
		MLink* pLink = m_CommandServer.m_LinkMap.GetLink(tcpsocket.GetSocket());

		if ((pLink) && (pLink->IsAllowed()))
		{
			// New Cmd Buffer ////////////////
			minet::MPacketHeader* pPacket = (minet::MPacketHeader*)(pData);
			minet::MCommandStream* pCmdStream = pLink->GetCommandStream();
			if (!pCmdStream->Read((char*)pPacket, dwPacketLen))
			{
				// ��Ŷ�� ����� �ȿ��� ���������.
				pLink->SetAllowed(false);
				pLink->Disconnect();

				return;
			}

			while(minet::MCommand* pCmd = pCmdStream->GetCommand()) 
			{
				m_CommandServer.Post(pCmd);
			}
		}
	}
};

void MNetServer::SendCommand( minet::MCommand* pCommand )
{
	_ASSERT(pCommand->GetReceiverUID().Value);

	int nSize = pCommand->GetSize();
	if ((nSize <= 0) || (nSize >= MAX_PACKET_SIZE)) return;

	char CmdDataBuf[MAX_PACKET_SIZE];
	nSize = pCommand->GetData(CmdDataBuf, nSize);


	if (pCommand->HasAdditiveReceivers() == false)
	{
		SendCommandToLink(pCommand->m_Receiver, CmdDataBuf, nSize);
	}
	else
	{
		// receiver���� �켱 ����
		SendCommandToLink(pCommand->m_Receiver, CmdDataBuf, nSize);

		// �߰� receiver���� ����
		for (list<MUID>::iterator itor = pCommand->GetAdditiveReceiverListBegin(); 
			itor != pCommand->GetAdditiveReceiverListEnd(); ++itor)
		{
			_ASSERTE(pCommand->m_Receiver != *itor && "�̰� �߸� ��(birdkr)���� ������ּ���");
			SendCommandToLink((*itor), CmdDataBuf, nSize);
		}
	}
}

MNetServer::MNetServer() : MCommandCommunicator(), m_uidGenerator(0,0)
{
	m_pCmdHandler = new minet::MCmdHandler_NetServer(this);
	m_pAcceptListener = new MCommandServerAcceptListener(*this);
	m_pDisconnectListner = new MCommandServerDisconnectListner(*this);
	m_pSendListener = new MCommandServerSendListener(*this);
	m_pRecvListener = new MCommandServerRecvListener(*this);

	m_TCPServer.m_pAcceptor->SetListener(m_pAcceptListener);
	m_TCPServer.m_pDisconnector->SetListener(m_pDisconnectListner);
	m_TCPServer.m_pSender->SetListener(m_pSendListener);
	m_TCPServer.m_pReceiver->SetListener(m_pRecvListener);
}

MNetServer::~MNetServer()
{
	delete m_pDisconnectListner;
	delete m_pAcceptListener;

	delete m_pCmdHandler;
}

bool MNetServer::Create( int nPort, const bool bReuse /*= false */ )
{
	bool bResult = true;

	// ���� UID ���� - ���߿� �л� ������ �Ǹ� �� �������� UID�� ��� ������� ����غ��� �Ѵ�.
	MUID new_uid = NewUID();
	SetUID(new_uid);

	m_TCPServer.Start(nPort);

	minet::MAddSharedCommandTable();

	return bResult;
}

void MNetServer::Destroy()
{

}

void MNetServer::Disconnect( MUID uidLink )
{
	cml2::MCriticalSection::Guard guard(m_LinkMap.m_csLock);
	MLink* pLink = m_LinkMap.GetLink(uidLink);
	if (pLink)
	{
		pLink->Disconnect();
	}
}

bool MNetServer::Post( minet::MCommand* pCommand )
{
	// ������ Post�� ������ Safely�ϰ� Post�Ѵ�.
	MCommandCommunicator::PostSafely(pCommand);
	return true;
}

MUID MNetServer::NewUID()
{
	MCriticalSection::Guard guard(m_csNewUID);
	m_uidGenerator.Increase();
	return m_uidGenerator;
}

void MNetServer::SendCommandToLink( MUID uidReceiver, char* pData, int nDataSize )
{
	if (nDataSize <= 0) return;

	int nPacketSize = nDataSize + sizeof(minet::MPacketHeader);

	minet::MCommandMsg* pMsg = (minet::MCommandMsg*)malloc(nPacketSize);	// malloc�� �޸� �Ҵ�
	pMsg->Buffer[0] = 0;
	pMsg->nCheckSum = 0;	// CheckSum�� ���ԵǱ� ������ ���� �ʱ�ȭ 
	pMsg->nMsg = MSGID_COMMAND;		// �ϴ��� ��ȣȭ���� ����
	pMsg->nSize = nPacketSize;

	CopyMemory(pMsg->Buffer, pData, nDataSize);
	pMsg->nCheckSum = MBuildCheckSum(pMsg, nPacketSize);

	// Lock Scope
	{
		cml2::MCriticalSection::Guard guard(m_LinkMap.m_csLock);

		MLink* pLink = m_LinkMap.GetLink(uidReceiver);
		if (pLink)
		{
			pLink->Send((BYTE*)(pMsg), nPacketSize);
		}
		else
		{
			free(pMsg);
		}
	}
}


}