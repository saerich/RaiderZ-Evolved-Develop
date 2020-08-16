#include "stdafx.h"
#include "XSoundField.h"
#include "XFieldInfo.h"
#include "XMyPlayer.h"
#include "XGameState.h"


XSoundField::XSoundField()
{
	m_nMuteAmbient = 0;
	m_dwLastTime = 0;
}


void XSoundField::Clear()
{
	m_nMuteAmbient = 0;

	if ( global.sound)
		global.sound->ClearLocalReverb();
}


bool XSoundField::IsMutedAmbient() const
{
	return ( m_nMuteAmbient > 0  ?  true : false);
}


XSoundField* XSoundField::GetInstance()
{
	static XSoundField instance;
	return &instance;
}


void XSoundField::Update( XFieldInfo* pInfo)
{
	if ( pInfo == NULL  ||  gvar.Game.pMyPlayer == NULL)	return;


	// 1�ʿ� �ѹ��� �����ϵ���.
	DWORD dwCurTime = timeGetTime();
	if ( ( m_dwLastTime - dwCurTime) < 1000)				return;
	m_dwLastTime = dwCurTime;


	int nPrevMuteAmbient = m_nMuteAmbient;
	bool bUpdatedAmbient = false;

	vector< FIELDSOUND_INFO>& vecSound = pInfo->m_vecSound;
	size_t _size = vecSound.size();
	for ( size_t i = 0;  i < _size;  i++)
	{
		FIELDSOUND_INFO& info = vecSound[i];

		if (global.sound  &&  global.sound->IsBGMPalying())
		{
			char szID[ 256];
			sprintf_s( szID, "fieldsound_%u", i);
			global.sound->Stop( szID);

			// contact �ʱ�ȭ
			info.bContact = false;
			continue;
		}

		// ���� ��ġ�� ���� �Ÿ� �ȿ� �ִ��� Ȯ��
		vec3 vSrc = gvar.Game.pMyPlayer->GetPosition();
		vec3 vTar = info.vPos;
		float fRadius = info.fMaxDist;
		bool inner = false;

		// ��� �ӵ��� ���ؼ� ���� x,y,z �ఢ �Ÿ��� ��
		{
			float dx = abs( vTar.x - vSrc.x);
			if ( dx < fRadius)
			{
				float dy = abs( vTar.y - vSrc.y);
				if ( dy < fRadius)
				{
					float dz = abs( vTar.z - vSrc.z);
					if ( dz < fRadius)
					{
						// �ఢ �Ÿ� �ȿ� �����ϸ� ���� �Ÿ��� ��
						if ( (dx * dx + dy * dy + dz * dz) < (fRadius * fRadius + (info.bContact ? 250000.0f : 0.0f)))
							inner = true;
					}
				}
			}
		}


		// ������ �����ߴ��� Ȯ��
		if ( info.bContact == false  &&  inner == true)
		{
			char szID[ 256];
			sprintf_s( szID, "fieldsound_%u", i);
			RSound* pSound = NULL;
			if ( global.sound)
				pSound = global.sound->PlayStatic3D( MLocale::ConvUTF16ToAnsi(info.strName.c_str()).c_str(), szID, 0, info.vPos);
			if ( pSound != NULL)			pSound->SetDistance( info.fMinDist, info.fMaxDist);

			info.bContact = true;

			if ( info.bMuteAmbient == true)
			{
				m_nMuteAmbient++;
				bUpdatedAmbient = true;
			}
		}

		// ������ �������� Ȯ��
		else if ( info.bContact == true  &&  inner == false)
		{
			char szID[ 256];
			sprintf_s( szID, "fieldsound_%u", i);

			if ( global.sound)
				global.sound->Stop( szID);

			info.bContact = false;
			bUpdatedAmbient = true;

			if ( info.bMuteAmbient == true)
			{
				m_nMuteAmbient--;
				bUpdatedAmbient = true;
			}
		}
	}


	// Indoor ���� ���� ��ȭ�� üũ�ؼ� ambient ���带 ������Ʈ �Ѵ�
	if ( bUpdatedAmbient == true)
	{
		if ( nPrevMuteAmbient == 0  &&  m_nMuteAmbient > 0)
		{
			if ( global.sound)
				global.sound->Stop( "ambient");
		}
		else if ( nPrevMuteAmbient > 0  &&  m_nMuteAmbient == 0)
		{
			if ( gg.currentgamestate && gg.currentgamestate->GetWorld())
				gg.currentgamestate->GetWorld()->PlayFieldSound();
		}
	}
}
