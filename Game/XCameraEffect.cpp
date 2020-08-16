#include "stdafx.h"
#include "XCameraEffect.h"
#include "XCameraManager.h"

//////////////////////////////////////////////////////////////////////////
XCameraEffect::XCameraEffect() : m_nCameraControllerId(0)
{
}

XCameraEffect::~XCameraEffect()
{
	DeleteCameraEffect();
}


void XCameraEffect::LoadCameraEffect( const wchar_t* szDirPath )
{
	// ����Ÿ ��� ����
	m_strDirPath = szDirPath;

/*	
	// ī�޶� ����Ʈ �����ȿ� �ִ� ���� ��ü �˻�
	HANDLE hDir;
	WIN32_FIND_DATA DirData = {0};
	BOOL bRet = TRUE;
	wstring strFilePath = m_strDirPath + L"*.elu.ani";

	hDir = FindFirstFile(strFilePath.c_str(), &DirData);
	while( (hDir != INVALID_HANDLE_VALUE) && bRet )
	{
		if (DirData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) 
		{
			// ī�޶� ����Ʈ ����
			CreateCameraEffect(DirData.cFileName);
		}

		bRet = FindNextFile(hDir, &DirData);
	}

	FindClose(hDir);
*/

	// �̹� Ŭ��2���� �ϵ��ڵ��մϴ�. Ŭ�� ������ �����մϴ�.
	const wstring _fname[ 16] =
	{
		L"dummy_camera_arek_roar_01.elu",
		L"dummy_camera_hit_l.elu",
		L"dummy_camera_hit_s.elu",
		L"dummy_camera_hitvib_s.elu",
		L"dummy_camera_L.elu",
		L"dummy_camera_login.elu",
		L"dummy_camera_long.elu",
		L"dummy_camera_loop.elu",
		L"dummy_camera_r.elu",
		L"dummy_camera_roar_01.elu",
		L"dummy_camera_roar_02.elu",
		L"dummy_camera_s.elu",
		L"dummy_camera_short.elu",
		L"dummy_camera_spell_01.elu",
		L"dummy_camera_sx3.elu",
		L"dummy_camera_xs.elu"
	};

	for ( int i = 0;  i < 16;  i++)
	{
		wstring strPathFileName = _fname[ i] + wstring( L".ani");
		CreateCameraEffect( strPathFileName.c_str());
	}
}

void XCameraEffect::CreateCameraEffect( const wchar_t* szFileName )
{
	CAM_ANI_CONTROL::iterator itr = m_mapCameraAnimation.find( szFileName );
	if( itr == m_mapCameraAnimation.end() )
	{
		wchar_t szBuffer[256];
		szBuffer[0] = '\0';
		swprintf_s(szBuffer, L"%s_%03d", L"00_Ani", m_nCameraControllerId);

		RCameraAnimationController * pCameraAnimation = new RCameraAnimationController( MLocale::ConvUTF16ToAnsi(szBuffer).c_str(), MLocale::ConvUTF16ToAnsi((m_strDirPath + szFileName).c_str()).c_str(), false);
		if (pCameraAnimation->IsValidAnimation() == false)
		{
			mlog("�ε� �ȵ� ī�޶� ����Ʈ ����. ����!\n");
			delete pCameraAnimation;
		}
		else
		{
			wstring strAniName(szFileName);
			int nPosExt = strAniName.rfind(L".elu.ani");
			strAniName = strAniName.substr(0, nPosExt);
			std::transform(strAniName.begin(), strAniName.end(), strAniName.begin(), [](unsigned char c) { return std::tolower(c); });

			m_mapCameraAnimation.insert( CAM_ANI_CONTROL::value_type( strAniName, pCameraAnimation ) );
			++m_nCameraControllerId;
		}
	}
}

void XCameraEffect::DeleteCameraEffect()
{
	for (CAM_ANI_CONTROL::iterator itr = m_mapCameraAnimation.begin(); itr != m_mapCameraAnimation.end(); ++itr)
		delete itr->second;

	m_mapCameraAnimation.clear();
}

RCameraAnimationController * XCameraEffect::GetCameraEffect(const wstring& szAniName)
{
	CAM_ANI_CONTROL::iterator itr = m_mapCameraAnimation.find( szAniName );
	if ( itr != m_mapCameraAnimation.end() )
		return itr->second;
	else
		return NULL;
}

bool XCameraEffect::PlayCameraEffect( const wchar_t* szAniName, float _fDuration )
{
	//// Main Backview ī�޶� ��鸱 �� �ִ�.
	//if (global.camera->IsPrimaryCameraType(CAMERA_MAIN_BACKVIEW) == false) return false;

	wstring strAniName(szAniName);
	std::transform(strAniName.begin(), strAniName.end(), strAniName.begin(), [](unsigned char c) { return std::tolower(c); });

	RCameraSceneNode * pCamera = global.camera->GetPrimaryCamera();
	RCameraAnimationController * pCameraAnimation	= GetCameraEffect(szAniName);

	if(pCamera && pCameraAnimation)
	{
		// �̹� ������ΰ�?
		pCameraAnimation->RemoveFromCamera();

		// ī�޶� ����Ʈ ���
		pCameraAnimation->ResetTimer(_fDuration);
		pCamera->AddCameraController(pCameraAnimation);

		return true;
	}

	dlog("��ϵ��� ���� ī�޶� �ִϸ��̼� ��û: %s.elu.ani \n", MLocale::ConvUTF16ToAnsi(strAniName.c_str()).c_str());
	return false;
}


