#pragma once

#include "MTypes.h"
#include "CSChrInfo.h"
#include "TDef.h"
#include "MWLua.h"
#include "MXml.h"

struct TPlayerModelInfo
{
	tstring			MODEL_NAME;
	tstring			MODEL_NAME_SIMPLE;
	tstring			DEFAULT_MESHNAME_HAT;
	tstring			DEFAULT_MESHNAME_CHEST;
	tstring			DEFAULT_MESHNAME_HANDS;
	tstring			DEFAULT_MESHNAME_LEGS;
	tstring			DEFAULT_MESHNAME_FEET;
	vector<tstring>	HAIR_MODEL;
	vector<tstring>	FACE_MODEL;
};

class TPlayerModelConst
{
private:
	void ParseDefaultModelPath(MXmlElement* pElement);
	void ParseDefaultMesh(MXmlElement* pElement);
	void ParseHair(MXmlElement* pElement);
	void ParseFace(MXmlElement* pElement);
	SEX TransSex(const TCHAR* szValue);
public:
	TPlayerModelInfo	Info[SEX_MAX];
public:
	TPlayerModelConst() {}
	bool Init(const TCHAR* szFileName);
};

struct TDefaultEquipments
{
	int		count;
	int		items[ FEATURE_ITEMSLOT_MAX];
};


/// �����
class TCONST
{
public:
	static TPlayerModelConst	PlayerModel;

	static TDefaultEquipments	DEFAULT_EQUIPMENTS[ 5];

	// ���� ��� --------------------------
	static int			CAMERA_FOV;							// Field of view ( 4:3 , Y ���� )
	static float		CAMERA_NEAR_Z;
	static float		CAMERA_FAR_Z;

	static float		GRASS_LOD_NEAR;
	static float		GRASS_LOD_FAR;
	static TCHAR			PATH_TEXTURE[1024];

	static float		DOF_FOCUS_DIST;
	static float		DOF_FOCUS_RANGE;
	static float		DOF_COC_SIZE;
	static float		CHAR_DOF_FOCUS_DIST;
	static float		CHAR_DOF_FOCUS_RANGE;
	static float		CHAR_DOF_COC_SIZE;

	// �̵���
	static float		MOVE_SPEED_NONE;
	static float		MOVE_SPEED_1HS;
	static float		MOVE_SPEED_1HB;
	static float		MOVE_SPEED_1HP;
	static float		MOVE_SPEED_2HD;
	static float		MOVE_SPEED_STF;
	static float		MOVE_SPEED_ARC;
	static float		MOVE_SPEED_2HB;
	static float		MOVE_SPEED_DWD;
	static float		MOVE_SPEED_DWP;

	static float		MOVE_BACK_SPEED_NONE;
	static float		MOVE_BACK_SPEED_1HS;
	static float		MOVE_BACK_SPEED_1HB;
	static float		MOVE_BACK_SPEED_1HP;
	static float		MOVE_BACK_SPEED_2HD;
	static float		MOVE_BACK_SPEED_STF;
	static float		MOVE_BACK_SPEED_ARC;
	static float		MOVE_BACK_SPEED_2HB;
	static float		MOVE_BACK_SPEED_DWD;
	static float		MOVE_BACK_SPEED_DWP;

	static float		JUMP_SPEED;
	static float		MOTION_FACTOR_KNOCKBACK_MOTION_EVENT_DIST;		// �Ÿ��� �󸶳� ������ �� �ִϸ��̼� ��ȯ�ϳ�
	static float		MOTION_FACTOR_THROWUP_MOTION_EVENT_DIST;		// �Ÿ��� �󸶳� ������ �� �ִϸ��̼� ��ȯ�ϳ�

	static float		HIT_FREEZE_TIME;		// ���� ���� �� ������ ���� �ð�(1 = 1��)
	static float		HIT_FREEZE_DELAY;		// ���� ���� �� ������ ���� �ð� ������(1 = 1��)
	// UI ����
	static int			LOADING_IMAGE_COUNT;

	static TCHAR			CHARACTER_CAPTION_FONT_NAME[256];
	static TCHAR			CHARACTER_CAPTION_FONT_FILE[256];
	static int			CHARACTER_CAPTION_FONT_HEIGHT;

	static TCHAR			SENSOR_EFFECT_NAME[256];
	static TCHAR			INTERACT_QUEST_OBJECTIVE_ANI_NAME[256];
	static float		INTERACT_QUEST_OBJECTIVE_ANI_TIME;

	// RealSpace3 ���� ����
	static bool			USING_COMPILED_SHADER;
	static bool			RESOURCE_LOADING_LOG;

	// caption effect ����
	static uint32		CAPTION_EFFECT_COLOR_MY_DAMAGE;
	static uint32		CAPTION_EFFECT_COLOR_MY_CRITICAL;
	static uint32		CAPTION_EFFECT_COLOR_ENEMY_DAMAGE;
	static uint32		CAPTION_EFFECT_COLOR_ENEMY_CRITICAL;
	static uint32		CAPTION_EFFECT_COLOR_MY_BUFF;
	static uint32		CAPTION_EFFECT_COLOR_MY_DEBUFF;
	static uint32		CAPTION_EFFECT_COLOR_MY_GUARD_DEFENSE;
	static uint32		CAPTION_EFFECT_COLOR_MY_HEAL;
	static uint32		CAPTION_EFFECT_COLOR_MY_HP_REGEN;
	static uint32		CAPTION_EFFECT_COLOR_MY_EXP;
	static uint32		CAPTION_EFFECT_COLOR_MY_EN;

	// caption ������
	static int			CAPTION_DAMAGE_FONT_SIZE;

	// ī�޶� ����Ʈ
	static float		CONST_CAMERA_LIGHT_EFFECT_DISTANCE;
	static float 		CONST_CAMERA_LIGHT_EFFECT_RADIUS;
	static uint32		CONST_CAMERA_LIGHT_EFFECT_COLOR;
	static bool			CONST_CAMERA_LIGHT_EFFECT_ENABLE;

	// �� ����
	static TCHAR			HIT_BASE_BLOOD_EFFECT_NAME[256];
	static float		HIT_CRITICAL_BLOOD_EFFECT_SIZE;

	// ����׿� ��� ----------------------
	static float		MIN_CAMERA_DIST;				// ī�޶�� �� ĳ���Ϳ��� �ּ� �Ÿ�
	static float		MAX_CAMERA_DIST;				// ī�޶�� �� ĳ���Ϳ��� �ִ� �Ÿ�
	static float		CAMERA_TARGET_DIST;
	static float		CAMERA_TARGET_HEIGHT;
	static float		CAMERA_TARGET_LEFT;
	static float		MOUSE_WHEEL_SENSIBILITY;		// �� �ΰ���
	static bool			SEX_MALE;						// �� ĳ���� ����


	static uint32		SCREEN_DEBUG_FONT_COLOR;		// ����� �۾� ����
	static int			SCREEN_DEBUG_TOP;				// ȭ�� ����� �۾� TOP ��
	static TCHAR			STANDALONE_EQUIP_ITEMS[256];	// ����� ������ ��� ID(��ĭ���� ������ ���� ����)
	static bool			SHOW_UI;						// ó�� ���۽� UI�� �������� ����
	static bool			CAMERA_FIX;						// ������ ī�޶� ������ ���������� ����� ������ ���� Ǯ�� ����
	static bool			DEBUG_EXECUTE_UNITTEST;	
	static bool			DEBUG_LOG;						// ����� �α� ��� ����

	static bool			USE_NET_DEBUGGER;				// XNetDebugger�� ������� ����
	static bool			USE_PHYSX_REMOTE_DEBUGGER;		// Physx remote debugger ��� ����

	static bool			USE_HIT_EFFECT_MODEL_ADD_COLOR;		// �ǰݽ� �ٲ�� �� ���� ��� ����
	static float		HIT_EFFECT_MODEL_ADD_COLOR_TIME;	// �ǰݽ� �ٲ�� �� ���� ��� �ð�
	static vec3			HIT_EFFECT_MODEL_ADD_COLOR;			// �ǰݽ� �ٲ�� �� ����
	static vec3			HIT_EFFECT_BPARTS_ADD_COLOR;			// �ǰݽ� �ٲ�� ���� ����

	static TCHAR		HIT_FAKE_BEATEN_DEFAULT_ANI_NAME[256];			///< �ǰ� ���׼� �ִ� �̸�
	static float		HIT_FAKE_BEATEN_ANIMATION_BLEND_WEIGHT;			///< �ǰ� ���׼� ���� Weight
	static float		HIT_HUGE_FAKE_BEATEN_ANIMATION_BLEND_WEIGHT;	///< Huge ���� �ǰ� ���׼� ���� Weight
	static float		HIT_FAKE_BEATEN_ANIMATION_BLEND_SPEED;			///< �ǰ� ���׼� ���� Speed
	static float		HIT_HUGE_FAKE_BEATEN_ANIMATION_BLEND_SPEED;		///< Huge ���� �ǰ� ���׼� ���� Speed
	static float		HIT_FAKE_BEATEN_ANIMATION_BLEND_CIRITICAL_WEIGHT;			///< �ǰ� ���׼� ���� ũ��Ƽ�� Weight
	static float		HIT_FAKE_BEATEN_ANIMATION_BLEND_CIRITICAL_SPEED;			///< �ǰ� ���׼� ���� ũ��Ƽ�� Speed

	static float		HIT_FAKE_BEATEN_WEAPON_TYPE_BLEND_WEIGHT[9];
	static float		HIT_FAKE_BEATEN_WEAPON_TYPE_BLEND_SPEED[9];

	static float		MOVE_MOTION_SPEED_RATIO;						///< �̵���� �ӵ�����

	static float		DECALEFFECT_DEPTH_VALUE;		// ��Į ���� ��


	// ���ͷ��� ��� �ܰ���
	static vec4			INTERACTION_TARGET_EDGE_COLOR;				// �ܰ��� ����
	static float		INTERACTION_TARGET_EDGE_TICKNESS;		// �ܰ��� �β�
	static float		INTERACTION_TARGET_EDGE_BLUR_FACTOR;	// �ܰ��� ���� ����

	// ���� Ÿ�� �ܰ���
	static vec4			GROUND_TARGET_EDGE_COLOR;				// �ܰ��� ����


	// ����׿� ��� ----------------------

	static bool			CONST_MAKE_COMMAND_RESULT;
	
	static TCHAR			DEFAULT_UI_FONT[256];
	static int			DEFAULT_UI_FONT_HEIGHT;
	static bool			AUTO_REPORT_CRASHDUMP;
	static bool			FULL_DUMP;
	static bool			DEBUG_LOG_COMMAND;
	static bool			AUTO_SAVE_REPLAY;				// ���ӽ����ϸ� �ڵ����� ���÷��� ����
	static bool			COMMAND_PROFILER_AUTO_SAVE;		// ���� ����� Ŀ�ǵ� �������Ϸ� �α׸� �ڵ����� �������� ����

	static int			REFILL_POTION_HP_ITEMID;
	static int			REFILL_POTION_MP_ITEMID;
	static int			REFILL_POTION_SP_ITEMID;

	static AGE_LIMIT	PLAYER_AGE_LIMIT;

	static TCHAR			DIEBGMNAME[256];

	// CrashDump
	static tstring		CONST_CRASH_DUMP_DEST_IP_LOCAL;
	static tstring		CONST_CRASH_DUMP_DEST_IP;
	static int			CONST_CRASH_DUMP_DEST_PORT;

	static float		COLLISION_CAPSULE_RADIUS_OFFSET;

	static void Init();
private:
	static void InitFromScript();
	static void InitFromExt();
	static void SetStringValue(MWLua* pLua, TCHAR* szoutValue, int nValueLen, const TCHAR* szScriptValueName);
	static void InitDefaultEquipments(MWLua* pLua);
	static void InitCaptionEffectColor(MWLua* pLua);
	static void SetColor(MWLua* pLua, uint32* poutValue, const TCHAR* szValueName);
	static void InitAgeLimit(MWLua* pLua);
	static void SetFakeBeatenWeapontypeAnimationBlend(MWLua* pLua);
};




