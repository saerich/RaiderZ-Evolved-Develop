// Character_NET.cpp : �⺻ ������Ʈ �����Դϴ�.

#include "stdafx.h"
#include "Form1.h"
// 
//using namespace Character_NET;
 
[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// ��Ʈ���� ��������� ���� Windows XP �ð� ȿ���� Ȱ��ȭ�մϴ�.
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �� â�� ����� �����մϴ�.
	GlobalObjects::g_pGlobal	= gcnew Global();
	GlobalObjects::g_pMainForm  = gcnew Form1();

	GlobalObjects::g_pMainForm->Show();
	while(GlobalObjects::g_pMainForm->Created)
	{
		GlobalObjects::g_pMainForm->MainLoop();
		Application::DoEvents();

		if( !GlobalObjects::g_pMainForm->m_bActive )
			Sleep(1);
	}
	return 0;
}
