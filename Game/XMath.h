#ifndef _XMATH_H
#define _XMATH_H

#include "AMath.h"
#include "MVector3.h"
#include "MVector4.h"
#include "MMatrix.h"



// Ŭ���̾�Ʈ���� ����ϴ� ���� ���� �Լ� ����
class XMath : public crl::AMath
{
public:
	/// ���� ���� ��Ʈ������ ����� ���� �Լ�
	static void MakeModelMatrix(mat4* pout, const vec3& pos, const vec3& dir, const vec3& up);
	static bool RandomBool();

	static vec3 MoveParabolaInTime(vec3 vStartPos, vec3 vTargetPos, vec3& vCalParabola, float fMaxHeight, float fTickTime, float& fRemainTime);
};












#endif // _XMATH_H