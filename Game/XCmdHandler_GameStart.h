#pragma once

#include "McommandHandler.h"
using namespace minet;

/// ���� ������ ���� ���� Ŀ�ǵ� �ڵ鷯
class XCmdHandler_GameStart : public MCommandHandler
{
protected:
	static bool CommonResponseLogin(MCommand* pCommand, MCommandHandler* pHandler);
	DECL_CMD_HANDLER(OnResponseLoginGameServer);
	DECL_CMD_HANDLER(OnResponseMoveGameServer);
public:
	XCmdHandler_GameStart(MCommandCommunicator* pCC);	
};

