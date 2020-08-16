#include "stdafx.h"

#include "RPhysXClothActorNode.h"
#include "RActor.h"
#include "RVType.h"
#include "RMeshNode.h"
#include "RMaterialResource.h"
#include "RMaterialResourceMgr.h"
#include "RShaderCompositeManager.h"
#include "RShaderComposite.h"
#include "REnvironmentSet.h"
#include "RCameraSceneNode.h"

#include "RRenderHelper.h"
#include "RPhysXClothController.h"

namespace rs3 {

RPhysXClothActorNode::RPhysXClothActorNode(NxPhysicsSDK* pSDK, NxScene* pScene, NxCookingInterface* pCooking)
: RActorNode(ACTOR_NODE_CLOTH)
{
	m_pSDK			= pSDK;
	m_pScene		= pScene;
	m_pCooking		= pCooking;

	m_pController	= NULL;

	m_eClothState	= E_NONE;

	ClearNxClothMeshData();
	m_ClothCPUMeshData.Init(this);
}

RPhysXClothActorNode::~RPhysXClothActorNode()
{
}

void RPhysXClothActorNode::ConnectClothMeshData( NxMeshData& rMeshData )
{
	m_NxClothMeshData = rMeshData;
}

void RPhysXClothActorNode::ChangeClothState( E_CLOTH_STATE _eClothState )
{
	m_eClothState = _eClothState;

	switch(m_eClothState)
	{
	case E_NONE:
		ClearNxClothMeshData();
		break;

	case E_CPU:
		ClearNxClothMeshData();
		UpdateTransformAndVertex();
		break;

	case E_PHYSX:
		_ASSERT(GetNxClothVertexPosBegin() != NULL);
	    break;
	}
}

void RPhysXClothActorNode::OnDestroy()
{
	_ASSERT(m_pController != NULL);

	// cloth �����
	m_pController->RemoveClothNode(GetNodeName());

	// ��Ʈ�ѷ� ����
	int nRefCount = m_pController->Drop();
	_ASSERT(nRefCount >= 0);
	if ( 0 == nRefCount)
	{
		m_pActor->RemoveController(m_pController);
		delete m_pController;
	}
	m_pController = NULL;

	// RActorNode
	RActorNode::OnDestroy();
}

void RPhysXClothActorNode::OnAdd()
{
	RActorNode::OnAdd();

	// ������Ʈ ��Ʈ�ѷ� ����
	_ASSERT(m_pController == NULL);

	// �̹� ���Ϳ� ��ϵ� ��Ʈ�ѷ��� �ִ��� Ȯ��
	m_pController = static_cast<RPhysXClothController*>( m_pActor->GetFirstControllerByName( RPhysXClothController::CONTROLLER_TYPE(), RPhysXClothController::CONTROLLER_NAME() ) );

	// ��ϵ� ��Ʈ�ѷ��� �������
	if (NULL == m_pController)
	{
		// ������ ���Ŵ����� ��û
		RSceneNodeController* pController = REngine::GetSceneManager().CreateSceneNodeController( RPhysXClothController::CONTROLLER_NAME() );

		// ���Ŵ������� �����Ҽ� �ִ� ��Ʈ�ѷ��� �������
		if (pController == NULL)
		{
			pController = new RPhysXClothController(m_pSDK, m_pScene, m_pCooking);
		}

		// ���� ���
		m_pController =  static_cast<RPhysXClothController*>(pController);
		m_pActor->AddController(m_pController);
	}

	m_pController->AddClothNode(GetNodeName());
	m_pController->Grap();
}

void RPhysXClothActorNode::UpdateTransformAndVertex()
{
	if (E_CPU == m_eClothState)
		m_ClothCPUMeshData.UpdateVertex();
}

void RPhysXClothActorNode::SetVertexStream()
{
	if (E_NONE == m_eClothState)
		return;

	if (E_PHYSX == m_eClothState)
	{
		RVector* arrReceivePosition	= GetNxClothVertexPosBegin();
		RVector* arrReceiveNormal	= GetNxClothVertexNormalBegin();

		bool bNeedLock = (m_pMeshNode->m_pVb->m_bUseHWVertex && m_pMeshNode->m_pVb->m_hVB != -1);
		if (bNeedLock)
			m_pMeshNode->m_pVb->Lock();

		char* arrVertex	= m_pMeshNode->m_pVb->m_v;

		int cursorPos = m_pMeshNode->m_pVb->m_nPosOffset;
		int cursorNormal = m_pMeshNode->m_pVb->m_nNormalOffset;

		for(int i = 0; i < m_pMeshNode->m_pVb->m_nVertexCnt; ++i)
		{
			memcpy( &arrVertex[cursorPos], &arrReceivePosition[i], sizeof(RVector) );
			memcpy( &arrVertex[cursorNormal], &arrReceiveNormal[i], sizeof(RVector) );

			cursorPos += m_pMeshNode->m_pVb->m_nVertexSize;
			cursorNormal += m_pMeshNode->m_pVb->m_nVertexSize;
		}

		if (bNeedLock)
			m_pMeshNode->m_pVb->Unlock();
	}

	RActorNode::SetVertexStream();
}

#ifdef INFERRED_WORK
void RPhysXClothActorNode::SetNodeRenderState(  RCameraSceneNode* pCurrentCamera, RMaterial* pMaterial, bool bFogEnable, bool bBlurAlphaForce, bool bSetAboutLight)
{
	if (E_NONE == m_eClothState)
		return;

	RActorNode::SetNodeRenderState( pCurrentCamera, pMaterial, bFogEnable, bBlurAlphaForce, bSetAboutLight);
	SetNodeRenderStateForCloth( pCurrentCamera, pMaterial );
}
#else
void RPhysXClothActorNode::SetNodeRenderState(  RCameraSceneNode* pCurrentCamera, RMaterial* pMaterial, bool bFogEnable, bool bBlurAlphaForce)
{
	if (E_NONE == m_eClothState)
		return;

	RActorNode::SetNodeRenderState( pCurrentCamera, pMaterial, bFogEnable, bBlurAlphaForce);
	SetNodeRenderStateForCloth( pCurrentCamera, pMaterial );
}
#endif


void RPhysXClothActorNode::SetNodeRenderStateForDepth( RCameraSceneNode* pCurrentCamera,  RMaterial* pMaterial )
{
	RActorNode::SetNodeRenderStateForDepth( pCurrentCamera, pMaterial );
	SetNodeRenderStateForCloth( pCurrentCamera, pMaterial );
}

void RPhysXClothActorNode::SetNodeRenderStateForCloth(  RCameraSceneNode* pCurrentCamera, RMaterial* pMaterial )
{
	// �̹� ���� �������� �ùķ��̼��� �� �����̹Ƿ� �״�� ����ϸ� �ȴ�.
	RMatrix matIdentity = RMatrix::IDENTITY;//m_pActor->GetWorldTransform();
	RShaderCompositeManager::m_cWorld.SetMatrix(matIdentity);
	RShaderCompositeManager::m_cWorldViewProj.SetMatrix( /*matWorld * */pCurrentCamera->GetViewProjectionMatrix() );
	RShaderCompositeManager::m_cWorldView.SetMatrix( /*matWorld **/ pCurrentCamera->GetViewMatrix() );
	RShaderCompositeManager::m_cTextureFactor.SetVector4( /*m_pActor->m_vMultiplyColor */ RVector4(1,1,1,1) );
	REngine::GetDevice().SetAlphaTestEnable(false);

	if (!m_matBones.empty())
		RShaderCompositeManager::m_cBlendMatrices.SetMatrixArray(&matIdentity/*m_matBones[0]*/,m_pMeshNode->m_nBoneTableCnt);

	//if (!m_matBonesPrev.empty())
	//	RShaderCompositeManager::m_cBlendMatricesPrev.SetMatrixArray(&matIdentity/*m_matBones[0]*/,m_pMeshNode->m_nBoneTableCnt);	// 090629, OZ
}

void RPhysXClothActorNode::RenderPrimitive(int index)
{
	if (E_NONE == m_eClothState)
		return;

	RActorNode::RenderPrimitive(index);
	REngine::GetProfileInfo().AddPolygon(PPT_ACTORPHYSIQUE, m_pMeshNode->m_pMtrlTableInfo[index].count );
}

void RPhysXClothActorNode::RCPUClothMeshData::UpdateVertex()
{
	if (NULL == m_pOwnerClothActorNode)
		return;

	RMeshNode* pMeshNode = m_pOwnerClothActorNode->m_pMeshNode;
	RMesh* pParentMesh = pMeshNode->m_pParentMesh;
	int nVertCount = pMeshNode->m_nPointCnt;
	int nCntFaceIndex = pMeshNode->m_nFaceIndexTableCnt; // == m_nCntFace * 2 * 3;	// �ϳ��� ������(��)�� 2���� �ﰢ������ �̷������ �ﰢ���� ������ 3���̴�.

	if (NULL == m_pVertPos)
		m_pVertPos = new RVector[nVertCount];

	if (NULL == m_pVertNorm)
		m_pVertNorm = new RVector[nVertCount];

	//////////////////////////////////////////////////////////////////////////
	//
	// from RCharCloth
	// position update and normal update
	//
	//////////////////////////////////////////////////////////////////////////

	int		nRefBone;
	int		nCid;
	float	fWeight;
	RMatrix mat;
	RVector vResult;
	RVector vCurPos;

	RMatrix _matBone[60];// Mesh���� �����ϴ� Bone�� �ִ� ����
	// Bone TM�� REFERENCE ���� TM �̴�. ��ȯ ������ �ʿ���.
	for(int i = 0; i < pMeshNode->m_nBoneTableCnt; ++i)
	{
		RActorNode* pBoneNode = m_pOwnerClothActorNode->m_refBones[i];

		const RMatrix& matResult = pBoneNode->GetResultTransform();

		// Bone World TM
		//				RMatrix matResult = m_refBones[i]->GetResultTransform();
		// 		RMatrix matResult = m_pActorNode->m_refBones[i]->GetWorldTransform()
		// 			* m_pActorNode->m_pActor->GetWorldInverseTransform();// * m_pActorNode->m_pActor->GetWorldInverseTransform();

		// Base�� REFRENCE ���� TM�� ���Ѵ�.
		// MeshNode Base * Bone Inverse Base // m_pMeshNode->m_pBoneTable[iBone] * 
		_matBone[i] = pMeshNode->m_matBase * pBoneNode->m_pMeshNode->m_matInvBase * matResult;
	}

	for( int i = 0; i < nVertCount; ++i )
	{
		vResult.Set( 0, 0, 0 );
		
//		if( pMeshNode->isHWPhysique() )
		// isPhysique �� �������� �ȵǼ� ���ĳ����ϴ�. - dub
		if( pMeshNode->isPhysique() )
		{
			nRefBone = pMeshNode->m_pPhysique[i].m_num;

			for( int j = 0; j < nRefBone; ++j )
			{
				nCid	= pMeshNode->m_pPhysique[i].GetBoneID(j);
				fWeight	= pMeshNode->m_pPhysique[i].GetWeight(j);

				vCurPos	= pMeshNode->m_pPointTableOrg[i] * _matBone[nCid];
				vResult	+= vCurPos * fWeight;
			}

			vResult = vResult * m_pOwnerClothActorNode->m_pActor->GetWorldTransform();
		}
		else
		{
			RMatrix matWorld = m_pOwnerClothActorNode->GetWorldTransform();
			vResult = pMeshNode->m_pPointTableOrg[i] * matWorld;
		}
		m_pVertPos[i].Set(vResult);
	}

	// normal update
	ZeroMemory( m_pVertNorm, sizeof(RVector)*nVertCount );

	int	nIndexVertex;
	RVertexIndex* pVertexIndexTable;
	RVector vPoint[3];
	for( int i = 0; i < nCntFaceIndex / 3; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			pVertexIndexTable = &pMeshNode->m_pVertexIndexTable[ pMeshNode->m_pFaceIndexTable[i * 3 + j] ];
			nIndexVertex = pVertexIndexTable->p;

			_ASSERT( nIndexVertex < nVertCount );
			_ASSERT( nIndexVertex >= 0 );

			vPoint[j]		= m_pVertPos[nIndexVertex];
		}

		RVector vNormal( (vPoint[2] - vPoint[0]).CrossProduct(vPoint[2] - vPoint[1]) );

		for( int j = 0; j < 3; ++j )
		{
			pVertexIndexTable = &pMeshNode->m_pVertexIndexTable[ pMeshNode->m_pFaceIndexTable[i * 3 + j] ];
			nIndexVertex = pVertexIndexTable->n;

			m_pVertNorm[nIndexVertex] += vNormal;
		}
	}

	for( int i = 0; i < nVertCount; ++i )
		m_pVertNorm[i].Normalize();

	// vertex buffer update 
	BYTE* pVertex = (BYTE*)REngine::GetDevice().LockVertexBuffer(pMeshNode->m_pVb->m_hVB);
	_ASSERT(pVertex);
	const int nVertexSize = pMeshNode->m_pVb->m_nVertexSize;
	for( int i = 0; i < pMeshNode->m_nVertexIndexTableCnt; ++i )
	{
		// pos ����
		RVector* pPosition = (RVector*) ( pVertex + pMeshNode->m_pVb->m_nPosOffset );
		*pPosition = m_pVertPos[pMeshNode->m_pVertexIndexTable[ i ].p];

		// normal 
		RVector* pNormal = (RVector*) ( pVertex + pMeshNode->m_pVb->m_nNormalOffset );
		*pNormal = m_pVertNorm[ pMeshNode->m_pVertexIndexTable[ i ].n ];

		pVertex += nVertexSize;
	}

	REngine::GetDevice().UnlockVertexBuffer(pMeshNode->m_pVb->m_hVB);
}

}
