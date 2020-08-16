#include "StdAfx.h"
#include "RActorNodePhysique.h"
#include "RMeshNode.h"
#include "RMesh.h"
#include "RActor.h"
#include "RProfileInfo.h"
#include "RShaderCompositeManager.h"


namespace rs3 {

MImplementRTTI(RActorNodePhysique, RActorNode);
MImplementRTTI(RActorNodePhysiqueGPU, RActorNodePhysique);

RActorNodePhysique::RActorNodePhysique(ACTOR_NODE_TYPE eActorNodeType = ACTOR_NODE_PHYSIQUE) : RActorNode(eActorNodeType)
{
	m_baseComponents = RSC_POSITION | RSC_NORMAL | RSC_PHYSIQUE;

	// ������ ���� �ν��Ͻ� ������ ���� �ʴ´�. �ؽ��� ���� �ν��Ͻ��� �۾��ϸ� �׶� ��������.
	m_bCanInstancing = false;

	//m_bResetVertexAgainstMatrix = false;
}

RActorNodePhysique::~RActorNodePhysique(void)
{
}

void RActorNodePhysique::UpdateTransformAndVertex()
{
	RPFC_THISFUNC;
	// �ѹ� ���� �� ���� �ִ��� ������ üũ �ϹǷ� �Ź� �����ϵ� ��� ���� ��..
	//ResetVertexAgainstMatrix();

	// ���� ��Ʈ���� ���.
	const int MAX_BONE_COUNT = 54;
	_ASSERT(m_refBones.size()<=MAX_BONE_COUNT);	// ���ÿ� �����Ǵ� ���� ���� ����

	// �� ������ �ȿ��� ������ ���� ������ ������ �� ���� �ִ�. ���� �ð��� ���Ͽ� ������ ���ϵ��� ����.
	if( m_nLastUpdatedFrame == REngine::GetDevice().GetFrameCount()) 
		return;
	m_nLastUpdatedFrame = REngine::GetDevice().GetFrameCount();

	if( IsVertexMotionBlur() || IsPixelMotionBlur())
	{
#ifdef USING_RSC_PHYSIQUE_QT
		// ���� �������� ���� 
		m_vBoneTransPrev = m_vBoneTrans;
		m_quatBoneRotsPrev = m_quatBoneRots;
#endif
	}

	for(size_t i=0;i<m_refBones.size();i++) 
	{
		RActorNode* pBoneNode = m_refBones[i];

		// ���ʹϾ�� �̵����� ���� �Ǿ� �ִ� ���� �װ� ����. 
		if( pBoneNode->IsUseForPhysiqueBone())
		{
#ifdef USING_RSC_PHYSIQUE_QT
			m_quatBoneRots[i] = m_quatBoneBaseRots[i] * pBoneNode->GetResultRotation();	// <- It's OK!!!
			RVector4 vResultTran = RVector4( pBoneNode->GetResultTranslation().x, pBoneNode->GetResultTranslation().y, pBoneNode->GetResultTranslation().z, 1);
			// V = ( PR * VT * S) + PT
			m_vBoneTrans[i] = ( pBoneNode->GetResultRotation() * (m_vBoneBaseTrans[i] * pBoneNode->GetResultScale().z)) + vResultTran;

#endif
		}
 		else
		{
			const RMatrix& matResult = pBoneNode->GetResultTransform();

			m_matBoneBases[i].MultiplyTo(matResult,m_matBones[i]);

			/* ���� base * �� inv base * �ִϸ��̼� ���
			m_matBones[i] = m_pMeshNode->m_matBase  
			* pBoneNode->m_pMeshNode->m_matInvBase
			* matResult;

			���� m_pMeshNode->m_matBase * pBoneNode->m_pMeshNode->m_matInvBase ��
			m_matBoneBases[i] ���ٰ� �̸� ����� ���Ҵ�

			*/
			
#ifdef USING_RSC_PHYSIQUE_QT
			// ���ʹϾ��� �̾� ���� ������... 
			// �̰� �� �Ǹ� ������ ����
			RQuaternion qBoneRot;
			RVector3 vBoneScale;
			RVector4 vBoneTran;

			// ���ʹϾ� �̵� �� ��� �ý����� �Ϻ��� ���߾� ���� �ʾҴ�.
			// ��� ������ �����͸� ��������..
			//m_matBones[i].Decompose( vBoneScale,  m_vBoneTrans[i], m_quatBoneRots[i]);
			// ���� ���� ���� ��Ʈ�������� ���ظ� �ϴ� �� ���� ���������� ���� �� �����.

			// ���� ��Ʈ�������� ���� ���� ���� ����� �غ���
			matResult.Decompose( vBoneScale,  vBoneTran, qBoneRot);
			m_quatBoneRots[i] = m_quatBoneBaseRots[i] * qBoneRot;	// <- It's OK!!!
			RVector4 vTran = RVector4( vBoneTran.x, vBoneTran.y, vBoneTran.z, 1);
			// V = ( PR * VT * S) + PT
			// �����ϸ��� m_fSBasecale�� ���� ���� �ƴ϶�, matResult�� �����ϸ� ������ �Ǿ�� �Ѵ�.
			// ���� = ���̽� * ����Ʈ �̹Ƿ� ���̽��� �̵� ���� ����Ʈ�� �����ϸ��� ���� ���Ѿ� ����. 
			// �����ϸ��� z���� ������ ���.
			m_vBoneTrans[i] = ( qBoneRot * (m_vBoneBaseTrans[i] * vBoneScale.z)) + vTran;
			// �����ϸ� ���� xyz�� �ϳ��� �Ѱܾ� �Ǵµ� ������ z�� ����
			// w�����ٰ� �����ϸ� ���� ������
			m_vBoneTrans[i].w = vBoneScale.x;

			// ���� �� ������ �״�� ��� �ϴ� ���� �ƴ϶� ������ ����Ʈ�� ���� �����ϰ� 
			//float fSlerp = REngine::GetDevice().GetFrameRate() / 30.0f;
			//float fSlerpDecimal = fSlerp;
			//if( fSlerp > 1)
			//	fSlerpDecimal = fSlerp - (float)((int)fSlerp);
			// to : prev  from : curr
			//*this = to-from; // delta on all 3 axis
			//*this*=slerp;    // times interpolant distance.
			//*this+=from;     // plus source
			// �켱 �ִϸ��̼��� ������ ���� �ʾƺ���... 
			//m_vBoneTrans[i].InterpolateTo( m_vBoneTransPrev[i], fSlerp, m_vBoneTransPrev[i]);
			//m_quatBoneRotsPrev[i] = m_quatBoneRotsPrev[i].Slerp( m_quatBoneRots[i], m_quatBoneRotsPrev[i], fSlerpDecimal);
			// �� ������ ���� ���..
#else		
 #ifdef USING_RSC_PHYSIQUE_DQ
			// ���ʹϾ�� �̵� ���� ���� 
			//m_matBones[i].Decompose( m_vBoneScales[i],  m_vBoneTrans[i], m_quatBoneRots[i]);
			// ��� ���ʹϾ� �� ���� 
			//m_dqBoneDQ[i] = RDQuaternion( m_quatBoneRots[i], m_vBoneTrans[i]);
 #endif
#endif
		}
	}

}


int RActorNodePhysique::UpdatePoint_SoftPhysiqueAnimation()
{
	RPFC_THISFUNC;

	// �������ӿ� �ι� ������Ʈ�� ���´�
	if(m_nLastUpdatedFrame==REngine::GetDevice().GetFrameCount()) 
		return 1;

	m_nLastUpdatedFrame = REngine::GetDevice().GetFrameCount();


	int p_num,i,j,c_id;
	RVector _vec_all,_vec;
	float weight;
	RMatrix t_mat,local,basemat,inv;

	for(i=0;i<m_pMeshNode->m_nPhysiqueCnt;i++) { // point_num

		_vec_all = RVector(0,0,0);

		p_num = m_pMeshNode->m_pPhysique[i].m_num;

		//		_ASSERT(p_num<5);
		//		if(p_num > MAX_PHYSIQUE_KEY) p_num = MAX_PHYSIQUE_KEY;

		for(j=0;j<p_num;j++) {// 1 - 3

			c_id	= m_pMeshNode->m_pPhysique[i].GetBoneID(j);
			weight	= m_pMeshNode->m_pPhysique[i].GetWeight(j);

			_vec = m_pMeshNode->m_pPointTableOrg[i] * m_matBones[c_id];

			_vec_all += _vec * weight;
		}

		//		_ASSERT(_vec_all.Length()>0.01);

		m_pMeshNode->m_pPointTable[i] = _vec_all;
	}


	/////// normal update �׽�Ʈ
	if(!m_pMeshNode->m_pPointNormalTable) return 1;

	for(int i=0;i< m_pMeshNode->m_nVertexIndexTableCnt;i++ ) {

		int nPosIndex = m_pMeshNode->m_pVertexIndexTable[i].p;

		RVector n   = m_pMeshNode->m_pNormalTable[m_pMeshNode->m_pVertexIndexTable[i].n];

		_vec_all = RVector(0,0,0);
		p_num = m_pMeshNode->m_pPhysique[nPosIndex].m_num;
//		if(p_num > MAX_PHYSIQUE_KEY) p_num = MAX_PHYSIQUE_KEY;

		RVector trNormal[20];
		for(j=0;j<p_num;j++) {// 1 - 3

			c_id	= m_pMeshNode->m_pPhysique[nPosIndex].GetBoneID(j);
			weight	= m_pMeshNode->m_pPhysique[nPosIndex].GetWeight(j);

			m_matBones[c_id].TransformNormal(n,trNormal[j]);
			_vec_all += trNormal[j] * weight;
		}

		m_pMeshNode->m_pPointNormalTable[i] = _vec_all;
	}

	return 1;
}

void RActorNodePhysique::RenderNormal(DWORD dwColor)
{
	UpdatePoint_SoftPhysiqueAnimation();
	RActorNode::RenderNormal(dwColor);
}

void RActorNodePhysique::RenderWire(DWORD dwColor)
{
	UpdatePoint_SoftPhysiqueAnimation();
	RActorNode::RenderWire(dwColor);
}

bool RActorNodePhysique::OnPick(RPICKINFO& pickInfo)
{
	// detail �϶��� polygon picking �� �����ϴ�.
	if(pickInfo.inMethod!=RPM_DETAIL) return false;

	RMatrix matWorld = m_pActor->GetWorldTransform();
	RMatrix invWorld;
	matWorld.GetInverse(&invWorld);

	RVector localOrigin = pickInfo.inPickOrigin * invWorld;
	RVector localTo = pickInfo.inPickTo * invWorld;

	UpdatePoint_SoftPhysiqueAnimation();

	if(PickLocal(localOrigin,localTo,&pickInfo))
	{
		pickInfo.outPosition = pickInfo.outPosition * matWorld;
		return true;
	}
	return false;
}

void RActorNodePhysique::ResetTransform()
{
//	RActorNode::ResetTransform();
	m_matLocal->MakeIdentity();
//	m_matResult.MakeIdentity();
	m_fVisibility = m_pMeshNode->m_fBaseVisibility;
	m_fNoiseRef = m_pMeshNode->m_fBaseNoiseRef;
}

void RActorNodePhysique::RenderPrimitive(int index)
{
	RPFC_THISFUNC;

	UpdatePoint_SoftPhysiqueAnimation();
	m_pMeshNode->UpdateNodeBuffer( ); // m_pPointTable �� ��������� m_pVert �� �ű��...

	m_pMeshNode->m_pVb->RenderIndexSoft( 
		&m_pMeshNode->m_pIb->m_pIndex[m_pMeshNode->m_pMtrlTableInfo[index].offset],
		m_pMeshNode->m_pMtrlTableInfo[index].count,
		m_pMeshNode->m_pIb->m_PrimitiveType);
	REngine::GetResourceProfiler().AddPolygon(PPT_ACTORPHYSIQUE, m_pMeshNode->m_pMtrlTableInfo[index].count );
	REngine::GetResourceProfiler().AddDrawCall( PPT_ACTORPHYSIQUE, 1);
	REngine::GetResourceProfiler().AddMeshProfileInfo(m_pMeshNode, index);
}

void RActorNodePhysique::RenderAllNodePrimitive()
{
	RPFC_THISFUNC;
	UpdatePoint_SoftPhysiqueAnimation();
	m_pMeshNode->UpdateNodeBuffer( ); // m_pPointTable �� ��������� m_pVert �� �ű��...

	m_pMeshNode->m_pVb->RenderIndexSoft( 
		&m_pMeshNode->m_pIb->m_pIndex[0],
		m_pMeshNode->m_nFaceCnt,
		m_pMeshNode->m_pIb->m_PrimitiveType);
	REngine::GetResourceProfiler().AddPolygon(PPT_ACTORPHYSIQUE, m_pMeshNode->m_nFaceCnt );
	REngine::GetResourceProfiler().AddDrawCall( PPT_ACTORPHYSIQUE, 1);
	REngine::GetResourceProfiler().AddMeshProfileInfo(m_pMeshNode, -1);
}

bool RActorNodePhysique::Validate()
{
	for(unsigned int i=0;i<m_refBones.size();i++)
	{
		if(m_refBones[i]==NULL) return false;
	}
	return RActorNode::Validate();
}


//////////////////////////////////////////////////////////////////////////
// RActorNodePhysiqueGPU

RActorNodePhysiqueGPU::RActorNodePhysiqueGPU(void)
{
}

RActorNodePhysiqueGPU::~RActorNodePhysiqueGPU(void)
{
}

void RActorNodePhysiqueGPU::SetShaderTransformConstant(const RMatrix& matView, const RMatrix& matViewProj)
{
	RPFC_THISFUNC;

	RShaderCompositeManager::SetTransformConstant(m_pActor->GetWorldTransform(), matView, matViewProj);

#ifdef USING_RSC_PHYSIQUE_DQ	
	if(!m_dqBoneDQ.empty())
	{
		RShaderCompositeManager::m_cBlendDQ.SetDQuaternionArray( &m_dqBoneDQ[0],m_pMeshNode->m_nBoneTableCnt);
	}
#else
 #ifdef USING_RSC_PHYSIQUE_QT
	if(!m_quatBoneRots.empty())
	{
		// ���ʹϾ�� ������� �����Ƿ� Vector4 array�� �Ѱ��ຸ��.. - 097022, OZ
		// ���ʹϾ��� SetValue�� �ѱ�� ���� ���µ� Vector4�� �ѱ�� �Ȼ���... ����..
		//RShaderCompositeManager::m_cBlendQuats.SetQuaternionArray( &m_quatBoneRots[0],m_pMeshNode->m_nBoneTableCnt);
		RShaderCompositeManager::m_cBlendQuats.SetVector4Array( (RVector4*)&m_quatBoneRots[0],m_pMeshNode->m_nBoneTableCnt);
	}

	if(!m_vBoneTrans.empty())
	{
		RShaderCompositeManager::m_cBlendTrans.SetVector4Array( &m_vBoneTrans[0],m_pMeshNode->m_nBoneTableCnt);
	}

	if( IsVertexMotionBlur() || IsPixelMotionBlur())
	{
		if(!m_quatBoneRotsPrev.empty())
		{
			// ���ʹϾ�� ������� �����Ƿ� Vector4 array�� �Ѱ��ຸ��.. - 097022, OZ
			// ���ʹϾ��� SetValue�� �ѱ�� ���� ���µ� Vector4�� �ѱ�� �Ȼ���... ����..
			//RShaderCompositeManager::m_cBlendQuatsPrev.SetQuaternionArray( &m_quatBoneRotsPrev[0],m_pMeshNode->m_nBoneTableCnt);
			RShaderCompositeManager::m_cBlendQuatsPrev.SetVector4Array( (RVector4*)&m_quatBoneRotsPrev[0],m_pMeshNode->m_nBoneTableCnt);
		}

		if(!m_vBoneTransPrev.empty())
		{
			RShaderCompositeManager::m_cBlendTransPrev.SetVector4Array( &m_vBoneTransPrev[0],m_pMeshNode->m_nBoneTableCnt);
		}
	}

	if( m_fSBasecale > 0)
		RShaderCompositeManager::m_cBlendScale.SetFloat( m_fSBasecale);

 #else
	if(!m_matBones.empty())
	{
		RShaderCompositeManager::m_cBlendMatrices.SetMatrixArray(&m_matBones[0],m_pMeshNode->m_nBoneTableCnt);
	}
 #endif
#endif
}

// COMMENT OZ - �̰����� �ɸ��Ͱ� ���� �ȴ�. 
void RActorNodePhysiqueGPU::RenderPrimitive(int index)
{
	RPFC_THISFUNC;
	m_pMeshNode->m_pVb->RenderIndexBuffer( 
		m_pMeshNode->m_pMtrlTableInfo[index].offset,
		m_pMeshNode->m_pMtrlTableInfo[index].count,
		m_pMeshNode->m_pIb->m_PrimitiveType);
	REngine::GetResourceProfiler().AddPolygon(PPT_ACTORPHYSIQUE, m_pMeshNode->m_pMtrlTableInfo[index].count );
	REngine::GetResourceProfiler().AddDrawCall( PPT_ACTORPHYSIQUE, 1);
	REngine::GetResourceProfiler().AddMeshProfileInfo(m_pMeshNode, index);
}

void RActorNodePhysiqueGPU::RenderAllNodePrimitive()
{
	RPFC_THISFUNC;
	m_pMeshNode->m_pVb->RenderIndexBuffer( 
		0,
		m_pMeshNode->m_nFaceCnt,
		m_pMeshNode->m_pIb->m_PrimitiveType);
	REngine::GetResourceProfiler().AddPolygon(PPT_ACTORPHYSIQUE, m_pMeshNode->m_nFaceCnt );
	REngine::GetResourceProfiler().AddDrawCall( PPT_ACTORPHYSIQUE, 1);
	REngine::GetResourceProfiler().AddMeshProfileInfo(m_pMeshNode, -1);
}

}