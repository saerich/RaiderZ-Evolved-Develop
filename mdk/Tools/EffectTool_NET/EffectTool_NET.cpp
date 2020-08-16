// EffectTool_NET.cpp : �⺻ ������Ʈ �����Դϴ�.

#include "stdafx.h"
#include "Global.h"
#include "MainForm.h"
#include "MProfiler.h"

using namespace EffectTool_NET;

[STAThreadAttribute]

int main(array<System::String ^> ^args)
{
	// ������ �ּ� ����
	static DWORD gTickDelay = 1;

	// ��Ʈ���� ��������� ���� Windows XP �ð� ȿ���� Ȱ��ȭ�մϴ�.
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �� â�� ����� �����մϴ�.
	GlobalObjects::g_pMainForm = gcnew MainForm();

	// tree �����ܵ� ���Ϸ� �ٽ� �����ϰ������
	/*
	 	for( int i=0;i<9; i++ )
	 	{
	 		String^ strFilename = "icons\\" + GlobalObjects::g_pMainForm->imageListSceneNodeType->Images->Keys[i];
	 		GlobalObjects::g_pMainForm->imageListSceneNodeType->Images->default[i]->Save( strFilename );
	 
	 	}
	//*/

	GlobalObjects::g_pMainForm->Show();

	DWORD dwStartTick = GetTickCount();
	while(GlobalObjects::g_pMainForm->Created)
	{	
		PFC_RESET;

		DWORD dwCurrentTick = GetTickCount();
		if ( dwCurrentTick - dwStartTick >= gTickDelay )
		{
			dwStartTick = dwCurrentTick;
			GlobalObjects::g_pMainForm->MainLoop();
		}

		Application::DoEvents();

		if( !GlobalObjects::g_pMainForm->m_bActive )
			Sleep(1);
	}

	PFC_FINALANALYSIS("profile_effectool.txt");
	return 0;
}
