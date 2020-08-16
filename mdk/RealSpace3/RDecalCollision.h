#pragma once

#include <vector>
#include <string>
#include "MBox.h"
#include "RVType.h"
#include "RUtil.h"
#include "RCollision.h"

namespace rs3 {

class RCollisionTree;
class RTexture;
class RCollisionTree;
class RDecalCollision;

#define DECALFLAG_TERRAIN		0x001
//#define DECALFLAG_STATICMESH	0x002	// ��������
#define DECALFLAG_ACTOR			0x008	// ĳ���͸� ������ Actor (cl2�� ���ԵǴ� ������)

class RDecalCollision : public RCollision
{
protected:
	virtual bool			IsCollidable(const MBox &box);
	virtual bool			CheckPolygon(const MVector3 &lv0,const MVector3 &lv1,const MVector3 &lv2, const DWORD dwAttribute = 0);
	virtual bool			CheckTangency(const MVector3& vPos, const RCollisionPolygon& poly);	// �� ������� ���ϴ°�?

	virtual void			GetBoundingBox(MBox &outBox);
	virtual float			GetWalkableDistance(const RCollisionPolygon &collisionPolygon);
	virtual MVector3		GetContactPoint(const MVector3& vPos, const MVector3& vNorm);

public:
	RDecalCollision();
	~RDecalCollision();

	RBoundingBox			m_aabb;
	std::vector<RVt_pos>*	m_vecVertex;

	void					SetAABB( const RBoundingBox& rbb);
	void					SetVertex(std::vector<RVt_pos>* vecVertex);
	void					CalcBoundingBox();		// �̹� ������ ��ü�� �ٿ���ڽ��� ���Ѵ�
};

} //namespace rs3
