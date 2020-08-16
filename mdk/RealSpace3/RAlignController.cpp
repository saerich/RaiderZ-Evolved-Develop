#include "StdAfx.h"

#include "RAlignController.h"
#include "RActorNode.h"
#include "RActor.h"

namespace rs3 {

RAlignController::RAlignController(void) : RSceneNodeController(RSNCTRL_PRERENDER)
{
	AddAttribute( RCA_STICKY );
}

RAlignController::~RAlignController(void)
{
	m_bEnable = false;
}

void RAlignController::Update(RCameraSceneNode* pCamera /*= NULL*/)
{
	RPFC_THISFUNC;
	RSceneNode *pTarget = m_pSceneNode;
	_ASSERT(pTarget);

	if (pCamera == NULL)
		return;

	// �Ϲ����� ������ó�� ���̰� �����. 2D �簢��ó�� ...
	const RMatrix& matWorld = pTarget->GetWorldTransform();
	RVector wP(matWorld._41,matWorld._42,matWorld._43);

	const RVector &vU = pCamera->GetUp();
	const RVector &vD = pCamera->GetDirection();

	RMatrix matResult;
	matResult.SetLocalMatrix(wP, vD, vU);

	RMatrix matInvParent;
	pTarget->GetParent()->GetWorldTransform().GetInverse(&matInvParent);
	RMatrix matLocal = matResult * matInvParent;

	// ���� up vector �� local�� x ���̶� �̷��� �Ǿ���. -> Z������?
	RVector localDir = RVector(matLocal._21,matLocal._22,matLocal._23);
	RVector localUp = RVector(matLocal._31,matLocal._32,matLocal._33);
	pTarget->SetDirection(localDir,localUp);

	// update children
	RSceneNodeUpdateTransformVisitor updateChildrenVisitor;
	pTarget->Traverse( &updateChildrenVisitor);
}

}