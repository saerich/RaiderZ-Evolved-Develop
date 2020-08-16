#pragma once

#include "GActorObserver.h"
#include "CSTalentInfoEnum.h"
#include "GBuffObserver.h"

class GEntityActor;
class GTalentInfo;
class GBuffInfo;
class GBuff;

class GBuffReleaser : public GActorObserver, public GBuffObserver, public MTestMemPool<GBuffReleaser>
{
public:
	GBuffReleaser(GBuff* pBuff);
	virtual ~GBuffReleaser(void);

	// ƽ �̺�Ʈ
	void Update(float fDelta);

private:
	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// �ǰ� �̺�Ʈ
	virtual void OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// �ǰ� ���� �̺�Ʈ (�ŷ�Ʈ ����� �� 1ȸ ȣ��)
	virtual void OnHitSuccess( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) override;
	// ��� �ǰ� ���� �̺�Ʈ (��Ʈĸ������ �뵵)
	virtual void OnHitAll( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) override;
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
	virtual void OnPerfectGuard( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo ) override;
	// ���� ��� �̺�Ʈ
	virtual void OnAbsoluteGuard( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo ) override;
	// ���� ������ �������� �̺�Ʈ
	virtual void OnGuardEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo) override;
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) override;
	// ������ �ǰݵ����� �̺�Ʈ
	virtual void OnBuffHit(MUID uidUser, GBuffInfo* pBuffInfo) override;
	// �������� ����� �� �̺�Ʈ
	virtual void OnDamage(GEntityActor* pOwner, MUID uidAttacker, int nDamage, EFFECT_SOURCE_TYPE nEffectSourceType, int nEffectSourceID) override;
	// ������ �ִ� ���õ� ���
	virtual void OnMaxStacked() override;
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

	// �̵�, ���ݵ� �������� �ൿ�� ���� �� (Player ����)
	virtual void OnDoSomething() override;
private:
	// Triggered �� ��ϵ� ������ �ۿ����� ��
	virtual void OnAffected() override;

private:
	// ��ƽ���� ���� üũ
	void OnResist();

	// ������ Ǯ������ �����ΰ�?
	bool CheckType( TALENT_CONDITION condition );
	bool CheckParam( int nConditionParam );

	// ���� ���
	void Cancel();
	void CancelForced();
private:
	GBuff*		m_pBuff;
	bool		m_bCreateThisTick;
	// ���� ���� �˸���
	MRegulator	m_rgrResist;

	// Instance Data To Check
	int			m_nAccumulatedHitCount;		///< ������ �ǰ� ��
	int			m_nAccumulatedCriHitCount;	///< ������ ũ��Ƽ�� �ǰ� ��
	int			m_nAccumulatedDamage;		///< ������ ������
};
