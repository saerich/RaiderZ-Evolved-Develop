#ifndef _XCMD_HANDLER_CRAFT_H
#define _XCMD_HANDLER_CRAFT_H

#include "MCommandHandler.h"
using namespace minet;

/// ũ����Ʈ Ŀ�ǵ� ó�� �ڵ鷯
class XCmdHandler_Craft : public MCommandHandler
{
public:
	XCmdHandler_Craft(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnShow);
	DECL_CMD_HANDLER(OnMake);
	DECL_CMD_HANDLER(OnRecipeInsert);
	DECL_CMD_HANDLER(OnRecipeDelete);
};

#endif//_XCMD_HANDLER_CRAFT_H
