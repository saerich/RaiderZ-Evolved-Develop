#pragma once

#include "GTestWrapper.h"
#include "GPlayerObjectManager.h"
#include "STypes.h"

class MockPlayerObject;
class GField;
class GEntityPlayer;
class GEntityActor;

struct FBasePlayer
{
	FBasePlayer()
	{

	}
	virtual ~FBasePlayer()
	{

	}

	MockPlayerObject* NewMockPlayerObject(MUID uidPlayerObject);

	// �÷��̾� ��ƼƼ�� �����Ѵ�.  uidNew�� ��ȿ�Ұ�� ���ο� UID�� �߱����ش�.
	GEntityPlayer* NewEntityPlayer( GField* pField, vec3 pos=vec3::ZERO, MUID uidNew=MUID::Invalid());
	// �÷��̾� ��ƼƼ�� �����Ѵ�.
	void DeleteEntityPlayer(GEntityPlayer* pPlayer);
	// �÷��̾��� ���°��� ����
	void SetStatus( GEntityPlayer* pEntityPlayer, int nHP, int nEN, int nSTA );
	// �ش� ���Ϳ� �������·� ����
	void MakeCombat( GEntityPlayer* pEntityPlayer, GEntityNPC* pOpponent );

	GTestMgrWrapper<GPlayerObjectManager>	m_PlayerObjectManagerWrapper;
};