#pragma once
#include "XModuleActorControl.h"
#include "XCutScenePlayerActionState.h"

class XCutScenePlayerAligner;

class XModuleCutSceneControl : public XModuleActorControl, public MMemPool<XModuleCutSceneControl>
{
	friend class XCutScenePlayerActionTalent;

protected:

	//////////////////////////////////////////////////////////////////////////
	// Module �ʱ�ȭ
	//
	virtual void				OnInitialized();

	//////////////////////////////////////////////////////////////////////////
	// FSM
	//
	XCutScenePlayerActionFSM*			m_pActionFSM;
	void					InitStates();
	void					DoAction(CUTSCENEPLAYER_ACTION_STATE nState, void* pParam=NULL);			///< �׼� ����

	XCutScenePlayerAligner*				m_pAligner;

public:

	//////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ� / Owner
	//
	XModuleCutSceneControl(XObject* pOwner=NULL);
	virtual ~XModuleCutSceneControl(void);

	XCutScenePlayer*					GetOwner();

public:
	virtual void	OnUseTalent(int nTalentID, vec3& vPos, vec3& vDir);
	XCutScenePlayerAligner*				GetAligner()	{ return m_pAligner; }

protected:
	bool UsableTalent(int nTalentID);
};