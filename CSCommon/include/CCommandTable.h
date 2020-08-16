#ifndef _CCOMMAND_TABLE_H
#define _CCOMMAND_TABLE_H

#include "CSCommonLib.h"

// ���̴��� Ŀ�ǵ� ����
#define SH_COMMAND_VERSION				127

// ----------------------------------------------------------------------
// Ŀ�ǵ� ���̺� ID �Ҵ�
//
// Client - GameServer : 1001 ~ 10000 (CCommandTable)
// LoginServer - MasterServer : 10001 ~ 12000 (CCommandTable_Login)
// GameServer - LogServer : 12001 ~ 14000 (CCommandTable_Log)
// MasterServer - All Server : 14001 ~ 16000 (CCommandTable_Masater)
// GameServer - AppServer : 16001 ~ 18000 (CCommandTable_App)
// ServerCommon : 18001 ~ 19000 (SCommandTable)
// Client - WorldLocator : 20000 ~ 21000 (CCommandTable_WorldLocator)
// ----------------------------------------------------------------------


enum CCommandTable
{
// Ŀ�ǵ� ���� //////////////////////////////////////////////////////////////////////////
// Ŀ�ǵ� ��ȣ 1001 ~ 10000 ���� ���

// ��Ʈ�� ���(COMM) ---------------------------------------------------------------------------------
	MC_COMM_REQUEST_ACCOUNT_CHAR_LIST			= 1011,		///< (C->S) ĳ���� ����Ʈ ��û
	MC_COMM_RESPONSE_ACCOUNT_CHAR_LIST			= 1012,		///< (S->C) ĳ���� ����Ʈ ����
	MC_COMM_MOVE_TO_GAME_SERVER					= 1013,		///< (S->C) ���Ӽ����� �̵��ض� (From LS To GS)
	MC_COMM_DUPLICATED_PLAYER_LOGIN				= 1014,		///< (S->C) �ٸ� ������ �ߺ����� �α���
	MC_COMM_START_MOVE_TO_LOGIN_SERVER_REQ		= 1015,		///< (C->S) ĳ���� ���� ȭ������ �̵� ���� ��û
	MC_COMM_START_MOVE_TO_LOGIN_SERVER_RES		= 1016,		///< (S->C) ĳ���� ���� ȭ������ �̵� ���� ����

// ĳ���� ���� --------------------------------------------------------------------------------------
	MC_CHAR_REQUEST_INSERT_CHAR		= 1020,		///< (C->S) ĳ���� ���� ��û
	MC_CHAR_RESPONSE_INSERT_CHAR,				///< (S->C) ĳ���� ���� ����
	MC_CHAR_REQUEST_DELETE_CHAR,				///< (C->S) ĳ���� ���� ��û
	MC_CHAR_RESPONSE_DELETE_CHAR,				///< (S->C) ĳ���� ���� ����

	MC_CHAR_REQUEST_SEL_MYCHAR,					///< (C->S) �� ĳ���� ����
	MC_CHAR_RESPONSE_SEL_MYCHAR,				///< (S->C) ĳ���� ���� ����

	MC_CHAR_GAMESTART,							///< (C->S) ���� ��������

// ĳ����(CHAR) --------------------------------------------------------------------------------------
	MC_CHAR_MYINFO					= 1040,		///< (S->C) �� ���� ����
	MC_CHAR_MYTIMEINFO,							///< (S->C) �� �ð� ���� ���� (���� �ð�, �ŷ�Ʈ ��Ÿ��, ��Ȱ ���ð�)
	MC_CHAR_REBIRTH_REQ,						///< (C->S) ��Ȱ ��û [����ȭ]
	MC_CHAR_REBIRTH_MYPLAYER,					///< (S->C) ��Ȱ ���� �˸� [����ȭ]
	MC_CHAR_REBIRTH_NETPLAYER,					///< (S->C) ��Ȱ �ٸ��̵鿡�� �˸� [����ȭ]
	MC_CHAR_REWARD_EXP,							///< (S->C) ����ġ ȹ��
	MC_CHAR_LEVEL_UP,							///< (S->C) ������
	MC_CHAR_UPDATE_SIMPLE_STATUS,				///< (S->C) ����(HP, EN)�� ��ȭ�ߴ�. [����ȭ]
	MC_CHAR_SAVE_SOULBINDING,					///< (S->C) �ҿ� ���ε� ����
	MC_CHAR_SAVE_CHECKPOINT,					///< (S->C) üũ����Ʈ ����
	MC_CHAR_UPDATE_MONEY,						///< (S->C) �ǹ� ȹ��
	MC_CHAR_ENEMY_INFO,							///< (S->C) ������ HP ����
	MC_CHAR_ENEMY_INFO_CLEAR,					///< (S->C) ������ ������
	MC_CHAR_UPDATE_TP,							///< (S->C) TP ����
	MC_CHAR_TIP,								///< (S->C) ���ο� Narration
	MC_CHAR_ADD_ENEMYNPC,						///< (S->C) ��׷θ� ���� NPC�� �߰���
	MC_CHAR_DEL_ENEMYNPC,						///< (S->C) ��׷θ� ���� NPC�� ������
	MC_CHAR_TARGET_INFO_REQ,					///< (C->S) ������ ���� ���õ� ����� ���� ��û
	MC_CHAR_TARGET_INFO,						///< (S->C) ������ ���� ���õ� ����� ����
	MC_CHAR_DIE_REQ,							///< (C->S) �ڻ� ��û

// ���� ///////////////
	MC_FIELD_IN_PLAYER				= 1115,		///< (S->C) �÷��̾� ���� [����ȭ]
	MC_FIELD_OUT_PLAYER				= 1116,		///< (S->C) �÷��̾� ���� [����ȭ]
	MC_FIELD_IN_NPC					= 1117,		///< (S->C) NPC ����
	MC_FIELD_OUT_NPC				= 1118,		///< (S->C) NPC ���� [����ȭ]
	MC_FIELD_IN_BUFFENTITY			= 1119,		///< (S->C) ������ ���� [����ȭ]
	MC_FIELD_OUT_BUFFENTITY			= 1120,		///< (S->C) ������ ������ [����ȭ]
	MC_FIELD_TRAP_TRIGGERED			= 1121,		///< (S->C) Ʈ�� ��Ƽ� ������ [����ȭ]
	MC_FIELD_SECTOR_ENTITY_INFO		= 1123,		///< (S->C) ���� ��ƼƼ ����
	MC_FIELD_SET_TIME				= 1124,		///< (S->C) ���� �ð� ���� [����ȭ]
	MC_FIELD_SET_WEATHER			= 1125,		///< (S->C) ���� ���� ���� [����ȭ]
	MC_FIELD_SET_TIME_WEATHER		= 1126,		///< (S->C) �ð�, ���� ���� [����ȭ]
	MC_FIELD_WHO_IS_REQ				= 1127,		///< (C->S) �ش� UID�� ������ (UID)
	MC_FIELD_WHO_IS_UIID_REQ		= 1128,		///< (C->S) �ش� UID�� ������ (UIID)
	MC_FIELD_PLAYER_EXTRA_INFO_REQ	= 1129,		///< (C->S) �÷��̾� �߰� ���� �ּ���
	MC_FIELD_IN_BPART				= 1130,		///< (S->C) BPART ����
	MC_FIELD_OUT_BPART				= 1131,		///< (S->C) BPART ����

// Ŀ�´�Ƽ ///////////
	MC_MSG_REQ						= 1200,		///< (C->S) ��ȭ �߽�
	MC_MSG_RES,									///< (S->C) ��ȭ ����
	
	MC_MSG_CREATE_CHANNEL_REQ,					///< (C->S) ä�� ä�� ���� ��û	
	MC_MSG_ENTER_CHANNEL_REQ,					///< (C->S) ä�� ä�� ���� ��û
	MC_MSG_ENTER_CHANNEL,						///< (S->C) ä�� ä�� ����
	MC_MSG_LEAVE_CHANNEL_REQ,					///< (C->S) ä�� ä�� ���� ��û
	MC_MSG_LEAVE_CHANNEL,						///< (S->C) ä�� ä�� ����
	MC_MSG_KICK_PLAYER_REQ,						///< (C->S) ä�� ä�� �÷��̾� kick ��û
	MC_MSG_KICK_PLAYER,							///< (S->C) ä�� ä�� �÷��̾� kick
	MC_MSG_BAN_PLAYER_REQ,						///< (C->S) ä�� ä�� �÷��̾� ban ��û
	MC_MSG_BAN_PLAYER,							///< (S->C) ä�� ä�� �÷��̾� ban	
	MC_MSG_UNBAN_PLAYER_REQ,					///< (C->S) ä�� ä�� �÷��̾� unban ��û	
	MC_MSG_UNBAN_PLAYER,						///< (S->C) ä�� ä�� �÷��̾� unban
	MC_MSG_CHANGE_HOST_REQ,						///< (C->S) ä�� ä�� ȣ��Ʈ ���� ��û
	MC_MSG_CHANGE_HOST,							///< (S->C) ä�� ä�� ȣ��Ʈ ����

	MC_MSG_ENTER_CHANNEL_PLAYER_INFO,			///< (S->C) ä�� ä�� �����÷��̾� ����
	MC_MSG_LEAVE_CHANNEL_PLAYER_INFO,			///< (S->C) ä�� ä�� �����÷��̾� ����
	MC_MSG_KICK_PLAYER_INFO,					///< (S->C) ä�� ä�� �����÷��̾� ����

// �ʵ� �̵�(FIELD) ----------------------------------------------------------------------------------
	MC_FIELD_PREPARING_TO_LOAD		= 1331,		///< (S->C) �ε� �غ��ض�
	MC_FIELD_LOADING_COMPLETE		= 1332,		///< (C->S) �ε� �Ϸ�(���� �ε� ȭ��)
	MC_FIELD_START_GAME				= 1333,		///< (S->C) ���� ����(�ε�ȭ�鿡�� ����ȭ������ ��ȯ)
	MC_FIELD_CHANGE_FIELD			= 1334,		///< (S->C) �÷��̾� �ʵ� �̵� (����/�ν��Ͻ� ����)
	MC_FIELD_PREPARE_CHANGE_CHANNEL_REQ	= 1335,		///< (C->S) �÷��̾� ä�� ���� �غ� ��û
	MC_FIELD_PREPARE_CHANGE_CHANNEL_RES	= 1336,		///< (S->C) �÷��̾� ä�� ���� �غ� ����
	MC_FIELD_CANCEL_CHANGE_CHANNEL	= 1337,		///< (C->S) �÷��̾� ä�� ���� �غ� ��û
	MC_FIELD_REQUEST_CHANGE_CHANNEL	= 1338,		///< (C->S) �÷��̾� ä�� ���� ��û (�غ� ��Ŷ �Ŀ� 10�� �ڿ� ������ ��)
	MC_FIELD_REQUEST_CHANNEL_LIST	= 1339,		///< (C->S) �÷��̾� ä�� ��� ��û
	MC_FIELD_CHANNEL_LIST			= 1340,		///< (S->C) �÷��̾� ä�� ���
	MC_FIELD_CANCEL_CHANGE_FIELD	= 1341,		///< (S->C) �ʵ� �̵� ���

	MC_DYNAMIC_FIELD_ENTER_REQ		= 1350,		///< (C->S) �÷��̾� ���̳��� �ʵ� ���� (����/�ν��Ͻ� ����)
	MC_DYNAMIC_FIELD_LEAVE_REQ		= 1351,		///< (C->S) �÷��̾� ���̳��� �ʵ� ������
	MC_DYNAMIC_FIELD_EXPIRING		= 1352,		///< (S->C) �����ʵ� �ı��� �˸�
	MC_DYNAMIC_FIELD_CANCEL_EXPIRE	= 1353,		///< (S->C) �����ʵ� �ı� cancel


// �׼�(ACTION) --------------------------------------------------------------------------------------
	MC_ACTION_FAILED				= 1401,		///< (S->C) Ŭ���̾�Ʈ�� ��û�� �׼��� ���еǾ���.

	MC_ACTION_MOVE_REQ				= 1411,		///< (C->S) �ʵ忡�� �̵� ��û [����ȭ]
	MC_ACTION_MOVE					= 1412,		///< (S->C) �̵� [����ȭ]
	MC_ACTION_STOP_REQ				= 1413,		///< (C->S) �ʵ忡�� ���� ��û [����ȭ]
	MC_ACTION_STOP					= 1414,		///< (S->C) ���� [����ȭ]
	MC_ACTION_STOP_MOVE_REQ			= 1415,		///< (C->S) �÷��̾� ���� ��û [����ȭ]
	MC_ACTION_STOP_MOVE				= 1416,		///< (S->C) �÷��̾� ���� [����ȭ]
	MC_ACTION_ROTATE_REQ			= 1417,		///< (C->S) ���� ���� ��û [����ȭ]
	MC_ACTION_ROTATE				= 1418,		///< (S->C) ���� ���� [����ȭ]

	MC_ACTION_CHANGE_STANCE_REQ		= 1419,		///< (C->S) ���Ľ� ���� [����ȭ]
	MC_ACTION_CHANGE_STANCE			= 1420,		///< (S->C) ���Ľ� ���� [����ȭ]

	MC_ACTION_ATTACK_MELEE_REQ		= 1421,		///< (C->S) �÷��̾� ����
	MC_ACTION_ATTACK_ARCHERY_REQ	= 1423,		///< (C->S) Ȱ�� ���� ��û
	MC_ACTION_ATTACK_ARCHERY		= 1424,		///< (S->C) Ȱ�� ����
	MC_ACTION_ATTACK_ARCHERY_DETAIL_REQ	= 1425,		///< (C->S) Ȱ�� ���� ��û
	MC_ACTION_ATTACK_ARCHERY_DETAIL		= 1426,		///< (S->C) Ȱ�� ����
	MC_ACTION_ATTACK_GUN_REQ		= 1427,		///< (C->S) ������ ���� ��û
	MC_ACTION_ATTACK_GUN			= 1428,		///< (S->C) ������ ����

	MC_ACTION_ATTACK_CANCEL_REQ		= 1429,		///< (C->S) ���� ��� [����ȭ]
	MC_ACTION_ATTACK_CANCEL			= 1430,		///< (S->C) ���� ��� [����ȭ]

	MC_ACTION_PREPARE_ATTACK_REQ	= 1431,		///< (C->S) �÷��̾� ���� �غ� (Ȱ ���� �����) [����ȭ]
	MC_ACTION_PREPARE_ATTACK		= 1432,		///< (S->C) �÷��̾� ���� �غ� [����ȭ]

	MC_ACTION_GUARD_REQ				= 1433,		///< (C->S) ���� ���� ��û [����ȭ]
	MC_ACTION_GUARD					= 1434,		///< (S->C) ���� ���� [����ȭ]
	MC_ACTION_GUARD_FAILED			= 1435,		///< (S->C) ���� ����
	MC_ACTION_GUARD_RELEASED_REQ	= 1436,		///< (C->S) ���� ���� ��û [����ȭ]
	MC_ACTION_GUARD_RELEASED		= 1437,		///< (S->C) ���� ���� [����ȭ]


	MC_ACTION_GUARD_DEFENSE			= 1440,		///< (S->C) ��� ���� [����ȭ]
	MC_ACTION_GUARD_DEFENSE_PARTIAL	= 1441,		///< (S->C) ��� ���� [����ȭ]

	MC_ACTION_USE_TALENT_REQ		= 1442,		///< (C->S) �ŷ�Ʈ ��� ��û [����ȭ]
	MC_ACTION_USE_TALENT			= 1443,		///< (S->C) �ŷ�Ʈ ���(��� ��� �ŷ�Ʈ�� ��� ���� ���, �ƴ� ��� �ŷ�Ʈ �غ� �ܰ�) [����ȭ]
	MC_ACTION_ACT_TALENT_REQ		= 1444,		///< (S->C) �ŷ�Ʈ ���� ��� [����ȭ]
	MC_ACTION_ACT_TALENT_WITH_HITCAPSULE_REQ = 1445,	///< (S->C) �ŷ�Ʈ ���� ��� (��Ʈĸ�� ���� ����)
	MC_ACTION_ACT_TALENT_WITH_GROUND_REQ	 = 1446,	///< (S->C) �ŷ�Ʈ ���� ��� (�����ǥ ���� ����)
	MC_ACTION_ACT_TALENT			= 1447,		///< (S->C) �ŷ�Ʈ ���� ��� ��û [����ȭ]
	MC_ACTION_ACT_TALENT_WITH_GROUND = 1448,		///< (S->C) �ŷ�Ʈ ���� ��� ��û (���� ����) [����ȭ]
	MC_ACTION_CANCEL_TALENT_REQ		= 1449,		///< (C->S) �ŷ�Ʈ ��� ��û [����ȭ]
	MC_ACTION_CANCEL_TALENT			= 1450,		///< (S->C) �ŷ�Ʈ ��� [����ȭ]
	MC_ACTION_END_TALENT_COOLDOWN	= 1451,		///< (S->C) ��ٿ� ��. ���� �ŷ�Ʈ ��� �����ϴ�. �Ķ��Ÿ�� �ŷ�Ʈ ID [����ȭ]
	MC_ACTION_EXTRA_ACT_TALENT		= 1452,		///< (S->C) �ŷ�Ʈ Extra Phase�� �̵� [����ȭ]
	MC_ACTION_REQUEST_FAIL_TALENT	= 1453,		///< (S->C) �ŷ�Ʈ ��� ��û ���� [����ȭ]

	MC_ACTION_ACT_TALENT_PROJECTILE	= 1454,		///< (S->C) ���� ���2 (ACT_SPELL�� ���� �뵵������ �Ķ��Ÿ�� TargetUIDList��� ��ǥ�� ��ġ�� ����Ѵ�.)
	MC_ACTION_ACT_SPELL_MAGICAREA	= 1455,		///< (S->C) ���� ���3 (ACT_SPELL�� ���� �뵵������ �Ķ��Ÿ�� TargetUIDList��� ��ǥ�� ��ġ�� ����Ѵ�.) [����ȭ]
	MC_ACTION_RECASTING_SPELL		= 1456,		///< (S->C) ���� ĳ���� ���� (EN�� �����Ͽ� ĳ������ ������� Ǭ��.) [����ȭ]
	
	MC_ACTION_TALENT_HIT				= 1460,		///< (S->C) �ŷ�Ʈ ����(�������� ���� �� ��ɾ� ���) [����ȭ]
	MC_ACTION_TALENT_HIT_NO_MF			= 1461,		///< (S->C) �ŷ�Ʈ ����(������� �� ���� �������� ���) [����ȭ]
	MC_ACTION_TALENT_HIT_KNOCKBACK_MF	= 1462,		///< (S->C) �ŷ�Ʈ ����(�˹�, ���ο� �� �������) [����ȭ]
	MC_ACTION_TALENT_HEAL				= 1463,		///< (S->C) �ŷ�Ʈ�� ȸ��

	MC_ACTION_BUFF_HIT				= 1464,		///< (S->C) ���� ����(�������� ���� �� ��ɾ� ���) [����ȭ]
	MC_ACTION_BUFF_HIT_NO_MF		= 1465,		///< (S->C) ���� ����(������� �� ���� �������� ���) [����ȭ]
	MC_ACTION_TALENT_HIT_MISS		= 1466,		///< (S->C) �ŷ�Ʈ ȸ����(ȸ������ ��) [����ȭ]
	MC_ACTION_TALENT_DRAIN			= 1467,		///< (S->C) �ŷ�Ʈ�� ���� ������ ����� [����ȭ]

	MC_ACTION_BREAK_PART			= 1470,		///< (S->C) NPC�� �극��Ŀ�� ���� �ı���
	MC_ACTION_BPART_RECOVERY		= 1471,		///< (S->C) NPC�� �극��Ŀ�� ����
	MC_ACTION_BREAK_PART_EX			= 1472,		///< (S->C) NPC�� �극��Ŀ�� ���� �ı��� (��밡���� ���� ����)

	MC_ACTION_JUMP_REQ				= 1480,		///< (C->S) ���� ��û [����ȭ]
	MC_ACTION_JUMP					= 1481,		///< (S->C) �÷��̾� ���� [����ȭ]
	MC_ACTION_FALLING_REQ			= 1482,		///< (C->S) �� �������� [����ȭ]
	MC_ACTION_FALLING				= 1483,		///< (S->C) �÷��̾� ������ [����ȭ]
	MC_ACTION_END_FALLING			= 1484,		///< (C->S) �� �������� ���� ���� [����ȭ]
	MC_ACTION_FALLING_DAMAGE		= 1485,		///< (S->C) ���� ������ [����ȭ]

	MC_ACTION_DIE					= 1490,		///< (S->C) ��ƼƼ ����	[����ȭ]
	MC_ACTION_DESPAWN				= 1492,		///< (S->C) ��ƼƼ �Ҹ� [����ȭ]

	MC_ACTION_SWIM_REQ				= 1493,		///< (C->S) ����(or ���� ����) ��û [����ȭ]

	MC_ACTION_CANCEL_NEXT_ATTACK	= 1500,		///< (S->C) ���� ������ ĵ���϶� [����ȭ]

	MC_ACTION_STANDUP_REQ			= 1501,		///< (C->S) �˴ٿ�� ��� ��û [����ȭ]
	MC_ACTION_STANDUP				= 1502,		///< (S->C) �˴ٿ�� ��� [����ȭ]

	MC_ACTION_POS_SYNC_REQ			= 1503,		///< (C->S) MoveReq �̿��� ��ġ ���Ⱑ �ʿ��ϸ� �� Ŀ�ǵ带 ����Ѵ�. [����ȭ]
	MC_ACTION_MOVING_MF_POS_SYNC_REQ  = 1504,	///< (C->S) ��� ���Ͱ� ������ ���� ��ġ ���� ���߱� ��û [����ȭ]

	MC_ACTION_SWITCHING_WEAPON_BEGIN_REQ	= 1510,		///< (C->S) �ֹ���/�������� ���� ���� [����ȭ]
	MC_ACTION_SWITCHING_WEAPON_BEGIN		= 1511,		///< (S->C) �ֹ���/�������� ���� ���� [����ȭ]
	MC_ACTION_SWITCHING_WEAPON_REQ	= 1512,		///< (C->S) �ֹ���/�������� ���� [����ȭ]
	MC_ACTION_SWITCHING_WEAPON		= 1513,		///< (S->C) �ֹ���/�������� ���� [����ȭ]

	MC_ACTION_SIT_REQ				= 1514,		///< (C->S) �ɱ� ��û [����ȭ]
	MC_ACTION_SIT					= 1515,		///< (S->C) �ɱ� [����ȭ]

	MC_ACTION_SIT_RISE_REQ			= 1516,		///< (C->S) ���� ��û [����ȭ]
	MC_ACTION_SIT_RISE				= 1517,		///< (S->C) ���� [����ȭ]

	MC_ACTION_HIDE					= 1520,		///< (S->C) ����ȭ - ����
	MC_ACTION_AFK_BEGIN				= 1521,		///< (S->C) �ڸ���� ����
	MC_ACTION_AFK_END				= 1522,		///< (S->C) �ڸ���� ��
	MC_ACTION_AFK_REQ				= 1523,		///< (C->S) �ڸ���� ��û
	MC_ACTION_END_COMBAT			= 1524,		///< (S->C) ���� ����

	// Ʈ���� Ŀ�ǵ�(TRIGGER) -----------------------------------------------------------------------------
	MC_TRIGGER_TOUCH_SENSOR_ENTER			= 1531,		///< (C->S) ��ġ ���� �������� ����
	MC_TRIGGER_TOUCH_SENSOR_LEAVE			= 1532,		///< (C->S) ��ġ ���� �������� ����
	MC_TRIGGER_UPDATE_USABLE_SENSOR			= 1533,		///< (S->C) ��밡���� ���� ���� ����
	MC_TRIGGER_SENSOR_DESTINATION_SEL_REQ	= 1534,		///< (S->C) ������ ���� ������ ���� ��û
	MC_TRIGGER_SENSOR_DESTINATION_SEL		= 1535,		///< (C->S) ������ ���� ������ ����
	MC_TRIGGER_SENSOR_INTERACTION_REQ		= 1536,		///< (C->S) �������� ���ͷ��� �õ�
	MC_TRIGGER_SENSOR_ERROR_MSG				= 1537,		///< (S->C) ���� ������ ���� �뺸
	MC_TRIGGER_SENSOR_FOLLOW_PARTY_REQ		= 1538,		///< (S->C) ������ ���� ��Ƽ�� �̵� ��, �i�ư��� ����Ȯ��
	MC_TRIGGER_SENSOR_FOLLOW_PARTY_RES		= 1539,		///< (C->S) ������ ���� ��Ƽ�� �̵� ��, �i�ư��� ���ΰ���

// ��� ��ü ��ɾ�
	MC_ENTITY_WARP					= 1541,		///< (S->C) Entity ���� �̵�

// NPC ���� Ŀ�ǵ�(NPC) --------------------------------------------------------------------------------
	MC_NPC_MOVE						= 1601,		///< (S->C) NPC �̵�(�ȱ�) [����ȭ]
	MC_NPC_FACE_TO					= 1602,		///< (S->C) NPC �ٷκ��� ���� ���� [����ȭ]
	MC_NPC_RUN						= 1603,		///< (S->C) NPC �̵�(�޸���) [����ȭ]
	MC_NPC_MOVE_STOP				= 1604,		///< (S->C) NPC �̵� ���� [����ȭ]
	MC_NPC_CHANGE_CAPSULE			= 1609,		///< (S->C) NPC�� �浹 ĸ���� ����
	MC_NPC_SAY						= 1610,		///< (S->C) NPC ���ϱ�
	MC_NPC_BALLOON					= 1611,		///< (S->C) NPC ��ǳ��
	MC_NPC_SHOUT					= 1612,		///< (S->C) NPC ��ġ��
	MC_NPC_NARRATION				= 1613,		///< (S->C) �����̼�
	MC_NPC_SAY_AND_NARRATION		= 1614,		///< (S->C) NPC ��ǳ���� �����̼��� ���ÿ� ���ϱ�
	MC_NPC_CHANGE_ATTACKABLE		= 1615,		///< (S->C) NPC���� ���� ���� ���� ����
	MC_NPC_ENABLE_INTERACT			= 1616,		///< (S->C) ���ͷ��ǰ��� ���� ����
	MC_NPC_CHANGE_COMBAT_TARGET		= 1617,		///< (S->C) �ο� ��ǥ ���Ͱ� �����
	MC_NPC_SPEWUP					= 1618,		///< (S->C) NPC�� ������ ���
	MC_NPC_SPEWUP_INTERRUPT			= 1619,		///< (S->C) NPC�� ������ ������ �޾� ���


// ������(ITEM) ----------------------------------------------------------------------------------------
	MC_FIELD_REQUEST_FEATURE		= 1701,		///< (C->S) �ܸ� ���� ��û
	MC_FIELD_FEATURE				= 1702,		///< (S->C) �ܸ� ���� (���������)

	MC_ITEM_EQUIP_REQ				= 1703,		///< (C->S) ������ ���� ��û
	MC_ITEM_EQUIP					= 1704,		///< (S->C) ������ ����
	MC_ITEM_EQUIP_SWAPTOINVEN		= 1705,		///< (S->C) �κ��� �ִ� �������� �����ϸ鼭 ������ �������� �κ���ġ�� swap

	MC_ITEM_UNEQUIP_REQ				= 1710,		///< (C->S) ������ ���� ���� ��û
	MC_ITEM_UNEQUIP					= 1711,		///< (S->C) ������ ���� ����

	MC_ITEM_CHANGE_LOOK_EQUIP_ITEM	= 1712,		///< (S->C) ������ ���� ���� �˸�
	MC_ITEM_CHANGE_LOOK_UNEQUIP_ITEM= 1713,		///< (S->C) ������ ���� ���� �˸�

	MC_ITEM_USE_REQ					= 1714,		///< (C->S) ������ ��� ��û
	MC_ITEM_USE_FAIL				= 1715,		///< (S->C) ������ ��� ����
	MC_ITEM_TALENT					= 1716,		///< (S->C) ������ �ŷ�Ʈ ��� ���
	
	MC_ITEM_ADD						= 1717,		///< (S->C) ������ ȹ��
	MC_ITEM_DROP_REQ				= 1718,		///< (C->S) ������ ���� ��û
	MC_ITEM_REMOVE					= 1719,		///< (S->C)	������ ����	

	MC_ITEM_UPDATE_DURABILITY		= 1722,		///< (S->C)	������ ������ ����

	MC_ITEM_INTERACT_NPC_REQ		= 1723,		///< (C->S) �������� ���ͷ��� ��û.

	MC_ITEM_MOVE_REQ				= 1726,		///< (C->S) ������ �̵�(�̵�, ��ġ��, �ɰ���) ��û
	MC_ITEM_MOVE					= 1727,		///< (S->C) ������ �̵�

	MC_ITEM_SORT_INVEN_SLOT_REQ		= 1730,		///< (C->S) ������ �κ��丮 ��ġ ���� ��û
	MC_ITEM_SORT_INVEN_SLOT			= 1731,		///< (S->C) ������ �κ��丮 ��ġ ����

	MC_ITEM_DYE_PREPARE_REQ			= 1740,		///< (C->S)	������ ���� �غ� ��û
	MC_ITEM_DYE_PREPARECANCEL_REQ	= 1741,		///< (S->C)	������ ���� �غ� ��� ��û
	MC_ITEM_DYE_REQ					= 1742,		///< (C->S)	������ ���� ��û
	MC_ITEM_DYE						= 1743,		///< (S->C)	������ ����	


// ����Ʈ(QUEST) ---------------------------------------------------------------------------------------
	MC_QUEST_GIVE_REQ				= 1801,		///< (C->S) ����Ʈ ���� ��û
	MC_QUEST_GIVE					= 1802,		///< (S->C) ����Ʈ ���� ����
	MC_QUEST_UPDATE_OBJECTIVE		= 1803,		///< (S->C) ������Ƽ�� �޼� ���� ���� �˸�
	MC_QUEST_COMPLETE				= 1804,		///< (S->C) ������Ƽ�갡 ��� ����Ʈ�� ���ڸ��� �Ϸ�
	MC_QUEST_INCOMPLETE				= 1805,		///< (S->C) ������Ƽ�갡 ��� ����Ʈ�� ���ڸ��� �Ϸ�
	MC_QUEST_FAIL					= 1806,		///< (S->C) ����Ʈ ����
	MC_QUEST_REWARD_REQ				= 1807,		///< (C->S) ����Ʈ ���� ��û
	MC_QUEST_REWARD					= 1808,		///< (S->C) ����Ʈ ���� ����
	MC_QUEST_CANCEL_REQ				= 1809,		///< (C->S) ����Ʈ ��� ��û
	MC_QUEST_CANCEL					= 1810,		///< (S->C) ����Ʈ ��� ����
	MC_QUEST_SHOW_ACCPET_UI			= 1812,		///< (S->C) ����Ʈ ���� UI �����ֱ�
	MC_QUEST_SHOW_REWARD_UI			= 1813,		///< (S->C) ����Ʈ ���� UI �����ֱ�
	
	MC_QUEST_SHARE_REQ				= 1814,		///< (C->S) ����Ʈ ���� ��û
	MC_QUEST_SHARE_REQ_RELAY		= 1815,		///< (S->C) ����Ʈ ���� ��û ����
	MC_QUEST_SHARE					= 1816,		///< (S->C) ����Ʈ ���� ���
	MC_QUEST_SHARE_GIVE_REQ			= 1818,		///< (C->S) ����Ʈ ������ ���� ����Ʈ ����  ��û

	MC_QUEST_INTERACT_QOBJECTIVE_BEGIN	= 1819,		///< (S->C) ���ͷ������� ����Ʈ ���� ���� ����
	MC_QUEST_INTERACT_QOBJECTIVE_CANCEL_REQ	= 1820,	///< (C->S) ���ͷ������� ����Ʈ ���� ���� ��� ��û
	MC_QUEST_INTERACT_QOBJECTIVE_END	= 1821,		///< (S->C) ���ͷ������� ����Ʈ ���� ���� ����

	MC_CHALLENGER_QUEST_LIST		= 1822,		///< (S->C) ���ͷ������� ����Ʈ ���� ���� ����
	MC_CHALLENGER_QUEST_COMPLETE	= 1823,		///< (S->C) ����������Ʈ �Ϸ�
	MC_CHALLENGER_QUEST_LIST_REFRESH = 1824,	///< (S->C) ����������Ʈ �Ϸ�


// ���̾�α�(DIALOG) -----------------------------------------------------------------------------------
	MC_DIALOG_START					= 1901,		///< (S->C) ���̾�α� ����
	MC_DIALOG_SELECT_REQ			= 1902,		///< (C->S) ���̾�α� �б� ���� ��û
	MC_DIALOG_SELECT				= 1903,		///< (S->C) ���̾�α� ����

// Ʈ���̴�(TRAINING)----------------------------------------------------------------------------------------
	MC_TRAINING_TRAIN_REQ	= 2001,		///< (C->S)	�ŷ�Ʈ �н� ��û
	MC_TRAINING_TRAIN		= 2002,		///< (S->C)	�ŷ�Ʈ �н� ����	
	MC_TRAINING_UNTRAIN_ALL	= 2003,		///< (S->C)	��� �ŷ�Ʈ �ʱ�ȭ
	
// ����(BUFF) -------------------------------------------------------------------------------------------
	MC_BUFF_GAIN					= 2101,		///< (S->C)	���� ȿ�� ���� [����ȭ]
	MC_BUFF_LOST_REQ				= 2102,		///< (C->S)	���� ȿ�� ���� ��û [����ȭ]
	MC_BUFF_LOST					= 2103,		///< (S->C)	���� ȿ�� ���� [����ȭ]
	MC_BUFF_HEAL					= 2104,		///< (S->C)	���� ȸ��ȿ��

	MC_BUFF_INSTANT_EFFECT_GAIN		= 2105,		///< (S->C) ������ �ν���Ʈ ȿ���� ���� [����ȭ]
	MC_TALENT_INSTANT_EFFECT_GAIN	= 2106,		///< (S->C)	�ŷ�Ʈ�� �ν���Ʈ ȿ���� ���� [����ȭ]
	MC_BUFF_STACK_INCREASE			= 2107,		///< (S->C) ���� ���� �߰�
	MC_BUFF_STACK_DECREASE			= 2108,		///< (S->C) ���� ���� ����

// Ʈ���̵�(TRADE) --------------------------------------------------------------------------------------
	MC_TRADE_START_REQ				= 2201,		///< (C->S) �ŷ� ���� ��û
	MC_TRADE_START_REQ_RELAY		= 2202,		///< (S->C) �ŷ� ���� ��û ����
	MC_TRADE_START_ACCEPT_REQ		= 2203,		///< (C->S) �ŷ� ���� ���� ��û	
	MC_TRADE_START					= 2204,		///< (S->C) �ŷ� ���� ��û ����
	MC_TRADE_START_REJECT			= 2205,		///< (S->C) �ŷ� ���� ��û ����

	MC_TRADE_PUTUP_ITEM_REQ			= 2211,		///< (C->S) �ŷ�â�� ������ �ø����û
	MC_TRADE_PUTUP_ITEM				= 2212,		///< (S->C) �ŷ�â�� ������ �ø�������
	MC_TRADE_PUTDOWN_ITEM_REQ		= 2213,		///< (C->S) �ŷ�â���� ������ ������ ��û
	MC_TRADE_PUTDOWN_ITEM			= 2214,		///< (S->C) �ŷ�â���� ������ ������ ����

	MC_TRADE_PUTUP_MONEY_REQ		= 2221,		///< (C->S) �ŷ�â�� �ǹ� �ø��� ��û
	MC_TRADE_PUTUP_MONEY			= 2222,		///< (S->C) �ŷ�â�� �ǹ� �ø��� ���� ����

	MC_TRADE_CANCEL_REQ				= 2231,		///< (C->S) �ŷ� ��� ��û
	MC_TRADE_CANCEL					= 2232,		///< (S->C) �ŷ� ��� ��û ����
	MC_TRADE_CONFIRM_REQ			= 2233,		///< (C->S) �ŷ� Ȯ�� ��û
	MC_TRADE_CONFIRM				= 2234,		///< (S->C) �ŷ� Ȯ�� ��û ����
	MC_TRADE_COMPLETE				= 2235,		///< (S->C) �ŷ� �Ϸ�

// ��Ƽ(PARTY) ------------------------------------------------------------------------------------------
	MC_PARTY_INVITE_REQ					= 2253,		///< (C->S) ��Ƽ �ʴ�
	MC_PARTY_INVITE						= 2254,		///< (S->C) ��Ƽ �ʴ� ����
	MC_PARTY_LEAVE_REQ					= 2255,		///< (C->S) ��Ƽ ����
	MC_PARTY_KICK_REQ					= 2257,		///< (C->S) ��Ƽ �߹�
	MC_PARTY_INVITE_QUESTION			= 2261,		///< (S->C) ��Ƽ �ʴ� ����
	MC_PARTY_INVITE_QUESTION_RESPOND	= 2262,		///< (C->S) ��Ƽ �ʴ� ���� ����
	MC_PARTY_NOTIFY_JOIN				= 2263,		///< (S->C) ���� �˸�
	MC_PARTY_NOTIFY_LEAVE				= 2264,		///< (S->C) ���� �˸�
	MC_PARTY_REFRESH_INFO_ALL			= 2265,		///< (S->C) ��Ƽ�� ��� ����
	MC_PARTY_REFRESH_INFO				= 2266,		///< (S->C) ��Ƽ Player ����
	MC_PARTY_INVITE_FOR_ME_REQ			= 2267,		///< (C->S) ��Ƽ�忡�� ���� ��û
	MC_PARTY_INVITE_FOR_ME_QUESTION		= 2268,		///< (S->C) ��Ƽ�忡�� ���� ��û ����
	MC_PARTY_INVITE_FOR_ME_QUESTION_RESPOND	= 2269,	///< (C->S) ���� ��û ���� ����

	MC_PARTY_ACCEPT_CANCEL				= 2270,		///< (S->C)	���� ���� ���� ���

	MC_PARTY_CHANGE_NAME_REQ			= 2273,		///< (C->S) ��Ƽ �̸� ���� ��û
	MC_PARTY_CHANGE_NAME				= 2274,		///< (S->C) ��Ƽ �̸� ����
	MC_PARTY_CHANGE_LEADER_REQ			= 2275,		///< (C->S) ��Ƽ�� ���� ��û
	MC_PARTY_CHANGE_LEADER				= 2276,		///< (S->C) ��Ƽ�� ����
	MC_PARTY_CHANGE_LOOTING_RULE_REQ	= 2277,		///< (C->S) ��Ƽ ���÷� ���� ��û	[����ȭ]
	MC_PARTY_CHANGE_LOOTING_RULE		= 2278,		///< (S->C) ��Ƽ ���÷� ����		[����ȭ]
	MC_PARTY_CHANGE_QUESTID_REQ			= 2279,		///< (C->S) ��Ƽ ��������Ʈ ���� ��û
	MC_PARTY_CHANGE_QUESTID				= 2280,		///< (S->C) ��Ƽ ��������Ʈ ����
	MC_PARTY_CHANGE_LEADER_NOTIFY		= 2281,		///< (S->C) ��Ƽ�� ���� �˸�


// ���Ǿ� ����(NPC SHOP)
	MC_NPCSHOP_SHOW						= 2351,		///< (S->C) �����Ҽ� �ִ� ������ ��� ����
	MC_NPCSHOP_BUY_REQ					= 2352,		///< (C->S)	���� ��û
	MC_NPCSHOP_BUY						= 2353,		///< (S->C)	���� ����
	MC_NPCSHOP_SELL_REQ					= 2354,		///< (C->S)	�Ǹ� ��û
	MC_NPCSHOP_SELL						= 2355,		///< (S->C)	�Ǹ� ����
	MC_NPCSHOP_SELL_VERYCOMMON_REQ		= 2356,		///< (C->S)	�ſ� ���� ������ �Ǹ� ��û
	MC_NPCSHOP_SELL_VERYCOMMON			= 2357,		///< (S->C)	�ſ� ���� ������ �Ǹ� ����
	MC_NPCSHOP_REPAIR_ONE_REQ			= 2358,		///< (C->S)	���� �Ѱ� ��û
	MC_NPCSHOP_REPAIR_ALL_REQ			= 2359,		///< (C->S)	���� ��� ��û
	MC_NPCSHOP_REPAIR					= 2360,		///< (S->C)	���� ����	

// ����(LOOTING) ----------------------------------------------------------------------------------------	
	MC_LOOT_BEGIN_REQ				= 2401,		///< (C->S) ���� ���� ��û
	MC_LOOT_BEGIN					= 2402,		///< (S->C) ���� ���� ����

	MC_LOOT_SEE_ITEM_REQ			= 2403,		///< (C->S) ������ ��� ��û
	MC_LOOT_SEE_ITEM				= 2404,		///< (S->C) ������ ��� ����

	MC_LOOT_LOOT_ITEM_REQ			= 2405,		///< (C->S) Gettable : ȹ��ó��, Rollable : �ֻ�������, MasterLootable : UI �� NOTIFY ó��
	MC_LOOT_ROLLRESULT				= 2406,		///< (S->C) �ֻ��� ����
	MC_LOOT_MASTERLOOT_UI			= 2407,		///< (S->C) �����ͷ� UI
	MC_LOOT_MASTERLOOT_NOTIFY		= 2408,		///< (S->C) �����ͷ� ����	
	
	MC_LOOT_MASTERLOOT_ITEM_REQ		= 2410,		///< (C->S) ������ �����ͷ� ��û
	MC_LOOT_MASTERLOOT_UI_REMOVE	= 2414,		///< (S->C) �����ͷ�UI ����(��ü�� ��������� ���)

	MC_LOOT_REFRESH_ITEM			= 2415,		///< (S->C) ���� ������ ������ ��� ����
	MC_LOOT_OTHER_GAIN_ITEM			= 2416,		///< (S->C) �ٸ� �����ڰ� ������ ȹ��

	MC_LOOT_GETTABLE_ITEM_ADD		= 2417,		///< (S->C) ȹ�� ������ �������� �߰���(�ٸ� �ʵ忡 ���

	MC_LOOT_END_REQ					= 2418,		///< (C->S) ���� �Ϸ� ��û
	MC_LOOT_END						= 2419,		///< (S->C) ���� �Ϸ� ����

	MC_LOOT_INSERT_LOOTABLE_NPC		= 2420,		///< (S->C) ���� �����ϰ� �� NPC ���
	MC_LOOT_REMOVE_LOOTABLE_NPC		= 2421,		///< (S->C) ���� �Ұ����ϰ� �� NPC ���
	

// ���ͷ���(INTERACTION) ----------------------------------------------------------------------------------------
	MC_INTERACTION_INTERACTION_REQ	= 2501,		///< (C->S) ���ͷ��� ��û

// ���ͷ���(NPC INTERACTION) ----------------------------------------------------------------------------------------
	MC_NPCINTERACTION_INTERACTION	= 2502,		///< (S->C) ���ͷ��� ����
	MC_NPCINTERACTION_IELEMENT_REQ	= 2503,		///< (C->S) ���ͷ��� ������Ʈ ��û
	MC_NPCINTERACTION_END_REQ		= 2504,		///< (C->S) ���ͷ��� ���� ��û
	MC_NPCINTERACTION_END			= 2505,		///< (S->C) ���ͷ��� ���� ����

	MC_NPCINTERACTION_ICON			= 2507,		///< (S->C) NPC ������ ����
	MC_NPCINTERACTION_LOOT_START	= 2508,		///< (S->C) ���ͷ������� ���� ����	
	

// �̴ϸ�, ����� ------------------------------------------------------------------------------------------
	MC_MINIMAP_MARK_POS_REQ			= 2601,		///< (C->S) �̴ϸ� Ŭ����ǥ �˸�
	MC_MINIMAP_MARK_POS				= 2602,		///< (S->C) �̴ϸ� Ŭ����ǥ ��Ƽ���鿡�� �˸�


// �Ƿε� -------------------------------------------------------------------------------------------------
	MC_FATIGUE_UPDATE				= 2701,		///< (S->C) �Ƿε� ����

// ũ����Ʈ -------------------------------------------------------------------------------------------------
	MC_CRAFT_SHOW				= 2801,		///< (S->C) ũ����Ʈ UI �����ֱ�
	MC_CRAFT_MAKE_REQ			= 2802,		///< (C->S) ũ������ ���� ��û
	MC_CRAFT_MAKE				= 2803,		///< (S->C) ũ������ ���� ����
	MC_CRAFT_RECIPE_INSERT_REQ	= 2804,		///< (C->S) ������ �߰� ��û
	MC_CRAFT_RECIPE_INSERT		= 2805,		///< (S->C) ������ �߰�
	MC_CRAFT_RECIPE_DELETE_REQ	= 2806,		///< (C->S) ������ ���� ��û
	MC_CRAFT_RECIPE_DELETE		= 2807,		///< (S->C) ������ ����

// ���� ------------------------------------------------------------------------------------------------
	MC_INN_BEGIN_SLEEP				= 2903,		///< (S->C) ������ ���ڱ� ����
	MC_INN_END_SLEEP				= 2904,		///< (C->S) ������ ���ڱ� ���� ��û

// ��� ------------------------------------------------------------------------------------------------
	MC_GUILD_SHOW_CREATE_UI			= 3001,		///< (S->C) ��� ���� UI
	MC_GUILD_SHOW_DESTROY_UI		= 3002,		///< (S->C) ��� ���� UI

	MC_GUILD_CREATE_REQ				= 3003,		///< (C->S) ��� ���� ��û
	MC_GUILD_CREATE					= 3004,		///< (S->C) ��� ����

	MC_GUILD_INVITE_REQ				= 3005,		///< (C->S) ��� �ʴ� ��û
	MC_GUILD_INVITE_NOTIFY			= 3006,		///< (S->C) ��� �ʴ� ��û ����
	MC_GUILD_INVITE_RES				= 3007,		///< (C->S) ��� �ʴ� ���� ��û
	MC_GUILD_INVITE_REJECT			= 3008,		///< (S->C) ��� �ʴ� ����

	MC_GUILD_JOIN_ME				= 3009,		///< (S->C) ���� ��� ����
	MC_GUILD_JOIN_OTHER				= 3010,		///< (S->C) �ٸ� ����� ���� ������ ��忡 ����
	MC_GUILD_JOIN_SECTOR			= 3011,		///< (S->C) ���� ���Ϳ� �ִ� ����� ���� �������� ���� ��忡 ����

	MC_GUILD_LEAVE_REQ				= 3012,		///< (C->S) ��� Ż�� ��û
	MC_GUILD_LEAVE					= 3013,		///< (S->C) ��� Ż��
	MC_GUILD_LEAVE_SECTOR			= 3014,		///< (S->C) ���� ���Ϳ� �ִ� ����� ���� �������� ���� ��忡�� Ż��

	MC_GUILD_DESTROY_REQ			= 3015,		///< (C->S) ��� ��ü ��û
	MC_GUILD_DESTROY				= 3016,		///< (S->C) ��� ��ü

	MC_GUILD_ONLINE					= 3017,		///< (S->C) ���� ����
	MC_GUILD_OFFLINE				= 3018,		///< (S->C) ���� ����
	MC_GUILD_MOVEFIELD				= 3019,		///< (S->C) ���� �ʵ��̵�

	MC_GUILD_CHANGE_MASTER_REQ		= 3020,		///< (C->S) ��� ������ ���� ��û
	MC_GUILD_CHANGE_MASTER			= 3021,		///< (S->C) ��� ������ ����

	MC_GUILD_INFO					= 3022,		///< (S->C) ���� ����ȭ�� ��� ���� ����.

	MC_GUILD_KICK_REQ				= 3023,		///< (C->S) ��� ���� ��û
	MC_GUILD_KICK					= 3024,		///< (S->C) ��� ����

	MC_GUILD_SHOW_STORAGE_UI			= 3025,		///< (C->S) ��� ������ UI
	MC_GUILD_DEPOSIT_STORAGEMONEY_REQ	= 3026,		///< (C->S) ��� ������ �Ա� ��û
	MC_GUILD_DEPOSIT_STORAGEMONEY		= 3027,		///< (S->C) ��� ������ �Ա�
	MC_GUILD_WITHDRAW_STORAGEMONEY_REQ	= 3028,		///< (C->S) ��� ������ ��� ��û
	MC_GUILD_WITHDRAW_STORAGEMONEY		= 3029,		///< (S->C) ��� ������ ���
	MC_GUILD_MOVE_STORAGEITEM			= 3030,		///< (S->C) ��� ������ ������ �̵�

// ���� ------------------------------------------------------------------------------------------------
	MC_DUEL_REQUEST					= 3101,		///< (C->S) ���� ��û
	MC_DUEL_QUESTION				= 3102,		///< (S->C) ���� �����û
	MC_DUEL_QUESTION_RESPOND		= 3103,		///< (S->C) ���� ����
	MC_DUEL_CANCEL					= 3104,		///< (S->C) ���� ���
	MC_DUEL_READY					= 3105,		///< (S->C) ���� �غ�
	MC_DUEL_FIGHT					= 3106,		///< (S->C) ���� ����
	MC_DUEL_FINISHED				= 3107,		///< (S->C) ���� ����
	MC_DUEL_PARTY_READY				= 3108,		///< (S->C) ��Ƽ ���� �غ�
	MC_DUEL_PARTY_FIGHT				= 3109,		///< (S->C) ��Ƽ ���� ����
	MC_DUEL_PARTY_CANCEL			= 3110,		///< (S->C) ��Ƽ ���� ���
	MC_DUEL_PARTY_FINISHED			= 3111,		///< (S->C) ��Ƽ ���� ����
	MC_DUEL_DEFEATED				= 3112,		///< (S->C) ���� �й��� �˸�
	MC_DUEL_PARTY_REQUEST			= 3113,		///< (C->S) ��Ƽ ���� ��û
	MC_DUEL_READY_COUNT				= 3114,		///< (S->C) ���� ���� ī��Ʈ�ٿ�

// ������ ------------------------------------------------------------------------------------------------
	MC_BATTLEARENA_REGISTER_REQ		= 3151,		///< (C->S) ������ ��û ��û
	MC_BATTLEARENA_REGISTER			= 3152,		///< (S->C) ������ ��û ����
	MC_BATTLEARENA_DEREGISTER_REQ	= 3153,		///< (C->S) ������ ��û��� ��û
	MC_BATTLEARENA_DEREGISTER		= 3154,		///< (S->C) ������ ��û��� ����
	MC_BATTLEARENA_MATCHED			= 3155,		///< (S->C) ������ ��Ī �˸�
	MC_BATTLEARENA_COUNTING			= 3156,		///< (S->C) ������ ���ӽ��� ī���� �˸�
	MC_BATTLEARENA_GAMESTART		= 3157,		///< (S->C) ������ ���ӽ��� �˸�
	MC_BATTLEARENA_READY_DEATHMATCH		= 3158,	///< (S->C) ������ ������ġ�� �غ�
	MC_BATTLEARENA_SCOREBOARD_DEATHMATCH = 3159,///< (S->C) ������ ������ġ�� ������ �˸�
	MC_BATTLEARENA_REJOIN_DEATHMATCH = 3160,	///< (S->C) ������ ������ġ�� ������
	MC_BATTLEARENA_DEATHMATCH_UPDATE = 3161,	///< (S->C) ������ ������ġ�� ������Ʈ
	MC_BATTLEARENA_FINISHED			= 3162,		///< (S->C) ������ ��������
	MC_BATTLEARENA_DIE				= 3163,		///< (S->C) ������ ����
	MC_BATTLEARENA_REBIRTH_REQ		= 3164,		///< (C->S) ������ ��Ȱ ��û	
	MC_BATTLEARENA_KILL_INFO		= 3165,		///< (S->C) ������ ų ���� �˸�

// �ŷ�Ʈ��Ŀ��
	MC_FOCUS_GAIN					= 3201,		///< (S->C) �ŷ�Ʈ��Ŀ���� ����
	MC_FOCUS_LOST					= 3202,		///< (S->C) �ŷ�Ʈ��Ŀ���� ����
	MC_FOCUS_UPDATE_BUBBLE			= 3203,		///< (S->C) �ŷ�Ʈ��Ŀ�� ���� ���� ����
	

// �ȷ�Ʈ
	MC_PALETTE_SELECT_REQ			= 3301,		///< (C->S) �ȷ�Ʈ ���� ��û
	MC_PALETTE_SELECT				= 3302,		///< (S->C) �ȷ�Ʈ ����
	MC_PALETTE_SET_PRIMARY_REQ		= 3303,		///< (C->S) ����� �ȷ�Ʈ ���� ��û
	MC_PALETTE_SET_PRIMARY			= 3304,		///< (S->C) ����� �ȷ�Ʈ ����
	MC_PALETTE_SET_SECONDARY_REQ	= 3305,		///< (C->S) ���� ��� �ȷ�Ʈ ���� ��û
	MC_PALETTE_SET_SECONDARY		= 3306,		///< (S->C) ���� ��� �ȷ�Ʈ ����
	
	MC_PALETTE_PUTUP_REQ			= 3307,		///< (C->S) �ȷ�Ʈ�� �ø��� ��û
	MC_PALETTE_PUTUP				= 3308,		///< (S->C) �ȷ�Ʈ�� �ø���
	MC_PALETTE_PUTDOWN_REQ			= 3309,		///< (C->S) �ȷ�Ʈ�� ������ ��û
	MC_PALETTE_PUTDOWN				= 3310,		///< (S->C) �ȷ�Ʈ�� ������	
	MC_PALETTE_CHANGE_REQ			= 3311,		///< (C->S) �ȷ�Ʈ�� �÷����� ��ȯ ��û
	MC_PALETTE_CHANGE				= 3312,		///< (S->C) �ȷ�Ʈ�� �÷����� ��ȯ
	MC_PALETTE_PUTDOWN_ALLTALENT	= 3313,		///< (S->C) �ȷ�Ʈ�� �÷��� �ŷ�Ʈ ��� ������

 // �ҿ�
	MC_SOUL_DRAIN_ME				= 3401,		///< (S->C) �ҿ� ���(����)
	MC_SOUL_DRAIN_OTHER				= 3402,		///< (S->C) �ҿ� ���(�ٸ���)	

// �Ѽ�
	MC_FACTION_INCREASE				= 3501,		///< (S->C) �ѼǼ�ġ�� �ö�
	MC_FACTION_DECREASE				= 3502,		///< (S->C) �ѼǼ�ġ�� ������

// �ƾ�
	MC_CUTSCENE_BEGIN_REQ			= 3601,		///< (C->S) �ƽ� ���� ��û
	MC_CUTSCENE_BEGIN_ME			= 3602,		///< (S->C) �ƽ� ���� ����
	MC_CUTSCENE_BEGIN_OTHER			= 3603,		///< (S->C) �ƽ� �ٸ��̰� ����
	MC_CUTSCENE_END_REQ				= 3604,		///< (C->S) �ƽ� ���� ��û
	MC_CUTSCENE_END_ME				= 3605,		///< (S->C) �ƽ� ���� ����
	MC_CUTSCENE_END_OTHER			= 3606,		///< (S->C) �ƽ� �ٸ��̰� ����

// ȯ��
	MC_ENV_PLAY_BGM					= 3700,		///< (S->C) BGM ���

// ����
	MC_MAIL_MAILBOX_IS_OPENED			= 3800,	///< (S->C) ������ ���ͷ��� ���� (with ������ ���)
	MC_MAIL_MAILBOX_IS_CLOSED,					///< (S->C) ������ ���ͷ��� ���� �˸�
	MC_MAIL_POST_REQ,							///< (C->S) ���� ���� ��û
	MC_MAIL_POST_RES,							///< (S->C) ���� ���� ����
	MC_MAIL_READ_REQ,							///< (C->S) ���� �б� ��û
	MC_MAIL_READ_RES,							///< (S->C) ���� �б� ����
	MC_MAIL_DELETE_REQ,							///< (C->S) ���� ���� ��û
	MC_MAIL_DELETE_RES,							///< (S->C) ���� ���� ����
	MC_MAIL_TAKE_APPENDED_ITEM_REQ,				///< (C->S) Ư�� ������ ���� ÷�� ��ǰ ��� ��û
	MC_MAIL_TAKE_APPENDED_ITEM_RES,				///< (S->C) Ư�� ������ ���� ÷�� ��ǰ ��� ����
	MC_MAIL_TAKE_APPENDED_MONEY_REQ,			///< (C->S) ÷�ε� �� ��� ��û
	MC_MAIL_TAKE_APPENDED_MONEY_RES,			///< (S->C) ÷�ε� �� ��� ����
	MC_MAIL_NOTIFY_MAIL,						///< (S->C) ���� ���� ���� �˸� �˸�

	MC_MAIL_APPEND_PUTUP_ITEM_REQ,				///< (C->S) ���� ÷�� ������ �ø��� ��û
	MC_MAIL_APPEND_PUTUP_ITEM_RES,				///< (S->C) ���� ÷�� ������ �ø��� ����
	MC_MAIL_APPEND_PUTDOWN_ITEM_REQ,			///< (C->S) ���� ÷�� ������ ������ ��û
	MC_MAIL_APPEND_PUTDOWN_ITEM_RES,			///< (S->C) ���� ÷�� ������ ������ ����	

	MC_MAIL_APPEND_RESET_ITEM_REQ,				///< (C->S)	���� ÷�� ������ ��� ������ (or �������� ���)

// ��ȭ
	MC_ENCHANT_CHECK_REQ			= 3900,							///< (C->S) ��ȭ ���ɰ˻� ��û
	MC_ENCHANT_CHECK_RESULT			= 3901,							///< (S->C) ��ȭ ���ɰ˻� ����
	MC_ENCHANT_REQ					= 3902,							///< (C->S) ��ȭ ��û
	MC_ENCHANT_SUCCESS				= 3903,							///< (S->C) ��ȭ �Ϸ�
	MC_ENCHANT_BROKEN				= 3904,							///< (S->C) ��ȭ ����
	MC_ENCHANT_PREPARE				= 3905,							///< (C->S) ��ȭ �غ�
	MC_ENCHANT_CANCEL				= 3906,							///< (C->S) ��ȭ ���

// �ڵ���Ƽ
	MC_AUTOPARTY_ENQUEUE_REQ		= 4000,							///< (C->S) �ڵ���Ƽ ���� ��û
	MC_AUTOPARTY_INVITE				= 4001,							///< (S->C) �ڵ���Ƽ �Ἲ �Ϸ�
	MC_AUTOPARTY_INVITE_RES			= 4002,							///< (C->S) �ڵ���Ƽ �Ἲ ����
	MC_AUTOPARTY_INVITE_FAIL		= 4003,							///< (S->C) �ڵ���Ƽ �Ἲ ����
	MC_AUTOPARTY_DEQUEUE_REQ		= 4004,							///< (C->S) �ڵ���Ƽ ���� ��û
	MC_AUTOPARTY_CHECK_SENSOR_REQ	= 4005,							///< (S->C) �ڵ���Ƽ ���� �˻� ��û
	MC_AUTOPARTY_CHECK_SENSOR		= 4006,							///< (C->S) �ڵ���Ƽ ���� �˻�
	MC_AUTOPARTY_CHANGE_STATE_REQ	= 4007,							///< (C->S) �ڵ���Ƽ ���º��� ��û
	MC_AUTOPARTY_CHANGE_STATE		= 4008,							///< (S->C) �ڵ���Ƽ ���º���

// ����ƮPVP
	MC_QUESTPVP_TEAMCOUNT			= 4100,							///< (S->C) ���ο���
	MC_QUESTPVP_ENTER_REQ			= 4101,							///< (C->S) QPVP���� ���� ��û
	MC_QUESTPVP_ENTER				= 4102,							///< (S->C) QPVP���� ����
	MC_QUESTPVP_BEGIN_EVENT			= 4103,							///< (S->C) QPVP Event ����
	MC_QUESTPVP_END_EVENT			= 4104,							///< (S->C) QPVP Event ����
	MC_QUESTPVP_MARK_NPCPOS			= 4105,							///< (S->C) �ʿ� NPC�� ������ġ�� ǥ��
	MC_QUESTPVP_UNMARK_NPCPOS		= 4106,							///< (S->C) �ʿ� ǥ���� NPC�� ��ġ�� ����

// ������
	MC_STORAGE_SHOW_UI				= 4201,							///< (S->C) ������ UI(���� ����ȭ�� ������ ���� ����)
	MC_STORAGE_DEPOSIT_MONEY_REQ	= 4202,							///< (C->S) �Ա� ��û 
	MC_STORAGE_DEPOSIT_MONEY		= 4203,							///< (S->C) �Ա�
	MC_STORAGE_WITHDRAW_MONEY_REQ	= 4204,							///< (C->S) ��� ��û 
	MC_STORAGE_WITHDRAW_MONEY		= 4205,							///< (S->C) ���

// ��ǰ�ŷ���
	MC_SC_TRADEMARKET_EXCEPTION		= 4300,							///< (S->C) ����

	MC_CS_TRADEMARKET_PUT_ON		= 4301,							///< (C->S) ������ ����Ѵ�
	MC_SC_TRADEMARKET_PUTTED_ON		= 4302,							///< (S->C) ������ ��ϵǾ���

	MC_CS_TRADEMARKET_BUY			= 4303,							///< (C->S) ������ �����Ѵ�
	MC_SC_TRADEMARKET_BOUGHT		= 4304,							///< (S->C) ������ �����Ͽ���

	MC_CS_TRADEMARKET_SEARCH		= 4305,							///< (C->S) ������ �˻��Ѵ�
	MC_CS_TRADEMARKET_SEARCH_NEXT	= 4306,							///< (C->S) ������ �˻��� ���� �������� ��û
	MC_CS_TRADEMARKET_REFRESH		= 4307,							///< (C->S) �˻� ����� �ֽ����� �����Ѵ�
	MC_SC_TRADEMARKET_ITEM_LIST		= 4308,							///< (S->C) �˻� ����� �޾Ҵ�

	MC_CS_TRADEMARKET_GET_MYLIST	= 4309,							///< (C->S) �� �ŷ��� ������ ����� ��´�
	MC_SC_TRADEMARKET_MYLIST		= 4310,							///< (S->C) �� �ŷ��� ������ ���

// ����(GM) ------------------------------------------------------------------------------------------
	MC_GM_REQUEST_SPAWN				= 9001,		///< (C->S) NPC ���� ��û
	MC_GM_REQUEST_DESPAWN			= 9002,		///< (C->S) NPC ���� ��û
	MC_GM_MOVE_REQ					= 9003,		///< (C->S) GM �̵� ��û
	MC_GM_MOVE_TO_PLAYER_REQ		= 9004,		///< (C->S) Ư�� PC���Է� �̵� ��û
	MC_GM_MOVE_TO_MYSPOT_REQ		= 9005,		///< (C->S) �� �� ��ġ�� �̵� ��û(����� ó�� ������ ��)
	MC_GM_REBIRTH_REQ				= 9006,		///< (C->S) GM ��Ȱ ��û
	MC_GM_GOD_REQ					= 9007,		///< (C->S) GM ���� ��û
	MC_GM_CHANGE_WEATHER_REQ		= 9008,		///< (C->S) ���� ���� ��û
	MC_GM_CHANGE_TIME_REQ			= 9009,		///< (C->S) �ð� ���� ��û
	MC_GM_QUEST_GIVE_REQ			= 9010,		///< (C->S) ����Ʈ ������ ��û
	MC_GM_ITEM_GIVE_REQ				= 9011,		///< (C->S) ������ ���� ��û
	MC_GM_GOD						= 9012,		///< (S->C) GM ���� ����
	MC_GM_AI_RUNNING_REQ			= 9013,		///< (C->S) AI ���۾��ϰ� ���ּ���.
	MC_GM_AI_USE_TALENT_REQ			= 9014,		///< (C->S) �ش� NPC���� Ư�� �ŷ�Ʈ ����϶�� ���
	MC_GM_AI_SET_MONITOR_TARGET		= 9015,		///< (C->S) �׽�Ʈ �뵵�� ����͸��� Ÿ�� NPC ����
	MC_GM_GET_PLAYERUID_REQ			= 9016,		///< (C->S) �÷��̾� ID�� UID ��û
	MC_GM_GET_PLAYERUID				= 9017,		///< (S->C) �÷��̾� ID�� UID ��û ����
	MC_GM_REPORT_TARGET_ENTITY		= 9020,		///< (C->S) NPC Report
	MC_GM_KILL_ENTITY_REQ			= 9021,		///< (C->S) ����Ƽ ���̱� ��û
	MC_GM_RANGE_KILL_NPC_REQ		= 9022,		///< (C->S) ������ NPC ���̱� ��û
	MC_GM_SET_ME_REQ				= 9026,		///< (C->S) �� ���� ���� ��û
	MC_GM_QUEST_RESET_REQ			= 9027,		///< (C->S) ������ ����Ʈ �ʱ�ȭ ��û
	MC_GM_QUEST_RESET				= 9028,		///< (S->C) ������ ����Ʈ �ʱ�ȭ	
	MC_GM_SET_ITEM_REQ				= 9029,		///< (C->S) ������ ���� ���� ��û
	MC_GM_NPC_AI_RUNNING_REQ		= 9030,		///< (C->S) Ư�� NPC AI ��� ��û		
	MC_GM_INSERT_BUFF_REQ			= 9035,		///< (C->S)	���� �߰� ��û
	MC_GM_DELETE_BUFF_REQ			= 9036,		///< (C->S)	���� ���� ��û
	MC_GM_RESET_COOLTIME_REQ		= 9037,		///< (C->S)	���� ���� ��û
	MC_GM_SHOW_ENEMY_UID_REQ		= 9038,		///< (C->S)	��׷θ� ���� NPC UID ��û
	MC_GM_SHOW_ENEMY_UID			= 9039,		///< (S->C)	��׷θ� ���� NPC UID
	MC_GM_MOVE_TO_NPC_REQ			= 9040,		///< (C->S) Ư�� NPC���Է� �̵� ��û
	MC_GM_BREAK_PART_REQ			= 9041,		///< (C->S) Ư�� NPC�� Ư�� bpart �νñ�
	MC_GM_RANGE_BREAK_PART_REQ		= 9042,		///< (C->S) �ֺ� NPC�� ��� bpart �νñ�
	MC_GM_QUEST_COMPLETE_REQ		= 9043,		///< (C->S) ����Ʈ �Ϸ�
	MC_GM_QUEST_VAR_REQ				= 9044,		///< (C->S) quest_var ��û
	MC_GM_QUEST_VAR					= 9045,		///< (C->S) quest_var
	MC_GM_QUEST_FAIL_REQ			= 9046,		///< (C->S) ����Ʈ ���� ��û
	MC_GM_CLEAR_INVENTORY_REQ		= 9047,		///< (C->S) �κ��丮 ������ ��� ����
	MC_GM_QUEST_REWARD_REQ			= 9048,		///< (C->S) ����Ʈ ����ޱ�

	MC_GM_LOG_CRT_INSERT_REQ		= 9050,		///< (C->S) FailCRT �α� ����� �߰�
	MC_GM_LOG_CRT_DELETE_REQ		= 9051,		///< (C->S) FailCRT �α� ����� ����
	MC_GM_LOG_CRT_LIST				= 9052,		///< (S->C) FailCRT �α� ����� ���
	MC_GM_QUERY_MULTILOGIN_REQ		= 9053,		///< (C->S) ��Ƽ�α��� ���� ��û
	MC_GM_QUERY_MULTILOGIN			= 9054,		///< (S->C) ��Ƽ�α��� ���� ����


	MC_GM_FACTION_INCREASE_REQ		= 9060,		///< (C->S) �Ѽ� ����
	MC_GM_FACTION_DECREASE_REQ		= 9061,		///< (C->S) �Ѽ� ����
	MC_GM_SERVER_DUMP_REQ			= 9062,		///< (C->S) ���� ���� ��û
	MC_GM_SERVER_DUMP_RESP			= 9063,		///< (C->S) ���� ���� ����
	MC_GM_GHOST_REQ					= 9064,		///< (C->S) GM ��Ʈ ��û
	MC_GM_GHOST						= 9065,		///< (S->C) GM ��Ʈ

	MC_GM_SPAWN						= 9066,		///< (S->C) GM NPC ���� ó�� ���
	MC_GM_QUEST_RESET_ALL_REQ		= 9067,		///< (C->S) ������ ����Ʈ �� ������ �ִ� ����Ʈ �ʱ�ȭ ��û

	MC_GM_ENABLE_ICHECK_REQ			= 9068,		///< (C->S) ���ͷ��� üũ �ѱ�
	MC_GM_DISABLE_ICHECK_REQ		= 9069,		///< (C->S) ���ͷ��� üũ ����



	MC_GM_SET_NPC_REQ				= 9071,		///< (C->S) NPC�� ���°� ���� ��û

	MC_GM_DYE_REQ					= 9072,		///< (C->S) ���� ��û

	MC_GM_SUMMON_REQ				= 9073,		///< (S->C) Ư�� PC�� ��ȯ

	MC_GM_FACTION_RESET_REQ			= 9074,		///< (C->S) �Ѽ� �ʱ�ȭ
	MC_GM_REGEN_REQ					= 9075,		///< (C->S) �����/���ŷ�/���[�ڵ�ȸ��] ��� ��û
	MC_GM_DESPAWNBYID_REQ			= 9076,		///< (C->S) NPCID�� ����

	MC_GM_CHANGE_SERVER_MODE_REQ	= 9077,		///< (C->S) ���� ��� ���� ��û
	MC_GM_CHANGE_SERVER_MODE		= 9078,		///< (S->C) ���� ��� ����

	MC_GM_RESET_EXPO_CHARACTERS_REQ	= 9079,		///< (C->S) Expo �� ĳ���� Reset

// ����(GameGuard) ------------------------------------------------------------------------------------------
	MC_GG_AUTH_REQ					= 9080,		///< (S->C) �ֱ��� ���� ��û
	MC_GG_AUTH_RESPOND				= 9081,		///< (C->S) �ֱ��� ���� ����
	MC_GG_AUTH_ENABLE				= 9082,		///< (C->S) ���� ���� ���� ����

// �׽�Ʈ(TEST) -----------------------------------------------------------------------------------------
	MC_DEBUG_STRING					= 9103,		///< (S->C) ����� ��Ʈ���� Ŭ���̾�Ʈ�� �����ش�.
	MC_REQUEST_DEBUG_STRING			= 9104,		///< (C->S) ����� ��Ʈ���� ������ �����ش�.
	MC_AI_DEBUG_MESSAGE				= 9105,		///< (S->C) AI ����� ������ Ŭ���̾�Ʈ�� �����ش�.
	MC_DEBUG_COMBATCALC				= 9106,		///< (S->C) ���� ���� ����� ������ Ŭ���̾�Ʈ�� �����ش�.

	MC_DEBUG_START_COMMAND_PROFILE	= 9107,		///< (C->S) Ŀ�ǵ� �������ϸ� ����
	MC_DEBUG_DUMP_COMMAND_PROFILE	= 9108,		///< (C->S) Ŀ�ǵ� �������� ����
	MC_DEBUG_END_COMMAND_PROFILE	= 9109,		///< (C->S) Ŀ�ǵ� �������ϸ� ��

	MC_DEBUG_NPC_NETLOG				= 9110,		///< (S->C) ������ NPC �α� ������ �˷��ֱ�
	
	MC_DEBUG_ECHO_REQ				= 9111,		///< (C->S) ���� ��û
	MC_DEBUG_ECHO					= 9112,		///< (S->C) ���� ����

// ����(COMMON) -----------------------------------------------------------------------------------------
	MC_TIME_SYNCH					= 9200,		///< (S->C) ���� ���� Ÿ���� Ŭ�󿡰� ����ȭ ��Ű�� [����ȭ]

	
};



// ����,Ŭ���̾�Ʈ ��� �����ϴ� Ŀ�ǵ� ���
CSCOMMON_API void AddCommonCommandTable();


namespace CCommandVersionChecker
{
	CSCOMMON_API bool IsValid(int nCommandVersion);
}

#endif
