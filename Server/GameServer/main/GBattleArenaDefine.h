#pragma once

namespace BattleArena {

enum GameType
{
	GAMETYPE_INVALID	 = 0,
	GAMETYPE_DEATHMATCH	 = 1,
};

struct Team
{
	enum Status
	{
		INVALID = 0,	// ��ȿ�� ����
		WAITING_FOR_MATCH = 1,	// ��Ī �����
		WAITING_FOR_PLAY = 2,	// ���� �����
		PLAYING = 3,	// ������
	};

	MUID	UID;
	MUID	uidBattleArena;
	Status	nStatus;
	MUID	uidParty;

	Team()	{ Reset(); }
	void Reset()
	{
		UID.SetInvalid();
		nStatus = INVALID;
		uidParty.SetInvalid();
	}
	bool IsInvalid() const 
	{
		return nStatus == INVALID;
	}

	bool IsMatched() const { return  nStatus != WAITING_FOR_MATCH; }
};

typedef std::map<MUID, Team>	TEAM_MAP;

};	// namespace BattleArena