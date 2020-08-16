#include "stdafx.h"
#include "GCombatCombat.h"
#include "GEntityNPC.h"
#include "GField.h"
#include "GJob_MoveToEntity.h"
#include "GJob_Talent.h"
#include "GMessageID.h"
#include "GJob_Broker.h"
#include "GConst.h"
#include "GColtMgr.h"
#include "GGlobal.h"
#include "GScriptManager.h"
#include "GScriptGroup_NPC.h"
#include "GAIProfiler.h"
#include "GJobMgr.h"
#include "GHateTable.h"
#include "GNPCStress.h"
#include "GFieldNPCSession.h"

/////////////////////////////////////////////////////////////////////////////////////////
// GCombatCombat ////////////////////////////////////////////////////////////////////////
GCombatCombat::GCombatCombat(GModuleAI* pOwner, GStateMachine<GModuleAI>* pFSM) 
: GCombatState(pOwner, pFSM)
, m_pNPCInfo(NULL)
, m_bAlreadyCallAggro(false)
{
	m_pNPCInfo = static_cast<GEntityNPC*>(pOwner->GetOwner())->GetNPCInfo();
	m_ChaseStressRegulator.Stop();
}

GCombatCombat::~GCombatCombat()
{
	;
}

void GCombatCombat::OnRegisterTrainstionRule()
{
	AddTransitionRule(GMSG_AI_EVT_FLEE,				AI_COMBAT_FLEE);
	AddTransitionRule(GMSG_AI_EVT_START_VICTORY,	AI_COMBAT_VICTORY);
	AddTransitionRule(GMSG_AI_EVT_SESSION,			AI_BEHAVIOR_SESSION);
	AddTransitionRule(GMSG_AI_EVT_SESSION_PUSHJOB,	AI_BEHAVIOR_SESSION);
}

void GCombatCombat::Enter()
{
	m_uidOldTarget = MUID::Invalid();
	m_bAlreadyCallAggro = false;

	m_ChaseStressRegulator.SetTickTime(GConst::STRESS_CHASE_INTERVAL);
	m_ChaseStressRegulator.Start();

	// ���� ��ġ�� Ȩ����Ʈ�� ����
	mmlog("%s - %.1f %.1f\n", __FUNCTION__, GetNPC()->GetDir().x, GetNPC()->GetDir().y);
	GetNPC()->SetHomePoint(GetNPC()->GetPos(), GetNPC()->GetDir());
}

void GCombatCombat::Exit(int nNextStateID)
{
	GetNPC()->doGuardReleased();
}

GMessageResult GCombatCombat::Message(GMessage& msg)
{
	if (__super::Message(msg) == MR_TRUE)		return MR_TRUE;

	switch (msg.m_nID)
	{
	case GMSG_AI_EVT_ATTACK:
		m_ChaseStressRegulator.Reset(); // ������ ���� ��Ʈ������ �ʱ�ȭ
		break;
	}

	return MR_FALSE;
}

void GCombatCombat::Update(float fDelta)
{
	AI_PFI_BLOCK(APT_ID_COMBAT_COMBAT, L"CombatCombat");

	OnAggro();

	if (!GetAI()->GetOwner()->IsNPC())	return;

	_UpdateChase(fDelta);

	if(IsEndCombat())
	{
		// �ŷ�Ʈ�� �� ����Ҷ�����, combatcombat ������Ʈ���� �ٸ� ������Ʈ�� �ٲ��� �ʴ´�.
		if (_IsRunnginTalentJob()) return;

		_EndCombat();
		return;
	}

	if (!GetNPC()->GetTargetActor())
	{
		TargetNewEnemy();
	}

	_UpdateCombat();

	if (CheckCombatRadius())
	{
		// ���� ���� ���
		GetNPC()->SetHomePoint(GetNPC()->GetSpawnPos(), GetNPC()->GetDir()); // ������ġ�� �̵��ǵ��� ����
		_EndCombatForced();
		return;
	}
}

bool GCombatCombat::CheckCombatRadius()
{
	vec3 vSpawnPos = GetNPC()->GetSpawnPos();
	vec3 vCurPos = GetNPC()->GetPos();
	float fCombatRadius = GetNPC()->GetCombatRadius();
	if (fCombatRadius == 0.0f)
		return false; // �������� ����
	
	return (vCurPos.DistanceTo(vSpawnPos) > fCombatRadius);
}

void GCombatCombat::TargetNewEnemy()
{
	GEnemyFinder enemy_finder;
	MUID uidEnemy = enemy_finder.Find(GetNPC());

	EVENT_FOUND_ENEMY param;
	param.uidTarget = uidEnemy;
	GMessage msg(GMSG_AI_EVT_FIND_TARGET, &param);
	m_pOwner->Message(msg);
}

bool GCombatCombat::IsEndCombat()
{
	if (IsExistEnemy())
		return false; // ����ϰ� �ִ� ���� ����

	if (GetNPC()->GetNPCInfo()->m_bAssistAfterCombat)
	{
		GEnemyFinder enemy_finder;
		if (enemy_finder.Find(GetNPC()).IsValid())
			return false; // �ֺ��� ���� ����
	}
	
	return true; // ���� �����
}

bool GCombatCombat::_IsRunnginTalentJob()
{
	GJobMgr* pJobMgr = m_pOwner->GetJobMgr();
	GJob* pJob = pJobMgr->GetCurrJob();

	if (NULL == pJob) return false;
	if (GJOB_TALENT != pJob->GetID() && GJOB_GUARD != pJob->GetID()) return false;

	return true;
}

void GCombatCombat::_EndCombat()
{
	GetNPC()->OnEndCombat();

	if (IsObtainVictory())
	{
		SendMsg_StartVictory();
	}
	else
	{
		SendMsg_EndCombat();
	}
}

void GCombatCombat::_EndCombatForced()
{
	GetNPC()->OnEndCombat();

	SendMsg_EndCombat();
}

void GCombatCombat::OnAggro()
{
	VALID(GetNPC()->GetNPCInfo());
	if (!GetNPC()->GetNPCInfo()->bCounterAttack)
		return; // �ݰ� ���ϴ� ����

	if (!GetNPC()->IsNPC())	
		return;
	if (GetAI()->IsAssist())	
		return;
	if (m_bAlreadyCallAggro)
		return;

	m_bAlreadyCallAggro = true;

	GEntityActor* pTargetActor = GetNPC()->GetTargetActor();
	if (!pTargetActor)			return;

	// LuaCall -> NPC:OnAggro()
	GetScriptNPC()->OnAggro(GetNPC()->GetDelegator(), pTargetActor->GetDelegator());

	// Colt
	GetAI()->GetColt()->RunColtAggro(GetNPC(), pTargetActor);
}

void GCombatCombat::_UpdateChase(float fDelta)
{
	if ((!GetNPC()->IsNowUsingTalent() || GetNPC()->IsNowUsingRotateTalent()) && // ȸ�� �ŷ�Ʈ�� �ƴ��� ���� ��Ʈ����
		m_ChaseStressRegulator.IsReady(fDelta))
	{
		GetNPC()->GetNPCStress().AddStress(GConst::STRESS_ADD_LIMITED_CHASE_TIME);
	}
}

void GCombatCombat::OnTargetChanged(MUID uidOld, MUID uidNew)
{
	GetNPC()->RouteChangeCombatTarget(uidNew);
}

void GCombatCombat::_UpdateCombat()
{
	GEntityNPC* pThisNPC			= GetNPC();
	GEntityActor* pOpponentActor	= GetNPC()->GetTargetActor();

	VALID(pThisNPC->GetNPCInfo());

	if (!pThisNPC->GetNPCInfo()->bCounterAttack)
		return; // �ݰ� ���ϴ� ����

	MUID uidCurTarget = pThisNPC->GetTarget();
	if (uidCurTarget != m_uidOldTarget)
	{
		OnTargetChanged(m_uidOldTarget, uidCurTarget);
		m_uidOldTarget = uidCurTarget;
	}

	GFieldNPCSession* pSession = 
		pThisNPC->GetSession();
	if (pSession)
	{
		if (!pSession->IsThinkable())
		{
			GMessage msg(GMSG_AI_EVT_SESSION);
			m_pOwner->Message(msg);
			return; // NPC �������� ���� �Ұ����� ����
		}
	}

	if (pThisNPC->IsGuarding()) 	
		return;	// �����

	if (!pOpponentActor)
		return;	// ���� ��밡 ����

	if (IsJobRunning())				
		return; // ���� ������

	if (pThisNPC->HasMotionFactor())
		return;	// ��� ���Ͱ� �ɷ�����


	GetAI()->GetColt()->RunColtCombat(pThisNPC, pOpponentActor);

	m_ChaseStressRegulator.Reset();
}
