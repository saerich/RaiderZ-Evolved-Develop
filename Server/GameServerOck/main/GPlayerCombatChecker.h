#pragma once

#include "GActorObserver.h"

class GPlayerCombatChecker : private GActorObserver, public MTestMemPool<GPlayerCombatChecker>
{
public:
	GPlayerCombatChecker(GEntityActor* pOwner);
	~GPlayerCombatChecker(void);

	void Update(float fDelta);

private:
	// ��밡 �ڽ��� ����Ʈ���̺��� �߰��Ͽ�����
	virtual void OnAddHateTarget( GEntityActor* pEnemy ) override;
	// ��밡 �ڽ��� ����Ʈ���̺��� �����Ͽ�����
	virtual void OnLostHateTarget( MUID uidEnemy ) override;

private:
	GEntityActor*	m_pOwner;
	bool			m_bIsNowCombat;
};
