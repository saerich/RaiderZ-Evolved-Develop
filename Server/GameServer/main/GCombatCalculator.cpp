#include "stdafx.h"
#include "GCombatCalculator.h"
#include "GGlobal.h"
#include "GAttackDamageCalculator.h"
#include "GDefenseFactorCalculator.h"
#include "GStateFactorCalculator.h"
#include "GItemHolder.h"
#include "GDodgeCalculator.h"
#include "GResistCalculator.h"
#include "GCriticalCalculator.h"
#include "GGuardCalculator.h"
#include "GEquipmentSlot.h"

GCombatCalculator::GCombatCalculator()
{
}

int GCombatCalculator::CalcDamage( GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo, bool bCritical, float fResistFactor, const GDamageRangedInfo& DamageInfo, float* pfoutMotionFactor)
{
	// ���� ������
	float fAttackDamage = gsys.pAttackDamageCalculator->CalcAttackDamage(pAttacker, pVictim, pTalentInfo, bCritical, DamageInfo);

	// ������
	float fPenetrationFactor = CalcPenetrationFactor(pAttacker, pTalentInfo);

	// �Ƹӿ� ���� ������ ����ġ
	GDefenseFactorCalculator defenseFactorCalc;
	float fDefenseFactor = defenseFactorCalc.CalcDefenseFactor(pAttacker, pVictim, pTalentInfo);

	// ���� ������ ������ ���
	float fPenetrationDamage = fAttackDamage * fPenetrationFactor;
	float fDefensedDamage = (fAttackDamage - fPenetrationDamage) * fDefenseFactor;
	float fFinalAttackDamage = max(fPenetrationDamage + fDefensedDamage, 0.0f);



	// ���� ���� �˻�
	GStateFactorCalculator stateFactorCalc;
	bool isBackHit = false;
		
	if (pTalentInfo->IsPhysicalAttack())
	{
		// �޹��� �˻�� ���� ������ ��츸 �Ѵ�.
		isBackHit = pVictim->IsBack(pAttacker->GetPos());
	}

	// ������� ���
	MF_STATE mfState = pVictim->GetCurrentMotionFactor();

	float fStateFactor = stateFactorCalc.CalcStateFactor(mfState, isBackHit);
	float fMotionFactor = stateFactorCalc.CalcMotionFactor(mfState, isBackHit);

	if (pfoutMotionFactor != NULL)
	{
		*pfoutMotionFactor = fMotionFactor;
	}
	

	// ���� ������ ���
	int nFinalDamage = GMath::TruncateToInt(max(fFinalAttackDamage * fResistFactor * fStateFactor, 0));

	// TODO: Backstap

	return nFinalDamage;
}


int GCombatCalculator::CalcHealAmount( GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo, const GHealRangedInfo& infoHeal)
{
	int nHealAmount = 
		GMath::RandomNumber(infoHeal.nMin, infoHeal.nMax);

	return (int)((float)nHealAmount * pAttacker->GetSpellPower());
}

float GCombatCalculator::CalcPenetrationFactor(GEntityActor* pAttacker, const GTalentInfo* pTalentInfo)
{
	GEntityPlayer* pPlayer = ToEntityPlayer(pAttacker);

	// �÷��̾ ����Ѵ�.
	if (pPlayer == NULL)
	{
		return 0.0f;
	}


	// �������¿� ���� �⺻ �ɷ�ġ�� ���Ѵ�.
	ATTACK_TYPE_FOR_CALCULATE attackType = GAttackDamageCalculator::CalcAttackTypeForCalculate(pPlayer, pTalentInfo);

	int nBaseStat = 0;
	float fBuffBonus = 0.0f;

	switch(attackType)
	{
	case ATC_STR_WEAPON_ATTACK:		
		nBaseStat = pPlayer->GetSTR();
		fBuffBonus = pPlayer->GetChrStatus()->ActorModifier.fModMeleePene;
		break;
	case ATC_DEX_WEAPON_ATTACK:		
		nBaseStat = pPlayer->GetDEX();		
		fBuffBonus = pPlayer->GetChrStatus()->ActorModifier.fModRangePene;
		break;
	case ATC_MAGIC_ATTACK:			
		nBaseStat = pPlayer->GetINT();
		fBuffBonus = pPlayer->GetChrStatus()->ActorModifier.fModMagicPene;
		break;
	}	


	// ���� ����
	return min(max(nBaseStat - 150, 0) * 0.002f + fBuffBonus, 0.35f);
}

bool GCombatCalculator::IsFineDurationWeapon(GEntityActor* pAttacker, const GTalentInfo* pTalentInfo)
{
	GEntityPlayer* pPlayer = ToEntityPlayer(pAttacker);
	if (pPlayer == NULL)	return false;

	GItemHolder* pItemHolder = pPlayer->GetItemHolder();

	switch (pTalentInfo->m_WeaponReference)
	{
	case WR_LEFT:
		{
			GItem* pWeapon = pItemHolder->GetEquipment().GetLeftWeapon();

			if (pWeapon != NULL && !pWeapon->IsFineDuration())
			{
				return false;
			}
		}
		break;
	case WR_RIGHT:
		{
			GItem* pWeapon = pItemHolder->GetEquipment().GetRightWeapon();

			if (pWeapon != NULL && !pWeapon->IsFineDuration())
			{
				return false;
			}
		}
		break;
	case WR_BOTH:
		{
			GItem* pLeftWeapon = pItemHolder->GetEquipment().GetLeftWeapon();
			GItem* pRightWeapon = pItemHolder->GetEquipment().GetRightWeapon();

			if (pLeftWeapon != NULL && !pLeftWeapon->IsFineDuration() && 
				pRightWeapon != NULL && !pRightWeapon->IsFineDuration())
			{
				return false;
			}
		}
		break;
	default:
		return false;
	}

	return true;
}

bool GCombatCalculator::CheckMiss(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo)
{
	// ����, ���Ÿ�, ������ ����
	if (pTalentInfo->m_nCategory != TC_MELEE && 
		pTalentInfo->m_nCategory != TC_RANGE && 
		pTalentInfo->m_nCategory != TC_MAGIC) return false;

	GPercentDice dice;
	float fDodgePercent = GDodgeCalculator::CalcDodgePercent(pAttacker, pVictim, pTalentInfo);

	if (dice.Dice(fDodgePercent)) return true;

	return false;
}

bool GCombatCalculator::CheckRiposte(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo)
{
	return false;
}

bool GCombatCalculator::CheckResist(float& foutResistPercent, GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentResist& Resist)
{
	float fResistPercent = GResistCalculator::CalcResistPercent(pAttacker, pVictim, Resist);

	GPercentDice dice;
	if (dice.Dice(fResistPercent))
	{
		if (Resist.m_nMethod == TRM_PARTIAL)
		{
			foutResistPercent = fResistPercent;
		}
		return true;
	}

	return false;
}

bool GCombatCalculator::CheckCritical(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo)
{
	if (!pTalentInfo->m_bCriticalEnable) 
		return false;

	if (!IsFineDurationWeapon(pAttacker, pTalentInfo))
		return false;

	GPercentDice dice;
	GCriticalCalculator criticalCalculator;
	float fCriticalPercent = criticalCalculator.CalcCriticalPercent(pAttacker, pVictim, pTalentInfo);
	return dice.Dice(fCriticalPercent);
}

float GCombatCalculator::CalcPerfectGuardRate(GEntityActor* pAttacker, GEntityActor* pGuarder)
{
	return GGuardCalculator::CalcPerfectGuardRate(pAttacker, pGuarder);
}
