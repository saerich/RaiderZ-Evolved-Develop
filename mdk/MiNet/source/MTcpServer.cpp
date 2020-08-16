#include "stdafx.h"
#include "MTcpServer.h"
#include "MProactor.h"
#include "MAcceptor.h"
#include "MConnector.h"
#include "MDisconnector.h"
#include "MSender.h"
#include "MReceiver.h"
#include "MServerTcpSocket.h"
#include "MServerTcpSocketPool.h"

namespace minet {


//////////////////////////////////////////////////////////////////////////

MTcpServer::MTcpServer() : m_nListenPort(6000), m_nThreadPoolSize(4), m_nSocketPoolSize(2000)
{
	m_pProactor		= new MProactor();
	assert(NULL!=m_pProactor);

	m_pAcceptor		= new MAcceptor();
	assert(NULL!=m_pAcceptor);

	m_pDisconnector	= new MDisconnector();
	assert(NULL!=m_pDisconnector);

	m_pSender		= new MSender();
	assert(NULL!=m_pSender);

	m_pReceiver		= new MReceiver();
	assert(NULL!=m_pReceiver);

	m_pAcceptSocketPool	= new MServerTcpSocketPool(m_pAcceptor, m_pDisconnector, m_pSender, m_pReceiver);
	assert(NULL!=m_pAcceptSocketPool);

	m_pConnector	= new MConnector(m_pProactor, m_pAcceptSocketPool);
	assert(NULL!=m_pConnector);
}

MTcpServer::~MTcpServer()
{
	SAFE_DELETE(m_pProactor);

	m_pAcceptSocketPool->Destroy(); ///< MTcpActAccept �� ���ŵǱ� ���� closesocket() �� ȣ���մϴ�.
	SAFE_DELETE(m_pAcceptSocketPool);

	SAFE_DELETE(m_pAcceptor);
	SAFE_DELETE(m_pDisconnector);
	SAFE_DELETE(m_pSender);
	SAFE_DELETE(m_pReceiver);
	SAFE_DELETE(m_pConnector);
}

bool MTcpServer::MakeWorkers(const int& threadCnt)
{
	assert(NULL!=m_pProactor);
	if (threadCnt < 0)
	{
		m_nThreadPoolSize = 1;
	}
	else
	{
		m_nThreadPoolSize = threadCnt;
	}

	// Proactor �ʱ�ȭ
	assert(NULL!=m_pProactor);
	if (false == m_pProactor->Init( m_nThreadPoolSize ))
	{
		return false;
	}

	// Disconnector �ʱ�ȭ - Acceptor�� �����ؼ� ����Ǯ�� �����Ѵ�
	assert(NULL!=m_pDisconnector);
	m_pDisconnector->Init( m_pProactor, m_pAcceptSocketPool );

	// Receiver �ʱ�ȭ - ����Ǯ�� �����Ѵ�. �����ͼۼ���, �Ľ�ó��
	assert(NULL!=m_pReceiver);
	m_pReceiver->Init( m_pProactor );

	// Sender �ʱ�ȭ
	assert(NULL!=m_pSender);
	m_pSender->Init( m_pProactor );

	return true;
}

bool MTcpServer::Start(int addressFamily, int nListenPort, int nSocketPoolSize/*=2000*/, int nSendPendingLimitCount /*= INT_MAX*/, const char* szMyNetworkCardIP/*=""*/)
{
	m_nListenPort = nListenPort;
	m_nSocketPoolSize = nSocketPoolSize;

	if (false == MakeWorkers(m_nThreadPoolSize))
	{
		return false;
	}

	// Listen ���� �ʱ�ȭ
	if (m_ListenSocket.Init( addressFamily, m_nListenPort, false, szMyNetworkCardIP ) == false)
	{
		return false;
	}

	// Listen ����
	if (m_ListenSocket.Listen( m_pProactor ) == false)
	{
		return false;
	}

	// Acceptor �ʱ�ȭ - Disconnector�� �����ؼ� ����Ǯ�� �����Ѵ�.
	assert(NULL!=m_pAcceptor);
	m_pAcceptor->Init( &m_ListenSocket, m_pProactor, m_pAcceptSocketPool );

	// ����Ǯ ����
	for( int i=0; i < m_nSocketPoolSize; i++ )
	{
		MServerTcpSocket* pSocket = m_pAcceptSocketPool->AddSocket(addressFamily);
		
		pSocket->SetSendPendingLimitCount(nSendPendingLimitCount);

		if (m_pAcceptor->Register( pSocket ) == false)
		{
			m_pAcceptSocketPool->DeleteAllAcceptSockets();
			return false;
		}
	}

	return true;
}

void MTcpServer::Stop()
{
	if (NULL != m_pProactor)
	{
		m_pProactor->Fini();
	}
}

MServerTcpSocket* const MTcpServer::Connect(const char* const addr,const char* const port)
{
	assert(NULL!=addr);
	assert(NULL!=port);
	assert(NULL!=m_pConnector);

	return m_pConnector->Connect(addr, port);
}

void MTcpServer::SetAcceptorListener( MAcceptListener* pListner )
{
	assert(NULL!=m_pAcceptor);
	m_pAcceptor->SetListener(pListner);
}

void MTcpServer::SetConnectListener(MConnectListener* pListner)
{
	assert(NULL!=m_pConnector);
	m_pConnector->SetListener(pListner);
}

void MTcpServer::SetDisconnectorListener( MDisconnectListener* pListener )
{
	assert(NULL!=m_pDisconnector);
	m_pDisconnector->SetListener(pListener);
}

void MTcpServer::SetSenderListener( MSendListener* pListener )
{
	assert(NULL!=m_pSender);
	m_pSender->SetListener(pListener);
}

void MTcpServer::SetReceiverListener( MRecvListener* pListener )
{
	assert(NULL!=m_pReceiver);
	m_pReceiver->SetListener(pListener);
}





} // namespace minet {