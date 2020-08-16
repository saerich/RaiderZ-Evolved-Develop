#include "stdafx.h"
#include "MMathLinear.h"


void MMathLinear::solveLinearSystem(MVector3 *pX, const MMatrix3 &A, const MVector3 &B)
{
	MMatrix3 invMat;
	A.GetInverse(&invMat, NULL);

	_ASSERT(pX);

	// transpose and multiply - (���� �ڵ尡 ������ �����̶� �׷� �� ����)
	pX->x = B.x*invMat._11 + B.y*invMat._12 + B.z*invMat._13;
	pX->y = B.x*invMat._21 + B.y*invMat._22 + B.z*invMat._23;
	pX->z = B.x*invMat._31 + B.y*invMat._32 + B.z*invMat._33;
}

//////////////////////////////////////////////////////////////////////////
//

void MMathLinear::LeastSquareSums::clear()
{
	x = y = z = 0.0f;
	xx = xy = xz = yy = yz = 0.0f;
	samples = 0;
}

MMathLinear::LeastSquareSums::LeastSquareSums()
{
	clear();
}

void MMathLinear::calculateLeastSquareSums(MMathLinear::LeastSquareSums *pSums, const MVector3 *pVertices, int nVectorCount)
{
	_ASSERT(pSums);

	for ( int i = 0; i<nVectorCount; ++i )
	{
		const MVector3 &v = pVertices[i];
		pSums->x += v.x;
		pSums->y += v.y;
		pSums->z += v.z;
		pSums->xx += (v.x*v.x);
		pSums->xy += (v.x*v.y);
		pSums->xz += (v.x*v.z);
		pSums->yy += (v.y*v.y);
		pSums->yz += (v.y*v.z);

		++pSums->samples;
	}
	return;
}

// ��Ȯ�� �� �� ������ z �� 1.0f �� ���־���ϰ�, �Ƹ��� �� �ּ���������� HeightField( z���� up���ͷ� �ϴ�)���� ��ȿ�� �� ����.
void MMathLinear::calculateLeastSquarePlane(const MMathLinear::LeastSquareSums &sums, MVector3 &normal, MVector3 &center)
{
	MMatrix3 mA(
		sums.xx, sums.xy, sums.x,
		sums.xy, sums.yy, sums.y,
		sums.x , sums.y , (float)sums.samples);

	MVector3 vB(
		sums.xz, sums.yz, sums.z);

	MVector3 vX;

	solveLinearSystem(&vX, mA, vB);

	normal = -vX;
	normal.z = 1.0f;

	const float inverseSamples = 1.0f / (float)sums.samples;
	center.x = sums.x * inverseSamples;
	center.y = sums.y * inverseSamples;
	center.z = sums.z * inverseSamples;

	return;
}
