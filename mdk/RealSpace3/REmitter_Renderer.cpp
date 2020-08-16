#include "stdafx.h"
#include "REmitter.h"

namespace rs3 {

RVector* REmitter::REmitterRenderer::GetRandomVector(RVector *pOut)
{
	pOut->z = RandomNumber(-1.0f, 1.0f);
	float fR = (float)sqrtf(1.0f - pOut->z*pOut->z);

	float t = RandomNumber(-D3DX_PI, D3DX_PI);
	pOut->x = (float)cosf(t)*fR;
	pOut->y = (float)sinf(t)*fR;

	return pOut;
}

//////////////////////////////////////////////////////////////////////////
// ������ �⺻ ���� Ŭ����
//
class REmitterBillboard : public REmitter::REmitterRenderer
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix) { return 0; }

protected:
	int FillParticleFromAXIS(REmitter *pEmitter, list<RParticleHandle> *pArray, const RVector *pXAxis, const RVector *pYAxis, const RVector *pDir);
};

//////////////////////////////////////////////////////////////////////////
// XYZ Billboard �μ� ������ �ϴ� �˰��� �������̽�
//
class REmitterBillboardRender : public REmitterBillboard
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix);
};

// �⺻���� �� ���� �̿��Ͽ� �Ϲ����� �����带 ����� �ڵ�
int REmitterBillboard::FillParticleFromAXIS(REmitter *pEmitter, list<RParticleHandle> *pArray, const RVector *pXAxis, const RVector *pYAxis, const RVector *pDir)
{
	RPFC_THISFUNC;

	_ASSERT(pEmitter!=NULL);
	_ASSERT(pXAxis!=NULL);
	_ASSERT(pYAxis!=NULL);
	_ASSERT(pDir != NULL);
	_ASSERT(pArray!=NULL);
	_ASSERT(pArray->size());

	const REmitterDescription *pDesc = pEmitter->GetDesc();
	const std::vector<RParticleVariable> &varParticle = pDesc->vecParticleVar;

	RVector4 vertexScale( pEmitter->GetVertexScale() );

	// �θ� �������迡 ���� scale (TransformedScale) ����
	float fTransformedScale = pEmitter->GetResultScale(pEmitter->GetScale().x, pEmitter->GetParent());

	int particleNum = pArray->size();
	int vertexCount = 0;
	PVERTEXFORMAT* pVertex = pEmitter->GetVertexArray(particleNum, vertexCount);

	RVector vParticlePosition;
	RMatrix rotMat;
	RVector vXAxis;
	RVector vYAxis;
	RVector2 vScale;
	RVector vXAxisMinus;
	RVector vXAsisPlus;
	RVector vYAxisMinus;
	RVector vYAxisPlus;
	RVector vPos[4];
	for( list<RParticleHandle>::const_iterator it = pArray->begin(); it != pArray->end(); ++it )
	{
		RParticle *pParticle = REffectManager::GetParticle((*it).m_Handle);

		pParticle->GetResultPos( pEmitter->GetWorldTransform(), pDesc->bFollowParentPosition, pDesc->bFollowParentRotation, vParticlePosition );

		//��ƼŬ�� ȸ������ ����
		
		rotMat.SetRotationMatrixAxis( *pDir, pParticle->m_fRadiansum );

		// ������ �� ���� ����
		vXAxis.Set(pXAxis->x, pXAxis->y, pXAxis->z);
		vYAxis.Set(pYAxis->x, pYAxis->y, pYAxis->z);

		rotMat.TransformVect(vXAxis);
		rotMat.TransformVect(vYAxis);

		vScale = pParticle->GetParticleScale( pDesc );
		vXAxis *= vScale.x * fTransformedScale;
		vYAxis *= vScale.y * fTransformedScale;

		vXAxisMinus.Set(vXAxis*vertexScale.x);
		vXAsisPlus.Set(vXAxis*vertexScale.z);
		vYAxisMinus.Set(vYAxis*vertexScale.w);
		vYAxisPlus.Set(vYAxis*vertexScale.y);

		vPos[0].Set( vParticlePosition -vXAxisMinus +vYAxisPlus ); // left-top
		vPos[1].Set( vParticlePosition +vXAsisPlus +vYAxisPlus ); // right-top
		vPos[2].Set( vParticlePosition -vXAxisMinus -vYAxisMinus ); // left-bottom
		vPos[3].Set( vParticlePosition +vXAsisPlus -vYAxisMinus ); // right-bottom

		pVertex = pEmitter->FillVertex( pVertex, vPos, pParticle);
	}

	REffectManager::WriteVertexBuffer( vertexCount );
	return particleNum;
}

int REmitterBillboardRender::FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
{
	_ASSERT(pEmitter!=NULL);
	_ASSERT(pWorldViewMatrix!=NULL);
	_ASSERT(pArray!=NULL);

	// X, Y Vector�� ���Ѵ�.
	RVector vBaseXAxis( pWorldViewMatrix->_11, pWorldViewMatrix->_21, pWorldViewMatrix->_31);
	RVector vBaseYAxis( pWorldViewMatrix->_12, pWorldViewMatrix->_22, pWorldViewMatrix->_32);
	RVector vBaseZAxis( pWorldViewMatrix->_13, pWorldViewMatrix->_23, pWorldViewMatrix->_33);

	vBaseXAxis.FastNormalize();
	vBaseYAxis.FastNormalize();
	vBaseZAxis.FastNormalize();

	return FillParticleFromAXIS(pEmitter, pArray, &vBaseXAxis, &vBaseYAxis, &vBaseZAxis);
}

//////////////////////////////////////////////////////////////////////////

class REmitterVerticalBoardRender : public REmitterBillboard
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix);
};

int REmitterVerticalBoardRender::FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
{
	_ASSERT(pEmitter!=NULL);
	_ASSERT(pWorldViewMatrix!=NULL);
	_ASSERT(pArray!=NULL);

	// X, Y Vector�� ���Ѵ�.
	RVector vBaseXAxis( pWorldViewMatrix->_11, pWorldViewMatrix->_21, pWorldViewMatrix->_31);
	RVector vBaseZAxis( pWorldViewMatrix->_13, pWorldViewMatrix->_23, pWorldViewMatrix->_33);

	vBaseXAxis.FastNormalize();
	vBaseZAxis.FastNormalize();

	return FillParticleFromAXIS(pEmitter, pArray, &vBaseXAxis, &RVector::AXISZ, &vBaseZAxis);
}

//////////////////////////////////////////////////////////////////////////

class REmitterPlaneXYRender : public REmitterBillboard
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
	{
		return FillParticleFromAXIS(pEmitter, pArray, &RVector::AXISX, &RVector::AXISY, &RVector::AXISZ);
	}
};

//////////////////////////////////////////////////////////////////////////

class REmitterPlaneYZRender : public REmitterBillboard
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
	{
		return FillParticleFromAXIS(pEmitter, pArray, &RVector::AXISY, &RVector::AXISZ, &RVector::AXISX);
	}
};

//////////////////////////////////////////////////////////////////////////

class REmitterPlaneZXRender : public REmitterBillboard
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
	{
		return FillParticleFromAXIS(pEmitter, pArray, &RVector::AXISZ, &RVector::AXISX, &RVector::AXISY);
	}
};

//////////////////////////////////////////////////////////////////////////

class REmitterAlignRender : public REmitterBillboard
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
	{
		RPFC_THISFUNC;
		_ASSERT(pEmitter!=NULL);
		_ASSERT(pArray!=NULL);

		RVector vBaseXAxis = pEmitter->GetWorldRight();
		RVector vBaseYAxis = pEmitter->GetWorldUp();
		RVector vBaseZAxis = pEmitter->GetWorldDirection();

		return FillParticleFromAXIS(pEmitter, pArray, &vBaseXAxis, &vBaseYAxis, &vBaseZAxis);
	}
};

//////////////////////////////////////////////////////////////////////////
// view �� particle move �� ���� �þ����� ��ƼŬ.
//
class REmitterSpread : public REmitter::REmitterRenderer
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix) { return 0; }

protected:
	int FillParticleFromAlignVector(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix, const RVector *pAlignVector);
};

int REmitterSpread::FillParticleFromAlignVector(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix, const RVector *pAlignVector)
{
	_ASSERT(pEmitter!=NULL);
	_ASSERT(pWorldViewMatrix!=NULL);
	_ASSERT(pAlignVector!=NULL);
	_ASSERT(pArray!=NULL);
	_ASSERT(pArray->size());

	RVector vCameraDirection(pWorldViewMatrix->_13, pWorldViewMatrix->_23, pWorldViewMatrix->_33);
	vCameraDirection.FastNormalize();

	const REmitterDescription *pDesc = pEmitter->GetDesc();
	const std::vector<RParticleVariable> &varParticle = pDesc->vecParticleVar;

	RVector4 vertexScale( pEmitter->GetVertexScale() );

	// �θ� �������迡 ���� scale (TransformedScale) ����
	float fTransformedScale = pEmitter->GetResultScale(pEmitter->GetScale().x, pEmitter->GetParent());

	int particleNum = pArray->size();
	int vertexCount = 0;
	PVERTEXFORMAT* pVertex = pEmitter->GetVertexArray(particleNum, vertexCount);
	int vertexCountPerParticle = pEmitter->GetVertexCountPerParticle();

	RVector vParticlePosition;
	RVector vXLine, vYLine;
	RVector2 vScale;
	RVector vXAxisMinus;
	RVector vXAsisPlus;
	RVector vYAxisMinus;
	RVector vYAxisPlus;
	RVector vPos[4];
	for( list<RParticleHandle>::iterator it = pArray->begin(); it != pArray->end(); ++it )
	{
		RParticle *pParticle = REffectManager::GetParticle((*it).m_Handle);

		pParticle->GetResultPos( pEmitter->GetWorldTransform(), pDesc->bFollowParentPosition, pDesc->bFollowParentRotation, vParticlePosition );

		vYLine = pParticle->m_vCurPos - pParticle->m_vPrevPos;

		if( pDesc->bFollowParentPosition && pDesc->bFollowParentRotation )
		{
			float length = vYLine.Length();
			vYLine.FastNormalize();

			pEmitter->GetWorldTransform().TransformNormal( vYLine );
			vYLine *= length;
		}

		float fDLenghtSq = vYLine.LengthSq();
		if ( fDLenghtSq <= FLT_EPSILON )
		{
			vPos[0].Set( vParticlePosition );
			vPos[1].Set( vParticlePosition );
			vPos[2].Set( vParticlePosition );
			vPos[3].Set( vParticlePosition );

			pVertex = pEmitter->FillVertex( pVertex, vPos, pParticle );
			continue;
		}

		float fDLenght = sqrtf(fDLenghtSq);
		vYLine /= fDLenght;

		// ���� ���Ϳ� ����� �������͸� �����Ͽ� �¿� ���͸� ���Ѵ�.
		vXLine = vYLine.CrossProduct(vCameraDirection);
		vXLine *= *pAlignVector;
		vXLine.FastNormalize();

		vScale = pParticle->GetParticleScale( pDesc );
		vXLine *= vScale.x * fTransformedScale;
		vYLine *= vScale.y * fTransformedScale;

		vXAxisMinus = (vXLine*vertexScale.x);
		vXAsisPlus = (vXLine*vertexScale.z);
		vYAxisMinus = (vYLine*vertexScale.w);
		vYAxisPlus = (vYLine*vertexScale.y);

		// 4���� ���͸� ����Ѵ�.
		vPos[0].Set( vParticlePosition -vXAxisMinus +vYAxisPlus ); // left-top
		vPos[1].Set( vParticlePosition +vXAsisPlus +vYAxisPlus ); // right-top
		vPos[2].Set( vParticlePosition -vXAxisMinus -vYAxisMinus ); // left-bottom
		vPos[3].Set( vParticlePosition +vXAsisPlus -vYAxisMinus ); // right-bottom

		pVertex = pEmitter->FillVertex( pVertex, vPos, pParticle );
	}

	REffectManager::WriteVertexBuffer( vertexCount );
	return particleNum;
}

//////////////////////////////////////////////////////////////////////////

class REmitterSpreadRender : public REmitterSpread
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
	{
		static const RVector vAlign( 1.f, 1.f, 1.f );
		return FillParticleFromAlignVector(pEmitter, pArray, pWorldViewMatrix, &vAlign);
	}
};

//////////////////////////////////////////////////////////////////////////

class REmitterSpreadAlignXYRender : public REmitterSpread
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
	{
		static const RVector vAlign( 1.f, 1.f, 0.f );
		return FillParticleFromAlignVector(pEmitter, pArray, pWorldViewMatrix, &vAlign);
	}
};

//////////////////////////////////////////////////////////////////////////

class REmitterSpreadAlignYZRender : public REmitterSpread
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
	{
		static const RVector vAlign( 0.f, 1.f, 1.f );
		return FillParticleFromAlignVector(pEmitter, pArray, pWorldViewMatrix, &vAlign);
	}
};

//////////////////////////////////////////////////////////////////////////

class REmitterSpreadAlignZXRender : public REmitterSpread
{
public:
	virtual int FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix)
	{
		static const RVector vAlign( 1.f, 0.f, 1.f );
		return FillParticleFromAlignVector(pEmitter, pArray, pWorldViewMatrix, &vAlign);
	}
};

//////////////////////////////////////////////////////////////////////////

void REmitter::SetBillboardType(BILLBOARD_TYPE eType)
{
	SAFE_DELETE(m_pRenderer);
	switch ( eType )
	{
	case BILLBOARD_XYZ:
		m_pRenderer = new REmitterBillboardRender;
		break;
	case BILLBOARD_XZ:
		m_pRenderer = new REmitterVerticalBoardRender;
		break;
	case PLANE_XY:
		m_pRenderer = new REmitterPlaneXYRender;
		break;
	case PLANE_YZ:
		m_pRenderer = new REmitterPlaneYZRender;
		break;
	case PLANE_ZX:
		m_pRenderer = new REmitterPlaneZXRender;
		break;
	case LINE_SPREAD:
		m_pRenderer = new REmitterSpreadRender;
		break;
	case LINE_SPREAD_XY:
		m_pRenderer = new REmitterSpreadAlignXYRender;
		break;
	case LINE_SPREAD_YZ:
		m_pRenderer = new REmitterSpreadAlignYZRender;
		break;
	case LINE_SPREAD_ZX:
		m_pRenderer = new REmitterSpreadAlignZXRender;
		break;
	case EMITTER_ALIGN:
		m_pRenderer = new REmitterAlignRender;
		break;
	}

	m_sEmitterDesc.eBillboardType = eType;
	return;
}

}