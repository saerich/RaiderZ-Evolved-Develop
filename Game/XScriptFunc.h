#pragma once

#include "MWLua.h"

/// lua���� ����ϴ� C �Լ� ����
class XScriptFunc
{
public:
	static void Register(MWLua* pLua);

	// Debug ---------
	static void Debug_AddLogFilter(const char* szFilter);
	static void Debug_Console(const char* szCommand);
};
