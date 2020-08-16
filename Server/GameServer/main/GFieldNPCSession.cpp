#include "StdAfx.h"
#include "GFieldNPCSession.h"
#include "GJob.h"
#include "AStlUtil.h"
#include "GEntityNPC.h"
#include "GGlobal.h"
#include "GNPCMgr.h"
#include "GJobMgr.h"
#include "GGlueNPC.h"
#include "GServer.h"
#include "GScriptUtil.h"
#include "GFieldMgr.h"
#include "GField.h"
#include "GGlueField.h"
#include "GNullGlueField.h"
#include "GColtChecker.h"
#include "GStrings.h"
#include "GNullGlueActor.h"
#include "GJobRunner.h"
#include "GModuleBuff.h"
#include "GConst.h"
#include "GAIMonitorPort.h"
#include "GSessionJobs.h"

GFieldNPCSession::GFieldNPCSession(MUID uid, MUID uidField, string strName)
: m_UID(uid)
, m_uidField(uidField)
, m_strName(strName)
, m_bEndedSession(false)
, m_bExpired(false)
, m_nCombatState(COMBATSTATE_NONE)
{
	m_pScene = new Scene(this);
	m_rgrWait.Stop();
}

GFieldNPCSession::~GFieldNPCSession(void)
{
	Reset();

	SAFE_DELETE(m_pScene);
}

void GFieldNPCSession::PushJob( GJob* pJob )
{
	VALID(pJob);

	pJob->Attr.bAutoDeletable = false;
	m_vecScheduledJobs.push_back(pJob);
}

size_t GFieldNPCSession::GetJobQty() const
{
	return m_vecScheduledJobs.size();
}

bool GFieldNPCSession::HasCombatingNPC()
{
	for each (MUID each in m_setNPCs)
	{
		GEntityNPC* pNPC = gmgr.pNPCMgr->GetEntityNPC(each);
		if (!pNPC)
			continue;

		if (pNPC->IsNowCombat())
			return true;
	}

	return false;
}

void GFieldNPCSession::Update( float fDelta )
{
	if (GetSceneName() == string("__none__"))
	{
		ChangeScene("Begin", false);
	}

	if (!m_strChangeSceneName.empty())
	{
		ChangeScene(m_strChangeSceneName.c_str());
		m_strChangeSceneName = "";
	}

	if (m_pScene->Update(fDelta))
		return; // ���°� ���� ��

	//if (m_vecScheduledJobs.empty() &&
	//	!m_pScene->HasChangeRule())
	//{
	//	EndSessionImpl();
	//}

	if (m_bEndedSession)
	{
		if (!HasRunningJob())
		{
			ChangeScene("Finish", false);
			SendFinishMsg();
			Reset();	
			Expired();
			return;
		}

		return;
	}

	// ��� �ð�ó��
	if (m_rgrWait.IsReady(fDelta))
		m_rgrWait.Stop();
	
	RunJobs(m_vecScheduledJobs);
}

void GFieldNPCSession::SendFinishMsg()
{
	for each (MUID each in m_setNPCs)
	{
		GEntityNPC* pNPC = gmgr.pNPCMgr->GetEntityNPC(each);
		if (!pNPC)
			continue;

		GMessage msg(GMSG_AI_EVT_SESSION_FINISHED);
		pNPC->Message(msg);
	}
}

void GFieldNPCSession::RunJobs(deque<GJob*>& vecJobs)
{
	while(!vecJobs.empty())
	{
		if (m_rgrWait.IsActive())
		{
			// ���� �����
			return;
		}

		if (IsBlocking() &&
			HasRunningJob())
		{
			// ��ŷ �����
			return;
		}

		if (!m_strChangeSceneName.empty())
		{
			// ���� �� ������ ����Ǹ� ����
			return;
		}

		GJob* pJob = vecJobs.front();
		if (!pJob)
		{
			PopJob(vecJobs);
			continue;
		}

		if (pJob->IsSessionJob())
		{
			if (!OnProcessSessionJob(pJob))
			{
				PopJob(vecJobs);
				PushDoneJob(pJob);
			}
			continue;
		}

		MUID uidJobOwner = pJob->GetOwnerUID();
		GEntityNPC* pOwner = gmgr.pNPCMgr->GetEntityNPC(uidJobOwner); // �ʹ� ���� �ɸ��� Field���� �˻��ϵ��� �����ؾ� �� 
		if (!pOwner)
		{
			// �������� �ʴ� ������
			PopJob(vecJobs);
			PushDoneJob(pJob);
			continue;
		}

		//pOwner->GetJobRunner()->PushSessionJob(pOwner->GetModuleAI(), pJob);
		GMessage msg(GMSG_AI_EVT_SESSION_PUSHJOB, pJob);
		pOwner->Message(msg);

		if (m_setNPCs.find(uidJobOwner) == m_setNPCs.end())
		{
			// ���ǿ� ���� ������
			m_vecDoneJobs.push_back(pJob);
		}
		else
		{
			// ���ǿ� �ִ� ������
			m_setRunningJobs.insert(pJob);
		}

		PopJob(vecJobs);
	}
}

void GFieldNPCSession::PopJob(deque<GJob*>& vecJobs)
{
	if (vecJobs.empty())
		return;

	vecJobs.pop_front();
}

void GFieldNPCSession::OnJobFinished( const GJob* pJob )
{
	if (m_setRunningJobs.find(pJob) == m_setRunningJobs.end())
		return; // ������� ���� ��

	m_setRunningJobs.erase(pJob);
	PushDoneJob(pJob);
}

bool GFieldNPCSession::IsEmptyJob() const
{
	return (GetJobQty() <= 0);
}

void GFieldNPCSession::NonBlocking()
{
	PushJob(new GJobS_Blocking(false));
}

void GFieldNPCSession::Blocking()
{
	PushJob(new GJobS_Blocking(true));
}

void GFieldNPCSession::Wait( float fDuration )
{
	PushJob(new GJobS_Wait(fDuration));
}

void GFieldNPCSession::RemoveNPC( MUID uid )
{
	PushJob(new GJobS_RemoveNPC(uid));
}

void GFieldNPCSession::EndSession()
{
	PushJob(new GJobS_EndSession());
}

void GFieldNPCSession::ChangeState_ForGlue( const char* szName )
{
	PushJob(new GJobS_ChangeState(szName));
}

void GFieldNPCSession::MakePeace_ForGlue()
{
	PushJob(new GJobS_MakePeace());
}

void GFieldNPCSession::MakeCombat_ForGlue()
{
	PushJob(new GJobS_MakeCombat());
}

void GFieldNPCSession::AddNPC( MUID uid )
{
	// NPC�� ���ǿ� �����ϸ� ������ ��ť�� �׿��ִ� ���� ��� ���� �ϴ� ���ǿ� ���Խ�Ŵ
	GEntityNPC* pNPC = gmgr.pNPCMgr->GetEntityNPC(uid);
	if (!pNPC)
		return;

	if (pNPC->HasSession())
	{
		// ������ ���ǿ� ������ �־��ٸ� ���� ���� ����
		GFieldNPCSession* pFieldSession = 
			pNPC->GetSession();

		if (pFieldSession)
			pFieldSession->RemoveNPCImpl(uid);
	}

	MUID uidSession = GetUID();
	GMessage msg(GMSG_AI_EVT_SESSION, &uidSession);
	pNPC->Message(msg);
	//pNPC->GetJobMgr()->SetSession(this);
	PushJob(new GJobS_AddNPC(uid));
}

void GFieldNPCSession::Reset()
{
	ResetStatusAllNPC();
	ResetJobs();
		
	set<MUID> setNPCs = m_setNPCs;
	for each (MUID each in setNPCs)
	{
		RemoveNPCImpl(each);
	}
}

size_t GFieldNPCSession::GetNPCQty() const
{
	return m_setNPCs.size();
}

void GFieldNPCSession::AddNPC_ForGlue( GGlueNPC* pNPC )
{
	VALID(pNPC);
	VALID(!pNPC->IsInvalidOwner());
	AddNPC(pNPC->GetUID());
}

void GFieldNPCSession::RemoveNPC_ForGlue( GGlueNPC* pNPC )
{
	VALID(pNPC);
	VALID(!pNPC->IsInvalidOwner());
	RemoveNPC(pNPC->GetUID());
}

bool GFieldNPCSession::IsBlocking() const
{
	return m_Blocker.IsBlocking();
}

GFieldNPCSession* GFieldNPCSession::New(string strName, MUID uidField)
{
	return new GFieldNPCSession(gsys.pServer->NewUID(), uidField, strName);
}

float GFieldNPCSession::GetWaitRemainTime()
{
	return m_rgrWait.GetTickTime() - m_rgrWait.GetElapsedTime();
}

void GFieldNPCSession::AddNPCImpl( MUID uid )
{
	GEntityNPC* pNPC = gmgr.pNPCMgr->GetEntityNPC(uid);
	if (!pNPC)
		return;

	pNPC->ClearJob();

	m_setNPCs.insert(uid);
}

void GFieldNPCSession::RemoveNPCImpl( MUID uid )
{
	m_setNPCs.erase(uid);

	// ������ �ʱ�ȭ�� ��, ������ NPC���� ������ ����
	GEntityNPC* pNPC = gmgr.pNPCMgr->GetEntityNPC(uid);
	if (!pNPC)
		return;

	if (GConst::AIMON_ENABLE &&
		GAIMonitorPort::Inst().IsSetedNPC(pNPC->GetUID()))
	{
		GAIMonitorPort::Inst().OnJobEvent(false, "ClearJob - cause by 'Reset NPC-Session'");
	}

	//pNPC->GetJobMgr()->ResetSession();
	pNPC->GetJobMgr()->Clear();
}

void GFieldNPCSession::EndSessionImpl()
{
	m_bEndedSession = true;
}

bool GFieldNPCSession::OnProcessSessionJob( GJob* pRawJob )
{
	VALID_RET(pRawJob, false);

	if (GConst::LOG_NPCSESSION)
	{
		mlog("[SessionLog] %s(%s/%s) - job(%s)\n", 
			__FUNCTION__, GetName().c_str(), GetSceneName(),
			pRawJob?MLocale::ConvUTF16ToAnsi(pRawJob->GetJobName()).c_str():"{none}");
	}

	switch (pRawJob->GetID())
	{
	case GJOB_SESSION_BLOCKING:
		{
			GJobS_Blocking* pJob = dynamic_cast<GJobS_Blocking*>(pRawJob);
			if (pJob)
			{
				if (pJob->IsEnable())
					m_Blocker.Blocking();
				else
					m_Blocker.NonBlocking();
			}
		}break;
	case GJOB_SESSION_WAIT:
		{
			GJobS_Wait* pJob = dynamic_cast<GJobS_Wait*>(pRawJob);
			if (pJob)
			{
				m_rgrWait.SetTickTime(pJob->GetWaitTime());
				m_rgrWait.Start();
			}
		}break;
	case GJOB_SESSION_END:
		{
			EndSessionImpl();
		}break;
	case GJOB_SESSION_ADD_NPC:
		{
			GJobS_AddNPC* pJob = dynamic_cast<GJobS_AddNPC*>(pRawJob);
			if (pJob)
			{
				AddNPCImpl(pJob->GetNPCUID());
			}
		}break;
	case GJOB_SESSION_REMOVE_NPC:
		{
			GJobS_RemoveNPC* pJob = dynamic_cast<GJobS_RemoveNPC*>(pRawJob);
			if (pJob)
			{
				RemoveNPCImpl(pJob->GetNPCUID());
			}
		}break;
	case GJOB_SESSION_CHANGE_STATE:
		{
			GJobS_ChangeState* pJob = dynamic_cast<GJobS_ChangeState*>(pRawJob);
			if (pJob)
			{
				m_strChangeSceneName = pJob->GetStateName();
				return true; // �ʱ�ȭ �Ǿ���
			}
		}break;
	case GJOB_SESSION_MAKE_PEACE:
		{
			MakePeaceAllNPC();
		}break;
	case GJOB_SESSION_MAKE_COMBAT:
		{
			MakeCombatAllNPC();
		}break;
		
	default:
		VALID_RET(!"unknown session job type", false);
	}

	return false;
}

bool GFieldNPCSession::HasRunningJob() const
{
	return !m_setRunningJobs.empty();
}

void GFieldNPCSession::PushDoneJob( const GJob* pJob )
{
	m_vecDoneJobs.push_back(pJob);
}

bool GFieldNPCSession::HasJoinnedNPC( MUID uid ) const
{
	return (m_setNPCs.find(uid) != m_setNPCs.end());
}

bool GFieldNPCSession::HasExpired() const
{
	return m_bExpired;
}

void GFieldNPCSession::Expired()
{
	m_bExpired = true;
}

const char* GFieldNPCSession::GetSceneName()
{
	return m_pScene->GetName();
}

size_t GFieldNPCSession::GetScneneConditionCount()
{
	return m_pScene->GetConditionCount();
}

void GFieldNPCSession::ChangeScene(const char* szName, bool bResetJobs)
{
	if (GConst::LOG_NPCSESSION)
	{
		mlog("[SessionLog] %s(%s/%s) => new_scene: %s\n", __FUNCTION__, GetName().c_str(), GetSceneName(), szName);
	}

	m_pScene->Change(szName, bResetJobs);

	string strFuncName = 
		"OnSessionScene_" + GetName() + "_" + szName;

	ResetStatusAllNPC();

	if (bResetJobs)
	{
		ResetJobs();
	}

	m_Blocker.Blocking();
	ChangeCombatState(COMBATSTATE_NONE);
	MakePeaceAllNPC();
	RunCallback(strFuncName);
}

void GFieldNPCSession::AddChangeSceneRule( Scene::Condition& condition )
{
	if (GConst::LOG_NPCSESSION)
	{
		mlog("[SessionLog] %s(%s/%s) => condition: %s/%s\n", 
			__FUNCTION__, GetName().c_str(), GetSceneName(),
			condition.strConditionType.c_str(), condition.strNextStateName.c_str());
	}

	m_pScene->AddChangeRule(condition);
}

void GFieldNPCSession::AddChangeSceneRule_ForGlue( const char* szNextState, GGlueNPC* pNPC, const char* szConditionType, MWLua::table tableParams )
{
	VALID(szNextState);
	VALID(szConditionType);
	VALID(pNPC);

	Scene::Condition condition;
	condition.strNextStateName = szNextState;
	condition.strConditionType = szConditionType;
	condition.uidNPC = pNPC->GetUID();

	tableParams.iteration();
	int nParam = -1;
	while (tableParams.next<int>(&nParam))  
	{ 
		condition.vecParams.push_back(nParam);
	} 

	AddChangeSceneRule(condition);
}

void GFieldNPCSession::RunCallback(string strFuncname)
{
	if (strFuncname.empty())
		return;

	GField* pField = gmgr.pFieldMgr->GetField(m_uidField);
	if (!pField)
		return;

	if (!gsys.pScriptManager->GetLua()->IsExistMemberFunc(pField->GetDelegator()->GetLuaTableName().c_str(), strFuncname.c_str()))
		return;

	WMEMCALL2(WLUA, pField->GetDelegator()->GetLuaTableName().c_str(), strFuncname.c_str(), 
		TO_SAFE_LUAPTR(pField), 
		this,
		WNULL);	
}

void GFieldNPCSession::ResetJobs()
{
	SAFE_DELETE_VECTOR(m_vecScheduledJobs);
	SAFE_DELETE_VECTOR(m_setRunningJobs);
	SAFE_DELETE_VECTOR(m_vecDoneJobs);
}

void GFieldNPCSession::MakePeaceAllNPC()
{
	for each (MUID each in m_setNPCs)
	{
		GEntityNPC* pNPC = gmgr.pNPCMgr->GetEntityNPC(each);
		if (!pNPC)
			continue;

		if (pNPC->IsNowCombat())
		{
			if (pNPC->HasSession())
			{
				MUID uidSession = GetUID();
				GMessage msg(GMSG_AI_EVT_SESSION, &uidSession);
				pNPC->Message(msg);
			}
			else
			{
				pNPC->doMakePeaceForce();
			}
		}
		
	}
}

void GFieldNPCSession::MakeCombatAllNPC()
{
	for each (MUID each in m_setNPCs)
	{
		GEntityNPC* pNPC = gmgr.pNPCMgr->GetEntityNPC(each);
		if (!pNPC)
			continue;

		// ���� ���ɻ��·� ����
		pNPC->GetNPCStatus().nAA = NAAT_FACTION;
		pNPC->ChangeAttackable(NAT_FACTION);
	}
}

void GFieldNPCSession::ResetStatusAllNPC()
{
	for each (MUID each in m_setNPCs)
	{
		GEntityNPC* pNPC = gmgr.pNPCMgr->GetEntityNPC(each);
		if (!pNPC)
			continue;

		pNPC->ClearJob();
		
		//pNPC->GetJobRunner()->ClearSessionJob(pNPC->GetModuleAI());
		GMessage msg(GMSG_AI_EVT_SESSION_CHANGE_SCENE);
		pNPC->Message(msg);

		pNPC->GetModuleBuff()->RemoveAllDebuff();
		pNPC->GetJobMgr()->CancelCurrJob();
	}
}

bool GFieldNPCSession::IsThinkable() const
{
	if (m_nCombatState == COMBATSTATE_ALL)
		return true;

	return false;
}

void GFieldNPCSession::ChangeCombatState( int nState )
{
	m_nCombatState = (eCombatState)nState;
}

bool GFieldNPCSession::IsHittable( GEntityActor* pAttacker, GEntityActor* pVictim ) const
{
	VALID_RET(pAttacker, false);
	VALID_RET(pVictim, false);

	switch (m_nCombatState)
	{
	case COMBATSTATE_NONE:
		return false; // ������ �ǰ� �Ұ���

	case COMBATSTATE_SESSION:
		if (pAttacker->IsPlayer())
			return false;

		if (HasJoinnedNPC(pAttacker->GetUID()) &&
			HasJoinnedNPC(pVictim->GetUID()))
		{
			return true; // ���� ���� NPC �鳢�� �� �� ����
		}
	case COMBATSTATE_ALL:
		return true; // ������ �ǰ� ����
	}

	return false;
}

int GFieldNPCSession::GetCombatState()
{
	return (int)m_nCombatState;
}

void GFieldNPCSession::CombatSession_ForGlue()
{
	ChangeCombatState((int)COMBATSTATE_SESSION);
}

void GFieldNPCSession::CombatAll_ForGlue()
{
	ChangeCombatState((int)COMBATSTATE_ALL);
}

void GFieldNPCSession::CombatNone_ForGlue()
{
	ChangeCombatState((int)COMBATSTATE_NONE);
}

bool GFieldNPCSession::IsFirstStrike() const
{
	switch (m_nCombatState)
	{
	case COMBATSTATE_NONE:
		return false;

	case COMBATSTATE_SESSION:
		return false;

	case COMBATSTATE_ALL:
		return true;
	}

	return false;
}

void GFieldNPCSession::AddBookmark( const char* szName, GGlueActor* pActor )
{
	if (!pActor)
		return;

	m_setBookmark.insert(make_pair(szName, pActor->GetUID()));
}

GGlueActor* GFieldNPCSession::FindBookmark( const char* szName )
{
	typedef map<const char*, MUID> MAP_TYPE;
	MAP_TYPE::iterator it = m_setBookmark.find(szName);
	if (it == m_setBookmark.end())
		return GNullGlueActor::GetNull();

	GField* pField = gmgr.pFieldMgr->GetField(m_uidField);
	if (!pField)
		return GNullGlueActor::GetNull();

	GEntityActor* pActor = pField->FindActor(it->second);
	if (!pActor)
		return GNullGlueActor::GetNull();

	return pActor->GetDelegator();
}






//////////////////////////////////////////////////////////////////////////
//
// GFieldNPCSession::Blocker
//
//////////////////////////////////////////////////////////////////////////

GFieldNPCSession::Blocker::Blocker()
: m_bIsNonBlocking(false)
{

}

bool GFieldNPCSession::Blocker::IsBlocking() const
{
	return !m_bIsNonBlocking;
}

void GFieldNPCSession::Blocker::NonBlocking()
{
	m_bIsNonBlocking = true;
}

void GFieldNPCSession::Blocker::Blocking()
{
	m_bIsNonBlocking = false;
}


//////////////////////////////////////////////////////////////////////////
//
// GFieldNPCSession::State
//
//////////////////////////////////////////////////////////////////////////

GFieldNPCSession::Scene::Scene(GFieldNPCSession* pOwner)
: m_pOwner(pOwner)
, m_strName("__none__")
{
	m_rgrUpdateTick.SetTickTime(1.0f);
	m_rgrUpdateTick.Start();
}

bool GFieldNPCSession::Scene::Update( float fDelta )
{
	if (m_rgrUpdateTick.IsReady(fDelta))
	{
		if (CheckAllConditions())
			return true;
	}

	return false;
}

const char* GFieldNPCSession::Scene::GetName()
{
	return m_strName.c_str();
}

void GFieldNPCSession::Scene::Change( const char* szName, bool bResetJobs )
{
	m_strName = szName;
}

void GFieldNPCSession::Scene::AddChangeRule( Condition& condition )
{
	m_vecConditions.push_back(condition);
}

bool GFieldNPCSession::Scene::CheckAllConditions()
{
	typedef vector<Condition>::iterator ConditionIter;

	for (ConditionIter it = m_vecConditions.begin();
		it != m_vecConditions.end();)
	{
		Condition& cond = *it;
		if (CheckCondition(cond))
		{
			Condition condCopy = cond;
			it = m_vecConditions.erase(it); // Change() �ݹ鿡�� ������� �߰��� �� �����Ƿ� �켱 ����

			m_pOwner->ChangeScene(condCopy.strNextStateName.c_str());
			
			return true;
		}
		else
		{
			++it;
		}
	}

	return false;
}

bool GFieldNPCSession::Scene::CheckCondition( Condition& condition )
{
	GEntityNPC* pOwner = gmgr.pNPCMgr->GetEntityNPC(condition.uidNPC); // �ʹ� ���� �ɸ��� Field���� �˻��ϵ��� �����ؾ� �� 	
	if (!pOwner)
		return false; // �ش� NPC�� �������� ����
		
	GColtCheck condition_info;
	condition_info.nType = 
		(COLT_CHECK_TYPE)GStrings::StringToColtConditionState(
			MLocale::ConvAnsiToUTF16(condition.strConditionType.c_str()).c_str()
		);

	condition_info.vecParams.clear();
	for each (int nParam in condition.vecParams)
	{
		condition_info.vecParams.push_back(make_pair(true, nParam));
	}
	if (condition.vecParams.size() < MAX_AI_INFO_PARAM)
	{
		for (int i=0; i<MAX_AI_INFO_PARAM-(int)condition.vecParams.size(); ++i)
			condition_info.vecParams.push_back(make_pair(false, -1));
	}
	
	GColtChecker colt_conditioner;
	if (!colt_conditioner.CheckCondition(condition_info, pOwner))
		return false; // ����� ���� ������

	return true;
}

bool GFieldNPCSession::Scene::HasChangeRule() const
{
	return !m_vecConditions.empty();
}

size_t GFieldNPCSession::Scene::GetConditionCount() const
{
	return m_vecConditions.size();
}


