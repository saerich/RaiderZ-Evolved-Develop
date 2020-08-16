#pragma once

#include "MSingleton.h"
#include "MTime.h"
#include "PNetServer.h"
#include "PServerInfo.h"
#include "PServerSystems.h"
#include "SFrameCounter.h"
#include "PProxyPlayerListener.h"

class PCmdHandler_Master;
class PCommandCenter;
class PServerStatusUpdater;
class SProxyPlayerManager;
class PServableChecker;
class PMasterServerFacade;
class SCommandProfileController;

/// ���� ���� ���� Ŭ����
class PServer
{
protected:
	friend class PNetServer;

	bool						m_bCreated;
	bool						m_bExiting;						///< ������ ����Ǵ� ������ ����
	PServerInfo					m_ServerInfo;					///< ���� ����
	minet::MNetServer*			m_pNetServer;					///< ��Ʈ��ũ ����
	PCommandCenter*				m_pCommandCenter;
	SCommandProfileController*	m_pCommandProfileController;
	MRegulator*					m_pServerInfoViewRglt;
	PServerStatusUpdater*		m_pServerStatusUpdater;
	PServableChecker*			m_pServableChecker;

	PMasterServerFacade*		m_pMasterServerFacade;
	SProxyPlayerManager*		m_pProxyPlayerManager;
	PProxyPlayerListener		m_proxyPlayerListener;


	SFrameCounter				m_FrameCounter;
private:
	void			WriteServerInfoLog();
	void			UpdateServerInfoView();
	void			DeInitInfo();

protected:
	bool			InitRequisites();

	virtual bool 	CreateNetwork();
	virtual void 	DestroyNetwork();
	virtual bool	InitDB();
	virtual bool	InitInfo();
	virtual bool	LoadInfoFiles(void);

	virtual void	UpdateForDebug( float fDelta );
	void			UpdateLog();
public:
	PServer();
	virtual ~PServer();

	virtual bool Create();
	virtual void Destroy();
	virtual bool Update(float fDelta);

	void				Exit(); // ���� ���� ó��

	void					StartCommandProfile();
	void					FinishCommandProfile();

	virtual MUID			NewUID()				{ return m_pNetServer->NewUID(); }
			MUID			GetUID()				{ return m_pNetServer->GetUID(); }
	bool					IsCreated()				{ return m_bCreated; }
	PServerInfo&			GetServerInfo()			{ return m_ServerInfo; }
	PServableChecker&		GetServableChecker()	{ return *m_pServableChecker; }

	SProxyPlayerManager*	GetPlayerManager()		{ return m_pProxyPlayerManager;	}
};
