// Plugin_PhysX.cpp : �ش� DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
//

#include "stdafx.h"

#include "Plugin_PhysX.h"
#include "WS_WorkSpaceManager.h"
#include "WS_PhysX.h"
#include "C_ControlMediator.h"

#include "VR_PhysX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlugin_PhysXApp

BEGIN_MESSAGE_MAP(CPlugin_PhysXApp, CWinApp)
END_MESSAGE_MAP()


// CPlugin_PhysXApp ����

CPlugin_PhysXApp::CPlugin_PhysXApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CPlugin_PhysXApp ��ü�Դϴ�.

CPlugin_PhysXApp theApp;


// CPlugin_PhysXApp �ʱ�ȭ

BOOL CPlugin_PhysXApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

class PhysXAppPlugin : public CPluginInterface
{
public:
	bool Initialize(CWorkSpaceManager* pWorkSpaceManager) override
	{
		pWorkSpaceManager->RegisterView<CRollupPhysX> (theApp.m_hInstance);
		pWorkSpaceManager->RegisterWorkspace<CWorkSpacePhysX> ();
		return true;
	}

	void Finalize(CWorkSpaceManager* pWorkSpaceManager) override
	{
		pWorkSpaceManager->UnregisterView<CRollupPhysX> ();
		pWorkSpaceManager->UnregisterWorkspace<CWorkSpacePhysX> ();
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
