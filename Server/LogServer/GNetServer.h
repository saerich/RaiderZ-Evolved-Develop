#ifndef _GNET_SERVER_H
#define _GNET_SERVER_H


#include "MNetServer.h"
#include "MCommandHandler.h"

using namespace minet;

class GCmdHandler_Log;

/// ��Ʈ�� ������. �ַ� Ŀ�ǵ带 ó���Ѵ�.
class GNetServer : public MNetServer
{
private:
	int						m_nRecvTotalCmdCount, m_nSendTotalCmdCount, m_nLocalTotalCmdCount;
	int						m_nRecvCPS, m_nSendCPS, m_nLocalCPS;
	int						m_nRecvCmdCnt, m_nSendCmdCnt, m_nLocalCmdCnt;
	unsigned int			m_nLastCheckTime;

	virtual void OnPrepareCommand(MCommand* pCommand);
	void UpdateCmdCount(MCommand* pCommand);

protected:
	GCmdHandler_Log*			m_pLogCmdHandler;

	virtual void SendCommand(minet::MCommand* pCommand);
	void LogCommand(minet::MCommand* pCommand);
public:
	GNetServer(const MNetServerDesc& desc);
	virtual ~GNetServer();

	int GetRecvCPS() { return m_nRecvCPS; }
	int GetSendCPS() { return m_nSendCPS; }
	int GetLocalCPS() { return m_nLocalCPS; }

};



#endif
