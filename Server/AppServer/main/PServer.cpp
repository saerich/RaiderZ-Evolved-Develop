#include "stdafx.h"
#include "PServer.h"
#include "PDef.h"
#include "PConfig.h"
#include "PCommandTable.h"
#include "PCommandCenter.h"
#include "PGameServerObjectManager.h"
#include "PGameServerObject.h"
#include "SFilePath.h"
#include "PMasterNetClient.h"
#include "PDBManager.h"
#include "PServerStatusUpdater.h"
#include "PSystem.h"
#include "SProxyPlayerManager.h"
#include "PServableChecker.h"
#include "PMasterServerFacade.h"
#include "PChattingLogic.h"
#include "PFieldInfoManager.h"
#include "PChatChannelManager.h"
#include "SCommandProfileController.h"
#include "SNetworkCardMgr.h"
#include "PMServerInitLogger.h"
#include "MCommandProfiler.h"
#include "SDsnFactory.h"
#include "SBaseDsnFactory.h"
#include "SDefaultDsnFactory.h"
//#include "PmDsnFactory.h"
#include "MLocale.h"
#include "PPostOfficeSystem.h"

//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////

PServer::PServer()
: m_bCreated(false)
, m_bExiting(false)
, m_pCommandCenter(NULL)
, m_pNetServer(NULL)
, m_pServerStatusUpdater(NULL)
, m_pMasterServerFacade(NULL)
, m_pProxyPlayerManager(NULL)
, m_pCommandProfileController(NULL)
{
	m_pServerInfoViewRglt = new MRegulator(APP_SERVERVIEW_UPDATE_TICK);	
	m_pServableChecker = new PServableChecker();
}

PServer::~PServer()
{
	SAFE_DELETE(m_pServableChecker);
	SAFE_DELETE(m_pServerStatusUpdater);
	SAFE_DELETE(m_pServerInfoViewRglt);
}

bool PServer::Create()
{
	if (InitRequisites() == false)
	{
		return false;
	}
	
	if (InitInfo() == false)
	{
		return false;
	}

	m_bCreated = true;

	WriteServerInfoLog();

	/// �ε��� ��� ���� �ڿ� Server Status Update �� �����ؾ� �մϴ�.
	std::wstring strServerVersion = L"";
	m_pServerStatusUpdater->Start(strServerVersion);

	return true;
}

void PServer::Destroy()
{
	gsys.pDBManager->Release();

	DestroyNetwork();

	m_bCreated = false;

	mlog("Server Closed\n");
}

bool PServer::InitRequisites()
{
	if (CreateNetwork() == false)
	{
		mlog3("Failed! CreateNetwork.\n");
		SetServerInitResult(SERVERINIT_FAILED_NETWORK_INIT);
		return false;
	}

	if (InitDB() == false)
	{
		_ASSERT(0);
		mlog3("Failed! InitDB\n");
		SetServerInitResult(SERVERINIT_FAILED_DB_CONNECT);
		return false;
	}

	m_pServerStatusUpdater = new PServerStatusUpdater(gsys.pDBManager);

	return true;
}

bool PServer::CreateNetwork()
{
	minet::MNetServerDesc server_desc;	// heartbeat üũ�� ���� �ʽ��ϴ�.

	m_pNetServer = new PNetServer(server_desc);
	m_pNetServer->AttachCommandProfiler(new MCommandProfiler());

	m_pCommandCenter = new PCommandCenter(m_pNetServer);
	gsys.pCommandCenter = m_pCommandCenter;	// �ϴ�..
	m_pCommandProfileController = new SCommandProfileController(m_pNetServer,
		PConfig::m_strCommandProfiler_LogFolder,
		PConfig::m_strCommandProfiler_LogName,
		PConfig::m_bCommandProfiler_AutoSave,
		PConfig::m_nCommandProfiler_AutoSaveTickMinute);

	if (PConfig::m_bCommandProfiler_AutoStart)
		StartCommandProfile();

	/// ��Ʈ��ũ ��� ���� - ����
	MUID uidSeed = MUID(PConfig::m_nMyServerID * SERVER_UID_SEED_POWER, 0);
	wstring strMyNetworkCardIP = GetNetworkCardMgr()->GetIP(PConfig::m_nMyNetworkCardID);
	
	if (m_pNetServer->Create(PConfig::m_nPort, false, uidSeed, PConfig::m_nSocketPoolSize, PConfig::m_nSendPendingLimitCount, MLocale::ConvUTF16ToAnsi(strMyNetworkCardIP.c_str()).c_str()) == false)
	{
		return false;
	}


	/// ��Ʈ��ũ ��� ���� - Ŭ���̾�Ʈ
	minet::MNetClientDesc master_net_client_desc;	// heartbeat üũ�� ���� �ʽ��ϴ�.
	PMasterNetClient* pMasterNetClient = new PMasterNetClient(master_net_client_desc);
	m_pMasterServerFacade = new PMasterServerFacade(pMasterNetClient);
	gsys.pMasterServerFacade = m_pMasterServerFacade;

	if (m_pMasterServerFacade->CreateNetwork(PConfig::m_strMasterServerIP.c_str(), PConfig::m_nMasterServerPort) == false)
	{
		return false;
	}

	m_pProxyPlayerManager = new SProxyPlayerManager(pMasterNetClient);
	m_pProxyPlayerManager->AddListener(&m_proxyPlayerListener);

	mlog("Server Socket Opened. (Port = %d)\n", PConfig::m_nPort);

	return true;
}

void PServer::DestroyNetwork()
{
	if (m_pProxyPlayerManager)
	{
		m_pProxyPlayerManager->RemoveListener(&m_proxyPlayerListener);
	}

	if (m_pNetServer)
	{
		m_pNetServer->Destroy();
	}

	SAFE_DELETE(m_pCommandProfileController);
	SAFE_DELETE(m_pCommandCenter);
	SAFE_DELETE(m_pNetServer);

	SAFE_DELETE(m_pProxyPlayerManager);
	SAFE_DELETE(m_pMasterServerFacade);

	gsys.pCommandCenter = NULL;
}

bool PServer::InitDB()
{
	SDsnFactory::GetInstance().Set(
		new SDefaultDsnFactory(
		mdb::MDatabaseDesc()
		, mdb::MDatabaseDesc(PConfig::m_strOdbcDriver, PConfig::m_strGameDB_Server, PConfig::m_strGameDB_DatabaseName, PConfig::m_strGameDB_UserName, PConfig::m_strGameDB_Password)
		, mdb::MDatabaseDesc(PConfig::m_strOdbcDriver, PConfig::m_strLogDB_Server, PConfig::m_strLogDB_DatabaseName, PConfig::m_strLogDB_UserName, PConfig::m_strLogDB_Password)));

	//if (PConfig::m_bPmangDBEncrypt)
	//{
	//	SDsnFactory::GetInstance().Set(
	//		new PmDsnFactory(
	//		mdb::MDatabaseDesc()
	//		, mdb::MDatabaseDesc(PConfig::m_strOdbcDriver, PConfig::m_strGameDB_Server, PConfig::m_strGameDB_DatabaseName, PConfig::m_strGameDB_UserName, PConfig::m_strGameDB_Password)
	//		, mdb::MDatabaseDesc(PConfig::m_strOdbcDriver, PConfig::m_strLogDB_Server, PConfig::m_strLogDB_DatabaseName, PConfig::m_strLogDB_UserName, PConfig::m_strLogDB_Password)));
	//}

	mdb::MDatabaseDesc dbGameDesc = SDsnFactory::GetInstance().Get()->GetGameDSN();
	mdb::MDatabaseDesc dbLogDesc = SDsnFactory::GetInstance().Get()->GetLogDSN();

	if (gsys.pDBManager->Init(dbGameDesc, dbLogDesc))
	{
		mlog("DBMS connected\n");
	}
	else
	{
		mlog("Can't Connect To DBMS\n");
		return false;
	}

	return true;
}

bool PServer::InitInfo()
{
	// �ַ� �ʵ����� �б�
	if (!LoadInfoFiles())
	{
		mlog3("Failed! Load Info Files.\n");
		SetServerInitResult(SERVERINIT_FAILED_DATAFILE_LOAD);
		return false;
	}

	// �⺻ ä�� ���� - �ʵ������� ����
	PChattingLogic logic;
	if (!logic.CreateDefaultChannels())
	{
		mlog3("Failed! Create Default Chatting Channels.\n");
		return false;
	}

	return true;
}

bool PServer::LoadInfoFiles(void)
{
	// �ʵ����� �б�
	VALID_RET(gmgr.pFieldInfoManager != NULL, false);

	wstring strSystemPath = PConfig::m_strSystemPath;
	wstring strFieldListFileName = SFilePath::MakeFieldList(strSystemPath.c_str());
	
	if (false == gmgr.pFieldInfoManager->Load(strFieldListFileName.c_str()))
	{
		mlog3("Failed! Load Field Info.\n");
		return false;
	}

	return true;
}

void PServer::DeInitInfo()
{
}

bool PServer::Update(float fDelta)
{
	if (m_bExiting)
		return false;

	// Ŀ�ǵ� ������ ƽ�� ������� ó���Ѵ�.
	gsys.pMasterServerFacade->RunNetwork();
	m_pNetServer->Run();	
	// DB�Ϸ� ó���� ƽ�� ������� ó���Ѵ�.
	gsys.pDBManager->Update();

	{
		int nTick = DEFAULT_UPDATE_TICK;

		static float fElapsed = 0;
		fElapsed += fDelta;

		if (fElapsed < (1.0f / nTick)) 
		{
			Sleep(10);
			return true;
		}
		fDelta = fElapsed;
		fElapsed = 0.0f;

		m_ServerInfo.nFPS = m_FrameCounter.Update();
	}

	gmgr.pChatChannelManager->Update(fDelta);
	gsys.pPostOfficeSystem->Update();

	UpdateLog();
	UpdateForDebug(fDelta);
	m_pServableChecker->Update(fDelta);
	m_pServerStatusUpdater->Update(fDelta);
	if (m_pCommandProfileController)
		m_pCommandProfileController->Update(fDelta);

	return true;
}

// �������� ���� ���� ���� ������ ǥ���Ѵ�.
void PServer::WriteServerInfoLog()
{
	mlog("Server Created. (WorldID=%d , ServerID=%d)\n", PConfig::m_nMyWorldID, PConfig::m_nMyServerID);
}


void PServer::UpdateServerInfoView()
{
	gmgr.pGameServerObjectManager->Lock();
	m_ServerInfo.nCurrGameServerCount = gmgr.pGameServerObjectManager->GetClientsCount();
	gmgr.pGameServerObjectManager->Unlock();

	PNetServer* pNetServer = dynamic_cast<PNetServer*>(m_pNetServer);
	m_ServerInfo.nRecvCPS = pNetServer->GetRecvCPS();
	m_ServerInfo.nSendCPS = pNetServer->GetSendCPS();
	m_ServerInfo.nLocalCPS = pNetServer->GetLocalCPS();

	m_pNetServer->GetSecondTraffic(m_ServerInfo.nRecvBPS, m_ServerInfo.nSendBPS);

	// UI ������Ʈ
	// ZApplication::GetInstancePtr()->SetServerInfoView(m_ServerInfo);
}

void PServer::UpdateForDebug( float fDelta )
{
	// ���� ���� ������Ʈ
	if (m_pServerInfoViewRglt->IsReady(fDelta))
	{
		UpdateServerInfoView();	
	}
}


void PServer::UpdateLog()
{
	vector<PLog*> vecLog;
	gmgr.pGameServerObjectManager->PickUpLog(MAX_EXECUTE_QUERY, vecLog);

	for each (PLog* pLog in vecLog)
	{
		wstring strQuery = pLog->MakeQuery();		
		gsys.pDBManager->Execute(strQuery.c_str());
		SAFE_DELETE(pLog);
	}	
}

void PServer::StartCommandProfile()
{
	if (m_pCommandProfileController->IsStarted())
	{
		mlog("!Notice! Command Profiler Already Started!\n");
		return;
	}

	mlog("Command Profiler Start!\n");
	m_pCommandProfileController->Start();
}

void PServer::FinishCommandProfile()
{
	mlog("Command Profiler Finish!\n");
	m_pCommandProfileController->Save(true);
}

void PServer::Exit()
{
	m_bExiting = true;
}
