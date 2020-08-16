#pragma once

#include "ZServerAcceptor.h"
#include "STransData_M2G.h"

class ZAppServerInfo;

/// ���� ������ �α��� ��û�� ����ִ� Ŭ����
class ZAppServerAcceptor: public ZServerAcceptor
{
private:
	void CreateAppServerObject( MUID uidClient, ZAppServerInfo*	pAppServerInfo);

public:
	ZAppServerAcceptor(MUID uidClient, int nServerID, vector<TD_APP_SERVER_INFO>& vecAppServerList) : ZServerAcceptor(SERVER_APPLICATION, uidClient, nServerID), m_vecAppServerList(vecAppServerList) {}
	virtual ~ZAppServerAcceptor() {}

	bool IsAlreadyExist() override;
	bool Accept() override;
	minet::MCommand* MakeResponseCommand() override;
	minet::MCommand* MakeFailCommand(int nCommandResult) override;

private:
	vector<TD_APP_SERVER_INFO>	m_vecAppServerList;
};
