#pragma once

#include "MTypes.h"
#include "MNetClient.h"

/// ���� ����
enum XConnectionStatus
{
	CONN_STATUS_DISCONNECT = 0,								///< ���Ӿȵ�

	CONN_STATUS_CONNECTED_TO_LOGIN_SERVER,					///< �α��� ������ ���ӵ�
	CONN_STATUS_CONNECTED_TO_GAME_SERVER,					///< ���� ������ ���ӵ�

	CONN_STATUS_MOVING_FROM_LOGINSERVER_TO_GAMESERVER,		///< �α��� �������� ���� ������ �̵���
	CONN_STATUS_MOVING_FROM_GAMESERVER_TO_GAMESERVER,		///< ���� �������� �ٸ� ���� ������ �̵���
	CONN_STATUS_MOVING_FROM_GAMESERVER_TO_LOGINSERVER,		///< ���� �������� �α��� ������ �̵���

	CONN_STATUS_CONNECTING_TO_GAME_SERVER					// standalone�� ���
};

class XNetClientWrapper
{
protected:
	minet::MNetClient*		m_pNetClient;
public:
	XNetClientWrapper() : m_pNetClient(NULL) {}
	XNetClientWrapper(minet::MNetClient* pNetClient) : m_pNetClient(pNetClient) {}

	virtual minet::MCommand* NewCmd(int nCmdID);
	virtual bool Post(minet::MCommand* pCommand);
	virtual void Disconnect();
	virtual const wchar_t* GetLoginID();
	virtual const wchar_t* GetPassword();
	virtual int GetDirectLoginFieldID();
	virtual vec3 GetStartPos();
};

/// ������ ������ �����ϴ� Ŭ����
class XConnectionManager
{
public:
	struct ToServer
	{
		bool	bFilled;
		wstring	strIP;
		int		nPort;
		MUID	uidConnectionKey;
		ToServer() : bFilled(false), nPort(0), uidConnectionKey(MUID::ZERO) {}
		void Reset()
		{
			bFilled = false;
			nPort = 0;
			uidConnectionKey = MUID::ZERO;
			strIP.clear();
		}
	};
private:
	XNetClientWrapper*		m_pNetClientWrapper;
	XConnectionStatus		m_nStatus;

	ToServer				m_ToServer;

	void StartToMoveServer(wstring strIP, int nPort, MUID uidConnectionKey);
	bool IsStandAloneServer();
	bool IsMovingStatus();
	void PostRequestLogin();
	void PostRequestLoginToGameServer();
	void PostRequestMoveToGameServer();
	void PostRequestMoveToLoginServer();
	void _PostRequestLogin(const wchar_t* szID, const wchar_t* szPassword, int nCommandVersion);
	//void _PostRequestLoginOnPmang(int nCommandVersion);
	void _PostRequestLoginToGameServer(MUID uidConnectionKey, int nCommandVersion);
	//void _PostRequestLoginToGameServerOnPmang(MUID uidConnectionKey, int nCommandVersion);
	void _PostRequestMoveToGameServer(MUID uidConnectionKey, int nCommandVersion);
	//void _PostRequestMoveToGameServerOnPmang(MUID uidConnectionKey, int nCommandVersion);
	void _PostRequestMoveToLoginServer(MUID uidConnectionKey, int nCommandVersion);
	//void _PostRequestMoveToLoginServerOnPmang(MUID uidConnectionKey, int nCommandVersion);

	void _PostGameStart(int nFieldID=-1, vec3& pos=vec3(0,0,0));
	void PostConnectToServer( const wchar_t* szIP, int nPort );

	void _PostEnterWorldMsgLocal(int nResult);
public:
	XConnectionManager(minet::MNetClient* pNetClient);
	XConnectionManager(XNetClientWrapper* pNetClientWrapper);
	~XConnectionManager();

	void OnReplyConnect();
	bool OnDisconnect();
	void OnConnectedToLoginServer();
	void OnConnectedToGameServer();

	void MoveFromLoginServerToGameServer(wstring strIP, int nPort, MUID uidConnectionKey);
	bool MoveFromGameServerToGameServer(wstring strIP, int nPort, MUID uidConnectionKey);
	bool MoveFromGameServerToLoginServer(wstring strIP, int nPort, MUID uidConnectionKey);

	void PostGameStart();

	XConnectionStatus GetStatus() { return m_nStatus; }
};
