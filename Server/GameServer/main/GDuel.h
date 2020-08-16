#pragma once

#include "MCommand.h"
#include "MTime.h"

using namespace minet;

class GField;
class GEntityPlayer;
class GEntityActor;
class GEntityNPC;

enum DUEL_STATE
{
	DUEL_QUESTION = 0,
	DUEL_READY,
	DUEL_FIGHTING,
	DUEL_FINISH,
};

#define DUEL_READY_COUNT	3
#define DUEL_READY_TIME		DUEL_READY_COUNT*1.0f
#define DUEL_RADIUS			3000.0f


//
// GDuel
//
class GDuel : public MTestMemPool<GDuel>
{
	enum DUEL_TEAM
	{
		TEAM_NONE=-1,
		TEAM_1=0,
		TEAM_2=1,
	};
public:
	GDuel(MUID uid, GField* pDuelField, GEntityPlayer* pFighter1, GEntityPlayer* pFighter2);
	GDuel( MUID uid, GField* pDuelField, const vector<GEntityPlayer*>& pFighters1, const vector<GEntityPlayer*>& pFighters2 );
	virtual ~GDuel();

	// �ش� �� ���ϱ�
	GDuel::DUEL_TEAM GetTeam( MUID uidPlayer );
	// ƽ���� ȣ���
	void Update(float fDelta);
	// ������ �غ������ ȣ��
	void Ready();
	// ������ ��ҵ����� ȣ��
	void Cancel();
	// ������ �� �Ѹ��� �׾����� ȣ��
	void Killed(MUID uidDeadMan);
	// ������ �� �Ѹ��� ������ �������� ȣ��
	void Disconnect( GEntityPlayer* pDisconnector );
	// �ش� ���Ͱ� ���������� Ȯ���ϴ� �Լ�
	bool IsDuelMember(GEntityActor* pInvoker);
	bool IsDuelMember( MUID uidInvoker );
	// ���� ��ü�� UID�� ��ȯ
	MUID GetUID()			{ return m_UID; }
	// ���� ���¸� ��ȯ
	DUEL_STATE		GetState()			{ return m_nState; }
	// ������ ����ƴ��� ��ȯ
	bool			IsFinished()		{ return m_nState == DUEL_FINISH; }
	// ���� ������ ������ ��ȯ
	vec3			GetCenterPos()		{ return m_vCenterPos; }
	// ����� ��ȯ
	DUEL_TEAM GetOpponentTeam(DUEL_TEAM nTeam);
	// ����� ��ȯ
	vector<MUID>& GetOpponentTeamUIDList(DUEL_TEAM nTeam);
	// �� UID ��ȯ
	vector<MUID>& GetTeamUIDList(DUEL_TEAM nTeam);
	// �� UID ��ȯ
	vector<MUID>& GetAllFightersUIDList();
	// �����ڵ鿡�� Ŀ�ǵ带 ����	
	void RouteToFighters(MCommand* pNewCmd);
	// ������ ��ȯ
	bool IsEnemy( MUID uidPlayer, MUID uidOpponent );
	// �÷��̾� ���ϱ�
	GEntityPlayer* GetPlayer(MUID uidPlayer);
	// Ż���� �÷��̾����� ����
	bool IsLoser(MUID uid);
	// ��ǥ�� ���ϱ�
	GEntityPlayer* GetLeader1();
	GEntityPlayer* GetLeader2();

	// ���� ���� ����
	bool IsObservable() const;
	// ���� ī���� ����
	void IncreaseObserveCount();
	// ���� ī���� ����
	void DecreaseObserveCount();
	// ���� ī���� ��ȯ
	int GetObserveCount() const;

	// ��Ƽ���� ���� ����
	bool IsSingleDuel();

protected:
	DUEL_STATE		m_nState;

private:
	// ������ �����Ѵ�.
	void Duel();
	// ������ �����Ѵ�.
	void Finished();
	// �غ� �Ϸ�� �������� Ȯ��
	bool CheckReady();
	// ���� ���Ῡ�� ȭ��
	bool CheckFinish();
	// �������߿� ����ģ ����� �ִ��� Ȯ��
	void CheckRunaway();
	bool CheckRunawayImpl(MUID uid);
	// ���� ������ ������ ���
	void CalcCenterPoint();
	// ��������� ������Ų��.
	void CreateFlag();
	// ��������� �Ҹ��Ų��.
	void DestroyFlag();

	// �غ� Ŀ�ǵ带 ���� (�����ڵ鿡��)
	void RouteReady();
	// ���� Ŀ�ǵ带 ���� (�����ڵ鿡��)
	void RouteFight();
	// �Ϸ� Ŀ�ǵ带 ���� (�����ڵ鿡��)
	void RouteFinished();
	// ������� Ŀ�ǵ带 ���� (�����ڵ鿡��)
	void RouteCancel();
	// ���� ī���� Ŀ�ǵ带 ���� (�����ڵ鿡��)
	void RouteReadyCount();
	// ���� �й��� ��� �˸�
	void RouteDefeated(GEntityPlayer* pDefeater);

	// ���� �ʱ�ȭ
	void Cleanup();
	// �ش� �÷��̾��� ����� ���� ����
	void RemoveAllDebuff(GEntityPlayer* pPlayer);

	// �÷��̾� ����
	void InsertFighter(DUEL_TEAM nTeam, GEntityPlayer* pPlayer);
	// �й��� �߰�
	void InsertLoser( MUID uidPlayer );
	
	// ��Ƽ�� ���ϱ�
	GEntityPlayer* GetLeaderImpl(const vector<MUID>& vecFighters);

private:
	typedef map<MUID, DUEL_TEAM> DuelTeamIndexMap;

	DuelTeamIndexMap	m_mapDuelTeamIndex;
	MUID				m_UID;
	int					m_nReadyCount;
	vector<MUID>	m_vecAllFighters;
	vector<MUID>	m_vecFighters1;
	vector<MUID>	m_vecFighters2;
	set<MUID>		m_setLosers1;
	set<MUID>		m_setLosers2;
	DUEL_TEAM		m_nWinTeam;
	MRegulator		m_rgrReady;
	vec3			m_vCenterPos;
	GField*			m_pField;
	GEntityNPC*		m_pFlag;
	int				m_nObserveCount;
};