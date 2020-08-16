#include "StdAfx.h"
#include "GNPCMinion.h"
#include "GEntityNPC.h"
#include "GEntityPlayer.h"
#include "GField.h"
#include "GJobMgr.h"
#include "GSpawnManager.h"
#include "GGlobal.h"
#include "GPartySystem.h"
#include "GPlayerObjectManager.h"
#include "GParty.h"

GNPCMinion::GNPCMinion( GEntityNPC* pOwner )
: m_pOwner(pOwner)
{
}

GNPCMinion::~GNPCMinion(void)
{
}

GEntityNPC* GNPCMinion::Summon(int nNPCID, vec3 vStartPos)
{
	GField* pField = m_pOwner->GetField();
	VALID_RET(pField, NULL);

	SpawnDetailInfo infoSpawn;
	infoSpawn.uidParent = m_pOwner->GetUID();

	GEntityNPC* pSpawnedNPC = pField->GetSpawn()->Spawn(nNPCID, vStartPos, vec3(0,1,0), 0.0f, 0.0f, &infoSpawn);
	VALID_RET(pSpawnedNPC, NULL)

	m_vecMinions.push_back(pSpawnedNPC->GetUID());

	SetTargetMasterEnemy(pSpawnedNPC);

	return pSpawnedNPC;
}

void GNPCMinion::OnDeadMinion( MUID uidMinion )
{
	for (vector<MUID>::iterator it = m_vecMinions.begin();
		 it != m_vecMinions.end();
		 ++it)
	{
		if (*it == uidMinion)
		{
			m_vecMinions.erase(it);
			break;
		}
	}
}

void GNPCMinion::OnDeadMaster()
{
	KillAllMinions();
}

void GNPCMinion::OnDead()
{
	GField* pField = m_pOwner->GetField();
	if (!pField)
		return;

	if (HasMaster())
	{
		GEntityNPC* pMasterNPC = pField->FindNPC(m_uidMaster);
		if (!pMasterNPC)
			return;
		pMasterNPC->GetNPCMinion().OnDeadMinion(m_pOwner->GetUID());
	}
	else
	{
		OnDeadMaster();
	}
}

void GNPCMinion::OnEndCombat()
{
	DespawnAllMinions();
}

void GNPCMinion::KillAllMinions()
{
	if (m_vecMinions.empty())
		return;

	GField* pField = m_pOwner->GetField();
	VALID(pField);


	GVectorMUID vecMinions;
	vecMinions.Vector() = m_vecMinions;
	
	for each (const MUID& each in vecMinions.Vector())
	{
		GEntityNPC* pEachNPC = pField->FindNPC(each);
		if (!pEachNPC)
			continue;
		if (pEachNPC->IsDead())
			continue;

		pEachNPC->doDie();
	}

	m_vecMinions.clear();
}

void GNPCMinion::DespawnAllMinions()
{
	if (m_vecMinions.empty())
		return;

	GField* pField = m_pOwner->GetField();
	VALID(pField);

	GVectorMUID vecMinions;
	vecMinions.Vector() = m_vecMinions;
	
	for each (const MUID& each in vecMinions.Vector())
	{
		GEntityNPC* pEachNPC = pField->FindNPC(each);
		if (!pEachNPC)
			continue;
		if (pEachNPC->IsDead())
			continue;

		pEachNPC->doDespawn(false);
	}

	m_vecMinions.clear();
}

void GNPCMinion::SetTargetMasterEnemy( GEntityNPC* pMinion )
{
	VALID(pMinion);
	GEntityActor* pTarget = m_pOwner->GetTargetActor();
	if (!pTarget)
		return;

	// ���� Ÿ�� �ɶ� ���� �������� �ʰ� ����
	pMinion->GetJobMgr()->EnableClearJob(false);

	// �÷��̾ �ƴ� ���� ��ǥ�� �ٷ� ����
	if (!pTarget->IsPlayer())
	{
		pMinion->doAttack(pTarget->GetUID());
		return;
	}
	else
	{
		GEntityPlayer* pTargetPlayer = ToEntityPlayer(pTarget);		
		VALID(pTargetPlayer);
		GEntityPlayer* pLeastDistanceTarget = pTargetPlayer;

		MUID uidParty = pTargetPlayer->GetPartyUID();
		GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
		
		if (pParty != NULL)
		{			
			for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
			{
				MUID uidMember = it->first;
				GEntityPlayer* pMember = gmgr.pPlayerObjectManager->GetEntity(uidMember);
				if (pMember == NULL)	continue;

				if (pMember->GetPos().DistanceToSq(pMinion->GetPos()) <
					pLeastDistanceTarget->GetPos().DistanceToSq(pMinion->GetPos()))
				{
					// �� ����� ��Ƽ���� ������ �� ��븦 ��ǥ�� ����
					pLeastDistanceTarget = pMember;
				}
			}
		}

		// ���� ����� ���� ����
		pMinion->doAttack(pLeastDistanceTarget->GetUID());
	}

	pMinion->GetJobMgr()->EnableClearJob(true);	// ����
}

GEntityNPC* GNPCMinion::GetSummoner() const
{
	if (m_uidMaster.IsInvalid())
		return NULL;

	return m_pOwner->FindNPC(m_uidMaster);
}