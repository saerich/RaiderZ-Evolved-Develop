#include "stdafx.h"
#include "RS3UnitTestUtility.h"
#include "RMeshNode.h"
#include "RActor.h"
#include "RActorNodeConstraintController.h"
#include "RWorldSceneNode.h"
#include "RCameraSceneNode.h"

using namespace rs3;

class FakeRMeshNode : public RMeshNode
{
};

class TestRActorNode : public RActorNode
{
public:
	TestRActorNode( ACTOR_NODE_TYPE eActorNodeType ) : RActorNode( eActorNodeType )
	{
		m_pMeshNode = new FakeRMeshNode;
	}

	virtual ~TestRActorNode()
	{
		delete m_pMeshNode;
	}
};

class TestRActor : public RActor
{
public:
	using RActor::UpdateForRender;
	using RActor::AddActorNode;
	using RActor::CreateRenderInfoAndNotifyToController;
};

class TestActor : public NullDeviceFixture
{
public:
	virtual ~TestActor()
	{
		Clear();
	}

	TestRActor* CreateActor()
	{
		TestRActor* pActor = new TestRActor;
		_ASSERT( pActor );

		pActor->SetUsable( true );

		RBoundingBox aabbLocal( -1.f, -1.f, -1.f, 1.f, 1.f, 1.f );
		pActor->SetLocalAABB( &aabbLocal );

		m_actorList.push_back( pActor );
		return pActor;
	}

	TestRActorNode* CreateActorNode( const char* name )
	{
		TestRActorNode* pActorNode = new TestRActorNode( ACTOR_NODE_COMMON );
		pActorNode->SetNodeName( name );
		_ASSERT( pActorNode );
		return pActorNode;
	}

	bool AddActorNode( TestRActor* pActor, TestRActorNode* pActorNode, RSceneNode* pParent )
	{
		_ASSERT( pActor );
		_ASSERT( pActorNode );
		_ASSERT( pParent );

		pActor->AddActorNode( pActorNode, pParent );

		// Add ���� �� Create �ؾ� ��.
		bool bCreateSuccess = pActorNode->Create();

		// Add ���� �� ���� ��� ����� �籸������� ��.
		pActor->CreateRenderInfoAndNotifyToController();

		return bCreateSuccess;
	}

	TestRActorNode* CreateActorNodeAndAddToActor( TestRActor* pActor, const char* actorNodeName = "" )
	{
		_ASSERT( pActor );

		TestRActorNode* pActorNode = CreateActorNode( actorNodeName );
		bool bAddSuccess = AddActorNode( pActor, pActorNode, pActor );
		_ASSERT( bAddSuccess );
		
		return pActorNode;
	}

	void UpdateSceneNode( RSceneNode* pSceneNodeToUpdate )
	{
		_ASSERT( pSceneNodeToUpdate );

		RSceneNodeUpdateVisitor updateVisitor;
		if( updateVisitor.OnPreOrderVisit( pSceneNodeToUpdate ) )
			updateVisitor.OnPostOrderVisit( pSceneNodeToUpdate );
	}

private:
	void Clear()
	{
		for( size_t i = 0; i < m_actorList.size(); ++i )
		{
			delete m_actorList[i];
		}
		m_actorList.clear();
	}

	vector<TestRActor*> m_actorList;
};

class TestActorNodeConstraintController : public TestActor
{
public:
	void SetActorNodeConstraintController( RSceneNode* pParent, RSceneNode* pChild, const char* targetActorNodeName )
	{
		pParent->AddChild( pChild );

		RActorNodeConstraintController* pConstraintController = new RActorNodeConstraintController;
		pConstraintController->SetTargetActorNode( targetActorNodeName );
	//	pConstraintController->SetIgnoreActorNodeDirection( false );
		pChild->AddController( pConstraintController );
	}
};

TEST_F( TestActor, UpdateTransform )
{
	TestRActor* pTestActor = CreateActor();
	TestRActorNode* pTestActorNode = CreateActorNodeAndAddToActor( pTestActor );
	ASSERT_EQ( 1, pTestActor->GetActorNodeCount() );

	const RVector EXPECT_POS( 100.f, 200.f, 300.f );

	pTestActor->SetPosition( EXPECT_POS );

	UpdateSceneNode( pTestActor );
	pTestActor->UpdateForRender();

	EXPECT_EQ( EXPECT_POS, pTestActor->GetWorldPosition() ) << "������ ��ġ ���� ���ŵ��� �ʾҴ�.";
	EXPECT_EQ( EXPECT_POS, pTestActorNode->GetWorldPosition() ) << "���� ����� ��ġ ���� ���ŵ��� �ʾҴ�.";
}

TEST_F( TestActor, UpdateVisibility )
{
	TestRActor* pTestActor = CreateActor();
	TestRActorNode* pTestActorNode = CreateActorNodeAndAddToActor( pTestActor, "Bip01" );
	ASSERT_EQ( 1, pTestActor->GetActorNodeCount() );

	pTestActor->SetVisibility( 0.f );

	UpdateSceneNode( pTestActor );
	pTestActor->UpdateForRender();

	EXPECT_EQ( 0.f, pTestActor->GetVisibilityResult() ) << "������ Visibility Result ���� ���ŵ��� �ʾҴ�.";
	EXPECT_EQ( 0.f, pTestActorNode->GetVisibilityResult() ) << "���� ����� Visibility Result ���� ���ŵ��� �ʾҴ�.";
}

TEST_F( TestActor, UpdateNoiseRef )
{
	TestRActor* pTestActor = CreateActor();
	TestRActorNode* pTestActorNode = CreateActorNodeAndAddToActor( pTestActor, "Bip01" );
	ASSERT_EQ( 1, pTestActor->GetActorNodeCount() );

	pTestActor->SetNoiseRef( 0.f );

	UpdateSceneNode( pTestActor );
	pTestActor->UpdateForRender();

	EXPECT_EQ( 0.f, pTestActor->GetNoiseRefResult() ) << "������ NoiseRef Result ���� ���ŵ��� �ʾҴ�.";
	EXPECT_EQ( 0.f, pTestActorNode->GetNoiseRefResult() ) << "���� ����� NoiseRef Result ���� ���ŵ��� �ʾҴ�.";
}

TEST_F( TestActor, TestBoundingVolume_SetLocalAABB )
{
	// ��ġ
	const RBoundingBox LOCAL_AABB( -20.f, 0.f, 100.f, 0.f, 100.f, 101.f );
	EXPECT_TRUE( LOCAL_AABB.IsValid() );

	const RVector WORLD_POS( 10.f, -70.f, 500.f );
	RMatrix WORLD_MAT;
	WORLD_MAT.SetTranslationMatrix( WORLD_POS );
	
	RBoundingBox EXPECT_AABB;
	TransformBox( &EXPECT_AABB, LOCAL_AABB, WORLD_MAT );

	RSphere EXPECT_BOUNDING_SPHERE;
	EXPECT_BOUNDING_SPHERE.radius = EXPECT_AABB.GetRadius();
	EXPECT_BOUNDING_SPHERE.center = EXPECT_AABB.GetCenter();

	TestRActor* pTestActor = CreateActor();
	REngine::GetSceneManager().GetRootSceneNode().AddChild( pTestActor );

	pTestActor->SetTransform( WORLD_MAT );
	pTestActor->NeedUpdateTransform();
	pTestActor->UpdateTransform();
	
	// ����
	pTestActor->SetLocalAABB( &LOCAL_AABB );
	// ����
	EXPECT_FALSE( EXPECT_AABB.IsEqual( pTestActor->GetAABB() ) ) << "Actor�� AABB�� SceneManager Update ������ ������� �ʴ´�.";
	EXPECT_NE( EXPECT_AABB.GetRadius(), pTestActor->GetSphere().radius ) << "Actor�� Bounding Sphere�� SceneManager Update ������ ������� �ʴ´�.";
	EXPECT_NE( EXPECT_AABB.GetCenter(), pTestActor->GetSphere().center ) << "Actor�� Bounding Sphere�� SceneManager Update ������ ������� �ʴ´�.";

	// ����
	UpdateSceneManager();

	// ����
	EXPECT_TRUE( EXPECT_AABB.IsEqual( pTestActor->GetAABB() ) ) << "Actor�� AABB�� SceneManager Update �� �ùٸ��� ���ŵǾ�� �Ѵ�.";
	EXPECT_EQ( EXPECT_AABB.GetRadius(), pTestActor->GetSphere().radius ) << "Actor�� Bounding Sphere�� SceneManager Update �� ���� �ùٸ��� ���ŵǾ�� �Ѵ�.";
	EXPECT_EQ( EXPECT_AABB.GetCenter(), pTestActor->GetSphere().center ) << "Actor�� Bounding Sphere�� SceneManager Update �� ���� �ùٸ��� ���ŵǾ�� �Ѵ�.";

	pTestActor->RemoveFromParent();
}

TEST_F( TestActor, TestBoundingVolume_UpdateTraverse_ChildActor )
{
	// ��ġ
	const RVector WORLD_POS( 100.f, -220.f, 30.f );
	RMatrix MAT_WORLD;
	MAT_WORLD.SetTranslationMatrix( WORLD_POS );

	RBoundingBox LOCAL_AABB( RVector( -90.f, -180.f, 40.f ), RVector( 320.f, 530.f, 120.f ) );
	EXPECT_FALSE( RBoundingBox::INVALID.IsEqual( LOCAL_AABB ) );
	EXPECT_NE( 0, LOCAL_AABB.GetRadius() );
	EXPECT_NE( RVector::ZERO, LOCAL_AABB.GetCenter() );

	RBoundingBox EXPECT_AABB;
	TransformBox( &EXPECT_AABB, LOCAL_AABB, MAT_WORLD );

	TestRActor* pTestActor = CreateActor();
	EXPECT_TRUE( RBoundingBox::INVALID.IsEqual( pTestActor->GetAABB() ) ) << "Actor BoundingBox �ʱⰪ�� invalid���� �Ѵ�.";
	pTestActor->SetLocalAABB( &LOCAL_AABB );

	RSceneNode parentSceneNode;
	parentSceneNode.AddChild( pTestActor );

	REngine::GetSceneManager().GetRootSceneNode().AddChild( &parentSceneNode );

	// ����
	parentSceneNode.SetPosition( WORLD_POS );
	RSceneNodeUpdateVisitor v;
	parentSceneNode.Traverse( &v );
	UpdateSceneManager();

	// ����
	EXPECT_TRUE( EXPECT_AABB.IsEqual( pTestActor->GetAABB() ) ) << "�θ� Ʈ�������� ����Ǹ� �ڽ� AABB�� �ùٸ��� ���ŵǾ�� �Ѵ�.";
	EXPECT_EQ( EXPECT_AABB.GetRadius(), pTestActor->GetSphere().radius ) << "�θ� Ʈ�������� ����Ǹ� �ڽ� BoundingSphere�� �ùٸ��� ���ŵǾ�� �Ѵ�.";
	EXPECT_EQ( EXPECT_AABB.GetCenter(), pTestActor->GetSphere().center ) << "�θ� Ʈ�������� ����Ǹ� �ڽ� BoundingSphere�� �ùٸ��� ���ŵǾ�� �Ѵ�.";

	parentSceneNode.RemoveFromParent();
}

TEST_F( TestActorNodeConstraintController, UpdateTransformByActorNodeConstraintController )
{
	TestRActor* pTestActor = CreateActor();
	TestRActorNode* pTestActorNode = CreateActorNodeAndAddToActor( pTestActor, "TestTarget" );
	ASSERT_EQ( 1, pTestActor->GetActorNodeCount() );

	TestRActor* pTestActorToConnect = CreateActor();
	SetActorNodeConstraintController( pTestActor, pTestActorToConnect, pTestActorNode->GetNodeName().c_str() );

	const RVector EXPECT_POS( 100.f, 200.f, 300.f );

	pTestActor->SetPosition( EXPECT_POS );

	UpdateSceneNode( pTestActorToConnect );

	EXPECT_EQ( EXPECT_POS, pTestActorToConnect->GetWorldPosition() ) << "������ ��ġ ���� ���ŵ��� �ʾҴ�.";
}

TEST_F( TestActorNodeConstraintController, UpdateVisibilityByActorNodeConstraintController )
{
	TestRActor* pTestActor = CreateActor();
	TestRActorNode* pTestActorNode = CreateActorNodeAndAddToActor( pTestActor, "TestTarget" );
	ASSERT_EQ( 1, pTestActor->GetActorNodeCount() );

	TestRActor* pTestActorToConnect = CreateActor();
	SetActorNodeConstraintController( pTestActor, pTestActorToConnect, pTestActorNode->GetNodeName().c_str() );

	pTestActor->SetVisibility( 0.f );

	UpdateSceneNode( pTestActorToConnect );

	EXPECT_EQ( 0.f, pTestActorToConnect->GetVisibilityResult() ) << "������ Visibility Result ���� ���ŵ��� �ʾҴ�.";
}

TEST_F( TestActorNodeConstraintController, UpdateNoiseRefByActorNodeConstraintController )
{
	TestRActor* pTestActor = CreateActor();
	TestRActorNode* pTestActorNode = CreateActorNodeAndAddToActor( pTestActor, "TestTarget" );
	ASSERT_EQ( 1, pTestActor->GetActorNodeCount() );

	TestRActor* pTestActorToConnect = CreateActor();
	SetActorNodeConstraintController( pTestActor, pTestActorToConnect, pTestActorNode->GetNodeName().c_str() );

	pTestActor->SetNoiseRef( 0.f );

	UpdateSceneNode( pTestActorToConnect );

	EXPECT_EQ( 0.f, pTestActorToConnect->GetNoiseRefResult() ) << "������ NoiseRef Result ���� ���ŵ��� �ʾҴ�.";
}

TEST_F( TestActor, DistortionActorRenderingList )
{
	//Create test actor...
	TestRActor* pTestActor = CreateActor();
	pTestActor->SetDistortion(true);


	//Add test actor to root scenenode...
	RWorldSceneNode worldSceneNode;
	worldSceneNode.AddChild(pTestActor);


	//Cull, then add test actor to rendering list...
 	RCameraSceneNode camera;
	camera.GetViewFrustumCullingMgr()->Cull(&worldSceneNode, NULL, 0, true);


	//Test if test actor is added to distortion rendering list...
	SCENE_SET* pSet = camera.GetViewFrustumCullingMgr()->GetRenderingList(RRP_DISTORTION);
	EXPECT_EQ( true, pSet->find(pTestActor)!=pSet->end() ) << "�ְ� ���Ͱ� ������ ����Ʈ�� �߰����� �ʾ���.";
	

	//remove test actor from root scenenode...
	pTestActor->Clear();


	//Test if test actor is removed from distortion rendering list...
	pSet = camera.GetViewFrustumCullingMgr()->GetRenderingList(RRP_DISTORTION);
	EXPECT_EQ( true, pSet->find(pTestActor)==pSet->end() ) << "�ְ� ���Ͱ� ������ ����Ʈ���� ������ �ʾ���.";
}