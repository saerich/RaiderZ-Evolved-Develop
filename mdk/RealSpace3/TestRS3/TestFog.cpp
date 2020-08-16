#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RSceneNodeID.h"
#include "REnvironmentSet.h"

SUITE(ETC)
{
	TEST(TestFog)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true),"Fog.scene.xml");
		CHECK(bRet);

		// ȯ�� ����� ����
		REnvironmentSet* pEnvironmentSet = new REnvironmentSet;
		pEnvironmentSet->CreateFromFile("Fog.zone.env.xml", false);

		// ���Ŵ����� ���ϱ�
		REngine::GetSceneManager().AddSceneNode(pEnvironmentSet);

		// ȯ�� ����
		bRet = pEnvironmentSet->SetCurrentEnvSet(REnvironment::NIGHT, REnvironment::SUNNY, false, 0.f, false);
		CHECK(bRet);

		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		//ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );
		//pCamera->Set(RVector3(8896,35204,1689), RVector(-0.03f,0.953f,-0.3f));
		pCamera->Set(RVector3(3587,2629,1591), RVector(-0.015f,0.953f,-0.3f));
		pCamera->UpdateTransform();

		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestFog");

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();

		// ȯ�� ����� �Ҹ�
		pEnvironmentSet->RemoveFromParent();
		delete pEnvironmentSet;
	}
}