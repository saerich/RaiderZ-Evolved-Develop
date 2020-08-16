#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "REnvironmentSet.h"
#include "RFont.h"
#include "RTextSceneNode.h"

SUITE(ETC)
{
	const char *INSTANCING_FILE_NAME = "Instancing.zone.xml";
	const char *INSTANCING_ENV_FILE_NAME = "Instancing.zone.env.xml";

	TEST(TestInstancing)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true), INSTANCING_FILE_NAME);
		CHECK(bRet);

		// ȯ�� ����� ����
		REnvironmentSet* pEnvironmentSet = new REnvironmentSet;

		// ���Ϸκ��� �����
		bRet = pEnvironmentSet->CreateFromFile(INSTANCING_ENV_FILE_NAME, false);
		CHECK(bRet);

		// ���Ŵ����� ���ϱ�
		REngine::GetSceneManager().AddSceneNode(pEnvironmentSet);

		// ȯ�� ����
		bRet = pEnvironmentSet->SetCurrentEnvSet(REnvironment::NIGHT, REnvironment::SUNNY, false, 0.f, false);
		CHECK(bRet);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );

		pCamera->SetNearFarZ( 2000, 6000);
		pCamera->SetPosition( RVector3( 29395, 28137, 2488) + RVector(0.012f, 0.854f, -0.518f) * 150); 
		//pCamera->SetDirection( RVector(0.012f, 0.854f, -0.518f) );
		pCamera->SetDirection( RVector(0.012f, 0.854f, -0.518f) );
		pCamera->UpdateViewFrustrum();
		pCamera->UpdateTransform();

		
		// �������ϸ� ����� ��� ���� �ѹ� ���� �� �ؽ�Ʈ ���� �ϰ� �ѹ� �� ����
		// ��ũ�� ��
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot(NULL);



		// ���� �ؽ�Ʈ
		RFont* pFont		= new RFont;
		pFont->Create( REngine::GetDevicePtr() ,L"����",12);

		RTextSceneNode* pTextNode = new RTextSceneNode;
		pTextNode->SetFont( pFont);
		pTextNode->SetText( "test");
		pTextNode->SetPosition( 0, 0);
		pTextNode->SetColor( 0xffff00ff);
		REngine::GetSceneManager().AddSceneNode( pTextNode);

		char szText[256];
		// �ؽ�Ʈ ����
		sprintf_s(szText," Status of Instancing \n Draw Primitive of Tree: %d \n - leaf:%d frond:%d branch:%d billboard:%d \n Draw Primitive of Actor: %d \n - static:%d physique:%d\n\n", 
			// tree
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_TREELEAF) + 
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_TREEFROND) + 
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_TREEBRANCH) + 
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_TREEBILLBOARD),
			// detail
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_TREELEAF),
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_TREEFROND),
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_TREEBRANCH),
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_TREEBILLBOARD),
			// actor
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_ACTOR) +
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_ACTORPHYSIQUE),
			// detail
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_ACTOR),
			REngine::GetResourceProfiler().GetDrawCallCount( PPT_ACTORPHYSIQUE)			
			);

		pTextNode->SetText(szText);


		// ��ũ�� ��
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestInstancing");
		

		// ȯ�� ����� �Ҹ�
		pEnvironmentSet->RemoveFromParent();
		delete pEnvironmentSet;

		SAFE_DELETE( pFont);
		
		if( pTextNode)
			pTextNode->RemoveFromParent();
		SAFE_DELETE( pTextNode);
	}
}
