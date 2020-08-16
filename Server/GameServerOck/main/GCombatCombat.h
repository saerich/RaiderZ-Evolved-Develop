#ifndef _GCOMBAT_COMBAT_H
#define _GCOMBAT_COMBAT_H

#include "GCombatState.h"

///////////////////////////////////////////////////////////////////////////////////////////
// GCombatCombat //////////

class GNPCInfo;

/// ���� ������ ó���ϴ� ���´��� Ŭ����
/// - colt�� ������� �˾Ƴ� ��Ȳ�� �׼ǵ��� �ٰŷ� ���� �ൿ�� ���Ѵ�.
class GCombatCombat : public GCombatState, public MTestMemPool<GCombatCombat>
{
private:
	MRegulator				m_ChaseStressRegulator;
	GNPCInfo*				m_pNPCInfo;
	MUID					m_uidOldTarget;
	bool					m_bAlreadyCallAggro;

private:
	void					_UpdateCombat();
	void					_UpdateChase(float fDelta);
	void					_UpdateThinkDelay(float fDelta);
	void					OnAggro();
	bool					IsEndCombat();
	bool					_IsRunnginTalentJob();
	void					_EndCombat();
	void					_EndCombatForced();
	void					OnTargetChanged(MUID uidOld, MUID uidNew);
	void					TargetNewEnemy();
	bool					CheckCombatRadius();
public:
	GCombatCombat(GModuleAI* pOwner, GStateMachine<GModuleAI>* pFSM);
	virtual ~GCombatCombat();	

	virtual void			OnRegisterTrainstionRule();
	virtual void			Enter();
	virtual void			Update(float fDelta);
	virtual void			Exit(int nNextStateID);
	virtual GMessageResult	Message(GMessage& msg);
	int						GetID() { return AI_COMBAT_COMBAT; }
	virtual const wchar_t*		DebugString() { return L"combat"; }
};



#endif // _GCOMBAT_COMBAT_H