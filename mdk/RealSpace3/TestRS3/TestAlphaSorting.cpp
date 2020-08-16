#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RActor.h"
#include "RActorPassRenderer.h"

SUITE(ETC)
{
	const char *ZONE_FILE_NAME2 = "testAlphaSoring.scene.xml";

	TEST(TestAlphaSorting)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true),ZONE_FILE_NAME2);
		CHECK(bRet);

		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );
		pCamera->Set(RVector3(3447,3133,1310), RVector(0.59f,0.69f,-0.40f));

		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestAlphaSorting");
		// �ε��� ���� RSceneManager Clear�ÿ� �˾Ƽ� ��������. ���� �������ʿ� ����

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}
}
