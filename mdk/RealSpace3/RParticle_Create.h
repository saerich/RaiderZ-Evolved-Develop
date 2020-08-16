#pragma once

#include "RTypes.h"

namespace rs3 {

//////////////////////////////////////////////////////////////////////////
// ���� ��ƼŬ�� ��� ������ �������� �����ϱ� ���� �������̽�
class RParticleCreator
{
public:
	virtual ~RParticleCreator() {}
	virtual RVector3	GetDirection(float fAngle) = 0;
	virtual RVector3	GetPosition(const RVector& vRange) = 0;

protected:
	virtual float		GetRandomFloat( float fMin, float fMax );
	virtual RVector3	GetRandomDirection(float fAngle);
};

//////////////////////////////////////////////////////////////////////////
// Box ������ �������� �߻�.
// - vRange�� Emitter�� ������ �����Ѵ�.
class RBoxEmit : public RParticleCreator
{
public:
	virtual RVector3	GetDirection(float fAngle) override;
	virtual RVector3	GetPosition(const RVector& vRange) override;
};

//////////////////////////////////////////////////////////////////////////
// ��������� ���������� ��.
// vRange�� �߻� ��ġ(X,Y,Z)�� Ÿ��ü�μ� �����ȴ�.
class RSphereEmit : public RParticleCreator
{
public:
	virtual RVector3	GetDirection(float fAngle) override;
	virtual RVector3	GetPosition(const RVector& vRange) override;
};

//////////////////////////////////////////////////////////////////////////
// Ring emitter
class RRingEmit : public RParticleCreator
{
	// torus�� �������� R �׸��� �β��� r�̶�� �Ҷ� z���� �߽����� ȸ���� torus�� equation��
	// ( R - sqrt(x^2+y^2) )^2 + z^2 = r^2
public:
	virtual RVector3	GetDirection(float fAngle) override;
	virtual RVector3	GetPosition(const RVector& vRange) override;
};

}