#include "StdAfx.h"
#include "GAsyncTask_MoveLoginServer.h"
#include "GPlayerLogoutDBExecuter.h"
#include "GGlobal.h"
#include "GPlayerObjectManager.h"
#include "GPlayerObject.h"
#include "GSystem.h"
#include "GMoveServerRouter.h"
#include "GMasterServerFacade.h"
#include "CCommandResultTable.h"


GAsyncTask_MoveLoginServer::GAsyncTask_MoveLoginServer(MUID uid, uint32 nNowTime)
: GAsyncTask(uid, nNowTime)
, m_stateMoveTask(MTS_INITIAL)
, m_pPlayerLogoutDBExecuter(NULL)
{
}

GAsyncTask_MoveLoginServer::~GAsyncTask_MoveLoginServer()
{
	SAFE_DELETE(m_pPlayerLogoutDBExecuter);
}

void GAsyncTask_MoveLoginServer::Start(MUID uidPlayer)
{
	if (m_stateMoveTask != MTS_INITIAL)
	{
		return;
	}

	m_uidPlayer = uidPlayer;

	GPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(m_uidPlayer);
	if (pPlayerObject == NULL)
	{
		Done();
		return;
	}

	if (pPlayerObject->IsReadyMoveLoginServer() == false)
	{
		mlog("Failed! GAsyncTask_MoveLoginServer::Start(), PlayerObject::IsReadyMoveLoginServer() is false! (PlayerUID: (%u:%u) , POS: %d)\n"
			, m_uidPlayer.High, m_uidPlayer.Low, pPlayerObject->GetState());
		Done();
		return;
	}

	m_pPlayerLogoutDBExecuter = new GPlayerLogoutDBExecuter(pPlayerObject);

	m_stateMoveTask = MTS_LOGOUT_DB_EXECUTE;
	OnUpdate();
}

void GAsyncTask_MoveLoginServer::OnUpdate()
{
	// ��ü�� ������ٸ�, �׽�ũ�� ������.
	GPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(m_uidPlayer);
	if (pPlayerObject == NULL)
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
	case MTS_ROUTE_REQUEST_MOVE_SERVER:
		OnTaskState_ROUTE_REQUEST_MOVE_SERVER(pPlayerObject);
		break;
	case MTS_WAIT_RESPONSE_COMMAND:
		OnTaskState_WAIT_RESPONSE_COMMAND(pPlayerObject);
		break;
	default:
		Done();
		break;
	}
}

void GAsyncTask_MoveLoginServer::OnTaskState_LOGOUT_DB_EXECUTE()
{
	if (GetState() != MTS_LOGOUT_DB_EXECUTE) return;

	m_pPlayerLogoutDBExecuter->Update();
	if (!m_pPlayerLogoutDBExecuter->IsDone())
		return;

	SetState(MTS_ROUTE_REQUEST_MOVE_SERVER);
}

void GAsyncTask_MoveLoginServer::OnTaskState_ROUTE_REQUEST_MOVE_SERVER(GPlayerObject* pPlayerObject)
{
	_ASSERT(NULL != pPlayerObject);
	if (GetState() != MTS_ROUTE_REQUEST_MOVE_SERVER) return;

	// �����ͼ����� ����
	GMoveServerRouter* pRouter = gsys.pMasterServerFacade->GetMoveServerRouter();
	RVALID(pRouter != NULL);
	pRouter->RouteReserveMoveLoginServerReq(pPlayerObject->GetUID());

	// ������ ���·� ����
	SetState(MTS_WAIT_RESPONSE_COMMAND);

	// ������ Ÿ�Ӿƿ� ���� �ð� ����
	m_nTimeoutStartTime = gsys.pSystem->GetNowTime();
}

void GAsyncTask_MoveLoginServer::OnTaskState_WAIT_RESPONSE_COMMAND(GPlayerObject* pPlayerObject)
{
	_ASSERT(NULL != pPlayerObject);
	if (GetState() != MTS_WAIT_RESPONSE_COMMAND) return;

	// �����̵� ���� Ŀ�ǵ尡 �����ؼ� ���°� ����Ǿ�����, �����̵� ����.
	if (pPlayerObject->IsMoveLoginServer())
	{
		pPlayerObject->GoMoveLoginServer(CR_SUCCESS);
		Done();
		return;
	}

	// Ÿ�Ӿƿ� ó��
	if (CheckTimeout())
	{
		mlog2("Timeout! GAsyncTask_MoveLoginServer::OnTaskState_WAIT_RESPONSE_COMMAND()\n");

		pPlayerObject->CancelMoveLoginServer(CR_MOVE_TO_SELECT_CHAR_TIMEOUT);	// �����̵� ���
		Done();
		return;
	}
}


bool GAsyncTask_MoveLoginServer::CheckTimeout()
{
	uint32 nNowTime = gsys.pSystem->GetNowTime();
	if ((nNowTime - m_nTimeoutStartTime) > TIMEOUT_TICK_MOVE_SERVER)
		return true;

	return false;
}
