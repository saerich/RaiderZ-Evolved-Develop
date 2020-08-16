#ifndef _GCOLLISIONTREE_H_
#define _GCOLLISIONTREE_H_

#include "RCollisionTree.h"
#include "MBox.h"
#include "MPathFinder.h"

/**
 RCollisionTree �� RHeightField�� �Ἥ ��ü ������ Picking �ϰ� �����س��� Class
 �̸��� ����ϰ� �������ϴ�. (ȥ������ �ʵ��� �ϱ� ����)
*/

class RHeightField;

class GCollisionTree : public MTestMemPool<GCollisionTree>
{
private:
	rs3::RCollisionTree *m_pCollisionTree;
	RHeightField *m_pHeightField;

	MBox m_aabb;
	int m_nZoneID;

public:
	GCollisionTree();
	virtual ~GCollisionTree();

	void Destroy();

	/// ������ �ٿ���� ���մϴ�. ( load�Ǿ��־�߸� �մϴ� )
	const MBox& GetBoundingBox();

	/// zone name �� �̿��Ͽ�, cl2�� .xml(�������������� ��� ����), _dis.dds �� �о���δ�.
	bool LoadZone(int nZoneID, const wstring& strZoneFileName);
	bool Pick(rs3::RCollisionPickInfo& pickInfo, bool bNoCull = false, DWORD dwAttributeMask = rs3::RCollisionTriangle::ALL);
	RHeightField* GetHeightField();
};

#endif