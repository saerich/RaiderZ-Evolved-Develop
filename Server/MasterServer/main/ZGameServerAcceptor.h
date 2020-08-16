#pragma once

#include "ZServerAcceptor.h"
#include "STransData_M2G.h"

class ZGameServerInfo;

/// ���� ������ �α��� ��û�� ����ִ� Ŭ����
class ZGameServerAcceptor : public ZServerAcceptor
{
protected:
	ZGameServerInfo*	m_pGameServerInfo;

	// accept
	void CreateGameServerObject( MUID uidClient );
	bool InitGameServerObject( MUID uidClient );

	// make command
	void MakeParam_SharedFields(vector<TDMG_MANAGED_SHARED_FIELD>& outvecData);
public:
	ZGameServerAcceptor(MUID uidClient, int nServerID) : ZServerAcceptor(SERVER_GAME, uidClient, nServerID), m_pGameServerInfo(NULL) {}

	bool IsAlreadyExist() override;
	bool Accept() override;
	minet::MCommand* MakeResponseCommand() override;
	minet::MCommand* MakeFailCommand(int nCommandResult) override;
};

