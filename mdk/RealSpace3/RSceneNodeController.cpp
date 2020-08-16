#include "StdAfx.h"
#include "RSceneNode.h"
#include "RSceneNodeController.h"
#include "RActorNode.h"

namespace rs3 {

RSceneNodeController::RSceneNodeController(RSNCTRL_TYPE eType)
: m_pSceneNode(NULL)
, m_bEnable(false)
, m_type(eType)
{
}

RSceneNodeController::~RSceneNodeController(void)
{
	if(m_pSceneNode)
		m_pSceneNode->RemoveController(this);
}

void RSceneNodeController::SetAutoUpdate( RSceneNode* pSceneNodeToSet, bool bUseAutoUpdate )
{
	_ASSERT(pSceneNodeToSet != NULL);
	pSceneNodeToSet->SetAutoUpdate(bUseAutoUpdate);
}

bool RSceneNodeController::IsAutoUpdate( RSceneNode* pSceneNode )
{
	_ASSERT(pSceneNode != NULL);
	return pSceneNode->IsAutoUpdate();
}

void RSceneNodeController::SetWorldTransform(const RMatrix& matWorld)
{
	_ASSERT( m_pSceneNode != NULL );

	(*m_pSceneNode->m_matWorld) = matWorld;
	m_pSceneNode->OnUpdateTransform(); // �������� Ʈ������ ������Ʈ �����̶� ��¿�� ���� ȣ���Ѵ�. - charmke
	m_pSceneNode->NeedUpdateBoundingVolume();

	// ���̻� ������Ʈ ���� �ʰ� UpdateID�� �������ش�.
	bool bNeedHierarchicalUpdate = m_pSceneNode->GetParent() && m_pSceneNode->IsNeedHierarchicalUpdate();
	int nParentUpdatedID = 0;
	if( bNeedHierarchicalUpdate )
	{
		nParentUpdatedID = m_pSceneNode->GetParent()->m_TransformUpdateContext.GetUpdatedID();
	}
	m_pSceneNode->m_TransformUpdateContext.SetForceUpdated( bNeedHierarchicalUpdate, nParentUpdatedID );
}

void RSceneNodeController::SetVisibilityResult( float fVisibilityResult )
{
	_ASSERT( m_pSceneNode != NULL );

	m_pSceneNode->m_fVisibilityResult = fVisibilityResult;

	// ���̻� ������Ʈ ���� �ʰ� UpdateID�� �������ش�.
	bool bNeedHierarchicalUpdate = m_pSceneNode->GetParent() && m_pSceneNode->IsNeedHierarchicalUpdate();
	int nParentUpdatedID = 0;
	if( bNeedHierarchicalUpdate )
	{
		nParentUpdatedID = m_pSceneNode->GetParent()->m_VisibilityUpdateContext.GetUpdatedID();
	}
	m_pSceneNode->m_VisibilityUpdateContext.SetForceUpdated( bNeedHierarchicalUpdate, nParentUpdatedID );
}

void RSceneNodeController::SetNoiseRefResult( float fNoiseRefResult )
{
	_ASSERT( m_pSceneNode != NULL );

	m_pSceneNode->m_fNoiseRefResult = fNoiseRefResult;

	// ���̻� ������Ʈ ���� �ʰ� UpdateID�� �������ش�.
	bool bNeedHierarchicalUpdate = m_pSceneNode->GetParent() && m_pSceneNode->IsNeedHierarchicalUpdate();
	int nParentUpdatedID = 0;
	if( bNeedHierarchicalUpdate )
	{
		nParentUpdatedID = m_pSceneNode->GetParent()->m_NoiseRefUpdateContext.GetUpdatedID();
	}
	m_pSceneNode->m_NoiseRefUpdateContext.SetForceUpdated( bNeedHierarchicalUpdate, nParentUpdatedID );
}

}