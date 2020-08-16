#include "stdafx.h"
#include "GCombatVictory.h"
#include "GEntityNPC.h"
#include "GConst.h"
#include "GJobRunner.h"
#include "GAIProfiler.h"
#include "GJobMgr.h"
#include "GBehaviorStateMachine.h"
#include "GAIMonitorPort.h"

#define COMBAT_CHASE_TIMEOUT	(30.0f)

/////////////////////////////////////////////////////////////////////////////////////////
// GCombatCombat ////////////////////////////////////////////////////////////////////////
GCombatVictory::GCombatVictory(GModuleAI* pOwner, GStateMachine<GModuleAI>* pFSM) 
: GCombatState(pOwner, pFSM)
, m_pJobRunner(NULL)
{
	m_pJobRunner = new GJobRunner();
}

GCombatVictory::~GCombatVictory()
{
	SAFE_DELETE(m_pJobRunner);
}

void GCombatVictory::OnRegisterTrainstionRule()
{
	if (true == GetNPC()->GetNPCInfo()->bCombatable)
	{
		AddTransitionRule(GMSG_AI_EVT_TRY_ATTACKED,		AI_BEHAVIOR_COMBAT);
		AddTransitionRule(GMSG_AI_EVT_ATTACK,			AI_BEHAVIOR_COMBAT);
		AddTransitionRule(GMSG_AI_EVT_ATTACKED,			AI_BEHAVIOR_COMBAT);
	}

	AddTransitionRule(GMSG_AI_EVT_MISSION,			AI_BEHAVIOR_MISSION);
}

void GCombatVictory::Enter()
{
	if (GetNPC()->IsDead())	
		return;

	if (GConst::AIMON_ENABLE &&
		GAIMonitorPort::Inst().IsSetedNPC(GetNPC()->GetUID()))
	{
		GAIMonitorPort::Inst().OnJobEvent(false, "ClearJob - cause by 'Enter CombatVictoryState'");
	}
	GJobMgr* pJobMgr = GetNPC()->GetJobMgr();
	pJobMgr->Clear(true);

	// �¸����¿����� �ٸ� ����� ������ �ȵ�
	// �ܺο��� �߰��Ǵ� ����� �̼������� ó��
//	GetNPC()->GetJobRunner()->MissionMode();

	int nTalentID = GetNPC()->GetNPCInfoMaster()->ColtInfo.attrCombat[GetNPC()->GetMode()].nVictoryTalentID;
	if (nTalentID == INVALID_TALENT_ID ||
		!m_pJobRunner->UseTalent(GetNPC(), NULL, nTalentID))
	{
		m_pJobRunner->Idle(GetAI(), GConst::VICTORY_DEFAULT_DELAY);
	}
	
	GetNPC()->OnVictory();
}

void GCombatVictory::Exit(int nNextStateID)
{
	//GetNPC()->GetJobRunner()->BehaviorMode();
}

GMessageResult GCombatVictory::Message(GMessage& msg)
{
	if (__super::Message(msg) == MR_TRUE)		return MR_TRUE;

	return MR_FALSE;
}

void GCombatVictory::Update(float fDelta)
{
	AI_PFI_BLOCK(APT_ID_COMBAT_VICTORY, L"CombatVictory");
	
	if (IsJobRunning())		
		return; // �� ������

	if (HasMissionJobs())
	{
		// ó���� �̼��� ������ �ٷ� �̼� ���·� ����
		GMessage msg(GMSG_AI_EVT_MISSION);
		GetNPC()->Message(msg);
	}
	else
	{
		// ó���� �̼��� ������ ���� ��ġ�� ����
		SendMsg_EndCombat();	
	}
}

bool GCombatVictory::HasMissionJobs()
{
	return GetNPC()->GetModuleAI()->GetBehaviorFSM()->GetMissionJobCounter() > 0;
}