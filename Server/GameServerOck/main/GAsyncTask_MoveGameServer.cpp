#include "stdafx.h"
#include "GAsyncTask_MoveGameServer.h"
#include "GEntityPlayer.h"
#include "GGlobal.h"
#include "GSystem.h"
#include "GMasterServerFacade.h"
#include "GServerPlayerRouter.h"
#include "GPlayerObjectManager.h"
#include "GPlayerObject.h"
#include "GServerGateRouter.h"
#include "GPlayerGameData.h"
#include "GServerGateRouter.h"
#include "GPlayerLogoutDBExecuter.h"


GAsyncTask_MoveGameServer::GAsyncTask_MoveGameServer(MUID uid, uint32 nNowTime)
: GAsyncTask(uid, nNowTime)
, m_stateMoveTask(MTS_INITIAL)
, m_pPlayerLogoutDBExecuter(NULL)
{
	// do nothing
}

GAsyncTask_MoveGameServer::~GAsyncTask_MoveGameServer()
{
	SAFE_DELETE(m_pPlayerLogoutDBExecuter);
}

void GAsyncTask_MoveGameServer::Start(MUID uidPlayer, int nTargetGameServerID)
{
	if (m_stateMoveTask != MTS_INITIAL)
	{
		return;
	}

	m_uidPlayer = uidPlayer;
	m_nTargetGameServerID = nTargetGameServerID;

	GPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(m_uidPlayer);
	if (pPlayerObject == NULL)
	{
		Done();
		return;
	}

	m_pPlayerLogoutDBExecuter = new GPlayerLogoutDBExecuter(pPlayerObject);

	m_stateMoveTask = MTS_LOGOUT_DB_EXECUTE;
	OnUpdate();
}

void GAsyncTask_MoveGameServer::OnUpdate()
{
	// ��ü�� ������ٸ�, �׽�ũ�� ������.
	GPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(m_uidPlayer);
	if (pPlayerObject == NULL)
	{
		Done();
		return;
	}

	GEntityPlayer* pEntityPlayer = pPlayerObject->GetEntity();
	if (pEntityPlayer == NULL)
	{
		Done();
		return;
	}

	// ���
	switch(GetState())
	{
	case MTS_INITIAL:
		// do nothing
		break;
	case MTS_LOGOUT_DB_EXECUTE:
		OnTaskState_LOGOUT_DB_EXECUTE();
		break;
	case MTS_PACK_AND_ROUTE_PLAYDATA:
		OnTaskState_PACK_AND_ROUTE_PLAYDATA(pEntityPlayer);
		break;
	case MTS_WAIT_RESPONSE_COMMAND:
		OnTaskState_WAIT_RESPONSE_COMMAND(pPlayerObject);
		break;
	default:
		Done();
		break;
	}
}

void GAsyncTask_MoveGameServer::OnTaskState_LOGOUT_DB_EXECUTE()
{
	if (GetState() != MTS_LOGOUT_DB_EXECUTE) return;

	m_pPlayerLogoutDBExecuter->Update();
	if (!m_pPlayerLogoutDBExecuter->IsDone())
		return;
	
	SetState(MTS_PACK_AND_ROUTE_PLAYDATA);
}

void GAsyncTask_MoveGameServer::OnTaskState_PACK_AND_ROUTE_PLAYDATA(GEntityPlayer* pEntityPlayer)
{
	_ASSERT(NULL != pEntityPlayer);
	if (GetState() != MTS_PACK_AND_ROUTE_PLAYDATA) return;

	// �÷��̵����� ��ŷ
	GPlayerGameData playerGameData;
	playerGameData.PackFromEntityPlayer(pEntityPlayer);

	// �����ͼ����� ����
	MUID uidRequestPlayer = pEntityPlayer->GetUID();
	GServerGateRouter* pServerRouter = gsys.pMasterServerFacade->GetGateRouter();
	pServerRouter->RequestReserveMoveGameServer(uidRequestPlayer, m_nTargetGameServerID, &playerGameData);

	// ������ ���·� ����
	SetState(MTS_WAIT_RESPONSE_COMMAND);

	// ������ Ÿ�Ӿƿ� ���� �ð� ����
	m_nTimeoutStartTime = gsys.pSystem->GetNowTime();
}

void GAsyncTask_MoveGameServer::OnTaskState_WAIT_RESPONSE_COMMAND(GPlayerObject* pPlayerObject)
{
	_ASSERT(NULL != pPlayerObject);
	if (GetState() != MTS_WAIT_RESPONSE_COMMAND) return;

	// �����̵� ���� Ŀ�ǵ尡 ����������, �����̵� ����.
	if (pPlayerObject->IsGetSetMoveGameServer())
	{
		pPlayerObject->GoMoveGameServer();
		Done();
		return;
	}

	// Ÿ�Ӿƿ� ó��
	if (CheckTimeout())
	{
		mlog2("Timeout! GAsyncTask_MoveGameServer::OnTaskState_WAIT_RESPONSE_COMMAND()\n");

		pPlayerObject->CancelMoveGameServer();	// �����̵� ���	
		Done();
		return;
	}
}

bool GAsyncTask_MoveGameServer::CheckTimeout()
{
	uint32 nNowTime = gsys.pSystem->GetNowTime();
	if ((nNowTime - m_nTimeoutStartTime) > TIMEOUT_TICK_MOVE_SERVER)
		return true;

	return false;
}
