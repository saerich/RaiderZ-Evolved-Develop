#pragma once

#include "GFrameworkLib.h"
#include "CSMotionFactorInfo.h"
#include "CSDef.h"
#include "CSItemData.h"

class MWLua;

/// �����
class GFRAMEWORK_API GConst
{
public:
	static bool		AI_OPTIMIZATION;						///< AI ����ȭ - �����׽�Ʈ ������ false�ؾ� ����� �� ����� ���´�.
	static bool		PVP_MODE;								///< �÷��̾ ���� ���� ����
	static bool		RUN_RESOURCE_VALIDATOR;					///< �ܺ� ������ ���� ��ȿ�� üũ ����
	static bool		RUN_FOR_TEST;							///< ������ �׽�Ʈ������ �������� ����, �ٸ� ����׿� CONST ���� �����־ �� ���� false�̸� �����, �׽�Ʈ �ڵ尡 �۵����� �ʽ��ϴ�.

	// ����
	static int		REGEN_TICK;								///< ���� �ֱ�(�� ����)
	static int		HP_REGEN_AMOUNT;						///< �ʴ� STA ������
	static float	HP_REGEN_RATE_IN_COMBAT;				///< ������ STA ���� ����ġ
	static int		EN_REGEN_AMOUNT;						///< �ʴ� STA ������
	static float	EN_REGEN_RATE_IN_COMBAT;				///< ������ STA ���� ����ġ
	static int		STA_REGEN_AMOUNT;						///< �ʴ� STA ������
	static float	STA_REGEN_RATE_IN_COMBAT;				///< ������ STA ���� ����ġ	
	static float	SITTING_REGEN_RATE;						///< �ɾ����� �� ���� ����ġ


	static int		STRESS_ADD_INTERRUPT_DURING_ATTACKING;	///< �����߿� ���ͷ�Ʈ �������� �߰��Ǵ� ��Ʈ���� ��ġ
	static int		STRESS_ADD_AVOID_MELEE_ATTACK;			///< NPC�� ���� ������ ���� ȸ��
	static int		STRESS_ADD_AVOID_RANGE_ATTACK;			///< NPC�� ���Ÿ� ������ ���� ȸ��
	static int		STRESS_ADD_LIMITED_CHASE_TIME;			///< �߰��߿� ��� ��Ʈ���� ��ġ
	static float	STRESS_CHASE_INTERVAL;					///< ��Ʈ���� ��ġ�� �ް� �Ǵ� �߰� ���� �ð�

	static float	STATE_FACTOR_FOR_DAMAGE_BY_BACK;		///< ���¿� ���� ������ ���� �ڿ��� �¾��� ��
	static float	STATE_FACTOR_FOR_DAMAGE[MF_SIZE];		///< ���¿� ���� ������ ���� MF_STATE
	
	static float	STATE_FACTOR_FOR_MF_VALUE_BY_BACK;		///< ���¿� ���� ������� ���� �ڿ��� �¾��� ��
	static float	STATE_FACTOR_FOR_MF_VALUE[MF_SIZE];		///< ���¿� ���� ������� ���� MF_STATE

	static wstring	REPORT_NPC_FILENAME;					///< report npc filename

	static float	VICTORY_DEFAULT_DELAY;					///< NPC�� �¸������� ��Ʈ�� ��õ��� �ʾ����� �⺻������ ����ϴ� �ð�

	static float	INVINCIBLE_TIME_REVIVE;					///< PC/NPC�� ��Ȱ������ ���� �ð�
	static float	INVINCIBLE_TIME_ENTER_FIELD;			///< PC�� �ʵ� ���� �������� ���� �ð�
	static int		INVINCIBLE_BUFF_ID;						///< ���� ������ ID

	static int		INVISIBLETONPC_BUFF_ID;					///< NPC���� ������ ������ ID

	// ��Ȱ ���� --------
	static float	REBIRTH_VALID_DISTANCE_XY;				///< Ŭ�� ������ ��Ȱ ��û�� ������ ��ȿ�� �Ÿ� ���� XY
	static float	REBIRTH_VALID_DISTANCE_Z;				///< Ŭ�� ������ ��Ȱ ��û�� ������ ��ȿ�� �Ÿ� ���� Z

	static int		REBIRTH_DEBUFF_TARGET_LEVEL;			///< ��Ȱ ����� ���� ��󷹺�
	static int		REBIRTH_SOULBINDING_DEBUFF;				///< ��ȥ�� ��Ȱ �����
	static int		REBIRTH_CHECKPOINT_DEBUFF;				///< �������� ��Ȱ �����
	static int		REBIRTH_QUESTPVP_DEBUFF;				///< ����ƮPVP ��Ȱ �����
	static int		REBIRTH_QUESTPVP_BUFF;					///< ����ƮPVP ��Ȱ ����

	static float	REBIRTH_SOULBINDING_RECOVER;			///< ��ȥ�� ��Ȱ ȸ����
	static float	REBIRTH_CHECKPOINT_RECOVER;				///< �������� ��Ȱ ȸ����
	static float	REBIRTH_QUESTPVP_RECOVER;				///< ����ƮPVP ��Ȱ ȸ����
	static float	REBIRTH_BATTLEARENA_RECOVER;			///< ��Ʋ�Ʒ��� ��Ȱ ȸ����

	static bool		INCLUDE_DEV_FIELD_LIST;					///< �������� �ʵ嵵 �ε��� �� ����
	static int		DYNAMICFIELD_EXPIRED_TIME;
	static int		TIME_SYNCH_INTERVAL;
	static int		MAX_BUFF_QTY;							// �ִ�� ���� �� �ִ� ���� ����
	static int		MAX_DEBUFF_QTY;							// �ִ�� ���� �� �ִ� ����� ����

	static float	TARGET_INFO_OLD_SEND_TIME;				// Ÿ�� ������ ���� �ð��� �����Ǿ��ٰ� �Ǵ��ϴ� �ð�

	static float	COMBAT_CHASE_MAX_DURATION_TIME;			// ������ �ŷ�Ʈ�� �������� ������ �ִ�ð�

	static float	ROAM_DEFAULT_IDLE_TIME;					// �ι��߿� �⺻���� ��ٸ��� ���ð�

	static float	INTERACT_QUEST_OBJECTIVE_ANI_TIME;		// ���ͷ������� ����Ʈ ���� ���°��� ���� ��ų�� ����ϴ� �ִϸ��̼� �ð�
	static float	LOOT_ROLL_TIME;							// �ֻ��� ������ �� �� �ִ� �ð�
	
	// ���� ���� -----
	static int		NO_WEAPON_DAMAGE_LEFT;					// �޼� ���⸦ �������� �ʾ��� ����� �⺻ ������
	static int		NO_WEAPON_DAMAGE_RIGHT;					// ������ ���⸦ �������� �ʾ��� ����� �⺻ ������
	static float	NPC_ASSIST_INTERVAL;					// NPC�� �ֺ� �Ʊ� NPC�� ���� �ֱ�
	static float	NPC_FIRSTSTRIKE_DELAY;					// NPC ������ ���� ������
	static float	PRE_HIT_CHECK_TIME;						// ���� ���� �ð����� ���� �����ϴ� �ð�(�⺻�� = 0.2��)
	static float	NPC_FINDPATH_RADIUS_LEVEL1;				// ��ã�� ����1 �������
	static float	NPC_FINDPATH_RADIUS_LEVEL2;				// ��ã�� ����2 �������
	static float	NPC_FINDPATH_RADIUS_LEVEL3;				// ��ã�� ����3 �������
	static float	NPC_FINDPATH_TICK_LEVEL1;				// ��ã�� ����1 ���� ƽ�ð�
	static float	NPC_FINDPATH_TICK_LEVEL2;				// ��ã�� ����2 ���� ƽ�ð�
	static float	NPC_FINDPATH_TICK_LEVEL3;				// ��ã�� ����3 ���� ƽ�ð�
	static float	NPC_FINDPATH_TICK_OTHER;				// ��ã�� ������ ���� ƽ�ð�
	static float	NPC_FINDPATH_MOVESPEED_FACTOR;			// ��ã�� ���� ƽ�ð� �ӵ� ����ġ (ƽ�ð� * �̵��ӵ� / ����)
	static float	NPC_DEFAULT_COMBAT_RADIUS;				// NPC �⺻ ���� ���� (������ �Ѿ�� ���� ����, ����: cm)
	
	// PVP ���� --------------
	static float	PVP_DURATIONTIME;						// �÷��̾ �ǰݽ� PVP���� ���ӽð�

	// ������ ���� --------------
	static float	MAGICAREA_EFFECTION_PERIOD;				// ������ ȿ�� �ֱ�

	// ����ȭ ����
	static float	INVISIBILITY_DETECT_TICK;				// ����ȭ Ž�� ó�� ƽ �ֱ� (second)
	static int		INVISIBILITY_DETECT_DISTANCE;			// ����ȭ Ž���Ÿ� (cm)

	// ��Ŀ�� ���� --------------
	static float	FOCUS_ADVENT_DURATION_TIME;				// ���� ��Ŀ���� �󸶳� ���ӵǴ°�
	static int		FOCUS_BERSERK_HIT_COUNT;				// ����� ������ ����ũ�� �ɸ��°�
	static float	FOCUS_BERSERK_DURATION_TIME;			// ����ũ ��Ŀ���� �󸶳� ���ӵǴ°�
	static float	FOCUS_BERSERK_RELAYHIT_TIME;			// ����ũ ��Ŀ���� ������ �����Ǵ� �ð�
	static float	FOCUS_COUNTER_DURATION_TIME;			// �ݰ� ��Ŀ���� �󸶳� ���ӵǴ°�
	static float	FOCUS_ENLIGHTEN_DURATION_TIME;			// ���� ��Ŀ���� �󸶳� ���ӵǴ°�
	static int		FOCUS_ENLIGHTEN_HIT_COUNT;				// ����� ������ ������ �ɸ��°�
	static float	FOCUS_ENLIGHTEN_RELAYHIT_TIME;			// ���� ��Ŀ���� ������ �����Ǵ� �ð�
	
	// ���� ������ ���� ---------
	static int		FALLING_DEBUFF_LV1_ID;					// ���Ͻ� '�ٸ��λ� LV1' ����� ID
	static int		FALLING_DEBUFF_LV2_ID;					// ���Ͻ� '�ٸ��λ� LV2' ����� ID

	// ������ ���� --------
	static int		ARENA_FIELDGROUP_ID;					// ������ �ʵ�׷� ���̵�
	static int		ARENA_FIELD_ID;							// ������ �ʵ� ���̵�
	static float	ARENA_GAME_COUNT;						// ������ ���ӽ��� ���ð�
	static float	ARENA_SCOREBOARD_COUNT;					// ������ ������ ���ð�
	static int		ARENA_DEATHMATCH_MAX_SCORE;				// ������ ������ġ �ִ�����
	static float	ARENA_DEATHMATCH_REBIRTH_TIME;			// ������ ������ġ ��Ȱ�ð�

	// �ð�,���� ���� ------------
	static int		CONST_ENV_TIME_DAWN_START_HOUR;
	static int		CONST_ENV_TIME_DAYTIME_START_HOUR;
	static int		CONST_ENV_TIME_SUNSET_START_HOUR;
	static int		CONST_ENV_TIME_NIGHT_START_HOUR;

	static int		CONST_LOCALTIME_PER_GAMETIME_ONE_HOUR;	// �ǽð� 14���� ���� 1�ð�
	static int		CONST_ENV_TIME_UPDATE_TICK_SEC;			// �ð�üũ ƽ��(��) - 60��
	static int		CONST_ENV_WEATHER_TICK_SEC;				// ���� ��ȭ ƽ 84�� (84 * 60)

	// NPC ��ü�Ҹ� ���� ------------
	static float	NPC_DECAY_TIME_NOITEM;					// NPC�� ������ ������ ��ü�� �Ҹ�Ǵ� �ð�
	static float	NPC_DECAY_TIME_GRADE[UCHAR_MAX];		// NPC�� �������� ������ ��޺� ��ü�� �Ҹ�Ǵ� �ð�

	//  �г� ���� -----------------
	static int		HIT_RAGE_AMOUNT;						// NPC�� �ǰ��� ���� �� ���� �г뷮
	static int		CRIHIT_RAGE_AMOUNT;						// NPC�� ġ��Ÿ�� ���� �� ���� �߰� �г뷮 (�⺻ �ǰݺг뷮�� �߰�)

	// ����Ʈ���̺� ���� -----------------
	static float	HATETABLE_ENTRY_TIMEOUT;				// ����Ʈ ���̺� �� �׸��� �ڵ����� ���ŵǴ� �ð� (����: ��)
	static float	HATETABLE_ENTRY_PARTY_SCAN_DISTANCE;	// ����Ʈ ���̺� �߰��� �÷��̾��� ��Ƽ�� Ž�� �Ÿ� (cm)

	// ����Ʈ ���� ------------------
	static float	QUEST_SHARE_DISTANCE;					// ����Ʈ ���� ���� �Ÿ�
	static float	QUEST_ESCORT_DISTANCE;					// ����Ʈ ȣ�� ���� �Ÿ�
	static float	QUEST_OBJECTIVE_UPDATE_SHARE_DISTANCE;	// ����Ʈ ���� ������Ʈ ���� ���� �Ÿ�

	// AutoTest ���� ------------
	static bool		AUTO_TEST_ACTIVE;
	static wstring	AUTO_TEST_TYPE;
	static float	AUTO_TEST_LIMIT_FRAME;
	static int		AUTO_TEST_REPEAT;
	static DWORD	AUTO_TEST_TIME;
	static int		AUTO_TEST_SPEED;	
	static int		AUTO_TEST_NPC_COUNT;
	static int		AUTO_TEST_FIELD_ID;
	static int		AUTO_TEST_QPVPEVENT_ID;
	
	static int		AITEST_TYPE;

	static bool		SERVER_SLEEP;

	// DynamicField Test ���� ------------
	static bool		DFTEST_ENABLE;
	static int		DFTEST_FIELDGROUP_ID;
	static int		DFTEST_FIELDGROUP_QTY;
	static float	DFTEST_SENSOR_ENTER_INTERVAL;

	// ����ȭ ����
	static int		SECTOR_CROWDED_LEVEL_CAPACITY;		// ���ⷹ���� �Ǳ� ���� �ʿ� �÷��̾� ��
	static int		SECTOR_PACKET_IGNORE_EACH_COUNT;	// ���ⷹ����  ���õ� ��Ŷ Ƚ��
	static int		SECTOR_PACKET_LIMIT_QUEUE_SIZE;		// ���ʹ� ������ �÷��̾ ī���� �ִ� ����
	static int		COMMAND_CROWDED_QUEUE_SIZE;			// Ŀ��� ������·� ����� Ŀ��� ť ũ��
	static int		COMMAND_OVERFULL_QUEUE_SIZE;		// Ŀ��� �ʰ����·� ����� Ŀ��� ť ũ��
	
	// Chatting ���� ---------------
	static bool		DEFAULT_GLOBAL_SAY;						// ��üä���� �⺻ä������ ����

	// ä��
	static float	GATHER_TIME[MAX_GATHER_TYPE][GATHER_TIER_COUNT + 1];
	static int		GATHER_RESPAWN_TIME[MAX_GATHER_TYPE];

	// ������ ����Ʈ
	static int		CQ_EXPIRED_HOUR;						// ������ ����Ʈ ����ð� (0 ~ 23)

	// �ҿ� ���ε�
	static int		SOULBINDING_DEFAULT_FIELDID;			// �ҿ���ε��� ID�� ��ȿ���� ������ ��ȥ�� ��Ȱ�� ����ϴ� FieldID
	static int		SOULBINDING_DEFAULT_MARKERID;			// �ҿ���ε��� ID�� ��ȿ���� ������ ��ȥ�� ��Ȱ�� ����ϴ� MarkerID

	// ����Ʈ PVP
	static float	QUESTPVP_REWARDOWNER_RELEASETIME;		// ����Ʈ PVP�������� ������ �����ð� ������ ������ ��������� �Ҹ�Ǵ´�, �� �ð��� ���(����: ��)
	static float	QUESTPVP_REWARDOWNER_DISTANCE;			// ����Ʈ PVP�������� ������ ������ ��������� ���� �����Ǵ� ����� �Ÿ�
	static float	QUESTPVP_INFO_SENDTIME;					// ����Ʈ PVP�������� �������� �ֱ�ð�
	static float	QUESTPVP_MARKPOS_REFRESH_TIME;			// ����Ʈ PVP�������� ��ŷ�� NPC��ġ ���� �ֱ�

	// ���
	static int		GUILD_CREATE_COST;						// ��� ���� ���


	// ġ�� ����
	static float	CHEAT_WARPHACK_LIMIT_DISTANCE;	// ������ ���ѰŸ�
	static float	CHEAT_SPEEDHACK_DISCONNECT_LIMIT_DISTANCE_RATE;	// ���ǵ��� �������� ���ѰŸ� ����
	static int		CHEAT_SPEEDHACK_DISCONNECT_EXECUTE_COUNT;	// ���ǵ��� �������� ���Ƚ��
	static float	CHEAT_SPEEDHACK_DISCONNECT_EXECUTE_DURATION;	// ���ǵ��� �������� ��� �����ð� (����: ��, �� �ð����� ������ ������ �ʱ�ȭ)
	static float	CHEAT_SPEEDHACK_WARNING_LIMIT_DISTANCE_RATE;	// ���ǵ��� ��� ���ѰŸ� ����
	static int		CHEAT_SPEEDHACK_WARNING_EXECUTE_COUNT;	// ���ǵ��� ���α� ���Ƚ��
	static float	CHEAT_SPEEDHACK_WARNING_EXECUTE_DURATION;	// ���ǵ��� ���α� �����ð� (����: ��, �� �ð����� ������ ������ �ʱ�ȭ)
	static bool		CHEAT_CHECK_DEVELOPER_TOO;	

	// AFK
	static float	AFK_TIMEOUT;

	// �ڵ���Ƽ
	static float	AUTOPARTY_MATCH_PROCESS_INTERVAL; // �ڵ���Ƽ ��ġ�۾� �ð�����

	// ����׿뵵 -----------
	static bool		NPC_AI_RUNNING;
	static bool		AUTO_PROFILE;
	static bool		TEST_SPAWN_TEST_START_POS;

	static bool		TEST_AI_DEBUG_MESSAGE;									///< ��ã�� ���� ���� AI �޼��� Ŭ���̾�Ʈ�� �����ֱ�
	static bool		TEST_SHOW_CLIENT_NPC_MOVE_PATH;								///< Ŭ���̾�Ʈ�� NPC �̵� ��θ� �������� ����
	static int		TEST_SHOW_CLIENT_NPC_PICKING;
	static bool		TEST_SHOW_NAVIGATION_MESH;

	static bool		TEST_ENABLE_TUTORIAL;					///< Ʃ�丮�� �Ѱ����
					
	static bool		TEST_RANDOM_MOVE;

	static bool		TEST_MAIL_POST_TO_ONESELF;

	static int		PATH_NEAR_HORIZONE_RANGE;
	static int		PATH_NEAR_VERTICAL_RANGE;

	static int		GAME_TICK;
	static bool		LOG_TALENT;
	static bool		LOG_FSM;
	static bool		LOG_JOB;
	static bool		LOG_SENSOR;
	static bool		LOG_PACKET;
	static bool		LOG_PATHFIND;
	static bool		LOG_SCRIPT_TIMER;
	static bool		LOG_AUTOPARTY;
	static bool		LOG_NPCSESSION;

	static bool		ENABLE_COLT_TIMEOUT_CHECKER;						///< ��Ʈ Ÿ�Ӿƿ� üĿ�� ��뿩��
	static bool		SHOW_LUA_ERROR_LOG;
	static bool		DEBUG_CREATE_CONVERT_LUAFILE;
	static bool		TEST_MAKE_SOLO_PARTY;
	static bool		TEST_ENABLE_OFFLINE_PARTY_FOR_STAND_ALONE_MODE;		///< ���ٵ��� ��忡��, ��� ��Ƽ�� �������λ��� ���	
	
	static bool		TEST_SHOW_CLIENT_FORMULA_DAMAGE;
	static bool		TEST_SHOW_CLIENT_FORMULA_GUARD;
	static bool		TEST_CMD_HANDLER_COMPARER;

	static bool		GODMODE_WITH_INVINCIBLE;
	static int		LOG_CRT;

	static int		MAX_NPC_REPORT_ALL_RECORD_QTY;			///< NPC����Ʈ ��ɿ� ĳ���ص� �� ���ڵ� ����
	static int		MAX_NPC_REPORT_SPECIFIC_RECORD_QTY;		///< NPC����Ʈ ��ɿ� ĳ���ص� Ư�� Ÿ���� ���ڵ� ����

	static bool		ENABLE_CHEATER_DISCONNECT;
	static bool		DUEL_PARTY_TEST;
	static int		DUEL_LIMIT_OBSERVE_COUNT;

	static int		BPART_BUFFSTACK_ID;

	static int		DISMOUNT_BUFFID;
	
	// AI Monitor -----------
	static bool		AIMON_ENABLE;
	static bool		AIMON_AUTO_TARGET;
	static int		AIMON_SET_FIELDID;
	static int		AIMON_SET_SPAWNID;
	static float	AIMON_UPDATE_INTERVAL;

	// ����׿뵵 -----------
	static bool		DEBUG_CONSOLE_SHOW_GAMETICK;
	static int		DEBUG_CONSOLE_GAMETICK_TICK;
	static float	DEBUG_GAME_TICK_MOD_SPEED;
	static bool		DEBUG_DISABLE_DEFAULT_FIELD_SPAWN; // �ʵ� xml�� ������ ������ ����
	

	static float	SPEAK_MAINTAIN_TIME;
	
	static class GFRAMEWORK_API PlayerStandupTimeTable
	{
	public:
		void Init();
		float GetStandupTime(WEAPON_TYPE nWeaponType);
	private:
		float m_fPlayerStandupTime[WEAPON_MAX];
	} PLAYER_STANDUPTIME;

	static void Init(MWLua* pLua);
	static void InitDebugConst(MWLua* pLua);


	static int GetFirstEnterFieldID();
};

inline bool GFRAMEWORK_API IsRunForTest()
{
	return GConst::RUN_FOR_TEST;
}
