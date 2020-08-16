#pragma once

#include "GFieldInfo.h"

class GFieldInfoMgr;
class GFieldMgr;
class GTestSystem;
class GSharedField;
class GSharedFieldMaster;
class GServer;

class GTrialField;
class GInnRoomField;
class GDynamicFieldMaster;

class GFieldFactory : public MTestMemPool<GFieldFactory>
{
protected:
	GFieldMgr*			m_pFieldMgr;

public:
	GFieldFactory(GFieldMgr* pFieldMgr) : m_pFieldMgr(pFieldMgr) {}
	virtual ~GFieldFactory() {}


	/// �����ϰ� �ִ� ��� ���� �ʵ带 ä�κ��� �����Ѵ�.
	bool CreateAllSharedField(GFieldInfoMgr* pFieldInfoMgr, GTestSystem* pTestSystem, GServer* pServer);



	// �����͸� ��� --
	GSharedField* CreateSharedField( MUID uidNewField, int nFieldID, CHANNELID nNewChannelID );
	GSharedFieldMaster* CreateSharedFieldMaster( GFieldInfo* pFieldInfo );


	// New
	GTrialField* NewTrialField(MUID& uidNew, MUID uidGroup);
	GInnRoomField* NewInnRoomField(MUID& uidNew, MUID uidGroup);
	
	GDynamicFieldMaster* NewDynamicFieldMaster(const MUID& uidCreator, const DYNAMIC_FIELD_TYPE eType);
	GSharedFieldMaster* NewSharedFieldMaster(GFieldInfo* pFieldInfo);
};