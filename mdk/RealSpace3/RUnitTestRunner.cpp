#include "stdafx.h"

#include "RUnitTestRunner.h"
#include "REngine.h"
#include "RDeviceD3D.h"
#include "RDeviceNull.h"

#include "RCameraSceneNode.h"
#include "RActor.h"

#include "RFont.h"
#include "RShadowStrategy.h"
#include "REnvironmentSet.h"
#include "RDirectionalLightSceneNode.h"

#include "MDebug.h"
#include "MFileLoader.h"
#include "FileInfo.h"
#include "shlwapi.h"

#pragma warning(disable : 4996)

namespace rs3 {

void RUnitTestRunner::Init(const char* szAppName, const char* szScreenShotFolder, const char* szOutXMLFileName, bool bMakeReferrenceImageMode)
{
	m_strAppName = szAppName;
	m_strOutXMLFileName = szOutXMLFileName;
	m_strScreenShotFolder = szScreenShotFolder;
	m_bMakeReferrenceImageMode = bMakeReferrenceImageMode;
	m_nFailShotCount = 0;

	DeleteFile( szOutXMLFileName);

	MXmlDeclaration* pDec = m_xml.CreateDeclaration("1.0", "UTF-8", "");
	m_xml.Doc()->LinkEndChild(pDec);

	m_pRootElement = new MXmlElement("unittest-results");
	m_xml.Doc()->LinkEndChild(m_pRootElement);
}

void RUnitTestRunner::InitLog(const char* szLogFolder, const char* szLogFileName)
{
	string strFullName = szLogFolder;
	strFullName += "/";
	strFullName += szLogFileName;
	if(m_bMakeReferrenceImageMode)
		strFullName += "_ref";
	strFullName += ".txt";
	MInitLog(MLogLevel::MLOG_LEVEL_DEBUG,MLOG_DEBUG | MLOG_FILE,NULL,strFullName.c_str());
}

void RUnitTestRunner::FinalizeLog()
{
	MFinalizeLog();
}

void RUnitTestRunner::Destroy()
{
	_SetAttribute( m_pRootElement, "failures", m_nFailShotCount );	
	m_xml.SaveFile(m_strOutXMLFileName.c_str());
}

bool RUnitTestRunner::InitEngine(HWND _hWnd, bool bConsole, bool bUseScreenShot)
{
	m_hWnd = _hWnd;
	m_bConsole = bConsole;
	m_bUseScreenShot = bUseScreenShot;
	m_bMock = false;

	// ����̽� ����
	RDeviceD3D* pDevice = new RDeviceD3D;
	bool bRet = pDevice->Create(m_hWnd);
	if(!bRet)
	{
		_ASSERT(false);
		delete pDevice;
		return false;
	}
	REngine::RegisterDevice(pDevice);

	// �� �Ŵ��� ����
	RSceneManager* pSceneManager = new RSceneManager;
	REngine::RegisterSceneManager(pSceneManager);
	bRet = pSceneManager->Create(pDevice);
	_ASSERT(bRet == true);
	bRet = REngine::InitSharedResource();
	if(!bRet) {
		HaltEngine();
		return false;
	}

	_ASSERT(bRet == true);

	pSceneManager->SetSSAO( false);

	ResetTimer();

	// Primary Camera ����
	ResetCamera();

// 	//�ܼ��� ���� ����, �ܼ� ��ũ������ ������ Actor�� ���õ� ���۹����� �־ �ѹ���� ����ش�.
// 	if (m_bConsole && m_bUseScreenShot)
// 	{
		// �ν��Ͻ� ����
// 		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
// 		_ASSERT(pActor);
//  
// 		// �ڿ����κ��� �ε�
// 		bool bRet = REngine::GetSceneManager().LoadSceneNode(pActor, "Monster/goblin_mage/goblin_Mage.elu");
// 		_ASSERT(bRet);
// 
// 		// ���Ŵ����� ���̱�
// 		REngine::GetSceneManager().AddSceneNode(pActor);

		// ������ ��
// 		UpdateScene(true);
// 		GrabScreenShot(NULL);
//		DeleteFile("__TEMP.bmp");

// 		pActor->RemoveFromParent();
// 		delete pActor;
// 	}
	return true;
}

void RUnitTestRunner::HaltEngine(bool bNeedToResetForNewCreation)
{
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();
	_ASSERT(pSceneManager != NULL);

	// ī�޶�
	RCameraSceneNode* pCameraSceneNode = pSceneManager->SetPrimaryCamera(NULL);
	if(pCameraSceneNode)
	{
		pCameraSceneNode->RemoveFromParent();
		delete pCameraSceneNode;
	}

	//DelDirectionalLight();

	// ���Ŵ���
	pSceneManager->Clear();

	REngine::DestroySharedResource();
	pSceneManager->Destroy();
	delete pSceneManager;
	REngine::RegisterSceneManager(NULL);

	// �������� �����ϴ� �÷��� �� �ڿ� �ʱ�ȭ.
	RFont::ResetFinalizeFlag();

	// ����̽� ����
	RDevice* pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice != NULL);
	pDevice->Destroy();
	delete pDevice;
	REngine::RegisterDevice(NULL);
}

void RUnitTestRunner::ResetEngine()
{
	ResetTimer();

	// SceneManager Reset
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();
	_ASSERT(pSceneManager != NULL);

	RCameraSceneNode* pCamera = pSceneManager->SetPrimaryCamera(NULL);
	_ASSERT(pCamera != NULL);
	pCamera->RemoveFromParent();
	delete pCamera;

	pSceneManager->SetHDREnable( false );
	pSceneManager->SetDOFEnable( false );
	pSceneManager->SetScreenMotionBlurEnable( false );
	pSceneManager->SetAccumMotionBlurEnable( false );
	pSceneManager->SetPixelMotionBlurEnable( false );
	pSceneManager->SetDistortionEnable( false );

	//DelDirectionalLight();
	// ������ ������ ������ ��� ������ ����
	pSceneManager->UseLighting( false);

	pSceneManager->Clear();

	ResetCamera();

	RShadowStrategy* pShadowStrategy = pSceneManager->GetShadowStrategy();
	if ( pShadowStrategy )
		pShadowStrategy->ClearBuffers();

	//_ASSERT(REnvironmentSet::GetInstancePtr() == NULL);

	// pass renderer reset
	for( int i = 0; i < RRP_MAX; ++i)
	{
		RPassRenderer* pPassRenderer = pSceneManager->GetPassRenderer( (RRENDERPASS)i );
		if( pPassRenderer)
			pPassRenderer->Reset();
	}
	
	// ȯ�� ���� ���� ��츦 ��� �Ͽ� ������ ����� F. ȯ�� ���� ��� �� �˾Ƽ� T�� �ȴ�.
	REngine::GetSceneManager().UseLighting( false);

	REngine::GetDevice().SetFillMode( RFILL_SOLID );

	REngine::GetResourceProfiler().Reset();
}

void RUnitTestRunner::AddDirectionalLight()
{
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();
	_ASSERT(pSceneManager != NULL);

	// �𷺼ų� ����Ʈ ����
	RDirectionalLightSceneNode* pDirLight = pSceneManager->GetDefaultLight();
	if(!pDirLight)
	{
		pDirLight = new RDirectionalLightSceneNode();
		pSceneManager->AddSceneNode(pDirLight);
		pDirLight->SetLightDirection( 1, -1, 0);
		pDirLight->SetLightAmbient( 0, 0, 0, 0);
		pDirLight->SetLightDiffuse(1,1,1,1);
	}
	// ������ ������ ������ ��� ������ ����
	pSceneManager->UseLighting( true);
}


void RUnitTestRunner::DelDirectionalLight()
{
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();
	_ASSERT(pSceneManager != NULL);

	RDirectionalLightSceneNode* pDirLight = pSceneManager->GetDefaultLight();
	if(pDirLight)
	{
		pDirLight->RemoveFromParent();
		SAFE_DELETE(pDirLight);
	}

	// ������ ������ ������ ��� ������ ����
	pSceneManager->UseLighting( false);
}


void RUnitTestRunner::InitMockEngine()
{
	// mock�� screenshot�� ���� �ʴ´�.
	m_bUseScreenShot = false;
	m_bMock = true;

	// ����̽� ����
	RDeviceNull* pDevice = new RDeviceNull;
	bool bRet = pDevice->Create(m_hWnd);
	_ASSERT(bRet==true);
	REngine::RegisterDevice(pDevice);

	// �� �Ŵ��� ����
	RSceneManager* pSceneManager = new RSceneManager;
	REngine::RegisterSceneManager(pSceneManager);

	ResetTimer();

	// Primary Camera ����
	ResetCamera();
}

void RUnitTestRunner::HaltMockEngine(bool bNeedToResetForNewCreation)
{
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();
	_ASSERT(pSceneManager != NULL);

	// ī�޶�
	RCameraSceneNode* pCameraSceneNode = pSceneManager->SetPrimaryCamera(NULL);
	_ASSERT(pCameraSceneNode != NULL);
	pCameraSceneNode->RemoveFromParent();
	delete pCameraSceneNode;

	// ���Ŵ���
	pSceneManager->Clear();
	pSceneManager->Destroy();
	delete pSceneManager;
	REngine::RegisterSceneManager(NULL);

	// ����̽�
	RDevice* pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice != NULL);
	pDevice->Destroy();
	delete pDevice;
	REngine::RegisterDevice(NULL);
}

void RUnitTestRunner::ResetMockEngine()
{
	ResetTimer();

	// SceneManager Reset
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();
	_ASSERT(pSceneManager != NULL);

	RCameraSceneNode* pCamera = pSceneManager->SetPrimaryCamera(NULL);
	_ASSERT(pCamera != NULL);
	pCamera->RemoveFromParent();

	pSceneManager->Clear();

	ResetCamera();
}

void RUnitTestRunner::GetRefImageFileName( char* szRefImageFilename, const char* szImageName)
{
	if(szImageName)
		sprintf( szRefImageFilename, "%s/%s_ref.png", m_strScreenShotFolder.c_str(), szImageName);
}

void RUnitTestRunner::GetImageFileName( char* szImageFilename, const char* szImageName)
{
	if(szImageName)
		sprintf( szImageFilename, "%s/%s.png", m_strScreenShotFolder.c_str(), szImageName);
}

void RUnitTestRunner::UpdateScene(bool bUpdateTimer)
{
	// timer update
	if (bUpdateTimer)
		REngine::GetSceneManager().UpdateTimerCustom(10);
}

void RUnitTestRunner::GrabScreenShot(const char* _pSzFileNamePreFix,RCameraSceneNode* pCamera)
{
	char szOutputImageName[MAX_PATH] = "";
	
	if( m_bMakeReferrenceImageMode )
		GetRefImageFileName( szOutputImageName,_pSzFileNamePreFix);
	else
		GetImageFileName( szOutputImageName,_pSzFileNamePreFix);

	// scene manager update
	RSceneManager *pSceneManager = REngine::GetSceneManagerPtr();
	pSceneManager->Update();
	if( pSceneManager->GetCurrentRootSceneNode() )
	{
		if( pSceneManager->GetCurrentRootSceneNode()->GetEnvironmentSet() )
			pSceneManager->GetCurrentRootSceneNode()->GetEnvironmentSet()->Update( NULL );
	}

	if (m_bConsole && IsEnableScreenShot() )
	{
		RCameraSceneNode *pRenderCamera = NULL;
		if( pCamera == NULL )	pRenderCamera = pSceneManager->GetPrimaryCamera();
		else					pRenderCamera = pCamera;
		
		RSceneNodeUpdateVisitor visitor;
		pRenderCamera->Traverse(&visitor);
		pRenderCamera->UpdateViewFrustrum();

		pRenderCamera->RenderScreenShot( szOutputImageName, pSceneManager->GetRenderingStrategy(), -1, -1);
		REngine::GetDevice().Flip();
	}
	else
	{
		REngine::GetDevice().Clear(true, true, false, D3DCOLOR_ARGB(255, 0, 0, 255));
		REngine::GetDevice().BeginScene();

		if( pCamera == NULL )		REngine::GetSceneManager().RenderPrimaryCamera();
		else						REngine::GetSceneManager().RenderCamera(pCamera);

		REngine::GetDevice().EndScene();
		REngine::GetDevice().Flip();

		if (IsEnableScreenShot())
			static_cast<RDeviceD3D*>(REngine::GetDevicePtr())->SaveScreenShot( szOutputImageName );
	}

	if (IsEnableScreenShot() && _pSzFileNamePreFix)
		mlog("\t %d frame, %s ScreenShot\n", REngine::GetDevice().GetFrameCount(), szOutputImageName);

	if(_pSzFileNamePreFix)
	{
		if(m_bMakeReferrenceImageMode) 
			mlog(" test screen shot is skipped ( ref mode ) \n");
		else
			// �̰����� �̹����� ���� �� �Ѵ�.
			TestScreenShot(_pSzFileNamePreFix);
	}
}

void RUnitTestRunner::SaveImageFile(const char* _pSzFileNamePreFix, RTexture* pTexture)
{
	if( R_NONE == pTexture )
		return;

	char szOutputImageName[MAX_PATH] = "";

	if( m_bMakeReferrenceImageMode )
		GetRefImageFileName( szOutputImageName,_pSzFileNamePreFix);
	else
		GetImageFileName( szOutputImageName,_pSzFileNamePreFix);

	pTexture->SaveToFile( szOutputImageName );
}

void RUnitTestRunner::ResetTimer()
{
	// Ÿ�̸� ����
	REngine::GetTimer().Reset();

	// �������� �����ϰ� �ϱ� ����
	srand( 100 );
}

void RUnitTestRunner::ResetCamera()
{
	RCameraSceneNode* pCameraSceneNode = new RCameraSceneNode;
	REngine::GetSceneManager().AddSceneNode(pCameraSceneNode);
	REngine::GetSceneManager().SetPrimaryCamera(pCameraSceneNode);

	RVector vPos(0, 0, 0);
	RVector vDir(0,1,0);

	// ī�޶� ����
	pCameraSceneNode->Set(vPos + vDir * 150 + RVector(0,0,70), -vDir);
}

// void RUnitTestRunner::UpdateTimer(float fDelta)
// {
// 	REngine::GetSceneManager().UpdateTimer();
//	REngine::GetSceneManager().SyncTimerToApp(fDelta);
//}

void RUnitTestRunner::UpdateTimerForcely(DWORD _dwElapsedTime)
{
	REngine::GetSceneManager().UpdateTimerCustom(_dwElapsedTime);
//	REngine::GetSceneManager().SyncTimerToApp(fDelta);
}

void RUnitTestRunner::UpdateSceneAndFlipDevice(bool bUpdateTimer)
{
	bool bOldScreenShot = m_bUseScreenShot;
	m_bUseScreenShot = false;

	UpdateScene(bUpdateTimer);
	GrabScreenShot("UpdateAndFlip");

	m_bUseScreenShot = bOldScreenShot;
}

bool RUnitTestRunner::TestScreenShot(const char* szFileName)
{
	char szImageFilename[_MAX_PATH];
	char szRefImageFilename[_MAX_PATH];
	GetImageFileName(szImageFilename, szFileName);
	GetRefImageFileName(szRefImageFilename, szFileName);

	// ref image �� ����°� �ƴϸ� ref image �� ���Ͽ�����
	int nTotalPixels;
	int nDiffPixels;
	CompareImages( szRefImageFilename, szImageFilename, &nDiffPixels, &nTotalPixels );

	MXmlElement* pTestElement = new MXmlElement("test");
	m_pRootElement->LinkEndChild( pTestElement );
	_SetAttribute( pTestElement, "suite", "ScreenShot" );
	_SetAttribute( pTestElement, "name", szFileName );
	
	// �̸�ŭ �ȼ� Ʋ���Ŵ� ������
	const int ERROR_PIXEL_LIMIT = 10;

	if(nDiffPixels > ERROR_PIXEL_LIMIT)
	{
		char szMessage[256];
		sprintf( szMessage, "%s %d pixels (%3.3f %%) do not match.", szFileName, nDiffPixels, (float)nDiffPixels/(float)nTotalPixels*100.f );
		mlog("%s\n", szMessage);
		MXmlElement* pFailureElement = new MXmlElement("failure");
		pTestElement->LinkEndChild( pFailureElement );
		_SetAttribute( pFailureElement, "message", szMessage );

		m_nFailShotCount++;
	}
	else
	{
		if( 0==nDiffPixels )
			mlog("%s is perfectly match.\n", szFileName);
		else
			mlog("%s is almost -_- match.\n", szFileName);

		// ��ġ �Ѵٸ� Diff ������ �����Ѵ�.
		char szDiffFileName[_MAX_PATH];
		sprintf( szDiffFileName, "%s.diff.bmp", szImageFilename );
		if( DeleteFile(szDiffFileName) )
			mlog("!! delete ref image.\n");
	}

	return true;
}

void RUnitTestRunner::CompareImages( const char* szRefImage, const char* szNewImage, int* pDiffPixels, int* pTotalPixels)
{
	int nRefWidth = 0;
	int nRefHeight = 0;
	int nRefChannel = 0;
	BYTE* pRefBits = MFileLoad::SOIL_CreateRawFromFile(szRefImage,&nRefWidth,&nRefHeight,&nRefChannel);

	int nNewWidth = 0;
	int nNewHeight = 0;
	int nNewChannel = 0;
	BYTE* pNewBits = MFileLoad::SOIL_CreateRawFromFile(szNewImage,&nNewWidth,&nNewHeight,&nNewChannel);

	if( nRefWidth!=nNewWidth || nRefHeight!=nNewHeight || nRefChannel!=nNewChannel)
	{
		if(pTotalPixels)
			*pTotalPixels = nNewHeight*nNewWidth;
		if(pDiffPixels)
			*pDiffPixels = *pTotalPixels;

		mlog("%s dimension does not match.\n", szNewImage);
		delete []pRefBits;
		delete []pNewBits;
		return;
	}

	DWORD* pdwRefBits = (DWORD*)pRefBits;
	DWORD* pdwNewBits = (DWORD*)pNewBits;
	DWORD* pdwDiffBits = new DWORD[ nNewHeight * nNewWidth ];

	int nDiffPixelCount = 0;
	for(int i=0;i<nRefHeight; i++)
	{
		for(int j=0;j<nRefWidth; j++)
		{
			int srcIndex = i*nRefWidth + j;
			int dstIndex = (nRefHeight-i-1)*nRefWidth + j;
			if( pdwRefBits[ srcIndex ] != pdwNewBits[ srcIndex ])
			{
				nDiffPixelCount++;
				pdwDiffBits[ dstIndex ] = 0xffff0000;
			}
			else
			{
				pdwDiffBits[ dstIndex ] = 0x00;
			}
		}
	}

	if(pDiffPixels)
		*pDiffPixels = nDiffPixelCount;
	if(pTotalPixels)
		*pTotalPixels = nNewHeight*nNewWidth;
	
	if(nDiffPixelCount)
	{
		char szDiffFileName[_MAX_PATH];
		sprintf( szDiffFileName, "%s.diff.bmp", szNewImage );
		// SaveBitmapFrom24bitRawTo24BMPFile ���Ʒ��� �ٲ���ִ� -_-;
		MFileLoad::SaveBitmapFrom24bitRawTo24BMPFile(szDiffFileName, nNewWidth, nNewHeight, pdwDiffBits);
	}

	delete []pdwDiffBits;

	delete []pRefBits;
	delete []pNewBits;
}

} //end of namespace
