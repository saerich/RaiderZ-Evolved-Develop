#pragma once

class XPlayerPVP
{
public:
	enum XPvPAreaTeam
	{
		ePvPAreaTeam_NA,
		ePvPAreaTeam_1,
		ePvPAreaTeam_2
	};
private:

	//bool				m_bDuel;					///< ��� �Ѽ�. ���� ������� �÷��̾�� �� ���� true
	uint32				m_nBattleArenaFaction;		///< ��Ʋ �Ʒ����� �Ѽ�

	XPvPAreaTeam		m_ePvPAreaFaction;

	bool IsAllyInPvPArea(XPlayerPVP* target);		///< pvp �������� �Ʊ������� üũ

public:
	XPlayerPVP(void);
	~XPlayerPVP(void);

	void SetBattleArenaEnemy();
	//void SetDuel();
	//bool GetDuel() { return m_bDuel; }

	void ReleaseBattleArenaEnemy();
	//void ReleaseDuel();

	void SetPvPAreaFaction(bool bFaction1, bool bFaction2);
	void ReleasePvPAreaFaction();

	bool IsAlly(XPlayerPVP* target);		///< ����, ������
	bool IsEnemy( XPlayerPVP* target );		///< ��

	XPvPAreaTeam GetPvPAreaTeam() { return m_ePvPAreaFaction; }
};
