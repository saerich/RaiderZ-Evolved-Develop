#ifndef _XCMD_HANDLER_LOCAL_H
#define _XCMD_HANDLER_LOCAL_H

#include "MCommandHandler.h"
using namespace minet;

/// ���� Ŀ�ǵ� ó�� �ڵ鷯
class XCmdHandler_Local  : public MCommandHandler
{
private:
protected:

public:
	XCmdHandler_Local(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnReloadUI);
	DECL_CMD_HANDLER(OnChangeGameFrame);
};



#endif