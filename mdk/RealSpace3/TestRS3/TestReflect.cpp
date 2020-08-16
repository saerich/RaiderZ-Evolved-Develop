#include "stdafx.h"
#include "RSceneManager.h"
#include "RActor.h"
#include "RCameraSceneNode.h"
#include "RShadowStrategy.h"

SUITE(ETC)
{
	RActor* CreateActor( RVector3& _vPos, RVector3& _vDir )
	{
		// �ν��Ͻ� ����
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		if( NULL == pActor )
			return NULL;

		// �ڿ����κ��� �ε�
		bool bRet = pActor->Create("hf_plate_armor.elu");

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		// ���� ���� - ��ǥ
		pActor->SetTransform( _vPos, _vDir );
		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);
		return pActor;
	}

	TEST(TestReflect)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RVector vPos(7, 0, 69);
		RVector vDir(0,-1,0);

		RActor* pActor	= CreateActor( RVector(0.f, 0.f, 0.f), vDir );

		CHECK(pActor);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 25 + RVector(0,0,70), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("Reflect");
	
		// �ν��Ͻ� ���ֱ�
		pActor->RemoveFromParent();
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
		delete pActor;
	}
}