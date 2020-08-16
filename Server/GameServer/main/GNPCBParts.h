#pragma once

#include "CTransData.h"
#include "GActorObserver.h"

class GEntityNPC;
struct BPART_REWARD_INFO;

class GNPCBParts : public GActorObserver, public MTestMemPool<GNPCBParts>
{
public:
	GNPCBParts(GEntityNPC* pOwner);
	~GNPCBParts(void);

	// ���� �μ���
	void Break(uint8 nPartID, MUID uidOpponent);

	// �μ��� ���� ����
	void Recovery();
	// ������Ʈ ó��
	void Update(float fDelta);

	// Ư�� ������ �μ����°�?
	bool IsBroken(uint8 nPartID) const;
	// ���� �μ��� ����
	void SetBreak(uint8 nPartID);
	// ������ �μ����� �ִ°�?
	bool IsAnyPartBroken() const;

	void MakeTD(TD_UPDATE_CACHE_NPC& out) const;
private:
	void RouteBreak( MUID uidOpponent, uint8 nPartID, const vector<TD_USABLE_BPART>& vecUsableBParts );
	void RouteRecovery();
	vector<BPART_REWARD_INFO> GetRewardInfo( uint8 nPartID );
	void OnReward(const vector<BPART_REWARD_INFO>& vecRewardInfo, MUID uidRewarder, vector<TD_USABLE_BPART>& outUsableBParts, vector<GEntityNPC*>& outBPartNPCs);
	void RouteInsertLoobatleNPC( const vector<GEntityNPC*>& outBPartNPCs );
private:
	// �ڽ��� �׾�����
	virtual void OnDie() override;

private:
	GEntityNPC* m_pOwner;
	set<CHAR>	m_setBrokenParts;
};
