#include "StdAfx.h"
#include "GNPCCombatAnticheat.h"
#include "GEntityNPC.h"

#define CONST_CHEAT_NPC_ATTACK_AVOID_COUNT 5
#define CONST_CHEAT_NPC_ATTACK_AVOID_RANGE 500.0f

GNPCCombatAnticheat::GNPCCombatAnticheat(GEntityNPC* pOwner)
: m_pOwner(pOwner)
, m_nAvoidCount(0)
, m_vLastAttackedPos(vec3::ZERO)
, m_vLastAttackedDir(vec3::ZERO)
{
	Reset();
	m_pOwner->AttachObserver(this);
}

GNPCCombatAnticheat::~GNPCCombatAnticheat(void)
{
	m_pOwner->DetachObserver(this);
}

void GNPCCombatAnticheat::OnCombatBegin(GEntityActor* pOwner)
{
	Reset();
}

void GNPCCombatAnticheat::OnCombatEnd(GEntityActor* pOwner)
{
	Reset();
}

void GNPCCombatAnticheat::OnHitEnemy( GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo )
{
	Reset();
}

void GNPCCombatAnticheat::OnGuardEnemy( GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo )
{
	Reset();
}


void GNPCCombatAnticheat::OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo )
{
	VALID(m_pOwner);

	if (m_vLastAttackedPos == vec3::ZERO)
	{
		m_vLastAttackedPos = m_pOwner->GetPos();
	}

	if (m_vLastAttackedDir == vec3::ZERO)
	{
		m_vLastAttackedDir = m_pOwner->GetDir();
	}

	if (pTalentInfo->m_nSkillType != ST_MELEE)		
		return;	// �������ݸ� ȸ���ߴٰ� �����Ѵ�.
	
	if (!pTalentInfo->isAttackable())				
		return; // ���ݿ� �ŷ�Ʈ�� �ƴ�

	if (!m_pOwner->IsNowCombat())
		return; // ���� ���� �ƴ�

	if (IsAvoidRange())
	{
		++m_nAvoidCount;
		m_vLastAttackedPos = m_pOwner->GetPos();
		m_vLastAttackedDir = m_pOwner->GetDir();
	}
	else
	{
		Reset();
	}
}

void GNPCCombatAnticheat::OnFinishTalent( GTalentInfo* pTalentInfo )
{
	if (!IsAvoidRange())
	{
		// �������� ������� ī���� �ʱ�ȭ
		Reset();
	}
}

void GNPCCombatAnticheat::OnTalentImmunedEnemy( GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo )
{
	Reset();
}

bool GNPCCombatAnticheat::IsCheating() const
{
	return m_nAvoidCount >= CONST_CHEAT_NPC_ATTACK_AVOID_COUNT;
}

void GNPCCombatAnticheat::Reset()
{
	m_nAvoidCount = 0;
	m_vLastAttackedPos = vec3::ZERO;
	m_vLastAttackedDir = vec3::ZERO;
}

bool GNPCCombatAnticheat::IsAvoidRange() const
{
	if (m_vLastAttackedPos.DistanceTo(m_pOwner->GetPos()) > CONST_CHEAT_NPC_ATTACK_AVOID_RANGE)
		return false;

	if (m_vLastAttackedDir.DotProduct(m_pOwner->GetDir()) < 0.75f)
		return false;

	return true;
}

