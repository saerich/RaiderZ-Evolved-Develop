#include "stdafx.h"
#include "RTypes.h"
#include "RSceneNode.h"
#include "ROctreeManager.h"

using namespace rs3;

//��ƿ��Ƽ
static void SetSceneNodeSphere(RSceneNode *pSceneNode,float x,float y,float z,float radius)
{
	pSceneNode->SetSphere(RSphere (RVector(x,y,z),radius));
}

//---------------------------------------------------------------------[ Coherent Octree Callback ]
class TestCoherentOcclusionCallBack : public ROctreeCallBack
{
public:
	virtual void CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE state)	{};
	virtual void CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE before_state,CULL_STATE current_state)
	{
		CULL_STATE modified_state = CheckStateChangeForOcclusionCoherentCulling(before_state,current_state);

		if( modified_state == CS_OUTSIDE || modified_state == CS_PARTIAL ) 
		{
			//�� �����ӿ����� �����ٰ�, �� �����ӿ����� �Ⱥ��̴�
			//����
			ASSERT_TRUE( ExistInCullingList(pItem->GetSceneNode()) );
			m_CullingList.erase(pItem->GetSceneNode());
		}
		else if(  modified_state == CS_INSIDE )
		{
			//�� �����ӿ����� �Ⱥ����ٰ�, �� �����ӿ����� �Ⱥ��̴� (�� �����ӿ��� ���� ���Ե� �������� �� �� �ִ�.
			//�߰�
			ASSERT_FALSE( ExistInCullingList(pItem->GetSceneNode()) );
			m_CullingList.insert(pItem->GetSceneNode());
		}

	}
public:
	bool	ExistInCullingList(RSceneNode* pSceneNode)	
	{ 
		if(	m_CullingList.end() == m_CullingList.find(pSceneNode) )
		{
			return false;
		}
		return true;
	}

public:
	std::set<RSceneNode*>	m_CullingList;
};

class TestCoherentOcclusionCullingMangerFixture : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		m_pOctreeManger = new ROctreeManager(&m_CoherentOcclusionCallBack);
	}
	virtual void TearDown()
	{
		delete m_pOctreeManger;
	}

protected:
	ROctreeManager*						m_pOctreeManger;
	TestCoherentOcclusionCallBack		m_CoherentOcclusionCallBack;

};

TEST_F(TestCoherentOcclusionCullingMangerFixture,Culling_Inside)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,3,0,0,1);
	m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);

	EXPECT_EQ(true,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));
}


TEST_F(TestCoherentOcclusionCullingMangerFixture,Culling_Partial)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);

	EXPECT_EQ(false,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));
}

TEST_F(TestCoherentOcclusionCullingMangerFixture,Culling_Outside)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,-3,0,0,1);
	m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);

	EXPECT_EQ(false,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));
}

TEST_F(TestCoherentOcclusionCullingMangerFixture,MovingNode)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,3,0,0,1);
	ROctreeItem* pItem = m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);

	EXPECT_EQ(true,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

	//partial ������ ���� ����� ��ġ �����Ų��.
	m_pOctreeManger->Move(pItem,RSphere(RVector(0,0,0),1));;
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(false,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

	//outside ������ ���� ����� ��ġ �����Ų��.
	m_pOctreeManger->Move(pItem,RSphere(RVector(-3,0,0),1));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(false,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

}

TEST_F(TestCoherentOcclusionCullingMangerFixture,ResizingNode)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,3,0,0,1);
	ROctreeItem* pItem = m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);

	EXPECT_EQ(true,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

	//������ ũ�� �÷��ش�
	m_pOctreeManger->Move(pItem,RSphere(RVector(3,0,0),2));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(true,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

	//������ ũ�� �÷��ش�
	m_pOctreeManger->Move(pItem,RSphere(RVector(3,0,0),4));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(false,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

}

TEST_F(TestCoherentOcclusionCullingMangerFixture,MovingPlane)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(false,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

	plane.SetPlane(RVector(1,0,0),RVector(-2,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(true,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

	plane.SetPlane(RVector(1,0,0),RVector(2,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(false,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));
}

TEST_F(TestCoherentOcclusionCullingMangerFixture,CoherentCullByPlanes)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	ROctreeItem* pItem = m_pOctreeManger->Add(&SceneNode);

	static const int PLANE_NUM = 2;
	RPlane planes[PLANE_NUM];
	planes[0].SetPlane(RVector(1,0,0),RVector(-2,0,0));
	planes[1].SetPlane(RVector(0,1,0),RVector(0,-2,0));

	m_pOctreeManger->Cull_MC(planes,PLANE_NUM,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(true,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

	//����� ��ġ �����Ų��.
	m_pOctreeManger->Move(pItem,RSphere(RVector(-1,0,0),1));
	m_pOctreeManger->Cull_MC(planes,PLANE_NUM,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(true,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

	//����� ��ġ �����Ų��.
	m_pOctreeManger->Move(pItem,RSphere(RVector(-2,0,0),1));
	m_pOctreeManger->Cull_MC(planes,PLANE_NUM,&m_CoherentOcclusionCallBack);
	EXPECT_EQ(false,m_CoherentOcclusionCallBack.ExistInCullingList(&SceneNode));

}