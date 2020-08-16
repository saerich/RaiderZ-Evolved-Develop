#include "stdafx.h"
#include "RMaterial.h"
#include "RShaderCompositeManager.h"
#include "RActor.h"

using namespace rs3;


TEST( TestMaterialDecal, TestShaderComponent_NoTexcoordDiffuse_DoNothing)
{
	//��ġ
	RSCID NoTexcoordDiffuseID;
	RSCID ExpectedID = NoTexcoordDiffuseID;
	RShaderCompositeManager manager;


	//����		
	manager.ApplyMaterialDecalComponents(NoTexcoordDiffuseID);	
	RSCID ResultID = NoTexcoordDiffuseID;
	

	//����
	EXPECT_TRUE(ResultID == ExpectedID);
}

TEST( TestMaterialDecal, TestShaderComponent_NoTexcoord_DoNothing)
{
	//��ġ
	RSCID NoTexcoordID = RSC_DIFFUSEMAP;
	RSCID ExpectedID = NoTexcoordID;
	RShaderCompositeManager manager;


	//����	
	manager.ApplyMaterialDecalComponents(NoTexcoordID);	
	RSCID ResultID = NoTexcoordID;


	//����
	EXPECT_TRUE(ResultID == ExpectedID);
}

TEST( TestMaterialDecal, TestShaderComponent_NoDiffuse_DoNothing)
{
	//��ġ
	RShaderCompositeManager manager;
	RSCID NoDiffuseID = manager.GetVertexTexcoordComponent();
	RSCID ExpectedID = NoDiffuseID;	


	//����		
	manager.ApplyMaterialDecalComponents(NoDiffuseID);	
	RSCID ResultID = NoDiffuseID;


	//����
	EXPECT_TRUE(ResultID == ExpectedID);
}

TEST( TestMaterialDecal, TestShaderComponent_HaveDependentComponent_ShouldAddMaterialDecalComponent)
{
	//��ġ
	RShaderCompositeManager manager;
	RSCID DependentID = manager.GetMaterialDecalDependentComponent();


	//����	
	manager.ApplyMaterialDecalComponents(DependentID);
	RSCID ResultID = DependentID;


	//����
	EXPECT_TRUE( ResultID.HasAllComponent(manager.GetMaterialDecalComponent()) );
}

TEST( TestMaterialDecal, TestShaderComponent_DependentAndIncompatibleComponent_ShouldAddMaterialDecalComponentAndRemoveIncompatibleComponent)
{
	//��ġ
	RShaderCompositeManager manager;
	RSCID DependentAndIncompatibleID = manager.GetMaterialDecalDependentComponent() | manager.GetIncompatibleComponentWithMaterialDecal();


	//����	
	manager.ApplyMaterialDecalComponents(DependentAndIncompatibleID);
	RSCID ResultID = DependentAndIncompatibleID;


	//����
	EXPECT_TRUE( ResultID.HasAllComponent(manager.GetMaterialDecalComponent()) );
	EXPECT_FALSE( ResultID.HasComponent(manager.GetIncompatibleComponentWithMaterialDecal()) );
}










class TestDescContainer
{
public:
	TestDescContainer()
	{
		m_Desc[0].nMapChannel =0;
		m_Desc[0].UVTransform.SetScale(RVector2(2.0f, 3.0f));
		m_Desc[0].UVTransform.SetOffset(RVector2(0.1f, 0.2f));
		m_Desc[0].textureAddressU =RTADDRESS_WRAP;
		m_Desc[0].textureAddressV =RTADDRESS_WRAP;

		m_Desc[1].nMapChannel =0;
		m_Desc[1].UVTransform.SetScale(RVector2(3.0f, 4.0f));
		m_Desc[1].UVTransform.SetOffset(RVector2(0.3f, 0.4f));
		m_Desc[1].textureAddressU =RTADDRESS_MIRROR;
		m_Desc[1].textureAddressV =RTADDRESS_MIRROR;

		m_Desc[2].nMapChannel =1;
		m_Desc[2].UVTransform.SetScale(RVector2(4.0f, 5.0f));
		m_Desc[2].UVTransform.SetOffset(RVector2(0.5f, 0.6f));
		m_Desc[2].textureAddressU =RTADDRESS_CLAMP;
		m_Desc[2].textureAddressV =RTADDRESS_CLAMP;

		m_Desc[3].nMapChannel =1;
		m_Desc[3].UVTransform.SetScale(RVector2(5.0f, 6.0f));
		m_Desc[3].UVTransform.SetOffset(RVector2(0.7f, 0.8f));
		m_Desc[3].textureAddressU =RTADDRESS_BORDER;
		m_Desc[3].textureAddressV =RTADDRESS_BORDER;
	}

	RTextureDesc GetDesc(int nIndex) { return m_Desc[nIndex]; }

private:
	RTextureDesc m_Desc[4];
};
TestDescContainer g_DescContainer;


TEST( TestMaterialDecal, TestGetMaterialDecalCount_AddAddDecalLayer_DecalLayerCountShouldBeIncreased)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, NULL, NULL);
	
	
	//����
	EXPECT_EQ(testMaterialDecal.GetDecalLayerCount(), 1);
}

TEST( TestMaterialDecal, TestHaveDecalLayer_AddNewDecal_ShouldHaveAddedDecal)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, NULL, NULL);


	//����
	EXPECT_TRUE(testMaterialDecal.HaveDecalLayer(MaterialName));
}

TEST( TestMaterialDecal, TestHaveDecalLayer_AddNewDecal_ShouldNotHaveNoAddedDecal)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";
	std::string NotExistMaterialName ="NoExist";


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, NULL, NULL);


	//����
	EXPECT_FALSE(testMaterialDecal.HaveDecalLayer(NotExistMaterialName));
}

TEST( TestMaterialDecal, TestGetDecalLayer_AddNewDecalWithoutTexture_ShouldReturnValidPointer)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, NULL, NULL);


	//����
	EXPECT_TRUE(testMaterialDecal.GetDecalLayer(MaterialName) != NULL);
}

TEST( TestMaterialDecal, TestGetDecalLayer_AddNewDecalWithDiffuseTexture_ShouldReturnValidPointer)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";
	RTextureDesc DiffuseTexDesc = g_DescContainer.GetDesc(0);


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, &DiffuseTexDesc, NULL);


	//����
	EXPECT_TRUE(testMaterialDecal.GetDecalLayer(MaterialName) != NULL);
}

TEST( TestMaterialDecal, TestGetDecalLayer_AddNewDecalWithMaskTexture_ShouldReturnValidPointer)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";
	RTextureDesc MaskTexDesc = g_DescContainer.GetDesc(0);


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, NULL, &MaskTexDesc);


	//����
	EXPECT_TRUE(testMaterialDecal.GetDecalLayer(MaterialName) != NULL);
}

TEST( TestMaterialDecal, TestGetDecalLayer_AddNewDecalWithDiffuseAndMaskTexture_ShouldReturnValidPointer)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";
	RTextureDesc DiffuseTexDesc = g_DescContainer.GetDesc(0);
	RTextureDesc MaskTexDesc = g_DescContainer.GetDesc(1);


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, &DiffuseTexDesc, &MaskTexDesc);


	//����
	EXPECT_TRUE(testMaterialDecal.GetDecalLayer(MaterialName) != NULL);
}


TEST( TestMaterialDecal, TestGetDecalLayer_AddNewDecal_ReturnedLayerInfoShouldBeSameToInputLayerInfo)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";

	RTextureDesc DiffuseTexDesc = g_DescContainer.GetDesc(0);
	RTextureDesc MaskTexDesc = g_DescContainer.GetDesc(1);

	COMPOSITE_LAYER_INFO ResultLayer = testMaterialDecal.CreateDecalLayer(&DiffuseTexDesc, &MaskTexDesc);


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, &DiffuseTexDesc, &MaskTexDesc);


	//����
	EXPECT_TRUE( ResultLayer == *testMaterialDecal.GetDecalLayer(MaterialName) );


	//����
	testMaterialDecal.DestoryLayer(ResultLayer);
}

TEST( TestMaterialDecal, TestAddDecalLayer_AddDecalLayerTwiceToSameMaterial_ShouldUpdatePrevLayerInfo )
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";

	RTextureDesc NewDiffuseTexDesc = g_DescContainer.GetDesc(0);
	RTextureDesc NewMaskTexDesc = g_DescContainer.GetDesc(1);
	RTextureDesc ReplacedDiffuseTexDesc = g_DescContainer.GetDesc(2);
	RTextureDesc ReplacedMaskTexDesc = g_DescContainer.GetDesc(3);

	COMPOSITE_LAYER_INFO ResultLayer = testMaterialDecal.CreateDecalLayer(&ReplacedDiffuseTexDesc, &ReplacedMaskTexDesc);


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, &NewDiffuseTexDesc, &NewMaskTexDesc);
	testMaterialDecal.AddDecalLayer(MaterialName, &ReplacedDiffuseTexDesc, &ReplacedMaskTexDesc);


	//����
	EXPECT_TRUE( ResultLayer == *testMaterialDecal.GetDecalLayer(MaterialName) );


	//����
	testMaterialDecal.DestoryLayer(ResultLayer);
}

TEST( TestMaterialDecal, TestSetDecalLayer_AddNewDecal_ShouldUpdatePrevTexInfo )
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";

	RTextureDesc NewDiffuseTexDesc = g_DescContainer.GetDesc(0);
	RTextureDesc NewMaskTexDesc = g_DescContainer.GetDesc(1);
	RTextureDesc ReplacedDiffuseTexDesc = g_DescContainer.GetDesc(2);
	RTextureDesc ReplacedMaskTexDesc = g_DescContainer.GetDesc(3);

	COMPOSITE_LAYER_INFO ResultLayer = testMaterialDecal.CreateDecalLayer(&ReplacedDiffuseTexDesc, &ReplacedMaskTexDesc);


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, &NewDiffuseTexDesc, &NewMaskTexDesc);
	testMaterialDecal.SetDecalLayer(MaterialName, &ReplacedDiffuseTexDesc, &ReplacedMaskTexDesc);


	//����
	EXPECT_TRUE( ResultLayer == *testMaterialDecal.GetDecalLayer(MaterialName) );


	//����
	testMaterialDecal.DestoryLayer(ResultLayer);
}

TEST( TestMaterialDecal, TestRemoveDecalLayer_AddNewDecal_ShouldRemoveDesiredDecalLayer)
{
	//��ġ
	RMaterialDecal testMaterialDecal;
	std::string MaterialName ="Test";
	RTextureDesc DiffuseTexDesc = g_DescContainer.GetDesc(0);
	RTextureDesc MaskTexDesc = g_DescContainer.GetDesc(1);


	//����
	testMaterialDecal.AddDecalLayer(MaterialName, &DiffuseTexDesc, &MaskTexDesc);
	testMaterialDecal.RemoveDecalLayer(MaterialName);


	//����
	EXPECT_FALSE(testMaterialDecal.HaveDecalLayer(MaterialName));
}