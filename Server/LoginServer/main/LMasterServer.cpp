#include "StdAfx.h"
#include "LMasterServer.h"

LMasterServer::LMasterServer( const LMasterServerInfo& Info )
: m_Info(Info)
{

}

LMasterServer::~LMasterServer( void )
{

}

void LMasterServer::TryConnection( MUID uidPlayer )
{
	// TODO: �ش� �����ͼ������� ������ ��û�Ѵ� (MC_MASTERSERVER_PLAYER_CONNECTION_REQ)
}