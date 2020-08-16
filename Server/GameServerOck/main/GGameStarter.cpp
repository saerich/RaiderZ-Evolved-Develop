#include "stdafx.h"
#include "GGameStarter.h"
#include "GPlayerObject.h"
#include "GPlayerObjectManager.h"
#include "GEntityPlayer.h"
#include "GPlayerGameStart_State.h"
#include "GPlayerGameStart_AutoRebirther.h"
#include "GPlayerGameStart_FieldGater.h"
#include "GWorld.h"
#include "GMasterServerFacade.h"
#include "GFieldMgr.h"
#include "GGlobal.h"
#include "GServerPlayerRouter.h"
#include "GPlayerBattleArena.h"
#include "GMsgCommandFacade.h"
#include "GConfig.h"
#include "GServer.h"


void GGameStarter::Start( MUID uidPlayer, int nReqFieldID, vec3 vReqPos )
{
	// �÷��̾� ã��
	GPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (pPlayerObject == NULL)
	{
		SetResult(RESULT_FAILED_TO_FIND_PLAYER);
		return;
	}

	GEntityPlayer* pPlayer = pPlayerObject->GetEntity();
	if (pPlayer == NULL) 
	{
		SetResult(RESULT_FAILED_TO_FIND_PLAYER);
		return;
	}


	// ���ӽ��� ����
	pPlayerObject->OnStartGame();


	GPlayerGameStart_State playerGameStartState = GPlayerGameStart_State(pPlayer);

	// ��Ȱ ó��
	GPlayerGameStart_AutoRebirther autoRebirther(pPlayer);
	autoRebirther.Rebirth(playerGameStartState);

	// �� ĳ���� ���� ����
	gsys.pWorld->SendMyInfo(uidPlayer);	


	// TODO: �������� ������

	// �ʵ� ����
	if (GateToField(pPlayer, playerGameStartState, nReqFieldID, vReqPos) == false)
	{
		SetResult(RESULT_FAILED_TO_GATE);
		return;
	}

	// ���� ����
	pPlayer->GameStart();

	SetResult(RESULT_SUCCESS);
}

bool GGameStarter::GateToField( GEntityPlayer* pPlayer, GPlayerGameStart_State& playerGameStartState, int nReqFieldID, vec3 vReqPos )
{
	if (nReqFieldID != -1)
	{
		// ��û �ʵ� ����
		if (!pPlayer->GetPlayerField().GateToRequestField(nReqFieldID, vReqPos))
		{
			return false;
		}
	}
	else
	{
		GPlayerGameStart_FieldGater fieldGater(pPlayer);

		if (!fieldGater.Gate(playerGameStartState))
		{
			// �����ʵ� ���� ���� �ʱ�ȭ
			pPlayer->GetPlayerField().SetDynamicFieldGroupUID(MUID::Invalid());
			pPlayer->GetPlayerBattleArena().SetBattleArenaUID(MUID::Invalid());

			// ������ �ҷ��� �ִ� ù �ʵ� ����
			if (!pPlayer->GetPlayerField().GateToFirstField())
			{
				return false;
			}			
		}
	}

	return true;
}