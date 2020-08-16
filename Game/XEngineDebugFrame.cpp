#include "stdafx.h"
#include "XEngineDebugFrame.h"
#include "RSceneManager.h"
#include "XDef.h"
#include "XGame.h"
#include "XController.h"
#include "XPost_GM.h"
#include "XGameFrameUILoader.h"
#include "XGameFrameHelper.h"
#include "XBaseApplication.h"
#include "XCameraManager.h"
#include "XScreenShotSaver.h"
#include "RBackgroundWorker.h"
#include "XMovableDebugger.h"
#include "XEngineDebugger.h"
#include "XGameInitializer.h"

#include "XWorld.h"

XEngineDebugFrame::XEngineDebugFrame()
{
	m_bInitAfterLoading = false;
	m_pEngineDebugger = new XEngineDebugger;
}

XEngineDebugFrame::~XEngineDebugFrame()
{
	delete m_pEngineDebugger;
}

void XEngineDebugFrame::Update(float fDelta)
{
	PFC_THISFUNC;

	if (m_bInitAfterLoading == false)
	{
		global.ui->ToggleConsole();

		if (global.script)
		{
			// World�� ���鶧 �� ������ �ѹ��� ����
			XScript* pScript = global.script;
			global.script = NULL;
			pScript->GetGlueGameEvent().OnDebugOnGameStarted();
			global.script = pScript;
		}

		m_bInitAfterLoading = true;
	}

	// base frame update : net / ui / mint
	XBaseFrame::Update(fDelta);

	// ��Ʈ�ѷ� �ھ� ������Ʈ
	if (gg.controller)
		gg.controller->OnUpdate(fDelta);

	// ���� �ھ� ������Ʈ
	if (m_pGame)
		m_pGame->Update(fDelta);

	// ����� ������Ʈ
	m_pEngineDebugger->Update(fDelta);
}

void XEngineDebugFrame::OnRender()
{
	PFC_THISFUNC;
	XPlayFrame::OnRender();
}

bool XEngineDebugFrame::OnEvent(MEvent& e)
{
	if (e.nMessage == MWM_KEYDOWN)
	{
		if (global.ui)
		{
			if (e.nKey == 192 && e.bCtrl == true)
			{
				// �ܼ�
				global.ui->ToggleConsole();

				// -_- �̵� �Էµ� ���� ��Ʈ���� �������� ���콺 ������ �������Ѵ�.
				global.ui->ShowMouse(global.ui->IsConsoleVisible());
				return true;
			}
		}
	}

	if (XPlayFrame::OnEvent(e))
		return true;

	if (XGameFrameHelper::OnDebugEvent(e))
		return true;

	return false;
}

bool XEngineDebugFrame::Open( int nPrevFrameID, const wchar_t* szArgs)
{
	XCONST::AUTO_REPORT_CRASHDUMP = true;

	global.app->GetInitializer()->Init_While_LoadingLoginCampaign();
	global.app->SetClearColor(0xFF000000);
	XPlayFrame::Open( nPrevFrameID, szArgs);
	return true;
}

void XEngineDebugFrame::Close( int nNextFrameID)
{
	XPlayFrame::Close( nNextFrameID);
}

bool XEngineDebugFrame::OnOpenWorldLoadingInit()
{
	return true;
}

bool XEngineDebugFrame::OnOpenControllerInit()
{
	if (gg.controller->Create() == false)
	{
		assert(!"XPlayFrame::Open - Failed to Create Controller");
		return false;
	}

	return true;
}

bool XEngineDebugFrame::OnOpenCameraInit()
{
	global.camera->CreateCamerasOnFrameOpen(GetID());
	global.camera->SetPrimaryCamera(CAMERA_ENGINE_DEBUG);

	return true;
}

bool XEngineDebugFrame::OnOpenGameInit()
{
	return true;
}
