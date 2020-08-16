#include "stdafx.h"
#include "GExtraActiveTalent.h"
#include "GEntityNPC.h"
#include "GEntityPlayer.h"
#include "GField.h"
#include "CCommandTable.h"
#include "GTalentHitter.h"
#include "GTalentRouter.h"
#include "GModuleCombat.h"
#include "GNPCSwallow.h"
#include "GuardEffector.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// GExtraActiveTalent ////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// GExtraActiveTalent_Grapple ////////////////////////////////////////////////////////////////

GExtraActiveTalent_Grapple::GExtraActiveTalent_Grapple( GEntityActor* pOwner, GTalentInfo* pTalentInfo, TALENT_TARGET_INFO Target, bool bGainStress/*=true*/ ) 
: GTalent(pOwner, pTalentInfo, Target, bGainStress)
{

}

void GExtraActiveTalent_Grapple::OnStart()
{
	// ��Ʈĸ�� Ÿ�ٸ� ��ȿ�մϴ�. (���� ����)
	m_pTalentInfo->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_HITCAPSULE;

	GTalent::OnStart();
}

void GExtraActiveTalent_Grapple::OnHitArea( GEntityActor* pVictim, uint16 nCheckTime, int8 nCapsuleGroupIndex, int8 nCapsuleIndex )
{
	if (pVictim->IsNPC())
		return; // NPC�� ���� �� ����

	// �� �ŷ�Ʈ�� �� 1�� ������ �� �ִ�.
	if (m_TalentHit.HasVictim()) return;

	GTalentHitter_Instant* pNewHitter = GTalentHitter::NewTalentHitter< GTalentHitter_Instant >();
	pNewHitter->Setup(m_pOwner->GetUID(), m_pTalentInfo, pVictim, 0, nCapsuleGroupIndex, nCapsuleIndex);
	if (m_pOwner->GetModuleCombat()) m_pOwner->GetModuleCombat()->AddTargetHitter(pNewHitter);

	if ( pVictim->IsNowInvincibility())
		return;

	GuardEffector guard_effector;
	if (GuardEffector::GUARD_SUCCESS == guard_effector.CheckGuard(pVictim, m_pOwner, m_pTalentInfo))
		return; // ���� ����

	if (pVictim->IsHittable(m_pOwner, m_pTalentInfo) == false) return;

	if (pVictim->GetCurrentMotionFactor() == MF_STUN) return;
	if (pVictim->GetActionState() == AS_SWIMMING) return;

	// ��⿡ �ɸ��� ������� �ŷ�Ʈ ���
	pVictim->doCancelTalentForce(false);

	ChangePhase(TALENT_PHASE_EXTRA);
}

void GExtraActiveTalent_Grapple::OnEnterPhaseExtra()
{
	__super::OnEnterPhaseExtra();

	// �ŷ�Ʈ ����ð��� ExtraPhaseTime��ŭ �ð� ����
	m_TalentExpirer.SetExpiredTime(m_fElapsedTime+m_pTalentInfo->m_fExtraPhaseTime);

	m_pRouter->RouteExtraActTalent(m_pTalentInfo);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// GExtraActiveTalent_Swallowed ////////////////////////////////////////////////////////////////

GExtraActiveTalent_Swallowed::GExtraActiveTalent_Swallowed( GEntityActor* pOwner, GTalentInfo* pTalentInfo, TALENT_TARGET_INFO Target, bool bGainStress/*=true*/ ) 
: GTalent(pOwner, pTalentInfo, Target, bGainStress)
{

}

void GExtraActiveTalent_Swallowed::OnStart()
{
	// ��Ʈĸ�� Ÿ�ٸ� ��ȿ�մϴ�. (���� ����)
	m_pTalentInfo->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_HITCAPSULE;

	GTalent::OnStart();
}

void GExtraActiveTalent_Swallowed::OnHitArea( GEntityActor* pVictim, uint16 nCheckTime, int8 nCapsuleGroupIndex, int8 nCapsuleIndex )
{
	if (!m_pOwner->IsNPC())
		return;	// NPC�� ���� �� ����

	if (pVictim->IsNPC())
		return; // NPC�� ���� �� ����

	// �� �ŷ�Ʈ�� �� 1�� ������ �� �ִ�.
	if (m_TalentHit.HasVictim()) return;

	GTalentHitter_Instant* pNewHitter = GTalentHitter::NewTalentHitter< GTalentHitter_Instant >();
	pNewHitter->Setup(m_pOwner->GetUID(), m_pTalentInfo, pVictim, 0, nCapsuleGroupIndex, nCapsuleIndex);

	if (m_pOwner->GetModuleCombat()) 
		m_pOwner->GetModuleCombat()->AddTargetHitter(pNewHitter);

	if ( pVictim->IsNowInvincibility())
		return;

	GuardEffector guard_effector;
	if (GuardEffector::GUARD_SUCCESS == guard_effector.CheckGuard(pVictim, m_pOwner, m_pTalentInfo))
		return; // ���� ����

	if (!pVictim->IsHittable(m_pOwner, m_pTalentInfo)) 
		return;

	if (pVictim->GetCurrentMotionFactor() == MF_STUN) 
		return;

	if (pVictim->GetActionState() == AS_SWIMMING) 
		return;

	// �Ա� ó��
	GEntityNPC* pOwnerNPC = ToEntityNPC(m_pOwner);
	if (!pOwnerNPC->GetNPCSwallow().Swallow(pVictim, m_pTalentInfo))
		return;	// ���� �� ���� ����

	// ��⿡ �ɸ��� ������� �ŷ�Ʈ ���
	pVictim->doCancelTalentForce(false);

	ChangePhase(TALENT_PHASE_EXTRA);
}

void GExtraActiveTalent_Swallowed::OnEnterPhaseExtra()
{
	__super::OnEnterPhaseExtra();

	m_fActTime = m_fElapsedTime; // �ŷ�Ʈ �����ð��� ���߱� ���� ��Ʈ�ð����� ����

	// �ŷ�Ʈ ����ð��� ExtraPhaseTime��ŭ �ð� ����
	m_TalentExpirer.SetExpiredTime(	m_fElapsedTime+
									m_pTalentInfo->m_fExtraPhaseTime+	// �Դ� �ִϸ��̼� 
									(float)m_pTalentInfo->m_nExtraActiveParam1+	// �԰� �ִ� �ð�
									max(m_pTalentInfo->m_fExtraPhaseTime2, m_pTalentInfo->m_fExtraPhaseTime3));	// ��� �ִϸ��̼�

	m_rgrPhaseExtra2.SetTickTime(m_pTalentInfo->m_fExtraPhaseTime+(float)m_pTalentInfo->m_nExtraActiveParam1);

	m_pRouter->RouteExtraActTalent(m_pTalentInfo);
}

void GExtraActiveTalent_Swallowed::OnUpdatePhaseExtra( float fDelta )
{
	__super::OnUpdatePhaseExtra(fDelta);

	m_fActTime = m_fElapsedTime; // �ŷ�Ʈ �����ð��� ���߱� ���� ��Ʈ�ð����� ����

	GEntityNPC* pOwnerNPC = ToEntityNPC(m_pOwner);
	if (pOwnerNPC->GetNPCSwallow().IsSwallowable())
	{
		// ���� �� �ִ� ���°� �Ǹ� ���� ����
		ChangePhase(TALENT_PHASE_EXTRA3);
		return;
	}

	if (m_rgrPhaseExtra2.IsReady(fDelta))
	{
		ChangePhase(TALENT_PHASE_EXTRA2);
		return;
	}
}

// ==> �ð��� �� �Ǿ� ���
void GExtraActiveTalent_Swallowed::OnEnterPhaseExtra2()
{
	__super::OnEnterPhaseExtra2();

	m_fActTime = m_fElapsedTime; // �ŷ�Ʈ �����ð��� ���߱� ���� ��Ʈ�ð����� ����

	// �ŷ�Ʈ ����ð��� ExtraPhaseTime2��ŭ �ð� ����
	m_TalentExpirer.SetExpiredTime(m_fElapsedTime+m_pTalentInfo->m_fExtraPhaseTime2);

	// ��� ó��
	GEntityNPC* pOwnerNPC = ToEntityNPC(m_pOwner);
	if (!pOwnerNPC->GetNPCSwallow().Spewup(false))
		return;	// ���� �� ���� ����
}

// ==> �Դ� ���߿� ���� �޾� ���
void GExtraActiveTalent_Swallowed::OnEnterPhaseExtra3()
{
	__super::OnEnterPhaseExtra3();

	// �ŷ�Ʈ ����ð��� ExtraPhaseTime3��ŭ �ð� ����
	m_TalentExpirer.SetExpiredTime(m_fElapsedTime+m_pTalentInfo->m_fExtraPhaseTime3);
}
