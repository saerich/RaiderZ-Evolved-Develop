#include "stdafx.h"
#include "MockServer.h"
#include "GCommandTable.h"
#include "GCommandCenter.h"
#include "MockNetServer.h"
#include "GMasterNetClient.h"
#include "GConfig.h"
#include "GMasterServerFacade.h"
#include "MockMasterNetClient.h"
#include "GStandAloneModePartyRouter.h"
#include "GStandAloneModePlayerRouter.h"
#include "GServerGateRouter.h"
#include "GServerFieldRouter.h"
#include "GAppServerFacade.h"
#include "MockAppNetClient.h"
#include "GStandAloneModeChatChannelRouter.h"
#include "GServerStatusUpdater.h"
#include "GStandAloneModeGateRouter.h"
#include "GStandAloneModeGuildRouter.h"


MockServer::MockServer()
{
	minet::MNetClientDesc net_client_desc;
	net_client_desc.bKeepAlive = false;

	// MasterServerFacade �ʱ�ȭ
	if (m_pMasterServerFacade)
	{
		delete m_pMasterServerFacade;
	}

	MockMasterNetClient* pMasterMockNetClient = new MockMasterNetClient(net_client_desc);
	m_pMasterServerFacade = new GMasterServerFacade(pMasterMockNetClient);

	gsys.pMasterServerFacade = m_pMasterServerFacade;
	
	// StandAloneMode�� ���еǾ� �ִ� Router
	m_pMasterServerFacade->SetPartyRouter(new GStandAloneModePartyRouter());
	m_pMasterServerFacade->SetPlayerRouter(new GStandAloneModePlayerRouter());
	m_pMasterServerFacade->SetGateRouter(new GStandAloneModeGateRouter());
	m_pMasterServerFacade->SetGuildRouter(new GStandAloneModeGuildRouter());
	
	// StandAloneMode ���о��� Router
	m_pMasterServerFacade->SetFieldRouter(new GServerFieldRouter());


	// AppServerFacade �ʱ�ȭ
	if (m_pAppServerFacade)
	{
		delete m_pAppServerFacade;
	}

	MockAppNetClient* pAppMockNetClient = new MockAppNetClient(net_client_desc);
	m_pAppServerFacade = new GAppServerFacade(pAppMockNetClient);

	gsys.pAppServerFacade = m_pAppServerFacade;

	// StandAloneMode�� ���еǾ� �ִ� Router
	m_pAppServerFacade->SetChatChannelRouter(new GStandAloneModeChatChannelRouter());



	gsys.pScriptManager->Init();
}

bool MockServer::InitInfo()
{
	GConfig::m_bStandAlone = true;

	// GServer���� io�� �ʿ��� ���� ���� �ʱ�ȭ�Ѵ�.
	gsys.pScriptManager->Fini();
	
	if (!GStrings::Init())
		return false;

	GConst::Init(WLUA);
		
	// ȯ�� �ʱ�ȭ
	GEnvParam envParam;
	if (!gmgr.pEnvManager->Create(envParam))
		return false;

	return true;
}

MockServer::~MockServer()
{
	
}

bool MockServer::CreateNetwork()
{
	if (m_pCommandCenter) delete m_pCommandCenter;
	if (m_pNetServer) delete m_pNetServer;

	minet::MNetServerDesc server_desc;
	server_desc.bHeartBeat = false;

	m_pNetServer = new MockNetServer(server_desc);
	m_pCommandCenter = new GCommandCenter(m_pNetServer);
	gsys.pCommandCenter = m_pCommandCenter;

	return true;
}
