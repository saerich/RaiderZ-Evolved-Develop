#include "stdafx.h"
#include "XCaptionEffect.h"
#include "XPlayer.h"
#include "XCharacterCaption.h"
#include "XExpCaption.h"
#include "XCharacter.h"
#include "XModuleCollision.h"
#include "RSceneManager.h"
#include "XCaptionMgr.h"
#include "XTalentHelper.h"
#include "XStrings.h"

#define GUARD_MESSAGE_PERFECT		XGetStr( L"CAPTION_DEFENCESUCCESS")


XDamageCaption* XCaptionEffect::NewDamageCaption()
{
	return NewCaption<XDamageCaption>();
}

XExpCaption* XCaptionEffect::NewExpCaption()
{
	return NewCaption<XExpCaption>();
}

void XCaptionEffect::ShowGuardDefense( XObject* pOwner, int nDamage, GUARD_TYPE eGuardType )
{
	if (XGETCFG_GAME_CAPTION == false)	return;
	// ���� ���̵��� �Ѵ�.
	if (!IsMyPlayer(pOwner))	return;

	XDamageCaption* pNewCaption = NULL;

	vec3 vGuardDefencePos = GetHeadPos(pOwner);

	// ��� �������� �Ϲ� �������� ǥ���� �޷��� �ٲ�
	//uint32 nColor = XCONST::CAPTION_EFFECT_COLOR_MY_GUARD_DEFENSE;
	uint32 nColor = XCONST::CAPTION_EFFECT_COLOR_MY_DAMAGE;
	wstring strMessage = GUARD_MESSAGE_PERFECT;
	if(nDamage > 0)
	{
		wchar_t szGuard[64] = L"";		
		swprintf_s(szGuard, L"%d", nDamage);
		strMessage = szGuard;
		
		pNewCaption = NewDamageTextureCaption();
	}
	else
		pNewCaption = NewDamageCaption();

	pNewCaption->SetupMine(strMessage.c_str(), vGuardDefencePos, false, nColor);
}

void XCaptionEffect::ShowHeal( XObject* pOwner, int nHeal, UIID nUserUIID /*= UIID_INVALID */ )
{
	if (XGETCFG_GAME_CAPTION == false)	return;

	// ������ ���� ���� ó���� �Ѵ�.
	// ���� ���̵��� �Ѵ�.
	if (!IsMyPlayer(pOwner) &&
		(nUserUIID == UIID_INVALID || nUserUIID != XGetMyUIID()))	
		 return;

	XDamageTextureCaption* pNewCaption = NewDamageTextureCaption();

	vec3 vHealPos = GetHeadPos(pOwner);

	wchar_t szHeal[32] = L"";
	swprintf_s(szHeal, L"+%d", nHeal);
	uint32 nColor = XCONST::CAPTION_EFFECT_COLOR_MY_HEAL;

	pNewCaption->SetupMine(szHeal, vHealPos, false, nColor);
}

void XCaptionEffect::ShowHPRegen( XObject* pOwner, int nHeal )
{
	if (XGETCFG_GAME_CAPTION == false)	return;

	// ������ ���� ���� ó���� �Ѵ�.
	// ���� ���̵��� �Ѵ�.
	if (!IsMyPlayer(pOwner))	return;

	XDamageTextureCaption* pNewCaption = NewDamageTextureCaption();

	vec3 vHealPos = GetHeadPos(pOwner);

	wchar_t szHeal[32] = L"";
	swprintf_s(szHeal, L"+%d", nHeal);
	uint32 nColor = XCONST::CAPTION_EFFECT_COLOR_MY_HP_REGEN;

	pNewCaption->SetupMine(szHeal, vHealPos, false, nColor);
}

void XCaptionEffect::ShowDebug( XObject* pOwner, const wchar_t* szMessage, uint32 nColor )
{
	XDamageCaption* pNewCaption = NewDamageCaption();

	vec3 vPos = pOwner->GetPosition();
	vPos.z += 100.0f;

	pNewCaption->SetupEnemy(szMessage, vPos, false, nColor);
}

void XCaptionEffect::ShowBuff( XObject* pOwner, const wchar_t* szName, bool bIsDebuff )
{
	if (XGETCFG_GAME_CAPTION == false)	return;
	if (!IsMyPlayer(pOwner))	return;

	vec3 vPos = pOwner->GetPosition();
	vPos.z += 200.0f;

	XModuleCollision* pModuleCollision = pOwner->GetModuleCollision();
	if(pModuleCollision)
	{
		MCapsule playerCapsule = pModuleCollision->GetCollisionCapsuleByIndex(0);
		vPos = playerCapsule.top;
		vPos.z += playerCapsule.radius;
	}

	uint32 nColor = (bIsDebuff == true) ? XCONST::CAPTION_EFFECT_COLOR_MY_DEBUFF : XCONST::CAPTION_EFFECT_COLOR_MY_BUFF;

	XDamageCaption* pNewCaption = NewDamageCaption();
	pNewCaption->SetupMine( szName, vPos, false, nColor);
}

// �븻 �������� ���� �ο�� �ִ� ��븸 ���̵��� �ϰ�
// �ٸ� ���Ľ��� �� ���̵���
void XCaptionEffect::ShowDamage( XObject* pOwner, _DamageInfo * pInfo, vec3 vDamagePos )
{
	if (XGETCFG_GAME_CAPTION == false)	return;
//	if (pInfo->nDamage == 0) return;
	if (pInfo->IsFlagZeroDamage()) return;

	wstring strDamage = GetDamageString(pInfo);
	bool bBothDamage = false;

	//�븻 �������� ���� �����ִ� ����� �����ش�.
	// �巹���� �������� ���;� �Ѵ�.
	if (pInfo->nDamage != 0)
	{
		if(strDamage.empty() || pInfo->IsFlagDrain())
		{
			if (pInfo->nDamageFlag != DF_BUFF 
				&& (pOwner->GetUID() != XGetMyUID()
				&& pInfo->nAttackerUIID != XGetMyUIID())
				&& !pInfo->IsFlagFalling())
				return;

			wchar_t szDamage[16] = L"";
			_itow_s(pInfo->nDamage, szDamage, 16, 10);

			// �̰� ����
			XDamageCaption* pNewCaption = NewDamageTextureCaption();
			SetupDamageCaption(pOwner, pNewCaption, pInfo, vDamagePos, wstring(szDamage));

			bBothDamage = true;
		}
	}
	
	if(strDamage.empty() == false)
	{
		// ���ڶ� ���� ������ ��� ���ڴ� ���� ���� ��´�.
		if(bBothDamage)
			vDamagePos.z += 20.0f;

		// �̰� ����
		XDamageCaption* pNewCaption = NewDamageCaption();
		SetupDamageCaption(pOwner, pNewCaption, pInfo, vDamagePos, strDamage);
	}
}

wstring XCaptionEffect::GetDamageFlag( const uint32& nFlags, TALENT_RESIST_TYPE nResistType, bool bHitDodge )
{
	if (bHitDodge)									return XGetStr( L"CAPTION_DODGE");
	if (CheckBitSet(nFlags,CTR_RIPOSTE))			return XGetStr( L"CAPTION_RIPOSTE");
	if (CheckBitSet(nFlags,CTR_RESIST_PERFECT) && nResistType != TRT_NONE)		return XGetStr( L"CAPTION_RIPOSTE");
	if (CheckBitSet(nFlags,CTR_RESIST_PARTIAL) && nResistType != TRT_NONE)		return XGetStr( L"CAPTION_RESISTANCE");
	if (CheckBitSet(nFlags,CTR_DRAIN))				return XGetStr( L"CAPTION_ABSORB");
	if (CheckBitSet(nFlags,CTR_IMMUNE))				return XGetStr( L"CAPTION_DISPEL");
	if (CheckBitSet(nFlags,CTR_MISS))				return XGetStr( L"CAPTION_MISS");
	if (CheckBitSet(nFlags,CTR_AVOID))				return XGetStr( L"CAPTION_AVOID");

	return L"";
}

bool XCaptionEffect::IsMyPlayer( XObject* pOwner )
{
	return (pOwner->GetUID() == XGetMyUID());
}

vec3 XCaptionEffect::GetHeadPos( XObject* pOwner )
{
	vec3 vHeadPos = pOwner->GetPosition();
	vHeadPos.z += 150.0f;

	// ���� ��� ��ġ ��������
	if(pOwner->GetActor())
	{
		if (pOwner->GetActor()->GetActorNodePos(HEAD_EFFECT_BASE_BONE, vHeadPos, E_TS_WORLD))
		{
			vHeadPos.z += 40.0f;
		}
	}

	return vHeadPos;
}

void XCaptionEffect::ShowExp( XObject* pOwner, int nXP )
{
	if (XGETCFG_GAME_CAPTION == false)	return;

	// ���� ���̵��� �Ѵ�.
	if (!IsMyPlayer(pOwner))	return;

	XExpCaption* pNewCaption = NewExpCaption();

	vec3 vCaptionPos = pOwner->GetPosition();
	vCaptionPos.z += 140.0f;

	vCaptionPos.x += XMath::RandomNumber(-20.0f, 20.0f);
	vCaptionPos.y += XMath::RandomNumber(-20.0f, 20.0f);
	vCaptionPos.z += XMath::RandomNumber(-20.0f, 20.0f);

	pNewCaption->Setup(nXP, vCaptionPos);

}

XDamageTextureCaption* XCaptionEffect::NewDamageTextureCaption()
{
	return NewCaption<XDamageTextureCaption>();
}

void XCaptionEffect::ShowEn( XObject* pOwner, int nEn )
{
	if (XGETCFG_GAME_CAPTION == false)	return;

	// ������ ���� ���� ó���� �Ѵ�.
	// ���� ���̵��� �Ѵ�.
	if (!IsMyPlayer(pOwner))	return;

	XDamageTextureCaption* pNewCaption = NewDamageTextureCaption();

	vec3 vEnPos = GetHeadPos(pOwner);

	wchar_t szEn[32] = L"";
	swprintf_s(szEn, L"+%d", nEn);
	uint32 nColor = XCONST::CAPTION_EFFECT_COLOR_MY_EN;

	pNewCaption->SetupMine(szEn, vEnPos, false, nColor);
}

void XCaptionEffect::SetupDamageCaption( XObject* pOwner, XDamageCaption* pNewCaption, _DamageInfo * pInfo, vec3 vDamagePos, wstring& strDamage )
{
	bool bCritical = false;
	bool bResist = false;
	bool bMissOrAvoid = false;

	if (pInfo->IsFlagCritical()) bCritical = true;
	if (pInfo->IsFlagResistPerfect()) bResist = true;
	if (pInfo->IsFlagMiss() || pInfo->IsFlagAvoid()) bMissOrAvoid = true;

	if (pOwner->GetUID() == XGetMyUID())
	{
		uint32 nColor = XCONST::CAPTION_EFFECT_COLOR_MY_DAMAGE;

		if (bMissOrAvoid) nColor = XCONST::CAPTION_EFFECT_COLOR_MY_MISS;
		else if (bCritical) nColor = XCONST::CAPTION_EFFECT_COLOR_MY_CRITICAL;
		else if (bResist) nColor = XCONST::CAPTION_EFFECT_COLOR_MY_DAMAGE;
		

		pNewCaption->SetupMine(strDamage.c_str(), vDamagePos, bCritical, nColor);
	}
	else
	{
		uint32 nColor = XCONST::CAPTION_EFFECT_COLOR_ENEMY_DAMAGE;
		if (bMissOrAvoid) nColor = XCONST::CAPTION_EFFECT_COLOR_ENEMY_MISS;
		if (bCritical) nColor = XCONST::CAPTION_EFFECT_COLOR_ENEMY_CRITICAL;
		if (bResist) nColor = XCONST::CAPTION_EFFECT_COLOR_ENEMY_DAMAGE;

		pNewCaption->SetupEnemy(strDamage.c_str(), vDamagePos, bCritical, nColor);

		//if (bCritical)
		//{
		//		global.camera->PlayCameraEffect("dummy_camera_s", 0.f);
		//}
	}
}

wstring XCaptionEffect::GetDamageString( _DamageInfo * pInfo )
{
	TALENT_RESIST_TYPE nResistType = TRT_NONE;
	XObject* pAttacker = gg.omgr->FindActor_UIID(pInfo->nAttackerUIID);
	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(pInfo->nAttackedTalentID, pAttacker);
	if(pTalentInfo)
	{
		nResistType = pTalentInfo->m_Resist.m_nType;
	}
	
	return GetDamageFlag(pInfo->nFlags, nResistType, pInfo->IsHitDodge());
}
