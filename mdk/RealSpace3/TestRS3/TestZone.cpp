#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RCameraSceneNode.h"
#include "REnvironmentSet.h"
#include "RTerrain.h"

SUITE(Map)
{
	const char *ZONE_FILE_NAME1 = "Login.zone.xml";
	const char *ZONE_FILE_NAME2 = "BirdTown.zone.xml";

	TEST(TestLoadZone)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true),ZONE_FILE_NAME1);
		CHECK(bRet);

		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );
		pCamera->SetPosition(RVector3(15010.0f, 8510.0f, 410.0f)); // �� ������ ������

		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestLoadZone");
		// �ε��� ���� RSceneManager Clear�ÿ� �˾Ƽ� ��������. ���� �������ʿ� ����

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	TEST(TestGridZone)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true),ZONE_FILE_NAME1);
		CHECK(bRet);

		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );
		pCamera->SetPosition(RVector3(15010.0f, 8510.0f, 410.0f)); // �� ������ ������

		if( REngine::GetSceneManager().GetCurrentTerrain())
		{
			REngine::GetSceneManager().GetCurrentTerrain()->SetGridFlag(true);
			REngine::GetSceneManager().GetCurrentTerrain()->SetGrassLodParams( 0, 0);
		}
		
		pCamera->SetPassIgnore((RRENDERPASS)0);
		pCamera->SetPassIgnore( RRP_TERRAIN, false);

		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestGridZone");
		// �ε��� ���� RSceneManager Clear�ÿ� �˾Ƽ� ��������. ���� �������ʿ� ����

		if( REngine::GetSceneManager().GetCurrentTerrain())
			REngine::GetSceneManager().GetCurrentTerrain()->SetGridFlag(false);

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	TEST(TestChangeZone)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true),ZONE_FILE_NAME1);
		CHECK(bRet);

		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RCameraSceneNode* pOldCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pOldCamera );
		pOldCamera->SetPosition(RVector3(15010.0f, 8510.0f, 410.0f)); // �� ������ ������

		// ��ũ����2
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("ResetZoneLogin");

		// Primary Camera�� NULL�� ����� ������ �پ��ִ� Primary Camera�� ����ش�.
		RCameraSceneNode* pCamera = REngine::GetSceneManager().SetPrimaryCamera(NULL);
		CHECK( pOldCamera == pCamera );
		pCamera->RemoveFromParent();

		// ���Ŵ��� Clear
		REngine::GetSceneManager().Clear();

		// �� �ε�
		bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true),ZONE_FILE_NAME2);
		CHECK(bRet);

		// SetPrimaryCamera�� �ϸ� ������ Camera�� �������ش�.
		// ���Ŵ��� Clear���� SetPrimary(NULL)�� �����Ƿ� NULL�� �����ؾ� �Ѵ�.
		RCameraSceneNode* pPreviousCamera = REngine::GetSceneManager().SetPrimaryCamera(pCamera);
		CHECK( pPreviousCamera == NULL );
		CHECK(pCamera);
		REngine::GetSceneManager().AddSceneNode(pCamera);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("ResetZoneBirdTown");

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}
}
