#include "StdAfx.h"
#include "GBuffInstantApplier.h"
#include "GBuff.h"
#include "GEntityNPC.h"
#include "GEntityPlayer.h"
#include "GModuleBuff.h"
#include "GHateTable.h"
#include "CCommandTable.h"
#include "GCommand.h"
#include "GBuffInfo.h"
#include "GGlobal.h"
#include "GTalentInfoMgr.h"
#include "GTalentHitRouter.h"
#include "GActorHateTracker.h"
#include "GNPCLoot.h"
#include "GBuffRequirement.h"
#include "GTalentRouter.h"

GBuffInstantApplier::GBuffInstantApplier( GBuff* pOwner )
: m_pOwner(pOwner)
{
}

GBuffInstantApplier::~GBuffInstantApplier(void)
{
}

bool GBuffInstantApplier::OnEvent(TALENT_CONDITION nTiming)
{
	return ApplyEffects();
}

bool GBuffInstantApplier::ApplyEffects()
{
	if (!m_pOwner->GetOwner()->IsActor())
		return false;

	GBuffInfo* pBuffInfo = m_pOwner->GetInfo();
	GEntityActor* pOwnerActor = ToEntityActor(m_pOwner->GetOwner());

	GBuffRequirement buff_requirement;
	if (!buff_requirement.ProcessRequirement(pOwnerActor, pBuffInfo->m_RequireBuff))
		return false; // ���� �ʿ����� ������
		
	MUID uidLastHitOrHittedEnemy = pOwnerActor->GetLastHitOrHittedEnemy();
	GEntityActor* pLastHitOrHittedEnemy = pOwnerActor->FindActor(uidLastHitOrHittedEnemy);

	GEffectTargetSelector m_EffecteeTargetSelector;
	GEffectTargetSelector::Desc desc(pBuffInfo->m_EffectInfo);
	desc.pSelf = pOwnerActor;
	desc.pTarget = pLastHitOrHittedEnemy;
	desc.pCaster = pOwnerActor->FindActor(m_pOwner->GetUserUID());
	set<GEntityActor*> vecEffecteeTarget;
	m_EffecteeTargetSelector.Select(desc, vecEffecteeTarget);

	for each (GEntityActor* pEffecteeTarget in vecEffecteeTarget)
	{
		ApplySingleEffect(pEffecteeTarget, m_pOwner->GetUserUID());
	}	

	return pOwnerActor->IsDead();
}

void GBuffInstantApplier::ApplySingleEffect(GEntityActor* pEffecteeTarget, MUID uidUser)
{
	VALID(pEffecteeTarget);
	VALID(m_pOwner);
	GBuffInfo* pBuffInfo = m_pOwner->GetInfo();
	VALID(pBuffInfo);

	pEffecteeTarget->GetModuleBuff()->EnableActiveExtraAttrib(m_pOwner);
	GEntityActor* pUser = pEffecteeTarget->FindActor(uidUser);

	
	int nBeforeHP = pEffecteeTarget->GetHP();

	GModEffector				m_ModEffector;
	CSInstantModifier modifier = pBuffInfo->m_InstantModifier;
	for (int i=0; i<m_pOwner->GetStackCount(); ++i)
	{
		m_ModEffector.ApplyInstantModifier(pEffecteeTarget, modifier);	
	}
	pEffecteeTarget->GetModuleBuff()->EnableActiveExtraAttrib(m_pOwner);

	if (!pBuffInfo->IsEnchant())
	{
		RouteGainInstantEffect(pEffecteeTarget, m_pOwner);
	}
	
	// Modifier �������� HP�� ȸ���Ǿ�����, Hate ���̺� ����
	int nHealHPAmount = pEffecteeTarget->GetHP() - nBeforeHP;
	if (nHealHPAmount > 0)
	{
		BuffHealed(pEffecteeTarget, pBuffInfo, nHealHPAmount);
	}


	if (IsNeedEffect())
	{
		GainBuffDamageAndHeal(pEffecteeTarget, m_pOwner, uidUser, pUser);
	}

	// �߰� �ŷ�Ʈ ���
	if (pBuffInfo->HasUseTalent())
	{
		GTalentInfo* pTalentInfo = gmgr.pTalentInfoMgr->Find(pBuffInfo->m_nTalentID);
		if (pTalentInfo)
		{
			GTalentEffector::Info info;
			info.pTalentInfo = pTalentInfo;
			info.pUser = pEffecteeTarget;
			info.pTarget = pEffecteeTarget;
			info.nTalentCondition = TC_USE_TALENT;
			GTalentEffector effector;
			effector.ApplyTalent(info);
			//pEffecteeTarget->doUseTalent(pTalentInfo);
		}
	}

	// ����Ʈ ����
	if (pEffecteeTarget->IsNPC() &&
		pUser) // Ÿ���� ��ȿ��
	{
		GEntityNPC* pTargetNPC = ToEntityNPC(pEffecteeTarget);
		DCHECK(pTargetNPC);
		pTargetNPC->GetHateTable().AddPoint_EnemyApplyEffect(pUser, pBuffInfo->m_nHateAdj);
	}
}

void GBuffInstantApplier::GainBuffDamageAndHeal(GEntityActor* pEffecteeTarget, GBuff* pBuff, MUID uidUser, GEntityActor* pUser )
{
	VALID(pEffecteeTarget);
	VALID(pBuff);
	GBuffInfo* pBuffInfo = pBuff->GetInfo();
	VALID(pBuffInfo);

	GActorDamageCalculator		m_DamageCalculator;
	GCombatTurnResult combat_result = m_DamageCalculator.CalcBuffDamage(pUser, pEffecteeTarget, pBuffInfo);
	int nDamage = combat_result.nDamage * m_pOwner->GetStackCount();
	int nHealAmount = combat_result.nHealAmount * m_pOwner->GetStackCount();
	DAMAGE_ATTRIB nDamageAttrib = pBuffInfo->m_nDamageAttrib;

	if (!pEffecteeTarget->IsHittable(pBuffInfo)) 
		return;

	// �鿪 ó��
	//if (pBuff->GetInfo()->m_nType == BUFT_DEBUFF && // ������� ���������� �õ�
	//	pEffecteeTarget->HandleImmune(NULL, pBuffInfo))
	//	return;
	if (pBuffInfo->HasDamage() && // ������ �ִ� ������ ���� ����
		pEffecteeTarget->HandleImmune(NULL, pBuffInfo))
		return;

	// ���� ó��
	if (nDamage > 0)
	{
		pEffecteeTarget->doDamage(uidUser, nDamageAttrib, nDamage, EST_BUFF, pBuffInfo->m_nID);
	}

	// ������� ó��
	GMFApplyArgs mfApplyArgs(&pBuffInfo->m_MotionFactorGroup, pUser);
	GMFApplyReturnValue mfApplyRet = pEffecteeTarget->ApplyMotionFactor(mfApplyArgs);

	// ���� �ǰ� �̺�Ʈ ȣ��
	pEffecteeTarget->OnBuffHit(uidUser, pBuffInfo);

	// ��Ʈ ��Ŷ ����
	if (nDamage > 0 ||
		mfApplyRet.nMotionFactor != MF_FAKE_BEATEN && mfApplyRet.nMotionFactor != MF_NONE)
	{
		GTalentHitRouter talentHitRouter;
		talentHitRouter.RouteBuffHit(pEffecteeTarget, pBuffInfo->m_nID, nDamage, mfApplyRet.nMotionFactor, mfApplyRet.nWeight, combat_result.nResultFlags);
	}

	if (nHealAmount > 0)
	{
		pEffecteeTarget->doHeal(pUser?pUser->GetUID():MUID::Invalid(), 
								nHealAmount);
		BuffHealed(pEffecteeTarget, pBuffInfo, nHealAmount);

		GTalentRouter router;
		router.RouteBuffHeal(pUser, pEffecteeTarget, pBuffInfo->m_nID, nHealAmount);
	}

	// �̵� ó�� �и�
	pEffecteeTarget->DoScheduleMove();

	// ���� ó��
	if (pEffecteeTarget->IsDie()) 
	{
		pEffecteeTarget->SetKiller(pBuff->GetUserUID());
		if (pEffecteeTarget->IsPlayer())
		{
			ToEntityPlayer(pEffecteeTarget)->SetKillTalent(pBuff->GetUserTalentInfo());
		}

		pEffecteeTarget->doDie();
	}
}

void GBuffInstantApplier::BuffHealed(GEntityActor* pEffecteeTarget, GBuffInfo* pBuffInfo, int nHealHPAmount)
{
	pEffecteeTarget->GetActorHateTracker().AddHate_RecoverHP(nHealHPAmount, pBuffInfo->m_fHateFactor);
}

bool GBuffInstantApplier::IsNeedEffect()
{
	VALID_RET(m_pOwner, false);
	GBuffInfo* pBuffInfo = m_pOwner->GetInfo();
	VALID_RET(pBuffInfo, false);
	if (pBuffInfo->HasDamage())			return true;
	if (pBuffInfo->IsModified_MF())		return true;
	if (pBuffInfo->HasHealEffect())			return true;
	return false;
}

void GBuffInstantApplier::RouteGainInstantEffect(GEntityActor* pActor, GBuff* pBuff)
{
	VALID(pActor);
	VALID(pBuff);
	GBuffInfo* pBuffInfo = pBuff->GetInfo();
	VALID(pBuffInfo);		
	if (!pBuffInfo->IsExistModValue())	return;

	MCommand* pNewCommand = MakeNewCommand(MC_BUFF_INSTANT_EFFECT_GAIN, 2, 
		NEW_USHORT(pActor->GetUIID()), 
		NEW_INT(pBuffInfo->m_nID));

	pActor->RouteToThisCell(pNewCommand);
}