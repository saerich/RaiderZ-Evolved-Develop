#pragma once

#include "CSDef.h"
#include "XAccountInfo.h"


struct XAccountCharInfo
{
	wchar_t					szName[PLAYER_NAME_LEN+1];
	int						nLevel;
	int						nFieldID;
	XPlayerInfoFeature		Feature;
};



/// ���������� ����ϴ� ĳ���� ���� ���� ����
struct GlobalVar_Char
{
	vector<XAccountCharInfo>		m_CharInfos;
	XAccountInfo					m_AccountInfo;

	GlobalVar_Char()
	{
	}
};
