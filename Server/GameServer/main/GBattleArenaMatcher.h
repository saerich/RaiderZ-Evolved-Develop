#pragma once

#include "GBattleArena.h"
#include "GBattleArenaDefine.h"

class GEntityPlayer;

class GBattleArenaMatcher : public GBattleArena::Lisener, public MTestMemPool<GBattleArenaMatcher>
{
public:
	GBattleArenaMatcher(void);
	~GBattleArenaMatcher(void);

	class Rule
	{
	public:
		virtual ~Rule() {}

		virtual bool IsMatch(const BattleArena::Team& teamRequester, const BattleArena::Team& teamOpponent) = 0;
	};

	class Lisener
	{
	public:
		// ��Ʈ���� ��Ī�� ����
		virtual void OnMatched(BattleArena::Team& teamRequester, BattleArena::Team& teamOpponent)	{}
	};

	// ��Ī��Ģ �߰�
	void AddRule(Rule* pRule);
	// ��Ī��Ģ �ʱ�ȭ
	void ClearRules();
	// ������ ���
	void SetLisener(Lisener* pLisener)		{ m_pLisener = pLisener; }

	// ���� ��û
	bool RegisterTeam(GEntityPlayer* pPlayer);
	
	// ���� ���
	bool DeregisterTeam(GEntityPlayer* pPlayer);
	// ���� ���
	bool DeregisterTeam(MUID uidTeam);
	
	// ���������� Ȯ��
	bool IsEntryTeam(GEntityPlayer* pPlayer);
	// ���������� Ȯ��
	bool IsEntryTeam(MUID uidTeam);
	// ���� ��Ʈ�� ��
	size_t GetTeamQty() const				{ return m_mapTeams.size(); }
	// ��Ʈ�� ��ȯ
	BattleArena::Team Lookup(MUID uidTeam) const;
	// �����忡�� ������
	bool IsEnemy(GEntityPlayer* pPlayer1, GEntityPlayer* pPlayer2);

protected:
	// ��Ʈ���� ��ϵǾ��� ��
	virtual void OnRegisterTeam(BattleArena::Team& team);
	
	// ��Ʈ���� ������ �Ǿ��� ��
	virtual void OnDeregisterTeam(BattleArena::Team& team);
	
	// ��Ʈ���� ��Ī�� �õ��� ��
	virtual void OnTryMatch(BattleArena::Team& team);
	// ��Ʈ���� ��Ī�� ����
	virtual void OnMatched(BattleArena::Team& teamRequester, BattleArena::Team& teamOpponent);
private:
	bool CheckRules(const BattleArena::Team& teamRequester, const BattleArena::Team& teamOpponent);
	void RouteToTeam( const BattleArena::Team& team, minet::MCommand* pCommand );

	virtual void OnBattleArenaComplete(const BattleArena::TEAM_MAP& mapTeams);

	bool AddTeam( GEntityPlayer* pPlayer );
	void DeleteTeam( const BattleArena::Team &team );

	void RouteRegister( BattleArena::Team& team );
	void RouteDeregister( BattleArena::Team& team );
protected:
	// ������ ������
	BattleArena::TEAM_MAP	m_mapTeams;
	// ��Ģ���
	vector<Rule*>			m_vecRules;
	// ������
	Lisener*				m_pLisener;
};
