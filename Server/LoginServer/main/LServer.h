#pragma once

#include "MSingleton.h"
#include "MTime.h"
#include "LNetServer.h"
#include "LServerInfo.h"
#include "LServerSystems.h"
#include "SFrameCounter.h"

class LCmdHandler_Master;
class LCommandCenter;
class LMasterNetClient;
class LServerStatusUpdater;
class LTimeoutManager;
class LPlayerLoginGameServerProcManager;
class LPlayerLoginGameServerRetryer;
class LMasterClient;
class LServableChecker;
class SCommandProfileController;
class LWorldLocatorServer;

/// ���� ���� ���� Ŭ����
class LServer
{
protected:
	friend class LNetServer;

	bool						m_bCreated;
	bool						m_bExiting;						///< ������ ����Ǵ� ������ ����
	LServerInfo					m_ServerInfo;					///< ���� ����
	minet::MNetServer*			m_pNetServer;					///< ��Ʈ��ũ ����
	LWorldLocatorServer*		m_pWorldLocatorServer;			///< ���弱�� UDP ����
	LCommandCenter*				m_pCommandCenter;
	SCommandProfileController*	m_pCommandProfileController;
	MRegulator*					m_pServerInfoViewRglt;
	LMasterClient*				m_pMasterClient;				///< ������ ���� Ŭ���̾�Ʈ
	SFrameCounter				m_FrameCounter;
	LServerStatusUpdater*		m_pServerStatusUpdater;
	LServableChecker*			m_pServableChecker;
	
	LPlayerLoginGameServerProcManager*	m_pPlayerLoginGameServerProcManager;
	LPlayerLoginGameServerRetryer*		m_pPlayerLoginGameServerRetryer;

private:
	void	WriteServerInfoLog();
	void	UpdateServerInfoView();
	void	DeInitInfo();
protected:
	virtual bool CreateNetwork();
	virtual void DestroyNetwork();

	virtual void	SetDsnFactory();
	bool			InitDB();	
	virtual void	ReleaseDB();
	virtual bool	InitInfo();

	virtual void	UpdateForDebug( float fDelta );
public:
	LServer();
	virtual ~LServer();

	virtual bool Create();
	virtual void Destroy();
	virtual bool Update(float fDelta);
	
	virtual void DisconnectPlayer(MUID uidPlayer);
	void HardDisconnectPlayer(MUID uidPlayer);

	static void OnServerStartCallback();

	void Exit(); // ���� ���� ó��

	void StartCommandProfile();
	void FinishCommandProfile();

	virtual MUID			NewUID()				{ return m_pNetServer->NewUID(); }
			MUID			GetUID()				{ return m_pNetServer->GetUID(); }
	bool					IsCreated()				{ return m_bCreated; }
	LServerInfo&			GetServerInfo()			{ return m_ServerInfo; }
	LServableChecker&		GetServableChecker()	{ return *m_pServableChecker; }

	LMasterClient*			GetMasterClient()		{ return m_pMasterClient; }
	LNetServer*				GetNetServer()			{ return dynamic_cast<LNetServer*>(m_pNetServer); }
	LWorldLocatorServer*	GetWorldLocatorServer()	{ return m_pWorldLocatorServer;	}

	LPlayerLoginGameServerProcManager*	GetPlayerLoginGameServerProcManager() { return m_pPlayerLoginGameServerProcManager; }
};
