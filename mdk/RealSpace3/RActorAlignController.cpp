#include "StdAfx.h"
#include "RActorAlignController.h"
#include "RCameraSceneNode.h"
#include "RActor.h"

namespace rs3 {

//////////////////////////////////////////////////////////////////////////
// RActorAlignController
// align controller ��, scale �� ���� ó���� �Ǿ����� �ʴ�

RActorAlignController::RActorAlignController(RActor* pActor) : RSceneNodeController(RSNCTRL_PRERENDER), m_pActor(pActor)
{
	AddAttribute( RCA_STICKY );
}

void RActorAlignController::GetResultTransform( RMatrix& outResult, const RMatrix& matWorld, const RCameraSceneNode* pCamera )
{
	// ������ǥ��ȯ = ������ Target�� ���ϰ� up�� (0,0,1)�� ��ȯ

	RVector worldPos(matWorld._41,matWorld._42,matWorld._43);
	RVector cameraDir(pCamera->GetWorldDirection());

	outResult.SetLocalMatrix(worldPos, cameraDir, RVector::AXISZ);
}

void RActorAlignController::Update(RCameraSceneNode* pCamera)
{
	RPFC_THISFUNC;
	// �̶� �̹� �ִϸ��̼��� �ִٸ� UpdateForRender ����Ѵ�.
	_ASSERT( m_pActor->GetAnimation()==NULL || m_pActor->GetUpdatedFrame()==REngine::GetDevice().GetFrameCount() );

	if( pCamera )
	{
		const RMatrix& matWorld = m_pSceneNode->GetWorldTransform();
		RVector scale;
		matWorld.ExtractScale( scale );

		RMatrix matResult;
		GetResultTransform( matResult, matWorld, pCamera );

 		RMatrix matScale;
 		matScale.SetScaleMatrix( scale );

		// Target ActorNode �� local tm �� ��ȭ��Ű�� �ʰ�, world tm �� �������ش�
		SetWorldTransform( matScale * matResult );

		// update children
		RSceneNodeUpdateTransformVisitor updateChildrenVisitor;
		m_pSceneNode->Traverse( &updateChildrenVisitor);
	}
}

//////////////////////////////////////////////////////////////////////////
// RActorZFixedAlignController
RActorZFixedAlignController::RActorZFixedAlignController(RActor* pActor) : RActorAlignController(pActor)
{
	AddAttribute( RCA_STICKY );
}

void RActorZFixedAlignController::GetResultTransform( RMatrix& outResult, const RMatrix& matWorld, const RCameraSceneNode* pCamera )
{
	// ������ǥ��ȯ = z���� ������ ���¿��� y ������ Target�� ���ϴ� ��ȯ
	RVector worldPos(matWorld._41,matWorld._42,matWorld._43);
	RVector cameraDir(pCamera->GetWorldDirection());

	// �⺻���� up (z)�� ���� �� ������ ���Ѵ�
	RVector up = Normalize(RVector(matWorld._31,matWorld._32,matWorld._33));
	RVector right = Normalize(CrossProduct(cameraDir,up));
	RVector newDir = Normalize(CrossProduct(up,right));
		
	outResult.SetLocalMatrix(worldPos, newDir, up);
}

}