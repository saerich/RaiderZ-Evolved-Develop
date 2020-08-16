#include "stdafx.h"
#include "RSceneNode.h"
#include "REffectPassRenderer.h"

using namespace rs3;

class TestRSceneNode : public RSceneNode
{
public:
	void SetVisibilityResult( float fValue )
	{
		m_fVisibilityResult = fValue;
	}
};

class TestREffectPassRenderer : public REffectPassRenderer
{
public:
	using REffectPassRenderer::IsRenderable;
};

TEST( TestEffectPassRenderer, IsRenderable )
{
	TestREffectPassRenderer effectPassRenderer;
	TestRSceneNode sceneNode;
	
	{
		sceneNode.SetUsable( false );
		sceneNode.SetVisible( true );
		sceneNode.SetVisibilityResult( 1.f );
		
		bool bRenderable = effectPassRenderer.IsRenderable( &sceneNode );
		EXPECT_FALSE( bRenderable ) << "Usable �÷��װ� false�� ������ �ϸ� �ȵ˴ϴ�.";
	}
	{
		sceneNode.SetUsable( true );
		sceneNode.SetVisible( false );
		sceneNode.SetVisibilityResult( 1.f );

		bool bRenderable = effectPassRenderer.IsRenderable( &sceneNode );
		EXPECT_FALSE( bRenderable ) << "Visible �÷��װ� false�� ������ �ϸ� �ȵ˴ϴ�.";
	}
	{
		sceneNode.SetUsable( true );
		sceneNode.SetVisible( true );
		sceneNode.SetVisibilityResult( 0.f );

		bool bRenderable = effectPassRenderer.IsRenderable( &sceneNode );
		EXPECT_FALSE( bRenderable ) << "VisibilityResult�� 0�̸� ������ �ϸ� �ȵ˴ϴ�.";
	}
	{
		sceneNode.SetUsable( true );
		sceneNode.SetVisible( true );
		sceneNode.SetVisibilityResult( 0.5f );

		bool bRenderable = effectPassRenderer.IsRenderable( &sceneNode );
		EXPECT_TRUE( bRenderable );
	}
}