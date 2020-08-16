#ifndef _XDEF_H
#define _XDEF_H

// ���ӿ��� ����ϴ� ����� �����մϴ�.
#define APPLICATION_NAME	_T("Raiderz")


// Path
#define PATH_ZONE			_T("data/zone/")
#define PATH_FIELD			_T("data/field/")
#define PATH_UI				_T("data/interface/")
#define PATH_UI_LOADING		_T("data/interface/loading/")
#define PATH_UI_MINIMAP		_T("data/interface/minimap/")
#define PATH_SCRIPT			_T("data/script/")
#define PATH_SFX			_T("data/sfx/")
#define PATH_SFX_CAMERA		_T("data/sfx/camera/")
#define PATH_SHADER			_T("data/shader/;data/shader/components/")
#define PATH_SHADER_DEV 	_T("data/shaderdev/;data/shaderdev/components/")
#define PATH_SOUND			_T("data/sound/")
#define PATH_SYSTEM			_T("data/system/")

#define PATH_LANG			_T("data/system/lang/")
#define PATH_LANG_KOREAN	_T("data/system/lang/ko_KR/")

#define PATH_MODEL			_T("data/model/")
#define PATH_PLAYER_MALE	_T("data/model/Player/hm/")
#define PATH_PLAYER_FEMALE	_T("data/model/Player/hf/")
#define PATH_TREE			_T("data/Model/SpeedTree/")
#define PATH_MONSTER		_T("data/Model/Monster/")
#define PATH_NPC			_T("data/Model/NPC/")
#define PATH_SCREENEFFECT	_T("Data/Interface/screeneffects/")	// ��ũ�� ����Ʈ �⺻ ���
#define PATH_MAPOBJECT_MODELS _T("data/model/MapObject/")
#define PATH_MODELS			_T("data/model/;data/model/weapons;data/model/MapObject/;data/model/Sky;data/sfx")
#define DEFAULT_PATH_TEXTURES _T(".;Data/Texture/Character;Data/Texture/Map;Data/Texture/Tile;Data/Texture/SFX;Data/Texture/SpeedTree;Data/Texture/Weapon;Data/Texture/Sky")
#define PATH_FONT			_T("data/interface/font/")
#define PATH_CUTSCENE		_T("data/Cutscene/")

// ������ ���� PATH
#define PATH_MYDOC_ROOT				_T("Raiderz\\")
#define PATH_MYDOC_REPLAY			_T("Replay/")
#define PATH_MYDOC_SAVE				_T("Save/")
#define PATH_MYDOC_VIDEO			_T("Video/")
#define PATH_MYDOC_SCREEN_SHOT		_T("ScreenShot\\")
#define PATH_MYDOC_SHADER_CACHING	_T("Caching/")
#define PATH_MYDOC_CUSTOM			_T("Custom\\")
#define PATH_MYDOC_CUSTOM_CROSSHAIR	_T("Custom\\CrossHair\\")

// FileName
#define FILENAME_CONFIG				_T("config.xml")
#define FILENAME_CONFIG_REL			_T("config_rel.xml")
#define FILENAME_HISTORY			_T("history.xml")
#define FILENAME_ACCOUNT			_T("account.xml")

#define FILENAME_PMANG_CONFIG		_T("./version.ini")

#define FILENAME_SCRIPT_CONST		_T("data/Script/const.lua")
#define FILENAME_SCRIPT_CONST_REL	_T("data/script/rel_const.lua")
#define FILENAME_SCRIPT_DEBUG		_T("data/script/debug.lua")
#define FILENAME_SCRIPT_DEBUG_REL	_T("data/script/rel_debug.lua")

#define FILENAME_MOTION				_T("data/system/motion.xml")
#define FILENAME_MOTION_NPC			_T("data/system/motion_npc.xml")
#define FILENAME_MOTION_PC			_T("data/system/motion_pc.xml")
#define FILENAME_ITEM				_T("data/system/xitem.xml")
#define FILENAME_ITEM_CHANGEMTRL	_T("data/system/xitem_changemtrl.xml")
#define FILENAME_TALENT				_T("data/system/talent.xml")
#define FILENAME_TALENTHITINFO		_T("data/system/talent_hit_info.xml")
#define FILENAME_BUFFINFO			_T("data/system/buff.xml")
#define FILENAME_BUFF_EFFECTINFO	_T("data/system/buff_Effect_info.xml")
#define FILENAME_ENCHANT_BUFF_EFFECTINFO	_T("data/system/enchant_buff_Effect_info.xml")
#define FILENAME_EFFECTINFO			_T("data/system/talent_Effect_info.xml")
#define FILENAME_TALENT_EXT			_T("data/system/talent_ext.xml")
#define FILENAME_NPC				_T("data/system/npc.xml")
#define FILENAME_NPC_CHANGEMTRL		_T("data/system/npc_changemtrl.xml")
#define FILENAME_QUEST				_T("data/system/quest.xml")
#define FILENAME_DIALOG				_T("data/system/dialog.xml")
#define FILENAME_RECIPENPC			_T("data/system/recipenpc.xml")
#define FILENAME_CHAT_CMDS			_T("data/system/chatcmds.xml")
#define FILENAME_MESH_INFO			_T("data/system/mesh_info.xml")
//#define FILENAME_EFFECT			_T("data/sfx/effectList.xml")
#define FILENAME_MODEL_LIST			_T("data/model/ModelList.xml")
#define FILENAME_UI_MAINXML			_T("data/interface/ui_main.xml")
#define FILENAME_TIPS				_T("data/system/tips.xml")
#define FILENAME_HELP				_T("data/system/help.xml")
#define FILENAME_RECIPE				_T("data/system/recipe.xml")
#define FILENAME_TEXTUREMATERIAL	_T("data/system/TextureMaterial.xml")
#define FILENAME_PRELOAD			_T("data/system/PreLoad.xml")
#define FILENAME_CONDITIONS_ITEM	_T("data/system/conditions_item.xml")
#define FILENAME_CONDITIONS			_T("data/system/conditions.xml")
#define FILENAME_EFFECT_INFO		_T("data/system/effect.xml")
#define FILENAME_SOUNDLIST			_T("data/sound/sound.xml")
#define FILENAME_COLOR_TABLE		_T("data/system/color_table.xml")
#define FILENAME_PLAYER_MODEL		_T("data/system/player_model.xml")
#define FILENAME_SWORD_TRAIL_SAMPLING	_T("data/system/swordtrailsampling.xml")
#define FILENAME_FACTION			_T("data/system/faction.xml")
#define FILENAME_ITEM_DROPER		_T("data/system/item_droper.xml")
#define FILENAME_ITEM_QUESTREWARDER	_T("data/system/item_questrewarder.xml")


#define FILENAME_ZONELIST		_T("data/system/zonelist.xml")
#define FILENAME_FIELDLIST		_T("data/system/fieldlist.xml")
#define FILENAME_COMMAND_RESULT_TABLE_H	_T("../../CSCommon/include/CCommandResultTable.h")

#define FILENAME_PALETTE_TALENTLIST		_T("paletteTalent.xml")

#define FILENAME_LAUNCHER				_T("Raiderz Launcher.exe")

#define FILENAME_LANG					_T("lang.xml")
#define FILENAME_STR_ABUSE				_T("abuse.txt")
#define FILENAME_STR_INTERFACE			_T("interface.xml")
#define FILENAME_STR_LOCAL				_T("local.xml")
#define FILENAME_STR_STRINGS			_T("strings.xml")
#define FILENAME_STR_WORD				_T("word.xml")
#define FILENAME_STR_SYSTEM_MSG			_T("system_msg.xml")
#define FILENAME_STR_TIP				_T("tip.xml")
#define FILENAME_STR_NAME_ITEM			_T("name_xitem.xml")
#define FILENAME_STR_NAME_NPC			_T("name_npc.xml")
#define FILENAME_STR_NAME_BUFF			_T("name_buff.xml")
#define FILENAME_STR_NAME_TALENT		_T("name_talent.xml")
#define FILENAME_STR_NAME_QUEST			_T("name_quest.xml")
#define FILENAME_STR_NAME_DIALOG		_T("name_dialog.xml")
#define FILENAME_STR_NAME_FIELD			_T("name_fieldlist.xml")
#define FILENAME_STR_NAME_AREA			_T("name_area.xml")
#define FILENAME_STR_NAME_FACTION		_T("name_faction.xml")
#define FILENAME_RESULT_STRINGS			_T("result.xml")
#define FILENAME_RESULT_SOUND			_T("result_sound.xml")


// Ȯ����
#define FILEEXT_ZONE_COLLISION	_T(".cl2")
#define FILEEXT_ZONE_PICKZ		_T(".pickz.xml")
#define FILEEXT_ZONE			_T(".zone.xml")
#define FILEEXT_FIELD			_T(".field.xml")
#define FILEEXT_FIELD_SOUND		_T(".field.common.xml")
#define	FILEEXT_ZONE_ENV		_T(".zone.env.xml")
#define	FILEEXT_ZONE_SOUND		_T(".zone.common.xml")
#define FILEEXT_ZONE_RES		_T(".res.xml")

#define FILEEXT_REPLAY			_T(".smv")
#define FILEEXT_DUMMYCLIENT		_T(".dummy.xml")

#define FILEEXT_CUTSCENE		_T(".cutscene.xml")

#define POST_MOVE_TICK		0.2f		///< �̵��� �� ������ �̵� �޼��� ������ ƽ�ð�(�ʴ� 5��)


// Ʈ���� ����
#define TRIGGER_INVALID_SENSOR_ID			0

// ���� ���� ------------------------
#define GAME_TARGET_NPC_CHECK_TICK			0.1f		// 0.1�ʸ��� Target NPC üũ
#define GAME_ENVIRONMENT_FADING_TIME		5.0f		// ȯ�� ��ȭ ���̵� �ð�(��)

#define MAX_ATTACK_COMBO					6			// �⺻ ���� �޺� ��
#define MAX_ADVANCEATTACK_COMBO				2			// ���� ���� �޺� ��
#define ANIMATION_FRAME_PER_SEC				4800		// �ִϸ��̼� �������� �ʴ� 4800


#define MIN_CAMERA_THETA					(0.1f)
#define MAX_CAMERA_THETA					(MMath::PI-0.1f)

// ����� ���� ----------------------
#define KEY_CONSOLE							_T('`')			// �ܼ�âŰ


// �̸� �����ֱ� ���� �Ÿ� -----------------------------
#define SHOW_NAME_DISTANCE			2500.0f
#define SHOW_DAMAGE_COUNT_DISTANCE	2000.0f

// ���ͷ��� üũ ���� -----------------------------
#define CHECK_INTERACTION_DISTANCE	500.f
#define MINSIZE_INTERACTION_CIRCLE	80.f
#define MAXSIZE_INTERACTION_CIRCLE	1000.f
#define CHECK_INTERACTION_COL_Z		200.f

// ������ �˻縦 ���� z�� �ִ� ����
#define TERRAIN_HEIGHT_COLLISION_VALUE		-100000.0f

// ����
#define ITEM_DYED_COLOR_NAME			_T("item_dyed")

// birdkr�� mlog Helper �Լ�..-_-
inline void __cdecl mlog_birdkr(const char* pFormat, ...)
{
	char temp[1024];

	va_list args;
	va_start(args,pFormat);
	vsprintf_s(temp,pFormat,args);
	va_end(args);

	mlog_filter("birdkr", temp);
}
#define blog mlog_birdkr

inline void __cdecl mlog_luna1x(const char* pFormat, ...)
{
	char temp[1024];

	va_list args;
	va_start(args,pFormat);
	vsprintf_s(temp,pFormat,args);
	va_end(args);

	mlogf("luna1x", temp);
}

inline void __cdecl mlog_mizelan(const char* pFormat, ...)
{
	char temp[1024];

	va_list args;
	va_start(args,pFormat);
	vsprintf_s(temp,pFormat,args);
	va_end(args);

	mlog_filter("mizelan", temp);
}
#define mmlog mlog_mizelan

// runtime\Data\Script\debug.lua �� madduck �� ��ϵǾ� �־�� �Ѵ�.
inline void __cdecl mlog_duck(const char* pFormat, ...)
{
	char temp[1024];

	va_list args;
	va_start(args,pFormat);
	vsprintf_s(temp,pFormat,args);
	va_end(args);

	mlog_filter("madduck", temp);
}

enum INTERACTION_MENUTYPE
{
	MENU_NONE = 0,
	MENU_NPC,
	MENU_SECTOR,
	MENU_FOLLOW_WARP
};

enum NPC_ICON_STATE
{
	NIS_NONE = 0,

	NIS_ITEM_UNUSABLE,
	NIS_ITEM_USABLE,
	NIS_ONLY_INTERACTION,
	
	NIS_TRIGGER,
	NIS_QUEST_INTERACT,		// LOOT ����Ʈ
	NIS_LOOT,
	NIS_TALK,
	NIS_HELPER,
	NIS_SHOP,
	NIS_GUILD_CREATE,
	NIS_CRAFT,

	NIS_QUEST_NOT_BEGIN_ONLYICON,
	NIS_QUEST_NOT_BEGIN,
	NIS_QUEST_BEGIN,		//����Ʈ ����
	NIS_QUEST_INTERACTION,	//����Ʈ ���ͷ��� �ϱ�
	NIS_QUEST_ACCEPT,		//����Ʈ ����. -> CONTINUE��
	NIS_QUEST_END,			//����Ʈ �Ϸ�
	NIS_QUEST_CONTINUE_ONLYICON,
	NIS_QUEST_CONTINUE,		//����Ʈ ������
	NIS_QUEST_REWARD,		//���� ����.
	NIS_COMBAT,

	NIS_MAX
};


// XMsgType
enum XMsgType							// �޽��� Ÿ��
{
	MSGTYPE_GENERAL		= 1,			// �Ϲ�
	MSGTYPE_SYSTEM,						// �ý���
	MSGTYPE_WHISPER,					// �ӼӸ�
	MSGTYPE_FIELD,						// �ʵ�
	MSGTYPE_SHOUTING,					// ��ħ
	MSGTYPE_PARTY,						// ��Ƽ
	MSGTYPE_GUILD,						// ���
	MSGTYPE_AUCTION,					// ���
	MSGTYPE_CHANNEL,					// �缳 ä��
	MSGTYPE_ANNOUNCE,					// ����
	MSGTYPE_GM,							// ���
	MSGTYPE_SOCIAL,						// �Ҽ�
	MSGTYPE_BATTLE,						// �����޼���
};



// MActionProgressType
enum MActionProgressType
{
	MAPT_NONE				= -1,
	MAPT_LOOTING_ITEM,
	MAPT_INTERACTION,
	MAPT_CASTING_SPELL,
	MAPT_USE_ITEM,
};



// NPC, ���� ���� ����(�̸��� ���Դϴ�)
const DWORD QUEST_NPC_ID_COLOR		= 0xFFE5D018;
const DWORD PLAYER_ID_COLOR			= 0xFF08FA18;
//const DWORD ALLY_PLAYER_ID_COLOR	= 0xFF0000FF;
const DWORD VS_PLAYER_ID_COLOR		= 0xFFFF0000;
const DWORD DIE_MONSTER_ID_COLOR	= 0xFF464646;
// ����
const DWORD MONSTER_ID_COLOR		= 0xFFFF2222;
const DWORD MONSTER_ID_COLOR_FIGHTING			= 0xFFFF2222;	// ���� ����(����)
const DWORD MONSTER_ID_COLOR_FIRST_ATTACKER		= 0xFFFF2222;	// ���� ����(����)
const DWORD MONSTER_ID_COLOR_NOT_FIRST_ATTACKER	= 0xFFFFAA2B;	// �񼱰� ����(��Ȳ)


// ��� ����
#define GRAPPLED_NPC_GRIP_BONE_NAME				_T("dummy_grip")
#define GRAPPLED_NPC_THROW_BONE_NAME			_T("dummy_throw_dir")


// �̵� ���� ó��
#define PROCESS_MOVE_EXCEPTION_DISTANCE	500.0f
#define PROCESS_MOVE_EXCEPTION_HEIGHT	100.0f

// Net �÷��̾� ���� Ÿ�ٱ��� �̵��ӵ� ����
#define VIRTUAL_POSITION_VELOCITY		100.0f

// �ʿ� ���������� ���� ���� ó��
#define PROCESS_HOLD_EXCEPTION_TIME		0.3f
#define PROCESS_HOLD_EXCEPTION_FRAME	3

// �� ���̿� ���� ó��(�������� ���� 140���� ���� ���� �Դϴ�. �� ���۽� �˷��ּ���)
#define SWIMMING_HEIGHT_VALUE 140.0f

// ���� ��翡�� �̲������� �ʰ� Falling���� ó���ϱ� ���� ��� ��
#define FALLING_LIMIT	0.65f

// ���� ��翡�� �̲������� ��
#define SLIDING_LIMIT	0.76f

// ���� �̵��� ���ߺξ��� �� �ִ� �Ÿ���?
#define MOVE_LEVITATION_LEN			1000.0f

// color
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_DAMAGE			0xFFFF0000
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_CRITICAL		0xFFFF0000
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_MISS			0xFFFFFFFF

#define DEFAULT_CAPTION_EFFECT_COLOR_ENEMY_DAMAGE		0xFFFFFFFF
#define DEFAULT_CAPTION_EFFECT_COLOR_ENEMY_CRITICAL		0xFFFFFF00
#define DEFAULT_CAPTION_EFFECT_COLOR_ENEMY_MISS			0xFFFFFFFF

#define DEFAULT_CAPTION_EFFECT_COLOR_MY_BUFF			0xFF00FFFF
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_DEBUFF			0xFF6F14FF
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_GUARD_DEFENSE	0xFF00FFFF
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_HEAL			0xFFFF6600
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_HP_REGEN		0xFFFF6600
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_EXP				0xFFFF6600
#define DEFAULT_CAPTION_EFFECT_COLOR_MY_EN				0xFFBBFF00

// �˱� ���ͳ�� �̸�
#define SWORDTRAIL_START_NODE_NAME		_T("swordtrail_start")
#define SWORDTRAIL_END_NODE_NAME		_T("swordtrail_end")

// �̵��� ��� ����
#define MOVE_ALLOWED_HEIGHT 50.0f
#define MOVE_DIRECT_CHECK_HEIGHT		700.0f		// 7����
#define MOVE_TARGET_HEIGHT_CHECK_HEIGHT	200.0f		// 2����

// interaction
#define INTERACTION_CENTER_BONE_NAME				_T("dummy_ef_body")
#define INTERACTION_DUMMY							_T("dummy_interaction")

/// �ؽ��ķ� ���Ǵ� ���ϵ�-------------------
#define FILENAME_MINIBAR_HP						_T("minihp.png")
#define FILENAME_MINIBAR_BAR					_T("minihpbar.png")
#define FILENAME_EFFECT_TRACE_PARTICLE			_T("Particle.bmp")




#define EFFECT_NAME_LEVEL_UP					_T("LevelUp")
#define EFFECT_NAME_CREATEGUILD					_T("GuildCreate")
#define EFFECT_NAME_FATIGUE_BEST				_T("FatigueBest")
#define EFFECT_NAME_DYE_DONE					_T("buff_dye_done")

#define SOUND_NAME_QUEST_GIVE					_T("quest_start")
#define SOUND_NAME_QUEST_COMPLETE				_T("quest_finish")


#define AREA_BUFFER_RADIUS				300.f

#define RESTRICTED_AREA_WARNING_DISTANCE	3000.0f

#define BREAKABLEPARTS_MAX_COUNT			4

#define WEAPON_EFFECT_DUMMY_MAX_COUNT		6
#define WEAPON_EFFECT_DUMMY_NAME			_T("dummy_ef_enchant_")
#define WEAPON_LEFT_EFFECT_DUMMY_NAME		_T("dummy_ef_enchant_l_")
#define SHIELD_EFFECT_DUMMY_NAME			_T("dummy_ef_enchant_shield_00")


// ���ͷ��� �Ÿ�
#define INTERACTOR_DISTANCE					60.0f

enum BGM_INFO_INDEX
{
	FIELD_BGM = 0,
	DAWN_BGM,
	DAYTIME_BGM,
	SUNSET_BGM,
	NIGHT_BGM,
	CLODY_BGM,
	RAIN_BGM,
	HEAVYRAIN_BGM,
	HEAVYSNOW_BGM,
	DUNGEON_BGM,
	NORMAL_BGM,
	BGM_MAX
};

enum INTERACTION_VOICE_TYPE
{
	VOICE_TYPE_NONE = -1,

	VOICE_TYPE_MALE_1,
	VOICE_TYPE_SOLDIERMALE1,
	VOICE_TYPE_RENASIANMALE1,

	VOICE_TYPE_MALE_2,
	VOICE_TYPE_SOLDIERMALE2,
	VOICE_TYPE_RENASIANMALE2,

	VOICE_TYPE_MALE_3,
	VOICE_TYPE_SOLDIERMALE3,
	VOICE_TYPE_RENASIANMALE3,

	VOICE_TYPE_MALE_4,
	VOICE_TYPE_SOLDIERMALE4,
	VOICE_TYPE_RENASIANMALE4,

	VOICE_TYPE_FEMALE_1,
	VOICE_TYPE_SOLDIERFEMALE1,
	VOICE_TYPE_RENASIANFEMALE1,

	VOICE_TYPE_FEMALE_2,
	VOICE_TYPE_SOLDIERFEMALE2,
	VOICE_TYPE_RENASIANFEMALE2,

	VOICE_TYPE_FEMALE_3,
	VOICE_TYPE_SOLDIERFEMALE3,
	VOICE_TYPE_RENASIANFEMALE3,

	VOICE_TYPE_FEMALE_4,
	VOICE_TYPE_SOLDIERFEMALE4,
	VOICE_TYPE_RENASIANFEMALE4,

	VOICE_TYPE_FATMALE_1,
	VOICE_TYPE_FATMALE_2,
	VOICE_TYPE_FATFEMALE,

	VOICE_TYPE_MAX
};

enum INTERACTION_VOICE_SAY
{
	VOICE_SAY_NONE = -1,

	VOICE_SAY_HELLO1 = 0,			///< ��ȭ����
	VOICE_SAY_HELLO2,
	VOICE_SAY_HELLO3,

	VOICE_SAY_BYE1,					///< �ۺ��λ�
	VOICE_SAY_BYE2,
	VOICE_SAY_BYE3,
	VOICE_SAY_BYE4,

	VOICE_SAY_SHOP1,				///< ���� ����
	VOICE_SAY_SHOP2,
	VOICE_SAY_SHOP3,

	VOICE_SAY_QUEST1,				
	VOICE_SAY_QUEST2,
	VOICE_SAY_QUEST3,

	VOICE_SAY_QUEST_REPORT1,		///< ����Ʈ ����
	VOICE_SAY_QUEST_REPORT2,
	VOICE_SAY_QUEST_REPORT3,

	VOICE_SAY_DONE_CRAFT1,			///< ���� �Ϸ�
	VOICE_SAY_DONE_CRAFT2,
	VOICE_SAY_DONE_CRAFT3,

	VOICE_SAY_SIGH1,
	VOICE_SAY_SIGH2,
	VOICE_SAY_LAUGH,				///< ����ǥ��
	VOICE_SAY_CRY,
	VOICE_SAY_CHEER,

	VOICE_SAY_MAX
};

#endif