#include "stdafx.h"
#include "XScriptFunc.h"
#include "XUISystem.h"

void XScriptFunc::Register(MWLua* pLua)
{
	//----------------------------------------------------------------------------//
	// ��ũ��Ʈ���� ����� �Լ� ���
	pLua->Def("Debug_AddLogFilter", &XScriptFunc::Debug_AddLogFilter);
	pLua->Def("Debug_Console", &XScriptFunc::Debug_Console);
}


void XScriptFunc::Debug_AddLogFilter(const char* szFilter)
{
	MAddLogFilter(szFilter);
}

void XScriptFunc::Debug_Console(const char* szCommand)
{
	if (global.ui)
	{
		global.ui->InputConsoleCmd(szCommand);
	}
}
