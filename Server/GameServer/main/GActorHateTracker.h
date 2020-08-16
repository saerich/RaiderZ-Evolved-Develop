#pragma once

#include "GActorObserver.h"

class GActorHateTracker : private GActorObserver, public MTestMemPool<GActorHateTracker>
{
public:
	GActorHateTracker(GEntityActor* pOwner);
	~GActorHateTracker(void);

	// �ʱ�ȭ
	void Clear();

	// ��Ʈ�����̺��� ����ִ��� ����
	bool IsEmpty() const;

	// ��Ʈ�� ����
	size_t GetQty() const;

	// ��Ʈ�����̺� ��ȯ
	void GetHateNPCs(vector<MUID>& vecNPCs) const;
	
	// HP ȸ���� �����Ǵ� ����Ʈ ó��
	void AddHate_RecoverHP(int nRecoveredHP, float fHateFactor);
	// �ŷ�Ʈ ���� �����Ǵ� ����Ʈ ó��
	void AddHate_UseTalent(int nHateAdj);

private:
	// ��밡 �ڽ��� ����Ʈ���̺��� �߰��Ͽ�����
	virtual void OnAddHateTarget( GEntityActor* pEnemy ) override;
	// ��밡 �ڽ��� ����Ʈ���̺��� �����Ͽ�����
	virtual void OnLostHateTarget( MUID uidEnemy ) override;
	// �ڽ��� �׾�����
	virtual void OnDie() override;
	// �ڽ��� �ʵ带 �����Ҷ�
	virtual void OnChangeField() override;
	// �ڽ��� ��ü�� �Ҹ�ɶ�
	virtual void OnDestroy() override;
	// �ڽ��� �����ʵ忡�� �����̵� ������ (Player ����)
	virtual void OnWarpSameField() override;
	
private:
	GEntityActor*	m_pOwner;
	set<MUID>		m_setHateNPCs;	
};
