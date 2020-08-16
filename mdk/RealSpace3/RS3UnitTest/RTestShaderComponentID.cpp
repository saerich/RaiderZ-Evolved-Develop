#include "stdafx.h"
#include "RShaderComponentID.h"
#include "RMaterial.h"
#include "RShaderCompositeManager.h"

using namespace rs3;


TEST(TestShaderComponentID, TestHasComponent_HaveNoSameID_ShouldFalse)
{
	//��ġ
	RSCID testID = RSC_POSITION | RSC_TEXCOORD | RSC_DIFFUSEMAP;
	RSCID comparedID = RSC_TRANSFORM;


	//����
	bool bResult = testID.HasComponent(comparedID);


	//����
	EXPECT_FALSE(bResult);
}

TEST(TestShaderComponentID, TestHasComponent_HaveOneSameID_ShouldTrue)
{
	//��ġ
	RSCID testID = RSC_POSITION | RSC_TEXCOORD | RSC_DIFFUSEMAP;
	RSCID comparedID = RSC_POSITION;


	//����
	bool bResult = testID.HasComponent(comparedID);


	//����
	EXPECT_TRUE(bResult);
}

TEST(TestShaderComponentID, TestHasComponent_HaveSeveralSameID_ShouldTrue)
{
	//��ġ
	RSCID testID = RSC_POSITION | RSC_TEXCOORD | RSC_DIFFUSEMAP;
	RSCID comparedID = RSC_POSITION | RSC_TEXCOORD;


	//����
	bool bResult = testID.HasComponent(comparedID);


	//����
	EXPECT_TRUE(bResult);
}

TEST(TestShaderComponentID, TestHasComponent_HaveOneSameIDAndOneOtherID_ShouldTrue)
{
	//��ġ
	RSCID testID = RSC_POSITION | RSC_TEXCOORD | RSC_DIFFUSEMAP;
	RSCID comparedID = RSC_POSITION | RSC_TRANSFORM;


	//����
	bool bResult = testID.HasComponent(comparedID);


	//����
	EXPECT_TRUE(bResult);
}










TEST(TestShaderComponentID, TestHasAllComponentt_HaveNoSameID_ShouldFalse)
{
	//��ġ
	RSCID testID = RSC_POSITION | RSC_TEXCOORD | RSC_DIFFUSEMAP;
	RSCID comparedID = RSC_TRANSFORM;


	//����
	bool bResult = testID.HasAllComponent(comparedID);


	//����
	EXPECT_FALSE(bResult);
}

TEST(TestShaderComponentID, TestHasAllComponent_HaveOneSameID_ShouldTrue)
{
	//��ġ
	RSCID testID = RSC_POSITION | RSC_TEXCOORD | RSC_DIFFUSEMAP;
	RSCID comparedID = RSC_POSITION;


	//����
	bool bResult = testID.HasAllComponent(comparedID);


	//����
	EXPECT_TRUE(bResult);
}

TEST(TestShaderComponentID, TestHasAllComponent_HaveSeveralSameID_ShouldTrue)
{
	//��ġ
	RSCID testID = RSC_POSITION | RSC_TEXCOORD | RSC_DIFFUSEMAP;
	RSCID comparedID = RSC_POSITION | RSC_TEXCOORD;


	//����
	bool bResult = testID.HasAllComponent(comparedID);


	//����
	EXPECT_TRUE(bResult);
}

TEST(TestShaderComponentID, TestHasAllComponent_HaveOneSameIDAndOneOtherID_ShouldFalse)
{
	//��ġ
	RSCID testID = RSC_POSITION | RSC_TEXCOORD | RSC_DIFFUSEMAP;
	RSCID comparedID = RSC_POSITION | RSC_TRANSFORM;


	//����
	bool bResult = testID.HasAllComponent(comparedID);


	//����
	EXPECT_FALSE(bResult);
}