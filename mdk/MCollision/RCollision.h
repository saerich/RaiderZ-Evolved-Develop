#pragma once

#include "RLib.h"
#include "MMath.h"

namespace rs3 {

const float SPHERE_DEFAULT_RADIUS		=	1.f;

class RCollisionTree;
struct RCollisionTriangle;

struct RCollisionPolygon {
	RCollisionPolygon(const MVector3 &v0,const MVector3 &v1,const MVector3 &v2,const MVector3 &_nearestPoint, const MVector3 &_normal, int _nDebugFlag, DWORD dwAttribute) 
		: nearestPoint(_nearestPoint), normal(_normal), nDebugFlag(_nDebugFlag), dwPolygonAttribute(dwAttribute)
		
		{ v[0] = v0; v[1] = v1; v[2] = v2; }
	
	MVector3 v[3];
	MVector3 nearestPoint;
	MVector3 normal;

	int nDebugFlag;
	DWORD dwPolygonAttribute;
};

// ����׿� �浹 ������ ��� ���ؽ�
struct RColVertex {
	MVector3 pos;
	DWORD	color;
};

class RCollision
{
protected:
	RCollisionTree*	m_pTargetCollisionTree;	// �浹üũ ��� ( ���� )
	MVector3	m_source;
	MVector3	m_destination;		// �����ϰ� ������
	MBox		m_bbMovement;		// �����̴� ��ü�� �ٿ���ڽ�

	vector<RCollisionPolygon> m_colPolygons;

private:
	bool CheckAllowedHeight(const MVector3& v1, const MVector3& v2, const MVector3& v3, const MVector3* pOrgFrom, const float* pAllowedHeight);

protected:
	// �浹ü�� ���鶧 �����ؾ� �ϴ� ��ǵ�
	// �̹� ������ ��ü�� �ٿ���ڽ��� ���Ѵ�
	void CalcBoundingBox();
	
	/// �ٿ���ڽ� ���, ������ǥ��
	virtual void GetBoundingBox(MBox &outBox) = 0;
	/// �浹ü�� destionation ���� ���� �� �浹������ �����Ҷ����� �Ÿ����� ( = walkable distance / | destination - position | )
	virtual float GetWalkableDistance(const RCollisionPolygon &collisionPolygon) = 0;	

	virtual MVector3 GetContactPoint(const MVector3& vPos, const MVector3& vNorm) = 0;
	
	// from���� vDir�������� fDist��ŭ �̵��ϸ鼭 �浹�� �˻��Ѵ�.
	// MoveTest/Sliding�Լ��� ��������� ������ ȣ���ϱ⶧���� �̵������� ��� �ٲ��.
	// ���� ���������� ���� ��� �������� ��Ȳ�� ����� ǥ���� �� ���� �������� ����.
	// vFrom���� vDir�������� fDist��ŭ �̵������� ������� pOut�� �������
	bool MoveTestReal(const MVector3& vFrom, const MVector3& vDir, float fDist, MVector3& vOut);


	// ��������ؼ� �̵�, x-y�θ� �̵��ϰ� z�� ������ �����°� �ֿ��� ���.
	bool MoveTestTerrain(const MVector3& vFrom, const MVector3& vDir, float fDist, MVector3& vOut, MVector3* pOrgForm = NULL, float* pAllowedHeight = NULL);

public:

	void Clear();

	RCollision(void);
	virtual ~RCollision(void);

	void SetTargetCollisionTree(RCollisionTree* pTargetTree);


	// �ڽ��� ��ġ���� to�� �̵��Ҷ� ó�� �΋H���� ���� pOut���� ����, �΋H���� ���ٸ� return false
	bool MoveTest(const MVector3 &from,const MVector3 &to, MVector3& vOut, float *pMoved = NULL);

	// �ڽ��� ��ġ���� to �� �̵��Ҷ� �߷°���ؼ� �����Ѹ�ŭ�� pOut���� ����, �΋H���� ���ٸ� return false
	bool MoveTestGravity(const MVector3 &from,const MVector3 &to, MVector3& vOut, bool *pSliding = NULL, MVector3* pOrgForm = NULL, float* pAllowedHeight = NULL);


	void GetMovementBoundingBox(MBox &outBox);	// �������� ��ü �ٿ���ڽ��� ���Ѵ�, ������ǥ��	

	virtual bool CheckTangency(const MVector3& vPos, const RCollisionPolygon& poly) = 0;	// �� ������� ���ϴ°�?

public:
	/////////////////////////////////////////////////////
	// SceneNode ���� �浹�� �����ϱ� ���� ȣ���� ��ǵ�

//	void SetWorldMatrix(const RMatrix& matWorld);	// ����ó���� ���� ��������� ����, �Ʒ��� �� ��ǿ� ������ �ش�
	///	�̹������Ӱ� box�� ������ �浹���ɼ��� �ִ���
	virtual bool IsCollidable(const MBox &box) = 0;
	/// �־��� �������� �浹�ϴ��� �˻�
	virtual bool CheckPolygon(const MVector3 &v0,const MVector3 &v1,const MVector3 &v2, const DWORD dwAttribute = 0 ) = 0;
};


inline void RCollision::SetTargetCollisionTree(RCollisionTree* pTargetTree) { m_pTargetCollisionTree = pTargetTree; }
inline void RCollision::GetMovementBoundingBox(MBox &outBox) { outBox = m_bbMovement;}


/////////////////////
// Sphere �� collider

class RCollisionSphere : public RCollision
{
	float	m_fRadius;

protected:
	virtual void GetBoundingBox(MBox &outBox);
	virtual float GetWalkableDistance(const RCollisionPolygon &collisionPolygon);	

	virtual MVector3 GetContactPoint(const MVector3& vPos, const MVector3& vNorm);

public:
	RCollisionSphere(float fRadius = SPHERE_DEFAULT_RADIUS);
	~RCollisionSphere();

	void SetRadius(float fRadius);
	float GetRadius();

	virtual bool IsCollidable(const MBox &box);
	virtual bool CheckPolygon(const MVector3 &lv0,const MVector3 &lv1,const MVector3 &lv2, const DWORD dwAttribute = 0);

	static bool IsIntersectMovingSphereTriangle(const MVector3 &from,const MVector3 &to,const float fRadius,const MVector3 &v0,const MVector3 &v1,const MVector3 &v2, MVector3 &outNearest, int &debugFlag);

	virtual bool CheckTangency(const MVector3& vPos, const RCollisionPolygon& poly);	// �� ������� ���ϴ°�?
};

class RCollisionMovingCone : public RCollision
{
	MMatrix m_View;
	float m_fInitDistance;
	float m_fDistance;
	float m_fHeight;
	float m_fRadius;

protected:
	virtual void GetBoundingBox(MBox &outBox);
	virtual float GetWalkableDistance(const RCollisionPolygon &collisionPolygon){return true;}	

	virtual MVector3 GetContactPoint(const MVector3& vPos, const MVector3& vNorm){return MVector3(0,0,0);}

	float GetNearestDistance(MVector3& begin, MVector3& end);

public:
	RCollisionMovingCone(float fHeght, float fRadius, float fDistance, MMatrix mView);
	~RCollisionMovingCone(){}


	virtual bool IsCollidable(const MBox &box);
	virtual bool CheckPolygon(const MVector3 &lv0,const MVector3 &lv1,const MVector3 &lv2, const DWORD dwAttribute = 0);

	virtual bool CheckTangency(const MVector3& vPos, const RCollisionPolygon& poly){ return true;}

	float GetDistance(){return m_fDistance;}
};

}
	