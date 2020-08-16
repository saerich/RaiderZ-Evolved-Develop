#include "stdafx.h"
#include "XGameFrameHelper.h"
#include "XProfiler.h"
#include "XMovableDebugger.h"

#include "XPost_GM.h"
#include "XBaseApplication.h"
#include "XMyPlayer.h"

#include "XSoundResMgr.h"

#ifdef _MADDUCK
	#include "XMagicArea.h"
	//#include "XSystem.h"
	#include "XGame.h"
	#include "XModuleMyControl.h"
#endif

bool XGameFrameHelper::OnDebugEvent(MEvent& e)
{
	if(global.ui /*&& global.ui->IsFocusMainInput()*/)
	{
		switch(e.nMessage)
		{
		case MWM_CHAR:
			{
#ifndef _PUBLISH
				switch (e.nKey) 
				{
				case 'b':
					{
						XScreenDebugger::GetInstance()->Show(!XScreenDebugger::GetInstance()->IsVisible());
						return true;
					}
					break;
				}
#endif // _PUBLISH
			}
			break;
			// �׽�Ʈ������ ���� KeyDown�Դϴ�.
		case MWM_KEYDOWN:
			{
				switch (e.nKey) 
				{
				case VK_F5:
					{
						static int nTarget = 0;

						if(nTarget == 0)
							nTarget = 1;
						else
							nTarget = 0;

					}
					break;
				}
				break;
			}
		}
	}

	switch(e.nMessage)
	{
	case MWM_KEYDOWN:
		{
			int nKeyPadNum = 0;
			switch (e.nKey) 
			{
#ifndef _PUBLISH
			case VK_F4:
				{
					if (e.bCtrl == false)	global.app->ToggleSolidWire();
				}
				break;
			case VK_F3:
				{
					if(e.bAlt)
					{
						PFC_RESET;
					}
					else
					{
					}
				}
				break;
#endif // _PUBLISH
#ifdef _MADDUCK
			case VK_F7:
				{
					gvar.Game.pMyPlayer->GetModuleMyControl()->SetTargetIndicatorToggle(false);
				}
				break;
			case VK_F9:
				{
					global.ui->OnPresentationUpper(L"�����̼��� �׽�Ʈ�մϴ�.");
					global.ui->OnPresentationLower(L"�����̼��� �׽�Ʈ�մϴ�.");
					global.script->GetGlueGameEvent().OnGameEvent( "MAPSIGN", "�׽�Ʈ����");
				}
				break;
#endif // _madduck
			// bparts
			case VK_NUMPAD1:
				{
					nKeyPadNum = 1;
					break;
				}
			case VK_NUMPAD2:
				{
					nKeyPadNum = 2;
					break;
				}
			case VK_NUMPAD3:
				{
					nKeyPadNum = 3;
					break;
				}
			case VK_NUMPAD4:
				{
					nKeyPadNum = 4;
					break;
				}
			}
			if (nKeyPadNum > 0 && e.bCtrl == true && gg.omgr)
			{
				for (XObjectMap::iterator itor = gg.omgr->BeginItor(); itor != gg.omgr->EndItor(); ++itor)
				{
					XObject* pTarget = (*itor).second;
					if (pTarget == gvar.Game.pMyPlayer)
						continue;

					if (pTarget->GetPosition().DistanceTo(gvar.Game.pMyPlayer->GetPosition()) < 1000.f)
					{
						XPostGM_BreakPart(pTarget->GetUID(), nKeyPadNum);
					}
				}
			}
		}
		break;
	}
	return false;
}

void XGameFrameHelper::RenderMint()
{
//	TIMEPROFILE_THIS_FUNCTION;

	//////////////////////////////////////////////////////////////////////////
	// ������ ����
	// 1. ���� ���� (engine)
	// 2. 3D ����
	// 3. screen effect ( screen ī�޶� �Ἥ )
	// 4. Mint

	// mint�� ������� ���� ui ������ ( screeen effect manager / pane manager )
	PFC_THISFUNC;
	PFI_BLOCK_THISFUNC(104);

	global.mint->GetDC()->BeginDraw();

	// ����� ���� �׸���
	XScreenDebugger::GetInstance()->Draw(global.ui->GetDC());

	//if (XMovableDebugger::GetInstancePtr())
	//{
	//	XMovableDebugger::GetInstancePtr()->Draw(global.ui->GetDC());
	//}

	global.mint->GetDC()->EndDraw();

	{
		TIMEPROFILE("MINT");
		// UI �׸���
		PFC_B("global.mint->Draw()");
		global.mint->Draw();
		PFC_E;
	}
}

