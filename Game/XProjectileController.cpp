#include "stdafx.h"
#include "XProjectileController.h"
#include "XProjectile.h"
#include "XModuleCollision.h"
#include "XModuleEffect.h"

XProjectileController::XProjectileController()
{

}

XProjectileController::~XProjectileController()
{

}

void XProjectileController::ProjectileHit( XObject* pTarget, XObject* pAttacker, XTalentInfo* pTalentInfo )
{
	ProjectileColJudgment(pTarget, pAttacker, pTalentInfo, PCJT_HIT);
}

void XProjectileController::ProjectileDefence( XObject* pTarget, XObject* pAttacker, XTalentInfo* pTalentInfo )
{
	ProjectileColJudgment(pTarget, pAttacker, pTalentInfo, PCJT_DEFENCE);
}

void XProjectileController::ProjectileColJudgment( XObject* pTarget, XObject* pAttacker, XTalentInfo* pTalentInfo, ProjectileColJudgmentType colType )
{
	if(pTarget == NULL || pAttacker == NULL || pTalentInfo == NULL)
	{
		return;
	}

	m_pOwner = pTarget;

	// ���� ���õ� �߻�ü�� ã�´�.
	vector<XProjectile*> vecProjectile;
	GetMyProjectile(pAttacker, pTalentInfo, vecProjectile);

	if(vecProjectile.empty())
	{
		// ����? �ٵ�... �����̳�...
		// �׷��� ���� ����� ��������.
		ShowEmptyProjrctileEffect(pTarget, pAttacker, pTalentInfo, colType);
		return;
	}

	// �浹�� �߻�ü�� ã�´�.
	XProjectile* pProjectile = GetEndProjectile(pTarget, pTalentInfo, vecProjectile);

	// �浹��������� �˸���.
	if(pProjectile)
		pProjectile->ProjectileColResult(colType, m_pOwner);
	else
		ShowEmptyProjrctileEffect(pTarget, pAttacker, pTalentInfo, colType);
}

void XProjectileController::GetMyProjectile( XObject* pAttacker, XTalentInfo* pTalentInfo, vector<XProjectile*>& outvecMyProjectile )
{
	XObjectMap* pObjectMap = gg.omgr->GetObjectMap(XOBJ_PROJECTILE);
	if(pObjectMap == NULL)
		return;

	for(XObjectMap::iterator itObject = pObjectMap->begin(); itObject != pObjectMap->end(); itObject++)
	{
		XProjectile* pProjectile = static_cast<XProjectile*>((*itObject).second);
		if(pProjectile->IsEqualProjectile(pAttacker->GetUID(), pTalentInfo->m_nID))
		{
			outvecMyProjectile.push_back(pProjectile);
		}
	}
}

XProjectile* XProjectileController::GetEndProjectile( XObject* pTarget, XTalentInfo* pTalentInfo, vector<XProjectile*>& vecMyProjectile )
{
	for(vector<XProjectile*>::iterator itProjectile = vecMyProjectile.begin(); itProjectile != vecMyProjectile.end(); itProjectile++)
	{
		XProjectile* pProjectile = (*itProjectile);
		if(pProjectile->IsWaitColJudgment())
		{
			// �������.
			// �浹 Ÿ���� ���� Ÿ�ٰ� ����?
			if(pProjectile->GetColObject() == pTarget)
			{
				return pProjectile;
			}
		}
	}

	// �浹�� �ȵǾ��ٸ�... �������� ã�´�.
	return GetNearProjectile(vecMyProjectile, pTalentInfo);
}

XProjectile* XProjectileController::GetNearProjectile( vector<XProjectile*>& vecMyProjectile, XTalentInfo* pTalentInfo )
{
	XProjectile* pNearProjectile = NULL;
	float fMinLen = FLT_MAX;

	for(vector<XProjectile*>::iterator itProjectile = vecMyProjectile.begin(); itProjectile != vecMyProjectile.end(); itProjectile++)
	{
		XProjectile* pProjectile = (*itProjectile);
		float fLen = (m_pOwner->GetPosition() - pProjectile->GetPosition()).Length();
		if(fMinLen > fLen &&
			pTalentInfo->m_nID == pProjectile->GetTalentInfo()->m_nID)			// �߻�ü�� ����� �ŷ�Ʈ�� ���ƾ� �Ѵ�.
		{
			fMinLen = fLen;
			pNearProjectile = pProjectile;
		}
	}

	return pNearProjectile;
}

void XProjectileController::ShowEmptyProjrctileEffect( XObject* pTarget, XObject* pAttacker, XTalentInfo* pTalentInfo, ProjectileColJudgmentType colType )
{
	// �߻�ü�� ����� ����Ʈ�� ��������.
	CSProjectileInfo* pProjectileInfo = GetProjectileInfo(pTalentInfo);
	if(pProjectileInfo == NULL)
		return;

	// �߻�ü�� ��Ʈĸ���̶�� ���� ����Ʈ�� �ʿ������? �ϴ� �ǵ�����.
	if(pProjectileInfo->m_nType == TPT_HITCAPSULE_GUIDED)
		return;

	// ������Ʈ �浹�� �˻��ؼ� ĸ���� �´� ��ġ�� �����´�.
	XModuleCollision *pModuleCollision = pTarget->GetModuleCollision();
	if(pModuleCollision == NULL)
		return;

	XModuleEffect* pModuleEffect = pTarget->GetModuleEffect();
	if(pModuleEffect == NULL)
		return;

	XHitDamageEffectEventData * pEventData = new XHitDamageEffectEventData;
	if(pAttacker)	pEventData->m_uidAttacker	= pAttacker->GetUID();
	if(pTarget)		pEventData->m_uidTarget		= pTarget->GetUID();
	pEventData->m_pProjectileInfo = pProjectileInfo;
	pEventData->m_eColType		= colType;
	pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_SHOW_PROJECTIL_DAMAGE, pEventData);
}

CSProjectileInfo* XProjectileController::GetProjectileInfo( XTalentInfo* pTalentInfo )
{
	for(vector<CSTalentEventInfo>::iterator itNor = pTalentInfo->m_Events.m_vecEvents.begin(); itNor != pTalentInfo->m_Events.m_vecEvents.end(); itNor++)
	{
		if((*itNor).m_nEvent == TE_LAUNCH_PROJECTILE)
		{
			return &((*itNor).m_ProjectileInfo);
		}
	}

	for(vector<CSTalentEventInfo>::iterator itAct = pTalentInfo->m_Events.m_vecActEvents.begin(); itAct != pTalentInfo->m_Events.m_vecActEvents.end(); itAct++)
	{
		if((*itAct).m_nEvent == TE_LAUNCH_PROJECTILE)
		{
			return &((*itAct).m_ProjectileInfo);
		}
	}

	return NULL;
}