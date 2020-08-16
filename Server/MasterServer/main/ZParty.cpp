#include "stdafx.h"
#include "ZParty.h"
#include "ZPartyMember.h"
#include "CSDef_Loot.h"

ZParty::ZParty(MUID uidParty, MUID uidLeader, wstring strLeaderName, int nLeaderServerID, int nLeaderCID)
: m_uidParty(uidParty)
, m_isSyncReady(false)
{
	m_partySetting.m_uidLeader = uidLeader;	

	ZPartyMember* pLeader = new ZPartyMember(uidLeader, strLeaderName, nLeaderServerID, nLeaderCID);
	_ASSERT(pLeader != NULL);
	m_mapMembers.insert(make_pair(uidLeader, pLeader));
}

ZParty::~ZParty()
{
	for each(map_member::value_type member in m_mapMembers)
	{
		ZPartyMember* pPartyMember = member.second;
		pPartyMember->Drop();
	}

	m_mapMembers.clear();
}

ZPartyMember* ZParty::AddMember(MUID uidMember, wstring strMemberName, int nMemberServerID, int nMemberCID)
{	
	if (IsExistMember(uidMember))		return NULL;

	ZPartyMember* pMember = new ZPartyMember(uidMember, strMemberName, nMemberServerID, nMemberCID);
	_ASSERT(pMember != NULL);
	
	m_mapMembers.insert(make_pair(uidMember, pMember));

	return pMember;
}

void ZParty::RemoveMember(MUID uidMember)
{
	if (uidMember == m_partySetting.m_uidLeader)		return;
	
	member_iterator it = m_mapMembers.find(uidMember);
	if (it == m_mapMembers.end())		return;

	ZPartyMember* pPartyMember = it->second;
	pPartyMember->Drop();

	m_mapMembers.erase(it);
}

void ZParty::ChangeLeader(MUID uidNewLeader)
{
	// ��Ƽ�� Ȯ��
	ZPartyMember* pPartyMember = GetMember(uidNewLeader);
	if (pPartyMember == NULL)			return;

	// �¶��� ������ ��
	if (pPartyMember->IsOffline())		return;

	// ����
	m_partySetting.m_uidLeader = uidNewLeader;
}

MUID ZParty::GetUID(void) const
{
	return m_uidParty;
}

MUID ZParty::GetLeader(void) const
{
	return m_partySetting.m_uidLeader;
}

int ZParty::GetCount(void) const
{
	return m_mapMembers.size();
}

bool ZParty::IsFull(void) const
{
	return MAX_PARTY_MEMBER_COUNT <= m_mapMembers.size();
}

bool ZParty::IsEmpty(void) const
{
	int nOfflineCount = GetOfflineMemberCount();
	return m_mapMembers.size() == nOfflineCount;
}

ZParty::member_iterator ZParty::BeginMember(void) const
{
	return m_mapMembers.begin();
}

ZParty::member_iterator ZParty::EndMember(void) const
{
	return m_mapMembers.end();
}

ZParty::member_iterator ZParty::FindMember(MUID uidMember) const
{
	return m_mapMembers.find(uidMember);
}

ZPartyMember* ZParty::FindMember(wstring strName) const
{
	for(member_iterator it = m_mapMembers.begin(); it != m_mapMembers.end(); it++)
	{
		ZPartyMember* pMember = it->second;
		if (strName == pMember->GetName())
		{
			return pMember;
		}
	}

	return NULL;
}

bool ZParty::IsExistMember(MUID uidMember) const
{
	member_iterator it = m_mapMembers.find(uidMember);
	return it != m_mapMembers.end();
}

ZPartyMember* ZParty::GetMember(MUID uidMember) const
{
	member_iterator it = m_mapMembers.find(uidMember);
	if (it == m_mapMembers.end())	return NULL;

	return it->second;
}

MUID ZParty::GetFirstOfflineMember() const
{
	for each(const pair<MUID, ZPartyMember*>& member in m_mapMembers)
	{
		if (member.second->IsOffline())
		{
			return member.first;
		}
	}

	return MUID::Invalid();
}

MUID ZParty::GetFieldGroupUID(int nFieldGroupID)
{
	for each(map_member::value_type member in m_mapMembers)
	{
		ZPartyMember* pMember = member.second;
		if (nFieldGroupID == pMember->GetFieldGroupID())
		{
			return pMember->GetFieldGroupUID();
		}
	}

	return MUID::Invalid();
}

void ZParty::RemoveFieldSelf(int nFieldGroupID)
{
	for each(map_member::value_type member in m_mapMembers)
	{
		ZPartyMember* pMember = member.second;
		if (nFieldGroupID == pMember->GetFieldGroupID())
		{
			pMember->SetFieldGroup(-1, MUID::Invalid());
		}
	}
}

MUID ZParty::GetAloneMember(void) const
{
	if (1 != GetOnlineMemberCount())	return MUID::Invalid();

	for each(map_member::value_type pairMember in m_mapMembers)
	{
		MUID uidMember = pairMember.first;
		ZPartyMember* pPartyMember = pairMember.second;

		if (pPartyMember->IsOffline()) continue;

		return uidMember;
	}

	return MUID::Invalid();
}

MUID ZParty::FindCandidateForLeader(void) const
{
	if (GetOnlineMemberCount() <= 1)	return MUID::Invalid();

	for each(map_member::value_type pairMember in m_mapMembers)
	{
		MUID uidMember = pairMember.first;
		ZPartyMember* pPartyMember = pairMember.second;
		
		if (uidMember == m_partySetting.m_uidLeader)	continue;		
		if (pPartyMember->IsOffline())	continue;

		return uidMember;
	}

	return MUID::Invalid();
}

int ZParty::GetOnlineMemberCount(void) const
{
	int nOfflineCount = GetOfflineMemberCount();
	return m_mapMembers.size() - nOfflineCount;
}

int ZParty::GetOfflineMemberCount(void) const
{
	int nCount = 0;

	member_iterator it = m_mapMembers.begin();
	while(it != m_mapMembers.end())
	{
		ZPartyMember* pPartyMember = it->second;
		if (pPartyMember->IsOffline())
		{
			nCount++;
		}

		it++;
	}

	return nCount;
}

void ZParty::ChangeMemberUID(MUID uidOldMember, MUID uidNewMember)
{
	// �ٸ� ��Ƽ���� ��ġ�� ���� ��
	if (IsExistMember(uidNewMember))	return;


	// ���� ��Ƽ���� ��
	member_iterator it = m_mapMembers.find(uidOldMember);
	if (it == m_mapMembers.end())	return;

	ZPartyMember* pPartyMember = it->second;
	pPartyMember->SetUID(uidNewMember);
	
	m_mapMembers.erase(it);
	m_mapMembers.insert(make_pair(uidNewMember, pPartyMember));


	// ��Ƽ�� ����
	if (m_partySetting.m_uidLeader == uidOldMember)
	{
		m_partySetting.m_uidLeader = uidNewMember;
	}
}

bool ZParty::IsExistAllOnlineMembersIn(int nGameServerID) const
{
	for each(map_member::value_type member in m_mapMembers)
	{
		MUID uidMember = member.first;
		ZPartyMember* pPartyMember = member.second;

		if (pPartyMember->IsOffline())	continue;

		if (nGameServerID != pPartyMember->GetGameServerID())
		{
			return false;
		}
	}

	return true;
}

MUID ZParty::FindCandidateForLeaderOtherServer(void) const
{
	if (GetOnlineMemberCount() <= 1)	return MUID::Invalid();

	int nLeaderServerID = -1;

	ZPartyMember* pPartyMember = GetMember(m_partySetting.m_uidLeader);
	if (pPartyMember != NULL)
	{
		nLeaderServerID = pPartyMember->GetGameServerID();
	}

	for each(map_member::value_type pairMember in m_mapMembers)
	{
		MUID uidMember = pairMember.first;
		ZPartyMember* pPartyMember = pairMember.second;

		if (uidMember == m_partySetting.m_uidLeader)	continue;		
		if (pPartyMember->IsOffline())	continue;		
		if (nLeaderServerID == pPartyMember->GetGameServerID())	continue;
		
		return uidMember;
	}

	return MUID::Invalid();
}

bool ZParty::IsSyncReady(void)
{
	// ��Ƽ ���� ��, ��� ��Ƽ���� ������ ü���������� Ŀ��尡 ���޵Ǵ� ���� ���´�.
	// ���� �� ���Ӽ����� �̱���Ƽ ���� �ÿ��� �۵��ؾ��Ѵ�.

	// �ѹ� �غ���°��Ǹ� ������� �ʴ´�.
	if (m_isSyncReady)	return true;

	// ��� ����� ������ ü������ �غ���°� �ȴ�.
	for each(map_member::value_type pairMember in m_mapMembers)
	{
		ZPartyMember* pPartyMember = pairMember.second;

		if (!pPartyMember->IsSetted())
		{
			return false;
		}
	}
	
	// ���º���
	m_isSyncReady = true;

	return true;
}

const PARTY_SETTING& ZParty::GetPartySetting() const
{
	return m_partySetting;
}

void ZParty::SetPartySetting(const PARTY_SETTING& partySetting)
{
	m_partySetting = partySetting;
}

void ZParty::SetName( wstring strName )
{	
	wcsncpy_s(m_partySetting.m_szName, strName.c_str(), PARTY_NAME_LEN);
}

void ZParty::SetLootingRule( LOOTING_RULE_DATA rule )
{
	m_partySetting.m_lootingRuleData = rule;
}

void ZParty::SetAutoPartyQuestID(int nQuestID)
{
	m_partySetting.m_nAutoPartyQuestID = nQuestID;
}
