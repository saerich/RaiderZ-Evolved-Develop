#include "stdafx.h"
#include "MMatrix.h"
#include "MQuat.h"

static float _matIdentity[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
} ;

const MMatrix MMatrix::IDENTITY(_matIdentity);

void MMatrix::ExtractScale(MVector3& vecScale) const
{
	// scale�� ���Ѵ�
	vecScale.x = MVector3(_11,_12,_13).Length();
	vecScale.y = MVector3(_21,_22,_23).Length();
	vecScale.z = MVector3(_31,_32,_33).Length();
}

bool MMatrix::Decompose(MVector3& vecScale, MVector3& vecTranslation, MQuat& quatRotation) const
{
	// Affine �� �����Ѵ�

	// scale�� ���Ѵ�
	ExtractScale(vecScale);

	// ����� ȸ�� �κ��� �����Ϻκ� ����
	MMatrix matRotation;
	MVector3 invScale = MVector3(1.f/vecScale.x, 1.f/vecScale.y, 1.f/vecScale.z);
	matRotation._11 = _11*invScale.x; matRotation._12 = _12*invScale.x; matRotation._13 = _13*invScale.x;
	matRotation._21 = _21*invScale.y; matRotation._22 = _22*invScale.y; matRotation._23 = _23*invScale.y;
	matRotation._31 = _31*invScale.z; matRotation._32 = _32*invScale.z; matRotation._33 = _33*invScale.z;
	quatRotation = matRotation;	// ���ʹϾ����� ��ȯ

	// �̵����� ���Ѵ�
	vecTranslation = MVector3(_41,_42 ,_43);

	return true;
}


bool MMatrix::Decompose(MVector3& vecScale, MVector4& vecTranslation, MQuat& quatRotation) const	// Vector4¥�� - 090722, OZ
{
	// Affine �� �����Ѵ�

	// scale�� ���Ѵ�
	ExtractScale(vecScale);

	// ����� ȸ�� �κ��� �����Ϻκ� ����
	MMatrix matRotation;
	MVector3 invScale = MVector3(1.f/vecScale.x, 1.f/vecScale.y, 1.f/vecScale.z);
	matRotation._11 = _11*invScale.x; matRotation._12 = _12*invScale.x; matRotation._13 = _13*invScale.x;
	matRotation._21 = _21*invScale.y; matRotation._22 = _22*invScale.y; matRotation._23 = _23*invScale.y;
	matRotation._31 = _31*invScale.z; matRotation._32 = _32*invScale.z; matRotation._33 = _33*invScale.z;
	quatRotation = matRotation;	// ���ʹϾ����� ��ȯ

	// �̵����� ���Ѵ�
	// w������Ʈ�� �� 1��..
	vecTranslation = MVector4(_41,_42 ,_43, 1);	

	return true;
}


// �������� ���� �ʴ� Decompose() - 090724, OZ
bool MMatrix::Decompose(MVector3& vecTranslation, MQuat& quatRotation) const
{
	MVector3 vGarbage;
	return Decompose( vGarbage, vecTranslation, quatRotation);
}


void MMatrix::SetScreenSpaceMatrix(DWORD dwScreenWidth, DWORD dwScreenHeight)
{
	MakeIdentity();
	SetTranslation(MVector3(0.5f +0.5f/dwScreenWidth, 0.5f +0.5f/dwScreenHeight, 0.0f));
	SetScale(MVector3(0.5f, -0.5f, 1.0f));
}