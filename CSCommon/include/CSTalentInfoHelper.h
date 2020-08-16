#pragma once

#include "CSTalentInfo.h"
#include "CSSocialMotion.h"
#include "CSCommonLib.h"

class CSCOMMON_API CSTalentInfoHelper
{
private:
	static int _NormalAttackTalentID(WEAPON_TYPE nWeaponType, TALENT_NORMAL_ATTACK_TYPE nAttackType);
	static int NormalAdvanceAttackTalentID(WEAPON_TYPE nWeaponType);
	static int NormalRisingComboTalentID(WEAPON_TYPE nWeaponType);
	static int NormalRisingAttackTalentID(WEAPON_TYPE nWeaponType);
		static int NormalUpperAttackTalentID(WEAPON_TYPE nWeaponType);
	static int NormalDownAttackTalentID(WEAPON_TYPE nWeaponType);
	static int NormalSuperAttackTalentID(WEAPON_TYPE nWeaponType);
	static int NormalCounterAttackTalentID(WEAPON_TYPE nWeaponType, bool bEquipShield);
public:
	/// �⺻ ���� �ŷ�Ʈ ID ��ȯ
	static int NormalAttackTalentID(WEAPON_TYPE nWeaponType, TALENT_NORMAL_ATTACK_TYPE nAttackType, bool bEquipShield=true);
	///// �⺻ ��� �ŷ�Ʈ ID ��ȯ
	static int NormalGuardTalentID(WEAPON_TYPE nWeaponType, int nGuardLevel, bool bIsEquippedShield);

	static int NormalSwordDrawingTalentID(WEAPON_TYPE nWeaponType);

	static bool IsNormalAttackTalent(int nTalentID);

	static bool IsPlayerNormalAttackTalent(int nTalentID);

	/// �÷��̾� �⺻ �ŷ�Ʈ���� ���� ��ȯ
	static bool IsPlayerNormalTalentID(int nSKillID);

	/// ������ �������� Ŭ���̾�Ʈ�� ��� ����ϴ� �ŷ�Ʈ���� ����
	static bool IsImmediateTalent(CSTalentInfo* pTalentInfo);

	/// Collision������ ������ ������ ��ųŸ������ ����
	static bool IsCollidableSkillType(TALENT_SKILL_TYPE nSkillType);

	/// �Ҽ� ��� �ŷ�Ʈ ID ��ȯ
	static int SocialMotionTalentID(CSSocialMotionType nSocialMotionType);

	/// ĳ���� ������ �̸� ��� ����Ʈ�� �ŷ�Ʈ���� ����
	static bool IsInitialLearnedExtraPassiveTalent(TALENT_EXTRA_PASSIVE_TYPE nExtraPassiveType);

	// �̵� ���� -------------------------------------------------

	/// ����
	static vec3 MoveStraight(vec3 vStartPos, vec3 vVelocity, float fElapsedTime);
	/// ������
	static vec3 MoveParabolic(vec3 vStartPos, vec3 vVelocity, float fWeight, float fElapsedTime);
	/// ����(���̷�Ʈ ȸ��)
	static vec3 MoveInduction(vec3& vOutDir, vec3 vCurrentPos, vec3 vTargetPos, vec3 vVelocity, float fTickTime);
	static vec3 MoveSmooth(vec3 vCurrDir, vec3& vOutDir, vec3 vCurrentPos, vec3 vTargetPos, vec3 vVelocity, float fTickTime, float fStartDist);
	static vec3 MoveSmoothInTime(vec3 vCurrDir, vec3& vOutDir, vec3 vCurrentPos, vec3 vTargetPos, float fTickTime, float fStartDist, float& fRemainTime);

	// ��Ÿ��� ��ȿ�Ѱ�?
	static bool CheckEffectiveRange(vec3& vTargetPos, vec3& vMyPos, float fTalentRange);

	static int NormalDodgeFrontTalentID(WEAPON_TYPE nWeaponType);
	static int NormalDodgeBackTalentID(WEAPON_TYPE nWeaponType);
	static int NormalDodgeLeftTalentID(WEAPON_TYPE nWeaponType);
	static int NormalDodgeRightTalentID(WEAPON_TYPE nWeaponType);
};