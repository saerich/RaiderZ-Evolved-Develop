#pragma once

#include "GBattleArenaDefine.h"
#include "MTime.h"

class GEntityPlayer;
struct MARKER_INFO;

namespace minet {
class MCommand;
};

class GBattleArena : public MTestMemPool<GBattleArena>
{
public:
	enum Status
	{
		INVALID = 0,	// ��ȿ�� ����
		PREPARE,		// �����ڵ� ���
		COUNTING,		// ���� �����
		PLAYING,		// ������
		SCOREBOARD,		// ������ �����ֱ�
		FINISH,			// ����
	};

	class GameRule
	{
	public:
		virtual ~GameRule() {}

		virtual GameRule* Clone() = 0;

		// ��Ģ �ʱ�ȭ
		virtual void Init(const vector<const MARKER_INFO*> vecStaringMarkers, const BattleArena::TEAM_MAP& mapEntries) {}
		// ��Ģ ������Ʈ
		virtual void Update(float fDelta) {}
		// ������ �����°�?
		virtual bool IsFinished(const BattleArena::TEAM_MAP& mapEntries, MUID& uidWinner) = 0;
		// ��Ʈ�� ����� �Ѹ��� �׾��� ���
		virtual void OnTeamMemberDie(GEntityPlayer* pPlayer, const BattleArena::Team& team) {}
		// ��Ʈ�� ����� �Ѹ��� ������ ��Ȱ�Ǿ��� ���
		virtual void OnRebirthTeamMemberBySelf( GEntityPlayer* pPlayer, const BattleArena::Team& team ) {}
		// ��Ʈ�� ����� �Ѹ��� Ÿ���� ��Ȱ���׾��� ���
		virtual void OnRebirthTeamMemberByOther( GEntityPlayer* pPlayer, const BattleArena::Team& team ) {}
		// ��Ʈ�� ����� ���� ������ ���
		virtual void OnDisconnect(GEntityPlayer* pPlayer) {}
		// ��Ʈ���� ���ŵɶ� ȣ��
		virtual void OnDeleteTeam(const BattleArena::Team& team) {}
		// ������ ó��
		virtual void OnRejoin(GBattleArena::Status nGameStatus, GEntityPlayer* pPlayer) = 0;
		// ���� ��ġ ���
		virtual vec3 GetSpotPos(GEntityPlayer* pPlayer, const BattleArena::Team& team) { return vec3::ZERO; }
	};

	class Lisener
	{
	public:
		virtual void OnBattleArenaShowScoreBoard(const BattleArena::TEAM_MAP& mapEntries)	{}
		virtual void OnBattleArenaComplete(const BattleArena::TEAM_MAP& mapEntries)			{}
	};

	GBattleArena(MUID uid, GameRule* pRule);
	~GBattleArena(void);

	// ������ ���
	void RegisterLisener(Lisener* pLisener);

	// UID ��ȯ
	MUID GetUID()							{ return m_UID; }
	// ��������� ��ȯ
	Status GetStatus() const;
	// ��������� ����
	void ChangeStatus(Status nStatus);
	// ���ο� ����
	void Join(MUID uidTeam, BattleArena::Team& team);
	// ƽ ó��, ��ȯ���� �ش� �������� ����ƴ��� ����
	bool Update(float fDelta);
	
	// ������ �ο��� ��ȯ
	size_t GetTeamQty() const						{ return m_mapEntries.size(); }
	// ������ ��ȯ
	const BattleArena::Team* FindTeam(MUID uidTeam) const;

	// ������ ���� �ʵ� �׷� UID ��ȯ
	MUID GetFieldGroupUID() { return m_uidFieldGroup; }

	// ��Ʈ�� ����� �Ѹ��� �׾��� ���
	void OnTeamMemberDie(GEntityPlayer* pPlayer, const BattleArena::Team& team);
	// ��Ʈ�� ����� �Ѹ��� ������ ��Ȱ�Ǿ��� ���
	void OnRebirthTeamMemberBySelf( GEntityPlayer* pPlayer, const BattleArena::Team& team );
	// ��Ʈ�� ����� �Ѹ��� Ÿ���� ��Ȱ���׾��� ���
	void OnRebirthTeamMemberByOther( GEntityPlayer* pPlayer, const BattleArena::Team& team );
	// ��Ʈ�� ����� ���� ������ ���
	void OnDisconnect(GEntityPlayer* pPlayer);
	// ��Ʈ���� ���ŵɶ� ȣ��
	void OnDeleteTeam(const BattleArena::Team& team);

	// ������ �غ������� ����
	bool IsGamePreparing() const;
	// ������ ���������� ����
	bool IsGamePlaying() const;
	// ������ ���������� ����
	bool IsGameFinishing() const;

	// ������ ó��
	void OnRejoin(GEntityPlayer* pPlayer);

	// ���� ��ġ ��ȯ
	vec3 GetSpotPos(GEntityPlayer* pPlayer, const BattleArena::Team& team);
	
private:
	// ���ο� ������ ������
	void OnJoin(const BattleArena::Team& team);
	// ������ ������ �غ� �Ǿ����� ����
	bool IsAllReady();
	// Enter �̺�Ʈ
	void OnPrepareEnter();
	void OnCountingEnter();
	void OnPlayingEnter();
	void OnScoreBoardEnter();
	void OnFinishEnter();
	// Leave �̺�Ʈ
	void OnPrepareLeave();
	void OnCountingLeave();
	void OnPlayingLeave();
	void OnScoreBoardLeave();
	void OnFinishLeave();
	// Update �̺�Ʈ
	void OnPrepareUpdate(float fDelta);
	void OnCountingUpdate(float fDelta);
	void OnPlayingUpdate(float fDelta);
	void OnScoreBoardUpdate(float fDelta);
	void OnFinishUpdate(float fDelta);
	
	// ��Ʈ������ ��Ŷ ����
	void RouteToTeam(const BattleArena::Team& team, minet::MCommand* pCommand);
	// ��� ��Ʈ������ ��Ŷ ����
	void RouteToAllTeam(minet::MCommand* pCommand);
	// ���� ��Ŀ ����
	void ExtractStartingMarkers();
	// ��� ��Ʈ�� ������� �����ʵ�� �̵�
	bool MoveAllTeamToTrialField();
	// ��� ��Ʈ�� ������� �����ʵ� ��ġ�� �̵�
	bool MoveAllTeamToEnterPos();

	// ������ UID�� �÷��̾�� �ο�
	void SetBattleArenaUIDToTeam( BattleArena::Team &team );
	// ������ UID�� �÷��̾�κ��� ����
	void ClearBattleArenaUIDToTeam();

	// ������ ������ ���� �ʵ� UID ����
	void SetFieldGroupUID(MUID uidCreatedFieldGroup) { m_uidFieldGroup = uidCreatedFieldGroup; }

	// �����Ģ �ʱ�ȭ
	void InitGameRules();
	// �����Ģ �˻�
	bool CheckGameRules(MUID& uidWinner);
	// ���� ��Ģ ������Ʈ
	void UpdateGameRules( float fDelta );

	// ��Ʈ�� ��� ��� ��Ȱ
	void RebirthToAllTeam();
	// ��Ʈ�� ��� ��� ȸ��
	void FullHealToAllTeam();

	// ���� ���� ó��
	void ProcessFinish();
private:
	MUID		m_UID;
	Status		m_nStatus;
	BattleArena::TEAM_MAP	m_mapEntries;
	vector<Lisener*>		m_vecLiseners;
	// ���� ��Ģ
	GameRule*				m_pGameRule;
	// ���� ���� ī����
	MRegulator				m_rgrStartingCounter;
	// ������ ī����
	MRegulator				m_rgrScoreBoardCounter;
	// ���� ��Ʈ��
	MUID					m_uidWinner;
	// ���� ��Ŀ ����
	vector<const MARKER_INFO*>	m_vecStaringMarkers;
	// ������ ���� �ʵ� �׷� UID
	MUID					m_uidFieldGroup;
};
