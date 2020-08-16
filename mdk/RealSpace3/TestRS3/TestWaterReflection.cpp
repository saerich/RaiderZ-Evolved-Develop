#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RCameraSceneNode.h"
#include "RWaterSceneNode.h"
#include "RWaterPassRenderer.h"

SUITE(Water)
{
	const char *ZONE_FILE_NAME = "Login.zone.xml";

	bool GetAllWaterPlane(RSceneNode *pSceneNode)
	{
		if ( _stricmp(pSceneNode->GetNodeIDString(), "WATERPLANE") == 0 )
			return true;
		else
			return false;
	}

	TEST(TestWaterReflection)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true),ZONE_FILE_NAME);
		CHECK(bRet);

		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// bump adjust factor = 0
		// reflect adjust = 1
		vector<RSceneNode*> vWaterArray;
		REngine::GetSceneManager().GetCurrentRootSceneNode(false)->SerializeToVector(GetAllWaterPlane, vWaterArray);

		for( size_t i = 0; i < vWaterArray.size(); ++i )
		{
			RWaterPlane *pPlane = (RWaterPlane*)vWaterArray[i];
			pPlane->GetWaterPlaneProperty()->m_fBumpAdjustFactor = 0.0f;
			pPlane->GetWaterPlaneProperty()->m_fReflectAdjust = 1.0f;
		}

		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );
		pCamera->SetPosition(RVector3(11652.0f, 11185.0f, -44.22f)); // �� ������ ������
		pCamera->SetDirection(RVector3(0.9829f, -0.1610f, -0.088f));

		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestWaterReflection");
		// �ε��� ���� RSceneManager Clear�ÿ� �˾Ƽ� ��������. ���� �������ʿ� ����

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

}
