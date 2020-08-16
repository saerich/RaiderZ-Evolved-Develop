#pragma once

// [2007/5/25 isnara] Talent ����Ʈ
enum IMPACTPOS
{
	MODEL_POS = 0,			// �� ��ġ(����)
	HIT_POS,				// ���� ��ġ
	BONE_POS,				// �� ��ġ
	SHOCK_GROUND_POS,		// ������ �浹(���������� ���)
	TALENT_HIT_CAPSULE_POS	// �ŷ�Ʈ ��Ʈ ĸ��(����Ʈ Ÿ���� ����ΰ�쿡�� ����ϰڽ��ϴ�.)
};
enum TALENT_EFFECT_BASE_TYPE
{
	TEBY_MELEE_ATTACK,		// Į
	TEBY_MELEE_SECONDARY_ATTACK,		// Į ����
	TEBY_MAGIC_ATTACK,		// ����
	TEBY_ARCHERY_ATTACK,	// ȭ��
	TEBY_GUN_ATTACK,		// ��
	TEBY_DEFENSE,			// ���
	TEBY_NOSHIELD_DEFENSE,	// ���о��� ���
	TEBY_MONSTER_DEFENSE,	// ���� ���

	TEBY_BASE_MAX
};

enum TALENT_EFFECT_TYPE
{
	CAST_EFFECT = 0,		// ĳ��Ʈ
	USE_EFFECT,				// ���
	ATTACK_EFFECT,			// �ǰ�
	MISS_EFFECT,			// ����
	SHOCK_EFFECT,			// ���
	DURATION_EFFECT,		// ����
	DEFENSE_EFFECT,			// ���
	CAST_END_EFFECT,		// ĳ��Ʈ ����

	BLOOD_EFFECT,			// �ǰݽÿ� ���� �� ȿ��
	ATTACK_SECONDARY_EFFECT,// ���� �ǰ�

	HIT_EFFECT_POSDIR,		// �ǰ� ����Ʈ ��ġ����

	TALENT_EFFECT_TYPE_MAX
};

enum TALENT_EFFECT_DIRECTION
{
	TED_NONE = 0,			// ���⼺ ����
	TED_FRONT_EFFECT_USER,	// ����Ʈ ����� ����
	TED_AUTO_DIR_ATTACK_EFFECT, // �ǰ� ����Ʈ�� Į ���⿡ ���� �ڵ� ȸ��
};

enum TALENT_SHOVE_TYPE
{
	TST_NONE,
	TST_STOP,				// ����
	TST_PASS,				// ����
	TST_SCRAPE,				// ����
	TST_PUSH,				// ��ġ��
};