#include "stdafx.h"
#include "XTalentEffect.h"
#include "XTalentEffectMgr.h"
#include "XTalentInfoMgr.h"
#include "XCharacter.h"
#include "XNonPlayer.h"
#include "XModuleCollision.h"
#include "XModuleMovable.h"
#include "XGame.h"

// ����׿�
//#include "RDebugPassRenderer.h"

//////////////////////////////////////////////////////////////////////////
#define DEFAULT_EFFECT_FILE_NAME	L"meleeattack01"
#define BLOOD_EFFECT_COUNT			2

//////////////////////////////////////////////////////////////////////////

XTalentEffect::XTalentEffect()
{
	m_vecDelEffectList.clear();
	m_vecDelSoundList.clear();

	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( CAST_EFFECT,		CreateCastEffect) );
	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( USE_EFFECT,			CreateUseEffect) );
	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( ATTACK_EFFECT,		CreateAttackEffect) );
	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( ATTACK_SECONDARY_EFFECT,		CreateAttackEffect) );
	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( MISS_EFFECT,		CreateMissEffect) );
	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( SHOCK_EFFECT,		CreateShockEffect) );
	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( DURATION_EFFECT,	CreateDurationEffect) );
	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( DEFENSE_EFFECT,		CreateDefenseEffect) );
	m_mapTalentEffectHandlers.insert( map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::value_type( CAST_END_EFFECT,	CreateCastEndEffect) );

	m_bHaveAttackSound = false;
	m_bCheckAttackSound = false;
}

XTalentEffect::~XTalentEffect()
{
	ClearEffect();
}

void XTalentEffect::CreateByTalentEffectType( CREATE_TALENT_EFFECT_DATA& creatTalentEffectData )
{
	if (creatTalentEffectData.pTalentInfo == NULL || creatTalentEffectData.pOwner == NULL) 
		return;

	//////////////////////////////////////////////////////////////////////////
	// �غ�
	XTalentEffectDataMgr* pTalentEffectDataMgr = creatTalentEffectData.pTalentInfo->GetEffectDataMgr();

	// nType���� ����ƮInfo�� �����´�.
	vector<XTalentEffectInfo *> vecEffectInfoByType;
	if(pTalentEffectDataMgr && pTalentEffectDataMgr->IsEmpty() == false)
	{
		pTalentEffectDataMgr->Get(creatTalentEffectData.nType, vecEffectInfoByType);
	}

	m_bHaveAttackSound = false;
	m_bCheckAttackSound = false;
	//////////////////////////////////////////////////////////////////////////
	// ����Ʈ �߻�
	bool bModelEffect = false;
	vec3 vDir = vec3::ZERO;
	vector<XTalentEffectInfo*>::iterator it = vecEffectInfoByType.begin();
	while(it != vecEffectInfoByType.end())
	{
		vDir = GetTalentEffectDir((*it), creatTalentEffectData.pOwner, creatTalentEffectData.pAttacker);

		// ���� ĸ���̶�� �ɷ�����.
		if((*it)->m_nImpactPos == TALENT_HIT_CAPSULE_POS)
		{
			vec3 vTalentDir = creatTalentEffectData.pOwner->GetDirection();
			if(creatTalentEffectData.pOwner->GetUID() != XGetMyUID())
				vTalentDir = creatTalentEffectData.pOwner->GetModuleMovable()->GetMovableFactor().INTERPOLATION.m_vTargetDir;

			talentEffectHitCapsule.AddTalentHitCapsuleImpact(creatTalentEffectData.pOwner->GetUID(), creatTalentEffectData.pTalentInfo, (*it), creatTalentEffectData.nType, -vTalentDir);
		}
		else
		{
			TALENT_EFFECT_INFO_DATA talentEffectInfoData;
			map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER>::iterator itTalentEffect = m_mapTalentEffectHandlers.find((*it)->m_nEffectType);
			if(itTalentEffect != m_mapTalentEffectHandlers.end())
			{
				talentEffectInfoData.Set(*it, creatTalentEffectData.pOwner, creatTalentEffectData.effectPos, vDir, creatTalentEffectData.matEffect, creatTalentEffectData.pAttacker, creatTalentEffectData.nDamageFlags);
				itTalentEffect->second(&talentEffectInfoData);
			}

			if(creatTalentEffectData.nType == ATTACK_EFFECT)
			{
				m_bCheckAttackSound = true;
				CheckAttackEffectSound(talentEffectInfoData.vecOutResultData);
			}

			for(vector<EFFECT_RESULT_DATA>::iterator it = talentEffectInfoData.vecOutResultData.begin(); it != talentEffectInfoData.vecOutResultData.end(); it++)
			{
				if((*it).type == CET_MODEL)
				{
					if(creatTalentEffectData.bEffectStorage)
					{
						EFFECT_DEL EffectDel;
						EffectDel.uidEffect			= (*it).uidEffect;
						m_vecDelEffectList.push_back(EffectDel);
					}

					// �� ����Ʈ üũ
					// ������... ����Ʈ ����Ʈ �߻��ؾ� �ϳ�.
					bModelEffect = true;
				}

				if((*it).type == CET_SOUND)
				{
					if(creatTalentEffectData.bEffectStorage)
					{
						m_vecDelSoundList.push_back((*it).uidEffect);
					}
				}
			}
		}

		it++;
	}

	//////////////////////////////////////////////////////////////////////////
	// �ǰ� ����
	if(((creatTalentEffectData.nType == ATTACK_EFFECT || creatTalentEffectData.nType == ATTACK_SECONDARY_EFFECT) && vecEffectInfoByType.empty()) || 
		(m_bCheckAttackSound == true && m_bHaveAttackSound == false))
	{
		// ������ �𵨿��� ã��
		if(creatTalentEffectData.pAttacker && creatTalentEffectData.pAttacker->GetActor() && creatTalentEffectData.pAttacker->GetActor()->GetAnimationHitSoundInfo())
		{
			wstring strSoundName = creatTalentEffectData.pAttacker->GetActor()->GetAnimationHitSoundInfo()->Get(creatTalentEffectData.pAttacker->GetCurrentAnimationName());
			if ( strSoundName.empty() == false)
			{
				if ( global.sound)
					global.sound->PlayDynamic3D( MLocale::ConvUTF16ToAnsi(strSoundName.c_str()).c_str(), NULL, 0, creatTalentEffectData.pOwner->GetUID());
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// �� �� ����Ʈ�� ������ ǥ�Ⱑ �Ǿ��ĸ�...
	// �ŷ�Ʈ ����Ʈ��... �� ����Ʈ ������ ����.
	// �̰� �׳� ����Ʈ �������� �ѷ��ִ°��̴�.
	if(creatTalentEffectData.nType == BLOOD_EFFECT)
	{
		// ����.. �̰� �� �ƴϴ�. �Ф�(XModuleEffect�� ��� �� �ؾ�...)
		// �� ����Ʈ�� ���̴� �ŷ�Ʈ�ΰ�?
		vec3 vOtherDir = creatTalentEffectData.pOwner->GetDirection() * -1.0f;
		if(creatTalentEffectData.pAttacker != NULL)
			vOtherDir = creatTalentEffectData.pAttacker->GetDirection();

		if(creatTalentEffectData.pTalentInfo->m_bUseBloodEffect == true)
		{
			bool bCri = false;
			if (CheckBitSet(creatTalentEffectData.nDamageFlags, CTR_CRITICAL)) 
				bCri = true;

			CreateAttackBloodEffect(creatTalentEffectData.pOwner, creatTalentEffectData.effectPos, vOtherDir, bCri, creatTalentEffectData.pTalentInfo->m_fHitBloodScale, creatTalentEffectData.matEffect);					// ������Ʈ
		}
	}

	// ����Ʈ ����Ʈ �߻�
	if( vecEffectInfoByType.empty() || bModelEffect == false)
	{
		// 2008-12-10 ����Ʈ ����Ʈ ���� ����
		// �ƹ� ���� �� ����� �ȵž� �ִٸ�... ����Ʈ��
		CreateDefaultTalentEffect(creatTalentEffectData);
		return;
	}

	vecEffectInfoByType.clear();
}

void XTalentEffect::CreateDefaultTalentEffect( CREATE_TALENT_EFFECT_DATA& creatTalentEffectData )
{
	//�׽�Ʈ �ڵ� �ű�-�ʿ���� �����ֻ�.
	if(creatTalentEffectData.pOwner)
	{
		if(creatTalentEffectData.nType == ATTACK_EFFECT ||
			creatTalentEffectData.nType == ATTACK_SECONDARY_EFFECT)
		{
			return CreateAttackDefaultTalentEffect(creatTalentEffectData);
		}
		else if(creatTalentEffectData.nType == DEFENSE_EFFECT)
		{
			return CreateDefenseDefaultTalentEffect(creatTalentEffectData.pOwner, creatTalentEffectData.effectPos, creatTalentEffectData.pAttacker);
		}
	}
	return;
}

void XTalentEffect::CreateCastEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	EFFECT_POST_TRANSFORM postData;
	if(pInfo->pEffectInfo->m_nEffectDirection == TED_AUTO_DIR_ATTACK_EFFECT &&
		pInfo->matEffect != MMatrix::IDENTITY)
	{
		postData.matWorldMatrix = pInfo->matEffect;
	}
	else if(pInfo->effectPos != vec3::ZERO || 
			pInfo->effectDir != vec3::AXISY)
	{
		postData.matWorldMatrix.SetLocalMatrix(pInfo->effectPos, pInfo->effectDir, vec3::AXISZ);
	}

	if(pInfo->pAttacker && pInfo->pAttacker->GetUID() == XGetMyUID())
		postData.bAttackByMy = true;

	postData.nDamageFlags = pInfo->nDamageFlags;

	XEffectInvoker effectInvoker;
	effectInvoker.Invoke(pInfo->pOwner,pInfo->pEffectInfo->m_strEffectName, pInfo->pEffectInfo->m_strParameter, pInfo->pEffectInfo->m_strBoneName, &(pInfo->vecOutResultData), pInfo->pAttacker, &postData);
}

void XTalentEffect::CreateUseEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	CreateCastEffect(pInfo);
}

void XTalentEffect::CreateAttackEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	if (!pInfo->pOwner)
		return;

	// IMPACT_POS Ȯ��
	if(pInfo->pEffectInfo->m_nImpactPos == HIT_POS)
	{
		if(CreateEnchantHitEffect(pInfo) == false)
			CreateCastEffect(pInfo);
	}
	else if(pInfo->pEffectInfo->m_nImpactPos == BONE_POS)
	{
		pInfo->effectPos = vec3::ZERO;
		pInfo->effectDir = vec3::AXISY;
		pInfo->matEffect.MakeIdentity();

		if(CreateEnchantHitEffect(pInfo) == false)
			CreateCastEffect(pInfo);
	}
	else // ��
	{
		pInfo->effectPos = pInfo->pOwner->GetPosition();
		pInfo->effectDir = pInfo->pOwner->GetDirection();

		if(CreateEnchantHitEffect(pInfo) == false)
			CreateCastEffect(pInfo);
	}
}

void XTalentEffect::CreateMissEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	CreateCastEffect(pInfo);
}

void XTalentEffect::CreateShockEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	CreateCastEffect(pInfo);
}

void XTalentEffect::CreateDefenseEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	CreateAttackEffect(pInfo);
}

void XTalentEffect::CreateDurationEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	CreateCastEffect(pInfo);
}

void XTalentEffect::CreateAttackDefaultTalentEffect( CREATE_TALENT_EFFECT_DATA& creatTalentEffectData )
{
	if(creatTalentEffectData.pTalentInfo)
	{
		XTalentEffectBase * pEffectBase = NULL;

		if(creatTalentEffectData.pTalentInfo->m_nSkillType == ST_MELEE)
		{
			if(creatTalentEffectData.nType == ATTACK_SECONDARY_EFFECT)
				pEffectBase = info.talent_effect->GetTalentEffectBase(TEBY_MELEE_SECONDARY_ATTACK);
			else
				pEffectBase = info.talent_effect->GetTalentEffectBase(TEBY_MELEE_ATTACK);
		}
		else if(creatTalentEffectData.pTalentInfo->m_nSkillType == ST_MAGIC)
			pEffectBase = info.talent_effect->GetTalentEffectBase(TEBY_MAGIC_ATTACK);
		else if(creatTalentEffectData.pTalentInfo->m_nSkillType == ST_ARCHERY)
			pEffectBase = info.talent_effect->GetTalentEffectBase(TEBY_ARCHERY_ATTACK);
		else if(creatTalentEffectData.pTalentInfo->m_nSkillType == ST_GUN)
			pEffectBase = info.talent_effect->GetTalentEffectBase(TEBY_GUN_ATTACK);

		if(pEffectBase)
		{
			XTalentEffectInfo pEffectAttInfo;
			pEffectAttInfo.m_strEffectName	= pEffectBase->m_strEffectBaseFileName;
			pEffectAttInfo.m_strBoneName	= pEffectBase->m_strBoneName;

			pEffectAttInfo.m_nImpactPos		= pEffectBase->m_nImpactPos;

			//mlog("�ǰ� ����Ʈ��!!! %s\n", pEffectAttInfo.m_strEffectName.c_str());
			vec3 vDir = vec3(0.0f, 1.0f, 0.0f);
			if(creatTalentEffectData.pAttacker &&
				pEffectBase->m_nEffectDirection == TED_FRONT_EFFECT_USER)
			{
				vDir = creatTalentEffectData.pAttacker->GetDirection();
			}

			TALENT_EFFECT_INFO_DATA talentEffectInfoData;
			talentEffectInfoData.Set(&pEffectAttInfo, creatTalentEffectData.pOwner, creatTalentEffectData.effectPos, vDir, creatTalentEffectData.matEffect, creatTalentEffectData.pAttacker, creatTalentEffectData.nDamageFlags);
			return CreateAttackEffect(&talentEffectInfoData);
		}
	}
	else
	{
		// �ŷ�Ʈ�� ���ٸ�...
		XCharacter* pNode = creatTalentEffectData.pOwner->GetActor();
		if (pNode == NULL)
		{
			return;
		}

		global.emgr->AddXEffect(DEFAULT_EFFECT_FILE_NAME, pNode, NULL);
	}

	return;
}

void XTalentEffect::CreateDefenseDefaultTalentEffect(XObject* pObj, vec3 pos , XObject* pOtherObj /* = NULL */)
{
	if(pObj)
	{
		TALENT_EFFECT_BASE_TYPE effectType = TEBY_DEFENSE;

		if(pObj->GetEntityType() == ETID_NPC)
			effectType = TEBY_MONSTER_DEFENSE;
		else if(pObj->GetEntityType() == ETID_PLAYER)
		{
			XPlayer* player = static_cast<XPlayer*>(pObj);
			if(player->IsEquipShield() == false)
				effectType = TEBY_NOSHIELD_DEFENSE;
		}

		XTalentEffectBase * pEffectBase = info.talent_effect->GetTalentEffectBase(effectType);

		if(pEffectBase)
		{
			XTalentEffectInfo pEffectAttInfo;
			pEffectAttInfo.m_strEffectName	= pEffectBase->m_strEffectBaseFileName;
			pEffectAttInfo.m_strBoneName	= pEffectBase->m_strBoneName;

			pEffectAttInfo.m_nImpactPos		= pEffectBase->m_nImpactPos;

			//mlog("��� ����Ʈ��!!! %s\n", pEffectAttInfo.m_strEffectName.c_str());
			vec3 vDir = vec3(0.0f, 1.0f, 0.0f);
			if(pOtherObj &&
				pEffectBase->m_nEffectDirection == TED_FRONT_EFFECT_USER)
			{
				vDir = pOtherObj->GetDirection();
			}

			TALENT_EFFECT_INFO_DATA talentEffectInfoData;
			talentEffectInfoData.Set(&pEffectAttInfo, pObj, pos, vDir, MMatrix::IDENTITY, NULL, 0);
			return CreateDefenseEffect(&talentEffectInfoData);
		}
	}

	return;
}

vec3 XTalentEffect::GetTalentEffectDir( XTalentEffectInfo* pTalentEffectInfo, XObject* pObj_1, XObject* pObj_2 )
{
	vec3 vDir = vec3::AXISY;
	switch(pTalentEffectInfo->m_nEffectType)
	{
	case CAST_EFFECT:			// ĳ��Ʈ
	case USE_EFFECT:			// ���
		{
			//////////////////////////////////////////////////////////////////////////
			// by pok.
			// CreateFollowableEffect ���� ó�����ְ� ����
			//////////////////////////////////////////////////////////////////////////
			// pObj_1�� ������̰�, pObj_2�� �����̶��...
			//if(pObj_1 &&
			//	pTalentEffectInfo->m_nEffectDirection == TED_FRONT_EFFECT_USER)
			//{
			//	vDir = pObj_1->GetDirection();
			//}
		}
		break;
	case DEFENSE_EFFECT:
	case ATTACK_EFFECT:			// �ǰ�
		{
			// pObj_1�� �����̰�, pObj_2�� ����ڶ��...
			if(pObj_2 &&
				pTalentEffectInfo->m_nEffectDirection == TED_FRONT_EFFECT_USER)
			{
				vDir = pObj_2->GetDirection();
			}
		}
		break;
	case MISS_EFFECT:			// ����
	case SHOCK_EFFECT:			// ���
	case DURATION_EFFECT:		// ����
		break;
	}

	return vDir;
}

void XTalentEffect::CreateAttackBloodEffect( XObject* pObj, vec3 pos, vec3 dir, bool bCri, float fHitBloodScale, MMatrix matEffect )
{
	// �׿����� ��û�� ���� �� ���̴°� ���� �����Դϴ�.
	return;

	if(gg.game->Check18Over() == false)
		return;

	// ��Ƣ��� ����Ʈ �̸� ��������
	wchar_t chEffectName[64]	= {0,};
	if(pObj->GetEntityType() != ETID_NPC)
	{
		if(XCONST::HIT_BASE_BLOOD_EFFECT_NAME == tstring(L""))
			return;

		swprintf(chEffectName, L"%s_%02d", XCONST::HIT_BASE_BLOOD_EFFECT_NAME, MMath::RandomNumber(1, BLOOD_EFFECT_COUNT));

		return;
	}
	else
	{
		XNonPlayer* pNonObj		= AsNPC(pObj);
		if(pNonObj && pNonObj->GetInfo() && pNonObj->GetInfo()->strBloodEffectName.empty() == false)
		{
			swprintf(chEffectName, L"%s_%02d", pNonObj->GetInfo()->strBloodEffectName.c_str(), MMath::RandomNumber(1, BLOOD_EFFECT_COUNT));
		}
		else
		{
			// ������ �ȵǴٰ� �ؼ�...
			return;
			//// �������� �ȵŸ� ����Ʈ�� �����ش�.
			//sprintf(chEffectName, "ef_spread_blood_%02d", MMath::RandomNumber(1, 3));
		}
	}

	//------------------------------------------------------------------------
	// ��Ƣ��� ����Ʈ
	// ȸ�� ������
	int nxRotation	= MMath::RandomNumber(-90, -40);
	int nzRotation	= MMath::RandomNumber(-45, 45);
	vec3 vRanDir	= vec3(0.0f, -1.0f, 0.0f);
	vRanDir.Normalize();

	// ����� ���� ��ǥ ã��
	// ū NPC�� �Ѵ�.
	bool bNearBone			= false;
	vec3 vBloodPos			= pos;
	RActorNode * pNearBone	= NULL;
	if(pObj->GetModuleCollision() &&
		pObj->GetActor() &&
		pObj->GetActor()->GetRadius() >= 100.0f)
	{
		pObj->GetModuleCollision()->PickRayFindNearBone(pos, &pNearBone);

		if(pNearBone)
			bNearBone = true;
	}
	
	if(bNearBone == false)
	{
		// ���� NPC�� �߽� ������ �Ѵ�.
		pNearBone = pObj->GetActor()->GetActorNode("Bip01");
	}

	// ��Ʈ���� ����
	RMatrix matDir, matBaseFrame;

	matDir = matEffect;

	MMatrix matInverse;
	pObj->GetWorldTransform().GetInverse(&matInverse);
	//matInverse.SetTranslation(vec3::ZERO);

	// ����Ʈ ����	
	MUID uidEffect;
	uidEffect.SetZero();

	if(pNearBone)
	{
		matDir.SetTranslation(vBloodPos);
		if(matDir != MMatrix::IDENTITY)
			matDir *= matInverse;

		uidEffect = global.emgr->AddXEffect(chEffectName, pObj->GetActor(), NULL, matDir, false, true);
	}
	else
	{
		matBaseFrame.SetLocalMatrix(vBloodPos, dir, vec3::AXISZ);
		uidEffect = global.emgr->AddXEffect(chEffectName, NULL, NULL, matBaseFrame, false, false);
	}

	// ũ��Ƽ���� �ι�!!!
	if(uidEffect != MUID::ZERO)
	{
		if(bCri)
			global.emgr->SetScaleXEffect(uidEffect, false, XCONST::HIT_CRITICAL_BLOOD_EFFECT_SIZE);
		else if(fHitBloodScale > 1.0f)
			global.emgr->SetScaleXEffect(uidEffect, false, fHitBloodScale);
	}
	//#ifdef _DEBUG
		//REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugBoxAndAxis(matDir, 30, 7, D3DCOLOR_XRGB(255, 0, 0));
	//#endif

	return;
}

void XTalentEffect::ClearEffect()
{
	for(vector<EFFECT_DEL>::iterator it = m_vecDelEffectList.begin(); it != m_vecDelEffectList.end(); ++it)
	{
		global.emgr->DeleteXEffect((*it).uidEffect);
	}

	for(vector<MUID>::iterator itSound = m_vecDelSoundList.begin(); itSound != m_vecDelSoundList.end(); ++itSound)
	{
		global.sound->StopSound(*itSound);
	}

	m_vecDelEffectList.clear();

	talentEffectHitCapsule.ClearAllTalentHitCapsuleImpact();
}

void XTalentEffect::Update( float fDelta )
{
	PFC_THISFUNC;
	talentEffectHitCapsule.Update(fDelta);
}

void XTalentEffect::CheckAttackEffectSound( vector<EFFECT_RESULT_DATA>& vecEffectResult )
{
	for(vector<EFFECT_RESULT_DATA>::iterator it = vecEffectResult.begin(); it != vecEffectResult.end(); it++)
	{
		if(it->type == CET_SOUND)
			m_bHaveAttackSound = true;
	}
}

void XTalentEffect::CreateCastEndEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	CreateCastEffect(pInfo);
}

bool XTalentEffect::CreateEnchantHitEffect( TALENT_EFFECT_INFO_DATA* pInfo )
{
	if(pInfo->pAttacker == NULL || pInfo->pAttacker->GetEntityType() != ETID_PLAYER ||
		XEffectBaseInvoker::CheckReservedEffect(pInfo->pEffectInfo->m_strEffectName))			// ����Ȱ�... �ϸ� �ȵȴ�.
		return false;

	XPlayer* pPlayer = pInfo->pAttacker->AsPlayer();
	int nBuffID = pPlayer->GetVisualEnchantBuffID();

	// ���� ���� ��������
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return false;

	// ��ȭ ���� ����Ʈ ���� ��������
	XActor* pXActor = pInfo->pAttacker->AsActor();
	WEAPON_TYPE eRType = pXActor->GetEquipWeaponType(false);
	WEAPON_TYPE eLType = pXActor->GetEquipWeaponType(true);

	vector<XEnchantBuffEffectInfo *> vecEnchantEffectInfoByType;
	pBuffInfo->m_EnchantBuffEffect.GetEffectInfoList(EBET_HIT_EFFECT, eRType, eLType, PLAYER_PARTS_SLOT_RWEAPON, vecEnchantEffectInfoByType);

	for(vector<XEnchantBuffEffectInfo *>::iterator it = vecEnchantEffectInfoByType.begin(); it != vecEnchantEffectInfoByType.end(); ++it)
	{
		XTalentEffectInfo talentEffectInfo;
		talentEffectInfo.copyData(*pInfo->pEffectInfo);
		talentEffectInfo.m_strEffectName = (*it)->m_strEffectName;

		pInfo->pEffectInfo = &talentEffectInfo;
		CreateCastEffect(pInfo);
	}

	return (vecEnchantEffectInfoByType.size() > 0);
}