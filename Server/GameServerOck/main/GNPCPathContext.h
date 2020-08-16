#pragma once

#include "GActorObserver.h"

class GEntityNPC;

class GNPCPathContext : private GActorObserver, public MTestMemPool<GNPCPathContext>
{
public:
	GNPCPathContext(GEntityNPC* pOwner);
	~GNPCPathContext(void);

private:
	// �ڽ��� ����Ʈ���̺� Ÿ���� �߰��Ǿ�����
	virtual void OnAddHateEntry( GEntityActor* pEnemy ) override;
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override;

private:
	GEntityNPC* m_pOwner;
};
