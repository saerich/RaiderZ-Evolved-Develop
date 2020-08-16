#include "StdAfx.h"

#include "RActorNodeConstraintController.h"
#include "RActorNode.h"
#include "RActor.h"

namespace rs3 {

RActorNodeConstraintController::RActorNodeConstraintController()
: RActorController(RSNCTRL_PREUPDATE), m_pTargetActorNode( NULL )
, m_bIgnoreActorNodeDirection(false)
, m_bNeedUpdateParentAnimation(true)
{
	m_strControllerName = RCID_ACTORNODECONSTRAINT;
	AddAttribute( RCA_MANAGED );
}

RActorNodeConstraintController::~RActorNodeConstraintController()
{
	_ASSERT( m_pTargetActorNode== NULL );
}

bool RActorNodeConstraintController::CreateFromXML(MXmlElement &element)
{
	MXmlElement *pElement = element.FirstChildElement("TARGET");
	_Contents(m_TargetActorNodeName,pElement);

	return true;	
}

void RActorNodeConstraintController::SaveToXML(MXmlElement &element)
{
	MXmlElement *pElement = new MXmlElement("TARGET");
	_SetContents(pElement, m_TargetActorNodeName );
	element.LinkEndChild(pElement);
}

RSceneNodeController* RActorNodeConstraintController::Clone()
{
	RActorNodeConstraintController* pNewController = new RActorNodeConstraintController;
	pNewController->m_TargetActorNodeName = m_TargetActorNodeName;
	pNewController->m_bIgnoreActorNodeDirection = m_bIgnoreActorNodeDirection;
	pNewController->m_bNeedUpdateParentAnimation = m_bNeedUpdateParentAnimation;

	return pNewController;
}

void RActorNodeConstraintController::Update(RCameraSceneNode* pCamera /*= NULL*/)
{
	RPFC_THISFUNC;
	if(!m_pTargetActorNode) return;

	// update & update for render
	static RSceneNodeUpdateVisitor v;
	if(v.OnPreOrderVisit(m_pActor))
	{
		v.OnPostOrderVisit(m_pActor);
	}
	UpdateActorForRender();

	// ����Ʈ ����尡 �Ҹ������� UpdateForRender�� �ҷ��־�� �Ѵ�.
	_ASSERT(m_pTargetActorNode->m_pActor->GetUpdatedFrame() == REngine::GetDevice().GetFrameCount());
	_ASSERT(m_pTargetActorNode->m_pActor->CheckAlreadyUpdatedForRender());
	if(m_bNeedUpdateParentAnimation)
	{
		m_pTargetActorNode->ForceUpdateAnimation();
	}

	// �ִϸ��̼��� ���� ��쿡�� ���� ������Ʈ�� �Ǿ�� �Ѵ�.
	m_pTargetActorNode->UpdateAllParentDependentValues();
	m_pTargetActorNode->UpdateFinalResultTransforms();

	ForceUpdateWorldTransform();
	ForceUpdateVisibilityResult();
	ForceUpdateNoiseRefResult();
}

void RActorNodeConstraintController::OnAdd()
{
	// actor �� �ڽ��̾�� �Ѵ�  <- �����... 090605, COMMENT OZ
	if( !MIsDerivedFromClass(RActor, m_pSceneNode->GetParent()) )
	{
		_ASSERT(FALSE);
		return;
	}

	// TODO: actornode�� visibility, noiseref ���� ���濡 ���� ������ ����Ʈ ���� �����ϴ� ó���� �ϱ� ������ actornode�� �ȵȴ�.
	if( MIsDerivedFromClass(RActorNode, m_pSceneNode) )
	{
		_ASSERT(FALSE);
		return;
	}

	m_pActor = (RActor*)m_pSceneNode->GetParent();
	RActorNode* pActorNode = m_pActor->GetActorNode( m_TargetActorNodeName.c_str() );

	if(pActorNode)
	{
		m_pTargetActorNode = pActorNode;
		m_pTargetActorNode->m_pActor->AddUpdateForRenderRefCount();
		m_pTargetActorNode->AddRelatedController( this );

		if( !IsAutoUpdate(m_pActor) && m_pActor->m_bSceneActor )
			Update(NULL);
	}
	else
	{
		Enable(false);
	}
}

void RActorNodeConstraintController::OnRemove()
{
	if(m_pTargetActorNode)
	{
		m_pTargetActorNode->m_pActor->ReleaseUpdateForRenderRefCount();
		m_pTargetActorNode->RemoveRelatedController( this );
		m_pTargetActorNode = NULL;
	}

	m_pActor = NULL;
}

void RActorNodeConstraintController::OnEnable()
{
	if(!m_pTargetActorNode)
	{
		OnAdd();
	}
}

void RActorNodeConstraintController::Invalidate()
{
	m_pTargetActorNode->m_pActor->ReleaseUpdateForRenderRefCount();
	m_pTargetActorNode = NULL;
}

RActorNodeConstraintController* GetFirstActorNodeConstraintController( RSceneNode* pSceneNode )
{
	RSCENENODECONTROLLERLIST* pControllers = pSceneNode->GetControllers( RSNCTRL_PREUPDATE );

	for( RSCENENODECONTROLLERLIST::iterator it = pControllers->begin(); it != pControllers->end(); ++it )
	{
		RSceneNodeController* pController = *it;
		RActorNodeConstraintController* pActorNodeConstraintController = dynamic_cast<RActorNodeConstraintController*> (pController);
		if(pActorNodeConstraintController)
			return pActorNodeConstraintController;
	}
	return NULL;
}

void RActorNodeConstraintController::ForceUpdateWorldTransform()
{
	_ASSERT( m_pSceneNode );
	_ASSERT( m_pTargetActorNode );

	// this �� ������ �ִ� ����� world tm �� ������Ʈ,
	RMatrix matLocal;
	// ���� ����� ������ ����
	RMatrix matLocalScale;
	matLocalScale.SetScaleMatrix( m_pSceneNode->GetScale() );
	// matLocal = matLocalScale * m_pSceneNode->GetLocalTransform();
	matLocalScale.MultiplyTo(m_pSceneNode->GetLocalTransform(), matLocal);

	RMatrix matWorldAply;
	if(m_bIgnoreActorNodeDirection)
	{
		// actornode�� ������ �����Ѵ�, �� actor �� ���󰣴�
		const static RVector vX(0, 1, 0);
		const static RVector vUp(0, 0, 1);

		RMatrix matScale;
		matScale.SetScaleMatrix( m_pTargetActorNode->GetScale() );

		// actor ��ǥ�迡���� ��ġ�� ���Ѵ�
		RVector vecModelPosition = m_pTargetActorNode->GetWorldPosition() * m_pTargetActorNode->m_pActor->GetWorldInverseTransform();

		RMatrix matTranslation;
		matTranslation.SetLocalMatrix( vecModelPosition, vX, vUp);

		RMatrix matActorWorld = m_pTargetActorNode->m_pActor->GetWorldTransform();

		matWorldAply = matLocal * matScale * matTranslation * matActorWorld;
	}
	else
	{
		matWorldAply = matLocal * m_pTargetActorNode->GetWorldTransform();
	}
	SetWorldTransform( matWorldAply );
}

void RActorNodeConstraintController::ForceUpdateVisibilityResult()
{
	_ASSERT( m_pSceneNode );
	_ASSERT( m_pTargetActorNode );

	float fVisibilityParent = m_pTargetActorNode->GetVisibilityResult();
	float fVisibilityResult = fVisibilityParent * m_pSceneNode->GetVisibility();

	SetVisibilityResult( fVisibilityResult );
}

void RActorNodeConstraintController::ForceUpdateNoiseRefResult()
{
	_ASSERT( m_pSceneNode );
	_ASSERT( m_pTargetActorNode );

	float fNoiseRefParent = m_pTargetActorNode->GetNoiseRefResult();
	float fNoiseRefResult = fNoiseRefParent * m_pSceneNode->GetNoiseRef();

	SetNoiseRefResult( fNoiseRefResult );
}

}
