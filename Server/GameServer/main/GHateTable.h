#pragma once

#include "MUID.h"
#include "GTickRegulator.h"
#include "MTime.h"
#include "MMemPool.h"
#include "GHateTableDef.h"
#include "GHateTableSwapper.h"
#include "GHateTableLocker.h"
#include "GActorObserver.h"

class GEntityNPC;
class GEntityPlayer;
class GEntityActor;

#define HATE_ABSOLUTE_AMOUNT	999999

TEST_FORWARD_FT(HateTable, Fixture, EntryTimeout);
TEST_FORWARD_FT(HateTable, Fixture, EntryTimeout_NPCSpecificSetting);

class GHateTable : private GActorObserver, public MMemPool<GHateTable>
{
	TEST_FRIEND_FT(HateTable, Fixture, EntryTimeout);
	TEST_FRIEND_FT(HateTable, Fixture, EntryTimeout_NPCSpecificSetting);
private:
	typedef multimap<HatePoint, MUID> HatePoint_MAP_REVERSE;
	typedef map<MUID, float> HateTimeoutMap;
public:
	GHateTable(GEntityNPC* pOwner);
	~GHateTable();
		
	// �ʱ�ȭ
	void Init(float fExpiredTime);
	// ������Ʈ
	void	Update(float fDelta);
	// ���� ���� �ʱ�ȭ
	void	Reset();

	// ����Ʈ ���̺� ��ǥ ����UID ��ȯ
	MUID	GetTarget() const;
	// ����Ʈ���̺� �ֻ��� ����UID�� ��ȯ
	void	GetHighestTargetsUID(int nQty, vector<MUID>& vecTargets) const;
	// ����Ʈ���̺� �ֻ��� ����UID�� ��ȯ
	void	GetHighestTargets(int nQty, vector<pair<MUID,HatePoint>>& vecTargets) const;
	// ����Ʈ���̺� ��Ʈ�� ����
	size_t	GetQty() const;
	// ����Ʈ���̺� ������� ����
	bool	IsEmpty() const;
	// �ش� UID�� ����Ʈ���̺� ��ϵǾ��ִ��� ����
	bool	IsExist(const MUID& id) const;
	// �ش� UID�� ����Ʈ ��ġ ��ȯ 
	HatePoint	GetHate(const MUID& id) const;
	// �ش� UID�� ��Ʈ�� Ÿ�Ӿƿ� �ð�
	float	GetTimeoutTime(const MUID& id) const;
	// ���� �ð� ��ȯ
	float	GetExpiredTime() const;

	// ����Ʈ ��Ʈ�� ����
	void	DropPoint( MUID id );
	// ����Ʈ ���� ����
	void	EnableTimeout(bool bSwitch);
	// ����Ʈ ���� �̺�Ʈ ����
	bool	ApplyEvent(HATE_TARGET_SWAP_EVENT nEventType, float fDuration);
	// ��� ���͸� ���� ������
	void	Lock(MUID uidTarget, float fDurationTime);
	// Ÿ��Ʈ�� �ɸ��� ����
	void	SetTauntable(bool bEnable);
	// Ÿ��Ʈ�� �ɸ� �� �ִ��� ����
	bool	IsTauntable();
	
	// ����Ʈ �߰� �Լ��� ----------
	void	AddPoint_FoundEnemy(GEntityActor* pActor);	
	void	AddPoint_GainDamage(GEntityActor* pActor, int nDamage, float fHateFactor = 1.0f);
	void	AddPoint_EnemyApplyEffect(GEntityActor* pActor, int nHateAdj);
	void	AddPoint_EnemyRecoverHP(GEntityActor* pActor, int nRecoveredHP, float fHateFactor = 1.0f);
	void	AddPoint_Custom(GEntityActor* pActor, int nAmount);	

	// Ÿ�� UID�� ��ȯ
	void	CollectTarget(vector<MUID>& vecTargets) const;

private:
	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) override;
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override;
	// �ڽ��� �׾����� ȣ��Ǵ� �̺�Ʈ
	virtual void OnDie() override;
	// �ڽ��� ���� �Ǿ����� (NPC ����)
	virtual void OnDespawn() override;
	// �ڽ��� ��ü�� �Ҹ�ɶ�
	virtual void OnDestroy() override;
	// �ڽ��� ����Ʈ���̺� Ÿ���� �߰��Ǿ�����
	virtual void OnAddHateEntry( GEntityActor* pEnemy ) override;
private:
	// ����Ʈ ��Ʈ�� �߰�
	void InsertEntry(MUID uid, HatePoint point);
	// ����Ʈ ��Ʈ�� ����
	void DeleteEntity(const MUID& uid);
	// ����Ʈ ��Ʈ�� ���� �� �������� �˸�
	void Drop(const MUID& uid);
	// ����Ʈ���̺� ��Ʈ���� ��ġ����
	void UpdateTimeout(float fElapsedTime);
	// ����Ʈ���̺� ��ŷ ����
	void	UpdateHateRanking();
	// ����Ʈ���̺� ��ġ ����
	void	AdjHatePoint(GEntityActor* pActor, HatePoint point);
	// �ش� UID�� ���Ͱ� ��ȿ���� �˻�
	bool	IsValidTarget(MUID id) const;
	// �ش� UID�� ���� ��ü ��ȯ
	GEntityActor* FindActor(const MUID& id);
	// �ش� Ÿ���� ��� ���� ������ �ִ��� �˻�
	bool CheckHeight(GEntityPlayer* pEnemy);

private:
	GEntityNPC*			m_pOwner;
	HatePointMap		m_mapHateTable;
	MUID				m_uidLastTarget;
	GHateTableSwapper	m_Swapper;
	GHateTableLocker	m_TargetLocker;
	MRegulator			m_rgrTick;
	HateTimeoutMap		m_mapTimeout;
	bool				m_bEnableTimeout;
	float				m_fExpiredTime;
};