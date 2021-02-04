#include "stdafx.h"
#include "ZServer.h"
#include "ZDef.h"
#include "ZConfig.h"
#include "ZCommandTable.h"
#include "ZCommandCenter.h"
#include "ZGameServerObjectManager.h"
#include "ZGameServerObject.h"
#include "ZGameServerInfoLoader.h"
#include "ZGameServerInfoManager.h"
#include "ZLoginServerInfoLoader.h"
#include "ZLoginServerInfoManager.h"
#include "ZLoginServerFacade.h"
#include "ZAppServerInfoLoader.h"
#include "ZAppServerInfoManager.h"
#include "ZAppServerObjectManager.h"
#include "SFilePath.h"
#include "ZFieldInfoManager.h"
#include "ZDBManager.h"
#include "ZServerStatusUpdater.h"
#include "ZScriptManager.h"
#include "ZConst.h"
#include "ZServableChecker.h"
#include "SCommandProfileController.h"
#include "ZChannelBeholder.h"
#include "PMServerInitLogger.h"
#include "MCommandProfiler.h"
#include "SDsnFactory.h"
#include "SBaseDsnFactory.h"
#include "SDefaultDsnFactory.h"
//#include "PmDsnFactory.h"
#include "ZParty.h"
#include "ZPartyManager.h"
#include "ZPartyMember.h"
#include "CTransData.h"
#include "MLocale.h"
#include "ZWorldStatusUpdater.h"
#include "ZFixedPartyInfoManager.h"


//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////

ZServer::ZServer()
: m_bCreated(false)
, m_pCommandCenter(NULL)
, m_pNetServer(NULL)
, m_pServerStatusUpdater(NULL)
, m_pWorldStatusUpdater(NULL)
, m_ReserverKeyGenerator(1)
, m_pCommandProfileController(NULL)
, m_pChannelBeholder(NULL)
{
	m_pServerInfoViewRglt = new MRegulator(APP_SERVERVIEW_UPDATE_TICK);
	m_pServableChecker = new ZServableChecker();
}

ZServer::~ZServer()
{
	SAFE_DELETE(m_pChannelBeholder);
	SAFE_DELETE(m_pServableChecker);
	SAFE_DELETE(m_pServerStatusUpdater);
	SAFE_DELETE(m_pWorldStatusUpdater);
	SAFE_DELETE(m_pServerInfoViewRglt);
}

bool ZServer::Create()
{
	if (InitRequisites() == false)
	{
		return false;
	}
	
	if (InitInfo() == false)
	{
		return false;
	}

	m_pChannelBeholder = new ZChannelBeholder();

	m_bCreated = true;

	WriteServerInfoLog();


	/// �ε��� ��� ���� �ڿ� Server Status Update �� �����ؾ� �մϴ�.
	std::wstring strServerVersion = L"";
	m_pServerStatusUpdater->Start(strServerVersion);

	m_pWorldStatusUpdater->Start();


	return true;
}

void ZServer::Destroy()
{
	gsys.pScriptManager->Fini();

	ReleaseDB();

	DestroyNetwork();

	m_bCreated = false;

	mlog("Server Closed\n");
}

bool ZServer::InitRequisites()
{
	if (CreateNetwork() == false)
	{
		mlog3("Failed CreateNetwork.\n");
		SetServerInitResult(SERVERINIT_FAILED_NETWORK_INIT);
		return false;
	}

	if (InitDB() == false)
	{
		_ASSERT(0);
		mlog3("Failed InitDB\n");
		SetServerInitResult(SERVERINIT_FAILED_DB_CONNECT);
		return false;
	}
	
	// InitStatusUpdater
	m_pServerStatusUpdater = new ZServerStatusUpdater(gsys.pDBManager);
	m_pWorldStatusUpdater = new ZWorldStatusUpdater(gsys.pDBManager);


	return true;
}

bool ZServer::CreateNetwork()
{
	minet::MNetServerDesc server_desc;	// heartbeat üũ�� ���� �ʽ��ϴ�.
	m_pNetServer = new ZNetServer(server_desc);
	m_pNetServer->AttachCommandProfiler(new MCommandProfiler());

	m_pCommandCenter = new ZCommandCenter(m_pNetServer);
	gsys.pCommandCenter = m_pCommandCenter;	// �ϴ�..
	m_pCommandProfileController = new SCommandProfileController(m_pNetServer,
		ZConfig::m_strCommandProfiler_LogFolder,
		ZConfig::m_strCommandProfiler_LogName,
		ZConfig::m_bCommandProfiler_AutoSave,
		ZConfig::m_nCommandProfiler_AutoSaveTickMinute);

	if (ZConfig::m_bCommandProfiler_AutoStart)
		StartCommandProfile();

	/// ��Ʈ��ũ ��� ����
	MUID uidSeed = MUID(ZConfig::m_nMyServerID * SERVER_UID_SEED_POWER, 0);
	if (m_pNetServer->Create(ZConfig::m_nPort, false, uidSeed, ZConfig::m_nSocketPoolSize, ZConfig::m_nSendPendingLimitCount) == false)
	{
		return false;
	}

	mlog("Server Socket Opened. (Port = %d)\n", ZConfig::m_nPort);

	return true;
}

void ZServer::DestroyNetwork()
{
	if (m_pNetServer)
	{
		m_pNetServer->Destroy();
	}

	SAFE_DELETE(m_pCommandProfileController);
	SAFE_DELETE(m_pCommandCenter);
	SAFE_DELETE(m_pNetServer);
	gsys.pCommandCenter = NULL;
}

bool ZServer::InitDB()
{
	SDsnFactory::GetInstance().Set(
		new SDefaultDsnFactory(
		mdb::MDatabaseDesc(ZConfig::m_strOdbcDriver, ZConfig::m_strAccountDB_Server, ZConfig::m_strAccountDB_DatabaseName, ZConfig::m_strAccountDB_UserName, ZConfig::m_strAccountDB_Password)
		, mdb::MDatabaseDesc(ZConfig::m_strOdbcDriver, ZConfig::m_strGameDB_Server, ZConfig::m_strGameDB_DatabaseName, ZConfig::m_strGameDB_UserName, ZConfig::m_strGameDB_Password)
		, mdb::MDatabaseDesc(ZConfig::m_strOdbcDriver, ZConfig::m_strLogDB_Server, ZConfig::m_strLogDB_DatabaseName, ZConfig::m_strLogDB_UserName, ZConfig::m_strLogDB_Password)));

	//if (ZConfig::m_bPmangDBEncrypt)
	//{
	//	SDsnFactory::GetInstance().Set(
	//		new PmDsnFactory(
	//		mdb::MDatabaseDesc(ZConfig::m_strOdbcDriver, ZConfig::m_strAccountDB_Server, ZConfig::m_strAccountDB_DatabaseName, ZConfig::m_strAccountDB_UserName, ZConfig::m_strAccountDB_Password)
	//		, mdb::MDatabaseDesc(ZConfig::m_strOdbcDriver, ZConfig::m_strGameDB_Server, ZConfig::m_strGameDB_DatabaseName, ZConfig::m_strGameDB_UserName, ZConfig::m_strGameDB_Password)
	//		, mdb::MDatabaseDesc(ZConfig::m_strOdbcDriver, ZConfig::m_strLogDB_Server, ZConfig::m_strLogDB_DatabaseName, ZConfig::m_strLogDB_UserName, ZConfig::m_strLogDB_Password)));
	//}

	mdb::MDatabaseDesc dbAccountDesc = SDsnFactory::GetInstance().Get()->GetAccountDSN();
	mdb::MDatabaseDesc dbGameDesc = SDsnFactory::GetInstance().Get()->GetGameDSN();
	mdb::MDatabaseDesc dbLogDesc = SDsnFactory::GetInstance().Get()->GetLogDSN();

	if (gsys.pDBManager->Init(dbAccountDesc, dbGameDesc, dbLogDesc))
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

bool ZServer::InitInfo()
{
	if (InitDependencyInfo() == false)
	{
		SetServerInitResult(SERVERINIT_FAILED_DEPENDENCYFILE_LOAD);
		return false;
	}

	if (LoadInfoFiles() == false)
	{
		SetServerInitResult(SERVERINIT_FAILED_DATAFILE_LOAD);
		return false;
	}

	if (InitFixedParty() == false)
	{
		SetServerInitResult(SERVERINIT_FAILED_INIT_FIXED_PARTY);
		return false;
	}

	return true;
}

bool ZServer::InitDependencyInfo()
{
	bool bUseCallBackLogger = false;
#ifndef _PUBLISH
	bUseCallBackLogger = true;
#endif

	/// ��� �ʱ�ȭ
	gsys.pScriptManager->Init(bUseCallBackLogger);

	ZConst::Init();

	return true;
}

bool ZServer::LoadInfoFiles()
{
	// �ʵ� ����
	wstring strSystemPath = ZConfig::m_strSystemPath;

	wstring strFieldListFileName = SFilePath::MakeFieldList(strSystemPath.c_str());
	wstring strZoneListFileName = SFilePath::MakeZoneList(strSystemPath.c_str());

	wstring strFieldPath = ZConfig::m_strFieldPath;

	if(gmgr.pFieldInfoManager && gmgr.pFieldInfoManager->Load(strFieldListFileName.c_str(), 
															  strFieldPath.c_str(),
															  ZConst::INCLUDE_DEV_FIELD_LIST) == false)
	{
		return false;
	}


	// ���� ���� ����
	ZGameServerInfoLoader gameServerInfoLoader;
	if (gameServerInfoLoader.Load(gmgr.pGameServerInfoManager, FILENAME_SERVER_LIST, gmgr.pFieldInfoManager) == false)
	{
		mlog3("Failed Load GameServer Info : %s\n", MLocale::ConvUTF16ToAnsi(FILENAME_SERVER_LIST).c_str());
		return false;
	}

	// �α��� ���� ����
	ZLoginServerInfoLoader loginServerInfoLoader;
	if (loginServerInfoLoader.Load(gmgr.pLoginServerInfoManager, FILENAME_SERVER_LIST) == false)
	{
		mlog3("Failed Load LoginServer Info : %s\n", MLocale::ConvUTF16ToAnsi(FILENAME_SERVER_LIST).c_str());
		return false;
	}

	// �� ���� ����
	ZAppServerInfoLoader appServerInfoLoader;
	if (appServerInfoLoader.Load(gmgr.pAppServerInfoManager, FILENAME_SERVER_LIST) == false)
	{
		mlog3("Failed Load LoginServer info : %s\n", MLocale::ConvUTF16ToAnsi(FILENAME_SERVER_LIST).c_str());
		return false;
	}

	return true;
}

bool ZServer::InitFixedParty()
{
	bool bResult = gmgr.pFixedPartyInfoManager->Load(FILENAME_FIXED_PARTY_INFO);
	return bResult;	
}

void ZServer::DeInitInfo()
{
}


void ZServer::Update(float fDelta)
{
	// Ŀ�ǵ� ������ ƽ�� ������� ó���Ѵ�.
	if (m_pNetServer) m_pNetServer->Run();
	// ��� �Ϸ� ó���� ƽ�� ������� ó���Ѵ�.
	if (gsys.pDBManager) gsys.pDBManager->Update();

	{
		int nTick = DEFAULT_UPDATE_TICK;

		static float fElapsed = 0;
		fElapsed += fDelta;

		if (fElapsed < (1.0f / nTick)) 
		{
			Sleep(10);
			return;
		}
		fDelta = fElapsed;
		fElapsed = 0.0f;

		m_ServerInfo.nFPS = m_FrameCounter.Update();
	}

	m_pChannelBeholder->Update(fDelta);

	UpdateForDebug(fDelta);
	m_pServableChecker->Update(fDelta);
	m_pServerStatusUpdater->Update(fDelta);
	m_pWorldStatusUpdater->Update(fDelta);
	
	if (m_pCommandProfileController)
		m_pCommandProfileController->Update(fDelta);
}

// �������� ���� ���� ���� ������ ǥ���Ѵ�.
void ZServer::WriteServerInfoLog()
{
	mlog("Server Created. (WorldID=%d , ServerID=%d)\n", ZConfig::m_nMyWorldID, ZConfig::m_nMyServerID);
}

void ZServer::UpdateServerInfoView()
{
	m_ServerInfo.nMaxRequiredLoginServerCount = gmgr.pLoginServerInfoManager->GetSize();
	m_ServerInfo.nMaxRequiredGameServerCount = gmgr.pGameServerInfoManager->GetSize();
	m_ServerInfo.nMaxRequiredAppServerCount = gmgr.pAppServerInfoManager->GetSize();

	m_ServerInfo.nIsLoginServerConnected = gmgr.pLoginServerFacade->IsConnected() ? 1 : 0;
	m_ServerInfo.nCurrGameServerCount = gmgr.pGameServerObjectManager->GetClientsCount();
	m_ServerInfo.nCurrAppServerCount = gmgr.pAppServerObjectManager->GetClientsCount();

	m_ServerInfo.nCurrPlayerCount = gmgr.pPlayerManager->GetObjectCount();

	m_ServerInfo.nSharedFieldInfoCount = gmgr.pFieldInfoManager->GetFieldInfoMap().size();
	m_ServerInfo.nAllSharedFieldCount = gmgr.pFieldManager->GetSharedFieldCount();


	ZNetServer* pNetServer = dynamic_cast<ZNetServer*>(m_pNetServer);
	m_ServerInfo.nRecvCPS = pNetServer->GetRecvCPS();
	m_ServerInfo.nSendCPS = pNetServer->GetSendCPS();
	m_ServerInfo.nLocalCPS = pNetServer->GetLocalCPS();

	m_pNetServer->GetSecondTraffic(m_ServerInfo.nRecvBPS, m_ServerInfo.nSendBPS);

}

void ZServer::UpdateForDebug( float fDelta )
{
	// ���� ���� ������Ʈ
	if (m_pServerInfoViewRglt->IsReady(fDelta))
	{
		UpdateServerInfoView();	
	}
}

void ZServer::ReleaseDB()
{
	gsys.pDBManager->Release();
}

MUID ZServer::NewReserverKey(void)
{
	return m_ReserverKeyGenerator.Generate();
}

void ZServer::StartCommandProfile()
{
	if (m_pCommandProfileController->IsStarted())
	{
		mlog("!Notice! Command Profiler Already Started!\n");
		return;
	}

	mlog("Command Profiler Start!\n");
	m_pCommandProfileController->Start();
}

void ZServer::FinishCommandProfile()
{
	mlog("Command Profiler Finish!\n");
	m_pCommandProfileController->Save(true);
}
