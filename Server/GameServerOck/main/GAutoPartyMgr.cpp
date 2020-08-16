#include "StdAfx.h"
#include "GAutoPartyMgr.h"
#include "GQuestInfo.h"
#include "GQuestInfoMgr.h"
#include "GFieldInfoMgr.h"
#include "GGlobal.h"
#include "GEntityPlayer.h"
#include "GPlayerQuests.h"
#include "GServer.h"
#include "CCommandTable.h"
#include "GCommand.h"
#include "GCommandCenter.h"
#include "GConst.h"
#include "GPartySystem.h"
#include "GField.h"
#include "GQuestInfoMgr.h"
#include "GQuestInfo.h"
#include "GPlayerRole.h"
#include "GPlayerAFK.h"


GAutoPartyMgr::PartymemberCountMap GAutoPartyMgr::s_mapPartymemberCount;


GAutoPartyMgr::GAutoPartyMgr(GField* pField)
: m_pField(pField)
{
	m_rgrMatchProcess.SetTickTime(GConst::AUTOPARTY_MATCH_PROCESS_INTERVAL);
	m_rgrMatchProcess.Start();
}

GAutoPartyMgr::~GAutoPartyMgr(void)
{
}

void GAutoPartyMgr::Cooking()
{
	vector<GQuestInfo*> vecQuestInfos = gmgr.pQuestInfoMgr->GetAllQuestInfo();
	for each (GQuestInfo* each in vecQuestInfos)
	{
		// ����Ʈ�� �ڵ� ��Ƽ ����
		each->bAutoParty = true;

		// ������ �ڵ� ��Ƽ ����
		GFieldInfo* pFieldInfo = gmgr.pFieldInfoMgr->Find(each->nPartyFieldID);
		if (pFieldInfo)
		{
			const SENSOR_INFO* pSensorInfo = pFieldInfo->FindSensor(each->nPartySensorID);
			if (!pSensorInfo)
			{
				mlog3("%s - invalid sensor(%d)\n", __FUNCTION__, each->nPartySensorID);
				continue; // ������ ���� ���
			}

			SENSOR_INFO new_info = *pSensorInfo;
			new_info.bAutoParty = true;

			pFieldInfo->ChangeSensorSetting(new_info);
		}

		// �ڵ� ��Ƽ �ʿ� �ο� ����
		s_mapPartymemberCount.insert(make_pair(each->nID, each->nAutoPartyMemberLimit));


	}
}

void GAutoPartyMgr::ResetStatics()
{
	s_mapPartymemberCount.clear();
}

int GAutoPartyMgr::GetPartyMemberLimit( QuestID nQuestID )
{
	PartymemberCountMap::const_iterator it = s_mapPartymemberCount.find(nQuestID);
	if (it == s_mapPartymemberCount.end())
		return MAX_PARTY_MEMBER_COUNT;

	return it->second;
}

bool GAutoPartyMgr::Enqueue( QuestID nQuestID, GEntityPlayer* pPlayer )
{
	VALID_RET(pPlayer, false);
	MUID uidPlayer = pPlayer->GetUID();

	GQuestInfo* pQuestInfo = gmgr.pQuestInfoMgr->Get(nQuestID);
	VALID_RET(pQuestInfo, false);	// ����Ʈ�� �������� ����
	if (!pQuestInfo->bAutoParty)
		return false; // �ڵ���Ƽ�� ����Ʈ�� �ƴ�

	if (pPlayer->HasParty())
	{
		return EnqueueParty(nQuestID, pPlayer);
	}
	else
	{
		return EnqueueSingle(nQuestID, pPlayer);
	}

	return true;
}

bool GAutoPartyMgr::EnqueueSingle( QuestID nQuestID, GEntityPlayer* pPlayer )
{
	VALID_RET(pPlayer, false);
	MUID uidPlayer = pPlayer->GetUID();

	WaiterMap::iterator it = m_maptWaiters.find(nQuestID);
	if (it == m_maptWaiters.end())
	{
		WaiterGroup waiter_group;
		waiter_group.setSingles.insert(uidPlayer);
		m_maptWaiters.insert(std::make_pair(nQuestID, waiter_group));
		OnEnqueue(nQuestID, pPlayer);

		return true;
	}

	WaiterGroup& waiter_group = it->second;
	if (waiter_group.setSingles.find(uidPlayer) != waiter_group.setSingles.end())
		return false; // �̹� ��ϵǾ� ����

	// ���� ���
	waiter_group.setSingles.insert(uidPlayer);
	OnEnqueue(nQuestID, pPlayer);

	return true;
}


bool GAutoPartyMgr::EnqueueParty( QuestID nQuestID, GEntityPlayer* pPlayer )
{
	PartyUID uidParty = pPlayer->GetPartyUID();
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (!pParty)
		return false; // ��Ƽ ����

	if (!pParty->IsLeader(pPlayer->GetUID()))
		return false; // ��Ƽ ������ �ƴ�

	WaiterMap::iterator it = m_maptWaiters.find(nQuestID);
	if (it == m_maptWaiters.end())
	{
		WaiterGroup waiter_group;
		waiter_group.setParties.insert(uidParty);
		m_maptWaiters.insert(std::make_pair(nQuestID, waiter_group));
		OnEnqueue(nQuestID, pPlayer);

		return true;
	}

	WaiterGroup& waiter_group = it->second;
	if (waiter_group.setParties.find(uidParty) != waiter_group.setParties.end())
		return false; // �̹� ��ϵǾ� ����

	// ���� ���
	waiter_group.setParties.insert(uidParty);
	OnEnqueue(nQuestID, pPlayer);

	return true;
}



bool GAutoPartyMgr::Dequeue( QuestID nQuestID, GEntityPlayer* pPlayer )
{
	VALID_RET(pPlayer, false);
	
	if (pPlayer->HasParty())
	{
		return DequeueParty(nQuestID, pPlayer);
	}
	else
	{
		return DequeueSingle(nQuestID, pPlayer);
	}	

	return true;
}

bool GAutoPartyMgr::DequeueSingle( QuestID nQuestID, GEntityPlayer* pPlayer )
{
	VALID_RET(pPlayer, false);
	MUID uidPlayer = pPlayer->GetUID();

	WaiterMap::iterator it = m_maptWaiters.find(nQuestID);
	if (it == m_maptWaiters.end())
		return false; // ��ϵ��� ���� ����Ʈ

	WaiterGroup& waiter_group = it->second;
	waiter_group.setSingles.erase(uidPlayer);
	OnDequeue(nQuestID, pPlayer);

	return true;
}

bool GAutoPartyMgr::DequeueParty( QuestID nQuestID, GEntityPlayer* pPlayer )
{
	VALID_RET(pPlayer, false);
	MUID uidPlayer = pPlayer->GetUID();

	WaiterMap::iterator it = m_maptWaiters.find(nQuestID);
	if (it == m_maptWaiters.end())
		return false; // ��ϵ��� ���� ����Ʈ

	WaiterGroup& waiter_group = it->second;
	waiter_group.setSingles.erase(uidPlayer);
	OnDequeue(nQuestID, pPlayer);

	return true;
}

void GAutoPartyMgr::Clear()
{
	m_maptWaiters.clear();
}

GAutoPartyMgr::WaiterGroup* GAutoPartyMgr::GetEntries( QuestID nQuestID )
{
	WaiterMap::iterator it = m_maptWaiters.find(nQuestID);
	if (it == m_maptWaiters.end())
		return NULL;

	return &it->second;
}

int GAutoPartyMgr::GetSingleCount(QuestID nQuestID) const
{
	WaiterMap::const_iterator it = m_maptWaiters.find(nQuestID);
	if (it == m_maptWaiters.end())
		return 0;

	return it->second.setSingles.size();
}

int GAutoPartyMgr::GetSingleCountAll() const
{
	int nTotalCount = 0;
	for each (const WaiterMap::value_type& each in m_maptWaiters)
	{
		nTotalCount += each.second.setSingles.size();
	}

	return nTotalCount;
}

int GAutoPartyMgr::GetPartyCount(QuestID nQuestID) const
{
	WaiterMap::const_iterator it = m_maptWaiters.find(nQuestID);
	if (it == m_maptWaiters.end())
		return 0;

	return it->second.setParties.size();
}

int GAutoPartyMgr::GetPartyCountAll() const
{
	int nTotalCount = 0;
	for each (const WaiterMap::value_type& each in m_maptWaiters)
	{
		nTotalCount += each.second.setParties.size();
	}

	return nTotalCount;
}


void GAutoPartyMgr::OnEnqueue( QuestID nQuestID, GEntityPlayer* pPlayer )
{
	VALID(pPlayer);

	TestNarration(pPlayer, _T("AParty: ��ϵ� (����Ʈ: %d, ��������: %d, ����ο�: %d)"), nQuestID, pPlayer->GetRole().IsHealer(), GetSingleCount(nQuestID));

	ProcessMatching(nQuestID);

}

void GAutoPartyMgr::OnDequeue( QuestID nQuestID, GEntityPlayer* pPlayer )
{
	VALID(pPlayer);
	TestNarration(pPlayer, _T("AParty: ��� ��ҵ� (����Ʈ: %d, ��������: %d, ����ο�: %d)"), nQuestID, pPlayer->GetRole().IsHealer(), GetSingleCount(nQuestID));
}

void GAutoPartyMgr::OnSinglesMatched( QuestID nQuestID, PlayerUIDList setEntries )
{
	for each (PlayerUID each in setEntries)
	{
		EraseSingle(each);
	}

	if (GConst::LOG_AUTOPARTY)
	{
		for each (PlayerUID each in setEntries)
		{
			GEntityPlayer* pPlayer = FindPlayer(each);
			if (!pPlayer)
				continue;

			TestNarration(pPlayer, _T("AParty: ��Ƽ ��ġ�� (����Ʈ: %d, ���� ����ο�: %d)"), nQuestID, GetSingleCount(nQuestID));
		}
	}

	gsys.pPartySystem->CreateAutoPartyReq(nQuestID, setEntries);
}

void GAutoPartyMgr::OnPartyMemberAdded( GParty* pParty, QuestID nQuestID, PlayerUIDList setEntries )
{
	VALID(pParty);

	pParty->SetAutoPartyQuestID(nQuestID);
	for each (PlayerUID each in setEntries)
	{
		EraseSingle(each);

		GEntityPlayer* pPlayer = FindPlayer(each);
		if (!pPlayer)
			continue;

		MUID uidParty = pParty->GetUID();		
		gsys.pPartySystem->JoinAutoPartyReq(uidParty, pPlayer);

		if (GConst::LOG_AUTOPARTY)
		{
			TestNarration(pPlayer, _T("AParty: ��Ƽ �߰� ������ (����Ʈ: %d, ���� ����ο�: %d)"), nQuestID, GetSingleCount(nQuestID));
		}
	}
}

bool GAutoPartyMgr::ProcessMatchedImpl( QuestID nQuestID, PlayerUIDList& listMatchedPlayers, int nExistMemberCount, int nExistHealerCount )
{
	WaiterGroup* pEntries = 
		GetEntries(nQuestID);

	if (!pEntries)
		return false; // �ش�����Ʈ ����ڰ� ����

	const int nMemberLimit =
		GetPartyMemberLimit(nQuestID) - nExistMemberCount;

	PartyUIDSet::const_iterator it = pEntries->setSingles.begin();

	const int nNeedHealerCount = 
		GetNeedHealerCount(nMemberLimit);

	int nHealerCount = nExistHealerCount;

	while(true)
	{
		if (it == pEntries->setSingles.end())
			break; // ��ȸ �Ϸ�

		if ((int)listMatchedPlayers.size() >= nMemberLimit &&
			nHealerCount >= nNeedHealerCount)
		{
			return true; // ��ġ�ο� Ȯ��
		}

		GEntityPlayer* pPlayer = FindPlayer(*it);
		if (!pPlayer)
		{
			// ��ȿ���� ���� �÷��̾�
			it = pEntries->setSingles.erase(it);
			continue;
		}

		if (pPlayer->HasParty())
		{
			// ���߿� ��Ƽ�� ���ܹ��� �÷��̾�
			it = pEntries->setSingles.erase(it);
			continue;
		}

		if (pPlayer->GetAFK().IsNowAFK())
		{
			// �ڸ���� ����
			++it;
			continue;
		}

		if (pPlayer->GetRole().IsHealer())
		{
			++nHealerCount;
			listMatchedPlayers.push_back(*it); // ������ ������ �߰�
		}
		else
		{
			if ((int)listMatchedPlayers.size()+nNeedHealerCount < nMemberLimit)
				listMatchedPlayers.push_back(*it); // ���� ����� ���� ������ �߰�
		}

		++it;
	}

	if (GConst::LOG_AUTOPARTY)
	{
		for each (MUID uid in pEntries->setSingles)
		{
			GEntityPlayer* pPlayer = FindPlayer(uid);
			if (pPlayer)
			{
				TestNarration(pPlayer, _T("AParty: ��Ƽ��ġ���� (����Ʈ: %d, �ʿ��ο�: %d, ����ο�: %d, ����: %d)"), nQuestID, nMemberLimit, GetSingleCount(nQuestID), nHealerCount);
			}
		}
	}

	return ((int)listMatchedPlayers.size() == nMemberLimit);
}

void GAutoPartyMgr::ProcessMatchedSingles( QuestID nQuestID )
{
	if (GetSingleCount(nQuestID) < GetPartyMemberLimit(nQuestID))
		return; // ��Ƽ ��ġ �ο��� ���ġ ����

	PlayerUIDList listMatchedPlayers;
	if (ProcessMatchedImpl(nQuestID, listMatchedPlayers))
	{
		OnSinglesMatched(nQuestID, listMatchedPlayers);	
	}
}

void GAutoPartyMgr::ProcessMatchedParties( QuestID nQuestID )
{
	WaiterGroup* pEntries = 
		GetEntries(nQuestID);

	if (!pEntries)
		return; // �ش�����Ʈ ����ڰ� ����

	const int nMemberLimit =
		GetPartyMemberLimit(nQuestID);

	for (PartyUIDSet::iterator it = pEntries->setParties.begin();
		 it != pEntries->setParties.end();)
	{
		GParty* pParty = gsys.pPartySystem->FindParty(*it);
		if (!pParty)
		{
			// ������ ��Ƽ
			it = pEntries->setParties.erase(it);
			continue; 
		}

		GEntityPlayer* pLeader = 
			FindPlayer(pParty->GetLeader());
		if (!pLeader)
		{
			// ��ȿ���� ���� ��Ƽ��
			it = pEntries->setParties.erase(it);
			continue; 
		}

		if (pLeader->GetAFK().IsNowAFK())
		{
			// ��Ƽ�� �ڸ����
			++it;
			continue;
		}

		int nMemberCount = pParty->GetMemberCount();
		if (nMemberCount >= nMemberLimit)
		{
			// �ʿ��ο����� ����
			it = pEntries->setParties.erase(it);
			continue; 
		}
		
		int nHealerCount = 0;
		PlayerUIDList listMatchedPlayers;
		for (partymember_iterator itMember = pParty->GetMemberBegin();
			itMember != pParty->GetMemberEnd();
			++itMember)
		{
			GEntityPlayer* pMember = FindPlayer(itMember->first);
			if (!pMember)
				continue;

			if (pMember->GetRole().IsHealer())
				++nHealerCount;
		}

		if (ProcessMatchedImpl(nQuestID, listMatchedPlayers, nMemberCount, nHealerCount))
		{
			OnPartyMemberAdded(pParty, nQuestID, listMatchedPlayers);	
		}

		++it;

	}
}

int GAutoPartyMgr::GetNeedHealerCount(int nMemberLimit)
{
	if (nMemberLimit >= 4)	
		return 1; // 4�� �̻��� ���� 1��

	// �������� ���� ��� ��
	return 0;
}

GEntityPlayer* GAutoPartyMgr::FindPlayer( const PlayerUID uidPlayer )
{
	return m_pField->FindPlayer(uidPlayer);
}

void GAutoPartyMgr::Update( float fDelta )
{
	float fElapsedTime = m_rgrMatchProcess.GetElapsedTime() + fDelta;
	if (!m_rgrMatchProcess.IsReady(fDelta))
		return; // ƽ�� ���� ����

	ProcessAllMatchings();
}

#include "GStrings.h"

void GAutoPartyMgr::TestNarration(GEntityPlayer* pPlayer, const wchar_t* pFormat, ...)
{
	if (!GConst::LOG_AUTOPARTY)
		return;

	VALID(pPlayer);

	wchar_t temp[1024];
	va_list args;
	va_start(args,pFormat);
	vswprintf_s(temp,pFormat,args);
	va_end(args);

	MCommand* pNewCommand = MakeNewCommand(	MC_NPC_NARRATION, 
		1, 
		NEW_WSTR(temp));

	pPlayer->RouteToMe(pNewCommand);

	mlog("%s\n", MLocale::ConvUTF16ToAnsi(temp).c_str());
}

void GAutoPartyMgr::EraseSingle( PlayerUID uidPlayer )
{
	for (WaiterMap::iterator itWaiter = m_maptWaiters.begin();
		itWaiter != m_maptWaiters.end();
		++itWaiter)
	{
		PlayerUIDSet& setPlayers = itWaiter->second.setSingles;
		setPlayers.erase(uidPlayer);
	}
}

AUTOPARTY_STATE GAutoPartyMgr::GetAutopartyState( int nQuestID, GEntityPlayer* pPlayer )
{
	if (pPlayer->GetQuests().IsCompleteQuest(nQuestID))
		return AUTOPARTY_PAUSE; // ����Ʈ�� ������ ���� ����

	return AUTOPARTY_LOOKUP;
}

void GAutoPartyMgr::RouteCheckSensor( GEntityPlayer* pPlayer, int nQuestID, AUTOPARTY_STATE nState )
{
	VALID(pPlayer);
	MCommand* pNewCommand = MakeNewCommand(	MC_AUTOPARTY_CHECK_SENSOR, 
		2, 
		NEW_INT(nQuestID),
		NEW_INT(nState));

	pPlayer->RouteToMe(pNewCommand);
}

void GAutoPartyMgr::ChangeAutopartyState( GEntityPlayer* pPlayer, int nQuestID, AUTOPARTY_STATE nState )
{
	VALID(pPlayer);
	if (!pPlayer->HasParty())
		return; // ��Ƽ ����

	MCommand* pNewCommand = MakeNewCommand(	MC_AUTOPARTY_CHANGE_STATE, 
		2, 
		NEW_INT(nQuestID),
		NEW_INT(nState));

	pPlayer->RouteToParty(pNewCommand);
}

void GAutoPartyMgr::ProcessMatching( QuestID nQuestID )
{
	// ���� ��Ƽ ����
	ProcessMatchedParties(nQuestID);

	// ���� ��Ƽ ����
	ProcessMatchedSingles(nQuestID);
}

void GAutoPartyMgr::ProcessAllMatchings()
{
	for each (const WaiterMap::value_type& each in m_maptWaiters)
	{
		ProcessMatching(each.first);
	}	
}
