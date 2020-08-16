#include "stdafx.h"
#include "X3DButtonLook.h"

//////////////////////////////////////////////////////////////////////////
X3DButtonLook::X3DButtonLook(string strScreenEffectName, string strFileName, RSceneManager * pManager, XScreenEffectManager * pScreenEffectManager) 
{
	m_strScreenEffectName		= strScreenEffectName;
	m_strScreenEffectFileName	= strFileName;

	m_pSceneManager				= pManager;
	m_pScreenEffectManager		= pScreenEffectManager;
	m_eButtonState				= MBT_STATE_NOT;
}
X3DButtonLook::~X3DButtonLook() 
{
	m_pSceneManager				= NULL;
	m_pScreenEffectManager		= NULL;
}

void X3DButtonLook::SetAnimationFileName(int nIndex, string strAniFileName, int AniEndAtt)
{
	m_stAnimationInfo[nIndex].strAniName	= strAniFileName;
	m_stAnimationInfo[nIndex].nAniEndAtt	= (XSCREENEFFECT_ANIMATION_END_ATT)AniEndAtt;
}

void X3DButtonLook::OnDraw(MButton* pButton, MDrawContext* pDC)
{
	MBButtonLook::OnDraw(pButton, pDC);	
}

void X3DButtonLook::OnDownDraw(MButton* pButton, MDrawContext* pDC)
{
	if(m_eButtonState != MBT_STATE_PRESS &&	CreateReady())
	{
		m_eButtonState = MBT_STATE_PRESS;
		m_pScreenEffectManager->ChangeAnimation(m_strScreenEffectName, 
													m_stAnimationInfo[MBT_STATE_PRESS].strAniName, 
													m_stAnimationInfo[MBT_STATE_PRESS].nAniEndAtt);
	}
}

void X3DButtonLook::OnUpDraw(MButton* pButton, MDrawContext* pDC)
{
	if( pButton->GetType() == MBT_PUSHBTN && pButton->GetCheck() ) 
	{
		OnDownDraw( pButton, pDC );
		return;
	}

	// Up�� �Ϲ� Draw�� ���� ������ �Ѵ�.
	if(m_eButtonState != MBT_STATE_NORMAL && CreateReady())
	{
		m_eButtonState = MBT_STATE_NORMAL;
		m_pScreenEffectManager->ChangeAnimation(m_strScreenEffectName, 
													m_stAnimationInfo[MBT_STATE_NORMAL].strAniName, 
													m_stAnimationInfo[MBT_STATE_NORMAL].nAniEndAtt);
	}
}

void X3DButtonLook::OnOverDraw(MButton* pButton, MDrawContext* pDC)
{
	if( pButton->GetType() == MBT_PUSHBTN )
	{
		if( pButton->GetCheck() ) 
			OnDownDraw( pButton, pDC );
		else 
			OnUpDraw(pButton, pDC );

		return;
	}

	if(m_eButtonState != MBT_STATE_OVER && CreateReady())
	{
		m_eButtonState = MBT_STATE_OVER;
		m_pScreenEffectManager->ChangeAnimation(m_strScreenEffectName, 
													m_stAnimationInfo[MBT_STATE_OVER].strAniName, 
													m_stAnimationInfo[MBT_STATE_OVER].nAniEndAtt);
	}
}

void X3DButtonLook::OnDisableDraw(MButton* pButton, MDrawContext* pDC)
{

}

void X3DButtonLook::Create()
{
	// ��ũ�� ����Ʈ ���
	m_pScreenEffectManager->RegisterScreenEffect(m_strScreenEffectName,
												m_strScreenEffectFileName,
												CHARACTER_NORMAL,
												RAniLoopType_HoldLastFrame);
	m_pScreenEffectManager->SetPosition(m_strScreenEffectName, vec3(0, 0, 0), LOCATION_LEFT);
}

bool X3DButtonLook::CreateReady()
{
	// �� Ŭ������ ������ �Ǿ��� �� ī�޶� ���� �غ� ���̴�.
	// �׷��Ƿ� Draw�ÿ� �˻��Ͽ� �غ� �Ǿ��� �� ������ �Ѵ�.
	// ������ ���� �ʾ��� ��...
	if(m_eButtonState == MBT_STATE_NOT)
	{
		// ī�޶� �ִ°�.
		if(m_pSceneManager->GetPrimaryCamera())
		{
			Create();
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}