#include "stdafx.h"
#include "GFieldSystem.h"
#include "GServer.h"
#include "GEntityPlayer.h"
#include "GFieldMgr.h"
#include "GSharedField.h"
#include "CCommandTable.h"
#include "GCommand.h"
#include "GCommandCenter.h"
#include "GFieldInfoMgr.h"
#include "GSystem.h"
#include "GPlayerField.h"
#include "GPlayerFieldMasterServer.h"
#include "GPlayerFieldShared.h"
#include "GProxyFieldManager.h"
#include "GProxyField.h"
#include "GMasterServerFacade.h"
#include "CCommandResultTable.h"
#include "GConfig.h"
#include "GServerGateRouter.h"
#include "GPlayerObject.h"
#include "GPlayerObjectManager.h"
#include "GPlayerDoing.h"
#include "GClientFieldRouter.h"


bool GFieldSystem::PrepareChangeChannel( GEntityPlayer* pPlayer, CHANNELID nChannelID )
{
	// ä�� �̵� ���� üũ
	if (false == Check_PrepareChangeChannel(pPlayer, nChannelID)) return false;

	// MasterServer�� ä�� �̵� ���� ��û
	GServerGateRouter* pServerRouter = gsys.pMasterServerFacade->GetGateRouter();
	if (NULL == pServerRouter) return false;
	pServerRouter->PrepareChangeChannelReq(pPlayer->GetUID(), nChannelID);

	return true;
}
bool GFieldSystem::Check_PrepareChangeChannel( GEntityPlayer* pPlayer, CHANNELID nChannelID )
{
	VALID_RET(pPlayer, false);
	GField* pField = pPlayer->GetField();
	VALID_RET(pField, false);
	GPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(pPlayer->GetUID());
	VALID_RET(pPlayerObject, false);

	if (!pPlayerObject->IsInWorld())											return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_INVALID_STATE);		
	if (pPlayer->GetDoing().IsBusy())											return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_BUSY);					
	if (pPlayer->IsNowCombat())													return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_DONT_DURING_COMBAT);	
	if (!pField->IsSharedField())												return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_UNABLE_PLACE);
	if (nChannelID == ToSharedField(pField)->GetChannelID())					return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_SAME_CHANNEL);
	if (pPlayer->GetPlayerField().GetFieldShared().IsPreparedChangeChannel())	return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_PROCEED);

	// ä�� �˻�
	if (GConfig::m_bStandAlone)
	{
		GSharedField* pDestChannel = gmgr.pFieldMgr->GetSharedField(pPlayer->GetFieldID(), nChannelID);
		if (NULL == pDestChannel)
		{
			// ���� ���� �ʴ� ä��
			return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_IS_INVALID);
		}
		if (!pDestChannel->IsEnterable())
		{
			// ���� ���� �ʴ� ä��
			return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_IS_FULL);
		}
	}
	else
	{
		GProxyFieldManager* pProxyFieldManager = gsys.pMasterServerFacade->GetProxyFieldManager();
		VALID_RET(pProxyFieldManager != NULL, false);

		GProxyField* pProxyField = pProxyFieldManager->FindSharedField(pPlayer->GetFieldID(), nChannelID);
		if (NULL == pProxyField)
		{
			// ���� ���� �ʴ� ä��
			return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_IS_INVALID);
		}
		if (!pProxyField->IsEnterable())
		{
			// ���� ���� �ʴ� ä��
			return FailAndRoutePrepareChangeChannelRes(pPlayer, CR_FAIL_CHANGE_CHANNEL_IS_FULL);
		}
	}

	return true;
}

bool GFieldSystem::CancelChangeChannel( GEntityPlayer* pPlayer )
{
	VALID_RET(pPlayer, false);

	if (!pPlayer->GetPlayerField().GetFieldShared().IsPreparedChangeChannel())
		return false;

	// MasterServer�� �˸�
	GServerGateRouter* pServerRouter = gsys.pMasterServerFacade->GetGateRouter();
	if (NULL == pServerRouter) return false;
	pServerRouter->CancelChangeChannel(pPlayer->GetUID());

	// ä�� �̵� ���� ���� ����
	pPlayer->GetPlayerField().GetFieldShared().ClearPrepareChannelChange();

	return true;
}

bool GFieldSystem::ChangeChannel( GEntityPlayer* pPlayer )
{
	VALID_RET(pPlayer, false);

	if (false == Check_ChangeChannel(pPlayer)) return false;
	Apply_ChangeChannel(pPlayer);

	return true;
}

bool GFieldSystem::Check_ChangeChannel( GEntityPlayer* pPlayer )
{
	CHANNELID nChannelID	= pPlayer->GetPlayerField().GetFieldShared().GetPrepareChannelID();
	uint32 nPrepareTime		= pPlayer->GetPlayerField().GetFieldShared().GetPrepareChannelTime();

	int32 nElapsedTime = gsys.pSystem->GetNowTime() - nPrepareTime;
	nElapsedTime /= 1000;	// sec ��������

	if (nElapsedTime == 0 ||								// �غ�ܰ� ��������
		nElapsedTime < CHANNEL_CHANGE_PREPARE_TIME_MIN ||	// �ʹ� ���� �̵���û
		nElapsedTime > CHANNEL_CHANGE_PREPARE_TIME_MAX)		// �ʹ� �ʰ� �̵���û
	{
		pPlayer->FailAndRouteSystemMsg(CR_FAIL_CHANGE_CHANNEL_INVALID_PREPARE_TIME);
		return false;	// ä���̵��� �ϱ⿡ ��ȿ���� ���� ��û�ð��Դϴ�.
	}

	GField* pField = pPlayer->GetField();
	VALID_RET(pField, false);
	if (!pField->IsSharedField())	
	{
		pPlayer->FailAndRouteSystemMsg(CR_FAIL_CHANGE_CHANNEL_UNABLE_PLACE);
		return false;	// ���� �ʵ尡 �ƴϸ� ä�κ��� �Ұ���
	}

	if (nChannelID == ToSharedField(pField)->GetChannelID())
	{
		pPlayer->FailAndRouteSystemMsg(CR_FAIL_CHANGE_CHANNEL_SAME_CHANNEL);
		return false;	// ���� ä���� ���� �Ұ���
	}

	if (GConfig::m_bStandAlone)
	{
		GSharedField* pDestChannel = gmgr.pFieldMgr->GetSharedField(pPlayer->GetFieldID(), nChannelID);	
		if (NULL == pDestChannel)
		{
			pPlayer->FailAndRouteSystemMsg(CR_FAIL_CHANGE_CHANNEL_IS_INVALID);
			return false;	// ���� ���� �ʴ� ä��
		}
		if (!pDestChannel->HasReservation_ForStandAlone(pPlayer->GetUID()))
		{
			pPlayer->FailAndRouteSystemMsg(CR_FAIL_CHANGE_CHANNEL_INVALID_PREPARE_TIME);
			return false;	// ������� ���� �÷��̾�
		}
	}
	else
	{
		GProxyFieldManager* pProxyFieldManager = gsys.pMasterServerFacade->GetProxyFieldManager();
		VALID_RET(pProxyFieldManager != NULL, false);

		GProxyField* pProxyField = pProxyFieldManager->FindSharedField(pPlayer->GetFieldID(), nChannelID);
		if (NULL == pProxyField)
		{
			pPlayer->FailAndRouteSystemMsg(CR_FAIL_CHANGE_CHANNEL_IS_INVALID);
			return false;	// ���� ���� �ʴ� ä��
		}
	}
	return true;
}
void GFieldSystem::Apply_ChangeChannel( GEntityPlayer* pPlayer )
{
	CHANNELID nChannelID = pPlayer->GetPlayerField().GetFieldShared().GetPrepareChannelID();

	/// MasterServer �� ä�� �̵� ����
	pPlayer->GetPlayerField().GetFieldMasterServer().AskGateChangeChannel(pPlayer->GetFieldID(), pPlayer->GetPos(), pPlayer->GetDir(), nChannelID);

	pPlayer->GetPlayerField().GetFieldShared().ClearPrepareChannelChange();
}


void GFieldSystem::Warp( GEntityActor* pActor, vec3 vecDestPos, vec3 vecDestDir, bool bRoute )
{
	pActor->ClearEnteredSensorID();

	int nOldSectorX = pActor->GetSectorX();
	int nOldSectorY = pActor->GetSectorY();

	pActor->SetPos(vecDestPos);
	pActor->SetDir(vecDestDir);
	pActor->SetFacingDir(vecDestDir);

	// ��ƼġƮ
	if (pActor->IsPlayer())
		ToEntityPlayer(pActor)->CheatCheck_Warp();

	if (bRoute)
	{
		MCommand* pNewCommand = MakeNewCommand(MC_ENTITY_WARP,
									3, 
									NEW_UID(pActor->GetUID()),
									NEW_VEC(vecDestPos),
									NEW_VEC(vecDestDir)
									);

		// ������ �̿������� �̿����ͳ����� �̷�����ٸ� �ֺ� �뺸
		int nDifferenceX = abs(nOldSectorX - pActor->GetSectorX());
		int nDifferenceY = abs(nOldSectorY - pActor->GetSectorY());		
		
		if (nDifferenceX <= 2 && nDifferenceY <= 2)
		{
			pActor->RouteToThisCell(pNewCommand->Clone(), pActor->GetUID());
		}

		pActor->RouteToMe(pNewCommand);
	}
}

bool GFieldSystem::FailAndRoutePrepareChangeChannelRes( GEntityPlayer* pPlayer, CCommandResultTable error_code )
{
	GClientFieldRouter router;
	router.PrepareChangeChannel(pPlayer, error_code);
	return false;
}