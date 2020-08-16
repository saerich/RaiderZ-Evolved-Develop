#include "stdafx.h"

#include "RSceneManager.h"
#include "RActor.h"
// #include "RMesh.h"
// #include "RMeshMgr.h"
#include "RCameraSceneNode.h"
//#include "REnvironmentSet.h"

//#include "RActorController.h"
//#include "RAnimationController.h"
#include "RShadowStrategy.h"

SUITE(ETC)
{
	TEST(TestMultiLayer)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
// 		// ����Ʈ ����
// 		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		CHECK(pActor);

		bool bRet = pActor->Create("N_bos_003.elu");
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		// ���� ���� - �ִϸ��̼� / ��ǥ
		pActor->SetTransform( vPos, -vDir );
		pActor->SetAnimation( "idle" );

		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + RVector(-1000,1300,1000), RVector3(1, -1, -1));

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("MultiLayer");

		// �ν��Ͻ� ���ֱ�
		pActor->RemoveFromParent();
// 		// ����Ʈ ���ֱ�
// 		RUnitTestRunner::GetInstance().DelDirectionalLight();
		delete pActor;
	}
}