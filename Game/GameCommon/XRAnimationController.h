#pragma once

#include "RSceneNode.h"
#include "RAnimationController.h"
#include "MTstring.h"

using namespace rs3;

class XRAnimationController : public RAnimationController
{
public:
	XRAnimationController(RSNCTRL_TYPE eType = RSNCTRL_POSTANIMATION, RSNCTRL_TYPE eFrameControllerType = RSNCTRL_UPDATE);
	virtual ~XRAnimationController();

	void			OnPreUpdate();
	void			OnUpdate();

	void			DisableAniamtionEventPreFrame(unsigned int nFrame);

	bool			PushActiveNode(tstring& strNodeName);

	bool			IsActiveNode(tstring& strNodeName);
	bool			IsAnimationLoop();

	// �Ժη� ���� ������(�����϶���..)
	// �ܺο��� ���� XRAnimationController�� ���� ���ð�... XTransformController Ŭ������ ��ӹ޴� Ŭ�����鸸 ������ �սô�
	// ���� XBaseAnimationTC::GetAnimatedActorNodeTransform() �Լ��� ���� �ٶ��ϴ�.
	void			GetLocalTransformAt(DWORD nFrame, RMatrix& outMatrix, RActorNode* pActorNode);	///< Ư�� �������� local tm �� ��´�
	void			GetWorldTransformAt(DWORD nFrame, RMatrix& outMatrix, RActorNode* pActorNode);	///< Ư�� �������� world tm �� ��´�
	void			GetResultTransformAt(DWORD nFrame, RMatrix& outMatrix, RActorNode* pActorNode);	///< Ư�� �������� world tm �� ��´�


	void			ResetAniamtionSpeed();
	float			GetOrgAniamtionSpeed();

	RAnimationControllerNode* GetControllerNode(tstring& strNodeName);
};