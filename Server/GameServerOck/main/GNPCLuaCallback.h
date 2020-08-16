#pragma once

#include "GActorObserver.h"

class GEntityNPC;

class GNPCLuaCallback : public GActorObserver, public MTestMemPool<GNPCLuaCallback>
{
public:
	GNPCLuaCallback(GEntityNPC* pOwner);
	~GNPCLuaCallback(void);

private:
	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) override;
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override;
private:
	GEntityNPC* m_pOwner;
};
