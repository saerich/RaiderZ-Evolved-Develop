// Plugin_CutScene.cpp : �ش� DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
//

#include "stdafx.h"

#include "Plugin_CutScene.h"
#include "WS_WorkSpaceManager.h"
#include "WS_CutScene.h"
#include "C_ControlMediator.h"

#include "VR_CutSceneObjectList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlugin_CutSceneApp

BEGIN_MESSAGE_MAP(CPlugin_CutSceneApp, CWinApp)
END_MESSAGE_MAP()


// CPlugin_CutSceneApp ����

CPlugin_CutSceneApp::CPlugin_CutSceneApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CPlugin_CutSceneApp ��ü�Դϴ�.

CPlugin_CutSceneApp theApp;


// CPlugin_CutSceneApp �ʱ�ȭ

BOOL CPlugin_CutSceneApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

class CutSceneAppPlugin : public CPluginInterface
{
public:
	bool Initialize(CWorkSpaceManager* pWorkSpaceManager) override
	{
		pWorkSpaceManager->RegisterView<CRollupCutSceneObjectList> (theApp.m_hInstance);
		pWorkSpaceManager->RegisterWorkspace<CWorkSpaceCutScene> ();
		return true;
	}

	void Finalize(CWorkSpaceManager* pWorkSpaceManager) override
	{
		pWorkSpaceManager->UnregisterView<CRollupCutSceneObjectList> ();
		pWorkSpaceManager->UnregisterWorkspace<CWorkSpaceCutScene> ();
	}

	HINSTANCE GetInstanceHandle()
	{
		return theApp.m_hInstance;
	}
} g_pluginInstance;

extern "C" __declspec(dllexport) CPluginInterface* CreatePlugin()
{
	return &g_pluginInstance;
}