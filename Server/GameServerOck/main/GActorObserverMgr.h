#pragma once

#include "GActorObserverInterface.h"

class GActorObserver;
class GEntityActor;
class GTalentInfo;
class GBuffInfo;
class GItem;
class GJob;

class GActorObserverMgr : public GActorObserverInterface
{
public:

	// �������� �����Ŵ
	void AttachObserver(GActorObserver* pOvr);
	// �������� �и���Ŵ
	void DetachObserver(GActorObserver* pOvr);

	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) override;
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override;
	// �ڽ��� �׾�����
	virtual void OnDie() override;
	// �ڽ��� ���� �Ǿ����� (NPC ����)
	virtual void OnDespawn() override;
	// �ڽ��� ��ü�� �Ҹ�ɶ�
	virtual void OnDestroy() override;
	// �ڽ��� �ʵ带 �����Ҷ� (Player ����)
	virtual void OnChangeField() override;
	// �ڽ��� �����ʵ忡�� �����̵� ������ (Player ����)
	virtual void OnWarpSameField() override;

	// ��밡 �ڽ��� ����Ʈ���̺��� �߰��Ͽ�����
	virtual void OnAddHateTarget( GEntityActor* pEnemy ) override;
	// ��밡 �ڽ��� ����Ʈ���̺��� �����Ͽ�����
	virtual void OnLostHateTarget( MUID uidEnemy ) override;
	// �ڽ��� ����Ʈ���̺� Ÿ���� �߰��Ǿ����� (NPC ����)
	virtual void OnAddHateEntry( GEntityActor* pEnemy ) override;
	// �ڽ��� ����Ʈ���̺� �߰������� ��Ƽ��� Ÿ���� �߰� �Ǿ����� (NPC ����)
	virtual void OnAddHateEntryMember(GEntityActor* pEnemyMember) override;
	// �ڽ��� ����Ʈ���̺� Ÿ���� ���ŵǾ����� (NPC ����)
	virtual void OnLostHateEntry(MUID nEnemyUID) override;
	// �ڽ��� ����Ʈ���̺��� �������
	virtual void OnEmptyHateTable() override;

	// ���� �غ� (Player ����)
	virtual void OnDuelReady( const vector<MUID>& pEnemy ) override;
	// ���� ���� (Player ����)
	virtual void OnDuelStart( const vector<MUID>& pEnemy ) override;
	// ���� ���� (Player ����)
	virtual void OnDuelFinish( const vector<MUID>& pEnemy ) override;


	// �ǰ� �̺�Ʈ
	virtual void OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// �ǰ� ���� �̺�Ʈ (�ŷ�Ʈ ����� �� 1ȸ ȣ��)
	virtual void OnHitSuccess( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) override;
	// ��� �ǰ� ���� �̺�Ʈ (��Ʈĸ������ �뵵)
	virtual void OnHitAll( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) override;
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo)  override;
	// ġ��Ÿ �ǰ� �̺�Ʈ
	virtual void OnCriHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// ���� ġ��Ÿ�� ���������� �̺�Ʈ
	virtual void OnCriHitEnemy( GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo ) override;
	// ������ �̺�Ʈ
	virtual void OnMiss(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// ���� ������ ȸ�������� �̺�Ʈ
	virtual void OnMissEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo)  override;
	// ��� �̺�Ʈ (��� ���)
	virtual void OnGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// ���� ��� �̺�Ʈ
	virtual void OnPerfectGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// �κ� ��� �̺�Ʈ
	virtual void OnPartialGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// ���� ��� �̺�Ʈ
	virtual void OnAbsoluteGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// ���� ������ �������� �̺�Ʈ
	virtual void OnGuardEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo)  override;
	// �������� ����� �� �̺�Ʈ
	virtual void OnDamage(GEntityActor* pOwner, MUID uidAttacker, int nDamage, EFFECT_SOURCE_TYPE nEffectSourceType, int nEffectSourceID) override;
	// ���� �׿��� �� �̺�Ʈ
	virtual void OnKilled(GEntityActor* pTarget) override;
	// �������� �ڽ��� �ǰݵ��� ��
	virtual void OnMagicHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;	
	// �������� �ڽ��� �������� ��
	virtual void OnMagicHitEnemy(GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;	
	// ���ٰ������� �ڽ��� �ǰݵ��� ��
	virtual void OnMeleeHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;	
	// ���ٰ������� �ڽ��� �������� ��
	virtual void OnMeleeHitEnemy(GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;	


	// ������ ������� �̺�Ʈ
	virtual void OnGainBuff(int nBuffID) override;
	// ������ �Ҿ����� �̺�Ʈ
	virtual void OnLostBuff(int nBuffID) override;
	// ������ �ǰݵ����� �̺�Ʈ
	virtual void OnBuffHit(MUID uidUser, GBuffInfo* pBuffInfo) override;

	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnActTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// ���� �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnMagicActTalent( GTalentInfo* pTalentInfo ) override;
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) override;
	// �ŷ�Ʈ�� ��ȿ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmuned(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;
	// ���� �ŷ�Ʈ�� ��ȿȭ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmunedEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;
	// �ŷ�Ʈ�� ȸ�ǽ������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentAvoid(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;
	// ���� �ŷ�Ʈ�� ȸ�ǽ������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentAvoidEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;


	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTogglePrecision() override;
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnToggleSneak(int nSneakBuffType) override;
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnToggleEnlighten() override;
	// ������ ����ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnApplyAdvent() override;

	// ���ͷ��� ������ �̺�Ʈ (Player ����)
	virtual void OnInteraction() override;

	// BuffEntity�� ���� ���� ��
	virtual void OnCreateBuffEntity(int nBuffID, MUID uidEntity) override;
	// BuffEntity�� �Ҹ� ���� ��
	virtual void OnDestroyBuffEntity(int nBuffID, MUID uidEntity) override;


	// NPC�� �׿��� ������ ������ �̺�Ʈ (Player ����)
	virtual void OnRewarded(int nNPCID) override;




	// ������ ��ȭ ���� (Player ����)
	virtual void OnItemEnchantSuccess(GItem* pItem, int nEnchantItemID) override;
	// ������ ���� (Player ����)
	virtual void OnItemEquipped(GItem* pItem) override;
	// ������ �������� (Player ����)
	virtual void OnItemUnequipped(GItem* pItem) override;
	// ���� ���� (Player ����)
	virtual void OnSwitchingWeaponSet(SH_ITEM_SWITCH_WEAPON val) override;

	// ���� �Ϸ� �Ǿ��� �� (NPC ����)
	virtual void OnJobFinished(const GJob* pJob) override;

	// �̵�, ���ݵ� �������� �ൿ�� ���� �� (Player ����)
	virtual void OnDoSomething() override;
private:
	// ������ ����
	set<GActorObserver*>	m_setActorObservers;
};
