#pragma once

#include "GBattleArena.h"
#include "GBattleArenaMatcher.h"

class GEntityPlayer;
typedef std::map<MUID, GBattleArena*>	BATTLEARENA_MAP;

class GBattleArenaMgr : public GBattleArenaMatcher::Lisener, public MTestMemPool<GBattleArenaMgr>
{
public:
	GBattleArenaMgr(void);
	~GBattleArenaMgr(void);

	// �׽�Ʈ�� ��Ģ �߰�
	void CreateTestRules();
	// ƽ ó��
	void Update(float fDelta);
	// ���� ��û
	bool RegisterTeam(GEntityPlayer* pPlayer);
	// ���� ���
	bool DeregisterTeam(GEntityPlayer* pPlayer);
	bool DeregisterTeam( MUID uidTeam );
	// ���������� Ȯ��
	bool IsEntryTeam(GEntityPlayer* pPlayer);
	bool IsEntryTeam( MUID uidTeam );
	// ���� ��Ʈ�� �� ��ȯ
	size_t GetTeamQty() const;
	// ������ �� ��ȯ
	size_t GetBattleArenaQty() const;
	// ��Ʈ�� ��ȯ
	BattleArena::Team FindTeam(MUID uidTeam) const;
	// ������ ��ȯ
	GBattleArena* FindBattleArena(MUID uidBattleArena) const;
	// ��ó ��ȯ
	inline GBattleArenaMatcher& GetMatcher()		{ _ASSERT(m_pMatcher); return *m_pMatcher; }
	// ��ó ��ȯ
	inline GBattleArenaMatcher& GetMatcher() const	{ _ASSERT(m_pMatcher); return *m_pMatcher; }

	// ������ ó��
	void TryRejoin(GEntityPlayer* pPlayer);

	// �����Ģ �߰�
	void AddGameRule(GBattleArena::GameRule* pRule);
	// �����Ģ �ʱ�ȭ
	void ClearGameRules();

	// ��Ʈ���� ���ŵɶ� ȣ��
	void OnDeleteTeam(MUID uidTeam);

	// ������ ��������� ����
	bool IsBattleArenaOpponent(GEntityPlayer* pPlayer1, GEntityPlayer* pPlayer2) const;

	// ���� ��ġ ��ȯ
	vec3 GetSpotPos(GEntityPlayer* pPlayer) const;

protected:
	virtual void CreateDefaultMatcher();
private:
	// ��Ʈ���� ��Ī�� ����
	virtual void OnMatched(BattleArena::Team& teamRequester, BattleArena::Team& teamOpponent) override;
	// ������ ������ �����
	void OnBattleArenaFinished(MUID uidBattleArena, GBattleArena* pBattleArena);
	// ������ �Ҵ�
	static GBattleArena* NewBattleArena(GBattleArena::GameRule* pRule);
	// ������ ����
	void CreateBattleArena(BattleArena::Team& teamRequester, BattleArena::Team& teamOpponent);
	// ������ ����
	void DestroyBattleArena(GBattleArena* pBattleArena);
private:
	GBattleArenaMatcher*	m_pMatcher;
	BATTLEARENA_MAP			m_mapBattleArena;
	// ���� ��Ģ���
	vector<GBattleArena::GameRule*>			m_vecGameRules;
};
