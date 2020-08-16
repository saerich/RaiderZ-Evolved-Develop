#include "stdafx.h"
#include "RTypes.h"
#include "RSceneNode.h"
#include "ROctreeManager.h"


/*
	���� Cull�� �׽�Ʈ ���� ����

*/

/*	Coherent Cull�� �׽�Ʈ ���� ����

	- ������ �Ѱ��� ����� �ø�
	- ������ �Ѱ��� 2��� �ø�
	- ������ ������ �ø�
	- �������� ��ȯ ������ �ø�
	- ����� ��ġ�� ����� �� �ø�

	!�����Ѹ� :
				 �⺻���� �׽�Ʈ �ۿ� �̷�� ���� �ʾ���. 
				 ���� ���� �� �� �ִ� ��Ȳ�� ������, 
				 ���۴�� ���װ� ���������� �����׽�Ʈ�� �ٿ����� ���� �߰��� �׽�Ʈ�� �ϼ��Ǿ��� ����
*/

using namespace rs3;

//��ƿ��Ƽ
static void SetSceneNodeSphere(RSceneNode *pSceneNode,float x,float y,float z,float radius)
{
	pSceneNode->SetSphere(RSphere (RVector(x,y,z),radius));
}

//---------------------------------------------------------------------[ Octree Callback ]
class TestOctreeCallBack : public ROctreeCallBack
{
public:
	virtual void CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE state)
	{
		if( state == CS_PARTIAL || state == CS_INSIDE )
		{
			//�� �����ӿ����� ���̴�
			//�߰�
			m_CullingList.insert(pItem->GetSceneNode());
		}
		else if( state == CS_OUTSIDE )
		{
			//�� �����ӿ� �Ⱥ��̴� 
			//�߰� �����ش�. �� �ƹ��͵� ������
		}
		else
		{
			ADD_FAILURE();
		}

	}

	virtual void CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE before_state,CULL_STATE current_state) {};

public:
	bool	ExistInCullingList(RSceneNode* pSceneNode)	
	{ 
		if(	m_CullingList.end() == m_CullingList.find(pSceneNode) )
		{
			return false;
		}
		return true;
	}

	void	ResetCullingList(void)
	{
		m_CullingList.clear();
	}

public:
	std::set<RSceneNode*>	m_CullingList;
};

class TestOctreeMangerFixture : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		m_pOctreeManger = new ROctreeManager(&m_OctreeCallBack);
	}
	virtual void TearDown()
	{
		delete m_pOctreeManger;
	}

protected:
	ROctreeManager*					m_pOctreeManger;
	TestOctreeCallBack				m_OctreeCallBack;
};

//---------------------------------------------------------------------[ Coherent Octree Callback ]
class TestCoherentOctreeCallBack : public ROctreeCallBack
{
public:
	virtual void CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE state)	{};
	virtual void CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE before_state,CULL_STATE current_state)
	{
		CULL_STATE modified_state = CheckStateChangeForCoherentCulling(before_state,current_state);

		if( modified_state == CS_OUTSIDE )
		{
			//�� �����ӿ����� �����ٰ�, �� �����ӿ����� �Ⱥ��̴�
			//����
			ASSERT_TRUE( ExistInCullingList(pItem->GetSceneNode()) );
			m_CullingList.erase(pItem->GetSceneNode());
		}
		else if( modified_state == CS_PARTIAL || modified_state == CS_INSIDE )
		{
			//�� �����ӿ����� �Ⱥ����ٰ�, �� �����ӿ����� �Ⱥ��̴� (�� �����ӿ��� ���� ���Ե� �������� �� �� �ִ�.
			//�߰�
			ASSERT_FALSE( ExistInCullingList(pItem->GetSceneNode()) );
			m_CullingList.insert(pItem->GetSceneNode());
		}
		else 
		{
			ADD_FAILURE();
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

class TestCoherentOctreeMangerFixture : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		m_pOctreeManger = new ROctreeManager(&m_CoherentOctreeCallBack);
	}
	virtual void TearDown()
	{
		delete m_pOctreeManger;
	}

protected:
	ROctreeManager*					m_pOctreeManger;
	TestCoherentOctreeCallBack		m_CoherentOctreeCallBack;

};


TEST_F(TestOctreeMangerFixture,Cull)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull(&plane,1,&m_OctreeCallBack);

	EXPECT_EQ(true,m_OctreeCallBack.ExistInCullingList(&SceneNode));
}

TEST_F(TestOctreeMangerFixture,Move)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	ROctreeItem* pItem = m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull(&plane,1,&m_OctreeCallBack);
	EXPECT_EQ(true,m_OctreeCallBack.ExistInCullingList(&SceneNode));

	m_OctreeCallBack.ResetCullingList();

	m_pOctreeManger->Move(pItem,RSphere(RVector(-2,0,0),1));
	m_pOctreeManger->Cull_MC(&plane,1,&m_OctreeCallBack);
	EXPECT_EQ(false,m_OctreeCallBack.ExistInCullingList(&SceneNode));
}


TEST_F(TestCoherentOctreeMangerFixture,CoherentCull)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOctreeCallBack);

	EXPECT_EQ(true,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));
}

TEST_F(TestCoherentOctreeMangerFixture,CoherentCullByPlanes)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,-1,0,1);
	m_pOctreeManger->Add(&SceneNode);

	static const int PLANE_NUM = 2;
	RPlane planes[PLANE_NUM];
	planes[0].SetPlane(RVector(1,0,0),RVector(0,0,0));
	planes[1].SetPlane(RVector(0,1,0),RVector(0,1,0));
	
	m_pOctreeManger->Cull_MC(&planes[0],1,&m_CoherentOctreeCallBack);
	EXPECT_EQ(true,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));

	m_pOctreeManger->Cull_MC(planes,PLANE_NUM,&m_CoherentOctreeCallBack);
	EXPECT_EQ(false,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));
}

TEST_F(TestCoherentOctreeMangerFixture,MovingNode)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	ROctreeItem* pItem = m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOctreeCallBack);
	EXPECT_EQ(true,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));

	//����� ��ġ �����Ų��.
	m_pOctreeManger->Move(pItem,RSphere(RVector(-2,0,0),1));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOctreeCallBack);
	EXPECT_EQ(false,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));
}

TEST_F(TestCoherentOctreeMangerFixture,ResizingNodeRadius)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	ROctreeItem* pItem = m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(2,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOctreeCallBack);
	EXPECT_EQ(false,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));

	//������ ũ�� �÷��ش�
	m_pOctreeManger->Move(pItem,RSphere(RVector(0,0,0),2));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOctreeCallBack);
	EXPECT_EQ(true,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));

}

TEST_F(TestCoherentOctreeMangerFixture,MovingPlane)
{
	RSceneNode SceneNode;	
	SetSceneNodeSphere(&SceneNode,0,0,0,1);
	m_pOctreeManger->Add(&SceneNode);

	RPlane plane(RVector(1,0,0),RVector(0,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOctreeCallBack);
	EXPECT_EQ(true,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));

	plane.SetPlane(RVector(1,0,0),RVector(0.5,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOctreeCallBack);
	EXPECT_EQ(true,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));

	plane.SetPlane(RVector(1,0,0),RVector(2,0,0));
	m_pOctreeManger->Cull_MC(&plane,1,&m_CoherentOctreeCallBack);
	EXPECT_EQ(false,m_CoherentOctreeCallBack.ExistInCullingList(&SceneNode));
}

