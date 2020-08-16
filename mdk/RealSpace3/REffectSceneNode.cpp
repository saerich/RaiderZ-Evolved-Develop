#include "StdAfx.h"
#include <list>
#include "REffectSceneNode.h"
#include "RMeshMgr.h"
#include "REffectActor.h"
#include "REmitter.h"
#include "RTrail.h"
#include "RProfileInfo.h"
#include "RPointLightSceneNode.h"
#include "RActorNodeConstraintController.h"

namespace rs3 {

MImplementRTTI(REffectSceneNode, REffectBase);

RVector REffectSceneNode::m_vGravity	= RVector(0, 0, -9.8f);
RVector REffectSceneNode::m_vWindForce	= RVector(0, 0, 0);

REffectSceneNode::REffectSceneNode(void) 
: m_bLastFramePause(false)
, m_bDeleteNow(false)
, m_bOncePlayed(false)
, m_fElapsedTime(0)
{
	m_EffectSceneNodePlayControlVisitor.m_pRequestEffectSceneNode = this;
	SetAutoUpdate(true);
	m_bCullingNode = false;
}

REffectSceneNode::~REffectSceneNode(void)
{
}

int REffectSceneNode::GetCurrentCount()
{
	return static_cast<int>(m_children.size());
}

void REffectSceneNode::OnRemove()
{
	RemoveController(&m_EffectAlignController);
}

void REffectSceneNode::SetAlignTarget( RSceneNode* pParent )
{
//	_ASSERT( (pParent == m_pParent || pParent == m_pParentActorNode ) );
	m_EffectAlignController.m_pAlignReferenceSceneNode = pParent;
	AddController(&m_EffectAlignController);
}

void REffectSceneNode::REffectAlignController::Update(RCameraSceneNode* pCamera /*= NULL*/)
{
	RPFC_THISFUNC;
	RSceneNode *pTarget = m_pSceneNode;
	_ASSERT(pTarget);
	_ASSERT( m_pAlignReferenceSceneNode );

	// �Ϲ����� ������ó�� ���̰� �����. 2D �簢��ó�� ...
	const RMatrix& matWorld = pTarget->GetWorldTransform();
	RVector wP(matWorld._41,matWorld._42,matWorld._43);
	const RVector &vU = m_pAlignReferenceSceneNode->GetUp();
	const RVector &vD = m_pAlignReferenceSceneNode->GetDirection();

	RMatrix matResult;
	matResult.SetLocalMatrix(wP, vD, vU);

	RMatrix matInvParent;
	pTarget->GetParent()->GetWorldTransform().GetInverse(&matInvParent);
	RMatrix matLocal = matResult * matInvParent;

	// ���� up vector �� local�� x ���̶� �̷��� �Ǿ���. -> Z������?
	RVector localDir = RVector(matLocal._21,matLocal._22,matLocal._23);
	RVector localUp = RVector(matLocal._31,matLocal._32,matLocal._33);
	pTarget->SetDirection(localDir,localUp);
}

bool REffectSceneNode::REffectSceneNodePlayControlVisitor::OnPreOrderVisit(RSceneNode* pSceneNode )
{
	// ��û�ڰ� �ڱ��ڽ��̸� ����
	if (pSceneNode == m_pRequestEffectSceneNode)
		return true;

	// REffectBase�� ��ӹ޴� ���
	pSceneNode->UpdateTransform();
	REffectBase *pEffectBase = MDynamicCast(REffectBase, pSceneNode);
	if ( pEffectBase )
	{
		switch (m_ePlayType)
		{
		case PLAY_START:
			pEffectBase->Start();
			break;
		case PLAY_STOP:
			pEffectBase->Stop();
			break;
		default:
			pEffectBase->Pause();
		}
	}
	else
	{
		// REffectActor �� ���������� ó��
		REffectActor* pEffectActor = MDynamicCast( REffectActor, pSceneNode );
		if (pEffectActor)
		{
			switch (m_ePlayType)
			{
			case PLAY_START:
				pEffectActor->EffectStart();
				break;
			case PLAY_STOP:
				pEffectActor->EffectStop();
				break;
			default:
				pEffectActor->EffectPause();
			}
		}
	}

	return true;
}

void REffectSceneNode::Start()
{
	m_bOncePlayed = true;
	m_EffectSceneNodePlayControlVisitor.m_ePlayType = REffectSceneNodePlayControlVisitor::PLAY_START;
	Traverse(&m_EffectSceneNodePlayControlVisitor);
}

void REffectSceneNode::Stop()
{
	m_EffectSceneNodePlayControlVisitor.m_ePlayType = REffectSceneNodePlayControlVisitor::PLAY_STOP;
	Traverse(&m_EffectSceneNodePlayControlVisitor);
}

void REffectSceneNode::Pause()
{
	m_EffectSceneNodePlayControlVisitor.m_ePlayType = REffectSceneNodePlayControlVisitor::PLAY_PAUSE;
	Traverse(&m_EffectSceneNodePlayControlVisitor);
}

bool REffectSceneNode::REffectSceneNodeLoopSetterVisitor::OnPreOrderVisit(RSceneNode* pSceneNode )
{
	// ��û�ڰ� �ڱ��ڽ��̸� ����
	if ( pSceneNode == m_pRequestEffectSceneNode )
		return true;

	REffectBase *pEffectBase = MDynamicCast( REffectBase, pSceneNode );
	if ( pEffectBase )
	{
		pEffectBase->SetLooping(m_bLoop);
	}
	else
	{
		// TODO: REffectActor���� REffectBase�� ��ӹ޾� �ߺ��� ��������.
		REffectActor* pEffectActor = MDynamicCast( REffectActor, pSceneNode );
		if ( pEffectActor )
		{
			pEffectActor->SetLooping(m_bLoop);
		}
	}

	return true;
}

bool REffectSceneNode::REffectSceneNodeLoopGetterVisitor::OnPreOrderVisit(RSceneNode* pSceneNode )
{
	// ��û�ڰ� �ڱ��ڽ��̸� ����
	if ( pSceneNode == m_pRequestEffectSceneNode )
		return true;

	REffectBase *pEffectBase = MDynamicCast( REffectBase, pSceneNode );
	if ( pEffectBase )
	{
		if(pEffectBase->IsLooping())
		{
			m_bLoopResult = true;
			return false; // �ڽĵ� �� �ϳ��� loop�� ���̻� ��ȸ �� �ʿ䰡 ����
		}
	}
	else
	{
		// TODO: REffectActor���� REffectBase�� ��ӹ޾� �ߺ��� ��������.
		REffectActor* pEffectActor = MDynamicCast( REffectActor, pSceneNode );
		if ( pEffectActor )
		{
			if(pEffectActor->IsLooping())
			{
				m_bLoopResult = true;
				return false; // �ڽĵ� �� �ϳ��� loop�� ���̻� ��ȸ �� �ʿ䰡 ����
			}
		}
	}

	return true;
}

bool REffectSceneNode::IsLooping()
{
	REffectSceneNodeLoopGetterVisitor visitor(this);
	Traverse(&visitor);
	return visitor.m_bLoopResult;
}

void REffectSceneNode::SetLooping(bool bLoop)
{
	REffectSceneNodeLoopSetterVisitor visitor(this, bLoop);
	Traverse(&visitor);
}

void REffectSceneNode::UpdateRenderableNodesList()
{
	class RRenderableNodesVisitor : public RSceneNodeVisitor
	{
	public:
		RRenderableNodesVisitor() : m_pEffectSceneNode(NULL), m_nRenderOrder(0) {}
		virtual ~RRenderableNodesVisitor(){}
		virtual bool OnPreOrderVisit(RSceneNode* pSceneNode )
		{
			if( REffectBase* pEffectBase = MDynamicCast(REffectBase, pSceneNode) )
			{
				pEffectBase->SetParentEffectSceneNode( m_pEffectSceneNode );
				pEffectBase->SetRenderOrder( m_nRenderOrder++ );
			}
			return true;
		}
		virtual void OnPostOrderVisit( RSceneNode* pSceneNode ){}

		REffectSceneNode* m_pEffectSceneNode;
		int m_nRenderOrder;
	};

	static RRenderableNodesVisitor v;
	v.m_pEffectSceneNode = this;
	v.m_nRenderOrder = 0;
	Traverse( &v );
}

bool REffectSceneNode::REffectSceneNodeLifeControlVisitor::OnPreOrderVisit(RSceneNode* pSceneNode )
{
	REffectBase *pEffectBase = MDynamicCast( REffectBase, pSceneNode );
	if (pEffectBase)
	{
		if (pEffectBase->IsAlive())
		{
			m_bDeleteEffect = false;
			return false;
		}

		return true;
	}

	// ����Ʈ ���Ϳ� ����
	REffectActor* pEffectActor = MDynamicCast( REffectActor, pSceneNode );
	if (pEffectActor)
	{
		if (pEffectActor->IsAlive())
		{
			m_bDeleteEffect = false;
			return false;
		}

		return true;
	}

	return true;
}

bool REffectSceneNode::REffectSceneNodeChildrenGetterVisitor::OnPreOrderVisit(RSceneNode* pSceneNode )
{
	RPFC_THISFUNC;
	if (m_pRequsetSceneNode == pSceneNode)
		return true;

	// REffectSceneNode ������ REffectActor�� �ڽ��̰� �̰͵��� �ڽľ�⿡�� ���� �ʴ´�.
	if ( MDynamicCast( REffectSceneNode, pSceneNode ) )
		return false;

	if (MIsDerivedFromClass(REffectBase, pSceneNode))
	{
		REffectBase *pEffectBase = static_cast<REffectBase*>(pSceneNode);
		if (m_pSzChildName != NULL)
		{
			if ( pEffectBase->GetNodeName() == m_pSzChildName )
				m_vecEffectChildren.push_back(pEffectBase);

		}
		else
		{
			m_vecEffectChildren.push_back(pEffectBase);
			m_vecNameTable.push_back(pEffectBase->GetNodeName());
		}
	}
	else if (MIsDerivedFromClass(REffectActor, pSceneNode))
	{
		REffectActor* pEffectActor = static_cast<REffectActor*>(pSceneNode);
		if (m_pSzChildName != NULL)
		{
			if (pEffectActor->GetNodeName() == m_pSzChildName)
				m_vecEffectActorChildren.push_back(pEffectActor);

		}
		else
		{
			m_vecEffectActorChildren.push_back(pEffectActor);
			m_vecNameTable.push_back(pEffectActor->GetNodeName());
		}
	}

	return true;
}

// Effect Children Getter
std::vector<std::string> REffectSceneNode::GetChildrenNameTable()
{
	REffectSceneNodeChildrenGetterVisitor visitor(NULL, this);
	Traverse(&visitor);

	return visitor.m_vecNameTable;
}
REffectSceneNode::EFFECT_NODETYPE REffectSceneNode::GetChild( const char* pSzName )
{
	REffectSceneNodeChildrenGetterVisitor visitor(pSzName, this);
	Traverse(&visitor);
	
	REffectBase *pEffectBase = NULL;
	REffectActor *pEffectActor = NULL;

	if (visitor.m_vecEffectActorChildren.empty() == false)
	{
		_ASSERT( visitor.m_vecEffectActorChildren.size() == 1 );
		pEffectActor = visitor.m_vecEffectActorChildren[0];
	}
	else if (visitor.m_vecEffectChildren.empty() == false )
	{
		_ASSERT( visitor.m_vecEffectChildren.size() == 1 );
		pEffectBase = visitor.m_vecEffectChildren[0];
	}

	_ASSERT((pEffectActor == NULL) || (pEffectBase == NULL)); //���� �ϳ��� ��

	return EFFECT_NODETYPE(pEffectBase, pEffectActor);
}

void REffectSceneNode::ControlEffectSceneNodeLife( REffectSceneNode** ppEffectSceneNode )
{
	REffectSceneNode* pEffectSceneNode = *ppEffectSceneNode;
	if (pEffectSceneNode == NULL)
		return;

	// ������ �����÷��� Ȱ���̸�, ������ �ʴ´�.
	if (pEffectSceneNode->m_bLastFramePause)
		return;

	// ���� �ε����� ����Ʈ�� ������ �ʴ´�
	if (pEffectSceneNode->GetUsable() == false)
		return;

	REffectSceneNodeLifeControlVisitor visitor;

	visitor.m_bDeleteEffect = true; // lifecontrol�� true �̸� �ϴ� ���δٰ� �����ϰ� �����ϰ��� Ȯ��.

	// ����Ʈ�� �ѹ��� �÷��� ������ ������ �׳� �����Ѵ�.
	// ����Ʈ�� �����ϰ� Start()���� �ʰ� ����� ��찡 ����.
	if( pEffectSceneNode->m_bOncePlayed )
		pEffectSceneNode->Traverse(&visitor);

	if (visitor.m_bDeleteEffect)
	{
		pEffectSceneNode->m_bDeleteNow = true;
		pEffectSceneNode->RemoveFromParent();
		delete pEffectSceneNode;
		*ppEffectSceneNode = NULL;
	}
}

bool REffectSceneNode::IsLoadingCompleted()
{
	REffectSceneNodeChildrenGetterVisitor visitor(NULL, this);
	Traverse(&visitor);

	for (std::vector< REffectActor* >::iterator itr = visitor.m_vecEffectActorChildren.begin(); itr != visitor.m_vecEffectActorChildren.end(); ++itr)
	{
		if ((*itr)->GetUsable() == false)
			return false;
	}

	for (std::vector< REffectBase* >::iterator itr = visitor.m_vecEffectChildren.begin(); itr != visitor.m_vecEffectChildren.end(); ++itr)
	{
		if ((*itr)->GetUsable() == false)
			return false;
	}

	return true;
}

RSceneNode* REffectSceneNode::Clone()
{
	REffectSceneNode* pNewNode = new REffectSceneNode;
	
	pNewNode->m_strFileName = m_strFileName;
	pNewNode->SetUsable(true);
	pNewNode->SetLastFramePause(m_bLastFramePause);
	pNewNode->SetTransform( GetLocalTransform() );
	pNewNode->SetScale( GetScale() );

	return pNewNode;
}

} //namespace rs3
