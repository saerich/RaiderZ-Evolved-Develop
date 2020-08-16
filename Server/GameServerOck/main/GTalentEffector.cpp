#include "stdafx.h"
#include "GTalentEffector.h"
#include "GTalentInfo.h"
#include "GModuleAI.h"	// for ��׷�
#include "GEntityPlayer.h"
#include "CCommandTable.h"	// for RoutePacket
#include "GTalentInfoMgr.h"
#include "GGlobal.h"
#include "GCommand.h"
#include "GScriptManager.h"
#include "GScriptGroup_NPC.h"
#include "GGlueNPC.h"
#include "GEntityNPC.h"
#include "GNPCSwallow.h"
#include "GNPCLoot.h"
#include "GTalentHitRouter.h"
#include "GActorHateTracker.h"
#include "GDeadMotionFactorImpactor.h"
#include "GItemDurabilityUpdater.h"
#include "GBuffInfo.h"
#include "GModuleBuff.h"
#include "GTalentRouter.h"

#include "GEntityActor.h"
#include "GConst.h"

void GTalentEffector::ApplyTalent(Info& info)
{
	VALID(info.pUser);
	VALID(info.pTalentInfo);	
	info.pField = info.pUser->GetField();

	{
		if (GConst::RUN_FOR_TEST &&
			!info.pField)
		{
			if (info.pUser->IsActor())
			{
				mlog3("effector: m_pField is null. (is_npc: %d, name = %s, uid = %I64d)\n", info.pUser->IsNPC(), MLocale::ConvUTF16ToAnsi(ToEntityActor(info.pUser)->GetName()).c_str(), info.pUser->GetUID())	;
			}

		}
	}

	VALID(info.pField);	



	GTalentInfo* pModifiedTalentInfo = ModifyBackHitTalentInfo(info.pTalentInfo, info.pTarget);
	bool bTalentInfoModified = pModifiedTalentInfo != info.pTalentInfo;

	// �ŷ�Ʈ�� ����Ҷ��� ��� ȿ���� ��´�.
	if (info.nTalentCondition == pModifiedTalentInfo->m_nTiming)
	{
		PFI_B(9016, "GTalentEffector::ApplyTalent-GEffectTargetSelector::Select");
		GEffectTargetSelector::Desc desc(pModifiedTalentInfo->m_EffectInfo);
		desc.pSelf = info.pUser;
		desc.pTarget = info.pTarget;
		desc.vCenterPoint = info.vGroundPoint;
		set<GEntityActor*> qEffecteeList;
		m_EffectTargetSelector.Select(desc, qEffecteeList);
		PFI_E(9016);

		for each (GEntityActor* pActor in qEffecteeList)
		{
			PFI_B(9017, "GTalentEffector::ApplyTalent-GTalentEffector::GainTalentEffect");
			GainTalentEffect(info.pUser, pModifiedTalentInfo, pActor, info.bIgnoreMF, bTalentInfoModified);
			PFI_E(9017);
		}	
	}

	PFI_B(9018, "GTalentEffector::ApplyTalent-GTalentEffector::GainTalentBuff");
	GainTalentBuff(info, pModifiedTalentInfo);
	PFI_E(9018);
}

bool GTalentEffector::GainTalentEffect(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pTarget, bool bIgnoreMF, bool bTalentInfoModified)
{
	VALID_RET(pUser, false);
	VALID_RET(pTarget, false);	

	if (!pTalentInfo->HasExtraActiveEvent())
	{
		// ExtraActive Event�� ���� ���� ��� ȿ���� ����		
		m_ExtraActiveTalentRunner.Apply(pUser, vec3::ZERO, pTalentInfo, pTarget);
	}
	
	if (pTarget->IsDead())
		return false;	// ���� ��ǥ���Դ� ȿ�� ����

	bool bRet = true;
	if (IsNeedEffect(pTalentInfo))
	{
		bRet = 
			GainDamageAndHeal(  pUser, 
								pTarget, 
								pTalentInfo, 
								GDamageRangedInfo(pTalentInfo->m_nMaxDamage, pTalentInfo->m_nMinDamage), 
								GHealRangedInfo(pTalentInfo->m_nMaxHeal, pTalentInfo->m_nMinHeal), 
								bIgnoreMF, 
								bTalentInfoModified);
	}
	
	// ������̾�
	if (pTalentInfo->IsPassiveSkill())
	{
		m_ModEffector.AddActorModifier(pTarget->GetChrStatus()->ActorModifier, pTalentInfo->m_ActorModifier);

		if (pTarget->IsPlayer())
		{
			GEntityPlayer* pTargetPlayer = ToEntityPlayer(pTarget);
			m_ModEffector.AddPlayerModifier(pTargetPlayer->GetPlayerModifier(), pTalentInfo->m_PlayerModifier);
		}

		m_ModEffector.RefreshStatus(pTarget);
	}
	else
	{
		int nBeforeHP = pTarget->GetHP();
		m_ModEffector.ApplyInstantModifier(pTarget, pTalentInfo->m_InstantModifier);
		if (IsInstantEffect(pTalentInfo))
		{
			RouteGainInstantEffect(pTarget, pTalentInfo);
		}
		
		int nHealHPAmount = pTarget->GetHP() - nBeforeHP;
		if (nHealHPAmount > 0)
		{
			OnHealed(pTarget, pTalentInfo, nHealHPAmount);
		}
	}	

	// ������ Ưȭ ó��
	if (pTalentInfo->m_strSpecialization == TALENT_PROPERTY_SAZAHU &&
		pUser->IsPlayer())
	{
		int nSazahuTalentID = static_cast<GEntityPlayer*>(pUser)->GetPassiveValue(TEPT_TALENT_SAZAHU_SPECIALIZATION);
		GTalentInfo* pSazahuTalent = gmgr.pTalentInfoMgr->Find(nSazahuTalentID);
		if (pSazahuTalent)
			m_ExtraActiveTalentRunner.Apply(pUser, vec3::ZERO, pSazahuTalent, pTarget);
	}

	return bRet;
}

GTalentInfo* GTalentEffector::ModifyBackHitTalentInfo(GTalentInfo* pTalentInfo, GEntityActor* pTarget)
{
	if (NULL == pTalentInfo) return pTalentInfo;
	if (NULL == pTarget) return pTalentInfo;

	if (0 == pTalentInfo->m_nBackHitTalent) return pTalentInfo;

	GHitInfo hitInfo = pTarget->GetLastHitInfo();	
	GTalentInfo* pBackHItTalentInfo = gmgr.pTalentInfoMgr->Find(pTalentInfo->m_nBackHitTalent);	

	if (pTalentInfo != hitInfo.pTalentInfo || false == hitInfo.bBackHit || NULL == pBackHItTalentInfo)
	{
		return pTalentInfo;
	}

	return pBackHItTalentInfo;	
}

void GTalentEffector::LostTalentEffect(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pTarget)
{
	VALID(pUser);
	VALID(pTarget);

	// ������̾�
	if (pTalentInfo->IsPassiveSkill())
	{
		m_ModEffector.EraseActorModifier(pTarget->GetChrStatus()->ActorModifier, pTalentInfo->m_ActorModifier);

		if (pTarget->IsPlayer())
		{
			GEntityPlayer* pPlayer = ToEntityPlayer(pTarget);
			m_ModEffector.ErasePlayerModifier(pPlayer->GetPlayerModifier(), pTalentInfo->m_PlayerModifier);
		}

		m_ModEffector.RefreshStatus(pTarget);
	}
}

bool GTalentEffector::GainDamageAndHeal(GEntityActor* pUser, GEntityActor* pTarget, GTalentInfo* pTalentInfo, GDamageRangedInfo infoDamage, GHealRangedInfo infoHeal, bool bIgnoreMF, bool bNeedDamageRecalc, float fDamageApplyRate)
{
	PFI_BLOCK_THISFUNC(9022);

	VALID_RET(pTarget, false);

	PFI_B(9028, "GEntityActor::IsHittable");
	if (!pTarget->IsHittable(pUser, pTalentInfo)) 	
		return false;
	PFI_E(9028);

	// �鿪 ó��
	PFI_B(9029, "GEntityActor::HandleImmune");
	if (pTarget->HandleImmune(pUser, pTalentInfo))	
		return false;
	PFI_E(9029);

	GHitInfo infoHit = pTarget->GetLastHitInfo();
	DAMAGE_ATTRIB nDamageAttrib = DA_NONE;
	MOTION_FACTOR_GROUP mf = pTalentInfo->m_MotionFactorGroup;

	// ���º��� ������� ����
	float fStateMotionFactor = 1.0f;
	nDamageAttrib = pUser->GetDamageType(pTalentInfo);
	infoHit.nDamageType = nDamageAttrib;

	// ������ ��Ʈ�� ����� ���ų� ������ ������ �ʿ��� ���
	if (false == infoHit.IsValidInfo() || true == bNeedDamageRecalc)
	{
		PFI_B(9030, "GActorDamageCalculator::CalcDamage");
		GActorDamageCalculator::Desc desc;
		desc.pAttacker = pUser;
		desc.pVictim = pTarget;
		desc.pTalentInfo = pTalentInfo;
		desc.DamageInfo = infoDamage;
		desc.HealInfo = infoHeal;
		desc.nHitCapGroup = infoHit.nCapsuleGroup;
		desc.nHitCapIndex = infoHit.nCapsuleIndex;
		GCombatTurnResult combat_result = 
			m_DamageCalculator.CalcDamage(desc, &fStateMotionFactor);
		PFI_E(9030);

		infoHit.nDamage = combat_result.nDamage;
		infoHit.nHealAmount = combat_result.nHealAmount;
		infoHit.nCombatResultFlags = combat_result.nResultFlags;
		infoHit.pTalentInfo			= pTalentInfo;		
	}	
	else
	{
		bool bGuarded = infoHit.bGuarded;
		pTarget->ClearLastHitInfo(); // ��Ʈ���� ����

		if (bGuarded)
			return false; // �����ߴٸ� ����
	}

	infoHit.nDamage = static_cast<int32>(infoHit.nDamage * fDamageApplyRate);

	bool bRunCallback = false;

	if (pTarget->IsNPC())
	{
		GEntityNPC* pNPC = ToEntityNPC(pTarget);
		infoHit.pMotionFactorGroup = &mf;
		infoHit.pAttacker = pUser;
		infoHit.pVictim = pNPC;

		PFI_B(9031, "GTalentEffector::GainDamageAndHeal-CallBack");

		// �Ա� ���� �ݹ�
		if (pNPC->GetNPCSwallow().IsSwallowing())
		{
			bRunCallback = true;
			GetScriptNPC()->OnHitSwallowed(pNPC->GetDelegator(), infoHit);
			nDamageAttrib = (DAMAGE_ATTRIB)infoHit.nDamageType;
		}
		
		// �κ� ���� �ݹ�
		if (infoHit.nCapsuleGroup >=0 &&
			infoHit.nCapsuleIndex >=0)
		{
			bRunCallback = true;
			GetScriptNPC()->OnHitCapsule(pNPC->GetDelegator(), infoHit);
			nDamageAttrib = (DAMAGE_ATTRIB)infoHit.nDamageType;
		}

		PFI_E(9031);
	}
	
	if (!bRunCallback)
	{
		// �ݹ��� ������� ���� ��쿡�� �⺻���� ����
		infoHit.pMotionFactorGroup = &pTalentInfo->m_MotionFactorGroup;
	}

	PFI_B(9032, "GTalentEffector::GainDamageAndHeal-Resist");

	// ���� ����
	if (CheckBitSet(infoHit.nCombatResultFlags, CTR_RESIST_PERFECT))
	{
		// ���� ���׵� ��Ŷ ����
		GTalentHitRouter talentHitRouter;
		talentHitRouter.Route_Resist(pUser, pTarget, pTalentInfo, infoHit.nCheckTime, infoHit.nCombatResultFlags);

		return false;
	}

	PFI_E(9032);

	PFI_B(9033, "GTalentEffector::GainDamageAndHeal-MotionFactor");

	// ������� ó��
	GMFApplyReturnValue mfApplyRet(MF_NONE, 0);

	if (!bIgnoreMF)
	{
		GMFApplyArgs mfApplyArgs;
		mfApplyArgs.pMotionFactorGroup = infoHit.pMotionFactorGroup;
		mfApplyArgs.pOpponent = pUser;
		mfApplyArgs.nNotApplyMF = MF_NONE;
		mfApplyArgs.nForceMF = pTalentInfo->m_nForceMF;
		mfApplyArgs.nForceMFWeight = pTalentInfo->m_nForceMFWeight;
		mfApplyArgs.bKnockbackDirSyncToOpponentDir = pTalentInfo->IsKnockbackDirSyncToAttackerDir();	// �̵��ϴ� �ŷ�Ʈ�� �˹�� ������ �������� �˹�ǵ��� ������
		mfApplyArgs.fStateMotionFactor = fStateMotionFactor;											// ���º����� ���� �������

		// �׾��� ������ ����Ʈ �ִ� ȿ���� ���� ������ �˹����� �״´�.
		GDeadMotionFactorImpactor deadMotionFactorImpactor;
		deadMotionFactorImpactor.CheckUp(mfApplyArgs, pTarget, pTalentInfo, infoHit.nDamage);

		// drag�� �� Weight�� ���� ���
		if (mfApplyArgs.nForceMF == MF_DRAG)
		{
			float fActorDistance = pUser->GetPos().DistanceTo(pTarget->GetPos());
			float fOpponentRadius = pUser->GetColRadius();
			float fVictimRadius = pTarget->GetColRadius();

			float fDistance;
			if (fActorDistance > fVictimRadius + fOpponentRadius)
			{
				fDistance = fActorDistance - (fVictimRadius + fOpponentRadius);
			}
			else
			{
				fDistance = 0;
			}
			
			mfApplyArgs.nForceMFWeight = (int)fDistance;
		}


		mfApplyRet = pTarget->ApplyMotionFactor(mfApplyArgs);
	}

	PFI_E(9033);

	PFI_B(9034, "GTalentEffector::GainDamageAndHeal-Heal");

	// ȸ�� ó��
	int nHealAmount=infoHit.nHealAmount;
	if (nHealAmount)
	{
		pTarget->doHeal(pUser->GetUID(), nHealAmount);

		GTalentRouter router;
		router.RouteTalentHeal(pUser, pTarget, pTalentInfo->m_nID, nHealAmount);

		OnHealed(pTarget, pTalentInfo, nHealAmount);
	}

	PFI_E(9034);

	PFI_B(9035, "GTalentEffector::GainDamageAndHeal-Damage");

	// ���� ó��
	int nDamage=infoHit.nDamage;
	if (nDamage > 0)
	{
		pTarget->doDamage(pUser->GetUID(), nDamageAttrib, nDamage, EST_TALENT, pTalentInfo->m_nID);
	}

	PFI_E(9035);


	PFI_B(9036, "GTalentEffector::GainDamageAndHeal-HitPackit");

	// ��Ʈ ��Ŷ ����
	if (pTalentInfo->HasAttack())
	{
		GTalentHitRouter talentHitRouter;
		talentHitRouter.RouteTalentHit(pUser, pTarget, pTalentInfo, nDamage, mfApplyRet.nMotionFactor, mfApplyRet.nWeight, infoHit.nCheckTime, infoHit.nCombatResultFlags);
	}

	PFI_E(9036);

	PFI_B(9037, "GTalentEffector::GainDamageAndHeal-Attacker");
	
	// ������ ó��
	if (infoHit.pAttacker)
	{
		if (mfApplyRet.nMotionFactor == MF_UPPERED)
		{
			pTarget->RouteActionCancelNextAttack(infoHit.pAttacker->GetUIID(), CANCEL_NEXT_ATTACK_PARAM_UPPERED);
		}
	}

	PFI_E(9037);

	PFI_B(9038, "GTalentEffector::GainDamageAndHeal-Die");

	// ���� ó��
	if (pTarget->IsDie()) 
	{
		pTarget->SetKiller(pUser->GetUID());
		if (pTarget->IsPlayer())
		{
			ToEntityPlayer(pTarget)->SetKillTalent(pTalentInfo);
		}
		pTarget->doDie();
	}

	PFI_E(9038);

	PFI_B(9039, "GTalentEffector::GainDamageAndHeal-Durability");

	gsys.pItemSystem->GetDurabilityUpdater().LostDurabilityByCombat(pUser, pTarget, infoHit.bCriticalHit, infoHit.bTryGuard);

	PFI_E(9039);

	return true;
}

void GTalentEffector::GainTalentBuff(const Info& info, GTalentInfo* pTalentInfo)
{
	GainSingleBuff(info, pTalentInfo, pTalentInfo->m_Buff1);
	GainSingleBuff(info, pTalentInfo, pTalentInfo->m_Buff2);
}

void GTalentEffector::GainSingleBuff( const Info& info, GTalentInfo* pTalentInfo, const CSBuffEnchantInfo& BuffInvokeInfo )
{
	VALID(info.pUser);
	VALID(pTalentInfo);

	if (BuffInvokeInfo.nID == INVALID_BUFF_ID)				return;
	if (BuffInvokeInfo.nEnchantEvent != info.nTalentCondition)	return;
	
	GBuffInfo* pBuffInfo = gmgr.pBuffInfoMgr->Get(BuffInvokeInfo.nID);
	_ASSERT(pBuffInfo);
	if (!pBuffInfo)									return;

	// ��ġ�� �Ӽ��� �ŷ�Ʈ�� ���, �нú� ȿ�� �߰�
	float fAddEffecteeRadius = 0.0f;	
	if (pTalentInfo->m_strSpecialization == TALENT_PROPERTY_SHOUT && info.pUser->IsPlayer())
	{
		fAddEffecteeRadius = (float)ToEntityPlayer(info.pUser)->GetPassiveValue(TEPT_TALENT_SHOUT_RADIUS)*100;
	}

	GEffectTargetSelector::Desc desc(BuffInvokeInfo.EffectInfo);
	desc.pSelf = info.pUser;
	desc.pTarget = info.pTarget;
	desc.fAddEffectRadius = fAddEffecteeRadius;
	desc.vCenterPoint = info.vGroundPoint;
	set<GEntityActor*> qEffecteeList;
	m_EffectTargetSelector.Select(desc, qEffecteeList);

	for each (GEntityActor* pActor in qEffecteeList)
	{
		pActor->GetModuleBuff()->GainBuff(pBuffInfo, BuffInvokeInfo, pTalentInfo, info.pUser);
	}
}

bool GTalentEffector::IsInstantEffect( GTalentInfo* pTalentInfo )
{
	VALID_RET(pTalentInfo, false);		

	if (IsNeedEffect(pTalentInfo))
		return false; // ���ذ� ������ �ν���Ʈ ����Ʈ�� �ƴ�

	if (pTalentInfo->m_nHateAdj != 0)
		return true; // ����Ʈ�� �ٲ�

	if (pTalentInfo->IsExistModValue())
		return true; // ������̾ �ٲ�

	return false;

}

void GTalentEffector::RouteGainInstantEffect( GEntityActor* pUser, GTalentInfo* pTalentInfo )
{
	VALID(pUser);
	VALID(pTalentInfo);		

	MCommand* pNewCommand = MakeNewCommand(MC_TALENT_INSTANT_EFFECT_GAIN, 2, 
			NEW_USHORT(pUser->GetUIID()), 
			NEW_INT(pTalentInfo->m_nID));

	pUser->RouteToThisCell(pNewCommand);
}

bool GTalentEffector::IsNeedEffect( GTalentInfo* pTalentInfo )
{
	VALID_RET(pTalentInfo, false);

	if (pTalentInfo->HasDamage())			return true;
	if (pTalentInfo->HasMotionfactor())		return true;
	if (pTalentInfo->HasHealEffect())		return true;
	if (pTalentInfo->HasHeal())				return true;
	if (pTalentInfo->HasBuff())				return true;
	return false;
}

void GTalentEffector::OnHealed(GEntityActor* pTarget, GTalentInfo* pTalentInfo, int nHealHPAmount)
{
	PFI_BLOCK_THISFUNC(9023);
	pTarget->GetActorHateTracker().AddHate_RecoverHP(nHealHPAmount, pTalentInfo->m_fHateFactor);
}
