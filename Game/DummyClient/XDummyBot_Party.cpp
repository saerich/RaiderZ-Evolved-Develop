#include "stdafx.h"
#include "XDummyBot_Party.h"
#include "XBirdDummyAgent.h"

XDummyBot_Party::XDummyBot_Party(XBirdDummyAgent* pAgent, XDummyAgentInfo* pAgentInfo)
: XDummyBot_Roam(pAgent, pAgentInfo)
{
}

XDummyBot_Party::~XDummyBot_Party()
{
}

MCommandResult XDummyBot_Party::OnCommand(XBirdDummyAgent* pAgent, MCommand* pCommand)
{
	MCommandResult nResult = XDummyBot_Roam::OnCommand(pAgent, pCommand);
	if (nResult == CR_TRUE)	return CR_TRUE;

	XBirdClient* pClient = pAgent->GetClient();	
	switch(pCommand->GetID())
	{
	case MC_FIELD_IN_PLAYER:
		{	
			TD_UPDATE_CACHE_PLAYER* pPlayer;
			if (!pCommand->GetSingleBlob(pPlayer, 0))	return CR_ERROR;

			ENTITY_PLAYER player;

			player.uid = pPlayer->uid;
			player.nUIID = pPlayer->nUIID;
			player.vPos = pPlayer->vPos;

			m_mapPlayers.insert(make_pair(player.nUIID, player));
		}
		break;
	case MC_FIELD_OUT_PLAYER:
		{
			UIID nUIID;
			if (!pCommand->GetParameter(&nUIID, 0, MPT_USHORT))	return CR_ERROR;
			
			m_mapPlayers.erase(nUIID);
		}
		break;
	case MC_FIELD_SECTOR_ENTITY_INFO:
		{
			vector<TD_UPDATE_CACHE_PLAYER> vecPlayer;
			if (!pCommand->GetBlob(vecPlayer, 0))	return CR_ERROR;

			for(vector<TD_UPDATE_CACHE_PLAYER>::iterator it = vecPlayer.begin(); it != vecPlayer.end(); it++)
			{
				ENTITY_PLAYER player;

				player.uid = it->uid;
				player.nUIID = it->nUIID;
				player.vPos = it->vPos;

				m_mapPlayers.insert(make_pair(player.nUIID, player));
			}
		}
		break;
	case MC_ACTION_MOVE:
		{
			UIID nUIID;
			TD_PC_MOVE* pMove;

			if (!pCommand->GetParameter(&nUIID, 0, MPT_USHORT))	return CR_ERROR;
			if (!pCommand->GetSingleBlob(pMove, 1))	return CR_ERROR;

			map<UIID, ENTITY_PLAYER>::iterator it = m_mapPlayers.find(nUIID);
			if (it != m_mapPlayers.end())
			{
				it->second.vPos = pMove->vTarPos;
			}
		}
		break;
	case MC_PARTY_INVITE_QUESTION:
		{
			// ��Ƽ������û
			MUID uidInvitor;
			if (!pCommand->GetParameter(&uidInvitor, 0, MPT_UID))	return CR_ERROR;

			m_uidInvitor = uidInvitor;
		}
		break;
	case MC_PARTY_REFRESH_INFO_ALL:
		{		
			// ��Ƽ���� ���� ����
			TD_PARTY* pParty;
			int nCount;
			vector<TD_PARTY_MEMBER> members;

			if (!pCommand->GetBlob(pParty, nCount, 0))	return CR_ERROR;
			if (!pCommand->GetBlob(members, 1))			return CR_ERROR;


			m_uidParty = pParty->m_uidParty;
			m_uidLeader = pParty->m_partySetting.m_uidLeader;

			m_vecMemberUID.clear();
			for(vector<TD_PARTY_MEMBER>::iterator it = members.begin(); it != members.end(); it++)
			{
				m_vecMemberUID.push_back(it->m_uidPlayer);
			}
		}
		break;
	case MC_PARTY_CHANGE_LEADER:		
		{
			// ��Ƽ�� ����
			MUID uidNewLeader;
			if (!pCommand->GetParameter(&uidNewLeader, 0, MPT_UID))	return CR_ERROR;

			m_uidLeader= uidNewLeader;
		}
		break;
	case MC_PARTY_CHANGE_NAME:
		// �̸�����; �����Ѵ�
		break;
	case MC_PARTY_CHANGE_LOOTING_RULE:
		// ���÷� ����; �����Ѵ�
		break;
	case MC_PARTY_INVITE:
		// ��Ƽ�� �ʴ� ��û�� ���� ����; �����Ѵ�
		break;
	case MC_PARTY_NOTIFY_JOIN:
		// ��Ƽ ����; �����Ѵ�
		break;
	case MC_PARTY_NOTIFY_LEAVE:
		{
			MUID uidLeaveMember;
			if (!pCommand->GetParameter(&uidLeaveMember, 0, MPT_UID))	return CR_ERROR;

			if (m_pAgent->GetUID() == uidLeaveMember)
			{
				m_uidParty.SetInvalid();
				m_vecMemberUID.clear();
			}
		}
		break;
	case MC_PARTY_REFRESH_INFO:
		// ��Ƽ�� ���� ����; �����Ѵ�
		break;
	case MC_PARTY_ACCEPT_CANCEL:
		// ���� ����; �����Ѵ�
		break;
	case MC_PARTY_INVITE_FOR_ME_QUESTION:		
		// �ʴ��û; ������		
		break;
	default:
		return CR_FALSE;
	}

	return CR_TRUE;
}

void XDummyBot_Party::OnRun(float fDelta)
{
	//XDummyBot_Roam::OnRun(fDelta);

	// �ൿ
	vector<PARTY_BEHAVIOR> vecBehavior;
	
	GetAllowedBehavior(vecBehavior);
	PARTY_BEHAVIOR nSelectedBehavior = SelectBehavior(vecBehavior);
	DoBehavior(nSelectedBehavior);
}

void XDummyBot_Party::PostInviteReq()
{
	XBIRDPOSTCMD1(m_pAgent->GetClient(), MC_PARTY_INVITE_REQ, MCommandParameterUID(m_uidAdjacencyPlayer));
}

void XDummyBot_Party::PostLeaveReq()
{
	if (m_uidParty.IsInvalid())	return;

	XBIRDPOSTCMD0(m_pAgent->GetClient(), MC_PARTY_LEAVE_REQ);
}

void XDummyBot_Party::PostKickReq()
{
	if (m_uidLeader != m_pAgent->GetUID())	return;
	if (m_vecMemberUID.empty())	return;

	// �������� ű
	int nMemberCount = m_vecMemberUID.size();
	int nRandomIndex = MMath::RandomNumber(0, nMemberCount - 1);

	MUID uidTargetPlayer = m_vecMemberUID[nRandomIndex];
	XBIRDPOSTCMD1(m_pAgent->GetClient(), MC_PARTY_KICK_REQ, MCommandParameterUID(uidTargetPlayer));
}

void XDummyBot_Party::PostInviteQuestionRespond()
{	
	if (m_uidInvitor.IsInvalid())	return;

	m_uidInvitor.SetZero();

	// �������� ����; 70%
	bool isAccept = MMath::RandomNumber(0, 100) > 30 ? true : false;
	XBIRDPOSTCMD1(m_pAgent->GetClient(), MC_PARTY_INVITE_QUESTION_RESPOND, MCommandParameterInt(isAccept));
}

void XDummyBot_Party::PostInviteForMeReq()
{
	// �ʴ��û; ������
}

void XDummyBot_Party::PostInviteForMeQuestionRespond()
{
	// �ʴ��û ����; ������
}

void XDummyBot_Party::PostChangeNameReq()
{
	XBIRDPOSTCMD1(m_pAgent->GetClient(), MC_PARTY_CHANGE_NAME_REQ, MCommandParameterString("TestName"));
}

void XDummyBot_Party::PostChangeLeaderReq()
{
	if (m_vecMemberUID.empty())				return;

	int nMemberCount = m_vecMemberUID.size();
	int nRandomIndex = MMath::RandomNumber(0, nMemberCount - 1);
	
	MUID uidNewLeader = m_vecMemberUID[nRandomIndex];

	XBIRDPOSTCMD1(m_pAgent->GetClient(), MC_PARTY_CHANGE_LEADER_REQ, MCommandParameterUID(uidNewLeader));
}

void XDummyBot_Party::PostChangeLootingRuleReq()
{
	char nCommand = LRC_FREE_FOR_ALL;
	char nRare = LRR_FREE_FOR_ALL;
	char nRareFor = LRRF_RARE_OR_HIGHER;
	
	XBIRDPOSTCMD3(m_pAgent->GetClient(), MC_PARTY_CHANGE_LOOTING_RULE_REQ, MCommandParameterChar(nCommand), MCommandParameterChar(nRare), MCommandParameterChar(nRareFor));
}

void XDummyBot_Party::GetAllowedBehavior( vector<PARTY_BEHAVIOR>& vecBehavior )
{
	vecBehavior.push_back(PARTY_BEHAVIOR_IDLE);

	if (m_uidParty.IsInvalid())
	{
		// ��Ƽ����; ���� �÷��̾� Ȯ��
		if (BeAdjacencyPlayer())
		{
			vecBehavior.push_back(PARTY_BEHAVIOR_INVITE);
		}

		// ������� ��
		if (m_uidInvitor.IsValid())
		{
			vecBehavior.push_back(PARTY_BEHAVIOR_RESPOND);
		}
	}
	else
	{
		// ��Ƽ��
		vecBehavior.push_back(PARTY_BEHAVIOR_LEAVE);

		// ��Ƽ��
		if (m_uidLeader == m_pAgent->GetUID())
		{
			if (BeAdjacencyPlayer())
			{
				vecBehavior.push_back(PARTY_BEHAVIOR_INVITE);
			}

			vecBehavior.push_back(PARTY_BEHAVIOR_CHANGE_NAME);
			vecBehavior.push_back(PARTY_BEHAVIOR_CHANGE_LEADER);
			vecBehavior.push_back(PARTY_BEHAVIOR_CHANGE_LOOTING_RULE);
			vecBehavior.push_back(PARTY_BEHAVIOR_KICK);
		}			
	}
}

PARTY_BEHAVIOR XDummyBot_Party::SelectBehavior( vector<PARTY_BEHAVIOR>& vecBehavior )
{
	if (vecBehavior.empty())	return PARTY_BEHAVIOR_IDLE;

#if 0
	
	int nSelectedIndex = MMath::RandomNumber(0, vecBehavior.size() - 1);
	return vecBehavior[nSelectedIndex];

#else
	// �ൿ �� ����
	int nFactor[MAX_PARTY_MEMBER_COUNT] = {1, 1, 2, 5, 10};

	vector<int> vecRange;	
	for each(PARTY_BEHAVIOR behavior in vecBehavior)
	{
		switch(behavior)
		{
		case PARTY_BEHAVIOR_IDLE:					vecRange.push_back(50);	break;
		case PARTY_BEHAVIOR_INVITE:					vecRange.push_back(20);	break;
		case PARTY_BEHAVIOR_RESPOND:				vecRange.push_back(20);	break;
		case PARTY_BEHAVIOR_LEAVE:					vecRange.push_back(nFactor[m_vecMemberUID.size()]);	break;
		case PARTY_BEHAVIOR_KICK:					vecRange.push_back(nFactor[m_vecMemberUID.size()]);	break;		
		case PARTY_BEHAVIOR_CHANGE_NAME:			vecRange.push_back(10);	break;
		case PARTY_BEHAVIOR_CHANGE_LEADER:			vecRange.push_back(10);	break;
		case PARTY_BEHAVIOR_CHANGE_LOOTING_RULE:	vecRange.push_back(10);	break;
		default:
			vecRange.push_back(0);
			break;
		}
	}

	// ����; ��������
	int nSum = 0;
	for each(int nRange in vecRange)
	{
		nSum += nRange;
	}

	int nNumber = MMath::RandomNumber(0, nSum);

	// �ش� �ൿ ã��
	for(size_t i = 0; i < vecRange.size(); i++)
	{
		if (nNumber < vecRange[i])
		{
			return vecBehavior[i];
		}

		nNumber -= vecRange[i];
	}

	return PARTY_BEHAVIOR_IDLE;
#endif
}

void XDummyBot_Party::DoBehavior( PARTY_BEHAVIOR nBehavior )
{
	switch(nBehavior)
	{
	case PARTY_BEHAVIOR_IDLE:
		// do nothing
		break;
	case PARTY_BEHAVIOR_INVITE:
		PostInviteReq();
		break;
	case PARTY_BEHAVIOR_RESPOND:
		PostInviteQuestionRespond();
		break;
	case PARTY_BEHAVIOR_LEAVE:
		PostLeaveReq();
		break;
	case PARTY_BEHAVIOR_KICK:
		PostKickReq();
		break;
	case PARTY_BEHAVIOR_CHANGE_NAME:
		PostChangeNameReq();
		break;
	case PARTY_BEHAVIOR_CHANGE_LEADER:
		PostChangeLeaderReq();
		break;
	case PARTY_BEHAVIOR_CHANGE_LOOTING_RULE:
		PostChangeLootingRuleReq();
		break;
	default:
		// do nothing
		break;
	}
}

bool XDummyBot_Party::BeAdjacencyPlayer()
{
	m_uidAdjacencyPlayer.SetZero();

	for(map<UIID, ENTITY_PLAYER>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		// ���ͷ��� �Ÿ� �ȿ� �ִ��� Ȯ��
		vec3 vPos = it->second.vPos;
		float fDistance = m_vPosition.XYDistanceTo(vPos);
		
		if (fDistance < CHECK_INTERACTION_DISTANCE)
		{
			// ��Ƽ���� �����Ѵ�.
			if (m_vecMemberUID.end() == find(m_vecMemberUID.begin(), m_vecMemberUID.end(), it->second.uid))
			{
				m_uidAdjacencyPlayer = it->second.uid;
				return true;
			}
		}
	}

	return false;
}
