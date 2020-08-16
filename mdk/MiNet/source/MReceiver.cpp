#include "stdafx.h"
#include "MReceiver.h"
#include "MServerTcpSocket.h"

namespace minet {

MReceiver::MReceiver() : m_pListener(NULL)
{

}

void MReceiver::OnRecv(MServerTcpSocket& tcpsocket, MUID uidLink, BYTE* pData, DWORD dwPacketLen)
{
	if (m_pListener) m_pListener->OnRecv(tcpsocket, uidLink, pData, dwPacketLen);
}

void MReceiver::ProcEvent( MAct* act, DWORD bytes_transferred )
{
	MTcpAct* pTcpAct = static_cast<MTcpAct*>(act);

	assert(act);
	assert(pTcpAct->m_pTcpSocket);

	MServerTcpSocket& tcpsocket = *(pTcpAct->m_pTcpSocket);

	// passive �������
	if( bytes_transferred == 0 )
	{
		if (m_pListener) m_pListener->OnDisconnect(pTcpAct->m_uidLink);
	}
	else
	{
		OnRecv(tcpsocket, pTcpAct->m_uidLink, (BYTE*)(&tcpsocket.m_RecvBuffer[0]), bytes_transferred);

		// Async Recv ����Ʈ
		tcpsocket.Recv();
	}
}

void MReceiver::ProcError( MAct* act, DWORD bytes_transferred, DWORD error )
{
	MTcpAct* pTcpAct = static_cast<MTcpAct*>(act);

	assert(pTcpAct);
	assert(pTcpAct->m_pTcpSocket);

	MServerTcpSocket* pTcpSocket = pTcpAct->m_pTcpSocket;

	// �αװ� �ʹ� ���� ���Ƽ� ERROR_NETNAME_DELETED�� �α� �ȳ��� - birdkr
	if (error != ERROR_NETNAME_DELETED)
	{
		minet_logd("MReceiver ProcError socket(%d) err(%d)\n", pTcpSocket->GetSocket(), error );
	}

	// �̹� ������ ������ �ƴ��� Recv�� �� ������ ������ ���´�.
	if (bytes_transferred == 0)
	{
		if (m_pListener) m_pListener->OnHardDisconnect(pTcpAct->m_uidLink);
	}

}

void MReceiver::Init( MProactor* proactor )
{
	m_pProactor = proactor;
}


} // namespace minet