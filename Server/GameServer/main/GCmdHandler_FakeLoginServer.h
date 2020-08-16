#pragma once

#include "MCommandHandler.h"
using namespace minet;

/// ���� �α��� �������� ��������� StandAlone�� ���� ���Ӽ��������� ó���ϴ� Ŀ�ǵ� �ڵ鷯
class GCmdHandler_FakeLoginServer : public MCommandHandler, public MTestMemPool<GCmdHandler_FakeLoginServer>
{
public:
	GCmdHandler_FakeLoginServer(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnRequestLogin);					///< �α���
	DECL_CMD_HANDLER(OnRequestAccountCharList);			///< ĳ���� ����Ʈ ��û
	DECL_CMD_HANDLER(OnRequestInsertCharacter);			///< ĳ���� ���� ��û
	DECL_CMD_HANDLER(OnRequestDeleteCharacter);			///< ĳ���� ���� ��û
	DECL_CMD_HANDLER(OnRequestSelMyChar);				///< ĳ���� ����

	DECL_CMD_HANDLER(OnRequestMoveLoginServer);			///< ĳ���� ���� ȭ������(�α��μ�����) �̵� ���� ��û

private:
	static void RouteResponseMoveLoginServer(int nResult, MUID uidPlayer);

};

