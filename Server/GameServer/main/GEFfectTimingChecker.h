#pragma once

#include "GActorObserver.h"
#include "GBuffObserver.h"

class GEntitySync;
class GEntityActor;
class GBuffInfo;

class GEFfectTimingChecker : public GActorObserver, public GBuffObserver, public MTestMemPool<GEFfectTimingChecker>
{
public:
	class Listener
	{
	public:
		virtual bool OnEvent(TALENT_CONDITION nTiming) = 0;
	};

	GEFfectTimingChecker();
	void AddListener(TALENT_CONDITION nTiming, Listener* pListenner);

private:
	void NotifyEvent(TALENT_CONDITION nTiming);

private:
	// �ǰ� �̺�Ʈ
	virtual void OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;
	// ġ��Ÿ �ǰ� �̺�Ʈ
	virtual void OnCriHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// ���� ġ��Ÿ�� ���������� �̺�Ʈ
	virtual void OnCriHitEnemy( GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo ) override;
	// ������ �̺�Ʈ
	virtual void OnMiss(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override;
	// ���� ������ ȸ�������� �̺�Ʈ
	virtual void OnMissEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo) override;
	// ��� �̺�Ʈ
	virtual void OnGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override;
	// ���� ��� �̺�Ʈ
	virtual void OnPerfectGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override;
	// ���� ��� �̺�Ʈ
	virtual void OnAbsoluteGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override;
	// ���� ������ �������� �̺�Ʈ
	virtual void OnGuardEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo) override;
	// �������� �ڽ��� �ǰݵ��� ��
	virtual void OnMagicHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;
	// �������� �ڽ��� �������� ��
	virtual void OnMagicHitEnemy(GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;
	// ���� �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnMagicActTalent( GTalentInfo* pTalentInfo ) override;
	// ���ٰ������� �ڽ��� �ǰݵ��� ��
	virtual void OnMeleeHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;
	// ���ٰ������� �ڽ��� �������� ��
	virtual void OnMeleeHitEnemy(GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;

private:
	// ���� ������ ȿ��ó��
	virtual void OnGain(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo) override;
	// ���� ������ ȿ��ó��
	virtual void OnLost(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo, bool bRoute) override;
	// ���� ������ ȿ��ó��
	virtual void OnPeriod() override;
	// ������ ���� ���� ��
	virtual void OnExpired(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo) override;
	// ������ ���� ���� ��
	virtual void OnDispelled() override;
	// ������ �ߺ��Ǽ� �ɸ� ���
	virtual void OnDuplicated(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo) override;
	// ������ �ִ� ���õ� ���
	virtual void OnMaxStacked() override;
	// ������ ���õǼ� �ɸ� ���
	virtual void OnStacked(float fDurationTime, float fPeriodTime) override;

private:
	typedef map<TALENT_CONDITION, vector<Listener*>> TimingMap;
	TimingMap			m_mapTimings;
};
