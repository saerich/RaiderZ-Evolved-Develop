#include "stdafx.h"
#include "X3DFrameLook.h"

////////////////////////////////////////////////////////////////////////
X3DFrameLook::X3DFrameLook(string strScreenEffectName, string strFileName, RSceneManager * pManager, XScreenEffectManager * pScreenEffectManager) 
{
	m_strScreenEffectName		= strScreenEffectName;
	m_strScreenEffectFileName	= strFileName;

	m_pSceneManager				= pManager;
	m_pScreenEffectManager		= pScreenEffectManager;
}
X3DFrameLook::~X3DFrameLook() 
{
	m_pSceneManager				= NULL;
	m_pScreenEffectManager		= NULL;
}

void X3DFrameLook::OnDraw(MFrame* pFrame, MDrawContext* pDC)
{
	//if(pFrame->m_bTitleBar == true)
	//{
	//	if (pFrame->GetCloseButton()->m_bVisible)
	//	{
	//		MBitmap* pBitmap;
	//		if (pFrame->GetCloseButton()->IsButtonDown() == false)
	//		{
	//			pBitmap = m_pCloseButtonBitmaps[0];
	//		}
	//		else
	//		{
	//			pBitmap = m_pCloseButtonBitmaps[1];
	//			if (pBitmap == NULL) pBitmap = m_pCloseButtonBitmaps[0];
	//		}

	//		if (pBitmap!=NULL)
	//		{
	//			int x, y;
	//			x = pFrame->GetCloseButton()->m_Rect.x;
	//			y = pFrame->GetCloseButton()->m_Rect.y;

	//			pDC->SetBitmap(pBitmap);
	//			pDC->Draw(x, y);
	//		}

	//	}
	//	if (pFrame->GetMinimizeButton()->m_bVisible)
	//	{
	//		MBitmap* pBitmap;
	//		if (pFrame->GetMinimizeButton()->IsButtonDown() == false)
	//		{
	//			pBitmap = m_pMinimizeButtonBitmaps[0];
	//		}
	//		else
	//		{
	//			pBitmap = m_pMinimizeButtonBitmaps[1];
	//			if (pBitmap == NULL) pBitmap = m_pMinimizeButtonBitmaps[0];
	//		}

	//		if (pBitmap!=NULL)
	//		{
	//			int x, y;
	//			x = pFrame->GetMinimizeButton()->m_Rect.x;
	//			y = pFrame->GetMinimizeButton()->m_Rect.y;

	//			pDC->SetBitmap(pBitmap);
	//			pDC->Draw(x, y);
	//		}

	//	}

	//	if(m_pFont!=NULL) pDC->SetFont(m_pFont);
	//	pDC->SetColor(MCOLOR(0x0));
	//	if( GetCustomLook() == 0 ) pDC->Text(r.x+16, r.y+12, pFrame->m_szText);

	//	pDC->SetColor(m_FontColor);

	//	// ���߿� align����ؼ� �ٽ� �����ؾ� ��

	//	int y = int(m_TitlePosition.y*GetScale());

	//	if(m_pFrameBitmaps[7])
	//	{
	//		int au = (int)(GetScale() * m_pFrameBitmaps[7]->GetHeight());
	//		y = (au - m_pFont->GetHeight())/2;
	//	}

	//	pDC->Text(int(r.x+m_TitlePosition.x*GetScale()), r.y+y, pFrame->m_szText);
	//}	
}

void X3DFrameLook::ChangeAnimation(string strAnimation, XSCREENEFFECT_ANIMATION_END_ATT aniDel)
{
	m_pScreenEffectManager->ChangeAnimation(m_strScreenEffectName, strAnimation, aniDel);
}

void X3DFrameLook::Create()
{
	// ��ũ�� ����Ʈ ���
	m_pScreenEffectManager->RegisterScreenEffect(m_strScreenEffectName,
												m_strScreenEffectFileName,
												CHARACTER_NORMAL,
												RAniLoopType_HoldLastFrame);
	m_pScreenEffectManager->SetPosition(m_strScreenEffectName, vec3(0, 0, 0), LOCATION_LEFT);
}

bool X3DFrameLook::CreateReady()
{
	// �� Ŭ������ ������ �Ǿ��� �� ī�޶� ���� �غ� ���̴�.
	// �׷��Ƿ� Draw�ÿ� �˻��Ͽ� �غ� �Ǿ��� �� ������ �Ѵ�.
	// ������ ���� �ʾ��� ��...
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

	return true;
}