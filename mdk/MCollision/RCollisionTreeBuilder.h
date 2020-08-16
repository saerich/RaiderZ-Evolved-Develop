#pragma once

#include "RLib.h"
#include "RCollisionTree.h"

namespace rs3 {

// �浹�� AABB Ʈ�� builder

////////////////////////////////////////////////////////////////////////////////////////////////
// RCollisionTreeBuilder

class RCollisionTreeBuilder;
class RCollisionTreeBuilderNode
{
	friend RCollisionTreeBuilder;
	RCollisionTreeBuilder* m_pBuilder;

	MBox m_bb;
	RCollisionTreeBuilderNode *m_pLeft,*m_pRight;
	vector<RCollisionTriangle*> m_polygons;
	int m_nIndex;
	static int m_nMaxPolygonPerNode;

	RCollisionTreeBuilderNode();
	RCollisionTreeBuilderNode(RCollisionTreeBuilder* pBuilder);
	~RCollisionTreeBuilderNode();

	void Build();

	int	GetNodeCount();
	int GetPolygonCount();

	bool IsLeafNode() { return m_pLeft==NULL; }
};

const int POLYGON_COUNT_BOUND = 10;

class RCollisionTreeBuilder
{
public:
	int m_nTotalPoly;
	int m_nCurrentPoly;

protected:
	bool	m_bIncludeTerrain;
	int m_nSavingTriangleIndex;
	RCollisionTreeBuilderNode* m_rootNode;

	bool Save(FILE *file, RCollisionTreeBuilderNode* pNode);
	bool SaveXml(const char *szFilename);

	void CopyToCollisionTree(RCollisionTree *pCollisionTree, RCollisionTreeNode *pCNode, RCollisionTreeBuilderNode *pBNode);

public:
	// material table
	std::vector<std::string> m_vecMaterialList;
	// �̸��� ��ġ�Ǵ� Material Table�� �ε����� �����ش�.
	DWORD GetZoneMaterialID(const std::string &strMaterialName);
	// ������ ���ϵ��� �̸��� �����Ѵ�.
	std::vector<std::string> m_vecFailedList;

public:
	RCollisionTreeBuilder(void);
	~RCollisionTreeBuilder(void);

	// Builder �� ������� RCollisionTree�� �����Ѵ�.
	RCollisionTree* CreateCollisionTree();

	// .cl2 �� .cl2.xml (material table)�� �����Ѵ�.
	bool Save(const char* szFileName);
//	void SetIncludeTerrain(bool bInclude);

	// �������� AddPolygon �� BuildTree �Ҽ��ְ�,
	void AddPolygon(const MVector3& v0, const MVector3& v1, const MVector3& v2, WORD wNodeFlag, WORD wUserFlag, DWORD dwMaterial);
	
	void BuildTree(int nMaxPolygonPerNode = POLYGON_COUNT_BOUND);
	int  GetNumOfTreeToBuild(){ return (int)m_rootNode->m_polygons.size(); }

	const MBox& GetRootAABB();

	// MaxPlugin Export �� �۾� ������� �˷��ֱ� ���� �Լ�
	virtual void OnUpdateProgress(int, int);
};

}
