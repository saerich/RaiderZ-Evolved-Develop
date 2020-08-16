#pragma once

#include "XNetClient.h"
#include "XCommandTable.h"
#include "CTransData.h"
#include "MCommandParameter.h"

// ���� ��û
inline void XPostDuel_Request(const MUID& uidTarget)
{
	XPOSTCMD1(MC_DUEL_REQUEST, MCommandParameterUID(uidTarget));
}

// ���� ��û ����
inline void XPostDuel_QuestionRespond(const bool& bAnswer)
{
	XPOSTCMD1(MC_DUEL_QUESTION_RESPOND, MCommandParameterBool(bAnswer));
}

// ��Ƽ ���� ��û
inline void XPostPartyDuel_Request(const MUID& uidTarget)
{
	XPOSTCMD1(MC_DUEL_PARTY_REQUEST, MCommandParameterUID(uidTarget));
}