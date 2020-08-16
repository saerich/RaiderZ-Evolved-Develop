#pragma once

#include "CSCommonLib.h"

/// MasterServer - GameServer ���� Ŀ�ǵ� ���̺�
/// - Ŀ�ǵ� ��ȣ : 14001 ~ 15000
enum CMaster2GameCommandTable
{
	// ��Ʈ�� ���
	MMC_COMM_REQUEST_LOGIN_G2M						= 14001,	///< (G->M)	���� ������ ������ ������ �α��� ��û
	MMC_COMM_RESPONSE_LOGIN_M2G						= 14002,	///< (M->G) ���� ������ ������ ������ �α��� ����
	MMC_COMM_REQUEST_LOGIN_GAME_SERVER				= 14003,	///< (M->G) �÷��̾� GameServer �α��ο��� ��û
	MMC_COMM_RESPONSE_LOGIN_GAME_SERVER 			= 14004,	///< (G->M) �÷��̾� GameServer �α��ο��� ����
	MMC_COMM_REQUEST_RESERVE_MOVE_GAME_SERVER		= 14005,	///< (G->M) ���Ӽ��� �̵� ���� ��û
	MMC_COMM_RESPONSE_RESERVE_MOVE_GAME_SERVER		= 14006,	///< (M->G) ���Ӽ��� ���� ����
	MMC_COMM_REQUEST_READY_ENTER_MOVE_PLAYER		= 14007,	///< (M->G) �̵� �÷��̾� ���� �غ� ��û
	MMC_COMM_RESPONSE_READY_ENTER_MOVE_PLAYER		= 14008,	///< (G->M) �̵� �÷��̾� ���� �غ� ����
	MMC_COMM_REQUEST_APP_SERVER_LIST				= 14009,	///< (G->M) �ۼ��� ��� ��û
	MMC_COMM_RESPONSE_APP_SERVER_LIST				= 14010,	///< (M->G)	�ۼ��� ��� ����
	MMC_COMM_CANCEL_MOVE_GAME_SERVER				= 14011,	///< (G->M) ���Ӽ��� �̵� ���
	MMC_COMM_CANCEL_RESERVED_MOVE_PLAYER			= 14012,	///< (M->G) ���Ӽ��� �̵� ���� ���
		// GS -> LS �̵�
	MMC_COMM_RESERVE_MOVE_LOGINSERVER_REQ			= 14020,	///< (G->M) �α��μ����� �̵� ���� ��û
	MMC_COMM_RESERVE_MOVE_LOGINSERVER_RES			= 14021,	///< (M->G) �α��μ����� �̵� ���� ����
	MMC_COMM_CANCEL_MOVE_LOGINSERVER				= 14022,	///< (G->M) �α��μ��� �̵� ���

	// Player ����
	MMC_PLAYER_DELETE_PLAYER						= 14101,	///< (G->M) �÷��̾� ���� (�߰��� LoginServer ����)
	MMC_PLAYER_DUPLICATED_PLAYER_LOGIN				= 14102,	///< (M->G)	�÷��̾� �ߺ� �α���

	MMC_PLAYER_STATE_IN_WORLD_REQ					= 14103,	///< (G->M) �÷��̾� ���� ���� ��û - (���� ����)
	MMC_PLAYER_STATE_IN_WORLD_RES					= 14104,	///< (M->G)	�÷��̾� ���� ���� ���� - (���� ����)
	MMC_PLAYER_IN_FIELD_NOTIFY						= 14105,	///< (G->M) �÷��̾ �ʵ忡 ���� ����

	// Field ����
	MMC_FIELD_ADD_SHARED_FIELD						= 14201,	///< (M->G) ���� �ʵ� �߰���
	MMC_FIELD_DEL_SHARED_FIELD						= 14202,	///< (M->G) ���� �ʵ� ���ŵ�
	MMC_FIELD_SHARED_FIELD_INFO_ALL					= 14203,	///< (M->G) ��ü ���� �ʵ� ����
	MMC_FIELD_REQUEST_SHARED_FIELD_INFO_ALL			= 14204,	///< (G->M) ��ü ���� �ʵ� ���� ��û

	MMC_FIELD_ADD_DYNAMIC_FIELD_GROUP				= 14208,	///< (M->G) ���� �ʵ� �׷� �߰���
	MMC_FIELD_REQUEST_DEL_DYNAMIC_FIELD_GROUP		= 14209,	///< (G->M) ���� �ʵ� �׷� ���� ��û
	MMC_FIELD_DEL_DYNAMIC_FIELD_GROUP				= 14210,	///< (M->G) ���� �ʵ� �׷� ���ŵ�
	MMC_FIELD_REQUEST_DYNAMIC_FIELD_GROUP_INFO_ALL	= 14211,	///< (M->G) ��ü ���� �ʵ� �׷� ���� ��û
	MMC_FIELD_DYNAMIC_FIELD_GROUP_INFO_ALL			= 14212,	///< (M->G) ��ü ���� �ʵ� �׷� ����

	MMC_FIELD_UPDATE_PROXY_FIELD_INFO				= 14220,	///< (M->G) ���Ӽ����� Proxy �ʵ� ���� ����

	// Gate ����
	MMC_GATE_ASK_SHARED_FIELD						= 14301,	///< (G->M) ���� �ʵ� �̵� ����
	MMC_GATE_ASK_DYNAMIC_FIELD						= 14302,	///< (G->M) ���� �ʵ� �̵� ����
	MMC_GATE_RES									= 14303,	///< (M->G) �ʵ� �̵� ����
	MMC_GATE_MOVE_SERVER							= 14304,	///< (M->G) �����̵�
	MMC_GATE_PREPARE_CHANGE_CHANNEL_REQ				= 14305,	///< (G->M) ä�� ���� ���� ��û
	MMC_GATE_PREPARE_CHANGE_CHANNEL_RES				= 14306,	///< (M->G) ä�� ���� ���� ����
	MMC_GATE_CANCEL_CHANGE_CHANNEL					= 14307,	///< (G->M) ä�� ���� ���
	MMC_GATE_ASK_CHANGE_CHANNEL						= 14308,	///< (G->M) ä�� ���� ����

	// Party ����	
	MMC_PARTY_ADD									= 14401,	///< (M->G) ��Ƽ �߰�
	MMC_PARTY_REMOVE								= 14402,	///< (M->G) ��Ƽ ����
	MMC_PARTY_ADD_MEMBER							= 14403,	///< (M->G) ��� �߰�
	MMC_PARTY_REMOVE_MEMBER							= 14404,	///< (M->G) ��� ����	

	MMC_PARTY_INVITE_REQ							= 14410,	///< (G->M) ��Ƽ ���� ��û
	MMC_PARTY_INVITE_RES							= 14411,	///< (M->G) ��Ƽ ���� ����	
	MMC_PARTY_ACCEPT_REQ							= 14412,	///< (M->G) ���� ���� ��û
	MMC_PARTY_ACCEPT_RES							= 14413,	///< (G->M) ���� ���� ����
	MMC_PARTY_ACCEPT_CANCEL							= 14414,	///< (M->G) ���� ���� ���

	MMC_PARTY_UPDATE								= 14415,	///< (G->M) ��Ƽ ���� ����
	MMC_PARTY_SYNC									= 14416,	///< (M->G) ��Ƽ ���� ����
	MMC_PARTY_UPDATE_MEMBER							= 14417,	///< (G->M) ��Ƽ�� ���� ����
	MMC_PARTY_MEMBER_SYNC							= 14418,	///< (M->G) ��Ƽ�� ���� ����

	MMC_PARTY_ADD_QUEST								= 14419,	///< (G->M) ����Ʈ �߰�
	MMC_PARTY_QUEST_SYNC							= 14420,	///< (M->G) ����Ʈ ����
	MMC_PARTY_ADD_FIELD								= 14421,	///< (G->M) �ʵ� �߰�
	MMC_PARTY_FIELD_SYNC							= 14422,	///< (M->G) �ʵ� ����
	MMC_PARTY_UPDATE_QUEST_RUNNER					= 14423,	///< (G->M) ����Ʈ ���� ���� ����
	MMC_PARTY_QUEST_RUNNER_SYNC						= 14424,	///< (M->G) ����Ʈ ���� ���� ����

	MMC_PARTY_LEAVE_REQ								= 14428,	///< (G->M) Ż�� ��û
	MMC_PARTY_LEAVE_RES								= 14429,	///< (M->G) Ż�� ����

	MMC_PARTY_REMOVE_QUEST							= 14430,	///< (G->M) ����Ʈ ����
	MMC_PARTY_REMOVE_FIELD							= 14431,	///< (G->M) �ʵ� ����
	MMC_PARTY_ERASE_QUEST_RUNNER					= 14432,	///< (G->M) ����Ʈ ���� ���� ��û
	MMC_PARTY_ERASE_QUEST_RUNNER_SYNC				= 14433,	///< (M->G) ����Ʈ ���� ���� ����

	MMC_PARTY_KICK_REQ								= 14434,	///< (G->M) ���� ��û
	MMC_PARTY_KICK_RES								= 14435,	///< (M->G) ���� ����
	
	MMC_PARTY_DO_OFFLINE							= 14438,	///< (G->M) �������� ���� ����
	MMC_PARTY_DO_ONLINE								= 14439,	///< (G->M) �¶��� ���� ����
	MMC_PARTY_ADD_OFFLINE_MEMBER					= 14440,	///< (M->G) �������� ��Ƽ�� �߰�
	MMC_PARTY_REMOVE_OFFLINE_MEMBER					= 14441,	///< (M->G) �������� ��Ƽ�� ����

	MMC_PARTY_JOIN_INVITE_REQ						= 14442,	///< (G->M) ���� ��û
	MMC_PARTY_JOIN_INVITE_RES						= 14443,	///< (M->G) ���� ����
	MMC_PARTY_JOIN_ACCEPT_REQ						= 14444,	///< (M->G) ���� ��û
	MMC_PARTY_JOIN_ACCEPT_RES						= 14445,	///< (G->M) ���� ����
	MMC_PARTY_JOIN_ACCEPT_CANCEL					= 14446,	///< (M->G) ���� ���

	MMC_PARTY_MOVE_SERVER							= 14447,	///< (G->M) �����̵� ���� �뺸
	MMC_PARTY_MOVE_SERVER_SYNC						= 14448,	///< (M->G) �����̵� �Ϸ� ���� ����

	MMC_PARTY_CREATE_SINGLE_REQ						= 14449,	///< (G->M) 1����Ƽ ���� ��û(Debug)

	MMC_PARTY_INFO_ALL_REQ							= 14450,	///< (G->M) ��ü ��Ƽ���� ��û
	MMC_PARTY_INFO_ALL_RES							= 14451,	///< (M->G) ��ü ��Ƽ���� ����
	MMC_PARTY_REMOVE_FIELD_SELF						= 14452,	///< (G->M) �ʵ� ����

	MMC_PARTY_CHANGE_NAME_REQ						= 14456,	///< (G->M) ��Ƽ �̸� ���� ��û	
	MMC_PARTY_CHANGE_NAME							= 14457,	///< (M->G) ��Ƽ �̸� ����
	
	MMC_PARTY_CHANGE_LEADER_REQ						= 14458,	///< (G->M) ��Ƽ�� ���� ��û
	MMC_PARTY_CHANGE_LEADER							= 14459,	///< (M->G) ��Ƽ�� ����

	MMC_PARTY_CHANGE_LOOTING_RULE_REQ				= 14460,	///< (G->M) ��Ƽ ���÷� ���� ��û
	MMC_PARTY_CHANGE_LOOTING_RULE					= 14461,	///< (M->G) ��Ƽ ���÷� ����
	MMC_PARTY_CHANGE_QUESTID_REQ					= 14462,	///< (G->M) ��Ƽ ��������Ʈ ���� ��û
	MMC_PARTY_CHANGE_QUESTID						= 14463,	///< (M->G) ��Ƽ ��������Ʈ ����
	
	MMC_PARTY_FIXED_LOG_ON							= 14464,	///< (G->M) Expo ��� �غ�� ��Ƽ�� �α׿�
	MMC_PARTY_CREATE_AUTO_PARTY_REQ					= 14465,	///< (G->M) �߱��� ��� ��Ƽ ����� ��û
	MMC_PARTY_JOIN_ATUTO_PARTY_REQ					= 14466,	///< (G->M) �ڵ���Ƽ ����

	MMC_PARTY_FAIL									= 14490,	///< (M->G) ó�� ����


	// Loot
	MMC_LOOT_MASTERLOOT_NOTIFY_REQ					= 14501,	///< (G->M) �����ͷ�˸� ���� ��û	
	MMC_LOOT_MASTERLOOT_NOTIFY						= 14502,	///< (M->G) �����ͷ�˸� ����

	MMC_LOOT_OTHER_GAIN_ITEM_REQ					= 14503,	///< (G->M) �ٸ������ ������ȹ�� ���� ��û
	MMC_LOOT_OTHER_GAIN_ITEM						= 14504,	///< (M->G) �ٸ������ ������ȹ�� ����

	MMC_LOOT_GETTABLE_ITEM_ADD_REQ					= 14505,	///< (G->M) ȹ�氡���� ������ �߰� ���� ��û
	MMC_LOOT_GETTABLE_ITEM_ADD						= 14506,	///< (M->G) ȹ�氡���� ������ �߰� ����

	MMC_LOOT_ROLLRESULT_REQ							= 14507,	///< (G->M) �ֻ��� ���� ���� ��û
	MMC_LOOT_ROLLRESULT								= 14508,	///< (M->G) �ֻ��� ���� ����

	// Guild
	MMC_GUILD_MEMBER_ONLINEINFO_REQ					= 14551,	///< (G->M) ���� ���ӿ��� ���� ��û
	MMC_GUILD_MEMBER_ONLINEINFO						= 14552,	///< (M->G) ���� ���ӿ��� ����

	MMC_GUILD_DESTROY								= 14553,	///< (G->M) ������
	MMC_GUILD_DESTROY_SYNC							= 14554,	///< (M->G) ������ ����

	MMC_GUILD_JOIN									= 14555,	///< (G->M) ��尡��
	MMC_GUILD_JOIN_SYNC								= 14556,	///< (M->G) ��尡�� ����

	MMC_GUILD_LEAVE									= 14557,	///< (G->M) ���Ż��
	MMC_GUILD_LEAVE_SYNC							= 14558,	///< (M->G) ���Ż�� ����

	MMC_GUILD_KICK									= 14559,	///< (G->M) ��尭��
	MMC_GUILD_KICK_SYNC								= 14560,	///< (M->G) ��尭�� ����

	MMC_GUILD_ONLINE								= 14561,	///< (G->M) ��������
	MMC_GUILD_ONLINE_SYNC							= 14562,	///< (M->G) �������� ����

	MMC_GUILD_OFFLINE								= 14563,	///< (G->M) ����������
	MMC_GUILD_OFFLINE_SYNC							= 14564,	///< (M->G) ���������� ����

	MMC_GUILD_MOVEFIELD								= 14565,	///< (G->M) �����ʵ��̵�
	MMC_GUILD_MOVEFIELD_SYNC						= 14566,	///< (M->G) �����ʵ��̵� ����

	MMC_GUILD_CHANGE_MASTER							= 14567,	///< (G->M) �渶����
	MMC_GUILD_CHANGE_MASTER_SYNC					= 14568,	///< (M->G) �渶���� ����

	MMC_GUILD_DEPOSIT_STORAGEMONEY					= 14569,	///< (G->M) ��庸���� �Ա�
	MMC_GUILD_DEPOSIT_STORAGEMONEY_SYNC				= 14570,	///< (M->G) ��庸���� �Ա� ����

	MMC_GUILD_WITHDRAW_STORAGEMONEY					= 14571,	///< (G->M) ��庸���� ���
	MMC_GUILD_WITHDRAW_STORAGEMONEY_SYNC			= 14572,	///< (M->G) ��庸���� ��� ����

	MMC_GUILD_MOVE_STORAGEITEM						= 14573,	///< (G->M) ��庸���� ������ �̵�(�̵�, ��ġ��, �ɰ���)
	MMC_GUILD_MOVE_STORAGEITEM_SYNC					= 14574,	///< (M->G) ��庸���� ������ �̵�(�̵�, ��ġ��, �ɰ���) ����

	// Char
	MMC_CHAR_NPCDIE_REWARD_REQ						= 14601,	///< (G->M) NPC�� �׿��� ���� ���� ��û (����ġ, ����Ʈ, �Ѽ�, ����) 
	MMC_CHAR_NPCDIE_REWARD							= 14602,	///< (M->G) NPC�� �׿��� ���� ����	

	// Msg
	MMC_MSG_ANNOUNCE								= 14701,	///< (M->G) PMS ���� ����

	// Admin
	MMC_ADMIN_CHANGE_SERVER_MODE_REQ				= 14900,	///< (G->M) ���� ��� ���� ��û
	MMC_ADMIN_CHANGE_SERVER_MODE					= 14901,	///< (M->G) ���� ��� ����

	// GM
	MMC_GM_SUMMON_REQ								= 14951,	///< (G->M) �÷��̾� ��ȯ ��û
	MMC_GM_SUMMON									= 14952,	///< (G->M) �÷��̾� ��ȯ


	// Debug
	MMC_DEBUG_DUMP									= 14910,	///< (M->G) ���� ���� ����		
};


/// MasterServer - GameServer Ŀ�ǵ� ���
CSCOMMON_API void AddMaster2GameCommandTable();


/// MasterServer - LoginServer ���� Ŀ�ǵ� ���̺�
/// - Ŀ�ǵ� ��ȣ : 15001 ~ 15500
enum CMaster2LoginCommandTable
{
	// ��Ʈ�� ���
	MLC_COMM_REQUEST_LOGIN					= 15001,	///< (L->M)	�α��� ������ ������ ������ �α��� ��û
	MLC_COMM_RESPONSE_LOGIN					= 15002,	///< (M->L) �α��� ������ ������ ������ �α��� ����
	MLC_COMM_REQUEST_LOGIN_GAME_SERVER		= 15003,	///< (L->M) �÷��̾��� GameServer �α��� ��û
	MLC_COMM_RESPONSE_LOGIN_GAME_SERVER 	= 15004,	///< (M->L) �÷��̾��� GameServer �α��� ����
	MLC_COMM_READY_ENTER_LOGINSERVER_REQ	= 15005,	///< (M->L) �÷��̾� �̵� �غ� ��û From GameServer
	MLC_COMM_READY_ENTER_LOGINSERVER_RES	= 15006,	///< (L->M) �÷��̾� �̵� �غ� ���� From GameServer
	MLC_COMM_CANCEL_MOVE_LOGINSERVER		= 15007,	///< (M->L) �α��μ��� �̵� ��� From GameServer


	// Player ����
	MLC_PLAYER_ADD_PLAYER_REQ				= 15101,	///< (L->M) �÷��̾� �߰� ��û
	MLC_PLAYER_ADD_PLAYER_RES				= 15102,	///< (M->L) �÷��̾� �߰� ����
	MLC_PLAYER_DELETE_PLAYER				= 15103,	///< (L->M) �÷��̾� ����
	MLC_PLAYER_DUPLICATED_PLAYER_LOGIN		= 15104,	///< (M->L) �÷��̾� �ߺ� �α���

	MLC_PLAYER_STATE_SELECT_CHAR			= 15105,	///< (L->M)	�÷��̾� ���� ���� (ĳ���� ����)

	MLC_PLAYER_REQUEST_SET_MYCHAR			= 15110,	///< (L->M) �÷��̾��� ĳ���� ���� ��û
	MLC_PLAYER_RESPONSE_SET_MYCHAR			= 15111,	///< (M->L) �÷��̾��� ĳ���� ���� ����

	// Pmang
	MLC_PLAYER_PMANG_ADD_PLAYER_REQ			= 15112,	///< (L->M) �Ǹ� �÷��̾� �߰� ��û
	MLC_PLAYER_PMANG_ADD_PLAYER_RES			= 15113,	///< (M->L) �Ǹ� �÷��̾� �߰� ����

	// Admin
	MLC_ADMIN_CHANGE_SERVER_MODE			= 15401,	///< (M->L) ���� ��� ����
};

/// MasterServer - LoginServer Ŀ�ǵ� ���
CSCOMMON_API void AddMaster2LoginCommandTable();


/// MasterServer - AppServer ���� Ŀ�ǵ� ���̺�
/// - Ŀ�ǵ� ��ȣ : 15501 ~ 16000
enum CMaster2AppCommandTable
{
	// ��Ʈ�� ���
	MAC_COMM_REQUEST_LOGIN				= 15501,	///< (A->M)	���ø����̼� ������ ������ ������ �α��� ��û
	MAC_COMM_RESPONSE_LOGIN				= 15502,	///< (M->A) ���ø����̼� ������ ������ ������ �α��� ����
};

/// MasterServer - AppServer Ŀ�ǵ� ���
CSCOMMON_API void AddMaster2AppCommandTable();
