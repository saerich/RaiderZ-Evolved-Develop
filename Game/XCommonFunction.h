#pragma once

/// C++, Lua���� �������� ����ϴ� �Լ���
class XCommonFunction
{
public:
	static void Connect(const wchar_t* szLoginID, const wchar_t* szPassword, const wchar_t* szIP);
	static const wchar_t* GetPlayerName( MUID uid);
	static const wchar_t* GetItemName(int nItemID);

};