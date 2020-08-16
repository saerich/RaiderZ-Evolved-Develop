#include "stdafx.h"
#include "RS3UnitTestUtility.h"
#include "REffectSceneNode.h"

using namespace rs3;

class TestEffectSceneNode : public NullDeviceFixture
{
};

class RTestEffectBase : public REffectBase
{
public:
	RTestEffectBase() : m_bLooping( false ) {}
	~RTestEffectBase() {}

	virtual void Start() override {};
	virtual void Stop() override {};
	virtual void Pause() override {};
	virtual bool IsAlive() override { return false; };

	virtual void SetLooping(bool loop) override { m_bLooping = loop; }
	virtual bool IsLooping() override { return m_bLooping; }

	virtual int GetCurrentCount() override { return 0; };

protected:
	bool m_bLooping;
};

TEST_F( TestEffectSceneNode, IsAlive )
{
	REffectSceneNode effectSceneNode;
	bool bAlive = effectSceneNode.IsAlive();
	EXPECT_FALSE( bAlive ) << "����Ʈ ������ IsAlive()�� false�� ��ȯ�ؾ� �Ѵ�."; 
}

TEST_F( TestEffectSceneNode, IsLoopingDefault )
{
	REffectSceneNode effectSceneNode;
	bool bLooping = effectSceneNode.IsLooping();
	EXPECT_FALSE( bLooping );

	// REffectSceneNode �ؿ� �ٸ� REffectSceneNode�� �ڽ����� �ٿ�����
	REffectSceneNode childEffectSceneNode;
	effectSceneNode.AddChild( &childEffectSceneNode );
	effectSceneNode.AddToSceneNodeGroup( &childEffectSceneNode );
	bLooping = effectSceneNode.IsLooping();
	EXPECT_FALSE( bLooping );

	// �׽�Ʈ�� ����Ʈ�� �ڽ����� �ٿ�����
	RTestEffectBase testEffectFirst;
	childEffectSceneNode.AddChild( &testEffectFirst );
	RTestEffectBase testEffectSecond;
	childEffectSceneNode.AddChild( &testEffectSecond );
	
	testEffectFirst.SetLooping( false );
	testEffectSecond.SetLooping( false );
	bLooping = effectSceneNode.IsLooping();
	EXPECT_FALSE( bLooping ) << "����Ʈ ������� �ڽĵ� �� ��� Loop �Ӽ��� �ƴϸ� false�� ��ȯ�ؾ� �Ѵ�.";

	testEffectFirst.SetLooping( true );
	testEffectSecond.SetLooping( false );
	bLooping = effectSceneNode.IsLooping();
	EXPECT_TRUE( bLooping ) << "����Ʈ ������� �ڽĵ� �� �ϳ��� Loop �Ӽ��� ������ true�� ��ȯ�ؾ� �Ѵ�.";

	testEffectFirst.SetLooping( true );
	testEffectSecond.SetLooping( true );
	bLooping = effectSceneNode.IsLooping();
	EXPECT_TRUE( bLooping ) << "����Ʈ ������� �ڽĵ� ��� Loop �Ӽ��� ������ true�� ��ȯ�ؾ� �Ѵ�.";

	testEffectFirst.RemoveFromParent();
	testEffectSecond.RemoveFromParent();
	childEffectSceneNode.RemoveFromParent();
}

TEST_F( TestEffectSceneNode, SetLooping )
{
	REffectSceneNode effectSceneNode;

	// REffectSceneNode �ؿ� �ٸ� REffectSceneNode�� �ڽ����� �ٿ�����
	REffectSceneNode childEffectSceneNode;
	effectSceneNode.AddChild( &childEffectSceneNode );

	// �׽�Ʈ�� ����Ʈ�� �ڽ����� �ٿ�����
	RTestEffectBase testEffectFirst;
	childEffectSceneNode.AddChild( &testEffectFirst );
	RTestEffectBase testEffectSecond;
	childEffectSceneNode.AddChild( &testEffectSecond );

	// REffectSceneNode���� SetLooping���ָ� �ڽĵ鵵 ��� Loop �Ӽ��� �ٲ��� �Ѵ�.
	effectSceneNode.SetLooping( false );
	EXPECT_FALSE( effectSceneNode.IsLooping() );
	EXPECT_FALSE( childEffectSceneNode.IsLooping() );
	EXPECT_FALSE( testEffectFirst.IsLooping() );
	EXPECT_FALSE( testEffectSecond.IsLooping() );

	effectSceneNode.SetLooping( true );
	EXPECT_TRUE( effectSceneNode.IsLooping() );
	EXPECT_TRUE( childEffectSceneNode.IsLooping() );
	EXPECT_TRUE( testEffectFirst.IsLooping() );
	EXPECT_TRUE( testEffectSecond.IsLooping() );

	testEffectFirst.RemoveFromParent();
	testEffectSecond.RemoveFromParent();
	childEffectSceneNode.RemoveFromParent();
}