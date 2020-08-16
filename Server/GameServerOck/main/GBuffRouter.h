#pragma once

#include "GBuffObserver.h"

class GBuff;

class GBuffRouter : public GBuffObserver, public MTestMemPool<GBuffRouter>
{
public:
	GBuffRouter(GBuff* pOwner);
	~GBuffRouter(void);

private:
	void NotifyForParty();
	void RouteGainBuff();
	void RouteLostBuff();
	void RouteStackIncrease();
	void RouteStackDecrease();

private:
	// ���� ������ ȿ��ó��
	virtual void OnGain(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo) override;
	// ���� ������ ȿ��ó��
	virtual void OnLost(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo, bool bRoute) override;
	// ������ �ߺ��Ǽ� �ɸ� ���
	virtual void OnDuplicated(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo) override;
	
private:
	GBuff* m_pOwner;
};
