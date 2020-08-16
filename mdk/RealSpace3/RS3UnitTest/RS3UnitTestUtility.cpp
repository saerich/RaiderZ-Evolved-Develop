#include "stdafx.h"
#include "RS3UnitTestUtility.h"
#include "REngine.h"
#include "RSceneManager.h"
#include "RDeviceNull.h"

using namespace rs3;

NullDeviceEngineCreater::NullDeviceEngineCreater()
{
	// ����̽� ����
	RDeviceNull* pDevice = new RDeviceNull;
	bool bRet = pDevice->Create( NULL );
	_ASSERT( bRet == true );
	REngine::RegisterDevice( pDevice );

	// �� �Ŵ��� ����
	RSceneManager* pSceneManager = new RSceneManager;
	REngine::RegisterSceneManager( pSceneManager );
}

NullDeviceEngineCreater::~NullDeviceEngineCreater()
{
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();
	_ASSERT( pSceneManager != NULL );

	// ���Ŵ���
	pSceneManager->Clear();
	pSceneManager->Destroy();
	delete pSceneManager;
	REngine::RegisterSceneManager( NULL );

	// ����̽�
	RDevice* pDevice = REngine::GetDevicePtr();
	_ASSERT( pDevice != NULL );
	pDevice->Destroy();
	delete pDevice;
	REngine::RegisterDevice(NULL);
}

void NullDeviceFixture::UpdateSceneManager( DWORD dwElapsed /*= 1*/ )
{
	REngine::GetSceneManager().UpdateTimerCustom( dwElapsed );
	REngine::GetSceneManager().Update();
}
