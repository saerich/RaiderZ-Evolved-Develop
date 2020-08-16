#include "stdafx.h"
#include "XModuleTalent.h"
#include "XObject.h"
#include "XEventID.h"
#include "XModuleCollision.h"
#include "XModuleEffect.h"
#include "XNonPlayer.h"
#include "XTalentInfoMgr.h"
#include "XTalentHelper.h"
#include "XAutoSoundChecker.h"
#include "XModuleEntity.h"
#include "XGameTransformControllerManager.h"
#include "XModuleActorControl.h"
#include "XMyPlayer.h"

//#include "RDebugPassRenderer.h"

XBaseTalent* XTalentFactory::NewTalent(XObject* pOwner, XTalentInfo* pTalentInfo)
{
	if (pTalentInfo)
	{
		switch (pTalentInfo->m_nSkillType)
		{
		//case ST_NONE: return new XBaseTalent(pOwner); break;
		case ST_NONE: 
			{
				break;
			}
		case ST_SOCIAL:
		case ST_MELEE: 
			{
				return new XMeleeTalent(pOwner); 
			}
			break;
		case ST_ARCHERY: return new XArcheryTalent(pOwner); break;
		case ST_MAGIC: return new XMagicTalent(pOwner); break;
		case ST_EXTRA_ACTIVE: 
			{
				if (pTalentInfo->m_nExtraActive == TEAT_NPC_ONLY_GRAPPLE) return new XExtraActiveTalent_Grapple(pOwner); 
				if (pTalentInfo->m_nExtraActive == TEAT_NPC_ONLY_EAT) return new XExtraActiveTalent_Eat(pOwner);
				return new XExtraActiveTalent(pOwner);
			}
			break;
		case ST_ITEM:	return new XItemTalent(pOwner); break;
		case ST_GUARD: return new XGuardTalent(pOwner); break;
		}
	}

	return new XBaseTalent(pOwner);
}

//////////////////////////////////////////////////////////////////////////
XModuleTalent::XModuleTalent(XObject* pOwner)
: XModule(pOwner)
{
	m_pCurrentTalent = XTalentFactory::NewTalent(pOwner, NULL);
	m_matServerStartTalent.MakeIdentity();
}

XModuleTalent::~XModuleTalent()
{
	SAFE_DELETE(m_pCurrentTalent);
}


void XModuleTalent::OnSubscribeEvent()
{
	Subscribe(XEVTL_MOTION_PLAYONCE);
	Subscribe(XEVTL_ON_CHANGED_MOTION_TO_IDLE);
	Subscribe(XEVTL_ON_CHANGE_MOTION);
	Subscribe(XEVTL_UPPER_MOTION_PLAYONCE);
	XModule::OnSubscribeEvent();
}

XEventResult XModuleTalent::OnEvent(XEvent& msg)
{
	if (m_pCurrentTalent) return m_pCurrentTalent->OnEvent(msg);
	return MR_FALSE;
}


void XModuleTalent::OnUpdate(float fDelta)
{
	PFC_THISFUNC;

	m_pCurrentTalent->OnUpdate(fDelta);

	int nCurrentTalentID = 0;
	if (m_pCurrentTalent->IsActive() && m_pCurrentTalent->GetInfo())
	{
		nCurrentTalentID = m_pCurrentTalent->GetInfo()->m_nID;
	}

	m_HitFrameFreezer.Update(fDelta, m_pOwner, m_pOwner->GetModuleEntity()->GetAnimationController()->GetAnimationName().c_str());
}

void XModuleTalent::OnFinalized()
{ 
	XModuleCollision * pModuleCollision = m_pOwner->GetModuleCollision();
	if(pModuleCollision)
	{
		pModuleCollision->SetHitTalentComplete();
	}

	if(m_pCurrentTalent)
	{
		m_pCurrentTalent->FinishTalent(); 
	}
}

void XModuleTalent::HitEffect( int nTalentID, XObject* pAttackObject, vec3 vHitPosition, HITEFFECT_RESULT& resultHitEffect, bool bSec /*= false*/, unsigned int nDamageFlags /*= 0*/ )
{
	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(nTalentID, pAttackObject);

	if(pTalentInfo)
	{
		bool bCreateEffect = false;
		if(pTalentInfo->m_nSkillType == ST_MELEE)
		{
			bCreateEffect = true;
		}
		else if (pTalentInfo->m_nSkillType == ST_MAGIC)
		{
			// �߻�ü ���� ������ ��쿡�� �ǰ� ����Ʈ�� ���� �� ����.
			if (pTalentInfo->m_Events.HasEvent(TE_LAUNCH_PROJECTILE) == false) bCreateEffect = true;

			// ȭ���̸� �ǰ��߿��� �ǰ� ���;� �Ѵ�.
			if(pTalentInfo->m_nCategory == TC_RANGE)
			{
				vec3 vHitCenter = m_pOwner->GetPosition();
				vHitCenter.z += 40;
				if(m_pOwner->GetModuleCollision())
				{
					vHitCenter = m_pOwner->GetModuleCollision()->GetColObjectCenter();
				}

				XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
				if(pModuleEffect)
				{
					//global.emgr->CreateByTalentEffectType(pTalentInfo, BLOOD_EFFECT, m_pOwner, vHitCenter, pAttackObject);	// �� ����Ʈ
					XTalentEffectEventData* pEffectData = new XTalentEffectEventData;
					pEffectData->Set(pTalentInfo, BLOOD_EFFECT, m_pOwner, vHitCenter, pAttackObject, false, nDamageFlags);
					pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT, pEffectData);
				}
			}
		}

		if (bCreateEffect)
			HitCreateEffect(pTalentInfo->m_nID, pTalentInfo, pAttackObject, vHitPosition, resultHitEffect, bSec, nDamageFlags);	

	}
}

void XModuleTalent::DefenseEffect( int nTalentID, XObject* pAttackObject, vec3 vHitPosition )
{
	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(nTalentID, m_pOwner);

	if(pTalentInfo)
	{
		if(pTalentInfo->m_nCategory == TC_MELEE)
		{
			// �����϶���...
			DefenseCreateEffect(pTalentInfo->m_nID, pTalentInfo, pAttackObject, vHitPosition);	
		}
	}
}

void XModuleTalent::HitCreateEffect( int nTalentID, XTalentInfo* pTalentInfo, XObject* pAttackObject, vec3 vHitPosition, HITEFFECT_RESULT& resultHitEffect, bool bSec /*= false*/, unsigned int nDamageFlags /*= 0*/ )
{
	//if(pAttackObject == NULL) 
	//{
	//	return;
	//}

	XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
	if(pModuleEffect)
	{
		// ���� �Ϲ�ȭ üũ
 		XAutoSoundChecker autoSound;
 		autoSound.HitSoundChecker(m_pOwner, pTalentInfo, pAttackObject);

		// Į ���� ��������
		MMatrix matMeleeAttackDirection = MMatrix::IDENTITY;
		vec3 vAttackerDir = -m_pOwner->GetDirection();

		if(pAttackObject)
		{
			XModuleEffect * pAttackerModuleEffect = pAttackObject->GetModuleEffect();
			if(pAttackerModuleEffect)
				matMeleeAttackDirection = pAttackerModuleEffect->GetMeleeHitDirection(pTalentInfo);

			vAttackerDir = pAttackObject->GetDirection();
		}

		// �ǰ� ��ġ ���
		MMatrix matHitEffect = CalHitEffectMatrix(vHitPosition, vAttackerDir, matMeleeAttackDirection);

		//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBoxAndAxis( matHitEffect, 5, 30, D3DCOLOR_XRGB(255, 0, 180));

		// �� ��ġ ���
		MMatrix matBloodEffect = CalBloodEffectMatrix(vHitPosition, vAttackerDir, matMeleeAttackDirection);

		// �ǰ� ����Ʈ
		XTalentEffectEventData * pAttackEffectData = new XTalentEffectEventData;
		if(bSec)
			pAttackEffectData->Set(pTalentInfo, ATTACK_SECONDARY_EFFECT, m_pOwner, vHitPosition, pAttackObject, false, nDamageFlags, matHitEffect);
		else
			pAttackEffectData->Set(pTalentInfo, ATTACK_EFFECT, m_pOwner, vHitPosition, pAttackObject, false, nDamageFlags, matHitEffect);
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT, pAttackEffectData);

		// �� ����Ʈ
		XTalentEffectEventData * pBloodEffectData = new XTalentEffectEventData;
		pBloodEffectData->Set(pTalentInfo, BLOOD_EFFECT, m_pOwner, vHitPosition, pAttackObject, false, nDamageFlags, matBloodEffect);
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT, pBloodEffectData);

		// ����
		resultHitEffect.matHitEffect = matHitEffect;
		resultHitEffect.matBloodEffect = matBloodEffect;
	}
}

void XModuleTalent::DefenseCreateEffect( int nTalentID, XTalentInfo* pTalentInfo, XObject* pTargetObject, vec3 vHitPosition )
{
	// ���潺�� �����ڿ��� �޼����� ���Ƿ� �浹 �˻� �� ������ �ǰ��� �������� ���߾���.
	if(pTargetObject == NULL) 
	{
		return;
	}

	XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
	if(pModuleEffect)
	{
		XTalentEffectEventData * pDefenseEffectData = new XTalentEffectEventData;
		pDefenseEffectData->Set(pTalentInfo, DEFENSE_EFFECT, m_pOwner, vHitPosition, pTargetObject);
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT, pDefenseEffectData);
	}
}

void XModuleTalent::ChangeNPCMode()
{
	if (m_pOwner->GetEntityType() != ETID_NPC) return;

	XTalentInfo* pTalentInfo = m_pCurrentTalent->GetInfo();
	if (pTalentInfo == NULL) return;

	if (pTalentInfo->m_ChangeModeInfo.m_bChangeMode)
	{
		XNonPlayer* pOwnerNPC = m_pOwner->AsNPC();
		pOwnerNPC->ChangeMode(pTalentInfo->m_ChangeModeInfo.m_nModeTo);
	}
}

bool XModuleTalent::Start(int nTalentID)
{
	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(nTalentID, m_pOwner);
	if (pTalentInfo == NULL) return false;										// ��ų�� �����ϴ��� üũ

	// ��ų Ÿ���� ���ؼ� m_pCurrentTalent�� �ٲ۴�.[10/23/2007 isnara]
	if(ChangeTalentStrategy(pTalentInfo) == false)
	{
		return false;
	}

	m_pCurrentTalent->SetServerStartMatrix(m_matServerStartTalent);
	m_pCurrentTalent->Start(pTalentInfo);

	if(CSTalentInfoHelper::IsImmediateTalent(pTalentInfo))
		ChangeNPCMode();

	// �����׷� ����
	XModuleCollision * pModuleCollision = m_pOwner->GetModuleCollision();
	if(pModuleCollision)
	{
		pModuleCollision->SetHitCapsuleByTalent(pTalentInfo->m_nHitCapsuleGroupIndex);
	}

	SetNPCGuardIcon(pTalentInfo);

	return true;
}

bool XModuleTalent::Act()
{
	m_pCurrentTalent->Act();

	ChangeNPCMode();
	
	return false;
}

// ��ų Ÿ���� ���ؼ� m_pCurrentTalent�� �ٲ۴�.
bool XModuleTalent::ChangeTalentStrategy( XTalentInfo* pTalentInfo )
{
	// �ŷ�Ʈ ���� ����
	// ���� �ŷ�Ʈ �ʱ�ȭ
	if (m_pCurrentTalent)
	{
		OnFinalized();
		SAFE_DELETE(m_pCurrentTalent);
	}

	m_pCurrentTalent = XTalentFactory::NewTalent(m_pOwner, pTalentInfo);

	return true;
}

void XModuleTalent::ReCastingTalent( int nTalentID, int nStep /*= 0*/ )
{
	m_pCurrentTalent->ReCastingTalent(nTalentID, nStep);
}

void XModuleTalent::ExtraAct()
{
	m_pCurrentTalent->ExtraAct();
}

void XModuleTalent::OnHitTarget(int nAttackedTalentID)
{
	if (GetTalent() && GetTalent()->GetInfo())
	{
		XTalentInfo* pTalentInfo = GetTalent()->GetInfo();
		if (pTalentInfo->m_nID == nAttackedTalentID && pTalentInfo->m_bFreezeFrame)
		{
			m_HitFrameFreezer.OnHit(m_pOwner, m_pOwner->GetModuleEntity()->GetAnimationController()->GetAnimationName().c_str());
		}
	}
}

void XModuleTalent::ExtraAct2()
{
	m_pCurrentTalent->ExtraAct2();
}

void XModuleTalent::ExtraAct3()
{
	m_pCurrentTalent->ExtraAct3();
}

bool XModuleTalent::IsTalentColliable()
{
	if(IsActive() == false)
		return true;

	if(GetInfo() == false)
		return true;

	if(GetInfo()->m_eTalentShoveType == TST_PASS)
		return false;

	return true;
}

MMatrix XModuleTalent::CalHitEffectMatrix( vec3 vHitPosition, vec3 vAttackerDir, MMatrix matMeleeHitDir )
{
	MMatrix matWorld;
	matWorld.SetLocalMatrix(vHitPosition, vAttackerDir, vec3::AXISZ);

	return matMeleeHitDir * matWorld;
}

MMatrix XModuleTalent::CalBloodEffectMatrix( vec3 vHitPosition, vec3 vAttackerDir, MMatrix matMeleeHitDir )
{
	MMatrix matCal;
	matCal.MakeIdentity();

	vec3 vMeleeUp = vec3(matMeleeHitDir._31, matMeleeHitDir._32, matMeleeHitDir._33);

	if(vMeleeUp == vec3::AXISZ)
		return matCal;

	vec3 vBloodUp = vMeleeUp.CrossProduct(-vec3::AXISY);

	MMatrix matBloodLocal;
	matBloodLocal.SetLocalMatrix(vec3::ZERO, vec3::AXISY, vBloodUp);

	matCal.SetLocalMatrix(vec3::ZERO, vAttackerDir, vec3::AXISZ);
	matCal = matBloodLocal * matCal;

	return matCal;
}

void XModuleTalent::SetNPCGuardIcon(XTalentInfo* pTalentInfo)
{
	// ���尡 �Ұ����� �ŷ�Ʈ�� ���������� ǥ�� (NPC��)
	if(m_pOwner->GetEntityType() == ETID_NPC)
	{
		XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
		if(pModuleEffect == NULL || pTalentInfo == NULL)
			return;

		// Min, Max�� 0�̸� ������ �ŷ�Ʈ�� �ƴϴ�. 
		if(pTalentInfo->m_nMinDamage == 0 &&
			pTalentInfo->m_nMaxDamage == 0)
			return;

		XTalentEffectEventData * pEffectData = new XTalentEffectEventData;

		// ���� �Ұ���
		if(pTalentInfo->m_nGuardCrashLevel == GCL_ALL)
		{
			pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT_GUARD_DISABLE, pEffectData);
			return;
		}
		// ���зθ� ���� ����
		else if(pTalentInfo->m_bPerfectGuardOnly)
		{
			XMyPlayer* pMyPlayer = gg.omgr->FindMyPlayer();
			if(pMyPlayer && pMyPlayer->IsEquipShield() == false)
			{
				pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT_GUARD_DISABLE, pEffectData);
				return;
			}				
		}

		SAFE_DELETE(pEffectData);
	}
}

void XModuleTalent::Reload()
{
	if (m_pCurrentTalent)
	{
		SAFE_DELETE(m_pCurrentTalent);
	}
	m_pCurrentTalent = XTalentFactory::NewTalent(m_pOwner, NULL);
	m_matServerStartTalent.MakeIdentity();
}
