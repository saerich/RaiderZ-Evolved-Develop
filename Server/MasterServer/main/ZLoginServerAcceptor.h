#pragma once

#include "ZServerAcceptor.h"

class ZLoginServerInfo;

/// ���� ������ �α��� ��û�� ����ִ� Ŭ����
class ZLoginServerAcceptor : public ZServerAcceptor
{
public:
	ZLoginServerAcceptor(MUID uidClient, int nServerID) : ZServerAcceptor(SERVER_LOGIN, uidClient, nServerID) {}
	virtual ~ZLoginServerAcceptor() {}

	bool IsAlreadyExist() override;
	bool Accept() override;
	minet::MCommand* MakeResponseCommand() override;
	minet::MCommand* MakeFailCommand(int nCommandResult) override;
};
