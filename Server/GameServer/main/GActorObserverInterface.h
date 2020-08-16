#pragma once

class GEntityActor;
class GTalentInfo;
class GBuffInfo;
class GItem;
class GJob;

class GActorObserverInterface
{
public:
	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) {}
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) {}
	// �ڽ��� �׾�����
	virtual void OnDie() {}
	// �ڽ��� ���� �Ǿ����� (NPC ����)
	virtual void OnDespawn() {}
	// �ڽ��� ��ü�� �Ҹ�ɶ�
	virtual void OnDestroy() {}
	// �ڽ��� �ʵ带 �����Ҷ� (Player ����)
	virtual void OnChangeField() {}
	// �ڽ��� �����ʵ忡�� �����̵� ������ (Player ����)
	virtual void OnWarpSameField() {}

	// ��밡 �ڽ��� ����Ʈ���̺��� �߰��Ͽ�����
	virtual void OnAddHateTarget( GEntityActor* pEnemy ) {}
	// ��밡 �ڽ��� ����Ʈ���̺��� �����Ͽ�����
	virtual void OnLostHateTarget( MUID uidEnemy ) {}
	// �ڽ��� ����Ʈ���̺� Ÿ���� �߰��Ǿ����� (NPC ����)
	virtual void OnAddHateEntry( GEntityActor* pEnemy ) {}
	// �ڽ��� ����Ʈ���̺� �߰������� ��Ƽ��� Ÿ���� �߰� �Ǿ����� (NPC ����)
	virtual void OnAddHateEntryMember(GEntityActor* pEnemyMember) {}
	// �ڽ��� ����Ʈ���̺� Ÿ���� ���ŵǾ����� (NPC ����)
	virtual void OnLostHateEntry(MUID nEnemyUID ) {}
	// �ڽ��� ����Ʈ���̺��� �������
	virtual void OnEmptyHateTable() {}

	// ���� �غ� (Player ����)
	virtual void OnDuelReady( const vector<MUID>& pEnemy ) {}
	// ���� ���� (Player ����)
	virtual void OnDuelStart( const vector<MUID>& pEnemy ) {}
	// ���� ���� (Player ����)
	virtual void OnDuelFinish( const vector<MUID>& pEnemy ) {}


	// �ǰ� �̺�Ʈ
	virtual void OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) {}
	// �ǰ� ���� �̺�Ʈ (�ŷ�Ʈ ����� �� 1ȸ ȣ��)
	virtual void OnHitSuccess( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) {}
	// ��� �ǰ� ���� �̺�Ʈ (��Ʈĸ������ �뵵)
	virtual void OnHitAll( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) {}
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo)  {}
	// ġ��Ÿ �ǰ� �̺�Ʈ
	virtual void OnCriHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) {}
	// ���� ġ��Ÿ�� ���������� �̺�Ʈ
	virtual void OnCriHitEnemy( GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo ) {}
	// ������ �̺�Ʈ
	virtual void OnMiss(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  {}
	// ���� ������ ���������� �̺�Ʈ
	virtual void OnMissEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo)  {}
	// ��� �̺�Ʈ (��� ���)
	virtual void OnGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  {}
	// ���� ��� �̺�Ʈ
	virtual void OnPerfectGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  {}
	// �κ� ��� �̺�Ʈ
	virtual void OnPartialGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  {}
	// ���� ��� �̺�Ʈ
	virtual void OnAbsoluteGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  {}
	// ���� ������ �������� �̺�Ʈ
	virtual void OnGuardEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo)  {}
	// �������� ����� �� �̺�Ʈ
	virtual void OnDamage(GEntityActor* pOwner, MUID uidAttacker, int nDamage, EFFECT_SOURCE_TYPE nEffectSourceType, int nEffectSourceID) {}
	// ���� �׿��� �� �̺�Ʈ
	virtual void OnKilled(GEntityActor* pTarget) {}
	// �������� �ڽ��� �ǰݵ��� ��
	virtual void OnMagicHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) {}
	// �������� �ڽ��� �������� ��
	virtual void OnMagicHitEnemy(GEntityActor* pTarget, GTalentInfo* pTalentInfo) {}
	// ���ٰ������� �ڽ��� �ǰݵ��� ��
	virtual void OnMeleeHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) {}
	// ���ٰ������� �ڽ��� �������� ��
	virtual void OnMeleeHitEnemy(GEntityActor* pTarget, GTalentInfo* pTalentInfo) {}


	// ������ ������� �̺�Ʈ
	virtual void OnGainBuff(int nBuffID) {}
	// ������ �Ҿ����� �̺�Ʈ
	virtual void OnLostBuff(int nBuffID) {}
	// ������ �ǰݵ����� �̺�Ʈ
	virtual void OnBuffHit(MUID uidUser, GBuffInfo* pBuffInfo) {}

	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) {}
	// �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnActTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) {}
	// ���� �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnMagicActTalent( GTalentInfo* pTalentInfo ) {}
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) {}
	// �ŷ�Ʈ�� ��ȿ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmuned(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) {}
	// ���� �ŷ�Ʈ�� ��ȿȭ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmunedEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo) {}
	// �ŷ�Ʈ�� ȸ�ǽ������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentAvoid(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) {}
	// ���� �ŷ�Ʈ�� ȸ�ǽ������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentAvoidEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo) {}


	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTogglePrecision() {}
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnToggleSneak(int nSneakBuffType) {}
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnToggleEnlighten() {}
	// ������ ����ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnApplyAdvent() {}

	// ���ͷ��� ������ �̺�Ʈ (Player ����)
	virtual void OnInteraction() {}

	// BuffEntity�� ���� ���� ��
	virtual void OnCreateBuffEntity(int nBuffID, MUID uidEntity) {}
	// BuffEntity�� �Ҹ� ���� ��
	virtual void OnDestroyBuffEntity(int nBuffID, MUID uidEntity) {}	

	// NPC�� �׿��� ������ ������ �̺�Ʈ (Player ����)
	virtual void OnRewarded(int nNPCID) {}


	// ������ ��ȭ ���� (Player ����)
	virtual void OnItemEnchantSuccess(GItem* pItem, int nBuffID) {}
	// ������ ���� (Player ����)
	virtual void OnItemEquipped(GItem* pItem) {}
	// ������ �������� (Player ����)
	virtual void OnItemUnequipped(GItem* pItem) {}
	// ���� ���� (Player ����)
	virtual void OnSwitchingWeaponSet(SH_ITEM_SWITCH_WEAPON val) {}

	// ���� �Ϸ� �Ǿ��� �� (NPC ����)
	virtual void OnJobFinished(const GJob* pJob) {}

	// �̵�, ���ݵ� �������� �ൿ�� ���� �� (Player ����)
	virtual void OnDoSomething() {}
};

