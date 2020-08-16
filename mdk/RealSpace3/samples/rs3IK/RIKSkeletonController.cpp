#include "MDebug.h"
#include "RTypes.h"
#include "RSkeleton.h"
#include "RIKSkeletonController.h"
#include "IK/CIKSolverJacobianTranspose.h"
#include "IK/CIKSolverCyclicCoordinateDescent.h"
#include "IK/CNode.h"

#include "RSimpleCylinderSceneNode.h"
#include "RSceneManager.h"
#include "RActor.h"



RIKSkeletonController::RIKSkeletonController(RActor* pActor)
: m_pIKSolver(new CIKSolverCyclicCoordinateDescent)
, m_pActor(pActor)
{
	m_OffsetMatrix.MakeIdentity();
	m_pIKSolver->SetTryLimit(100);
	m_pIKSolver->SetTargetThreshold(0.5f);
}


RIKSkeletonController::~RIKSkeletonController(void)
{
	// child ���� �����
	while(!m_nodes.empty()) 
	{
		RIKNode* pEffector = GetEffector();
		if(pEffector->pSceneNode) 
		{
			pEffector->pSceneNode->RemoveFromParent();
			delete pEffector->pSceneNode;
		}
		
		CNode *pParent = pEffector->node.AccessParent();
		if(pParent)
		{
			pParent->RemoveChild(&pEffector->node,false);
		}

		delete pEffector;
		m_nodes.pop_back();
	}
	delete m_pIKSolver;
}


void RIKSkeletonController::SetTargetPosition(const RVector& targetPosition)
{
	RVector posLocal = targetPosition;

	m_pIKSolver->SetTargetPosition(CVector3(posLocal.x,posLocal.y,posLocal.z));
}



void RIKSkeletonController::Solve()
{
	m_pIKSolver->Solve(true);
	GetRoot()->node.CalculateMatrix(true);
	
	bool firstNode = true;
	
	for(list<RIKNode*>::iterator i = m_nodes.begin(); i!=m_nodes.end(); i++) 
	{
		RIKNode *pNode = *i;
		if(!pNode->pSceneNode) continue;

		CMatrix4 mat = pNode->node.GetMatrix();
		RMatrix matTransform = (float*)&mat;
		{	// SceneNode �� ��ȯ��� (����׿�
			RMatrix matTransform = (float*)&mat;

			pNode->pSceneNode->SetTransform(matTransform);
			pNode->pSceneNode->Update();
		}

		if(pNode->pActorNode==NULL) continue;


		CQuaternion q = pNode->node.GetRotation();
		q.Normalize();
		CMatrix4 matLocal(q, pNode->node.GetTranslation());

		RMatrix matTemp;
		memcpy(&matTemp,&matLocal,sizeof(RMatrix));

		if( firstNode )
		{
			firstNode = false;
			RMatrix inverceRootParent ;
			m_OffsetMatrix.GetInverse(&inverceRootParent);
			matTransform.MultiplyTo( inverceRootParent,  matTemp );
		}

		pNode->pActorNode->SetTransform( matTemp);
	}
}


void RIKSkeletonController::Initialize()
{
	m_pIKSolver->Initialize(GetEffector()->node,(int)GetNodeCount());
}


void RIKSkeletonController::AddNode(RActorNode* pActorNode,  bool IsRoot)
{
	static bool feef= false;
	RIKNode *pNode = new RIKNode;
	pNode->pActorNode = pActorNode;

	RMatrix matLocal;
	if(pActorNode==NULL)
	{
		matLocal.MakeIdentity();
		matLocal._41 = 1;
		pNode->node.SetNegativeLimits(CVector3(0,0,0));
		pNode->node.SetPositiveLimits(CVector3(0,0,0));
	}
	else
	{
		IsRoot ? pActorNode->GetLocalTransform().MultiplyTo( m_OffsetMatrix, matLocal ) :  
				 matLocal = pActorNode->GetLocalTransform();
	}

	
	pNode->node.SetTranslation(CVector3(matLocal._41,matLocal._42,matLocal._43));


	CMatrix4 matRotation;
	memcpy(&matRotation,&matLocal,sizeof(CMatrix4));
	_ASSERT(sizeof(CMatrix4)==sizeof(RMatrix));
	CQuaternion q(matRotation);
	pNode->node.SetRotation(q);

	if(GetEffector()) 
	{	
		GetEffector()->node.AddChild(&pNode->node);
	}

	m_nodes.push_back(pNode);


	// Compute transformation matrices
	GetRoot()->node.CalculateMatrix(true);

	RSimpleCylinderSceneNode *pSceneNode = new RSimpleCylinderSceneNode;
	pNode->pSceneNode = pSceneNode;
	REngine::GetSceneManager().AddSceneNode(pSceneNode );	// todo: ����
	pSceneNode->m_fRadius = 1;
	pSceneNode->m_fHeight = 10;
	if(pActorNode==NULL)
	{
		pSceneNode->m_dwColor = 0xffff8080;
		pSceneNode->m_fHeight = 1;
	}
}


// start -> end �ʱ����� ���뿡 �ۿ��Ѵ�. end���� ������
bool RIKSkeletonController::Create(RActorNode* pStartNode,RActorNode* pEndNode)
{
	if( pStartNode->GetParentActorNode() != NULL)
	{
		m_OffsetMatrix = pStartNode->GetParentActorNode()->GetWorldTransform();
	}


	// end�����κ��� �θ� Ÿ�� �ö󰡼� startBip���� ��ũ�� ã�´�
	RActorNode* currentNode = pEndNode;
	list<RActorNode*> boneNodes;	// �θ�->�ڽ��� ������ ���������
	
	while(currentNode!=NULL) 
	{
		boneNodes.push_front(currentNode);
		
		if(currentNode==pStartNode) 
		{
			if(boneNodes.size()<2) 
			{
				mlog("at least 2 link required.\n");
				return false;
			}

			list<RActorNode*>::iterator i=boneNodes.begin();
			AddNode(*i, true);
			i++;
			for(; i!=boneNodes.end(); i++) 
			{
				AddNode(*i);
			}

			AddNode(NULL);	// �� �� effector node�� �ϳ� �����
		
			Initialize();
			return true;
		}
		currentNode = currentNode->GetParentActorNode();
	}

	// start -> end �� ���� ��ũ�� ����.
	mlog("IK Controller create failed\n");
	return false;
}
