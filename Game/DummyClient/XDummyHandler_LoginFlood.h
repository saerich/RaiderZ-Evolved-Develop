#pragma once

#include "XDummyHandler.h"

// ���� �α���
// - 
class XDummyHandler_LoginFlood : public XDummyHandler
{
public:
	virtual MCommandResult OnCommand(XBirdDummyAgent* pAgent, MCommand* pCmd);
	virtual const wchar_t* GetName() { return L"LoginFlood"; }
};



// �α���, �α׾ƿ� �ݺ�
class XDummyHandler_LoginDisconnect : public XDummyHandler
{
public:
	virtual MCommandResult OnCommand(XBirdDummyAgent* pAgent, MCommand* pCmd);
	virtual const wchar_t* GetName() { return L"LoginDisconnect"; }
};
