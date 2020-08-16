#ifndef _G_GAME_SERVER_MGR_H_
#define _G_GAME_SERVER_MGR_H_

#include "MUID.h"
#include "GDef.h"
#include "MSync.h"

class GGameServer;
class GLog;

/// ���� ���� ������
class GGameServerMgr
{
public:
	friend class GServer;
	typedef map<MUID, GGameServer*> MAP_GAMESERVER;

private:
	MAP_GAMESERVER			m_mapGameServer;
	MCriticalSection		m_Lock;

private:
	GGameServer* PickUpServer();

public:
	GGameServerMgr();
	~GGameServerMgr();
	GGameServer* New(MUID& uid);
	void Insert(GGameServer* pGameServer);
	void Delete(MUID uidClient);
	void Clear();
	int  GetSize() { return (int)m_mapGameServer.size(); }

	// �����忡 �������� ���� �Լ� - Lock, Unlock�� ����ؾ��Ѵ�.
	void Lock();
	void Unlock();
	GGameServer* Get(const MUID& uidClient);			// �����忡 �������� �ʴ�.
	MAP_GAMESERVER& GetContainer()	{ return m_mapGameServer; }	

	void PickUpLog(int nQuantity, vector<GLog*>& outvecLog);
};


#endif