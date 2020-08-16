#ifndef MINET_CLIENT_H
#define MINET_CLIENT_H

#include "MiNetLib.h"
#include "MCommandCommunicator.h"
#include "MPacketCrypter.h"
#include "MTCPSocket.h"
#include "MiNetCommon.h"
#include "MTrafficMonitor.h"
#include "MPacketBuilder.h"

namespace minet {

class MCommand;
class MCmdHandler_NetClient;
class MCommandStream;

struct MINET_API MNetClientDesc
{
	bool	bKeepAlive;
	uint32	nKeepAliveTickTime;
	MNetClientDesc() 
		: bKeepAlive(false),
		  nKeepAliveTickTime(60000)
	{}
};

class MINET_API MClientConnectionKeeper
{
protected:
	uint32	m_nSendTime;
public:
	MClientConnectionKeeper() : m_nSendTime(0) {}
	void SetSendTime(uint32 nTime) { m_nSendTime = nTime; }
	bool CheckToSendCommand(uint32 nNowTime, uint32 nKeepAliveTickTime)
	{
		if (m_nSendTime == 0 || nNowTime - m_nSendTime < nKeepAliveTickTime)
		{
			return false;
		}
		m_nSendTime = nNowTime;
		return true;
	}
};

/// Ŭ���̾�Ʈ Ŀ�´�������
class MINET_API MNetClient : public MCommandCommunicator
{
private:
	friend class MCmdHandler_NetClient;

	struct _COMMAND_HANDLER
	{
		MCommandHandler*	m_pHandler;		///< Ŀ�ǵ� �ڵ鷯
		MCommandHanderFunc* m_pFunc;		///< Ŀ�ǵ� �ڵ鷯 �Լ� ������
	};
	typedef std::map<int, _COMMAND_HANDLER> MCommandHandlerMap;
protected:
	MNetClientDesc			m_Desc;
	MUID					m_Server;				///< ����� Ŀ�´�������
	MTCPSocket				m_TCPSocket;			///< Ŭ���̾�Ʈ ���Ͽ� Ŭ����
	MCommandStream*			m_pCommandStream;
	MPacketCrypter			m_PacketCrypter;	///< MatchServer���� ��ȣȭ Ŭ����
	MCmdHandler_NetClient*	m_pCmdHandler;
	MPingData				m_PingData;
	MTrafficMonitor			m_TrafficMonitor;
	MClientPacketBuilder	m_PacketBuilder;
	MClientConnectionKeeper	m_ConnectionKeeper;
	MCommandHandlerMap		m_mapCommandHandler;
protected:
	virtual MCommandResult	OnCommand(MCommand* pCommand) override;
	virtual void			OnPrepareRun() override;


	virtual void			SendCommand(MCommand* pCommand);
	int						MakeCmdPacket(char* pOutPacket, int iMaxPacketSize, MCommand* pCommand);
	void					InitCrypt( MUID& uidHost, MUID& uidAlloc );
	
	// Socket Event - �ظ��ϸ� ��ӹ޾Ƽ� ó������ ���� �����ϴ� LOCAL Ŀ�ǵ带 �̿��ϵ��� ����.
	virtual bool			OnSockConnect(SOCKET sock);
	virtual bool			OnSockDisconnect();
	virtual bool			OnSockRecv(SOCKET sock, void* pPacket, DWORD dwSize);
	virtual void			OnSockError(SOCKET sock, SOCKET_ERROR_EVENT ErrorEvent, int &nErrorCode);

	static bool				SocketRecvEvent(void* pCallbackContext, SOCKET sock, void* pPacket, DWORD dwSize);
	static bool				SocketConnectEvent(void* pCallbackContext, SOCKET sock);
	static bool				SocketDisconnectEvent(void* pCallbackContext);
	static void				SocketErrorEvent(void* pCallbackContext, SOCKET sock, SOCKET_ERROR_EVENT ErrorEvent, int &ErrorCode);

	virtual void			OnReplyConnect(MUID& uidHost, MUID& uidAlloc, unsigned int nTimeStamp);
	virtual void			OnReallocUID(MUID uidAlloc);

	void					CheckConnectionKeeper();
public:
							/// ������
							MNetClient(const MNetClientDesc& desc);
							/// �Ҹ���
	virtual					~MNetClient();
							/// ������ �����Ѵ�.
	virtual int				Connect(const char* szRemoteIP, int nPort, const char* szMyNetworkCardIP = "");
							/// �������� ������ ���´�.
	virtual void			Disconnect();
							/// ������ UID ��ȯ
	MUID					GetServerUID(void)					{ return m_Server; }
							/// ���Ϲ�ȣ�� ������ UID ��ȯ
	MUID					GetSenderUIDBySocket(SOCKET socket);
							/// TCP ���� Ŭ���� ��ȯ
	MTCPSocket*				GetTCPSocket()						{ return &m_TCPSocket; }
							/// ������ �������ִ��� ���θ� ��ȯ
	virtual bool			IsConnected()						{ return m_TCPSocket.IsActive(); }
							/// ���۷� ��ȯ
	void					GetTraffic(int* nSendTraffic, int* nRecvTraffic);

							/// ������ ���� Ŀ�ǵ� ����
	MCommand*				NewSendCommand( int nCmdID );

	void					Ping();
	uint32					GetPing() { return m_PingData.GetPing(); }
	MPingData&				GetPingData() { return m_PingData; }

	void					GetTotalTraffic(int64& noutRecvSize, int64& noutSendSize);
	void					GetSecondTraffic(uint32& noutRecvSize, uint32& noutSendSize);
	
	void					SetCommandHandler(int nCmdID, MCommandHandler* pHandler, MCommandHanderFunc* fnFunc) override;

};


} // namespace minet
#endif
