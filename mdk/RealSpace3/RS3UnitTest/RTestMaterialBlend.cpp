#include "stdafx.h"
#include "RMaterial.h"
#include "RShaderCompositeManager.h"
#include "RActor.h"

using namespace rs3;


class MockMaterialWithDiffuseTexture : public RMaterial
{
public:
	MockMaterialWithDiffuseTexture() : RMaterial()
	{
		MakeDiffuseTextureInfo();
	}

	virtual ~MockMaterialWithDiffuseTexture()
	{
	}

	void MakeDiffuseTextureInfo()
	{
		CreateTexInfo( 1);
		RMtrlTexInfo MtrlTexInfo;
		MtrlTexInfo.SetTextureType(RTT_DIFFUSE);

		AddMtrlTexInfo(0, MtrlTexInfo);
	}
};

//							�׽�Ʈ �ϴ� ���	  _ ������    _    ��� �����
TEST( TestMaterialBlend, TestApplyDiffuseComponent_NoDiffuseSC_ShouldNotThrowDiffuseMapBlendSC)
{
	//��ġ
	RSCID NoDiffuseSC;
	RMaterial mtrl;
	RShaderCompositeManager manager;

	//����
	manager.ApplyDiffuseComponent( NoDiffuseSC, &mtrl, true, true);
	RSCID ResultSC = NoDiffuseSC;

	//����
	EXPECT_FALSE( ResultSC.HasComponent(RSC_DIFFUSEMAP_BLEND) );
}

TEST( TestMaterialBlend,  TestApplyDiffuseComponent_NoTextureCoordSC_ShouldNotThrowDiffuseMapBlendSC)
{
	//��ġ
	RSCID NoTextureCoordSC;
	RMaterial mtrl;
	RShaderCompositeManager manager;

	//����
	manager.ApplyDiffuseComponent( NoTextureCoordSC, &mtrl, true, false);
	RSCID ResultSC = NoTextureCoordSC;

	//����
	EXPECT_FALSE( ResultSC.HasComponent(RSC_DIFFUSEMAP_BLEND) );
}


TEST( TestMaterialBlend, TestApplyDiffuseComponent_MaterialWithDiffuseTexture_ShouldThrowDiffuseMapBlendSC)
{
	//��ġ
	RSCID ShaderComponent;
	MockMaterialWithDiffuseTexture MaterialWithDiffuseTexture;
	RShaderCompositeManager manager;

	//����
	manager.ApplyDiffuseComponent( ShaderComponent, &MaterialWithDiffuseTexture, true, true);

	//����
	EXPECT_TRUE( ShaderComponent.HasComponent(RSC_DIFFUSEMAP_BLEND) );
}


