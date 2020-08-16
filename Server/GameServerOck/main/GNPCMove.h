#pragma once

#include "CTransData.h"

class GEntityNPC;

class GNPCMove : public MTestMemPool<GNPCMove>
{
public:
	GNPCMove(GEntityNPC* pOwner);
	~GNPCMove(void);
	
	void MakeTD(TD_UPDATE_CACHE_NPC& out) const;

	float GetSpeed() const;
	float GetRunSpeed() const;
	float GetFleeSpeed() const;

	void SetAlwaysRun(bool bRun);
	void SetRetreat(bool var);
	void SetReturnToMove(bool var);
	bool IsRun() const;
	bool IsRetreat() const { return m_bNowRetreat; }
	bool IsReturnToMove() const { return m_bNowReturnToHome; }
	
	void RouteMove(vec3& tarPos);
	void RouteMoveStop();
	
private:
	GEntityNPC*				m_pOwner;
	// ������ �̵��� �޸��� ����
	bool					m_bAlwaysRun;
	// ����ġ�� ��
	bool					m_bNowRetreat;
	// ������ ���ư��� ��
	bool					m_bNowReturnToHome;
	
};
