#pragma once

#include "XGlobal.h"
#include "RTerrain.h"
#include "RCollisionTree.h"

typedef RCollisionTriangle CollisionAttribute;

using namespace rs3;
namespace rs3 {
	class RActorNode;
	class RPointLightSceneNode;
}

/// ������ �浹����
class XCollisionTree : public RCollisionTree
{
	RTerrain*				m_pTerrain;

	/// collision �� this�� ���������� �浹������ �������� �����ϴ�
	virtual void GetCollidablePolygons(RCollision &collision, DWORD dwAttribute);

public:
	XCollisionTree();
	void SetTerrain(RTerrain* pTerrain)	{ m_pTerrain = pTerrain;}

	/// ������ �����ϴ� Triangle�� ����
	int GetTerrainPolygonCount();

	/// �ش� �ε����� Triangle(CCW)�� �����´�. (0, 0, bottom), (0, 0, up), (1, 0, bottom) ... ������ �ε��� ����
	void GetTerrainTriangle(int index, RVector &v0, RVector &v1, RVector &v2);

	virtual bool Pick(RCollisionPickInfo& pickInfo, DWORD dwAttributeFlag, bool bNoCull = false);	// picking
};
