#include "stdafx.h"
#include "GAttackDamageCalculator.h"
#include "GCombatCalculator.h"
#include "GCriticalCalculator.h"
#include "GItemHolder.h"
#include "GConst.h"
#include "GEquipmentSlot.h"
#include "GGlobal.h"
#include "GItemEquiper.h"

/// GAttackDamageCalculator
///////////////////////////////////////
float GAttackDamageCalculator::CalcAttackDamage( GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo, bool bCritical, const GDamageRangedInfo& DamageInfo )
{
	int nTalentDamage = 0;			// �ŷ�Ʈ ������
	int nWeaponDamage = 0;			// ���� ������
	float fWeaponApplyRate = pTalentInfo->m_fWeaponApplyRate;		// ���� ������ ������

	float fTalentFactor1 = 1.0f;	// �ŷ�Ʈ ������1
	float fTalentFactor2 = 1.0f;	// �ŷ�Ʈ ������267

	float fArmorFactor1 = 1.0f;		// �� ������1
	float fArmorFactor2 = 1.0f;		// �� ������2

	float fCharacterFactor = 1.0f;	// ���� ������
	float fBuffPlusFactor = 0.0f;		// ������ ���� ������
	float fBuffDefenceFactor = 1.0f;	// ������ ���� �����
	float fCriticalFactor = 1.0f;	// ũ��Ƽ�� ����

	float fSpellPower = 1.0f;		// �ֹ� ������


	if (pAttacker->IsPlayer())
	{
		GEntityPlayer* pPlayerAttacker = static_cast< GEntityPlayer*>(pAttacker);

		// ���� ������
		nWeaponDamage = CalcPlayerWeaponDamage(pPlayerAttacker, pTalentInfo, bCritical);

		// ���� ������ ���
		if (pTalentInfo->IsMagicDamage())
		{
			// �ŷ�Ʈ ������1
			fTalentFactor1 = CalcTalentFactor1(pAttacker, pTalentInfo->m_nDamageAttrib, pTalentInfo->m_fWeaponApplyRate);

			// �� ������1
			fArmorFactor1 = CalcArmorFactor1(pAttacker, pTalentInfo);

			// �ֹ� ������
			fSpellPower = pPlayerAttacker->GetSpellPower();
		}
		else
		{
			// �ŷ�Ʈ ������2
			fTalentFactor2 = CalcTalentFactor2(pPlayerAttacker, pTalentInfo);

			fArmorFactor2 = CalcArmorFactor2(pAttacker, pTalentInfo);
		}

		// ���� ������
		fCharacterFactor = CalcCharacterFactor(pPlayerAttacker, pTalentInfo);
	}
	else if (pAttacker->IsNPC())
	{
		GEntityNPC* pNPCAttacker = static_cast< GEntityNPC*>(pAttacker);

		// ���� ������
		nWeaponDamage = CalcNPCWeaponDamage(pNPCAttacker->GetNPCInfo());

		// ���� ������ ���
		if (pTalentInfo->IsMagicDamage())
		{
			// �ֹ� ������
			fSpellPower = pNPCAttacker->GetSpellPower();
		}
	}


	// ����
	fBuffPlusFactor = CalcBuffPlusFactor(pAttacker, pTalentInfo);

	// ���� �����
	fBuffDefenceFactor = CalcBuffDefenceFactor(pAttacker, pVictim, pTalentInfo);


	// ũ��Ƽ�� ����
	if (bCritical)
	{
		fCriticalFactor = m_pCriticalCalculator->CalcCriticalDamageFactor(pAttacker, pTalentInfo);
	}

	nTalentDamage = CalcTalentDamage(DamageInfo, bCritical);

	_ASSERT(fTalentFactor1 < 10.0f);
	_ASSERT(fArmorFactor1 < 10.0f);
	_ASSERT(fTalentFactor2 < 10.0f);
	_ASSERT(fArmorFactor2 < 10.0f);
	_ASSERT(fWeaponApplyRate < 10.0f);
	
	float fFinalDamage = GMath::TruncateToInt(nTalentDamage * fTalentFactor1 * fArmorFactor1 + 
											  nWeaponDamage * fWeaponApplyRate * fTalentFactor2 * fArmorFactor2) 
						 * (fCharacterFactor + fBuffPlusFactor) 
						 * fCriticalFactor
						 * fBuffDefenceFactor
						 * fSpellPower;

	return fFinalDamage;
}

int GAttackDamageCalculator::MakeDamageFromMinMaxDamage( const GDamageRangedInfo& DamageInfo )
{
	return (GMath::RandomNumber(DamageInfo.nMinDamage, DamageInfo.nMaxDamage));
}

float GAttackDamageCalculator::CalcCharacterFactor( GEntityPlayer* pAttacker, const GTalentInfo* pTalentInfo )
{
	ATTACK_TYPE_FOR_CALCULATE attackType = CalcAttackTypeForCalculate(pAttacker, pTalentInfo);

	switch(attackType)	
	{
	case ATC_STR_WEAPON_ATTACK:		return CalcCharacterFactorForMelee(pAttacker);
	case ATC_DEX_WEAPON_ATTACK:		return CalcCharacterFactorForRange(pAttacker);
	case ATC_MAGIC_ATTACK:			return CalcCharacterFactorForMagic(pAttacker);
	};

	return CalcCharacterFactorForMelee(pAttacker);
}

float GAttackDamageCalculator::CalcBuffPlusFactor( GEntityActor* pAttacker, const GTalentInfo* pTalentInfo )
{
	float fBuffFactor = 0.0f;

	if (pTalentInfo->m_nCategory == TC_MELEE)
	{
		fBuffFactor += pAttacker->GetChrStatus()->ActorModifier.fMeleeDamageAmp;
	}
	else if (pTalentInfo->m_nCategory == TC_RANGE)
	{
		fBuffFactor += pAttacker->GetChrStatus()->ActorModifier.fRangeDamageAmp;
	}
	else if (pTalentInfo->m_nCategory == TC_MAGIC)
	{
		fBuffFactor += pAttacker->GetChrStatus()->ActorModifier.fMagicDamageAmp;
	}

	return fBuffFactor;
}

int GAttackDamageCalculator::CalcPlayerWeaponDamage(GEntityPlayer* pAttacker, const GTalentInfo* pTalentInfo, bool bCritical)
{
	if (pTalentInfo->m_nSkillType != ST_MELEE &&
		pTalentInfo->m_nSkillType != ST_ARCHERY ) return 0;

	int nDamage = 0;
	GItemHolder* pItemHolder = pAttacker->GetItemHolder();

	switch (pTalentInfo->m_WeaponReference)
	{
	case WR_LEFT:
		{
			const GItem* pLeftItem = pItemHolder->GetEquipment().GetLeftWeapon();
			if (pLeftItem)
			{
				nDamage = doCalcWeaponDamage(pAttacker, pLeftItem, bCritical);
			}
			else
			{
				nDamage = GConst::NO_WEAPON_DAMAGE_LEFT;
			}
		}
		break;
	case WR_RIGHT:
		{
			const GItem* pRightItem = pItemHolder->GetEquipment().GetRightWeapon();
			if (pRightItem)
			{
				nDamage = doCalcWeaponDamage(pAttacker, pRightItem, bCritical);
			}
			else
			{
				nDamage = GConst::NO_WEAPON_DAMAGE_RIGHT;	
			}
		}
		break;
	case WR_BOTH:
		{
			const GItem* pLeftItem = pItemHolder->GetEquipment().GetLeftWeapon();
			const GItem* pRightItem = pItemHolder->GetEquipment().GetRightWeapon();
			nDamage = doCalcWeaponDamage(pAttacker, pLeftItem, pRightItem, bCritical);

			if (pLeftItem == NULL)
			{
				nDamage += GConst::NO_WEAPON_DAMAGE_LEFT;
			}
			if (pRightItem == NULL)
			{
				nDamage += GConst::NO_WEAPON_DAMAGE_RIGHT;
			}
		}
		break;
	default:
		{
			_ASSERT(0);
		}
	}

	return nDamage;
}

int GAttackDamageCalculator::doCalcWeaponDamage(GEntityActor* pActor, const GItem* pWeaponItem, bool bCritical)
{
	if (!pWeaponItem)
		return 0;

	GItemData* pItemData = pWeaponItem->m_pItemData;
	VALID_RET(pItemData, 0);

	int nWeaponDamage = bCritical?  pItemData->m_nMaxDamage : 
									GMath::RandomNumber(pItemData->m_nMinDamage, pItemData->m_nMaxDamage);

	if (!pWeaponItem->IsFineDuration() || !HasWeaponPassiveTalent(pActor, pWeaponItem))
	{
		return static_cast<int>(nWeaponDamage * 0.2f);
	}	

	return nWeaponDamage;
}

int GAttackDamageCalculator::doCalcWeaponDamage(GEntityActor* pActor, const GItem* pLeftWeaponItem, const GItem* pRightWeaponItem, bool bCritical )
{
	int nLeftDamage = doCalcWeaponDamage(pActor, pLeftWeaponItem, bCritical);
	int nRightDamage = doCalcWeaponDamage(pActor, pRightWeaponItem, bCritical);

	int nDamage = static_cast<int>((nLeftDamage + nRightDamage) * 0.5f);

	return nDamage;
}

float GAttackDamageCalculator::CalcTalentFactor1( GEntityActor* pAttacker, DAMAGE_ATTRIB nDamageAttrib, float fWeaponApplyRatepTalentInfo )
{
	const float fFactorDefault = 1.0f;

	if (!pAttacker)
		return fFactorDefault;

	if (!pAttacker->IsPlayer())
		return fFactorDefault;

	GEntityPlayer* pPlayerAttacker = static_cast< GEntityPlayer*>(pAttacker);
	return CalcPlayerTalentFactor1(pPlayerAttacker, nDamageAttrib, fWeaponApplyRatepTalentInfo);
}

float GAttackDamageCalculator::CalcPlayerTalentFactor1( GEntityPlayer* pPlayerAttacker, DAMAGE_ATTRIB nDamageAttrib, float fWeaponApplyRate )
{
	float fFactor = 1.0f;

	// �ŷ�Ʈ ������1�� ���� ������ �����Ͽ� �����Ѵ�.
	if (fWeaponApplyRate < 0.001f)
	{
		switch (nDamageAttrib)
		{
		case DA_FIRE:
		case DA_LIGHTNING:
		case DA_COLD:
		case DA_POISON:
		case DA_UNHOLY:
			{
				// Factor1_talent = 1 + ���� ���� ��ũ * 0.04 + ���ݸ��� Ưȭ * 0.06
				int nBaseRank = pPlayerAttacker->GetPassiveRank(TEPT_BASE_ELEMENTAL);
				int nSpecialization = pPlayerAttacker->GetPassiveRank(TEPT_SPECIALIZATION_ELEMENTAL);

				fFactor = 1.0f + (nBaseRank * 0.04f) + (nSpecialization * 0.06f);
			}
			break;
		case DA_HOLY:
			{
				// Factor1_talent = 1 + �ż� ���� ��ũ * 0.04 + �ż� Ưȭ * 0.06
				int nBaseRank = pPlayerAttacker->GetPassiveRank(TEPT_BASE_HOLY);
				int nSpecialization = pPlayerAttacker->GetPassiveRank(TEPT_SPECIALIZATION_HOLY);

				fFactor = 1.0f + (nBaseRank * 0.04f) + (nSpecialization * 0.06f);
			}
			break;
		}
	}
	return fFactor;
}

float GAttackDamageCalculator::CalcTalentFactor2( GEntityPlayer* pAttacker, const GTalentInfo* pTalentInfo )
{
	float fCalcTalentFactor2 = 0.0f;
	GItemHolder* pItemHolder = pAttacker->GetItemHolder();

	if (pTalentInfo->m_nCategory == TC_MAGIC)
	{
		fCalcTalentFactor2 = 1.0f;
	}
	else
	{
		switch (pTalentInfo->m_WeaponReference) 
		{
		case WR_LEFT:
			{
				WEAPON_TYPE nLeftWeaponType = pItemHolder->GetEquipment().GetLeftWeaponType();
				fCalcTalentFactor2 = CalcTalentFactor2_NotMagic(pAttacker, nLeftWeaponType);
			}
			break;
		case WR_RIGHT:
			{
				WEAPON_TYPE nRightWeaponType = pItemHolder->GetEquipment().GetRightWeaponType();
				fCalcTalentFactor2 = CalcTalentFactor2_NotMagic(pAttacker, nRightWeaponType);
			}
			break;
		case WR_BOTH:
			{
				WEAPON_TYPE nLeftWeaponType = pItemHolder->GetEquipment().GetLeftWeaponType();
				WEAPON_TYPE nRightWeaponType = pItemHolder->GetEquipment().GetRightWeaponType();
				fCalcTalentFactor2 = CalcTalentFactor2_NotMagic(pAttacker, nLeftWeaponType, nRightWeaponType);
			}
			break;
		default:
			{
				_ASSERT(0);
			}
		}		
	}

	return fCalcTalentFactor2;
}

float GAttackDamageCalculator::CalcTalentFactor2_NotMagic(GEntityPlayer* pAttacker, WEAPON_TYPE nWeaponType)
{
	int nBaseRank = 0;
	int nSpecializationRank = 0;

	switch (nWeaponType)
	{
	case WEAPON_1H_SLASH:
		{
			nBaseRank = pAttacker->GetPassiveRank(TEPT_BASE_WEAPON_ONE_HANDED);
			nSpecializationRank = pAttacker->GetPassiveRank(TEPT_SPECIALIZATION_SLASH);
		}
		break;
	case WEAPON_1H_BLUNT:
		{
			nBaseRank = pAttacker->GetPassiveRank(TEPT_BASE_WEAPON_ONE_HANDED);
			nSpecializationRank = pAttacker->GetPassiveRank(TEPT_SPECIALIZATION_BLUNT);
		}
		break;
	case WEAPON_1H_PIERCE:
		{
			nBaseRank = pAttacker->GetPassiveRank(TEPT_BASE_WEAPON_1H_PIERCE);
			nSpecializationRank = pAttacker->GetPassiveRank(TEPT_SPECIALIZATION_PIERCE);
		}
		break;
	case WEAPON_TWO_HANDED:
		{
			nBaseRank = pAttacker->GetPassiveRank(TEPT_BASE_WEAPON_TWO_HANDED);
			nSpecializationRank = pAttacker->GetPassiveRank(TEPT_SPECIALIZATION_SLASH);
		}
		break;
	case WEAPON_2H_BLUNT:
		{
			nBaseRank = pAttacker->GetPassiveRank(TEPT_BASE_WEAPON_2H_BLUNT);
			nSpecializationRank = pAttacker->GetPassiveRank(TEPT_SPECIALIZATION_BLUNT);
		}
		break;
	case WEAPON_ARCHERY:
		{
			nBaseRank = pAttacker->GetPassiveRank(TEPT_BASE_WEAPON_ARCHERY);
			nSpecializationRank = pAttacker->GetPassiveRank(TEPT_SPECIALIZATION_PIERCE);
		}
		break;
	case WEAPON_DUAL_PIERCE:
		{
			nBaseRank = pAttacker->GetPassiveRank(TEPT_BASE_WEAPON_1H_PIERCE);
			nSpecializationRank = pAttacker->GetPassiveRank(TEPT_SPECIALIZATION_PIERCE);
		}
		break;
	}

	return ClacTalentFactor2Fomula(nBaseRank, nSpecializationRank);
}

float GAttackDamageCalculator::CalcTalentFactor2_NotMagic(GEntityPlayer* pAttacker, WEAPON_TYPE nLeftWeaponType, WEAPON_TYPE nRightWeaponType)
{
	float fLeftCalcTalentFactor2 = CalcTalentFactor2_NotMagic(pAttacker, nLeftWeaponType);
	float fRightCalcTalentFactor2 = CalcTalentFactor2_NotMagic(pAttacker, nRightWeaponType);

	float fCalcTalentFactor2 = (fLeftCalcTalentFactor2 + fRightCalcTalentFactor2) * 0.5f;

	return fCalcTalentFactor2;	
}

float GAttackDamageCalculator::ClacTalentFactor2Fomula(int nBaseTalentRank, int nSpecializationTalentRank)
{
	return ( (nBaseTalentRank * 0.05f) + (nSpecializationTalentRank * 0.05f) + 1 );
}

float GAttackDamageCalculator::CalcArmorFactor1( GEntityActor* pAttacker, const GTalentInfo* pTalentInfo )
{
	if (pAttacker->IsPlayer())
	{
		float fWeaponApplyRate = pTalentInfo->m_fWeaponApplyRate;		// ���� ������ ������

		GEntityPlayer* pPlayerAttacker = static_cast< GEntityPlayer*>(pAttacker);
		GItemHolder* pItemHolder = pPlayerAttacker->GetItemHolder();

		// �ŷ�Ʈ ������1�� ���� ������ �����Ͽ� �����Ѵ�.
		if (fWeaponApplyRate < 0.001f)
		{
			bool bHolyUnholy = false;
			if (pTalentInfo->m_nDamageAttrib == DA_HOLY || pTalentInfo->m_nDamageAttrib == DA_UNHOLY)
			{
				bHolyUnholy = true;
			}

			float fFactor = 0.0f;

			fFactor+= GetArmorFactor1Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_HEAD), bHolyUnholy);
			fFactor+= GetArmorFactor1Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_BODY), bHolyUnholy);
			fFactor+= GetArmorFactor1Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_LEG), bHolyUnholy);
			fFactor+= GetArmorFactor1Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_HANDS), bHolyUnholy);
			fFactor+= GetArmorFactor1Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_FEET), bHolyUnholy);

			return (fFactor / 5.0f);
		}
	}

	return 1.0f;
}

float GAttackDamageCalculator::m_fArmorFactor1Table[ARMOR_MAX][2] = {
	// ���ݸ���, �ż�/����
	{1.0f, 1.0f},		// none
	{1.0f, 1.0f},		// cloth
	{0.8f, 1.0f},		// light
	{0.6f, 1.0f},		// medium
	{0.4f, 1.0f},		// heavy
	{1.0f, 1.0f},		// shield
	{1.0f, 1.0f}		// accessory
};


float GAttackDamageCalculator::GetArmorFactor1Table( ARMOR_TYPE nEquipmentType, bool bHolyUnholyMagic )
{
	int nMagicCategoryIndex = (bHolyUnholyMagic) ? 1 : 0;

	if (nEquipmentType < 0 || nEquipmentType >= ARMOR_MAX) nEquipmentType = ARMOR_NONE;
	return m_fArmorFactor1Table[nEquipmentType][nMagicCategoryIndex];
}

float GAttackDamageCalculator::m_fArmorFactor2Table[ARMOR_MAX][WEAPON_MAX] = {
	// none, 1hs, 1hb, 1hp, 2hd, stf, arc, axe, dwd, dwp
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},		// none
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},		// cloth
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},		// light
	{1.0f, 1.0f, 1.0f, 0.7f, 1.0f, 1.0f, 0.7f, 1.0f, 0.7f, 1.0f},		// medium
	{1.0f, 1.0f, 1.0f, 0.7f, 1.0f, 1.0f, 0.5f, 1.0f, 0.5f, 1.0f},		// heavy
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},		// shield
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}		// accessory
};

float GAttackDamageCalculator::GetArmorFactor2Table( ARMOR_TYPE nEquipmentType, WEAPON_TYPE nWeaponType )
{
	return m_fArmorFactor2Table[nEquipmentType][nWeaponType];
}

float GAttackDamageCalculator::CalcArmorFactor2( GEntityActor* pAttacker, const GTalentInfo* pTalentInfo )
{
	float fFactor = 1.0f;

	if (pAttacker->IsPlayer())
	{
		GEntityPlayer* pPlayerAttacker = static_cast<GEntityPlayer*>(pAttacker);
		GItemHolder* pItemHolder = pPlayerAttacker->GetItemHolder();
		switch (pTalentInfo->m_WeaponReference)
		{
		case WR_LEFT:
			{
				WEAPON_TYPE nLeftWeaponType = pItemHolder->GetEquipment().GetLeftWeaponType();
				fFactor = CalcArmorFactor2_Player(pPlayerAttacker, pTalentInfo, nLeftWeaponType);
			}
			break;
		case WR_RIGHT:
			{
				WEAPON_TYPE nRightWeaponType = pItemHolder->GetEquipment().GetRightWeaponType();
				fFactor = CalcArmorFactor2_Player(pPlayerAttacker, pTalentInfo, nRightWeaponType);
			}
			break;
		case WR_BOTH:
			{
				WEAPON_TYPE nLeftWeaponType = pItemHolder->GetEquipment().GetLeftWeaponType();
				WEAPON_TYPE nRightWeaponType = pItemHolder->GetEquipment().GetRightWeaponType();
				fFactor = CalcArmorFactor2_Player(pPlayerAttacker, pTalentInfo, nLeftWeaponType, nRightWeaponType);
			}
			break;
		default:{
				_ASSERT(0);
			}
		}
	}

	return fFactor;
}

float GAttackDamageCalculator::CalcArmorFactor2_Player(GEntityPlayer* pPlayerAttacker, const GTalentInfo* pTalentInfo, WEAPON_TYPE nWeaponType)
{
	float fFactor = 1.0f;

	float fWeaponApplyRate = pTalentInfo->m_fWeaponApplyRate;		// ���� ������ ������
	// �ŷ�Ʈ ������2�� ���� ������ �����Ͽ� �����Ѵ�.
	if (fWeaponApplyRate < 0.001f)
	{
		GItemHolder* pItemHolder = pPlayerAttacker->GetItemHolder();
		fFactor+= GetArmorFactor2Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_HEAD), nWeaponType);
		fFactor+= GetArmorFactor2Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_BODY), nWeaponType);
		fFactor+= GetArmorFactor2Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_LEG), nWeaponType);
		fFactor+= GetArmorFactor2Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_HANDS), nWeaponType);
		fFactor+= GetArmorFactor2Table(pItemHolder->GetEquipment().GetSlotMaterial(ITEMSLOT_FEET), nWeaponType);

		fFactor /= 5.0f;
	}

	return fFactor;
}

float GAttackDamageCalculator::CalcArmorFactor2_Player(GEntityPlayer* pPlayerAttacker, const GTalentInfo* pTalentInfo, WEAPON_TYPE nLeftWeaponType, WEAPON_TYPE nRightWeaponType)
{		
	float fLeftFactor = CalcArmorFactor2_Player(pPlayerAttacker, pTalentInfo, nLeftWeaponType);
	float fRightFactor = CalcArmorFactor2_Player(pPlayerAttacker, pTalentInfo, nRightWeaponType);

	float fFactor = (fLeftFactor + fRightFactor) * 0.5f;

	return fFactor;
}

int GAttackDamageCalculator::CalcTalentDamage( const GDamageRangedInfo& DamageInfo, bool bCritical )
{
	// ũ��Ƽ�� ����
	if (bCritical)
	{
		return DamageInfo.nMaxDamage;
	}

	return MakeDamageFromMinMaxDamage(DamageInfo);
}

float GAttackDamageCalculator::CalcBuffDefenceFactor( GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo )
{
	float fBuffFactor = 1.0f;

	DAMAGE_ATTRIB nDamageAttrib = pTalentInfo->m_nDamageAttrib;
	if (nDamageAttrib == DA_NONE)
	{
		// ����Ÿ���� ������ ���� ����Ÿ���� ����
		if (pAttacker->IsPlayer())
		{
			nDamageAttrib = ToEntityPlayer(pAttacker)->GetDamageType(pTalentInfo);
		}
	}

	if (nDamageAttrib <= DA_NONE)
		return fBuffFactor;

	if (nDamageAttrib >= DA_MAX)
		return fBuffFactor;

	fBuffFactor += pVictim->GetChrStatus()->ActorModifier.fDefenceAmp[nDamageAttrib];
	return fBuffFactor;
}

GAttackDamageCalculator::GAttackDamageCalculator( GCriticalCalculator* pCriticalCalculator/*=NULL*/ )
{
	if (pCriticalCalculator==NULL)
	{
		m_pCriticalCalculator = new GCriticalCalculator();
	}
	else
	{
		m_pCriticalCalculator = pCriticalCalculator;
	}
}

GAttackDamageCalculator::~GAttackDamageCalculator()
{
	SAFE_DELETE(m_pCriticalCalculator);
}

int GAttackDamageCalculator::CalcNPCWeaponDamage( const GNPCInfo* pAttackerNPCInfo )
{
	if (pAttackerNPCInfo)
	{
		return GMath::RandomNumber(pAttackerNPCInfo->m_nMinDamage, pAttackerNPCInfo->m_nMaxDamage);
	}
	return 0;
}

float GAttackDamageCalculator::CalcCharacterFactorForMagic(GEntityPlayer* pAttacker)
{
	int nInt = pAttacker->GetINT();

	return GMath::Round4Combat((nInt - 50) * 0.002f + 0.9f, 2);
}

float GAttackDamageCalculator::CalcCharacterFactorForMelee(GEntityPlayer* pAttacker)
{
	int nStr = pAttacker->GetSTR();

	return GMath::Round4Combat((nStr - 50) * 0.002f + 0.9f, 2);
}

float GAttackDamageCalculator::CalcCharacterFactorForRange(GEntityPlayer* pAttacker)
{
	int nStr = pAttacker->GetSTR();
	int nDex = pAttacker->GetDEX();

	return GMath::Round4Combat(((nStr + nDex) * 0.5f - 50) * 0.002f + 0.9f, 2);
}

ATTACK_TYPE_FOR_CALCULATE GAttackDamageCalculator::CalcAttackTypeForCalculate(GEntityPlayer* pAttacker, const GTalentInfo* pTalentInfo)
{
	switch(pTalentInfo->m_nCategory)
	{
	case TC_MAGIC:
		return ATC_MAGIC_ATTACK;
	case TC_RANGE:
		return ATC_DEX_WEAPON_ATTACK;
	case TC_MELEE:
		{
			// ��꿡 ����� ���� Ÿ���� ���Ѵ�.
			GItemHolder* pItemHolder = pAttacker->GetItemHolder();
			WEAPON_TYPE nWeaponType = WEAPON_NONE;

			switch(pTalentInfo->m_WeaponReference)
			{
			case WR_LEFT:			
				nWeaponType = pItemHolder->GetEquipment().GetLeftWeaponType();
				break;
			case WR_RIGHT:
				nWeaponType = pItemHolder->GetEquipment().GetRightWeaponType();
				break;
			case WR_BOTH:
				nWeaponType = pItemHolder->GetEquipment().GetWeaponType();
				break;
			};

			// ���, ���Ÿ�, ��󹫱�
			if (nWeaponType == WEAPON_ARCHERY ||
				nWeaponType == WEAPON_1H_PIERCE ||
				nWeaponType == WEAPON_DUAL_WIELD || nWeaponType == WEAPON_DUAL_PIERCE)
			{
				return ATC_DEX_WEAPON_ATTACK;
			}
			else
			{
				return ATC_STR_WEAPON_ATTACK;
			}
		}		
	};	

	return ATC_STR_WEAPON_ATTACK;
}

bool GAttackDamageCalculator::HasWeaponPassiveTalent( GEntityActor* pActor, const GItem* pWeaponItem )
{
	if (!pActor->IsPlayer())
		return true;

	if (!gsys.pItemSystem->GetEquiper().CheckHasPassiveTalent(ToEntityPlayer(pActor), (GItem*) pWeaponItem))
		return false;

	return true;
}
