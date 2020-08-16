#pragma once

// �ŷ�Ʈ Ÿ��
enum TALENT_TYPE
{
	TT_SKILL	= 0,		// �Ϲ����� �ŷ�Ʈ
	TT_WEAPON	= 1,		// ���⿡ �����Ǵ� �ŷ�Ʈ
	TT_ENCHANT	= 2,		// �����ۿ� �����Ǵ� �ŷ�Ʈ
	TT_ITEM		= 3,		// ��Ƽ�� �Ӽ��� �����ۿ� �����Ǵ� �ŷ�Ʈ
	TT_GATHER   = 4,		// ä�� �ŷ�Ʈ
};

/// �ŷ�Ʈ ī�װ� - ��ų Ÿ��(TALENT_SKILL_TYPE)�� �Ϲ�ȭ
enum TALENT_CATEGORY
{
	TC_INVALID = -1,
	TC_COMMON = 0,
	TC_MELEE,
	TC_RANGE,
	TC_MAGIC,
	TC_MAX
};

/// �ŷ�Ʈ ��Ÿ��, �̰��� DB�� RZ_CODE������ ����ϰ� �ֱ� ������ ������ ������ DB�� �ݿ��� ����� �Ѵ�.
enum TALENT_STYLE
{
	TALENT_STYLE_NONE = 0,

	TALENT_STYLE_DEFENDER,
	TALENT_STYLE_BERSERKER,
	TALENT_STYLE_ASSASSIN,
	TALENT_STYLE_RANGER,
	TALENT_STYLE_SORCERER,
	TALENT_STYLE_CLERIC,
	TALENT_STYLE_COMMON,
	TALENT_STYLE_LICENSE,

	TALENT_STYLE_MAX
};

static const TCHAR* TALENT_CATEGORY_STR[] = 
{
	_T("common"),
	_T("melee"),
	_T("range"),
	_T("magic")
};

// �ŷ�Ʈ ��ų Ÿ��
enum TALENT_SKILL_TYPE
{
	ST_NONE = 0,		// ������
	ST_MELEE,			// ���� ����
	ST_ARCHERY,			// Ȱ ����
	ST_GUN,				// �� ����(���Ÿ� �������)
	ST_MAGIC,			// ������ ����ϴ� ���� (���� �ŷ�Ʈ �ϳ��� ���� ���� �������� �����), ������ ����
	ST_GUARD,			// �����	
	ST_PASSIVE,			// �нú�
	ST_BUFF,			// ����
	ST_EXTRA_ACTIVE,	// Ư���� ��Ƽ��
	ST_EXTRA_PASSIVE,	// Ư���� �нú�
	ST_SOCIAL,			// ��ɾ��� ���۸��� ���Ѵ� ex)�λ�
	ST_ITEM,			// �������� �̿��� �ŷ�Ʈ ��� ex)��ô

	ST_MAX
};

static const TCHAR* TALENT_SKILL_TYPE_STR[] = 
{
	_T("ST_NONE"),
	_T("ST_MELEE"),
	_T("ST_ARCHERY"),
	_T("ST_GUN"),
	_T("ST_MAGIC"),
	_T("ST_GUARD"),
	_T("ST_PASSIVE"),
	_T("ST_BUFF"),
	_T("ST_EXTRA_ACTIVE"),
	_T("ST_EXTRA_PASSIVE"),
	_T("ST_MOVE"),
	_T("ST_ITEM"),
	_T("ST_MAGICAREA"),
};

/// Ư���� �нú� �ŷ�Ʈ Ÿ��
enum TALENT_EXTRA_PASSIVE_TYPE
{
	TEPT_NONE		= 0,

	// ���� ���� ========================================
	TEPT_BASE_WEAPON_ONE_HANDED,	// �Ѽ� ���� ����
	TEPT_BASE_WEAPON_1H_SLASH,		// �Ѽ� �� ����
	TEPT_BASE_WEAPON_1H_BLUNT,		// �Ѽ� �б� ����
	TEPT_BASE_WEAPON_1H_PIERCE,		// �ܰ� ����
	TEPT_BASE_WEAPON_TWO_HANDED,	// ��� �� ����
	TEPT_BASE_WEAPON_STAFF,			// ������ ����
	TEPT_BASE_WEAPON_ARCHERY,		// Ȱ ����
	TEPT_BASE_WEAPON_2H_BLUNT,		// ��� �б� ����
	TEPT_BASE_WEAPON_DUAL_PIERCE,	// �� �ܰ� ����	
	
	TEPT_BASE_WEAPON_GUN,			// �� ����	
	TEPT_BASE_WEAPON_BOOK,			// å ����


	// ��Ÿ ���� ========================================
	TEPT_BASE_SNEAK,				// ���� ����
	TEPT_BASE_ELEMENTAL,			// ���� ����
	TEPT_BASE_HOLY,					// �ż� ����

	// ���� Ưȭ ========================================
	TEPT_SPECIALIZATION_SLASH,		// ���� Ưȭ
	TEPT_SPECIALIZATION_BLUNT,		// �ļ� Ưȭ
	TEPT_SPECIALIZATION_PIERCE,		// ��� Ưȭ
	TEPT_SPECIALIZATION_ELEMENTAL,	// ���� Ưȭ
	TEPT_SPECIALIZATION_HOLY,		// �ż� Ưȭ

	// ��ƿ��Ƽ ========================================
	TEPT_UTILITY_SAFEFALL,			// ���� ����
	TEPT_UTILITY_SAFEFALL2,			// �߱� ����

	// ���� ���� ========================================
	TEPT_BASE_ARMOR_LIGHT,			// ���ʰ��� ����
	TEPT_BASE_ARMOR_MEDUIM,			// �߱ް��� ����
	TEPT_BASE_ARMOR_HEAVY,			// ��ް��� ����
	
	// �ŷ�Ʈ ========================================
	TEPT_TALENT_SHOUT_RADIUS,				// ��ġ�� ���� �ŷ�Ʈ�� ���� ���� ����
	TEPT_TALENT_SAZAHU_SPECIALIZATION,		// ������ Ưȭ
	// ---------------------------------------------

	// ��Ŀ�� ======================================
	TEPT_FOCUS_USE_BERSERK,
	TEPT_FOCUS_USE_COUNTER,
	TEPT_FOCUS_BERSERK_DECREASE_MAXBUBBLE,		// ����Ŀ �ʿ���� ���� ����
	TEPT_FOCUS_BERSERK_EXTENDTIME,		// ����Ŀ ���ӽð� ����
	TEPT_FOCUS_COUNTER_EXTENDTIME,		// �ݰ� ���ӽð� ����
	TEPT_FOCUS_ADVENT_EXTENDTIME,	// ���� ���ӽð� ����
	// ---------------------------------------------

	// ���⼭���� �����
	TEPT_QUICK_RISE,				///< ���� ���
	TEPT_QUICK_DRAW,				///< �ż��� �����¼�
	TEPT_3COMBO,					///< 3�޺� ����
	TEPT_SWORD_DRAWING,				///< �ߵ�
	TEPT_UPPERCUT,					///< ����
	TEPT_RISING_ATTACK,				///< ��� ����
	TEPT_GUARD,						///< ��� ����ȭ
	TEPT_COUNTER,					///< ī���� ����	
	TEPT_MAGICAL_DYNAMICS,			///< ���� ����
	TEPT_ITEM_THROWING,				///< (������ ���)��ô
	TEPT_ITEM_TRAP,					///< (������ ���)�� ���


	TEPT_MAX,
};

/// Ư���� ��Ƽ�� �ŷ�Ʈ Ÿ��
enum TALENT_EXTRA_ACTIVE_TYPE
{
	TEAT_NONE		= 0,
	// pc�� ��� ----
	TEAT_PORTAL,			// ��Ż
	TEAT_PORTAL_SOULBINDING,// ��Ż - �ҿ���ε�
	TEAT_REBIRTH,			// ��Ȱ
	TEAT_TAUNT,				// ����
	TEAT_BUFF_ENTITY,		// ���� ��ƼƼ ����
	TEAT_SPAWN,				// ����

	// ��Ŀ�� ���� ------------
	TEAT_TOGGLE_PRECISION,		// ���� ���
	TEAT_TOGGLE_SNEAK,			// ���� ���
	TEAT_ADVENT,				// ����
	// pc�� ��� ----^

	// npc�� ��� ----
	TEAT_NPC_ONLY_GRAPPLE,		// ���, ����
	TEAT_NPC_ONLY_EAT,			// �Ա�
	// npc�� ��� ----^


	// ���� 
	TEAT_DISPEL,
	TEAT_CURE_POISON,
	TEAT_CURE_DISEASE,
	TEAT_CURE_CURSE,
	TEAT_DISPEL_MESMERIZE,
	TEAT_DISPEL_DEATHPENALTY,
	TEAT_DISPEL_BUFF,
	TEAT_DISPEL_FOCUS,
	TEAT_DISPEL_BPART,

	TEAT_MAX,
};

/// �ΰ������� ���� �ŷ�Ʈ ī�װ�
enum TALENT_AI_CATEGORY
{
	TAC_UTILITY_CLASS		= 0,
	TAC_OFFENSIVE_DD		= 1,
	TAC_OFFENSIVE_DOT		= 2,
	TAC_OFFENSIVE_AE		= 3,
	TAC_OFFENSIVE_PB		= 4,
	TAC_OFFENSIVE_DEBUFF	= 5,
	TAC_DEFENSIVE_BUFF		= 6,
	TAC_DEFENSIVE_HEALING	= 7,
	TAC_DEFENSIVE_ENCHANT	= 8,
};

/// �⺻ ������ Ÿ��
enum TALENT_NORMAL_ATTACK_TYPE
{
	NA_NA		= -1,
	NA_NORMAL1 = 0,			// �Ϲ� ���� �޺�1
	NA_NORMAL2,				// �Ϲ� ���� �޺�2
	NA_NORMAL3,				// �Ϲ� ���� �޺�3	
	NA_NORMAL4,				// �Ϲ� ���� �޺�4
	NA_NORMAL5,				// �Ϲ� ���� �޺�5
	NA_NORMAL6,				// �Ϲ� ���� �޺�6
	NA_ADVANCE,				// ���� ����
	NA_DODGE_FRONT,			// �Һ�
	NA_DODGE_BACK,
	NA_DODGE_LEFT,
	NA_DODGE_RIGHT,
	NA_UPPER,				// ����
	NA_DOWN,				// �ϴ� ����
	NA_SUPER,				// ���⺰ Ư�� ����
	NA_RISING,				// ��� ����
	NA_SWORD_DRAWING,		// �ߵ� ����
	NA_UPPERCOMBO1,			// ���� �޺�
	NA_COUNTER_ATTACK,		// ī���� ����

	// ���⼭���ʹ� Ŭ���̾�Ʈ�� ���(�޺� ���̺� ����� ���� - birdkr
	NA_USE_TALENT__CLIENT_ONLY,			
	NA_GUARD__CLIENT_ONLY,

	NA_MAX
};

// �߻�ü Ÿ��
enum TALENT_PROJECTILE_TYPE
{
	TPT_NONE = 0,
	TPT_MISSILE,			// �������� ���ư�
	TPT_MISSILE_GUIDED,		// ����
	TPT_HITCAPSULE_GUIDED	// ����ĸ�� ����
};

// �߻�ü ����� Ÿ��
enum TALENT_PROJECTILE_VISUAL_TYPE
{
	TPVT_CURVE,				// ����
	TPVT_STRAIGHT,			// �̻���
	TPVT_PARABOLA			// ������
};

/// �ŷ�Ʈ ����� �ܰ�
enum TALENT_PHASE
{
	TALENT_PHASE_NONE = -1,
	TALENT_PHASE_PREPARE=0,		// �غ� �ܰ�(ĳ����)
	TALENT_PHASE_ACT,			// ������ ����
	TALENT_PHASE_POST_DELAY,	// Post Delay ó��
	TALENT_PHASE_FINISH,		// ������
	TALENT_PHASE_CANCELED,		// �߰��� ��ҵ�
	TALENT_PHASE_EXTRA,			// �ŷ�Ʈ Ÿ�Կ� ���� ���������� ����� �� �ִ� �ܰ� - ������ ������� �ʴ´�.
	TALENT_PHASE_EXTRA2,		// �ŷ�Ʈ Ÿ�Կ� ���� ���������� ����� �� �ִ� �ܰ� - ������ ������� �ʴ´�.
	TALENT_PHASE_EXTRA3,		// �ŷ�Ʈ Ÿ�Կ� ���� ���������� ����� �� �ִ� �ܰ� - ������ ������� �ʴ´�.
	TALENT_PHASE_MAX
};

/// �ŷ�Ʈ �迭
enum TALENT_LINE
{
	TL_CLOSE_COMBAT_MASTERY=1,	///< ���� ���� �����͸�
	TL_SLASH_MASTERY=2,			///< ���� �����͸�
	TL_REVIVE1=1001,			///< ��Ȱ �迭
	TL_REVIVE2=1002,			///< ��Ȱ �迭
};


/// �ŷ�Ʈ �̺�Ʈ ���
enum TALENT_EVENT
{
	TE_NONE					= 0,
	TE_FIRE_EFFECT			,
	TE_LAUNCH_PROJECTILE	,		// �߻�ü �߻� - Ŭ���̾�Ʈ, ���� ���
	TE_EFFECT				,		// ����Ʈ ��� - Ŭ���̾�Ʈ ����
	TE_SOUND				,		// ���� ��� - Ŭ���̾�Ʈ ����
	TE_CAMERA				,		// ī�޶� ȿ�� - Ŭ���̾�Ʈ ����
	TE_DAMAGE				,		// ������ �߻� - Ŭ���̾�Ʈ ����
	TE_CUSTOM				,		// ����� ȿ�� - Ŭ���̾�Ʈ ����
	TE_DELAYED_ACT			,		// ���� ȿ��   - Ŭ���̾�Ʈ, ���� ���
	TE_EXTRAACTIVE_1		,		// Extra Active 1 - ���� ����
	TE_EXTRAACTIVE_2		,		// Extra Active 2 - ���� ����
	TE_CAMEAR_LOCK			,		// ī�޶� ����� -  Ŭ���̾�Ʈ ����
	TE_GROUND_FIRE_EFFECT	,		// ���� �߻� ����Ʈ - Ŭ���̾�Ʈ ����
	TE_MAX
};



enum TALENT_CONDITION
{
	TC_NONE = 0,
	TC_USE_TALENT,			// �ŷ�Ʈ�� ����� ���
	TC_FINISH_TALENT,		// �ŷ�Ʈ�� �Ϸ��� ���
	TC_BUFF_GAIN,			// ������ �ɸ� ���
	TC_BUFF_GONE,			// ������ ���� ���
	TC_BUFF_GAIN_N_PERIOD,	// ������ �ɸ� ���� �ֱ��� ȿ��
	TC_BUFF_PERIOD,			// ������ �ֱ��� ȿ��
	TC_BUFF_DISPELLED,		// ������ ���� ���� ��
	TC_BUFF_DUPLICATED,		// ������ �ߺ��ؼ� �ɷ��� ��
	TC_BUFF_EXPIRED,		// ������ ���� ���� ��
	TC_BUFF_HIT,			// ������ �ǰݵǾ��� ��
	TC_BUFF_MAX_STACKED,	// ������ �ִ�� ��ø�Ǿ��� ��
	TC_BUFF_STACKED,		// ������ ��ø�Ǿ��� ��
	TC_HIT_TALENT,			// �ŷ�Ʈ�� �ǰ�������
	TC_HIT_SUCCESS,			// �ŷ�Ʈ�� �ǰ��� �ѹ��̶� ��������
	TC_HIT_ALL_TALENT,		// �ŷ�Ʈ�� ��� �ǰݽ������� (Hitcapsule type only)
	TC_HIT_ME,				// ���� �ǰ� ��������
	TC_MISS_ME,			// ���� ������ ȸ��������
	TC_GUARD_ME,			// ���� ������ ���������
	TC_PERFECT_GUARD_ME,	// ���� �Ϻ� ���, ���� ���������
	TC_CRITICAL_HIT_ME,		// ���� ������ ũ��Ƽ�÷� �ǰ� ��������
	TC_HIT_ENEMY,			// ��밡 ���ݿ� �ǰݵ�����
	TC_MISS_ENEMY,			// ��밡 ������ ��������
	TC_GUARD_ENEMY,			// ��밡 ������ ���������
	TC_CRITICAL_HIT_ENEMY,	// ��밡 ���ݿ� ũ��Ƽ�÷� �ǰݵ�����
	TC_COUNTER,				// ����� ������ ��ȿȭ��Ű�� �ǰ� ������ ��
	TC_RESIST,				// �ֱ����� ����üũ (������)
	TC_HIT_ME_N,			// ���� �ǰ� ���� Ƚ���� N �̻��� �� (������)
	TC_CRIHIT_ME_N,			// ���� ũ��Ƽ�� �ǰ� ���� Ƚ���� N �̻��� �� (������)
	TC_DAMAGE,				// ���� ���� �������� N �̻��� �� (������)
	TC_AFFECTED,			// Trigging �� ������ ȿ���� �ۿ����� �� (������)
	TC_MAGIC_ACT,			// ���� �ߵ��� ��
	TC_MAGIC_HIT_ME,		// ������ �ǰݵ��� ��
	TC_MAGIC_HIT_ENEMY,		// ������ �������� ��
	TC_MELEE_HIT_ME,		// ���� ���ݿ� �ǰݵ��� ��
	TC_MELEE_HIT_ENEMY,		// ���� ������ �������� ��
	TC_DO_SOMETHING,		// ���� �������� �ൿ�� ���� ��
};

enum TALENT_GUARD_DIRECTION
{
	GUARD_DIRECTION_FRONT = 1,
	GUARD_DIRECTION_BACK  = 2,
	GUARD_DIRECTION_ALL	 = 3
};

/// �ŷ�Ʈ ���� ��밡���� ����
enum TALENT_WEAPON_ALLOWED
{
	TALENT_WEAPON_ALLOWED_1HS = 0,		///< 1hs�� ��� �Ұ�
	TALENT_WEAPON_ALLOWED_1HB,
	TALENT_WEAPON_ALLOWED_SHD,			///< ���带 �� ���� �־�� �ߵ���
	TALENT_WEAPON_ALLOWED_2HD_H,		///< ������ 2hd�� _h�Դϴ�. �߷���(heavy) ������ ��
	TALENT_WEAPON_ALLOWED_2HD_L,		///< �̱���. ���߿� �����Ұ̴ϴ�. �淮�� ��հ�(light)
	TALENT_WEAPON_ALLOWED_1HP_S,		///< ������ 1hp. short Ÿ��
	TALENT_WEAPON_ALLOWED_1HP_L,
	TALENT_WEAPON_ALLOWED_DWD,
	TALENT_WEAPON_ALLOWED_BOW,
	TALENT_WEAPON_ALLOWED_2HB,
	TALENT_WEAPON_ALLOWED_STF			///< ������
};


/// ���� ���
enum TALENT_RESIST_METHOD
{
	TRM_NONE	= -1,

	TRM_BINARY = 0,		// ���� ����
	TRM_PARTIAL,		// �κ� ����

	TRM_MAX
};

/// ���� Ÿ��
enum TALENT_RESIST_TYPE
{
	TRT_NONE	= -1,

	TRT_FR		= 0,
	TRT_CR,
	TRT_PR,
	TRT_LR,
	TRT_HR,
	TRT_UR,
	TRT_CHA,

	TRT_MAX
};

enum TALENT_DRAIN_TYPE : unsigned char
{
	TALENT_DRAIN_HP = 0,
	TALENT_DRAIN_EN,
	TALENT_DRAIN_SP,

	TALENT_DRAIN_MAX
};

/// ĵ�� �� ����� �Ķ��Ÿ
enum CANCEL_NEXT_ATTACK_PARAM
{
	CANCEL_NEXT_ATTACK_PARAM_ALL		= 0,		///< ��� ĵ��
	CANCEL_NEXT_ATTACK_PARAM_UPPERED	= 1,		///< uppercombo�� ����
};

