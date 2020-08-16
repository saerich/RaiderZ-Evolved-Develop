#include "stdafx.h"
#include "ZPartyLogic.h"
#include "ZPlayerManager.h"
#include "ZPartyCommandRouter.h"
#include "ZGameServerObjectManager.h"
#include "ZGameServerObject.h"
#include "ZPartyManager.h"
#include "ZParty.h"
#include "ZDynamicFieldMaster.h"
#include "STransData_M2G.h"
#include "ZPartyMember.h"
#include "ZPlayerCommandRouter.h"
#include "CSDef_Party.h"
#include "ZFixedPartyLogic.h"
#include "ZConfig.h"

ZPartyLogic::ZPartyLogic()
{
	m_pRouter = NewRouter();
	m_pFixedPartyLogic = new ZFixedPartyLogic();
}

ZPartyLogic::~ZPartyLogic()
{
	SAFE_DELETE(m_pRouter);
	SAFE_DELETE(m_pFixedPartyLogic);
}

ZPartyCommandRouter* ZPartyLogic::NewRouter(void)
{
	return new ZPartyCommandRouter();
}

void ZPartyLogic::InviteReq(MUID uidSender, MUID uidTargetPlayer, MUID uidRequestPlayer)
{
	// ��� Ȯ��
	ZPlayer* pTargetPlayer = gmgr.pPlayerManager->FindByUID(uidTargetPlayer);
	if (pTargetPlayer == NULL)
	{
		m_pRouter->InviteRes(uidSender, uidRequestPlayer, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}


	// ��� ���� Ȯ��
	int nServerID = pTargetPlayer->GetGameServerID();
	ZGameServerObject* pTargetGameServer = gmgr.pGameServerObjectManager->FindGameServer(nServerID);
	if (pTargetGameServer == NULL)
	{
		m_pRouter->InviteRes(uidSender, uidRequestPlayer, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}
	

	// ��� ��Ƽ Ȯ��
	MUID uidTargetParty = pTargetPlayer->GetPartyUID();
	if (uidTargetParty.IsValid())
	{
		ZParty* pParty = gmgr.pPartyManager->Find(uidTargetParty);
		if (pParty != NULL)
		{
			m_pRouter->InviteRes(uidSender, uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_TARGET_ALEADY_HAS_PARTY);
			return;
		}
	}
	

	// ��û�� Ȯ��
	ZPlayer* pRequestPlayer = gmgr.pPlayerManager->FindByUID(uidRequestPlayer);
	if (pRequestPlayer == NULL)
	{
		m_pRouter->InviteRes(uidSender, uidRequestPlayer, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}
	

	// ��û�� ��Ƽ Ȯ��
	MUID uidRequestParty = pRequestPlayer->GetPartyUID();
	if (uidRequestParty.IsValid())
	{
		ZParty* pParty = gmgr.pPartyManager->Find(uidRequestParty);
		if (pParty != NULL)
		{
			if (uidRequestPlayer != pParty->GetLeader())
			{
				m_pRouter->InviteRes(uidSender, uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_NOT_PARTY_LEADER);
				return;
			}

			if (pParty->IsFull())
			{
				m_pRouter->InviteRes(uidSender, uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_FULL_PARTY);
				return;
			}
		}		
	}


	// �ʴ� ���� ��û
	MUID uidTarget = pTargetGameServer->GetUID();
	m_pRouter->AcceptReq(uidTarget, uidTargetPlayer, uidRequestPlayer, pRequestPlayer->GetPlayerName());
}

void ZPartyLogic::AcceptRes(MUID uidSender, MUID uidRequestPlayer, MUID uidTargetPlayer, CCommandResultTable nResult)
{
	// ��û�� Ȯ��
	ZPlayer* pRequestPlayer = gmgr.pPlayerManager->FindByUID(uidRequestPlayer);
	if (pRequestPlayer == NULL)
	{
		m_pRouter->AcceptCancel(uidSender, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}


	// ��û�� ���� Ȯ��
	int nServerID = pRequestPlayer->GetGameServerID();
	ZGameServerObject* pRequestGameServer = gmgr.pGameServerObjectManager->FindGameServer(nServerID);
	if (pRequestGameServer == NULL)
	{
		m_pRouter->AcceptCancel(uidSender, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}
	

	// ��û�� ��Ƽ Ȯ��
	bool isExistParty = false;
	MUID uidRequestParty = pRequestPlayer->GetPartyUID();	
	if (uidRequestParty.IsValid())
	{
		ZParty* pParty = gmgr.pPartyManager->Find(uidRequestParty);
		if (pParty != NULL)
		{
			isExistParty = true;

			if (uidRequestPlayer != pParty->GetLeader())
			{
				MUID uidTarget = pRequestGameServer->GetUID();
				m_pRouter->InviteRes(uidTarget, uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_NOT_PARTY_LEADER);
				m_pRouter->AcceptCancel(uidSender, uidTargetPlayer, CR_FAIL_PARTY_NOT_PARTY_LEADER);
				return;
			}

			if (pParty->IsFull())
			{
				MUID uidTarget = pRequestGameServer->GetUID();
				m_pRouter->InviteRes(uidTarget, uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_FULL_PARTY);
				m_pRouter->AcceptCancel(uidSender, uidTargetPlayer, CR_FAIL_PARTY_FULL_PARTY);
				return;
			}
		}
	}


	// ��� Ȯ��
	ZPlayer* pTargetPlayer = gmgr.pPlayerManager->FindByUID(uidTargetPlayer);
	if (pTargetPlayer == NULL)
	{
		MUID uidTarget = pRequestGameServer->GetUID();
		m_pRouter->InviteRes(uidTarget, uidRequestPlayer, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		m_pRouter->AcceptCancel(uidSender, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);				
		return;
	}

	
	// ��� ��Ƽ Ȯ��
	MUID uidTargetParty = pTargetPlayer->GetPartyUID();	
	if (uidTargetParty.IsValid())
	{
		ZParty* pParty = gmgr.pPartyManager->Find(uidTargetParty);
		if (pParty != NULL)
		{
			MUID uidTarget = pRequestGameServer->GetUID();
			m_pRouter->InviteRes(uidTarget, uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_TARGET_ALEADY_HAS_PARTY);
			m_pRouter->AcceptCancel(uidSender, uidTargetPlayer, CR_FAIL_PARTY_ALEADY_HAS_PARTY);
			return;
		}
	}


	// �ʴ� ���� ���� Ȯ��
	if (nResult != CR_SUCCESS)
	{
		MUID uidTarget = pRequestGameServer->GetUID();
		m_pRouter->InviteRes(uidTarget, uidRequestPlayer, uidTargetPlayer, nResult);
		return;
	}


	// ��Ƽ ����	
	MUID uidParty = uidRequestParty;
	ZParty* pParty = NULL;

	if (isExistParty)
	{
		pParty = gmgr.pPartyManager->Find(uidParty);
		_ASSERT(pParty != NULL);
	}
	else
	{
		int nLeaderServerID = pRequestPlayer->GetGameServerID();
		pParty = gmgr.pPartyManager->AddParty(uidRequestPlayer, pRequestPlayer->GetPlayerName(), nLeaderServerID, pRequestPlayer->GetCID());
		_ASSERT(pParty != NULL);

		uidParty = pParty->GetUID();
		pRequestPlayer->SetPartyUID(uidParty);
		ZPlayerCommandRouter::RouteProxyPlayerUpdate(pRequestPlayer);
	}	

	int nMemberServerID = pTargetPlayer->GetGameServerID();
	pParty->AddMember(uidTargetPlayer, pTargetPlayer->GetPlayerName(), nMemberServerID, pTargetPlayer->GetCID());
	pTargetPlayer->SetPartyUID(uidParty);
	ZPlayerCommandRouter::RouteProxyPlayerUpdate(pTargetPlayer);


	// �ʴ� ���� ����
	MUID uidTarget = pRequestGameServer->GetUID();
	m_pRouter->InviteRes(uidTarget, uidRequestPlayer, uidTargetPlayer, nResult);


	// ��Ƽ ���� ����ȭ	
	if (!isExistParty)
	{
		m_pRouter->PartyAdd(uidParty, uidRequestPlayer, pRequestPlayer->GetPlayerName(), pRequestPlayer->GetCID());
	}

	m_pRouter->AddMember(uidParty, uidTargetPlayer, pTargetPlayer->GetPlayerName(), pTargetPlayer->GetCID());	
}

void ZPartyLogic::LeaveReq(MUID uidSender, MUID uidParty, MUID uidLeaveMember)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)
	{
		m_pRouter->LeaveRes(uidSender, uidLeaveMember, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}


	// ��Ƽ�� Ȯ��
	if (!pParty->IsExistMember(uidLeaveMember))
	{
		m_pRouter->LeaveRes(uidSender, uidLeaveMember, CR_FAIL_PARTY_NOT_IN_PARTY);
		return;
	}


	// ������ Ȯ��
	ZPlayer* pLeavePlayer = gmgr.pPlayerManager->FindByUID(uidLeaveMember);
	if (pLeavePlayer == NULL)
	{
		m_pRouter->LeaveRes(uidSender, uidLeaveMember, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}
	

	// ������ �¶��� ��Ƽ��
	if (uidLeaveMember == pParty->GetAloneMember())
	{
		gmgr.pPartyManager->RemoveParty(uidParty);
		pLeavePlayer->SetPartyUID(MUID::Invalid());
		ZPlayerCommandRouter::RouteProxyPlayerUpdate(pLeavePlayer);
		m_pRouter->LeaveRes(uidSender, uidLeaveMember, CR_SUCCESS);
		m_pRouter->RemoveMember(uidParty, uidLeaveMember);
		m_pRouter->PartyRemove(uidParty);		
		return;
	}


	// ��Ƽ���� ��� ��Ƽ�� ����
	if (uidLeaveMember == pParty->GetLeader())
	{
		MUID uidNewLeader = pParty->FindCandidateForLeader();
		pParty->ChangeLeader(uidNewLeader);

		m_pRouter->ChangeLeaderRes(uidParty, uidNewLeader);
	}

	// Ż��
	pParty->RemoveMember(uidLeaveMember);

	pLeavePlayer->SetPartyUID(MUID::Invalid());	
	ZPlayerCommandRouter::RouteProxyPlayerUpdate(pLeavePlayer);
	m_pRouter->LeaveRes(uidSender, uidLeaveMember, CR_SUCCESS);
	m_pRouter->RemoveMember(uidParty, uidLeaveMember);


	// ��Ƽ���� ȥ�� ������, �߰� Ż��
	if (1 == pParty->GetCount())
	{
		MUID uidAloneMember = pParty->GetAloneMember();
		gmgr.pPartyManager->RemoveParty(uidParty);

		ZPlayer* pAloneMember = gmgr.pPlayerManager->FindByUID(uidAloneMember);
		if (pAloneMember != NULL)
		{
			pAloneMember->SetPartyUID(MUID::Invalid());
			ZPlayerCommandRouter::RouteProxyPlayerUpdate(pAloneMember);
		}

		m_pRouter->RemoveMember(uidParty, uidAloneMember);
		m_pRouter->PartyRemove(uidParty);
		return;
	}


	// ��Ƽ ����ȭ	
	m_pRouter->EraseQuestRunnerSync(uidParty, uidLeaveMember);
	RouteRemoveMemberSync(pParty);
}

void ZPartyLogic::KickReq(MUID uidSender, MUID uidParty, MUID uidRequestPlayer, MUID uidTargetPlayer)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)
	{
		m_pRouter->KickRes(uidSender, uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}


	// ��û�� Ȯ��
	if (!pParty->IsExistMember(uidRequestPlayer))
	{
		m_pRouter->KickRes(uidSender, uidRequestPlayer, uidTargetPlayer,CR_FAIL_PARTY_NOT_IN_PARTY);
		return;
	}


	// ��û�� ��Ƽ�� ���� Ȯ��
	if (uidRequestPlayer != pParty->GetLeader())
	{
		m_pRouter->KickRes(uidSender, uidRequestPlayer, uidTargetPlayer,CR_FAIL_PARTY_NOT_PARTY_LEADER);
		return;
	}


	// �ڱ��ڽ� ���� ����
	if (uidRequestPlayer == uidTargetPlayer)
	{
		m_pRouter->KickRes(uidSender, uidRequestPlayer, uidTargetPlayer,CR_FAIL_PARTY_KICK_SELF);
		return;
	}


	// ��� ��Ƽ�� Ȯ��
	if (!pParty->IsExistMember(uidTargetPlayer))
	{
		m_pRouter->KickRes(uidSender, uidRequestPlayer, uidTargetPlayer,CR_FAIL_PARTY_TARGET_NOT_IN_PARTY);
		return;
	}


	// ����� �������� �ƴ϶�� Party ���� �ʱ�ȭ
	ZPlayer* pTargetPlayer = gmgr.pPlayerManager->FindByUID(uidTargetPlayer);
	if (pTargetPlayer != NULL)
	{
		pTargetPlayer->SetPartyUID(MUID::Invalid());
		ZPlayerCommandRouter::RouteProxyPlayerUpdate(pTargetPlayer);
	}


	// ����
	pParty->RemoveMember(uidTargetPlayer);

	m_pRouter->KickRes(uidSender, uidRequestPlayer, uidTargetPlayer, CR_SUCCESS);
	m_pRouter->RemoveMember(uidParty, uidTargetPlayer);


	// ��Ƽ���� ȥ�� ������, �߰� Ż��
	if (1 == pParty->GetCount())
	{
		MUID uidAloneMember = pParty->GetAloneMember();		
		gmgr.pPartyManager->RemoveParty(uidParty);

		ZPlayer* pAloneMember = gmgr.pPlayerManager->FindByUID(uidAloneMember);
		if (pAloneMember != NULL)
		{
			pAloneMember->SetPartyUID(MUID::Invalid());
			ZPlayerCommandRouter::RouteProxyPlayerUpdate(pAloneMember);
		}

		m_pRouter->RemoveMember(uidParty, uidAloneMember);
		m_pRouter->PartyRemove(uidParty);
		return;
	}

	// ��Ƽ ����ȭ
	m_pRouter->EraseQuestRunnerSync(uidParty, uidTargetPlayer);
	RouteRemoveMemberSync(pParty);
}

void ZPartyLogic::JoinInviteReq(MUID uidSender, MUID uidParty, MUID uidRequestPlayer)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)
	{
		m_pRouter->JoinInviteRes(uidSender, uidRequestPlayer, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}


	// Ǯ��Ƽ Ȯ��
	if (pParty->IsFull())
	{
		m_pRouter->JoinInviteRes(uidSender, uidRequestPlayer, CR_FAIL_PARTY_FULL_PARTY);
		return;
	}


	// ��Ƽ�� Ȯ��
	if (pParty->IsExistMember(uidRequestPlayer))
	{
		m_pRouter->JoinInviteRes(uidSender, uidRequestPlayer, CR_FAIL_PARTY_ALEADY_HAS_PARTY);
		return;
	}

	
	// ��Ƽ�� Ȯ��
	MUID uidLeader = pParty->GetLeader();
	ZPlayer* pLeader = gmgr.pPlayerManager->FindByUID(uidLeader);
	if (pLeader == NULL)
	{
		m_pRouter->JoinInviteRes(uidSender, uidRequestPlayer, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}


	// ��Ƽ�� ���� Ȯ��
	int nServerID = pLeader->GetGameServerID();
	ZGameServerObject* pGameServer = gmgr.pGameServerObjectManager->FindGameServer(nServerID);
	if (pGameServer == NULL)
	{
		m_pRouter->JoinInviteRes(uidSender, uidRequestPlayer, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}


	// ��û�� Ȯ��
	ZPlayer* pRequestPlayer = gmgr.pPlayerManager->FindByUID(uidRequestPlayer);
	if (pRequestPlayer == NULL)
	{
		m_pRouter->JoinInviteRes(uidSender, uidRequestPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}
	

	// ���� ��û
	MUID uidTarget = pGameServer->GetUID();
	wstring strRequestPlayerName = pRequestPlayer->GetPlayerName();
	m_pRouter->JoinAcceptReq(uidTarget, uidParty, uidLeader, uidRequestPlayer, strRequestPlayerName);
}

void ZPartyLogic::JoinAcceptRes(MUID uidSender, MUID uidParty, MUID uidLeader, MUID uidRequestPlayer, CCommandResultTable nResult)
{
	// ��û�� Ȯ��
	ZPlayer* pRequestPlayer = gmgr.pPlayerManager->FindByUID(uidRequestPlayer);
	if (pRequestPlayer == NULL)
	{
		m_pRouter->JoinAcceptCancel(uidSender, uidLeader, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}

	
	// ��û�� ���� Ȯ��
	int nServerID = pRequestPlayer->GetGameServerID();	
	ZGameServerObject* pGameServer = gmgr.pGameServerObjectManager->FindGameServer(nServerID);
	if (pGameServer == NULL)
	{
		m_pRouter->JoinAcceptCancel(uidSender, uidLeader, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}

	
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)
	{
		MUID uidTarget = pGameServer->GetUID();
		m_pRouter->JoinInviteRes(uidTarget, uidRequestPlayer, CR_FAIL_PARTY_INVALID_PARTY);
		m_pRouter->JoinAcceptCancel(uidSender, uidLeader, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}

	
	// ��Ƽ�� Ȯ��
	if (uidLeader != pParty->GetLeader())
	{
		MUID uidTarget = pGameServer->GetUID();
		m_pRouter->JoinInviteRes(uidTarget, uidRequestPlayer, CR_FAIL_PARTY_NOT_PARTY_LEADER);
		m_pRouter->JoinAcceptCancel(uidSender, uidLeader, CR_FAIL_PARTY_NOT_PARTY_LEADER);
		return;
	}


	// Ǯ��Ƽ Ȯ��
	if (pParty->IsFull())
	{
		MUID uidTarget = pGameServer->GetUID();
		m_pRouter->JoinInviteRes(uidTarget, uidRequestPlayer, CR_FAIL_PARTY_FULL_PARTY);
		m_pRouter->JoinAcceptCancel(uidSender, uidLeader, CR_FAIL_PARTY_FULL_PARTY);
		return;
	}


	// ���� Ȯ��
	if (nResult != CR_SUCCESS)
	{
		MUID uidTarget = pGameServer->GetUID();
		m_pRouter->JoinInviteRes(uidTarget, uidRequestPlayer, nResult);
		return;
	}


	// ���� ó��
	int nMemberServerID = pRequestPlayer->GetGameServerID();
	pParty->AddMember(uidRequestPlayer, pRequestPlayer->GetPlayerName(), nMemberServerID, pRequestPlayer->GetCID());
	pRequestPlayer->SetPartyUID(uidParty);
	ZPlayerCommandRouter::RouteProxyPlayerUpdate(pRequestPlayer);


	// ���� ����
	MUID uidTarget = pGameServer->GetUID();
	m_pRouter->JoinInviteRes(uidTarget, uidRequestPlayer, nResult);


	// ��Ƽ ���� ����ȭ	
	m_pRouter->AddMember(uidParty, uidRequestPlayer, pRequestPlayer->GetPlayerName(), pRequestPlayer->GetCID());
}

void ZPartyLogic::CreateSingleReq(MUID uidSender, MUID uidRequestPlayer)
{
	// ��û�� Ȯ��
	ZPlayer* pRequestPlayer = gmgr.pPlayerManager->FindByUID(uidRequestPlayer);
	if (pRequestPlayer == NULL)		return;


	// ��Ƽ Ȯ��
	MUID uidParty = pRequestPlayer->GetPartyUID();
	if (uidParty.IsValid())		return;


	// ��Ƽ ����
	int nLeaderServerID = pRequestPlayer->GetGameServerID();
	ZParty* pParty = gmgr.pPartyManager->AddParty(uidRequestPlayer, pRequestPlayer->GetPlayerName(), nLeaderServerID, pRequestPlayer->GetCID());
	_ASSERT(pParty != NULL);

	uidParty = pParty->GetUID();
	pRequestPlayer->SetPartyUID(uidParty);
	ZPlayerCommandRouter::RouteProxyPlayerUpdate(pRequestPlayer);


	// ��Ƽ ���� ����ȭ	
	m_pRouter->PartyAdd(uidParty, uidRequestPlayer, pRequestPlayer->GetPlayerName(), pRequestPlayer->GetCID());	
}

void ZPartyLogic::PartyInfoAllReq(MUID uidTargetServer)
{
	// ��ü ��Ƽ���� ����
	vector<TD_PARTY_INFO> vecPartyInfo;

	const ZPartyManager::ZPartyMap& mapPartys = gmgr.pPartyManager->GetPartyMap();
	for each(ZPartyManager::ZPartyMap::value_type party in mapPartys)
	{
		ZParty* pParty = party.second;
		TD_PARTY_INFO tdPartyInfo;
		
		tdPartyInfo.uidParty = pParty->GetUID();		
		tdPartyInfo.nLeaderIndex = 0;

		int nIndex = 0;
		ZParty::member_iterator it = pParty->BeginMember();
		while(it != pParty->EndMember())
		{
			MUID uidMember = it->first;
			ZPartyMember* pPartyMember = it->second;

			TD_PARTY_INFO::MEMBER_INFO& rMemberInfo = tdPartyInfo.members[nIndex];
			rMemberInfo.uidMember = uidMember;
			rMemberInfo.nMemberCID = pPartyMember->GetCID();

			wcsncpy_s(rMemberInfo.szMemberName, pPartyMember->GetName().c_str(), _TRUNCATE);			
			
			if (uidMember == pParty->GetLeader())
			{
				tdPartyInfo.nLeaderIndex = nIndex;
			}

			nIndex++;
			it++;			
		}

		tdPartyInfo.nMemberCount = nIndex;

		vecPartyInfo.push_back(tdPartyInfo);
	}


	// ��Ƽ���� ����
	m_pRouter->PartyInfoAllRes(uidTargetServer, vecPartyInfo);
}

void ZPartyLogic::AllMemberOfflineInGameServer(int nGameServerID)
{
	const ZPartyManager::ZPartyMap& mapPartys = gmgr.pPartyManager->GetPartyMap();

	ZPartyManager::ZPartyMap::const_iterator it = mapPartys.begin();
	while(it != mapPartys.end())
	{
		// RemoveParty���� ���� ���ͷ����Ͱ� ���ŵ� �� �ֱ⶧����, ���� �޾Ƴ��� �̸� ������Ų��.
		MUID uidParty = it->first;
		ZParty* pParty = it->second;
		it++;

		
		// ��� �¶��� ����� �ش� ������ �ִٸ�, ��Ƽ ����	
		if (pParty->IsExistAllOnlineMembersIn(nGameServerID))
		{			
			gmgr.pPartyManager->RemoveParty(uidParty);
			m_pRouter->PartyRemove(uidParty);
		}
		else
		{
			bool beOffline = false;

			// ��Ƽ���� �ش� ������ �ִٸ�, �ٸ� ������ �ִ� ��Ƽ������ ��Ƽ�� ����
			MUID uidLeader = pParty->GetLeader();
			ZPartyMember* pLeaderMember = pParty->GetMember(uidLeader);
			_ASSERT(pLeaderMember != NULL);
						
			if (nGameServerID == pLeaderMember->GetGameServerID())
			{
				MUID uidNewLeader = pParty->FindCandidateForLeaderOtherServer();
				pParty->ChangeLeader(uidNewLeader);
				m_pRouter->ChangeLeaderRes(uidParty, uidNewLeader);
			}


			// �ش� ������ �ִ� ��� ��Ƽ�� ��������
			ZParty::member_iterator it = pParty->BeginMember();
			while(it != pParty->EndMember())
			{
				MUID uidMember = it->first;
				ZPartyMember* pPartyMember = it->second;

				if (nGameServerID == pPartyMember->GetGameServerID())
				{
					pPartyMember->SetMoveServer(false);
					pPartyMember->SetOffline(true);
					pPartyMember->ClearFieldGroup();
					pPartyMember->ClearQuest();

					m_pRouter->AddOfflineMember(uidParty, uidMember);
					m_pRouter->EraseQuestRunnerSync(uidParty, uidMember);
					
					beOffline = true;
				}

				it++;
			}


			// ��Ƽ���� ����ȭ
			if (beOffline)
			{
				RouteRemoveMemberSync(pParty);
			}
		}
	}
}

void ZPartyLogic::UpdateParty(MUID uidParty, const ZPartyMember* pMember)
{
	_ASSERT(pMember != NULL);

	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)						return;


	// ��Ƽ�� Ȯ��
	MUID uidMember = pMember->GetUID();
	ZPartyMember* pPartyMember = pParty->GetMember(uidMember);
	if (pPartyMember == NULL)				return;


	// ��Ƽ�� ���� ����
	pPartyMember->Update(pMember);


	// ��Ƽ ���� ����ȭ - ù ��ũ�� ��� ��� ������ ������Ʈ�Ǳ� ���� ������ �ʴ´�.
	if (pParty->IsSyncReady())
	{
		m_pRouter->PartySync(pParty);
	}
}

void ZPartyLogic::UpdateMember(MUID uidParty, const ZPartyMember* pMember)
{
	_ASSERT(pMember != NULL);

	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)						return;


	// ��Ƽ�� Ȯ��
	MUID uidMember = pMember->GetUID();
	ZPartyMember* pPartyMember = pParty->GetMember(uidMember);
	if (pPartyMember == NULL)				return;


	// ��Ƽ�� ���� ����
	pPartyMember->Update(pMember);


	// ��Ƽ�� ���� ����ȭ
	m_pRouter->MemberSync(uidParty, pMember);
}

void ZPartyLogic::AddQuest(MUID uidParty, MUID uidMember, const vector<int>& vecQuestID)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)						return;

	
	// ��Ƽ�� Ȯ��
	ZPartyMember* pPartyMember = pParty->GetMember(uidMember);
	if (pPartyMember == NULL)				return;


	// ����Ʈ �߰�
	for each(int nQuestID in vecQuestID)
	{
		pPartyMember->AddQuest(nQuestID);
	}	


	// ����Ʈ ����ȭ
	vector<TD_PARTY_QUEST> vecPartyQuest;
	MakePartyQuest(pParty, vecPartyQuest);
	m_pRouter->QuestSync(uidParty, vecPartyQuest);
}

void ZPartyLogic::AddField(MUID uidParty, MUID uidMember, MUID uidFieldGroup)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)		return;


	// ��Ƽ�� Ȯ��
	ZPartyMember* pPartyMember = pParty->GetMember(uidMember);
	if (pPartyMember == NULL)	return;


	// �ʵ� Ȯ��
	ZDynamicFieldMaster* pFieldGroup = gmgr.pFieldManager->FindDynamicFieldMaster(uidFieldGroup);
	if (pFieldGroup == NULL)	return;


	// �ʵ� �߰�
	int nFieldGroupID = pFieldGroup->GetInfo()->nID;
	pPartyMember->SetFieldGroup(nFieldGroupID, uidFieldGroup);


	// �ʵ� ����ȭ
	vector<TD_PARTY_FIELD> vecPartyField;
	MakePartyField(pParty, vecPartyField);	
	m_pRouter->FieldSync(uidParty, vecPartyField);
}

void ZPartyLogic::UpdateQuestRunner(MUID uidParty, MUID uidMember, MUID uidField, const vector<int>& vecQuestID)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)						return;


	// ���Ȯ��
	if (!pParty->IsExistMember(uidMember))		return;


	// ����Ʈ ���� ����ȭ
	m_pRouter->QuestRunnerSync(uidParty, uidMember, uidField, vecQuestID);
}

void ZPartyLogic::RemoveQuest(MUID uidParty, MUID uidMember, const vector<int>& vecQuestID)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)						return;


	// ��Ƽ�� Ȯ��
	ZPartyMember* pPartyMember = pParty->GetMember(uidMember);
	if (pPartyMember == NULL)				return;
	

	// ����Ʈ ����
	for each(int nQuestID in vecQuestID)
	{
		pPartyMember->RemoveQuest(nQuestID);
	}


	// ����Ʈ ����ȭ
	vector<TD_PARTY_QUEST> vecPartyQuest;
	MakePartyQuest(pParty, vecPartyQuest);
	m_pRouter->QuestSync(uidParty, vecPartyQuest);
}

void ZPartyLogic::RemoveField(MUID uidParty, MUID uidMember, int nFieldGroupID)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)				return;

	
	// ��Ƽ�� Ȯ��
	ZPartyMember* pPartyMember = pParty->GetMember(uidMember);
	if (pPartyMember == NULL)		return;
	
	
	// �ʵ� ����
	pPartyMember->ClearFieldGroup();


	// �ʵ� ����ȭ
	vector<TD_PARTY_FIELD> vecPartyField;
	MakePartyField(pParty, vecPartyField);	
	m_pRouter->FieldSync(uidParty, vecPartyField);
}

void ZPartyLogic::RemoveFieldSelf(MUID uidParty, int nFieldGroupID)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)		return;

	
	// �ʵ� ����
	pParty->RemoveFieldSelf(nFieldGroupID);


	// �ʵ� ����ȭ
	vector<TD_PARTY_FIELD> vecPartyField;
	MakePartyField(pParty, vecPartyField);	
	m_pRouter->FieldSync(uidParty, vecPartyField);
}

void ZPartyLogic::EraseQuestRunner(MUID uidParty, MUID uidMember)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)						return;


	// ��Ƽ�� Ȯ��
	if (!pParty->IsExistMember(uidMember))		return;


	// ����Ʈ ���� ����ȭ
	m_pRouter->EraseQuestRunnerSync(uidParty, uidMember);
}

void ZPartyLogic::DoOffline(MUID uidParty, MUID uidMember)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)				return;


	// ��Ƽ�� Ȯ��
	ZPartyMember* pPartyMember = pParty->GetMember(uidMember);
	if (pPartyMember == NULL)		return;


	// ��Ƽ���� ��� ��Ƽ�� ����
	if (uidMember == pParty->GetLeader())
	{
		MUID uidNewLeader = pParty->FindCandidateForLeader();
		if (uidNewLeader.IsValid())
		{
			pParty->ChangeLeader(uidNewLeader);
			m_pRouter->ChangeLeaderRes(uidParty, uidNewLeader);
		}
	}

	// �������� ��Ƽ�� �߰�
	pPartyMember->SetOffline(true);
	pPartyMember->ClearFieldGroup();
	pPartyMember->ClearQuest();

	m_pRouter->AddOfflineMember(uidParty, uidMember);
	

	// �¶��� ��Ƽ���� ������ ��Ƽ ����
	if (pParty->IsEmpty())
	{
		// ������Ƽ�� ���� �������εǾ �������� �ʴ´�.
		gmgr.pPartyManager->RemoveParty(uidParty);
		m_pRouter->PartyRemove(uidParty);
		return;
	}


	// ��Ƽ ����ȭ
	m_pRouter->EraseQuestRunnerSync(uidParty, uidMember);
	RouteRemoveMemberSync(pParty);


	// ������Ƽ ����Ÿ ��� �׳� ��Ƽ���� ����	
	if (ZConfig::IsExpoMode())
	{
		wstring strName = pPartyMember->GetName();
		int nFixedPartyID = m_pFixedPartyLogic->GetFixedPartyID(strName);

		if (nFixedPartyID != INVALID_ID)
		{
			// ��� ����
			pParty->RemoveMember(uidMember);
			m_pRouter->RemoveMember(uidParty, uidMember);

			// ��Ƽ ����ȭ
			m_pRouter->EraseQuestRunnerSync(uidParty, uidMember);
			RouteRemoveMemberSync(pParty);
		}
	}
}

bool ZPartyLogic::DoOnline(MUID uidParty, MUID uidMember, MUID uidOffline)
{
	// ��Ƽ ã��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)				return false;


	// ��Ƽ Ȯ��
	ZPartyMember* pPartyMember = pParty->GetMember(uidOffline);
	if (pPartyMember == NULL)		return false;


	// ��Ƽ�� Ȯ��
	ZPlayer* pPlayer = gmgr.pPlayerManager->FindByUID(uidMember);
	if (pPlayer == NULL)			return false;


	// �����̵� ���� Ȯ��
	if (pPartyMember->IsMoveServer())
	{
		// ������ü
		int nGameServerID = pPlayer->GetGameServerID();
		pPartyMember->SetGameServerID(nGameServerID);

		pPartyMember->SetMoveServer(false);
		pParty->ChangeMemberUID(uidOffline, uidMember);

		pPlayer->SetPartyUID(uidParty);
		ZPlayerCommandRouter::RouteProxyPlayerUpdate(pPlayer);


		// �����̵� �Ϸ� ���� ����ȭ
		m_pRouter->MoveServerSync(uidParty, uidMember, uidOffline);
	}
	else
	{
		// �������� ��Ƽ������ �¶��� ������ ��ü
		pPartyMember->SetOffline(false);

		int nGameServerID = pPlayer->GetGameServerID();
		pPartyMember->SetGameServerID(nGameServerID);

		pParty->ChangeMemberUID(uidOffline, uidMember);

		pPlayer->SetPartyUID(uidParty);
		ZPlayerCommandRouter::RouteProxyPlayerUpdate(pPlayer);
		

		// ��Ƽ�� �¶��� ����ȭ
		m_pRouter->RemoveOfflineMember(uidParty, uidMember, uidOffline);
	}

	return true;
}

void ZPartyLogic::MoveServer(MUID uidParty, MUID uidMember)
{
	// ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)				return;


	// ��Ƽ�� Ȯ��
	ZPartyMember* pPartyMember = pParty->GetMember(uidMember);
	if (pPartyMember == NULL)		return;


	// �����̵� �߰�
	pPartyMember->SetMoveServer(true);
}

void ZPartyLogic::MakePartyField(const ZParty* pParty, vector<TD_PARTY_FIELD>& vecPartyField)
{
	_ASSERT(pParty != NULL);

	ZParty::member_iterator it = pParty->BeginMember();
	while(it != pParty->EndMember())
	{
		ZPartyMember* pPartyMember = it->second;

		if (!pPartyMember->IsEmptyFieldGroup())
		{
			TD_PARTY_FIELD tdPartyField;
			tdPartyField.nFieldGroupID = pPartyMember->GetFieldGroupID();
			tdPartyField.uidFieldGroup = pPartyMember->GetFieldGroupUID();

			vecPartyField.push_back(tdPartyField);
		}

		it++;
	}
}

void ZPartyLogic::MakePartyQuest(const ZParty* pParty, vector<TD_PARTY_QUEST>& vecPartyQuest)
{
	_ASSERT(pParty != NULL);
	
	map<int, int> mapQuestID;	// nQuestID, nQuestCount;
	
	// ��� ����Ʈ ����
	ZParty::member_iterator it = pParty->BeginMember();
	while(it != pParty->EndMember())
	{
		ZPartyMember* pPartyMember = it->second;
		MemberQuestCounting(pPartyMember, mapQuestID);

		it++;
	}

	// ���� ��ü�� ��ŷ
	for each(map<int, int>::value_type quest in mapQuestID)
	{
		TD_PARTY_QUEST tdPartyQuest;
		tdPartyQuest.nQuestID = quest.first;
		tdPartyQuest.nQuestCount = quest.second;
		
		vecPartyQuest.push_back(tdPartyQuest);
	}
}

void ZPartyLogic::MemberQuestCounting(const ZPartyMember* pPartyMember, map<int, int>& mapQuestID)
{
	_ASSERT(pPartyMember != NULL);

	ZPartyMember::quest_iterator it = pPartyMember->BeginQuest();
	while(it != pPartyMember->EndQuest())
	{
		int nQuestID = *it;
		mapQuestID[nQuestID]++;

		it++;
	}
}

void ZPartyLogic::RouteRemoveMemberSync(const ZParty* pParty)
{
	_ASSERT(pParty != NULL);
	
	MUID uidParty = pParty->GetUID();

	vector<TD_PARTY_FIELD> vecPartyField;
	MakePartyField(pParty, vecPartyField);	
	m_pRouter->FieldSync(uidParty, vecPartyField);

	vector<TD_PARTY_QUEST> vecPartyQuest;
	MakePartyQuest(pParty, vecPartyQuest);
	m_pRouter->QuestSync(uidParty, vecPartyQuest);

	m_pRouter->PartySync(pParty);
}

void ZPartyLogic::ChangeNameReq( MUID uidSender, MUID uidParty, MUID uidLeader, wstring strName )
{
	// ��Ƽ ã��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}

	// ��Ƽ�� ���� Ȯ��
	if (uidLeader != pParty->GetLeader())
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_PARTY_NOT_PARTY_LEADER);
		return;
	}
	
	// �̸� ����
	pParty->SetName(strName);

	// �뺸
	m_pRouter->ChangeNameRes(uidParty, strName);
}

void ZPartyLogic::ChangeLeaderReq( MUID uidSender, MUID uidParty, MUID uidLeader, MUID uidNewLeader )
{
	// ��Ƽ ã��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}

	// ��Ƽ�� ���� Ȯ��
	if (uidLeader != pParty->GetLeader())
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_PARTY_NOT_PARTY_LEADER);
		return;
	}

	// ���� ��Ƽ�� ��� ã��
	ZPartyMember* pNewLeader = pParty->GetMember(uidNewLeader);
	if (pNewLeader == NULL || pNewLeader->IsOffline())
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}

	// ��Ƽ�� ����
	pParty->ChangeLeader(uidNewLeader);

	// �뺸
	m_pRouter->ChangeLeaderRes(uidParty, uidNewLeader);
}

void ZPartyLogic::ChangeLootingRuleReq( MUID uidSender, MUID uidParty, MUID uidLeader, LOOTING_RULE_DATA rule )
{
	// ��Ƽ ã��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}

	// ��Ƽ�� ���� Ȯ��
	if (uidLeader != pParty->GetLeader())
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_PARTY_NOT_PARTY_LEADER);
		return;
	}

	// ���÷� ����
	pParty->SetLootingRule(rule);

	// ���÷� ���� �뺸
	m_pRouter->ChangeLootingRuleRes(uidParty, rule);
}

void ZPartyLogic::ChangeQuestIDReq( MUID uidSender, MUID uidParty, MUID uidLeader, int nQuestID )
{
	// ��Ƽ ã��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);
	if (pParty == NULL)
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}

	// ��Ƽ�� ���� Ȯ��
	if (uidLeader != pParty->GetLeader())
	{
		m_pRouter->Fail(uidSender, uidLeader, CR_FAIL_PARTY_NOT_PARTY_LEADER);
		return;
	}

	// ���÷� ����
	pParty->SetAutoPartyQuestID(nQuestID);

	// ���÷� ���� �뺸
	m_pRouter->ChangeQuestIDRes(uidParty, nQuestID);
}

bool ZPartyLogic::FixedPartyLogOn(MUID uidParty, MUID uidMember, MUID uidOffline)
{
	// �α׿� ���� Ȯ��
	ZPlayer* pPlayer = gmgr.pPlayerManager->FindByUID(uidMember);

	if (pPlayer == NULL)
	{
		return false;
	}


	// ���� ��Ƽ ����� ���, ���� ��Ƽ���� �ڽ��� ����� �����ִ��� Ȯ���Ѵ�.
	wstring strName = pPlayer->GetPlayerName();
	int nFixedPartyID = m_pFixedPartyLogic->GetFixedPartyID(strName);

	if (nFixedPartyID != INVALID_ID)
	{
		// ���� ��Ƽ�� ��ϵ� UID�� ã�Ƴ���. - ����Ÿ�� ���, ������Ƽ ������ ������� �ʾ� ã�ƾ��Ѵ�.
		ZParty* pParty = m_pFixedPartyLogic->FindParty(nFixedPartyID);

		if (pParty != NULL)
		{
			ZPartyMember* pPartyMember = pParty->FindMember(strName);

			if (pPartyMember != NULL)
			{
				uidParty = pParty->GetUID();
				uidOffline = pPartyMember->GetUID();
			}
		}
	}


	// Onlineó�� �����ϸ� Onlineó���Ѵ�.
	if (DoOnline(uidParty, uidMember, uidOffline))
	{
		return true;
	}


	// ��ϵ� ������Ƽ ������� Ȯ���Ѵ�.
	nFixedPartyID = m_pFixedPartyLogic->GetFixedPartyID(strName);

	if (nFixedPartyID == INVALID_ID)
	{
		return false;
	}


	// ��Ƽ�� ã�´�.
	ZParty* pParty = m_pFixedPartyLogic->FindParty(nFixedPartyID);

	if (pParty == NULL)
	{
		m_pFixedPartyLogic->AddWaiting(nFixedPartyID, uidMember);
		m_pFixedPartyLogic->CreateFixedParty(nFixedPartyID);
	}
	else
	{
		MUID uidParty = pParty->GetUID();

		// �ڸ��� ������, �������� ����� �Ѹ� �ڸ���.
		if (MAX_PARTY_MEMBER_COUNT <= pParty->GetCount())
		{
			// ��Ƽ Ǯ�̸� ���̻� ���� �� ����.
			if (0 == pParty->GetOfflineMemberCount())
			{
				return false;
			}

			// �������� ����� �Ѹ� �ڸ���.
			MUID uidOfflineMember = pParty->GetFirstOfflineMember();

			// ��� ����
			pParty->RemoveMember(uidOfflineMember);
			m_pRouter->RemoveMember(uidParty, uidOfflineMember);

			// ��Ƽ ����ȭ
			m_pRouter->EraseQuestRunnerSync(uidParty, uidOfflineMember);
			RouteRemoveMemberSync(pParty);
		}


		// ��� �߰�
		int nGameServerID = pPlayer->GetGameServerID();
		CID nCID = pPlayer->GetCID();		

		pParty->AddMember(uidMember, strName, nGameServerID, nCID);
		pPlayer->SetPartyUID(uidParty);


		// ��� �߰� ����ȭ
		ZPlayerCommandRouter::RouteProxyPlayerUpdate(pPlayer);
		m_pRouter->AddMember(uidParty, uidMember, strName, nCID);
	}

	return true;
}

void ZPartyLogic::CreateAutoPartyReq(int nQuestID, const vector<MUID>& vecMemberUID)
{	
	// ��ȿ�� �˻�
	vector<MUID> vecValidMemberUID;

	for each(const MUID& uidMember in vecMemberUID)
	{
		// ���� �÷��̾� �˻�
		ZPlayer* pPlayer = gmgr.pPlayerManager->FindByUID(uidMember);
		
		if (pPlayer == NULL)
		{
			continue;
		}

		// ��Ƽ�� ���� �÷��̾� �˻�
		MUID uidCheckParty = pPlayer->GetPartyUID();

		if (uidCheckParty.IsValid())
		{
			if (NULL != gmgr.pPartyManager->Find(uidCheckParty))
			{
				continue;
			}
		}

		// �߰�
		vecValidMemberUID.push_back(uidMember);
	}

	// ��ȿ�� �߱����� 2���� ���Ǹ�, ����
	if (vecValidMemberUID.size() < 2)
	{
		return;
	}

	
	// ��Ƽ ����
	ZParty* pParty = NULL;
	MUID uidParty;
	
	for each(const MUID& uidMember in vecValidMemberUID)
	{
		ZPlayer* pPlayer = gmgr.pPlayerManager->FindByUID(uidMember);
				
		if (pPlayer == NULL)
		{
			return;
		}

		int nGameServerID = pPlayer->GetGameServerID();

		if (pParty == NULL)
		{
			pParty = gmgr.pPartyManager->AddParty(uidMember, pPlayer->GetPlayerName(), nGameServerID, pPlayer->GetCID());
			pParty->SetAutoPartyQuestID(nQuestID);
			uidParty = pParty->GetUID();
		}
		else
		{
			pParty->AddMember(uidMember, pPlayer->GetPlayerName(), nGameServerID, pPlayer->GetCID());
		}
				
		pPlayer->SetPartyUID(uidParty);

		ZPlayerCommandRouter::RouteProxyPlayerUpdate(pPlayer);
	}


	// ��Ƽ ���� ����ȭ	
	for each(const MUID& uidMember in vecValidMemberUID)
	{		
		ZPartyMember* pMember = pParty->GetMember(uidMember);
		
		if (pMember->GetUID() == pParty->GetLeader())
		{			
			m_pRouter->PartyAdd(uidParty, pMember->GetUID(), pMember->GetName(), pMember->GetCID());		
		}
		else
		{
			m_pRouter->AddMember(uidParty, pMember->GetUID(), pMember->GetName(), pMember->GetCID());
		}
	}
}

void ZPartyLogic::JoinAotuPartyReq(MUID uidParty, MUID uidPlayer)
{
	// ���� ����� Ȯ��
	ZPlayer* pPlayer = gmgr.pPlayerManager->FindByUID(uidPlayer);

	if (pPlayer == NULL)
	{
		return;
	}


	// ���� ���Ե� ��Ƽ�� ������Ѵ�.
	MUID uidJoinedParty = pPlayer->GetPartyUID();

	if (gmgr.pPartyManager->IsPartyMember(uidJoinedParty, uidPlayer))
	{
		return;
	}


	// ������ ��Ƽ Ȯ��
	ZParty* pParty = gmgr.pPartyManager->Find(uidParty);

	if (pParty == NULL)
	{
		return;
	}


	// �̹� Ǯ��Ƽ�� ������ �� ����.
	if (pParty->IsFull())
	{
		return;
	}
	

	// �ڵ���Ƽ ����
	wstring strName = pPlayer->GetPlayerName();
	CID nCID = pPlayer->GetCID(); 
	int nMemberServerID = pPlayer->GetGameServerID();

	pParty->AddMember(uidPlayer, strName, nMemberServerID, nCID);
	pPlayer->SetPartyUID(uidParty);
	
	ZPlayerCommandRouter::RouteProxyPlayerUpdate(pPlayer);


	// ��Ƽ ���� ����ȭ		
	m_pRouter->AddMember(uidParty, uidPlayer, strName, nCID);	
}
