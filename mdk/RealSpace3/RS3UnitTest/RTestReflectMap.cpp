#include "stdafx.h"
#include "RMaterial.h"
#include "RShaderCompositeManager.h"
#include "RActor.h"
#include "RSceneManager.h"
using namespace rs3;



TEST( TestReflectMap, TestApplyReflectMap_HasReflect_ShouldHasReflect )
{
	//��ġ
	RSCID ReflectRC;
	RMaterial mtrl;
	RShaderCompositeManager manager;

	//����
	manager.AddReflectRSC( ReflectRC );
	RSCID ResultSC = ReflectRC;

	//����
	EXPECT_TRUE( ResultSC.HasComponent(RSC_REFLECTMAP) );
	EXPECT_TRUE( ResultSC.HasComponent(RSC_PPL_REFLECT) );
}


// Ʈ������ �׽�Ʈ, 
TEST( TestReflectMap, TestApplyReflectMap_HasTransform_ShouldHasTransform )
{
	//��ġ
	RSCID ReflectRC;
	RMaterial mtrl;
	RShaderCompositeManager manager;

	//����
	manager.ApplyTransformComponent( ReflectRC, false, true );
	RSCID ResultSC = ReflectRC;

	//����
	EXPECT_TRUE( ResultSC.HasComponent(RSC_TRANSFORM_CONSTANT) );
	EXPECT_TRUE( ResultSC.HasComponent(RSC_TRANSFORM) );
	EXPECT_TRUE( ResultSC.HasComponent(RSC_NORMAL) );
	EXPECT_TRUE( ResultSC.HasComponent(RSC_POSITION) );
}


// Texcoord Tranform �׽�Ʈ
TEST( TestReflectMap, TestApplyReflectMap_HasTexcoordTransform_ShouldHasTexcoordTransform )
{
	//��ġ
	RSCID ReflectRC;
	RMaterial mtrl;
	RShaderCompositeManager manager;

	mtrl.SetFlag(RM_FLAG_TEXTURE_TRANSFORM);
	//����
	manager.ApplyTexcoordComponent( ReflectRC, &mtrl, true, false );
	RSCID ResultSC = ReflectRC;

	//����
	EXPECT_TRUE( ResultSC.HasComponent(RSC_TEXCOORD_TRANSFORM) );
}


// Texcoord �׽�Ʈ
TEST( TestReflectMap, TestApplyReflectMap_HasTexcoord_ShouldHasTexcoord )
{
	//��ġ
	RSCID ReflectRC;
	RMaterial mtrl;
	RShaderCompositeManager manager;

	//����
	manager.ApplyTexcoordComponent( ReflectRC, &mtrl, true, false );
	RSCID ResultSC = ReflectRC;

	//����
	EXPECT_TRUE( ResultSC.HasComponent(RSC_TEXCOORD) );
}


// �븻�� �׽�Ʈ
TEST( TestReflectMap, TestApplyReflectMap_HasNormalMap_ShouldHasNormalMap )
{
	//��ġ
	RSCID ReflectRC;
	RMaterial mtrl;
	RShaderCompositeManager manager;

	mtrl.GetTextureInfo(0,0)->SetTextureType(RTT_NORMAL);

	//����
	manager.ApplyNormalMapComponent( ReflectRC, &mtrl, true );
	RSCID ResultSC = ReflectRC;

	//����
	EXPECT_TRUE( ResultSC.HasComponent(RSC_PPL_G_NORMALMAP) );
}


// ����ŧ�� �� �׽�Ʈ
TEST( TestReflectMap, TestApplyReflectMap_HasSpecularMap_ShouldHasSpecularMap )
{
	//��ġ
	RSCID ReflectRC;
	RMaterial mtrl;
	RShaderCompositeManager manager;

	mtrl.GetTextureInfo(0,0)->SetTextureType(RTT_SPECULAR);

	//����
	ReflectRC = manager.GetTextureLayerReadComponent( &mtrl, RTT_SPECULAR );
	RSCID ResultSC = ReflectRC;

	//����
	EXPECT_TRUE( ResultSC.HasComponent(RSC_SPECULARMAP) );
}