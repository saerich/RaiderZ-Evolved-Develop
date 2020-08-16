#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RCameraSceneNode.h"
#include "RTerrain.h"

SUITE(Terrain)
{
	const char *ZONE_FILE_NAME = "Login.zone.xml";
	const char *NORMALMAP_FILE_NAME = "BirdTownNormalmap.zone.xml";

	TEST(TestCreateBakedMap)
	{
		RUnitTestRunner::GetInstance().ResetEngine();

		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true), ZONE_FILE_NAME );
		CHECK( bRet );

		RTerrain* pTerrain = REngine::GetSceneManager().GetCurrentTerrain();
		CHECK( pTerrain );

		bRet = pTerrain->CreateBakedMap();
		CHECK( bRet );

		RUnitTestRunner::GetInstance().SaveImageFile( "TestTerrainCreateBakedMap", pTerrain->GetBakedMap() );
	}

	TEST(TestNormalMap)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true), NORMALMAP_FILE_NAME);
		CHECK(bRet);

		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();
		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );

		pCamera->SetPosition( RVector3( 29395, 28137, 40));			
		pCamera->SetDirection( RVector(0.012f, 0.854f, -0.718f) );
		pCamera->UpdateViewFrustrum();
		pCamera->UpdateTransform();

		// ��ũ�� ��
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestTerrainNormalmap");

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}
}
