#include "StdAfx.h"
#include "GLootEnder.h"
#include "CCommandTable.h"
#include "GCommand.h"
#include "GEntityPlayer.h"
#include "GPlayerLoot.h"
#include "GEntityNPC.h"
#include "GNPCLoot.h"
#include "GGlobal.h"
#include "GPlayerObjectManager.h"
#include "GLootSystem.h"
#include "GDropList.h"
#include "GLootModifyHandler.h"
#include "GDropItems.h"

bool GLootEnder::End(GEntityPlayer* pPlayer)
{
	VALID_RET(pPlayer, false);

	GEntityNPC* pNPC = pPlayer->GetPlayerLoot().GetLootingNPC();
	if (NULL != pNPC)
	{
		pNPC->GetNPCLoot().End(pPlayer->GetCID());		
		RemoveGarbageDropItem(pPlayer, pNPC);
	}

	pPlayer->GetPlayerLoot().EndLoot();

	MCommand* pNewCmd = MakeNewCommand(MC_LOOT_END, 1, NEW_UID(pPlayer->GetUID()));

	pPlayer->RouteToThisCell(pNewCmd);

	return true;
}

bool GLootEnder::End(GEntityNPC* pNPC)
{
	VALID_RET(pNPC, false);

	set<int> setLootingPlayerCID = pNPC->GetNPCLoot().GetLootingPlayerCID();
	for each (int nLootingPlayerCID in setLootingPlayerCID)
	{
		GEntityPlayer* pLootingPlayer = gmgr.pPlayerObjectManager->GetEntity(nLootingPlayerCID);
		if (NULL == pLootingPlayer) continue;

		End(pLootingPlayer);
	}

	return true;
}

// ���ͷ��� ������ NPC�� �׿��� �����Ű�� �ʱ� ������ �������� �����ʰ� ������ �����ϸ� NPC�� ������ �������� ������ �ִ�.
// �׷��� ���� ����ÿ� �������� ���Ž�Ų��.
void GLootEnder::RemoveGarbageDropItem(GEntityPlayer* pPlayer, GEntityNPC* pNPC)
{
	if (NULL == pPlayer) return;
	if (NULL == pNPC) return;

	if (pNPC->GetNPCLoot().GetDropList().IsDropByInteract_Mortal() || 
		pNPC->GetNPCLoot().GetDropList().IsDropByInteract_Immortal())
	{
		set<int> setPreViewableCID;
		pNPC->GetNPCLoot().GetDropList().GetDropItems().Clear();

		if (!pPlayer->GetPlayerLoot().IsILootRegulatorActive())
		{
			gsys.pLootSystem->GetModifyHandler().HandleDropListModification(pNPC, setPreViewableCID);
		}
	}
}
