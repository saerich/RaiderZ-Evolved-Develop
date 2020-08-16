#pragma once

#include <string>
#include "MTstring.h"
#include "CSCommonLib.h"

/// Ŭ���̾�Ʈ, ���� Application�� ���� Ŭ����
class CSCOMMON_API CAppHelper
{
public:
	static std::tstring MakeTitleCaption(const TCHAR* szProgName, const TCHAR* szVersion, const TCHAR* szBuildNumber, const TCHAR* szDate);
	static std::tstring MakeServerVersion(const TCHAR* szVersion, const TCHAR* szBuildNumber, const TCHAR* szDate);
};