#ifndef _XCMD_HANDLER_PALETTE_H
#define _XCMD_HANDLER_PALETTE_H

#include "MCommandHandler.h"
using namespace minet;

/// ũ����Ʈ Ŀ�ǵ� ó�� �ڵ鷯
class XCmdHandler_Palette : public MCommandHandler
{
public:
	XCmdHandler_Palette(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnSelect);
	DECL_CMD_HANDLER(OnSetPrimary);
	DECL_CMD_HANDLER(OnSetSecondary);

	DECL_CMD_HANDLER(OnPutUp);
	DECL_CMD_HANDLER(OnPutDown);
	DECL_CMD_HANDLER(OnChange);
	DECL_CMD_HANDLER(OnPutDownAllTalent);
};

#endif
