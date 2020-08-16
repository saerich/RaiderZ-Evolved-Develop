#pragma once

#include "MNetServer.h"
#include "MCommandHandler.h"

using namespace minet;

class PCmdHandlerGroup;


/// ��Ʈ�� ������. �ַ� Ŀ�ǵ带 ó���Ѵ�.
class PNetServer : public MNetServer
{
private:
	int						m_nRecvTotalCmdCount, m_nSendTotalCmdCount, m_nLocalTotalCmdCount;
	int						m_nRecvCPS, m_nSendCPS, m_nLocalCPS;
	int						m_nRecvCmdCnt, m_nSendCmdCnt, m_nLocalCmdCnt;
	unsigned int			m_nLastCheckTime;

	virtual void OnPrepareCommand(MCommand* pCommand);
	void UpdateCmdCount(MCommand* pCommand);

protected:
	PCmdHandlerGroup*			m_pCmdHandlerGroup;

	virtual void SendCommand(minet::MCommand* pCommand);
	void LogCommand(minet::MCommand* pCommand);
public:
	PNetServer(const MNetServerDesc& desc);
	virtual ~PNetServer();

	int GetRecvCPS() { return m_nRecvCPS; }
	int GetSendCPS() { return m_nSendCPS; }
	int GetLocalCPS() { return m_nLocalCPS; }
};
