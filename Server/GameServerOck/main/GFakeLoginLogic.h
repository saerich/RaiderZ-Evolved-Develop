#pragma once

#include "CSChrInfo.h"

class GPlayerObject;
struct TD_INSERT_CHARACTER_INFO;

class GFakeLoginLogic
{
public:
	// ĳ���� �߰�/����
	bool RequestAccountCharList(MUID uidPlayer);
	bool InsertCharacter(MUID uidPlayer, TD_INSERT_CHARACTER_INFO* pInsertCharInfo);
	bool DeleteCharacter(MUID uidPlayer, int8 nCharIndex);
	bool SelMyChar(MUID uidPlayer, int8 nCharIndex);

	// ĳ���� ���� ȭ������ �̵�
	bool MoveToLoginServer(const MUID& uidPlayer);

private:
	static MUIDGenerator m_AcceptorUIDGenerator;

	bool CheckInsertCharacter_Account(GPlayerObject* pPlayerObject);
	bool CheckInsertCharacter_Parameter(MUID uidPlayer, TD_INSERT_CHARACTER_INFO* pInsertCharInfo);
	bool ApplyInsertCharacter(MUID uidPlayer, AID nAID, TD_INSERT_CHARACTER_INFO* pInsertCharInfo);
};
