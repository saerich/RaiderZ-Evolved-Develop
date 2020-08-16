#pragma once

#include "RLib.h"
#include "MFileBuffer.h"


namespace rs3 {

class RCollision;

// �浹�� AABB Ʈ��
struct RCollisionTriangle
{
	enum ATTRIBUTE	// Bit flag, ���� 31�� ���� ����
	{
		NONE	= 0,
		WATER	= 0x01,
		MESH	= 0x02,
		TREE	= 0x04,
		TERRAIN = 0x08,
		// ...
		ALL		= 0xffff
	};
	RCollisionTriangle()
		: dwNodeAttribute(NONE)
		, dwUserMaterial(0)
	{ }

	MVector3 v[3];
	DWORD dwNodeAttribute;	// ��Ʈ�÷��׸� ����ϴ� �Ӽ�
	DWORD dwUserMaterial;	// ����� ����(unsigned 32 ��Ʈ ID)
};

////////////////////////////////////////////////////////////////////////////////////////////////
// RCollisionTree
class RCollisionTree;
class RCollisionTreeBuilder;

class RCollisionTreeImpl;
class RCollisionTreeImpl_v0;
class RCollisionTreeImpl_v1;

class RCollisionTreeNode
{
	friend RCollisionTree;
	friend RCollisionTreeBuilder;
	friend RCollisionTreeImpl;
	friend RCollisionTreeImpl_v0;
	friend RCollisionTreeImpl_v1;

protected:
	MBox m_bb;
	RCollisionTreeNode *m_pLeft,*m_pRight;
	int m_nPolygonCount;
	RCollisionTriangle *m_pTriangles;

public:
	const RCollisionTriangle&	GetTriangles(int nIndex) const	{ return m_pTriangles[nIndex];}
	const MBox&			GetBoundBox() const				{ return m_bb; }

	int							GetPolygonCount() const	{ return m_nPolygonCount; }
	bool						IsLeafNode() const		{ return m_pLeft==NULL; }

	const RCollisionTreeNode*	GetLeftNode() const		{ return m_pLeft; }
	const RCollisionTreeNode*	GetRightNode() const	{ return m_pRight; }

};

class RCollisionPickInfo	// picking�� ȣ���Ҷ� ���Ǵ� ����ü
{
private:
	friend RCollisionTree;
	MVector3 m_tempPickDir;		// ���ο��� ����
	float	m_tempToDistance;

public:
	MVector3	m_inPickOrigin;		// �Է� : pick�� ������.		-> ���� �ΰ��� ä���ְ� ȣ��
	MVector3 m_inPickTo;			// �Է� : pick�� ����.
	MVector3 m_outPosition;		// ��ȯ : pick�� ����			-> ���ϰ��� �Ʒ� �ΰ�
	MVector3 m_outNormal;		// ��ȯ : pick�� ������ normal
	float	m_outDistanceSq;	// ��ȯ : PickOrigin������ �Ÿ��� ����
	WORD	m_outNodeAttr;		// ��ȯ : Picking Face�� �Ӽ� (WORD , RCollisionTriangle�� enum ���� ����) - LoWord
	WORD	m_outUserAttr;		// ��ȯ : User�� ������ �Ӽ� (WORD) - HiWord
	DWORD	m_outMaterial;		// ��ȯ : Picking Face�� ����
	const char*	m_outMaterialName;	// ��ȯ : Material�� ���� �̸�, ã�� �� ������ NULL

	float GetPickDistance() { return m_tempToDistance; }	// pick ����~ ���Ÿ�
};

// �б� ����
class RCollisionTree
{
	friend RCollisionTreeBuilder;
	friend RCollisionTreeImpl;
	friend RCollisionTreeImpl_v0;
	friend RCollisionTreeImpl_v1;

private:
	// �ε��� ���� �ӽ÷� �ʿ��� ���
	RCollisionTriangle *m_pLoadingTriangle;
	RCollisionTreeNode *m_pLoadingNode;

	int					m_nTriangles;
	RCollisionTriangle *m_pTriangles;
	int					m_nNodeCount;
	RCollisionTreeNode *m_pRootNode;

	std::string		m_strCurrentFile;

	// ���� �̸�/ID ���̺� ( .cl2.xml ���� �ε� )
	std::vector<std::string> m_vecMaterialTable;

	/// Picking ���� �Լ�, �ﰢ�� ����� ������ �����ϴ��� �����ϴ��� ...
	void PickTriangle(RCollisionTreeNode *pNode, RCollisionPickInfo &pickInfo, DWORD dwAttributeMask
		, bool (*IntersectFunc)(const MVector3&, const MVector3&, MVector3&, MVector3&, MVector3&, MVector3*) );

public:
	// callback interface
	class ICallback
	{
	public:
		virtual void OnNode(const RCollisionTreeNode *pNode) = 0;
		virtual void OnTriangle(const RCollisionTriangle *pTriangle) = 0;
	};

public:
	RCollisionTree(void);
	~RCollisionTree(void);

	void Destroy();	// �ʱ�ȭ

	bool Load(const char* szFileName);

	/// ������ �ٿ���� ���մϴ�. ( load�Ǿ��־�߸� �մϴ� )
	const MBox& GetBoundingBox();

	/// �浹�� �˻��Ѵ�. pickInfo.origin - pickInfo.to �� ���±濡 �浹�ϸ� true�� ����.
	virtual bool Pick(RCollisionPickInfo& pickInfo, DWORD dwAttributeMask , bool bNoCull);

	/// collision �� this�� ���������� �浹������ �������� �����ϴ�
	virtual void GetCollidablePolygons(RCollision &collision, DWORD dwAttributeMask);

	/// ����׿� AABB �ݹ� �Լ�
	void CallbackTree(ICallback *piCallback);

	RCollisionTriangle* GetTriagles(){return m_pTriangles;}
	int					GetTriagleCount(){return m_nTriangles;}

	// root ���� ����
	const RCollisionTreeNode*	GetRootNode(){ return m_pRootNode; }
	const MBox&			GetRootAABB();

	const std::string&			GetMaterialName(DWORD dwID);

protected:
	void CallBackTreeNode(RCollisionTreeNode *pNode, ICallback *piCallback);
	void GetCollidablePolygons(RCollisionTreeNode *pNode,RCollision &collision, DWORD dwAttributeMask);
};

inline 	const MBox& RCollisionTree::GetBoundingBox() { return m_pRootNode->m_bb; }
inline	const std::string& RCollisionTree::GetMaterialName(DWORD dwID)
{
	return m_vecMaterialTable[dwID];
}

}
