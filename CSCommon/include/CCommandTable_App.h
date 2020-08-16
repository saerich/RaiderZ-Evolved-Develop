#pragma once

#include "CSCommonLib.h"

enum CApp2GameCommandTable
{
	// Ŀ�ǵ� ���� //////////////////////////////////////////////////////////////////////////
	// AppServer - GameServer ���� Ŀ�ǵ� ���̺�
	// Ŀ�ǵ� ��ȣ 16001 ~ 18000 ���� ���

	// �α���
	MPC_COMM_REQUEST_LOGIN						= 16001,		///< (G->A) ���Ӽ����� �ۼ����� �α��ο�û
	MPC_COMM_RESPONSE_LOGIN						= 16002,		///< (A->G) �ۼ����κ��� ���Ӽ����� �α��� ����

	MPC_MSG_REQ									= 16010,		///< (G->A) ä�� ��û
	MPC_MSG_RES									= 16011,		///< (A->G) ä�� ����

	MPC_MSG_CREATE_PRIVATE_CHANNEL_REQ			= 16020,		///< (G->A)	�缳 ä�� ä�� ���� ��û

	//////////////////////////////////////////////////////////////////////////
	/// ����ȭ ����
	MPC_MSG_ADDED_CHANNEL_INFO					= 16030,		///< (A->G)	ä�� ä�� ���� ����
	MPC_MSG_DELETED_CHANNEL_INFO				= 16031,		///< (A->G)	ä�� ä�� ���� ����
	MPC_MSG_ADDED_PLAYER_INFO					= 16032,		///< (A->G)	ä�� ä�� �÷��̾� ���� ����
	MPC_MSG_DELETED_PLAYER_INFO					= 16033,		///< (A->G)	ä�� ä�� �÷��̾� ���� ����
	MPC_MSG_PLAYER_GAMESERVER_INFO				= 16034,		///< (A->G)	ä�� ä�� �÷��̾� ���Ӽ��� ����
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	/// ä�� ä�� ���
	MPC_MSG_ENTER_CHANNEL_REQ					= 16050,		///< (G->A)	ä�� ä�� ���� ��û
	MPC_MSG_ENTER_CHANNEL						= 16051,		///< (A->G)	ä�� ä�� ���� ��� �÷��̾�� ����
	MPC_MSG_ENTER_CHANNEL_PLAYER_INFO			= 16052,		///< (A->G)	���� ä���� �ٸ� �÷��̾�鿡�� ���� ����

	MPC_MSG_LEAVE_CHANNEL_REQ					= 16053,		///< (G->A)	ä�� ä�� ���� ��û
	MPC_MSG_LEAVE_CHANNEL						= 16054,		///< (A->G)	ä�� ä�� ���� ��� �÷��̾�� ����
	MPC_MSG_LEAVE_CHANNEL_PLAYER_INFO			= 16055,		///< (A->G)	���� ä���� �ٸ� �÷��̾�鿡�� ���� ����

	MPC_MSG_KICK_REQ							= 16056,		///< (G->A)	kick ��û
	MPC_MSG_KICK								= 16057,		///< (A->G)	kick ��� �÷��̾�� ����
	MPC_MSG_KICK_PLAYER_INFO					= 16058,		///< (A->G)	���� ä���� �ٸ� �÷��̾�鿡�� kick ����

	MPC_MSG_BAN_REQ								= 16059,		///< (G->A)	ban ��û
	MPC_MSG_BAN									= 16060,		///< (A->G)	ban ��� �÷��̾�� ����
	MPC_MSG_BAN_PLAYER_INFO						= 16061,		///< (A->G)	���� ä���� �ٸ� �÷��̾�鿡�� ban ����

	MPC_MSG_UNBAN_REQ							= 16062,		///< (G->A)	unban ��û
	MPC_MSG_UNBAN_RES							= 16063,		///< (A->G)	unban ��� �÷��̾�� ����
	
	MPC_MSG_CHANGE_HOST_REQ						= 16064,		///< (G->A)	���� ���� ��û
	MPC_MSG_CHANGE_HOST_INFO					= 16065,		///< (A->G)	���� ä���� ��� �÷��̾�鿡�� ���� ���� ����
	
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	/// ���� �ý���
	MPC_MAIL_ROUTE_NEW_MAIL_TO_RECEIVER			= 16100,		///< (G->A) ���� �����ο��� �� ���� ����
	MPC_MAIL_TRANS_NEW_MAIL,									///< (A->G) ���� �����ο��� �� ���� ���� �뺸
	MPC_MAIL_RETRY_TRANS_NEW_MAIL,								///< (G->A) ���� �����ο��� �� ���� ������
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	/// ��ǰ�ŷ���
	MPC_GA_TRADEMARKET_ADD						= 16200,		///< (G->A) ������ �߰��Ǿ���
	MPC_GA_TRADEMARKET_DEL,										///< (G->A) ������ ���ŵǾ���
	MPC_GA_TRADEMARKET_SEARCH,									///< (G->A) �˻���û
	MPC_GA_TRADEMARKET_SEARCH_NEXT,								///< (G->A) ���������� �˻�
	MPC_AG_TRADEMARKET_SEARCH_RESULT,							///< (A->G) �˻� ���
	//////////////////////////////////////////////////////////////////////////
};

CSCOMMON_API void AddApp2GameCommandTable();
