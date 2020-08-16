#pragma once

#include "XPvPAreaInfo.h"
#include "XPvPAreaChecker.h"
#include "XPlayerFaction.h"

class XNetPlayer;

/// �����ʵ��� PVP ������ ���� ���� ó�� Ŭ����
class XPvPAreaLogic
{
protected:
	XPvPAreaInfo			m_PvPAreaInfo;		///< PvP ���� ����
	XPvPAreaChecker			m_PvPAreaChecker;

	void SetMyPlayerInPvPArea( XMyPlayer* pMyPlayer, XPlayerFactions& PlayerFactions );
	void OutputNarration( bool bPvPArea );
public:
	XPvPAreaLogic() {}
	~XPvPAreaLogic() {}

	wstring MakeEnemyName(XNetPlayer* pNetPlayer);
	void SetupEnemy( MUID uidPlayer );
	void Update( float fDelta, XPlayerFactions& myPlayerFactions );

	void RefreshNetPlayers( bool bInnerZone);

	bool IsInPvPArea();
	bool IsEnemy(XPlayer* pPlayer);
	wstring MakeEnemyChatMsg(const wchar_t* szOriginalChatMsg);

	wstring GetTeamName( int _team);
};


