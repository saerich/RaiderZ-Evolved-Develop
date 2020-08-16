#include "StdAfx.h"
#include "GuardEffector.h"
#include "GTalentHelper.h"
#include "GCombatCalculator.h"
#include "GGlobal.h"
#include "GCommand.h"
#include "GModuleBuff.h"
#include "CCommandTable.h"
#include "GEntityPlayer.h"
#include "GEntityNPC.h"
#include "GCalculator.h"
#include "GGuardCalculator.h"
#include "GTestSystem.h"

GuardEffector::Result GuardEffector::CheckGuard(GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)
{
	VALID_RET(pGuarder, GUARD_FAILED);
	VALID_RET(pAttacker, GUARD_FAILED);
	VALID_RET(pAttackTalentInfo, GUARD_FAILED);

	if (!pGuarder->IsGuarding())
		return GUARD_FAILED;	// ���������� ����

	GTalentInfo* pGuardTalentInfo = GTalentHelper::FindTalentInfo(pGuarder->GetGuardTalentID(), pGuarder);
	if (!pGuardTalentInfo)
		return GUARD_FAILED;	// ���� �ŷ�Ʈ �������� ����

	if (!pGuardTalentInfo->IsGuard())
		return GUARD_FAILED;	// ���� �ŷ�Ʈ�� �ƴ�

	const int nGuardCrashLevel = pAttackTalentInfo->m_nGuardCrashLevel;
	if (nGuardCrashLevel > GetGuardLevel(pGuarder, pAttacker, pAttackTalentInfo))
		return GUARD_FAILED;	// ���� �� �� ���� ����

	if (!pGuarder->IsGuardableDirection(pAttacker))
		return GUARD_FAILED;	// ����� �� ���� ����

	return GUARD_SUCCESS;
}

GUARD_TYPE GuardEffector::GetGuardLevel( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo )
{
	VALID_RET(pGuarder, GT_INVALID);
	VALID_RET(pAttacker, GT_INVALID);
	VALID_RET(pAttackTalentInfo, GT_INVALID);

	GUARD_TYPE nGuardType = GT_INVALID;
	if (pGuarder->IsNPC())
	{
		GEntityNPC* pThisNPC = static_cast<GEntityNPC*>(pGuarder);
		VALID_RET(pThisNPC->GetNPCInfo(), GT_INVALID);
		nGuardType = pThisNPC->GetNPCInfo()->nDefaultGuardType;
	}
	else
	{
		if (pGuarder->IsEquipShield())	nGuardType = GT_PERFECT;
		else							nGuardType = GT_PARTIAL;
	}

	// ������ ���ɹ����� �ִ��� ����
	if (IsAbsoluteGuard(pGuarder))
	{
		nGuardType = GT_ABSOLUTE;
	}

	// ��� ���� ����
	switch(nGuardType)
	{
	case GT_PARTIAL:
		if (pAttackTalentInfo->m_bPerfectGuardOnly)
			return GT_INVALID;	// ������ �ƴϸ� ���з� ó��

		return GT_PARTIAL;	// �κй��� ó���Ϸ�

	case GT_PERFECT:
		if (pAttackTalentInfo->m_bPartialGuardHit)
			return GT_PARTIAL;	// ������� �����ϴ��� �κй��� ó��

		if (!CheckPerfectGuard(pGuarder, pAttacker))
			return GT_PARTIAL;	// ������ ���еǾ� �κй�� ó��

		return GT_PERFECT;

	case GT_ABSOLUTE:
		return GT_ABSOLUTE;
	}

	return GT_INVALID;
}

bool GuardEffector::IsTryGuard( GEntityActor* pGuarder )
{
	VALID_RET(pGuarder, false);

	return pGuarder->IsGuarding();
}

void GuardEffector::ApplyEffect_Guard( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo, GUARD_TYPE nGuardType, uint16 nCheckTime, int nDamage)
{
	VALID(pGuarder);
	VALID(pAttacker);
	VALID(pAttackTalentInfo);

	pGuarder->RouteActionCancelNextAttack(pGuarder->GetUIID(), CANCEL_NEXT_ATTACK_PARAM_ALL);

	int nGuardedDamage = CalcGuardedDamage(pGuarder, pAttacker, nGuardType, nDamage);
	RouteGuardDefense(pGuarder, nGuardType, pAttacker->GetUIID(), pAttackTalentInfo->m_nID, nCheckTime, nGuardedDamage);
}

void GuardEffector::ApplyEffect_PerfectGuard( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo, int nDamage )
{
	ApplyEffect_SpecificGuardImpl(pGuarder, pAttacker, pAttackTalentInfo, GT_PERFECT, nDamage);
}

void GuardEffector::ApplyEffect_PartialGuard( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo, int nDamage )
{
	ApplyEffect_SpecificGuardImpl(pGuarder, pAttacker, pAttackTalentInfo, GT_PARTIAL, nDamage);
}

void GuardEffector::ApplyEffect_SpecificGuardImpl( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo, GUARD_TYPE nGuardType, int nDamage )
{
	VALID(pGuarder);
	VALID(pAttacker);
	VALID(pAttackTalentInfo);

	float fReduce = CalcReducedDamage(pGuarder, pAttacker, nGuardType);
	gsys.pTestSystem->ShowGuardFormula(pGuarder, nDamage, fReduce, nGuardType);

	nDamage = CalcGuardedDamage(pGuarder, pAttacker, nGuardType, nDamage);
	pGuarder->doDamage(pAttacker->GetUID(), pAttackTalentInfo->m_nDamageAttrib, nDamage);

	// ���� ó��
	if (pGuarder->IsDie()) 
	{
		pGuarder->SetKiller(pAttacker->GetUID());
		if (pGuarder->IsPlayer())
		{
			ToEntityPlayer(pGuarder)->SetKillTalent(pAttackTalentInfo);
		}
		pGuarder->doDie();
	}
}


bool GuardEffector::CheckPerfectGuard(GEntityActor* pGuarder, GEntityActor* pAttacker)
{
	GPercentDice dice;
	return dice.Dice(gsys.pCombatCalculator->CalcPerfectGuardRate(pAttacker, pGuarder));
}

int GuardEffector::CalcGuardedDamage(GEntityActor* pGuarder, GEntityActor* pAttacker, GUARD_TYPE nGuardType, int nDamage)
{	
	return (int)(nDamage * CalcReducedDamage(pGuarder, pAttacker, nGuardType));
}

float GuardEffector::CalcReducedDamage(GEntityActor* pGuarder, GEntityActor* pAttacker, GUARD_TYPE nGuardType)
{
	int nLF = GCalculator::CalcLevelFactor(pGuarder->GetLevel(), pAttacker->GetLevel());

	switch (nGuardType)
	{
	case GT_PARTIAL:
		return GGuardCalculator::CalcPartialGuardDamageMod(pGuarder, pAttacker);
	case GT_PERFECT:
		return 0.0f;
	case GT_ABSOLUTE:
		return 0.0f;
	}

	_ASSERT(FALSE && L"ó������ ���� ���Ÿ��");
	return 0.0f;
}


void GuardEffector::RouteGuardDefense(GEntityActor* pGuarder, GUARD_TYPE nType, UIID nAttackerUIID, int nTalentID, uint32 nHitTime, int nGuardedDamage)
{
	VALID(pGuarder);

	if (nType == GT_PARTIAL)
	{
		TD_TALENT_PARTIAL_GUARD td_talent_guard;
		td_talent_guard.nType = nType;
		td_talent_guard.nAttackerUIID = nAttackerUIID;
		td_talent_guard.nGuardUIID = pGuarder->GetUIID();
		td_talent_guard.nTalentID = nTalentID;
		td_talent_guard.nHitTime = nHitTime;
		td_talent_guard.nGuardedDamage = nGuardedDamage;	
		td_talent_guard.vGuarderDir = pGuarder->GetDir();

		MCommand* pNewCommand = MakeNewCommand(MC_ACTION_GUARD_DEFENSE_PARTIAL, 1, NEW_SINGLE_BLOB( &td_talent_guard, sizeof( TD_TALENT_PARTIAL_GUARD)));
		pGuarder->RouteToThisCell(pNewCommand);
	}
	else
	{
		TD_TALENT_GUARD td_talent_guard;
		td_talent_guard.nType = nType;
		td_talent_guard.nAttackerUIID = nAttackerUIID;
		td_talent_guard.nGuardUIID = pGuarder->GetUIID();
		td_talent_guard.nTalentID = nTalentID;
		td_talent_guard.nHitTime = nHitTime;

		MCommand* pNewCommand = MakeNewCommand(MC_ACTION_GUARD_DEFENSE, 1, NEW_SINGLE_BLOB( &td_talent_guard, sizeof( TD_TALENT_GUARD)));
		pGuarder->RouteToThisCell(pNewCommand);
	}
}

bool GuardEffector::IsAbsoluteGuard( GEntityActor* pGuarder )
{
	if (!pGuarder->GetModuleBuff()->IsNowAbsoluteGuard())
		return false; // ������ ������ ����

	if (pGuarder->IsPlayer())
	{
		if (!pGuarder->IsEquipShield())
			return false; // ������� ���а� �ʿ���

		return true;
	}
	else if (pGuarder->IsNPC())
	{
		GEntityNPC* pThisNPC = static_cast<GEntityNPC*>(pGuarder);
		VALID_RET(pThisNPC->GetNPCInfo(), false);
		if (GT_PERFECT == pThisNPC->GetNPCInfo()->nDefaultGuardType)
			return true; // �Ϻ� ����� ���� ���� ��� ����

		return false;
	}
	
	return false;
}
