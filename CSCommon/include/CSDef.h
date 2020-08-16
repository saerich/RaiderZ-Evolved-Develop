#pragma once

#include <tchar.h>


#define GAME_NAME_STR						_T("Raiderz")
#define GAME_NAME_STR_LOWER					_T("raiderz")

// ���� ���� -----------------------------------------------------------------
#define GAME_TARGET_NPC_CHECK_DISTANCE		300.0f		// NPC Ž�� �Ÿ�
#define ENTITY_KILL_CONTRIBUTE_DISTANCE		20000.0f	// ����Ƽ�� ���̴µ� ���� �⿩�ߴٰ� �����Ǵ� ��Ƽ���� �Ÿ�
#define GAME_INTERACTION_DISTANCE_Z			400.0f		// ���ͷ����� �����ϴ� Z�� �Ÿ�
#define GAME_INTERACTION_DISTANCE_XY_MULTIPLICATION	1.3f		// ���ͷ����� �����ϴ� XY�� �Ÿ��� ���Ҷ� ����ϴ� ������
#define GAME_INTERACTION_DISTANCE_XY_ADDITION		200.0f		// ���ͷ����� �����ϴ� XY�� �Ÿ��� ���Ҷ� ����ϴ� ������
#define GAME_INTERACTION_SERVER_TOLER_DISTANCE_XY	500.0f		// ���������� ���ͷ��� ���ɰŸ����� �̰Ÿ����� ���ؼ� ���ͷ��� �Ÿ��� ����Ѵ�.
#define GAME_INTERACTION_BPART_LOOTING_DISTANCE		1000.0f		// BPart ���� ���ͷ��� �Ÿ�(10����)


// NPC �������� ��ȣ���� ���� Buy/Sell Modulate
#define NPC_SHOP_BUY_MOD_FACTION_GOOD				0.98f
#define NPC_SHOP_BUY_MOD_FACTION_EXCELLENT			0.95f
#define NPC_SHOP_SELL_MOD_FACTION_GOOD				1.02f
#define NPC_SHOP_SELL_MOD_FACTION_EXCELLENT			1.05f

// ũ����Ʈ���� ��ȣ���� ���� Make Modulate
#define CRAFT_MAKE_MOD_FACTION_GOOD					0.98f
#define CRAFT_MAKE_MOD_FACTION_EXCELLENT			0.95f

// ���
//
#define MAX_ACCOUNT_CHAR_COUNT		5					// ������ �ִ� 4���� ĳ���͸� ���� �� �ִ�.

#define ACCOUNT_NAME_LEN	32							// ���� �̸� �ִ� ����
#define PLAYER_NAME_LEN		32							// �÷��̾� �̸� �ִ� ����
#define PLAYER_SUR_NAME_LEN	32							// �÷��̾� �� �ִ� ����

#define PLAYER_DEFAULT_TP	0							// ĳ���� ������ �� �⺻������ �����ϴ� TP
#define PLAYER_DEFAULT_SOULBINDINGID	109				// ĳ���� ������ �⺻ �ҿ���ε� ID
#define PLAYER_DEFAULT_INNROOMID		1				// ĳ���� ������ �⺻ ������ ID
#define PLAYER_DEFAULT_PRIPALETTE		5				// ĳ���� ������ �ֹ��⿡ ���ε��� �ȷ�Ʈ ID
#define PLAYER_DEFAULT_SECPALETTE		5				// ĳ���� ������ �������⿡ ���ε��� �ȷ�Ʈ ID

#define GUILD_NAME_LEN		32							// ��� �̸� �ִ� ����


#define SERVER_NAME_LEN		64							// ���� �̸� �ִ� ����

#define WORLD_NAME_LEN		32							// ���� �̸� �ִ� ����
#define IP_STRING_LEN		15							// IP ���� "###.###.###.###"


#define ITEM_NAME_LEN		32							// ������ �̸� ����
#define SECTOR_SIZE			3000						// �ʵ� ���� �Ѻ��� ����
#define HUGE_SECTOR_SCALE	4							// �ʵ� �Ŵ� ������ �Ϲ� ���� ����
#define MAX_RACE_NUM		1							// ������
#define MAX_LEVEL			60							// �ִ� ������ 60
#define MAX_TRADE_ITEM_SPACE		5					// �ѹ��� �ŷ� ������ �������� ����
#define MAX_TALENT_ADDITION_SIZE	3					// �� �ŷ�Ʈ�� ������� �߰������� ��� �ŷ�Ʈ�� ����

#define BASE_INVENTORY_SIZE			80					// �⺻ �κ��丮 ũ��.
#define MAX_INVENTORY_SIZE			160					// Ȯ�������� �ִ�� Ŀ�� �� �ִ� �κ��丮 ũ��.

#define BASE_STORAGE_SIZE			40					// �⺻ ������ ũ��.
#define MAX_STORAGE_SIZE			80					// Ȯ�������� �ִ�� Ŀ�� �� �ִ� ������ ũ��.

#define BASE_GUILDSTORAGE_SIZE			80				// �⺻ ��� ������ ũ��.
#define MAX_GUILDSTORAGE_SIZE			160				// Ȯ�������� �ִ�� Ŀ�� �� �ִ� ��� ������ ũ��.


#define MAX_CREATION_PLAYER_FACE_MALE		12			// ĳ���� ������ �� �� �ִ� �� ��
#define MAX_CREATION_PLAYER_HAIR_MALE		14			// ĳ���� ������ �� �� �ִ� �Ӹ��� ��
#define MAX_CREATION_PLAYER_VOICE_MALE		2			// ĳ���� ������ �� �� �ִ� ��Ҹ� ��
#define MAX_CREATION_PLAYER_MAKEUP_MALE		20			// ĳ���� ������ �� �� �ִ� ȭ�� ��
#define MAX_CREATION_PLAYER_TATTO_MALE		20			// ĳ���� ������ �� �� �ִ� ���� ��

#define MAX_CREATION_PLAYER_FACE_FEMALE		12			// ĳ���� ������ �� �� �ִ� �� ��
#define MAX_CREATION_PLAYER_HAIR_FEMALE		14			// ĳ���� ������ �� �� �ִ� �Ӹ��� ��
#define MAX_CREATION_PLAYER_VOICE_FEMALE	2			// ĳ���� ������ �� �� �ִ� ��Ҹ� ��
#define MAX_CREATION_PLAYER_MAKEUP_FEMALE	20			// ĳ���� ������ �� �� �ִ� ȭ�� ��
#define MAX_CREATION_PLAYER_TATTO_FEMALE	20			// ĳ���� ������ �� �� �ִ� ���� ��

#define CONSUME_TP_FOR_LEARN_TALENT			1			// �ŷ�Ʈ ���� ���� �ʿ��� TP

#define MAX_MONEY 400000000								// �ִ� �� ������

#define MAX_DB_TIMESTAMP_STR				32			// ����Ʈ �����ð�, ����ð� ��Ʈ�� ��ȯ �ִ� ����. MDB_TIMESTAMP_STR_LEN���� ���ƾ� �Ѵ�.


#define DEFAULT_ENTITY_RADIUS		40.0f				// ������Ʈ �⺻ ũ��(�÷��̾� ����)

#define GRAVITY						(-1400.0f)			// �߷�
#define UP_VECTOR					vec3(0,0,1)

#define MAX_STRINGID_LEN	1024	//���� ��¥����Ÿ�� ������ ũ�⸦ ����			

#define MAX_TARGET_NUMBER	15

#define INVALID_ID				-1
#define GUARD_TALENT_ID			INVALID_ID
#define INVALID_BUFF_ID			INVALID_ID
#define INVALID_TALENT_ID		INVALID_ID
#define INVALID_SPAWN_GROUP_ID	0

#define BUFF_DURATION_INFINITY	(-1.0f)

#define MAX_BREAKABLE_PARTS		4						// �� NPC�� �ִ� �극��Ŀ�� ������ 4���� �����Ѵ�.

#define MAX_PARTY_MEMBER_COUNT		5					///<< �ִ� ��Ƽ�� ��

#define HUGE_NPC_RADIUS		(300.0f)


#define NO_LIMIT_TIME 0

#define DEFAULT_CHANNEL_COUNT	2					///< ���� �ʵ��� �⺻ ä�� ��
#define	CHANNEL_PLAYER_LIMIT	200					///< �� ä�ο� ���� ������ �ο� ��
#define PRIME_CHANNELID			1					///< ���� ä�� ID
#define INVALID_CHANNELID		-1					///< �߸��� ä�� ID��
#define MAX_CHANNEL_SIZE		200					///< �ִ� ä�� ����
typedef int CHANNELID;

#define GAME_AUTO_REBIRTH_CHECK_TICK	300.f //5��

#define DEFAULT_SOULBINDING_ID 1

#define MAX_OWN_BUFF_NUMBER						16	// �ִ� ������ �ɸ��� �ִ� ��

#define DUEL_FLAG_NPCID		20002

#define TARGET_INFO_REFRESH_TIME	3000.f // sec

#define MAX_ENTERABLE_PRIVATE_CHAT_CHANNEL_COUNT	3	// �ִ� ���� ������ ä�� ä�� ����
#define MAX_INTERACTABLE_SENSOR_COUNT				8	// ���ÿ� ���ͷ��� ������ ���� �ִ� ����

// ��ȿ�� ��Ʈĸ���׷� ���̵�
#define INVALID_HITCAPSULEGROUP_ID		(-1)		// ��Ʈ ������ ���� �ʴ� ID
#define NONE_HITCAPSULEGROUP_ID			(-99)		// ������ ���� ���� ID


// ä���̵� �غ�ð�
#define CHANNEL_CHANGE_PREPARE_TIME_MIN		(8)		// sec
#define CHANNEL_CHANGE_PREPARE_TIME_MAX		(10*60)	// sec
#define CHANNEL_CHANGE_PREPARE_TIME			(10)	// sec

#define MAX_CHATTING_CHANNEL_NAME_LEN		64

// ƽ
const DWORD SECOND	= 1000;
const DWORD MINUTE	= 60 * SECOND;
const DWORD HOUR	= 60 * MINUTE;
const DWORD DAY		= 24 * HOUR;

enum MAIL_TYPE
{
	MT_PLAYER,		// �÷��̾ ���� ����
	MT_NPC,			// NPC�� ���� ����
	MT_GM,			// GM�� ���� ����
	MT_QPVP,		// QUEST PVP �������� ���� ����
};

// ���� ����
#define BATTLE_AREA_OUT_TIME			30			// �������� ���� �ð�

// NPC�� 1���� ������ ��ŷ�Ѵ�.
#define NPC_PICKING_MAP_HEIGHT 100.0f

// ���� ������ ���
enum COMBAT_TURN_RESULT
{
	CTR_NORMAL = 0,
	CTR_MISS,
	CTR_RIPOSTE,
	CTR_RESIST_PERFECT,
	CTR_RESIST_PARTIAL,
	CTR_CRITICAL,
	CTR_DRAIN,
	CTR_ZERO_TALENT_DAMAGE,		// �ŷ�Ʈ�� ������� 0
	CTR_IMMUNE,					// ��ȿ(�鿪)
	CTR_AVOID,					// ȸ��(�鿪)
	CTR_FALLING,
	CTR_DEAD,					// �� �������� �׾���
	CTR_HIT_BPART1,				// �극��Ŀ�� ������ �ǰ�����
	CTR_HIT_BPART2,				// �극��Ŀ�� ������ �ǰ�����
	CTR_HIT_BPART3,				// �극��Ŀ�� ������ �ǰ�����
	CTR_HIT_BPART4,				// �극��Ŀ�� ������ �ǰ�����

	// 16��Ʈ�� �־ ��Ŷ�� �����Ƿ� 16��Ʈ�� ������ �ȵ�. - birdkr
};

//�Ƿε�
enum FATIGUE_TYPE
{
	FT_NONE = 0,
	FT_TOO_BAD,
	FT_BAD,
	FT_NORMAL,
	FT_GOOD,
	FT_VERY_GOOD,	

	FT_MAX
};

enum AGE_LIMIT
{
	AL_18_UNDER = 0,
	AL_18_OVER
};


// �ŷ�Ʈ��Ŀ�� Ÿ��
enum TALENT_FOCUS_TYPE
{
	TFT_NONE = -1,
	TFT_ADVENT = 0,		// ����
	TFT_BERSERK,		// ����
	TFT_COUNTER,		// ����
	TFT_ENLIGHTEN,		// ����
	TFT_PRECISION,		// ����
	TFT_SNEAK,			// ����
	TFT_SIZE,
};


// Ż��Ʈ ��Ŀ�� ID ����
const int TALENT_FOCUS_ID [] = { 12000,  22000,  60501,  0,  0,  0 };

// ���� ��Ŀ�� ID ����
const int BUFF_FOCUS_ID [] = { 1550,  1551,  1552,  1553,  1554,  1561 };


// ȿ�� �ҽ�: �ŷ�Ʈ�� ������ ������ ȿ���� ������ ���� �����ϱ� ���� ���
enum EFFECT_SOURCE_TYPE
{
	EST_NONE = 0,
	
	EST_TALENT,
	EST_BUFF,

	EST_MAX
};

// ������ Ÿ��
enum DAMAGE_ATTRIB
{
	// ���� ������
	DA_NONE			= 0,
	DA_SLASH,
	DA_BLUNT,
	DA_PIERCE,
	DA_EXPLOSION,
	DA_INERTIA,

	// �ΰ��� �Ӽ�
	DA_HOLY,
	DA_UNHOLY,
	DA_FIRE,
	DA_COLD,
	DA_LIGHTNING,
	DA_POISON,

	DA_MAX
};


// ������ Ÿ�� ���ڿ�
// **����: ������ ���ڿ��� �������� ������.
static const TCHAR* DAMAGE_ATTRIB_NAME[] = 
{
	_T("DA_NONE"),
	_T("DA_SLASH"),
	_T("DA_BLUNT"),
	_T("DA_PIERCE"),
	_T("DA_EXPLOSION"),
	_T("DA_INERTIA"),

	_T("DA_HOLY"),
	_T("DA_UNHOLY"),
	_T("DA_FIRE"),
	_T("DA_COLD"),
	_T("DA_LIGHTNING"),
	_T("DA_POISON"),
};	

#define TALENT_REQUIRED_MOVE_TRUNCATED	0.01f

// ��� Ÿ��
enum GUARD_TYPE
{
	GT_INVALID = -1,
	GT_PARTIAL = 0,
	GT_PERFECT = 1,
	GT_ABSOLUTE = 2,
};

enum GUARD_CRASH_LEVEL
{
	GCL_PARTIAL = 0,
	GCL_PERFECT,
	GCL_ABSOLUTE,
	GCL_ALL,
};

static const char* GUARD_CRASH_LEVEL_STR[] = 
{
	"PARTIAL",
	"PERFECT",
	"ABSOLUTE",
	"ALL",
};	

/// ���� Ÿ��
enum SServerType
{
	SERVER_LOGIN		= 0,		///< �α��� ����
	SERVER_MASTER		= 1,		///< ������ ����
	SERVER_GAME			= 2,		///< ���� ����
	SERVER_APPLICATION	= 3			///< ���ø����̼� ����
};

/// Applicatoin ���
enum SAppServerMode
{
	APP_SERVER_CHAT		= 0,		///< ä�� ����
	APP_SERVER_AUCTION,				///< �ŷ� ���� - ������
	APP_SERVER_LOG,					///< �α� ���� - ������
};

/// ���� ���
///		LoginServer, GameServer, MasterServer ���� ���.
enum CSServerMode
{
	SERVER_MODE_INVALID		= -1,
	SERVER_MODE_NORMAL		= 0,	///< �Ϲ����� ����
	SERVER_MODE_EXPO		= 1,	///< ��� �ÿ��� ����
	SERVER_MODE_MAX			= 2,
};


/// �÷��̾ NPC�� �ε��� ID, UIID�� 1���� �����Ѵ�.
typedef unsigned short		UIID;
static const UIID UIID_INVALID = 0;

// NPC �α� ���ڵ�Ÿ��
enum NPCLogRecordType
{
	NPCLOG_COMMON,
	NPCLOG_USETALENT,
	NPCLOG_MOVE,
	NPCLOG_COLT,

	///////////////////////////////
	NPCLOG_ALL,
};

static const TCHAR* NPCLOG_RECORDTYPE[] =
{
	_T("Common"),
	_T("UseTalent"),
	_T("Move"),
	_T("Colt"),

	////////////
	_T("All"),
};


/// Field ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define FIELD_ATTR_CELL_SIZE	300 // cm

enum CHANNEL_STATUS
{
	CHANNEL_NORMAL,	 // ����
	CHANNEL_CROWDED, // ȥ��
	CHANNEL_FULL,	 // ��ȭ
};

// Challenger Quest Unable Type
enum ChallengerQuestUnableType
{
	CQUT_OK, // ���� �� ����
	CQUT_FAIL_QUEST_CONDITION, // ����Ʈ ����� ���� ������
	CQUT_FAIL_QUEST_NOT_TODAY, // ������ ����Ʈ�� �ƴ�
};

//////////////////////////////////////////////////////////////////////////
/// Chatting

enum CHATTING_CHANNEL_TYPE
{
	CCT_PRIVATE,
	CCT_PARTY,	
	CCT_AUCTION,
	CCT_FIELD,
};

#define CHATTING_LIMIT_SIZE		100		// ü�� �ִ����

//////////////////////////////////////////////////////////////////////////


// �޽��� Ÿ��
enum CSMsgType
{
	MT_SAY = 0,			// ��ȭ
	MT_NARRATION,		// �����̼�
	MT_SHOUT,			// ��ħ
	MT_SHOUT_FAKE,		// ��ħ(Fake)
	MT_AUCTION,			// ����
	MT_ANNOUNCE,		// �ȳ�
	MT_WHISPER,			// �Ӹ�
	MT_PARTY,			// ��Ƽ
	MT_GUILD,			// ���
	MT_CHANNEL,			// ä��
	
	MT_FIELD,			// �ʵ�
	MT_GLOBAL,			// ��ü

	MT_SP_SYSTEM,		// ��~������ �ý���

	MT_ADVICE,			// ���� �޽���

	MT_SYSTEM,			// �ý���
	MT_BATTLE,			// ���� �޼���	_by tripleJ 110504
	MT_NOTICE,			// ����

	MT_WHISPER_BACK,	// �Ӹ� �ڽ� �޽��� �ݼ�

	MT_SOCIAL,			// �Ҽ�
};



//////////////////////////////////////////////////////////////////////////

/// ä��
enum GATHER_TYPE
{
	GATHER_NONE = -1,	// ����

	GATHER_MINE = 0,	// ä��
	GATHER_HERB,		// �Ĺ�
	GATHER_RELIC,		// �߱�
	GATHER_LUMBER,		// ���� ä��

	MAX_GATHER_TYPE
};

#define GATHER_TIER_COUNT		10		// ä�� Ƽ�� 1~10

enum USABLE_BPART_DIRECTION
{
	UBD_NONE = -1,
	UBD_FRONT,
	UBD_BACK,
	UBD_LEFT,
	UBD_RIGHT,
	UBD_MAX,
};

static const char* USABLE_BPART_DIRECTION_STR[] = {
	"front",
	"back",
	"left",
	"right"
};

// ���� �׽�Ʈ
#define TEST_FORWARD_N(suite, test) \
	namespace Suite##suite { class Test##test; }

#define TEST_FRIEND_N(suite, test) \
	friend Suite##suite::Test##test

#define TEST_FORWARD_FT(suite, fixture, test) \
	namespace Suite##suite { class fixture##test##Helper; }

#define TEST_FRIEND_FT(suite, fixture, test) \
	friend Suite##suite::fixture##test##Helper

#define TEST_FORWARD_F(suite, fixture) \
	namespace Suite##suite { struct fixture; }

#define TEST_FRIEND_F(suite, fixture) \
	friend Suite##suite::fixture

#define TEST_VIRTUAL virtual


//////////////////////////////////////////////////////////////////////////
// PMang�� ����
#define PMANG_LOGIN_USER_ID_LEN		12
#define PMANG_LOGIN_STAT_INDEX_LEN	8

//////////////////////////////////////////////////////////////////////////
/// ����
#define MAX_MAILBOX_MAIL_COUNT				40
#define MAX_MAIL_TITLE_LEN					20
#define MAX_MAIL_TEXT_LEN					500
#define MAX_MAIL_APPENDED_ITEM_COUNT		5
#define MAX_MAIL_DELETE_SECOND				2592000		// ���� ��ȿ�Ⱓ 30��

#define MAIL_BASE_POSTAGE					50			//	50 CP
#define MAIL_EXTRA_POSTAGE_PER_SLOT			100			//	 1 SP


//////////////////////////////////////////////////////////////////////////
/// ��ȭ
#define ENCHANT_MAX_COUNT				6  // �ִ� ��ȭ ���� Ƚ��
#define ENCHANT_UNUSED_SLOT				0  // ������ ���� ����
#define ENCHANT_BROKEN_SLOT				-1 // ������ �μ��� ����

static int ENCHANT_AGENT_ITEMID[5] =
{
	39999000 //���ް�ȭ��
	, 39999001 //�Ϲݰ�ȭ��
	, 39999002 //��ް�ȭ��
	, 39999003 //�ֻ�ް�ȭ��
	, 39999004 //Ư�ް�ȭ��
};

//////////////////////////////////////////////////////////////////////////
/// �ڵ���Ƽ
enum AUTOPARTY_STATE
{
	AUTOPARTY_INVALID	=-1,	// ��ȿ�� ����
	AUTOPARTY_OFF		=0,		// ��� ����
	AUTOPARTY_PAUSE		=1,		// ����
	AUTOPARTY_STANDBY	=2,		// �˻� ���
	AUTOPARTY_LOOKUP	=3,		// �˻���
};



/// ���� ����
struct CSWorldInfo
{
	int					nID;
	wstring				strName;
	wstring				strIP;
	int					nCurrentPlayerCount;
	int					nMaxPlayerCount;
	int					nType;

	CSWorldInfo(): nID(0), nCurrentPlayerCount(0), nMaxPlayerCount(0), nType(0)
	{
	}
};

//////////////////////////////////////////////////////////////////////////
// ��ƿ��Ƽ ��ũ��
#define		_QUOTE(x)			# x
#define		QUOTE(x)			_QUOTE(x)
#define		__FILE__LINE__		__FILE__ "(" QUOTE(__LINE__) ") : "
#define		TODO( x )			message( __FILE__LINE__" TODO : " #x "\n" )
#define		todo( x )			TODO( x )