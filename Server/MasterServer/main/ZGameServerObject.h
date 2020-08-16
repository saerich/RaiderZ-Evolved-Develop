#pragma once

#include "MUID.h"
#include "SCommObject.h"
#include <map>
using namespace std;

#include "ZGameServerInfo.h"
#include "ZGameServerFieldMap.h"
#include "ZGameServerPlayerMap.h"
#include "MCommand.h"
using namespace minet;

class ZGameServerObjectManager;
class ZServer;
class ZFieldManager;

/// ���� ���� ���� ����
class ZGameServerStatus
{
public:
	wstring				m_strAddress;
	int					m_nUserCount;	///< ���� ��

	ZGameServerStatus() : m_nUserCount(0)
	{
		;
	}
};

struct TD_LOGIN_ACCOUNT_INFO;
struct TD_LOGIN_GAME_SERVER_INFO;

/// ���Ӽ��� ������Ʈ
class ZGameServerObject : public SCommObject
{	
protected:
	ZGameServerInfo*		m_pGameServerInfo;
	SServerType				m_nServerType;
	ZGameServerStatus		m_Status;

	ZGameServerFieldMap		m_FieldMap;
	ZGameServerPlayerMap	m_PlayerMap;

	ZGameServerObject() : SCommObject(), m_nServerType(SERVER_GAME), m_pGameServerInfo(NULL) {}
public:
	ZGameServerObject(MUID uidObject, ZGameServerInfo* pGameServerInfo);
	virtual ~ZGameServerObject();
	bool Init(ZFieldManager* pFieldManager);
	
	// field
	ZGameServerFieldMap& GetFieldMap()		{ return m_FieldMap; }

	// player
	ZGameServerPlayerMap& GetPlayerMap()	{ return m_PlayerMap; }


	int GetID() const { return m_pGameServerInfo->m_nID; }
};

