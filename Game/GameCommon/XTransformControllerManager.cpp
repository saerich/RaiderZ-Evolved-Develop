#include "stdafx.h"
#include "XTransformControllerManager.h"
#include "MLocale.h"

XTransformControllerManager::XTransformControllerManager( XCharacter* pCharacter )
: m_pActor(pCharacter)
, m_bUpdated(false)
{
	if(m_pActor == NULL)
		return;

	m_pActor->Stop();
}

XTransformControllerManager::~XTransformControllerManager()
{
	Destroy();
}

void XTransformControllerManager::Destroy()
{
	for(MAP_TC::iterator it = m_mapTransformController.begin(); it != m_mapTransformController.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}

	m_mapTransformController.clear();
}

void XTransformControllerManager::PreUpdate( float fDelta )
{
	if( !m_pActor->GetUsable() ) return;

	for(MAP_TC::iterator it = m_mapTransformController.begin(); it != m_mapTransformController.end(); ++it)
	{
		if ((it->second)->IsEnabled())
		{
			// Pre ������Ʈ
			(it->second)->PreUpdate(fDelta);
		}
	}
}

void XTransformControllerManager::Update( float fDelta )
{
	if( !m_pActor->GetUsable() ) return;

	//m_pActor->Stop();
	m_pActor->Pause();

	vector<tstring> vecActiveNodeList;

	for(MAP_TC::iterator it = m_mapTransformController.begin(); it != m_mapTransformController.end(); ++it)
	{
		if ((it->second)->IsEnabled())
		{
			// ������Ʈ
			(it->second)->Update(fDelta);

			// ���� ActorNode ����
			(it->second)->GetActiveRActorNodeList(vecActiveNodeList);
		}
	}

	// ����Ʈ�� RActorNode�� ��ȯ
	vector<RActorNode*> vecUpdateActiveNode;
	TransformActorNode(vecActiveNodeList, vecUpdateActiveNode);

	// ���� Transform ������Ʈ
	UpdateActiveNode(vecUpdateActiveNode);

	m_bUpdated = true;
	m_pActor->SetAnimationUpdated(m_bUpdated);
}

void XTransformControllerManager::PostUpdate( float fDelta )
{
	if( !m_pActor->GetUsable() ) return;

	for(MAP_TC::iterator it = m_mapTransformController.begin(); it != m_mapTransformController.end(); ++it)
	{
		if ((it->second)->IsEnabled())
		{
			// Pre ������Ʈ
			(it->second)->PostUpdate(fDelta);
		}
	}
}

XTransformController* XTransformControllerManager::GetTransformController( TRANSFORM_CONTROLLER_LIST nID )
{
	MAP_TC::iterator itFind = m_mapTransformController.find(nID);
	if(itFind != m_mapTransformController.end())
		return itFind->second;

	return NULL;
}

void XTransformControllerManager::ReSetUpdated()
{
	m_bUpdated = false;
	m_pActor->SetAnimationUpdated(m_bUpdated); 
}

void XTransformControllerManager::UpdateActiveNode( vector<RActorNode*> vecUpdate )
{
	vector<RActorNode*>& vecAnimationReferencedNodes = m_pActor->GetAnimationReferencedNodes();
	for(vector<RActorNode*>::iterator itRef = vecAnimationReferencedNodes.begin(); itRef != vecAnimationReferencedNodes.end(); ++itRef)
	{
		(*itRef)->UpdateAllParentDependentValues();
		(*itRef)->UpdateFinalResultTransforms();
	}

	// �� ActorNode ������Ʈ�� ������ �������� �Ѵ�.
	for(vector<RActorNode*>::iterator it = vecUpdate.begin(); it != vecUpdate.end(); ++it)
	{
		(*it)->UpdateAllParentDependentValues();
		(*it)->UpdateFinalResultTransforms();
	}
}

void XTransformControllerManager::TransformActorNode( vector<tstring>& vecList, vector<RActorNode*>& vecOutUpdate )
{
	for(vector<tstring>::iterator it = vecList.begin(); it != vecList.end(); ++it)
	{
		RActorNode* pActorNode = m_pActor->GetActorNode(MLocale::ConvTCHARToAnsi(it->c_str()).c_str());
		if(pActorNode)
			vecOutUpdate.push_back(pActorNode);
	}
}
