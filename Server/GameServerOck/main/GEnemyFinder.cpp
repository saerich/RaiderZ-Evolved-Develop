#include "stdafx.h"
#include "GEnemyFinder.h"
#include "GEntityPlayer.h"
#include "GEntityNPC.h"
#include "GModuleAI.h"
#include "GMessage.h"
#include "GMessageID.h"
#include "GInteractionSystem.h"
#include "GConfig.h"
#include "GGlobal.h"
#include "GNPCInfoMgr.h"
#include "GHateTable.h"
#include "GAIRoomMgr.h"
#include "GRelationChecker.h"
#include "GActorInvisibility.h"
#include "GConst.h"
#include "GNPCMove.h"
#include "GNPCFirstStrike.h"
#include "GFieldNPCSession.h"
#include "GJobMgr.h"

GEnemyFinder::GEnemyFinder()
: m_bCheckInvisibility(false)
{
	m_rgrCheckInvisibility.SetTickTime(GConst::INVISIBILITY_DETECT_TICK);
	m_rgrCheckInvisibility.Start();
}

void GEnemyFinder::Update( float fDelta )
{
	PFC_THISFUNC;
	if (m_rgrCheckInvisibility.IsReady(fDelta))
	{
		m_bCheckInvisibility = true;
		m_rgrCheckInvisibility.Stop();
	}
}

MUID GEnemyFinder::Find(GEntityNPC* pOwnerNPC)
{
	PFC_THISFUNC;
	VALID_RET(pOwnerNPC, MUID::Invalid());

	if (pOwnerNPC->IsDead())
		return MUID::Invalid(); // �׾�����

	GFieldNPCSession* pSession = 
		pOwnerNPC->GetSession();
	if (pSession)
	{
		if (!pSession->IsFirstStrike())
			return MUID::Invalid(); // ���ǿ� ���� �������� ����
	}

	if (!pOwnerNPC->GetNPCFirstStrike().IsEnable())
		return MUID::Invalid(); // �������� �� �� ����

	if (pOwnerNPC->GetNPCMove().IsRetreat())		
		return MUID::Invalid(); // ����ġ�� ��

	if (pOwnerNPC->GetNPCMove().IsReturnToMove())		
		return MUID::Invalid(); // ������ ���ư��� ��

	if (pOwnerNPC->GetAAType() == NAAT_NONE)		
	{
		return MUID::Invalid(); // �������� ����
	}

	MUID uidTarget = pOwnerNPC->GetTarget();	
	if (uidTarget.IsValid())	
		return MUID::Invalid(); // �������� ��밡 ����

	GField* pField = pOwnerNPC->GetField();
	if (!pField)								
		return MUID::Invalid(); // �ʵ尡 ����


	// ã�� ��ƼƼ ���� ����
	ENTITY_TYPEID nFindType = ETID_ENTITY;
	if (CONFIG_AUTO_TEST_AI == GConfig::m_strAutoTestType)
	{
		nFindType = ETID_NPC;
	}
	else
	{
		if (pOwnerNPC->GetNPCInfo()->HasFaction())
		{
			nFindType = ETID_ACTOR;
		}
		else
		{
			nFindType = ETID_PLAYER;
		}
	}

	MUID uidRet = _Find(pOwnerNPC, pField, nFindType);

	if (m_bCheckInvisibility)
	{
		// �̹� ƽ�� ����ü Ž���� ������ �ٽ� ���
		m_bCheckInvisibility = false;
		m_rgrCheckInvisibility.Start();
	}

	return uidRet;
}


MUID GEnemyFinder::_Find(GEntityNPC* pOwnerNPC, GField* pField, const ENTITY_TYPEID nEnemyEntityTypeID)
{
	PFI_BLOCK_THISFUNC(311);
	
	if (!pOwnerNPC)		return MUID::Invalid();
	if (!pField)		return MUID::Invalid();

	GFieldGrid::EntitySelector* grid_selector = 
		pField->GetGrid().GetEntitySelector();

	PFI_B(9004, "GEnemyFinder::_Find-GFieldGrid::EntitySelector::GetEntities");
	class CollectEntities : public GFieldGrid::Sector::IVisitor
	{
	public:
		CollectEntities(vector<MUID>& entities) : m_entities(entities) {}

	private:
		void OnVisit(const MUID& uid) override
		{
			m_entities.push_back(uid);
		}

	private:
		vector<MUID>& m_entities;
	};
	GVectorMUID vecEntities;
	grid_selector->VisitByRadius(CollectEntities(vecEntities.Vector()), nEnemyEntityTypeID, pOwnerNPC->GetPos(), (float)pOwnerNPC->GetMaxSightRange());

	PFI_E(9004);
	PFI_B(9005, "GEnemyFinder::PickupEnemy");
	MUID nEnemyUID = PickupEnemy(pOwnerNPC, pField, vecEntities.Vector(), nEnemyEntityTypeID);
	PFI_E(9005);
	if (nEnemyUID.IsValid())
	{
		return nEnemyUID;
	}

	return MUID::Invalid();
}

MUID GEnemyFinder::PickupEnemy(GEntityNPC* pOwnerNPC, GField* pField, vector<MUID> vecEntities, const ENTITY_TYPEID nEnemyEntityTypeID )
{
	if (vecEntities.empty())
		return MUID::Invalid(); // ���ͳ� Entity�� ����

	// ���ͳ� Entity�� ������ ����
	std::random_shuffle( vecEntities.begin(), vecEntities.end() ); 
	MUID uidEnemy;
	for each (const MUID& each in vecEntities)
	{
		GEntity* pEach = pField->FindEntity(each);
		if (!pEach)
			continue;

		if (pEach->IsDead())
			continue;

		if (pEach == pOwnerNPC)
			continue;

		if (!CheckSight(pOwnerNPC, pEach)) 
			continue;

		if (!CheckFaction(pOwnerNPC, pEach)) 
			continue;

		if (!CheckPicking(pOwnerNPC, pEach)) 
			continue;

		if (!CheckHeight(pOwnerNPC, pEach, pField))
			continue;

		uidEnemy = pEach->GetUID();
		break;
	}
	
	return uidEnemy;
}

bool GEnemyFinder::CheckSight(GEntityNPC* pOwnerNPC, GEntity* pEnemyEntity)
{
	int nEnemyLevel = 0;
	if (pEnemyEntity->IsActor())
	{
		GEntityActor* pEnemyActor = ToEntityActor(pEnemyEntity);
		if (pEnemyActor->IsNowInvisibilityToNPC())
		{
			return false;
		}

		if (pEnemyActor->IsNowInvisibility())
		{
			if (pEnemyActor->IsPlayer())
			{
				GEntityPlayer* pPlayer = ToEntityPlayer(pEnemyActor);
				if (pPlayer->IsNowGhost())
					return false;
			}

			if (m_bCheckInvisibility && // ����ü Ž��ƽ�� �ƴ��� ����
				pEnemyActor->GetActorInvisibility().IsInSight(pOwnerNPC))
			{
				if (pEnemyActor->GetActorInvisibility().CheckDetectChance(pOwnerNPC))
					return true;
			}

			return false;
		}

		nEnemyLevel = pEnemyActor->GetLevel();
	}

	// Entity�� �þ߿� �Ⱥ��̸� ����
	vec3 vOwnerPos = pOwnerNPC->GetPos();
	vec3 vEnemyPos = pEnemyEntity->GetPos();
	float fDistance = vOwnerPos.DistanceTo(vEnemyPos);

	if ((float)pOwnerNPC->GetSightRange(nEnemyLevel) <= fDistance)
	{
		return false;
	}

	return true;
}

bool GEnemyFinder::CheckFaction(GEntityNPC* pOwnerNPC, GEntity* pEnemyEntity)
{
	if (!pEnemyEntity->IsActor()) 
		return true;

	GRelationChecker relationChecker;
	return relationChecker.IsEnemy(pOwnerNPC, ToEntityActor(pEnemyEntity));
}

bool GEnemyFinder::CheckPicking(GEntityNPC* pOwnerNPC, GEntity* pEnemyEntity)
{
	if (!pEnemyEntity->IsActor()) 
		return true;

	// ���� ��ŷ (�Ⱥ��̴� ������ ������ ����)
	vec3 vEndPos = pEnemyEntity->GetPos();
	vEndPos.z += ToEntityActor(pEnemyEntity)->GetColRadius(); // �浹�� ��ŭ ���̸� �÷���
	return gmgr.pAIRoomMgr->IsValid(pOwnerNPC->GetGroupID(), vEndPos);
}

bool GEnemyFinder::CheckHeight(GEntityNPC* pOwnerNPC, GEntity* pEnemyEntity, GField* pField)
{
	vec3 vOwnerPos = pOwnerNPC->GetPos();
	vec3 vEnemyPos = pEnemyEntity->GetPos();
	
	const GFieldInfo* pFieldInfo = pField->GetInfo();	
	VALID_RET(pFieldInfo != NULL, true);

	return pFieldInfo->IsAllowSectorHeightLimit(vOwnerPos.z - vEnemyPos.z);
}
