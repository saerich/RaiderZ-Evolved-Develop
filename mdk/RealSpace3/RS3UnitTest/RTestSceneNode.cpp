#include "stdafx.h"
#include "RS3UnitTestUtility.h"
#include "RTypes.h"
#include "RSceneNode.h"
#include "RSceneManager.h"

using namespace rs3;

class TestSceneNode : public NullDeviceFixture
{
};

/// Ʈ������ ������Ʈ ���� �ʰ� AABB�� �����ص� Bounding Volume Sphere�� ����� ���ŵǴ��� �׽�Ʈ
TEST_F( TestSceneNode, TestBoundingVolume_SetAABB )
{
	// ��ġ
	RBoundingBox expectAABB( RVector( -90.f, -180.f, 40.f ), RVector( 320.f, 530.f, 120.f ) );
	EXPECT_FALSE( RBoundingBox::INVALID.IsEqual( expectAABB ) );
	EXPECT_NE( 0, expectAABB.GetRadius() );
	EXPECT_NE( RVector::ZERO, expectAABB.GetCenter() );

	RSceneNode sceneNode;
	REngine::GetSceneManager().GetRootSceneNode().AddChild( &sceneNode );
	EXPECT_TRUE( RBoundingBox::INVALID.IsEqual( sceneNode.GetAABB() ) ) << "SceneNode BoundingBox �ʱⰪ�� invalid���� �Ѵ�.";

	// ����
	sceneNode.SetAABB( expectAABB );
	// ����
	EXPECT_TRUE( expectAABB.IsEqual( sceneNode.GetAABB() ) );
	EXPECT_NE( expectAABB.GetRadius(), sceneNode.GetSphere().radius ) << "AABB�� ����Ǿ SceneManager Update ������ BoundingSphere���� ������ �ʴ´�.";
	EXPECT_NE( expectAABB.GetCenter(), sceneNode.GetSphere().center ) << "AABB�� ����Ǿ SceneManager Update ������ BoundingSphere���� ������ �ʴ´�.";

	// ����
	UpdateSceneManager();

	// ����
	EXPECT_TRUE( expectAABB.IsEqual( sceneNode.GetAABB() ) );
	EXPECT_EQ( expectAABB.GetRadius(), sceneNode.GetSphere().radius ) << "AABB�� ����Ǹ� SceneManager Update �� BoundingSphere���� �ùٸ��� ���ŵǾ�� �Ѵ�.";
	EXPECT_EQ( expectAABB.GetCenter(), sceneNode.GetSphere().center ) << "AABB�� ����Ǹ� SceneManager Update �� BoundingSphere���� �ùٸ��� ���ŵǾ�� �Ѵ�.";

	sceneNode.RemoveFromParent();
}