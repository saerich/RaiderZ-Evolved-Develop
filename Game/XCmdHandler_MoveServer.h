#pragma once

#include "McommandHandler.h"
using namespace minet;

class XCmdHandler_MoveServer : public MCommandHandler
{
public:
	XCmdHandler_MoveServer(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnMoveGameServer);
	DECL_CMD_HANDLER(OnStartMoveToLoginServerRes);	///< �����̵� ����.
	DECL_CMD_HANDLER(OnResponseMoveLoginServer);	///< �����̵��ؼ� �α��μ����� ���� ��û.

};
