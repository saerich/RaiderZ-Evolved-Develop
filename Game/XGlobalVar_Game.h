#pragma once

#include "XDialogInfo.h"
#include "XNPCInteractionInfo.h"
#include "XNPCShopInfo.h"
#include "XEnemyInfo.h"
#include "XTargetActorInfo.h"
#include "XSoundLootItem.h"

class XMyPlayer;
class XInteractionInfo;

typedef TD_ESSENTIAL_NPC_ICON	NPC_ICON_INFO;

struct DUEL
{
	int nTeam;
	vector<MUID> vecTeam1UIDs;
	vector<MUID> vecTeam2UIDs;

	void Clear()
	{
		nTeam = -1;
		vecTeam1UIDs.clear();
		vecTeam2UIDs.clear();
	}

	DUEL()
	{
		Clear();
	}
};

/// ���ӳ����� ����ϴ� ���� ����
class GlobalVar_Game
{	
private:
	XInteractionInfo*	m_pInteractionInfo;

public:
	bool		bTradeReq;
	bool		bPartyReq;
	bool		bGuildReq;
	bool		bDuelReq;

	wstring		strLastWhisper;				// �������� �Ӹ� �� ���

	XDialogInfo	DialogInfo;					// ��ȭ����
	XNPCInteractionInfo	NPCInteractionInfo;	// 
	XNPCShopInfo NPCShopInfo;				// ��������

	XEnemyInfo			EnemyInfo;

	struct MY_COMBAT_STANCE_AUTO_RELEASER_DATA
	{
		bool			bReserved;
		unsigned int	nReserveTime;
		MY_COMBAT_STANCE_AUTO_RELEASER_DATA() : bReserved(false), nReserveTime(0) {}
	};
	MY_COMBAT_STANCE_AUTO_RELEASER_DATA		MyCombatStanceAutoReleaserData;

	XMyPlayer*	pMyPlayer;

	GAME_TIME_TYPE		nLocalTime;
	GAME_WEATHER_TYPE	nLocalWeather;

	bool		bGameStart;					// ���� Ȱ��ȭ ����

	bool			bDisconnectByDuplicatedLogin;	// �ߺ����� ó���� ���� ���� �������� ����
	wstring			strLastErrorMsg;				// ������ ���� �޼���

	vector<TD_CHALLENGER_QUEST>	vecQuestList;

	XSoundLootItem		SoundLootItem;

	std::map<UIID, NPC_ICON_INFO>		vecNPCIcon;	// �ʵ峻�� ���� NPC Icon ����

	DUEL			Duel;

	GlobalVar_Game();

	~GlobalVar_Game();

	void Init_ForChangeField();

	void Init();

	XInteractionInfo& GetInteractionInfo() { return *m_pInteractionInfo; }

	bool IsCombatingNow();
};


