#ifndef _CCOMMAND_TABLE_LOGIN_H
#define _CCOMMAND_TABLE_LOGIN_H

#include "CSCommonLib.h"

enum CLoginCommandTable
{
	// Ŀ�ǵ� ���� //////////////////////////////////////////////////////////////////////////
	// Ŀ�ǵ� ��ȣ 10001 ~ 12000 ���� ���

// ��Ʈ�� ���(COMM) ---------------------------------------------------------------------------------
	MC_COMM_REQUEST_LOGIN						= 10001,		///< (C->S) �α��� ��û
	MC_COMM_RESPONSE_LOGIN						= 10002,		///< (S->C) �α��� ����

	MC_COMM_REQUEST_LOGIN_GAME_SERVER			= 10011,		///< (C->S) GameServer �α��� ��û
	MC_COMM_RESPONSE_LOGIN_GAME_SERVER			= 10012,		///< (S->C) GameServer �α��� ����

	MC_COMM_ENTER_WORLD_MSG						= 10013,		///< (S->C) ���� ���� ó���� �޽���
	MC_COMM_ENTER_WORLD_LOCAL_MSG				= 10014,		///< (LOCAL) ���� ���� ó���� �޽��� (����)

	MC_COMM_MOVE_GAME_SERVER					= 10021,		///< (S->C) ���Ӽ��� �̵� ���� (From GS To GS)
	MC_COMM_REQUEST_MOVE_GAME_SERVER			= 10022,		///< (C->S) ���Ӽ����̵� ���� Ȯ�� ��û
	MC_COMM_RESPONSE_MOVE_GAME_SERVER			= 10023,		///< (S->C) ���Ӽ����̵� ���� Ȯ�� ����

	MC_COMM_REQUEST_MOVE_LOGIN_SERVER			= 10032,		///< (C->S) �α��μ����̵� ���� Ȯ�� ��û
	MC_COMM_RESPONSE_MOVE_LOGIN_SERVER			= 10033,		///< (S->C) �α��μ����̵� ���� Ȯ�� ����

	//MC_COMM_REQUEST_LOGIN_ON_PMANG				= 10101,		///< (C->S) �Ǹ��� ���� �α��� ��û (MC_COMM_REQUEST_LOGIN �� ������ ���� �Ѵ�.)
	//MC_COMM_RESPONSE_LOGIN_ON_PMANG				= 10102,		///< (S->C) �Ǹ��� ���� �α��� ���� (MC_COMM_RESPONSE_LOGIN �� ������ ���� �Ѵ�.)

	//MC_COMM_REQUEST_LOGIN_GAME_SERVER_ON_PMANG	= 10103,		///< (C->S) �Ǹ����� ��, GameServer �α��� ��û 
	//MC_COMM_REQUEST_MOVE_GAME_SERVER_ON_PMANG	= 10104,		///< (C->S) �Ǹ����� ��, ���Ӽ����̵� ���� Ȯ�� ��û
	//MC_COMM_REQUEST_MOVE_LOGIN_SERVER_ON_PMANG	= 10105,		///< (C->S) �Ǹ����� ��, �α��μ����̵� ���� Ȯ�� ��û

};



// ����,Ŭ���̾�Ʈ ��� �����ϴ� Ŀ�ǵ� ���
CSCOMMON_API void AddLoginCommandTable();


#endif
