#pragma once

#include "GActorObserver.h"
#include "MTime.h"
#include "CTransData.h"

class GEntityPlayer;
class GEntityActor;

class GPlayerEnemyInfoSender : private GActorObserver, public MTestMemPool<GPlayerEnemyInfoSender>
{
public:
	GPlayerEnemyInfoSender(GEntityPlayer* pOwner);
	~GPlayerEnemyInfoSender();
	void Update(float fDelta);	
	void Clear();

	// ���� �ִ��� ����
	bool HasEnemy() const;


	bool IsObserveDuel() const;
	bool AttachObserveDuel(MUID uidDuel);
	bool DetachObserveDuel();
private:
	// �� ���� ����
	void UpdateEnemyEntry();

	TD_ENEMY_INFO MakeTD( GEntityActor* pEachActor );
	void RouteInfo();
	void RouteClearInfo();	

	// �ش� UID�� ���� ��ü ��ȯ
	GEntityActor* FindActor(const MUID& id);
	int			  CalcCheckSum();

	// �ǰ� �̺�Ʈ
	virtual void OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;
	// ��밡 �ڽ��� ����Ʈ���̺��� �߰��Ͽ�����
	virtual void OnAddHateTarget( GEntityActor* pEnemy ) override;
	// ��밡 �ڽ��� ����Ʈ���̺��� �����Ͽ�����
	virtual void OnLostHateTarget( MUID uidEnemy ) override;
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override;
	// ���� �غ�
	virtual void OnDuelReady( const vector<MUID>& pEnemy ) override;
	// ���� ����
	virtual void OnDuelFinish( const vector<MUID>& pEnemy ) override;
	
private:
	GEntityPlayer*	m_pOwner;
	MRegulator		m_rgrTick;	
	set<MUID>		m_setEnemies;	

	MUID			m_uidLastHittedEnemy;	///< ���������� �÷��̾�� ���� ��
	UIID			m_nLastHitEnemy;		///< ���������� �÷��̾�� ���� ��
	int				m_nOldCheckSum;			///< ���� �ٲ� ��������� �����ϴµ� ����	
	MUID			m_uidObserveDuel;		///< �������� ����
};
