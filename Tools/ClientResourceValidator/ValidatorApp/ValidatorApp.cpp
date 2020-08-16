// ValidatorApp.cpp : �⺻ ������Ʈ �����Դϴ�.

#include "stdafx.h"
#include "Form1.h"
#include "MFileSystem.h"

using namespace ValidatorApp;


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// File system �ʱ�ȭ
	MCreateFileSystem( MFILEACCESS_GENERIC);

	// ��Ʈ���� ��������� ���� Windows XP �ð� ȿ���� Ȱ��ȭ�մϴ�.
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// ���ڰ� Ȯ��
	if(args->Length > 0 && args[0] == "ccnet")
	{
		GlobalObjects::g_ValidatorCCNet = true;		
	}

	// �� â�� ����� �����մϴ�.
	GlobalObjects::g_pMainForm = gcnew Form1();
	GlobalObjects::g_pMainForm->m_bOneExecute = GlobalObjects::g_ValidatorCCNet;
	Application::Run(GlobalObjects::g_pMainForm);

	return GlobalObjects::g_ValidatorResult;
}
