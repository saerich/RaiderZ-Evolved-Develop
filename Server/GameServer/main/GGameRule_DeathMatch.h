#pragma once

#include "GBattleArena.h"

class GEntityPlayer;

class GGameRule_DeathMatch : public GBattleArena::GameRule, public MTestMemPool<GGameRule_DeathMatch>
{
public:
	GGameRule_DeathMatch();

	void SetMaxLifeCounter(int nMaxLifeCounter);

	// ��Ģ �ʱ�ȭ
	virtual void Init(const vector<const MARKER_INFO*> vecStaringMarkers, const BattleArena::TEAM_MAP& mapEntries) override;
	// ��Ģ ������Ʈ
	virtual void Update(float fDelta) override;

	void Rebirth( GEntityPlayer* pPlayer );
	// ������ �����°�?
	virtual bool IsFinished(const BattleArena::TEAM_MAP& mapEntries, MUID& uidWinner) override;
	// ��Ʈ�� ����� �Ѹ��� �׾��� ���
	virtual void OnTeamMemberDie(GEntityPlayer* pPlayer, const BattleArena::Team& team) override;
	// ��Ʈ�� ����� �Ѹ��� ������ ��Ȱ�Ǿ��� ���
	virtual void OnRebirthTeamMemberBySelf( GEntityPlayer* pPlayer, const BattleArena::Team& team ) override;
	// ��Ʈ�� ����� �Ѹ��� Ÿ���� ��Ȱ���׾��� ���
	virtual void OnRebirthTeamMemberByOther( GEntityPlayer* pPlayer, const BattleArena::Team& team ) override;
	// ��Ʈ�� ����� ���� ������ ���
	virtual void OnDisconnect(GEntityPlayer* pPlayer) override;
	// ��Ʈ���� ���ŵɶ� ȣ��
	virtual void OnDeleteTeam(const BattleArena::Team& team) override;
	// ������ ó��
	virtual void OnRejoin(GBattleArena::Status nGameStatus, GEntityPlayer* pPlayer) override;

	// ���� ��ġ ��ȯ
	virtual vec3 GetSpotPos(GEntityPlayer* pPlayer, const BattleArena::Team& team) override;

	virtual GBattleArena::GameRule* Clone() override;
private:
	// ������ ����� ��
	void OnFinished(MUID uidWinner);

	// ��Ȱ��⿡ �ֱ�
	void AddRebirthTimer( const MUID& uidPlayer, GEntityPlayer* pPlayer );
	// ��Ȱ��⿡ ���
	void EraseRebirthTimer(const MUID& uidPlayer);
	
	void RouteToTeam( const BattleArena::Team& team, minet::MCommand* pCommand );
	void RouteToAllTeam( minet::MCommand* pCommand );
	void RouteScore();
	void RouteDie(const MUID& uidPlayer);
	void RouteKill(GEntityPlayer* pPlayer);
private:
	typedef std::map<MUID, int>								LIFE_MAP;
	typedef std::map<MUID, const MARKER_INFO*>					STARTINGMARKER_MAP;
	typedef std::map<MUID, pair<MRegulator,GEntityPlayer*>>	REBIRTHTIME_MAP;

	LIFE_MAP				m_mapLifeCounter;
	BattleArena::TEAM_MAP  m_mapTeams;
	int						m_nMaxLifeCounter;
	REBIRTHTIME_MAP			m_mapRebirthTimers;	
	STARTINGMARKER_MAP		m_mapStaringMarkers;
	float					m_fElapsedTime;
};
