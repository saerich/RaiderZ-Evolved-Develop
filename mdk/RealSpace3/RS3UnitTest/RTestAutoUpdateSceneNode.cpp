#include "stdafx.h"
#include "RS3UnitTestUtility.h"
#include "REffectActor.h"
#include "REffectSceneNode.h"
#include "REmitter.h"
#include "RTrail.h"
#include "REnvironmentSet.h"
#include "RSkyManager.h"
#include "RWeatherEffect.h"
#include "RPointLightSceneNode.h"
#include "RSpotLightSceneNode.h"
#include "ROccluderSceneNode.h"

using namespace rs3;

/**
	�ڵ� ������Ʈ�� �Ǿ�� �ϴ� ������ �˻�
*/

class TestAutoUpdateSceneNode : public NullDeviceFixture
{
};

TEST_F( TestAutoUpdateSceneNode, CheckAutoUpdateSceneNodes )
{
	{
		REffectActor effectActor;
		EXPECT_TRUE( effectActor.IsAutoUpdate() );
	}
	{
		REffectSceneNode effectSceneNode;
		EXPECT_TRUE( effectSceneNode.IsAutoUpdate() );
	}
	{
		REmitter emitter;
		EXPECT_TRUE( emitter.IsAutoUpdate() );
	}
	{
		RTrail trail;
		EXPECT_TRUE( trail.IsAutoUpdate() );
	}
	{
		RSkyManager skyManager;
		EXPECT_TRUE( skyManager.IsAutoUpdate() );
	}
	{
		ROccluderSceneNode OccluderSceneNode;
		EXPECT_TRUE( OccluderSceneNode.IsAutoUpdate() );
	}
	
	// ���������Ʈ�� �⺻������ false�� ��.
	{
		REnvironmentSet environmentSet;
		EXPECT_FALSE( environmentSet.IsAutoUpdate() );
	}
	{
		RWeatherEffect weatherEffect;
		EXPECT_FALSE( weatherEffect.IsAutoUpdate() );
	}
}