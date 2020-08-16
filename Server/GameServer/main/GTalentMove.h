#pragma once

#include "GTalentObserver.h"

class GTalent;
class GEntityActor;
class GTalentInfo;
class GField;

//////////////////////////////////////////////////////////////////////////
//
//	GTalentMove
//
//////////////////////////////////////////////////////////////////////////

class GTalentMove : public GTalentObserver
{
public:
	struct MOVEINFO
	{
		vec3 vPos;
		vec3 vDir;
	};

	GTalentMove();

	// ������ �̺�Ʈ ------------------------------------
	//====================================================
	virtual void	OnStart(GTalent* pTalent)				override;
	// �ŷ�Ʈ�� ����ɶ� ȣ�� (Finish or Canceled)
	virtual void	OnExit(GTalent* pTalent)				override	{}
	virtual void	OnEnterPhasePrepare(GTalent* pTalent)	override;
	virtual void	OnEnterPhaseAct(GTalent* pTalent)		override;
	virtual void	OnEnterPhaseCanceled(GTalent* pTalent)	override	{}
	virtual void	OnEnterPhaseFinish(GTalent* pTalent)	override	{}
	virtual void	OnEnterPhaseExtra(GTalent* pTalent)		override	{}
	virtual void	OnEnterPhaseExtra2(GTalent* pTalent)		override	{}
	virtual void	OnLeavePhaseAct(GTalent* pTalent)		override	{}
	virtual void	OnLeavePhaseCanceled(GTalent* pTalent)	override;
	virtual void	OnLeavePhasePrepare(GTalent* pTalent)	override	{}
	virtual void	OnLeavePhaseFinish(GTalent* pTalent)	override;
	virtual void	OnLeavePhaseExtra(GTalent* pTalent)		override;
	virtual void	OnLeavePhaseExtra2(GTalent* pTalent)		override;
	virtual void	OnUpdatePhasePrepare(GTalent* pTalent, float fDelta) override {}
	virtual void	OnUpdatePhasePreAct(GTalent* pTalent, float fDelta) override;
	virtual void	OnUpdatePhaseAct(GTalent* pTalent, float fDelta) override ;
	virtual void	OnUpdatePhaseExtra(GTalent* pTalent, float fDelta) override {}
	virtual void	OnUpdatePhaseExtra2(GTalent* pTalent, float fDelta) {}
	virtual void	OnEnterPhaseExtra3(GTalent* pTalent)	override	{}
	virtual void	OnLeavePhaseExtra3(GTalent* pTalent)	override;
	virtual void	OnUpdatePhaseExtra3(GTalent* pTalent, float fDelta) override 	{}

	virtual void	OnDummyCall(GTalent* pTalent, float fDelta) override {}

	// �̵� �ŷ�Ʈ ��� ������
	void Finish( float fElapsedTime );
	// �̵� �ŷ�Ʈ�� �̵��� ����
	void Update( float fElapsedTime);
	/// ����Ǵ� ��ġ��
	GTalentMove::MOVEINFO GetExpectedMoveInfo(float fActElapsedTime);
	
private:
	// �̵��ŷ�Ʈ�� ��ȿ���� ����, ��ȿ�ϴٸ� true�� ��ȯ
	bool IsValid();
	// �̵��ؾߵ� �����ǥ�� �����ص�
	void BuildMoveInfo();
	// �ŷ�Ʈ ����ð��� �´� �̵����� �ε����� ��ȯ
	int GetMoveIndex( float fElapsedTime);
	// �ε����� �´� �̵����� ����ü�� ��ȯ
	MOVEINFO GetMoveInfo(int nIndex);
	
private:
	vector<MOVEINFO>	m_vecMoveInfo;
	int					m_nLastMoveIndex;
	bool				m_bBuiltOK;
	vec3				m_vInitPos;
	vec3				m_vInitDir;
	GEntityActor*		m_pActor;
	GField*				m_pField;
	GTalentInfo*		m_pMoveTalentInfo;
};