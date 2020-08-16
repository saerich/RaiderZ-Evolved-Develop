#pragma once

#include "XCharacter.h"
#include "RActorController.h"
#include "XRAnimationController.h"
#include "RAnimationResource.h"

// ��Ʈ�� ������Ʈ �����̴�. �� ��Ű��.
enum TRANSFORM_CONTROLLER_LIST
{
	TCL_NONE,
	TCL_BASE,
	TCL_PHYSX,
	TCL_UPPER,
	TCL_FACE,
	TCL_FAKE_BEATEN,

	TCL_OVERLAY,


	TCL_MAX
};

enum ANIMATION_CHECK_STATE
{
	AS_NONE,			// ����.
	AS_MODEL_LOADING,	// �� �ε���
	AS_LOADING,			// �ִϸ��̼� �ε���
	AS_LOADED			// �ִϸ��̼� �ε��Ϸ�
};

class XTransformController
{
protected:
	XCharacter*							m_pActor;
	bool								m_bEnable;
	
public:
	XTransformController(XCharacter* pCharacter);
	virtual ~XTransformController();

	void								Destroy();

	virtual void						PreUpdate(float fDelta) = 0;
	virtual void						Update(float fDelta);
	virtual void						PostUpdate(float fDelta) {}

	virtual void						GetActiveRActorNodeList(vector<tstring>& vecActorNodeList) {}
	XCharacter*							GetActor()				{ return m_pActor; }

	ANIMATION_CHECK_STATE				CheckAnimationLoadingComplete(tstring& strAniName);
	ANIMATION_CHECK_STATE				CheckAnimationUsable(tstring& strAniName);

	void								SetEnable(bool bEnable)	{ m_bEnable = bEnable; }	
	bool								IsEnabled()				{ return m_bEnable; }

	static TRANSFORM_CONTROLLER_LIST	GetID() { return TCL_NONE; }
};