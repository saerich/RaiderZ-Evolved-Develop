#include "StdAfx.h"
#include "GTalentHitcapsule.h"
#include "GEntityNPC.h"
#include "GTalentInfo.h"
#include "GTalent.h"
#include "GTalentHitcapsule.h"
#include "GNPCHitcapsuleGroup.h"

//////////////////////////////////////////////////////////////////////////
//
//	GTalentHitcapsule
//
//////////////////////////////////////////////////////////////////////////

void GTalentHitcapsule::ChangeCapsuleGroup_ByTalent( int nHitCapsuleGroupIndex, GEntityActor* pActor )
{
	PFI_B(3110, "GTalentHitcapsule::ChangeCapsuleGroup_ByTalent");

	VALID(pActor);
	if (!pActor->IsNPC())				return;	// NPC�� �ش�
	if (nHitCapsuleGroupIndex == NONE_HITCAPSULEGROUP_ID)	return;	// �ŷ�Ʈ�� ��ȭ ��尡 �ִ� ��츸 �ش�

	GEntityNPC* pNPC = ToEntityNPC(pActor);
	pNPC->GetNPCHitcapsuleGroup().ChangeCapsuleGroup_ByTalent(nHitCapsuleGroupIndex);

	PFI_E(3110);
}

void GTalentHitcapsule::OnStart( GTalent* pTalent )
{
	PFI_B(3120, "GTalentHitcapsule::OnStart");

	VALID(pTalent);
	VALID(pTalent->GetInfo());
	ChangeCapsuleGroup_ByTalent(pTalent->GetInfo()->m_nHitCapsuleGroupIndex, pTalent->GetOwner());

	PFI_E(3120);
}

void GTalentHitcapsule::OnExit( GTalent* pTalent )
{
	PFI_B(3130, "GTalentHitcapsule::OnExit");

	// �ŷ�Ʈ�� �������� ���� �ǵ���
	VALID(pTalent);
	VALID(pTalent->GetInfo());
	GEntityActor* pActor = pTalent->GetOwner();
	VALID(pActor);
	if (!pActor->IsNPC())				return;	// NPC�� �ش�
	if (pTalent->GetInfo()->m_nHitCapsuleGroupIndex == NONE_HITCAPSULEGROUP_ID)	return;	// �ŷ�Ʈ�� ��ȭ ��尡 �ִ� ��츸 �ش�

	GEntityNPC* pNPC = ToEntityNPC(pActor);
	pNPC->GetNPCHitcapsuleGroup().ChangeCapsuleGroup_ByTalentComplete();

	PFI_E(3130);
}

void GTalentHitcapsule::OnEnterPhaseExtra( GTalent* pTalent )
{
	PFI_B(3140, "GTalentHitcapsule::OnEnterPhaseExtra");

	VALID(pTalent);
	VALID(pTalent->GetInfo());
	ChangeCapsuleGroup_ByTalent(pTalent->GetInfo()->m_nExtraHitCapsuleGroup, pTalent->GetOwner());

	PFI_E(3140);
}

void GTalentHitcapsule::OnEnterPhaseExtra2( GTalent* pTalent )
{
	PFI_B(3150, "GTalentHitcapsule::OnEnterPhaseExtra2");

	VALID(pTalent);
	VALID(pTalent->GetInfo());
	ChangeCapsuleGroup_ByTalent(pTalent->GetInfo()->m_nExtraHitCapsuleGroup2, pTalent->GetOwner());

	PFI_E(3150);
}

void GTalentHitcapsule::OnEnterPhaseExtra3( GTalent* pTalent )
{
	PFI_B(3160, "GTalentHitcapsule::OnEnterPhaseExtra3");

	VALID(pTalent);
	VALID(pTalent->GetInfo());
	ChangeCapsuleGroup_ByTalent(pTalent->GetInfo()->m_nExtraHitCapsuleGroup3, pTalent->GetOwner());

	PFI_E(3160);
}
