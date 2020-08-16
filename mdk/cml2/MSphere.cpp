#include "stdafx.h"
#include "MSphere.h"

void MSphere::makeOBB( MOBox *pOBB )
{
	_ASSERT(NULL != pOBB);
	pOBB->vPosition = this->center;
	pOBB->vRange.x =pOBB->vRange.y =pOBB->vRange.z = this->radius;
	pOBB->vAxis[0] = MVector3(1,0,0);
	pOBB->vAxis[1] = MVector3(0,1,0);
	pOBB->vAxis[2] = MVector3(0,0,1);
}

bool MSphere::FitsIn( float x,float y,float z,float r ) const
{
	if( radius > r )	return false;

	if( 
		( center-MVector3(x,y,z) ).LengthSq() <=
		( r - radius ) * ( r - radius )
		)
	{
		return true;
	}

	return false;
}

bool MSphere::FitsIn(const MSphere& sphere) const
{
	return FitsIn(sphere.center.x,sphere.center.y,sphere.center.z,sphere.radius);
}

CULL_STATE MSphere::PlanesTest(const MPlane* pPlanes,int planesNum ) const
{
	CULL_STATE result = CS_INSIDE;
	for(int i=0;i<planesNum;i++)
	{
		float d =  pPlanes[i].GetDistanceTo(this->center);
		if( d < -this->radius )	return CS_OUTSIDE;
		if( d <  this->radius )
		{
			result = CS_PARTIAL;
		}
	}
	return result;
}

// �Է� : Masking �θ��忡�� Partial�̾��� ����� ��Ʈ����ŷ
// ��� : Masking ���ݳ�忡�� Partial�� �������� ����� ��Ʈ����ŷ
// ���� : �θ��忡�� inside,outside������ ���� ����� �ڽĳ�忡�Ե� ������ ������ ������.
//        �ڽĳ��� �θ��尡 �ٸ� ������ ��� �ϴ� ����� partial����̴�.
// �������� : �θ��尡 ��ü���鿡���ؼ� outside���ٸ� , �ڽĳ��� ����׽�Ʈ�� ���ϴ� �����϶��� ��밡��
// ���� BVH��� �׽�Ʈ�� �ٸ��� : ������ �׽�Ʈ�� ������̶� partial������ ���� �׽�Ʈ�� �ٷ� ������. ������ �� ������ partial������ ���� ����� ��� �˾Ƴ���,
//                                �ڽĿ��� �� ����� ������ �����ϰ�, �ڽ��� �� partial������ ���� ��鸸���� �׽�Ʈ�� �Ѵ�.
//                                �ڽ����� ������ partial ������ ���� ����� �������ٸ�, �׽�Ʈ�� �������� ���(�θ𿡼� partial������ �� ���)������ ����ɰŴ�.
CULL_STATE MSphere::PlanesTest_M( const MPlane* pPlanes,int planesNum ,int& Masking ) const
{
	CULL_STATE result = CS_INSIDE;
	int OutMasking = 0;
	int k=0;
	for( int i=0,k=1; k<=Masking/*,i<planesNum*/; i++,k+=k )
	{
		_ASSERT(i<planesNum);
		if( k & Masking )
		{
			float d =  pPlanes[i].GetDistanceTo(this->center);
			if( d < -this->radius )
			{
				return CS_OUTSIDE;
			}
			if( d <  this->radius )
			{
				result = CS_PARTIAL;
				OutMasking |= k;
			}
		}
	}
	Masking = OutMasking;
	return result;
}

CULL_STATE MSphere::PlanesTest_MC( const MPlane* pPlanes,int planesNum ,int& Masking,int &DeterminantPlaneIndex ) const
{
	CULL_STATE result = CS_INSIDE;
	int OutMasking = 0;
	int k=0;

	k = 1 << DeterminantPlaneIndex;
	if( k & Masking )
	{
		float d = pPlanes[DeterminantPlaneIndex].GetDistanceTo(this->center);
		if( d < -this->radius )
		{
			return CS_OUTSIDE;
		}
		if( d <  this->radius )
		{
			result = CS_PARTIAL;
			OutMasking |= k;
		}
	}


	for( int i=0,k=1; k<=Masking/*,i<planesNum*/; i++,k+=k )
	{
		_ASSERT(i<planesNum);
		if( (i != DeterminantPlaneIndex) && (k & Masking) )
		{
			float d =  pPlanes[i].GetDistanceTo(this->center);
			if( d < -this->radius )
			{
				DeterminantPlaneIndex = i;
				return CS_OUTSIDE;
			}
			if( d <  this->radius )
			{
				result = CS_PARTIAL;
				OutMasking |= k;
			}
		}
	}
	Masking = OutMasking;
	return result;
}

/*
int AABBvsFrustum(AABB *b, FRUSTUM *f, int in_mask, int *out_mask)
{
	float m, n; int i, k = 1 << b->start_id, result = INSIDE;

	PLANE *sp = f->plane + b->start_id; *out_mask=0;

	if (k & in_mask) 
	{

		m = (sp->a * b->v[sp->nx].x) + (sp->b * b->v[sp->ny].y) + (sp->c * b->v[sp->nz].z);
		if (m > -sp->d) return OUTSIDE;

		n = (sp->a * b->v[sp->px].x) + (sp->b * b->v[sp->py].y) + (sp->c * b->v[sp->pz].z);
		if (n > -sp->d) 
		{ 
			*out_mask |= k; 
			result = INTERSECT; 
		}
	}

	for (i = 0, k = 1; k <= in_mask; i++, k += k)
	{
		if ((i != b->start_id) && (k & in_mask)) 
		{ 
			PLANE *p = f->plane + i;

			m = (p->a * b->v[p->nx].x) + (p->b * b->v[p->ny].y) + (p->c * b->v[p->nz].z);
			if (m > -p->d) 
			{ 
				b->start_id = i; 
				return OUTSIDE; 
			}

			n = (p->a * b->v[p->px].x) + (p->b * b->v[p->py].y) + (p->c * b->v[p->pz].z);
			if (n > -p->d) 
			{ 
				*out_mask |= k; 
				result = INTERSECT; 
			}
		}
	}
	return result;
}

int AABBvsFrustum(AABB *b, FRUSTUM *f, int in_mask, int *out_mask)
{
	float m, n; int i, k, result = INSIDE; *out_mask=0;
	for (i = 0, k = 1; k <= in_mask; i++, k += k) 
	{
		if (k & in_mask) 
		{

			PLANE *p = f->plane + i;

			m = (p->a * b->v[p->nx].x) + (p->b * b->v[p->ny].y) + (p->c * b->v[p->nz].z);
			if (m > -p->d) return OUTSIDE;

			n = (p->a * b->v[p->px].x) + (p->b * b->v[p->py].y) + (p->c * b->v[p->pz].z);
			if (n > -p->d) { *out_mask |= k; result = INTERSECT; }

		} 
	}
	return result;
}
*/