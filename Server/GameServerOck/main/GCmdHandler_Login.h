#pragma once

#include "MCommandHandler.h"
using namespace minet;

#include "CSDef.h"

class GPlayerObject;
class GPlayerAcceptor;

/// ���� ������ �α��� ���� Ŀ�ǵ� �ڵ鷯
class GCmdHandler_Login : public MCommandHandler, public MTestMemPool<GCmdHandler_Login>
{
public:
	GCmdHandler_Login(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnRequestLoginGameServer);			///< �α��� ������ ���� �α���
	DECL_CMD_HANDLER(OnRequestMoveGameServer);			///< �ٸ� ���Ӽ����κ��� �̵��ؿ� �÷��̾� �α���

	DECL_CMD_HANDLER(OnGameStart);						///< ���� ����

	DECL_CMD_HANDLER(OnRequestLoginGameServerOnPmang);	///< �Ǹ� �α��� ��, �α��� ������ ���� ���Ӽ��� ����
	DECL_CMD_HANDLER(OnRequestMoveGameServerOnPmang);	///< �Ǹ� �α��� ��, �ٸ� ���Ӽ����κ��� �̵�

private:
	static bool CheckServerClosing(MUID uidPlayer);
};
