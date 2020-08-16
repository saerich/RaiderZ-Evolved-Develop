#pragma once

#include "MSingleton.h"
#include "MTime.h"
#include "LNetServer.h"
#include "LServerInfo.h"
#include "LServerSystems.h"

class LCmdHandler_Master;
class LCommandCenter;

/// ���� ���� ���� Ŭ����
class LServer
{
protected:
	friend class LNetServer;

	bool						m_bCreated;
	LServerInfo					m_ServerInfo;					///< ���� ����
	minet::MNetServer*			m_pNetServer;					///< ��Ʈ��ũ ����
	LCommandCenter*				m_pCommandCenter;
	MRegulator*					m_pServerInfoViewRglt;

private:
	void	WriteServerInfoLog();
	void	UpdateServerInfoView();
	void	DeInitInfo();
protected:
	virtual bool CreateNetwork();
	virtual void DestroyNetwork();

	bool	InitDB();	
	virtual bool	InitInfo();

	virtual void	UpdateForDebug( float fDelta );
public:
	LServer();
	virtual ~LServer();

	virtual bool Create();
	virtual void Destroy();
	virtual void Update(float fDelta);

	virtual MUID	NewUID()			{ return m_pNetServer->NewUID(); }
			MUID	GetUID()			{ return m_pNetServer->GetUID(); }
	bool			IsCreated()			{ return m_bCreated; }
	LServerInfo&	GetServerInfo()		{ return m_ServerInfo; }
};




