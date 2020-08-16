#include "StdAfx.h"
#include "GMatchRule_SameTeamMember.h"
#include "GDef.h"
#include "GGlobal.h"
#include "GPartySystem.h"

GMatchRule_SameTeamMember::GMatchRule_SameTeamMember()
{
}

bool GMatchRule_SameTeamMember::IsMatch( const BattleArena::Team& teamRequester, const BattleArena::Team& teamOpponent )
{
	// ��Ƽ ���� Ȯ��
	if (NULL == gsys.pPartySystem->FindParty(teamRequester.uidParty))	return false;
	if (NULL == gsys.pPartySystem->FindParty(teamOpponent.uidParty))	return false;


	// ���� ����� Ȯ��
	int nRequesterCount = gsys.pPartySystem->CalculateOnlineMemberCount(teamRequester.uidParty);
	int nOpponentCount = gsys.pPartySystem->CalculateOnlineMemberCount(teamOpponent.uidParty);

	return nRequesterCount == nOpponentCount;
}
