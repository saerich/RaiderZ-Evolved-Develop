#include "StdAfx.h"
#include "GDynamicFieldPolicy_BattleArena.h"
#include "GDynamicFieldMaster.h"


GDynamicFieldPolicy_BattleArena::GDynamicFieldPolicy_BattleArena(GDynamicFieldMaster* pOwner)
: GDynamicFieldPolicy(pOwner)
, m_bEnterable(true)
{
}

GDynamicFieldPolicy_BattleArena::~GDynamicFieldPolicy_BattleArena()
{
}

FIELD_ENTER_FAIL_TYPE GDynamicFieldPolicy_BattleArena::IsEnterable()
{
	if (!m_bEnterable)
		return ENTERFAIL_PROCEED_INTERMISSION;

	return __super::IsEnterable();
}

void GDynamicFieldPolicy_BattleArena::OnUpdate(float fDelta)
{
}

bool GDynamicFieldPolicy_BattleArena::OnEnterPlayer(GEntityPlayer* pPlayer)
{

	return true;
}

void GDynamicFieldPolicy_BattleArena::OnLeavePlayer(GEntityPlayer* pPlayer)
{
}

void GDynamicFieldPolicy_BattleArena::OnDestroy()
{
}

void GDynamicFieldPolicy_BattleArena::OnBattleArenaShowScoreBoard( const BattleArena::TEAM_MAP& mapEntries )
{
	// ������ ���̴� �������� ���� �Ұ�
	m_bEnterable = false;
}

void GDynamicFieldPolicy_BattleArena::OnBattleArenaComplete( const BattleArena::TEAM_MAP& mapEntries )
{
	// ���� ����� �ʵ� ����
	m_pOwner->RunSelfDestructor();
}

