#pragma once

class GEntityActor;
class GEntityPlayer;
class GTalentInfo;
enum QUESTPVP_TEAM;

//////////////////////////////////////////////////////////////////////////
//
// GPlayerPVPArea
//
//////////////////////////////////////////////////////////////////////////

class GPlayerPVPArea
{
public:
	GPlayerPVPArea(GEntityPlayer*	pOwnerPlayer);
	TEST_VIRTUAL ~GPlayerPVPArea(void);

	// �����ڷκ��� �ǰݰ������� ����
	bool IsHittable( GEntityActor* pAttacker );
	// PVP ������ ������ �ִ��� ����
	TEST_VIRTUAL bool IsLocatedPvPArea();
	// �ش� PVP �������� �����ڰ� ���������� ��ȯ
	TEST_VIRTUAL QUESTPVP_TEAM GetTeam();

	// PVP ������ ������ �ְ� Ư���� �����ִ��� ��ȯ
	TEST_VIRTUAL bool IsLocateAndBelongTeam();
	
private:
	GEntityPlayer*	m_pOwner;
};
