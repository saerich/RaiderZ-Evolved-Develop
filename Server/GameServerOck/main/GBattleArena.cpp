#include "StdAfx.h"
#include "GBattleArena.h"
#include "GDef.h"
#include "GEntityPlayer.h"
#include "GPlayerBattleArena.h"
#include "GGlobal.h"
#include "GPartySystem.h"
#include "CCommandTable.h"
#include "GCommand.h"
#include "GFieldInfoMgr.h"
#include "GFieldMgr.h"
#include "GDynamicFieldMaster.h"
#include "GConst.h"
#include "GSTLUtil.h"
#include "GRebirthSystem.h"
#include "GDynamicFieldPolicy_BattleArena.h"
#include "GGlobal.h"
#include "GPlayerFieldDynamic.h"
#include "GPlayerObjectManager.h"


#define NOTIFY_ARENA_EVENT(obs_type, set, event)		\
	for each (obs_type* pOvr in set)					\
		pOvr->event;									\


GBattleArena::GBattleArena(MUID uid, GameRule* pRule)
: m_UID(uid)
, m_nStatus(INVALID)
, m_pGameRule(pRule)
{
}

GBattleArena::~GBattleArena(void)
{
	SAFE_DELETE(m_pGameRule);
}


void GBattleArena::RegisterLisener( Lisener* pLisener )
{
	VALID(pLisener);
	m_vecLiseners.push_back(pLisener);
}

void GBattleArena::Join( MUID uidTeam, BattleArena::Team& team )
{
	VALID(team.uidParty.IsValid());
	VALID(m_mapEntries.find(uidTeam) == m_mapEntries.end());
	VALID(!team.IsInvalid());

	m_mapEntries.insert(std::make_pair(uidTeam, team));

	// ������ UID ����
	SetBattleArenaUIDToTeam(team);

	OnJoin(team);
}

GBattleArena::Status GBattleArena::GetStatus() const
{
	return m_nStatus;
}

void GBattleArena::ChangeStatus( Status nStatus )
{
	Status nOldStatus = m_nStatus;
	m_nStatus = nStatus;

	switch (nOldStatus)
	{
	case PREPARE:		OnPrepareLeave();		break;
	case COUNTING:		OnCountingLeave();		break;
	case PLAYING:		OnPlayingLeave();		break;
	case SCOREBOARD:	OnScoreBoardLeave();	break;
	//case FINISH:		OnFinishLeave();		break;
	}

	switch (m_nStatus)
	{
	case PREPARE:		OnPrepareEnter();		break;
	case COUNTING:		OnCountingEnter();		break;
	case PLAYING:		OnPlayingEnter();		break;
	case SCOREBOARD:	OnScoreBoardEnter();	break;
	case FINISH:		OnFinishEnter();		break;
	}
}

bool GBattleArena::Update( float fDelta )
{
	// ��Ģ ������Ʈ
	if (GetStatus() == PLAYING)
	{
		UpdateGameRules(fDelta);
	}
	
	switch (m_nStatus)
	{
	case PREPARE:		OnPrepareUpdate(fDelta);	break;
	case COUNTING:		OnCountingUpdate(fDelta);	break;
	case PLAYING:		OnPlayingUpdate(fDelta);	break;
	case SCOREBOARD:	OnScoreBoardUpdate(fDelta);	break;
	case FINISH:		OnFinishUpdate(fDelta);		break;
	}

	if (m_nStatus == FINISH)
	{
		OnFinishLeave();
		return true;
	}

	return false;
}

void GBattleArena::OnJoin( const BattleArena::Team& team )
{
	if (m_nStatus != PREPARE)
		return;	// ������ ��� ���¿��� ���� ����

	// �����ڰ� �� �̻��̸� ���� ����
	if (m_mapEntries.size() >= 2)
	{
		ChangeStatus(COUNTING);
	}
}

void GBattleArena::OnPrepareEnter()
{
	
}

void GBattleArena::OnCountingEnter()
{
	// ���� ��Ŀ ����
	ExtractStartingMarkers();

	// ���� �ʵ带 �����ϰ�, �����ڵ� ������Ű��
	MoveAllTeamToTrialField();

	// �� ��� ����
	for (BattleArena::TEAM_MAP::iterator it = m_mapEntries.begin();
		it != m_mapEntries.end();
		++it)
	{
		it->second.nStatus = BattleArena::Team::PLAYING;
	}

	// ���� ���� �ʱ�ȭ
	InitGameRules();

	m_rgrStartingCounter.SetTickTime(GConst::ARENA_GAME_COUNT);
	m_rgrStartingCounter.Start();

	// ���� ī��Ʈ ��Ŷ�� ����
	RouteToAllTeam(MakeNewCommand(MC_BATTLEARENA_COUNTING, 1, NEW_FLOAT(GConst::ARENA_GAME_COUNT)));
}

void GBattleArena::OnPlayingEnter()
{
	VALID(!m_mapEntries.empty());
	
	// ��Ʈ�� UID ����ü �����
	size_t i=0; 
	vector<MUID> vecTeamUIDs;	
	vecTeamUIDs.resize(m_mapEntries.size());
	for (BattleArena::TEAM_MAP::iterator it = m_mapEntries.begin();
		it != m_mapEntries.end();
		++it, ++i)
	{
		vecTeamUIDs[i] = it->first;
	}

	// ���� ���� ��Ŷ�� ����	
	RouteToAllTeam(MakeNewCommand(MC_BATTLEARENA_GAMESTART, 1, NEW_BLOB(&vecTeamUIDs[0], sizeof(MUID), vecTeamUIDs.size())));
}

void GBattleArena::OnScoreBoardEnter()
{
	// �̺�Ʈ ����
	for each (Lisener* each in m_vecLiseners)
	{
		each->OnBattleArenaShowScoreBoard(m_mapEntries); 
	}

	m_rgrScoreBoardCounter.SetTickTime(GConst::ARENA_SCOREBOARD_COUNT);
	m_rgrScoreBoardCounter.Start();
}

void GBattleArena::OnFinishEnter()
{
	// ���� �ǴϽ� ��Ŷ�� ����
	RouteToAllTeam(MakeNewCommand(MC_BATTLEARENA_FINISHED, 0, NULL));
	// ������ UID ����
	ClearBattleArenaUIDToTeam();
}

void GBattleArena::OnPrepareLeave()
{

}

void GBattleArena::OnCountingLeave()
{

}

void GBattleArena::OnPlayingLeave()
{
}

void GBattleArena::OnScoreBoardLeave()
{

}

void GBattleArena::OnFinishLeave()
{
	// ��Ʈ�� ��� ��� ��Ȱ
	RebirthToAllTeam();
	// ��Ʈ�� ��� ��� ȸ��
	FullHealToAllTeam();
	// �����ڵ� ���� �ʵ�� �ǵ���������
	MoveAllTeamToEnterPos();

	// �̺�Ʈ ����
	for each (Lisener* each in m_vecLiseners)
	{
		each->OnBattleArenaComplete(m_mapEntries); 
	}
}

void GBattleArena::OnPrepareUpdate(float fDelta)
{
}

void GBattleArena::OnCountingUpdate(float fDelta)
{
	if (!m_rgrStartingCounter.IsReady(fDelta))
		return;

	ChangeStatus(PLAYING);
}

void GBattleArena::OnPlayingUpdate(float fDelta)
{
}

void GBattleArena::OnScoreBoardUpdate( float fDelta )
{
	if (!m_rgrScoreBoardCounter.IsReady(fDelta))
		return;

	ChangeStatus(FINISH);
}

void GBattleArena::OnFinishUpdate( float fDelta )
{

}
const BattleArena::Team* GBattleArena::FindTeam(MUID uidTeam) const
{
	BattleArena::TEAM_MAP::const_iterator it = m_mapEntries.find(uidTeam);
	if (it == m_mapEntries.end())
		return NULL;

	return &(it->second);
}

void GBattleArena::InitGameRules()
{
	m_pGameRule->Init(m_vecStaringMarkers, m_mapEntries);
}

bool GBattleArena::CheckGameRules(MUID& uidWinner)
{
	return m_pGameRule->IsFinished(m_mapEntries, uidWinner);
}

void GBattleArena::UpdateGameRules( float fDelta )
{
	m_pGameRule->Update(fDelta);
}

void GBattleArena::OnTeamMemberDie( GEntityPlayer* pPlayer, const BattleArena::Team& team )
{
	if (GetStatus() != PLAYING)
		return;	// �������϶��� ��ȿ

	m_pGameRule->OnTeamMemberDie(pPlayer, team);
	ProcessFinish();
}

void GBattleArena::OnRebirthTeamMemberBySelf( GEntityPlayer* pPlayer, const BattleArena::Team& team )
{
	if (GetStatus() != PLAYING)
		return;	// �������϶��� ��ȿ

	m_pGameRule->OnRebirthTeamMemberBySelf(pPlayer, team);
	ProcessFinish();
}

void GBattleArena::OnRebirthTeamMemberByOther( GEntityPlayer* pPlayer, const BattleArena::Team& team )
{
	if (GetStatus() != PLAYING)
		return;	// �������϶��� ��ȿ

	m_pGameRule->OnRebirthTeamMemberByOther(pPlayer, team);
	ProcessFinish();
}

void GBattleArena::OnDisconnect( GEntityPlayer* pPlayer )
{
	VALID(pPlayer);

	m_pGameRule->OnDisconnect(pPlayer);
	ProcessFinish();
}

void GBattleArena::OnDeleteTeam( const BattleArena::Team& team )
{
	m_mapEntries.erase(team.UID);

	m_pGameRule->OnDeleteTeam(team);
	ProcessFinish();
}

void GBattleArena::OnRejoin( GEntityPlayer* pPlayer )
{
	VALID(pPlayer);

	GBattleArena::Status nStatus = GetStatus();

	// ���� ��Ģ�� �´� ���� ����
	m_pGameRule->OnRejoin(nStatus, pPlayer);
	ProcessFinish();

	// ������ ���� ���� ����
	switch(nStatus)
	{
	case COUNTING:	
		{
			// �����ð� ��ŭ�� ���� ī��Ʈ ��Ŷ�� ����
			pPlayer->RouteToMe(MakeNewCommand(MC_BATTLEARENA_COUNTING, 1, NEW_FLOAT(GConst::ARENA_GAME_COUNT-m_rgrStartingCounter.GetElapsedTime())));
		} break;	
	case PREPARE:		{} break;	// �غ����϶��� ���� (�ٸ� �÷��̾ �������̹Ƿ�)
	case PLAYING:		{} break;	// �������϶��� ���ٸ� ó�� ���� (��Ģ���� ��κ� ó��)
	case SCOREBOARD:	{} break;	// ������ ���� �߿��� ���� �Ұ�
	case FINISH:		{} break;	// ����Ǵ� �߿��� ���� �Ұ�
	}
}

vec3 GBattleArena::GetSpotPos( GEntityPlayer* pPlayer, const BattleArena::Team& team )
{
	VALID_RET(pPlayer, vec3::ZERO);

	return m_pGameRule->GetSpotPos(pPlayer, team);
}

void GBattleArena::RouteToTeam( const BattleArena::Team& team, MCommand* pCommand )
{
	VALID(pCommand);
	GetPartySys()->RouteToAllMembers(team.UID, pCommand);
}

void GBattleArena::RouteToAllTeam( MCommand* pCommand )
{
	VALID(pCommand);
	for (BattleArena::TEAM_MAP::iterator it = m_mapEntries.begin();
		it != m_mapEntries.end();
		++it)
	{
		BattleArena::Team& team = it->second;
		RouteToTeam(team, pCommand->Clone());
	}
	SAFE_DELETE(pCommand);
}

bool GBattleArena::MoveAllTeamToTrialField()
{
	VALID_RET(!m_vecStaringMarkers.empty(), false);	// ������ �ʵ� ����

	bool bFirstPlayer = true;

	int nCount = 0;
	for (BattleArena::TEAM_MAP::iterator it = m_mapEntries.begin();
		it != m_mapEntries.end();
		++it)
	{
		BattleArena::Team& team = it->second;
		_ASSERT(team.uidParty.IsValid());

		GParty* pParty = gsys.pPartySystem->FindParty(team.uidParty);
		_ASSERT(pParty != NULL);
		if (pParty == NULL)	continue;

		for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
		{
			MUID uidMember = it->first;

			GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidMember);
			if (pPlayer == NULL)	continue;


			const MARKER_INFO* pStartingMarker = m_vecStaringMarkers[nCount];

			GATE_INFO_DYNAMIC sGateInfo(DFT_BATTLEARENA, GConst::ARENA_FIELDGROUP_ID, GConst::ARENA_FIELD_ID, pStartingMarker->vPoint, pStartingMarker->vDir);
			pPlayer->GetPlayerField().GetFieldDynamic().GateRequest(&sGateInfo);
			
			if (bFirstPlayer)
			{
				bFirstPlayer = false;

				MUID uidCreatedBattleArenaField = pPlayer->GetPlayerField().GetDynamicFieldGroupUID();

				GDynamicFieldMaster* pDynamicFieldMaster = gmgr.pFieldMgr->GetDynamicFieldMaster(uidCreatedBattleArenaField);
				if (NULL == pDynamicFieldMaster) return false;

				GDynamicFieldPolicy_BattleArena* pBattleArenaPolicy = static_cast<GDynamicFieldPolicy_BattleArena*>(pDynamicFieldMaster->GetPolicy());
				RegisterLisener(pBattleArenaPolicy);

				SetFieldGroupUID(uidCreatedBattleArenaField);
			}
		}

		++nCount;
	}

	return true;
}

bool GBattleArena::MoveAllTeamToEnterPos()
{
	// ���� �ʵ� ��ġ�� �̵�
	for (BattleArena::TEAM_MAP::iterator it = m_mapEntries.begin();
		it != m_mapEntries.end();
		++it)
	{
		BattleArena::Team& team = it->second;
		_ASSERT(team.uidParty.IsValid());

		GParty* pParty = gsys.pPartySystem->FindParty(team.uidParty);
		_ASSERT(pParty != NULL);
		if (pParty == NULL)	continue;

		for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
		{
			MUID uidMember = it->first;
			GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidMember);
			if (pPlayer == NULL)	continue;

			pPlayer->GetPlayerField().GetFieldDynamic().GateToEnterPos(pPlayer->GetFieldID());
		}
	}

	return true;
}

void GBattleArena::RebirthToAllTeam()
{
	// ���� �ʵ� ��ġ�� �̵�
	for (BattleArena::TEAM_MAP::iterator it = m_mapEntries.begin();
		it != m_mapEntries.end();
		++it)
	{
		BattleArena::Team& team = it->second;
		_ASSERT(team.uidParty.IsValid());
		
		GParty* pParty = gsys.pPartySystem->FindParty(team.uidParty);
		_ASSERT(pParty != NULL);
		if (pParty == NULL)	continue;

		for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
		{
			MUID uidMember = it->first;
			GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidMember);
			if (pPlayer == NULL)	continue;
		
			gsys.pRebirthSystem->AutoRebirthInBattleArena(pPlayer, pPlayer->GetPos(), pPlayer->GetDir());
		}
	}
}

void GBattleArena::FullHealToAllTeam()
{
	// ���� �ʵ� ��ġ�� �̵�
	for (BattleArena::TEAM_MAP::iterator it = m_mapEntries.begin();
		it != m_mapEntries.end();
		++it)
	{
		BattleArena::Team& team = it->second;
		_ASSERT(team.uidParty.IsValid());

		GParty* pParty = gsys.pPartySystem->FindParty(team.uidParty);
		_ASSERT(pParty != NULL);
		if (pParty == NULL)	continue;

		for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
		{
			MUID uidMember = it->first;
			GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidMember);
			if (pPlayer == NULL)	continue;

			pPlayer->SetHP(pPlayer->GetMaxHP());
			pPlayer->SetEN(pPlayer->GetMaxEN());
			pPlayer->SetSTA(pPlayer->GetMaxSTA());
			pPlayer->RouteSimpleStatus(pPlayer->GetUID()); // �ڱ� �ڽſ��� ������ �˷���
		}
	}
}

void GBattleArena::SetBattleArenaUIDToTeam( BattleArena::Team &team )
{
	VALID(team.uidParty.IsValid());

	team.uidBattleArena = GetUID();

	GParty* pParty = gsys.pPartySystem->FindParty(team.uidParty);
	VALID(pParty != NULL);

	for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
	{
		MUID uidMember = it->first;
		GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidMember);
		if (pPlayer == NULL)	continue;

		pPlayer->GetPlayerBattleArena().SetBattleArenaUID(GetUID());
	}
}

void GBattleArena::ClearBattleArenaUIDToTeam()
{
	for (BattleArena::TEAM_MAP::iterator it = m_mapEntries.begin();
		it != m_mapEntries.end();
		++it)
	{
		BattleArena::Team& team = it->second;
		_ASSERT(team.uidParty.IsValid());
		
		team.uidBattleArena.SetInvalid();

		GParty* pParty = gsys.pPartySystem->FindParty(team.uidParty);
		_ASSERT(pParty != NULL);
		if (pParty == NULL)	continue;

		for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
		{
			MUID uidMember = it->first;
			GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidMember);
			if (pPlayer == NULL)	continue;

			pPlayer->GetPlayerBattleArena().SetBattleArenaUID(MUID::Invalid());
		}		
	}
}

void GBattleArena::ExtractStartingMarkers()
{
	m_vecStaringMarkers.clear();
	GFieldInfo* pFieldInfo = gmgr.pFieldInfoMgr->Find(GConst::ARENA_FIELD_ID);
	VALID(pFieldInfo);
	for (size_t i=0; i<m_mapEntries.size(); ++i)
	{
		const MARKER_INFO* pMarker = pFieldInfo->FindMarker(i+1);
		VALID(pMarker);
		m_vecStaringMarkers.push_back(pMarker);
	}
}

void GBattleArena::ProcessFinish()
{
	MUID uidWinner;
	if (CheckGameRules(uidWinner))
	{
		m_uidWinner = uidWinner;
		ChangeStatus(SCOREBOARD);
	}
}

bool GBattleArena::IsGamePreparing() const
{
	return (GetStatus() == PREPARE) || (GetStatus() == COUNTING);
}

bool GBattleArena::IsGamePlaying() const
{
	return (GetStatus() == PLAYING);
}

bool GBattleArena::IsGameFinishing() const
{
	return (GetStatus() == SCOREBOARD) || (GetStatus() == FINISH);
}