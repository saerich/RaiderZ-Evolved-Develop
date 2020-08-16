#include "stdafx.h"
#include "GStandAloneModePartyRouter.h"
#include "GEntityPlayer.h"
#include "GGlobal.h"
#include "GPlayerObjectManager.h"
#include "GPartySystem.h"
#include "CCommandTable_Master.h"
#include "GCommand.h"
#include "GMasterServerFacade.h"
#include "GProxyParty.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "GDynamicFieldMaster.h"
#include "GConst.h"
#include "CSDef_Loot.h"
#include "GCommandCenter.h"
#include "CCommandResultTable.h"
#include "GLootSystem.h"


GStandAloneModePartyRouter::GStandAloneModePartyRouter()
{
	// no nothing
}

GStandAloneModePartyRouter::~GStandAloneModePartyRouter()
{
	// no nothing
}

void GStandAloneModePartyRouter::SyncParty(const GParty* pParty)
{
	_ASSERT(pParty != NULL);


	// ���� ����
	TD_PARTY_SETTING tdPartySetting;
	tdPartySetting.Import(pParty->GetPartySetting());

	TD_PARTY tdParty(pParty->GetUID(), tdPartySetting);

	vector<TD_PARTY_MEMBER> vecPartyMember;
	vector<vector<int> > vecBuffs(MAX_PARTY_MEMBER_COUNT);

	int i = 0;
	for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
	{
		if (MAX_PARTY_MEMBER_COUNT <= i)	break;

		MUID uidMember = it->first;

		TD_PARTY_MEMBER tdMember;
		MakePartyMemberInfo(pParty, uidMember, &tdMember, &vecBuffs[i]);
		vecPartyMember.push_back(tdMember);

		i++;
	}


	_ASSERT(MAX_PARTY_MEMBER_COUNT == 5);

	// ���ڿ� Ŀ�ǵ� ����
	auto_ptr<MCommand> cmd(MakeNewCommand(MMC_PARTY_SYNC,
											7,
											NEW_BLOB(&tdParty, sizeof(tdParty), 1),
											NEW_BLOB(vecPartyMember),
											NEW_BLOB(vecBuffs[0]),
											NEW_BLOB(vecBuffs[1]),
											NEW_BLOB(vecBuffs[2]),
											NEW_BLOB(vecBuffs[3]),
											NEW_BLOB(vecBuffs[4]))
											);


	// ����ȭ
	MUID uidParty = pParty->GetUID();
	gsys.pPartySystem->SyncParty(tdParty, cmd.get());
}

void GStandAloneModePartyRouter::InviteReq(MUID uidTargetPlayer, MUID uidRequestPlayer)
{
	GEntityPlayer* pRequestPlayer = gmgr.pPlayerObjectManager->GetEntity(uidRequestPlayer);

	if (pRequestPlayer != NULL)
	{
		wstring strRequestPlayerName = pRequestPlayer->GetName();
		gsys.pPartySystem->AcceptReq(uidTargetPlayer, uidRequestPlayer, strRequestPlayerName);
	}
	else
	{
		gsys.pPartySystem->InviteRes(uidRequestPlayer, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
	}
}

void GStandAloneModePartyRouter::AcceptRes(MUID uidRequestPlayer, MUID uidTargetPlayer, CCommandResultTable nResult)
{
	GEntityPlayer* pRequestPlayer = gmgr.pPlayerObjectManager->GetEntity(uidRequestPlayer);
	if (pRequestPlayer == NULL)
	{
		gsys.pPartySystem->AcceptCancel(uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}

	bool isExistParty = false;
	MUID uidRequestParty = pRequestPlayer->GetPartyUID();
	if (uidRequestParty.IsValid())
	{
		GParty* pParty = gsys.pPartySystem->FindParty(uidRequestParty);
		if (pParty != NULL)
		{
			isExistParty = true;

			if (uidRequestPlayer != pParty->GetLeader())
			{
				gsys.pPartySystem->InviteRes(uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_NOT_PARTY_LEADER);
				gsys.pPartySystem->AcceptCancel(uidTargetPlayer, CR_FAIL_PARTY_NOT_PARTY_LEADER);
				return;
			}

			if (pParty->IsFull())
			{
				gsys.pPartySystem->InviteRes(uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_FULL_PARTY);				
				gsys.pPartySystem->AcceptCancel(uidTargetPlayer, CR_FAIL_PARTY_FULL_PARTY);
				return;
			}
		}
	}

	GEntityPlayer* pTargetPlayer = gmgr.pPlayerObjectManager->GetEntity(uidTargetPlayer);
	if (pTargetPlayer == NULL)
	{
		gsys.pPartySystem->InviteRes(uidRequestPlayer, uidTargetPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}


	if (pTargetPlayer->HasParty())
	{
		gsys.pPartySystem->InviteRes(uidRequestPlayer, uidTargetPlayer, CR_FAIL_PARTY_TARGET_ALEADY_HAS_PARTY);
		gsys.pPartySystem->AcceptCancel(uidTargetPlayer, CR_FAIL_PARTY_ALEADY_HAS_PARTY);		
		return;
	}


	gsys.pPartySystem->InviteRes(uidRequestPlayer, uidTargetPlayer, nResult);

	if (nResult == CR_SUCCESS)
	{
		MUID uidParty;

		if (isExistParty)
		{
			uidParty = uidRequestParty;
		}
		else
		{
			uidParty = gsys.pServer->NewUID();
			gsys.pMasterServerFacade->AddProxyParty(uidParty, uidRequestPlayer, pRequestPlayer->GetName(), pRequestPlayer->GetCID()); 
			gsys.pPartySystem->AddParty(uidParty, uidRequestPlayer);
		}

		gsys.pMasterServerFacade->AddProxyPartyMember(uidParty, uidTargetPlayer, pTargetPlayer->GetName(), pTargetPlayer->GetCID());
		gsys.pPartySystem->AddMember(uidParty, uidTargetPlayer, pTargetPlayer->GetName(), pTargetPlayer->GetCID());
	}
}

void GStandAloneModePartyRouter::UpdateParty(MUID uidParty, const TD_PARTY_MEMBER& tdPartyMember, const vector<int>& vecBuff)
{
	// ��Ƽ Ȯ��
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)		return;


	// ��Ƽ ����ȭ
	SyncParty(pParty);
}

void GStandAloneModePartyRouter::UpdateMember(MUID uidParty, const TD_PARTY_MEMBER& tdPartyMember, const vector<int>& vecBuff)
{
	auto_ptr<MCommand> cmd(MakeNewCommand(MMC_PARTY_MEMBER_SYNC,
											3,
											NEW_UID(uidParty),
											NEW_BLOB(&tdPartyMember, sizeof(tdPartyMember), 1),
											NEW_BLOB(vecBuff)
											));

	gsys.pPartySystem->SyncMember(uidParty, cmd.get());
}

void GStandAloneModePartyRouter::AddQuest(MUID uidParty, MUID uidMember, const vector<int>& vecQuestID)
{
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)		return;

	
	for each(int nQuestID in vecQuestID)
	{
		pParty->AddQuest(nQuestID);
	}
}

void GStandAloneModePartyRouter::AddField(MUID uidParty, MUID uidMember, MUID uidFieldGroup)
{
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)				return;

	GDynamicFieldMaster* pFieldMaster = gmgr.pFieldMgr->GetDynamicFieldMaster(uidFieldGroup);
	if (pFieldMaster == NULL)		return;

	int nFieldGroupID = pFieldMaster->GetGroupID();
	pParty->CreateField(nFieldGroupID, uidFieldGroup);
}

void GStandAloneModePartyRouter::UpdateQuestRunner(MUID uidParty, MUID uidMember, MUID uidField, const vector<int>& vecQuestID)
{
	gsys.pPartySystem->SyncQuestRunner(uidParty, uidMember, uidField, vecQuestID);
}

void GStandAloneModePartyRouter::RemoveQuest(MUID uidParty, MUID uidMember, const vector<int>& vecQuestID)
{
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)		return;

	
	for each(int nQuestID in vecQuestID)
	{
		pParty->RemoveQuest(nQuestID);
	}
}

void GStandAloneModePartyRouter::RemoveField(MUID uidParty, MUID uidMember, int nFieldGroupID)
{
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)		return;

	pParty->RemoveField(nFieldGroupID);
}

void GStandAloneModePartyRouter::RemoveFieldSelf(MUID uidParty, int nFieldGroupID)
{
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)		return;

	pParty->RemoveField(nFieldGroupID);
}

void GStandAloneModePartyRouter::EraseQuestRunner(MUID uidParty, MUID uidMember)
{
	gsys.pPartySystem->SyncEraseQuestRunner(uidParty, uidMember);
}

void GStandAloneModePartyRouter::LeaveReq(MUID uidParty, MUID uidLeavePlayer)
{
	// ��Ƽ Ȯ��
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)
	{
		gsys.pPartySystem->LeaveRes(uidLeavePlayer, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}

	// ��Ƽ�� Ȯ��
	if (!pParty->IsExistMember(uidLeavePlayer))
	{
		gsys.pPartySystem->LeaveRes(uidLeavePlayer, CR_FAIL_PARTY_NOT_IN_PARTY);
		return;
	}

	// ȥ�� �������� ���� ó��
	if (uidLeavePlayer == pParty->GetAloneMember())
	{
		gsys.pPartySystem->LeaveRes(uidLeavePlayer, CR_SUCCESS);
		gsys.pMasterServerFacade->RemoveProxyPartyMember(uidParty, uidLeavePlayer);		
		gsys.pPartySystem->RemoveMember(uidParty, uidLeavePlayer);
		gsys.pPartySystem->RemoveParty(uidParty);
		return;
	}


	// ��Ƽ���� ��� ��Ƽ�� ����
	if (uidLeavePlayer == pParty->GetLeader())
	{
		MUID uidNewLeader = pParty->FindCandidateForLeader();
		if (uidNewLeader.IsInvalid())
		{
			gsys.pPartySystem->LeaveRes(uidLeavePlayer, CR_FAIL_PARTY_INVALID_PARTY);
			return;
		}
		
		gsys.pMasterServerFacade->ChangeProxyPartyLeader(uidParty, uidNewLeader);
		gsys.pPartySystem->ChangePartyLeaderRes(uidParty, uidNewLeader);
	}

	// Ż��
	gsys.pPartySystem->LeaveRes(uidLeavePlayer, CR_SUCCESS);
	gsys.pMasterServerFacade->RemoveProxyPartyMember(uidParty, uidLeavePlayer);	
	gsys.pPartySystem->RemoveMember(uidParty, uidLeavePlayer);

	pParty = gsys.pPartySystem->FindParty(uidParty);

	if (pParty == NULL)
	{
		return;
	}


	// ��Ƽ���� ȥ�� ������, �߰� Ż��
	if (1 == pParty->GetMemberCount())
	{
		MUID uidAloneMember = pParty->GetAloneMember();

		gsys.pMasterServerFacade->RemoveProxyPartyMember(uidParty, uidAloneMember);
		gsys.pPartySystem->RemoveMember(uidParty, uidAloneMember);
		gsys.pPartySystem->RemoveParty(uidParty);
		return;
	}


	// ��Ƽ ����ȭ
	SyncParty(pParty);
}

void GStandAloneModePartyRouter::KickReq(MUID uidParty, MUID uidRequestPlayer, MUID uidTargetPlayer)
{
	// ��Ƽ Ȯ��
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)
	{
		gsys.pPartySystem->KickRes(uidRequestPlayer, uidRequestPlayer, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}


	// ��� Ȯ��
	GEntityPlayer* pTargetPlayer = gmgr.pPlayerObjectManager->GetEntity(uidTargetPlayer);
	if (pTargetPlayer == NULL)
	{
		gsys.pPartySystem->KickRes(uidRequestPlayer, uidRequestPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}


	// Ż��
	gsys.pPartySystem->KickRes(uidRequestPlayer, uidRequestPlayer, CR_SUCCESS);
	gsys.pMasterServerFacade->RemoveProxyPartyMember(uidParty, uidTargetPlayer);	
	gsys.pPartySystem->RemoveMember(uidParty, uidTargetPlayer);

	pParty = gsys.pPartySystem->FindParty(uidParty);

	if (pParty == NULL)
	{
		return;
	}


	// ��Ƽ���� ȥ�� ������, �߰� Ż��
	if (1 == pParty->GetOnlineMemberCount())
	{
		MUID uidAloneMember = pParty->GetAloneMember();		

		gsys.pMasterServerFacade->RemoveProxyPartyMember(uidParty, uidAloneMember);
		gsys.pPartySystem->RemoveMember(uidParty, uidAloneMember);
		gsys.pPartySystem->RemoveParty(uidParty);
		return;
	}


	// ��Ƽ ����ȭ
	SyncParty(pParty);
}

void GStandAloneModePartyRouter::DoOffline(MUID uidParty, MUID uidMember)
{
	// ��Ƽ Ȯ��
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)		return;

	GEntityPlayer* pMember = gmgr.pPlayerObjectManager->GetEntity(uidMember);
	if (pMember = NULL)		return;

	// ��Ƽ���� ��� ��Ƽ�� ����
	if (uidMember == pParty->GetLeader())
	{
		MUID uidNewLeader = pParty->FindCandidateForLeader();
		if (uidNewLeader.IsValid())
		{
			gsys.pMasterServerFacade->ChangeProxyPartyLeader(uidParty, uidNewLeader);
			gsys.pPartySystem->ChangePartyLeaderRes(uidParty, uidNewLeader);
		}
	}

	// �������� ��Ƽ�� �߰�	
	if (GConst::TEST_ENABLE_OFFLINE_PARTY_FOR_STAND_ALONE_MODE)
	{
		// �¶��� ����� ��� ��Ƽ�� �������� �ʴ´�.
		pParty->AddOfflineMember(uidMember);
	}
	else
	{
		// �¶��� ��� ������ ��Ƽ����
		gsys.pPartySystem->AddOfflineMember(uidParty, uidMember);
	}


	// ��Ƽ ����ȭ
	pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty != NULL)
	{
		SyncParty(pParty);
	}
	else
	{
		gsys.pMasterServerFacade->RemoveProxyParty(uidParty);
		gsys.pPartySystem->RemoveParty(uidParty);
	}
}

void GStandAloneModePartyRouter::DoOnline(MUID uidParty, MUID uidMember, MUID uidOffline)
{
	const GProxyParty* pProxyParty = gsys.pMasterServerFacade->FindProxyParty(uidParty);

	if (pProxyParty != NULL)
	{
		wstring strMemberName = pProxyParty->GetName(uidOffline);
		int nMemberCID = pProxyParty->GetCID(uidOffline);
		gsys.pMasterServerFacade->RemoveProxyPartyMember(uidParty, uidOffline);
		gsys.pMasterServerFacade->AddProxyPartyMember(uidParty, uidMember, strMemberName, nMemberCID);
	}

	gsys.pPartySystem->RemoveOfflineMember(uidParty, uidMember, uidOffline);
}

void GStandAloneModePartyRouter::JoinInviteReq(MUID uidParty, MUID uidRequestPlayer)
{
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)
	{
		gsys.pPartySystem->JoinInviteRes(uidRequestPlayer, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}

		
	GEntityPlayer* pRequestPlayer = gmgr.pPlayerObjectManager->GetEntity(uidRequestPlayer);
	if (pRequestPlayer == NULL)
	{
		gsys.pPartySystem->JoinInviteRes(uidRequestPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}


	MUID uidLeader = pParty->GetLeader();
	wstring strRequestPlayerName = pRequestPlayer->GetName();
	gsys.pPartySystem->JoinAcceptReq(uidParty, uidLeader, uidRequestPlayer, strRequestPlayerName);
}

void GStandAloneModePartyRouter::JoinAcceptRes(MUID uidParty, MUID uidLeader, MUID uidRequestPlayer, CCommandResultTable nResult)
{
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);
	if (pParty == NULL)
	{
		gsys.pPartySystem->JoinInviteRes(uidRequestPlayer, CR_FAIL_PARTY_INVALID_PARTY);
		gsys.pPartySystem->JoinAcceptCancel(uidLeader, CR_FAIL_PARTY_INVALID_PARTY);
		return;
	}

	GEntityPlayer* pRequestPlayer = gmgr.pPlayerObjectManager->GetEntity(uidRequestPlayer);
	if (pRequestPlayer == NULL)
	{
		gsys.pPartySystem->JoinInviteRes(uidRequestPlayer, CR_FAIL_SYSTEM_INVALID_PC_UID);
		gsys.pPartySystem->JoinAcceptCancel(uidLeader, CR_FAIL_SYSTEM_INVALID_PC_UID);
		return;
	}


	gsys.pPartySystem->JoinInviteRes(uidRequestPlayer, nResult);

	if (nResult == CR_SUCCESS)
	{
		gsys.pMasterServerFacade->AddProxyPartyMember(uidParty, uidRequestPlayer, pRequestPlayer->GetName(), pRequestPlayer->GetCID());
		gsys.pPartySystem->AddMember(uidParty, uidRequestPlayer, pRequestPlayer->GetName(), pRequestPlayer->GetCID());
	}
}

void GStandAloneModePartyRouter::MoveServer(MUID uidParty, MUID uidMember)
{
	// do nothing
}

void GStandAloneModePartyRouter::CreateSinglePartyReq(MUID uidRequestPlayer)
{
	GEntityPlayer* pRequestPlayer = gmgr.pPlayerObjectManager->GetEntity(uidRequestPlayer);
	if (pRequestPlayer == NULL)		return;

	MUID uidParty = gsys.pServer->NewUID();
	gsys.pMasterServerFacade->AddProxyParty(uidParty, uidRequestPlayer, pRequestPlayer->GetName(), pRequestPlayer->GetCID()); 
	gsys.pPartySystem->AddParty(uidParty, uidRequestPlayer);
}

void GStandAloneModePartyRouter::PartyInfoAllReq(void)
{
	// do nothing
}

void GStandAloneModePartyRouter::ChangePartyNameReq(MUID uidParty, MUID uidLeader, wstring strName)
{
	gsys.pPartySystem->ChangePartyNameRes(uidParty, strName);
}

void GStandAloneModePartyRouter::ChangePartyLeaderReq(MUID uidParty, MUID uidLeader, MUID uidNewLeader)
{
	gsys.pMasterServerFacade->ChangeProxyPartyLeader(uidParty, uidNewLeader);
	gsys.pPartySystem->ChangePartyLeaderRes(uidParty, uidNewLeader);
}

void GStandAloneModePartyRouter::ChangePartyLootingRuleReq(MUID uidParty, MUID uidLeader, LOOTING_RULE_DATA rule)
{
	gsys.pPartySystem->ChangePartyLootingRuleRes(uidParty, rule);
}

void GStandAloneModePartyRouter::ChangeQuestIDReq( MUID uidParty, MUID uidLeader, int nQuestID )
{
	gsys.pPartySystem->ChangeQuestIDRes(uidParty, nQuestID);
}

void GStandAloneModePartyRouter::FixedPartyLogOn(MUID uidParty, MUID uidMember, MUID uidOffline)
{
	// do nothing
}

void GStandAloneModePartyRouter::CreateAutoPartyReq(QuestID nQuestID, const vector<MUID> vecPromotersUID)
{
	// �ִ� ��Ƽ�� ����ŭ�� ��û�� �� �ִ�.
	if (MAX_PARTY_MEMBER_COUNT < vecPromotersUID.size())
	{
		return;
	}

	// ��Ƽ ���� ���� Ȯ��
	int nPossiblePlayerCount = 0;

	for each(const MUID& uidMember in vecPromotersUID)	
	{
		GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidMember);

		if (pPlayer == NULL)
		{
			continue;
		}

		if (pPlayer->HasParty())
		{
			continue;
		}

		nPossiblePlayerCount++;		
	}
		
	// �ּ� 2���� ��Ƽ�� �����ؾ��Ѵ�.
	if (nPossiblePlayerCount < 2)
	{
		return;
	}


	// ��Ƽ ����
	MUID uidParty = MUID::ZERO;

	for each(const MUID& uidMember in vecPromotersUID)	
	{		
		GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidMember);

		if (pPlayer == NULL)
		{
			continue;
		}

		wstring strName = pPlayer->GetName();
		CID nCID = pPlayer->GetCID();

		if (uidParty.IsInvalid())
		{
			uidParty = gsys.pServer->NewUID();

			gsys.pMasterServerFacade->AddProxyParty(uidParty, uidMember, strName, nCID);

			gsys.pPartySystem->AddParty(uidParty, uidMember, nQuestID);
		}
		else
		{
			gsys.pMasterServerFacade->AddProxyPartyMember(uidParty, uidMember, strName, nCID);	
			gsys.pPartySystem->AddMember(uidParty, uidMember, strName, nCID);
		}
	}
}

void GStandAloneModePartyRouter::JoinAutoPartyReq(MUID uidParty, MUID uidPlayer)
{
	// ��Ƽ Ȯ��
	GParty* pParty = gsys.pPartySystem->FindParty(uidParty);

	if (pParty == NULL)
	{
		return;
	}


	// �÷��̾� Ȯ��
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(uidPlayer);

	if (pPlayer == NULL)
	{
		return;
	}


	// �������Ե� ��Ƽ�� ������ �ȵȴ�.
	if (pPlayer->HasParty())
	{
		return;
	}


	// ��Ƽ ����
	wstring strName = pPlayer->GetName();
	CID nCID = pPlayer->GetCID();

	gsys.pMasterServerFacade->AddProxyPartyMember(uidParty, uidPlayer, strName, nCID);
	gsys.pPartySystem->AddMember(uidParty, uidPlayer, strName, nCID);
}
