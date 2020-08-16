#include "stdafx.h"
#include "TCameraEffect.h"

//////////////////////////////////////////////////////////////////////////
TCameraEffect::TCameraEffect() : m_nCameraControllerId(0)
{
}

TCameraEffect::~TCameraEffect()
{
	DeleteCameraEffect();
}


void TCameraEffect::LoadCameraEffect( const char* szDirPath )
{
	// ����Ÿ ��� ����
	m_strDirPath = szDirPath;

	// ī�޶� ����Ʈ �����ȿ� �ִ� ���� ��ü �˻�
	HANDLE hDir;
	WIN32_FIND_DATA DirData = {0};
	BOOL bRet = TRUE;
	string strFilePath = m_strDirPath + "*.elu.ani";

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
}

void TCameraEffect::CreateCameraEffect( const char* szFileName )
{
	CAM_ANI_CONTROL::iterator itr = m_mapCameraAnimation.find( szFileName );
	if( itr == m_mapCameraAnimation.end() )
	{
		char szBuffer[256];
		szBuffer[0] = '\0';
		sprintf_s(szBuffer,"%s_%03d", "00_Ani", m_nCameraControllerId);

		RCameraAnimationController * pCameraAnimation = new RCameraAnimationController( szBuffer, (m_strDirPath + szFileName).c_str(), false);
		if (pCameraAnimation->IsValidAnimation() == false)
		{
			mlog("�ε� �ȵ� ī�޶� ����Ʈ ����. ����!\n");
			delete pCameraAnimation;
		}
		else
		{
			string strAniName(szFileName);
			int nPosExt = strAniName.rfind(".elu.ani");
			strAniName = strAniName.substr(0, nPosExt);
			strAniName = CSStrings::StringToLower(strAniName);

			m_mapCameraAnimation.insert( CAM_ANI_CONTROL::value_type( strAniName, pCameraAnimation ) );
			++m_nCameraControllerId;
		}
	}
}

void TCameraEffect::DeleteCameraEffect()
{
	for (CAM_ANI_CONTROL::iterator itr = m_mapCameraAnimation.begin(); itr != m_mapCameraAnimation.end(); ++itr)
		delete itr->second;

	m_mapCameraAnimation.clear();
}

RCameraAnimationController * TCameraEffect::GetCameraEffect(const string& szAniName)
{
	CAM_ANI_CONTROL::iterator itr = m_mapCameraAnimation.find( szAniName );
	if ( itr != m_mapCameraAnimation.end() )
		return itr->second;
	else
		return NULL;
}

bool TCameraEffect::PlayCameraEffect( const char* szAniName, float _fDuration )
{
	//// Main Backview ī�޶� ��鸱 �� �ִ�.
	//if (global.camera->IsPrimaryCameraType(CAMERA_MAIN_BACKVIEW) == false) return false;

	string strAniName(szAniName);
	strAniName = CSStrings::StringToLower(strAniName);

	RCameraSceneNode * pCamera = g_pMainApp->GetCamera();
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

	mlog("��ϵ��� ���� ī�޶� �ִϸ��̼� ��û: %s.elu.ani \n", strAniName.c_str());
	return false;
}


