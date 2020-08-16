#include "StdAfx.h"
#include "GEffecteeCalculator.h"
#include "GEntityPlayer.h"
#include "GDuel.h"
#include "GParty.h"

GEffecteeCalculator::GEffecteeCalculator(void)
{
}

GEffecteeCalculator::~GEffecteeCalculator(void)
{
}

vector<GEntityActor*> GEffecteeCalculator::Calculate(GField* pField, 
													 GEntityActor* pUser, 
													 TALENT_TARGET nTargetType, 
													 GEntityActor* pTargetActor, 
													 vec3 vPoint, 
													 const float fRadius, 
													 GEntityActor* pAttacker/*=NULL*/, 
													 GEntityActor* pDefender/*=NULL*/ ) const
{
	vector<GEntityActor*> vecEffectees;
	if (!pField)		return vecEffectees; 
	VALID_RET(pUser, vecEffectees);
	VALID_RET(fRadius >= 0.0f, vecEffectees);

	
	if (pTargetActor)
	{
		// �߽� ���Ͱ� �ִٸ� ��ǥ�� ȿ���������� ������
		vPoint = pTargetActor->GetPos();
	}
	else if (vPoint == vec3::ZERO)
	{
		// �߽� ���͵� ���� ��ǥ�� �����Ǿ� ���� �ʴٸ� ����ڸ� ȿ���������� ������
		vPoint = pUser->GetPos();
	}

	// ���� ��� ó��
	GEntityActor* pDuelOpponent = FindValidDuelOpponent(pUser, nTargetType, vPoint, fRadius);
	if (pDuelOpponent)
		vecEffectees.push_back(pDuelOpponent);

	// ����� ã��
	ENTITY_TYPEID nFindActorType = GetFindActorType(pUser, nTargetType);

	switch(nTargetType)
	{
	case TALENT_TARGET_AREA_ENEMY:
	case TALENT_TARGET_AREA_ALLIED:
	case TALENT_TARGET_AREA_ALL:
		{
			OnTargetArea(pField, pUser, nFindActorType, vPoint, fRadius, vecEffectees);			
		}break;
	case TALENT_TARGET_SELF:			OnTargetSelf(pUser, vecEffectees);				break;
	case TALENT_TARGET_PARTY:			OnTargetParty(pField, pUser, pUser, fRadius, vecEffectees);		break;
	case TALENT_TARGET_PARTY_TARGET:	OnTargetParty(pField, pUser, pTargetActor, fRadius, vecEffectees);		break;
	case TALENT_TARGET_TARGET:			OnTargetVictim(pTargetActor, vecEffectees);		break;
	case TALENT_TARGET_ATTACKER:		OnTargetAttacker(pAttacker, vecEffectees);		break;
	case TALENT_TARGET_DEFENDER:		OnTargetDefender(pDefender, vecEffectees);		break;
	}

	return vecEffectees;
}

ENTITY_TYPEID 
GEffecteeCalculator::GetFindActorType(GEntityActor* pUser, TALENT_TARGET nTargetType) const
{
	ENTITY_TYPEID nTypeID = ETID_ACTOR;
	if (pUser->IsPlayer())
	{
		if (nTargetType == TALENT_TARGET_AREA_ENEMY)			nTypeID = ETID_NPC;
		else if (nTargetType == TALENT_TARGET_AREA_ALLIED)		nTypeID = ETID_PLAYER;
	}
	else if (pUser->IsNPC())
	{
		if (nTargetType == TALENT_TARGET_AREA_ENEMY)			nTypeID = ETID_PLAYER;
		else if (nTargetType == TALENT_TARGET_AREA_ALLIED)		nTypeID = ETID_NPC;
	}

	return nTypeID;
}

bool 
GEffecteeCalculator::IsValidTarget( GEntityActor* pTarget ) const
{
	if (!pTarget)				return false;
	if (pTarget->IsDead())		return false;
	return true;
}

bool 
GEffecteeCalculator::IsValidDistance( vec3 vPoint, 
								 GEntityActor* pTarget, 
								 const float fEffectRadius ) const
{
	float fDistance = vPoint.DistanceTo(pTarget->GetPos());
	return  (fDistance <= fEffectRadius);
}

bool GEffecteeCalculator::IsSameField( GEntityActor* pUser, GEntityActor* pTarget ) const
{
	return (pUser->GetFieldUID() == pTarget->GetFieldUID());
}

bool GEffecteeCalculator::IsAlreadyInserted(const vector<GEntityActor*> &vecEffectees, 
											GEntityActor* pTarget ) const
{
	return (find(vecEffectees.begin(), vecEffectees.end(), pTarget) != vecEffectees.end());
}

GEntityActor*
GEffecteeCalculator::FindValidDuelOpponent(GEntityActor* pUser, 
									  TALENT_TARGET nTargetType,
									  vec3 vPoint,
									  const float fRadius) const
{
	GEntityPlayer* pUsePlayer = ToEntityPlayer(pUser);
	if (!pUsePlayer)				return NULL;
	if (!pUsePlayer->HasDuel())		return NULL;

	// �������� ȿ���� �ƴ϶�� ����
	if (nTargetType != TALENT_TARGET_AREA_ALL &&
		nTargetType != TALENT_TARGET_AREA_ENEMY)			
	{
		return NULL;
	}

	GDuel* pDuel = pUsePlayer->GetDuel();
	if (!pDuel)												
		return NULL;

	GEntityPlayer* pDuelOpponent = pDuel->GetOpponent(pUsePlayer);
	if (!pDuelOpponent)										
		return NULL;

	if (!IsValidTarget(pDuelOpponent))						return NULL;
	if (!IsValidDistance(vPoint, pDuelOpponent, fRadius))	return NULL;

	return pDuelOpponent;
}

void GEffecteeCalculator::OnTargetArea(GField* pField, 
									   GEntityActor* pUser,
									   ENTITY_TYPEID nFindActorType, 
									   vec3 vPoint, 
									   const float fRadius,
									   vector<GEntityActor*> &vecEffectees) const
{
	if (vPoint == vec3::ZERO) 
		return; // ��ǥ�� �����Ǿ��־���Ѵ�.
	VALID(fRadius >= 0.0f);		// ���� ������ �־�ߵȴ�.

	// ������ 0�̸� �߽��ڸ� ���õ�
	if (fRadius == 0.0f)
	{
		VALID(pUser);
		vecEffectees.push_back(pUser);
		return;
	}

	GSector* ppSectors[NEIGHBOR_SECTORS_NUM] = {NULL, }; // �Ϲ� ����+���� ����
	MPoint ptSector = pField->GetSectorIndex(vPoint.x, vPoint.y, false);
	pField->GetNeighborSectors(ptSector.x, ptSector.y, ppSectors);

	for (int i = 0; i < NEIGHBOR_SECTORS_NUM; i++)
	{
		GSector* pSector = ppSectors[i];
		if (!pSector)	continue;

		for (GSector::GSectorEntitySet::iterator itor = pSector->ItorEntityBegin(nFindActorType);
			itor != pSector->ItorEntityEnd(nFindActorType);
			++itor)
		{
			MUID& uid = (*itor);
			GEntityActor* pTarget = pField->FindActor(uid);

			if (!IsValidTarget(pTarget))					continue;
			if (!IsValidDistance(vPoint, pTarget, fRadius))	continue;
			if (IsAlreadyInserted(vecEffectees, pTarget))	continue;

			vecEffectees.push_back(pTarget);
		}
	}
}

void GEffecteeCalculator::OnTargetSelf(GEntityActor* pUser, vector<GEntityActor*> &vecEffectees) const
{
	vecEffectees.push_back(pUser);
}

void GEffecteeCalculator::OnTargetParty(GField* pField, 
										GEntityActor* pUser, 
										GEntityActor* pCenterActor, 
										const float fRadius,
										vector<GEntityActor*> &vecEffectees) const
{
	// �߽� ���Ͱ� ���ٸ� �ڱ� �ڽ��� ��Ƽ�� �����Ѵ�
	GEntityActor* pTarget = pCenterActor?pCenterActor:pUser;

	// ��Ƽ�� �÷��̾ �����ϴ�.
	VALID(pTarget->IsPlayer());	

	GEntityPlayer* pTargetPlayer = ToEntityPlayer(pTarget);
	
	if (!pTargetPlayer->HasParty() || !pTargetPlayer->GetParty() ||	// ��Ƽ�� ������ �ڱ� �ڽŸ� ȿ���� ����
		fRadius == 0.0f	// ������ 0�̸� �߽��ڸ� ���õ�
		)
	{
		vecEffectees.push_back(pTargetPlayer);
		return;
	}

	vector<MUID> vecPartyMembers = pTargetPlayer->GetParty()->GetPartyMembers();
	for each (MUID uidMember in vecPartyMembers)
	{
		GEntityPlayer* pMember = pField->FindPlayer(uidMember);

		if (!IsValidTarget(pMember))							continue;
		if (!IsSameField(pTarget, pMember))						continue;
		if (!IsValidDistance(pTargetPlayer->GetPos(), pMember, fRadius))	continue;

		vecEffectees.push_back(pMember);
	}
}

void GEffecteeCalculator::OnTargetVictim(GEntityActor* pTargetActor, 
										 vector<GEntityActor*> &vecEffectees ) const
{
	if (!IsValidTarget(pTargetActor))			return;
	vecEffectees.push_back(pTargetActor);
}

void GEffecteeCalculator::OnTargetAttacker(GEntityActor* pAttacker, 
										 vector<GEntityActor*> &vecEffectees ) const
{
	if (!IsValidTarget(pAttacker))			return;
	vecEffectees.push_back(pAttacker);
}

void GEffecteeCalculator::OnTargetDefender(GEntityActor* pDefender, 
										   vector<GEntityActor*> &vecEffectees ) const
{
	if (!IsValidTarget(pDefender))			return;
	vecEffectees.push_back(pDefender);
}

