#pragma once

#include "XDummyHandler.h"

// ���� ����
// - 
class XDummyHandler_ConnFlood : public XDummyHandler
{
public:
	virtual MCommandResult OnCommand(XBirdDummyAgent* pAgent, MCommand* pCmd);
	virtual const wchar_t* GetName() { return L"ConnFlood"; }
};



