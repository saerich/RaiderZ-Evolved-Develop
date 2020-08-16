#pragma once

#include "GTalentObserver.h"

class GEntityActor;
class GTalent;

//////////////////////////////////////////////////////////////////////////
//
//	GTalentStress
//
//////////////////////////////////////////////////////////////////////////

class GTalentStress : public GTalentObserver
{
public:
	GTalentStress(bool bGainStress);

	// ������ �̺�Ʈ ------------------------------------
	//====================================================
	virtual void	OnStart(GTalent* pTalent)				override	{}
	// �ŷ�Ʈ�� ����ɶ� ȣ�� (Finish or Canceled)
	virtual void	OnExit(GTalent* pTalent)				override	{}
	virtual void	OnEnterPhasePrepare(GTalent* pTalent)	override	{}
	virtual void	OnEnterPhaseAct(GTalent* pTalent)		override	{}
	virtual void	OnEnterPhaseCanceled(GTalent* pTalent)	override;
	virtual void	OnEnterPhaseFinish(GTalent* pTalent)	override	{}
	virtual void	OnEnterPhaseExtra(GTalent* pTalent)		override	{}
	virtual void	OnEnterPhaseExtra2(GTalent* pTalent)		override	{}
	virtual void	OnLeavePhaseAct(GTalent* pTalent)		override	{}
	virtual void	OnLeavePhaseCanceled(GTalent* pTalent)	override	{}
	virtual void	OnLeavePhasePrepare(GTalent* pTalent)	override	{}
	virtual void	OnLeavePhaseFinish(GTalent* pTalent)	override;
	virtual void	OnLeavePhaseExtra(GTalent* pTalent)		override	{}
	virtual void	OnLeavePhaseExtra2(GTalent* pTalent)		override	{}
	virtual void	OnUpdatePhasePrepare(GTalent* pTalent, float fDelta) override {}
	virtual void	OnUpdatePhasePreAct(GTalent* pTalent, float fDelta) override {}
	virtual void	OnUpdatePhaseAct(GTalent* pTalent, float fDelta) override 	{}
	virtual void	OnUpdatePhaseExtra(GTalent* pTalent, float fDelta) override 	{}
	virtual void	OnUpdatePhaseExtra2(GTalent* pTalent, float fDelta) override 	{}
	virtual void	OnEnterPhaseExtra3(GTalent* pTalent)	override	{}
	virtual void	OnLeavePhaseExtra3(GTalent* pTalent)	override	{}
	virtual void	OnUpdatePhaseExtra3(GTalent* pTalent, float fDelta) override 	{}

	virtual void	OnDummyCall(GTalent* pTalent, float fDelta) override {}
private:
	// ȸ�Ƿ� ���� ��Ʈ���� ���
	void GainAvoidStress(GEntityActor* pOwner);
	// �ŷ�Ʈ ����� ���ͷ�Ʈ�� ���� ��Ʈ���� ���
	void GainInterruptStress(GEntityActor* pOwner);
	// �ŷ�Ʈ�� ȸ���ߴ��� ����
	bool IsAvoidThisTalent(GTalent* pTalent);
	// �ŷ�Ʈ�� �¾Ҵ��� ����
	bool IsHitThisTalent(GTalent* pTalent);

private:
	bool			m_bGainStress; 
};
