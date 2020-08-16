#pragma once

#include "ServerCommonLib.h"
#include "MTstring.h"

/// ���� �о��� ó���ϴ� Ŭ����
class SCOMMON_API SFilePath
{
private:
public:
	SFilePath() {}
	static wstring MakeFieldList(const wchar_t* szSystemPath, bool bCheckDebugFieldList=true);
	static wstring MakeZoneList(const wchar_t* szSystemPath);
};