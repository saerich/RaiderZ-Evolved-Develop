#pragma once

#include "MTypes.h"
#include "MXml.h"
#include <string>
#include "MTstring.h"
#include <map>
#include <set>
#include <deque>
using namespace std;

#include "CSCommonLib.h"
#include "CSMotionFactorInfo.h"
#include "CSItemData.h"
#include "CSChrInfo.h"
#include "CSDef.h"
#include "CSModifier.h"
#include "CSTalentDef.h"
#include "CSTalentInfoEnum.h"
#include "CSEffectInfo.h"

class CSTalentInfo;

// �ǹ޾�ġ�� ������
struct CSRiposte
{
	TALENT_CATEGORY		nType;
	float				fRate;		// (1 = 100%)
	CSRiposte() : nType(TC_MELEE), fRate(0.0f) {}
};

/// �ŷ�Ʈ ���� ������
class CSHitSegment
{
public:
	vector<MCapsule>	m_vCapsules;			///< �浹 ���� - ĸ��
	vector<bool>		m_vDoHitCheck;			///< ������ĸ���� �Ÿ���꿡 ���� ���� �Ǵ�
	float				m_fCheckTime;			///< �浹 üũ �ð� - �������� �ŷ�Ʈ ������ �� �ð��� �帥 �Ŀ� �ѹ� üũ�Ѵ�.
	int					m_nAddtiveDamage;		///< �߰� ������ - �ŷ�Ʈ ������ m_nDamage�� �� ���� �߰��ؼ� �������� ����Ѵ�.
	float				m_fHitMotionDelay;		///< �ǰ� �ִϸ��̼� ������ - Ŭ���̾�Ʈ���� �� �ִϸ��̼��� �ڿ������� ������ ���� ���. ���� Ŭ���̾�Ʈ�� ����ȵ�.
	CSHitSegment() : m_fCheckTime(0.0f), m_fHitMotionDelay(0.0f), m_nAddtiveDamage(0) {}
};

/// �ŷ�Ʈ ���� ����
class CSCOMMON_API CSTalentHitInfo
{
public:
	int						m_nID;
	vector<CSHitSegment>	m_vSegments;

	// ���۷���. TalentInfo Cooking�� �� ���۷��� �������� ���� �����´�
	int						m_nReferenceID;
	int						m_nReferenceMode;

	bool					TestCollision(MCapsule& c);
	void					CopyTo(CSTalentHitInfo& dest)
	{
		// reference ������ �������� �ʴ´�.

		dest.m_nID = this->m_nID;
		dest.m_vSegments = this->m_vSegments;
	}

	CSTalentHitInfo();
	void Clear();
};

/// Spell ����
class CSSpellInfo
{
public:
	int						m_nTalentID;
	float					m_fStartTime;

	CSSpellInfo(int nID, float fStart) : m_nTalentID(nID), m_fStartTime(fStart) {}
};

/// �߻�ü ����
class CSProjectileInfo
{
public:
	// ����
	TALENT_PROJECTILE_TYPE		m_nType;						// �߻�ü Ÿ��
	TALENT_PROJECTILE_VISUAL_TYPE m_nVisualType;				// �߻�ü ����� Ÿ��
	
	tstring						m_strBoneName;					// �߻� ��ġ�� �����ϴ� �� �̸�
	tstring						m_strEffectName;				// �߻�ü ����Ʈ
	tstring						m_strAttackEffectName;			// �߻�ü �ǰ� ����Ʈ
	tstring						m_strHitGroundEffectName;		// �߻�ü ���� �ǰ� ����Ʈ
	tstring						m_strDefenceEffectName;			// �߻�ü ��� ����Ʈ
	tstring						m_strFireEffectName;			// �߻�ü �߻� ����Ʈ

	float						m_fSpeed;						// �߻�ü �ӵ�
	float						m_fHitRadius;					// �߻�ü ���� ũ��
	float						m_fRange;						// �߻�ü�� �����Ÿ�(����, m)

	// ����
	vec3						m_vLocalStartPos;				// �߻� ��ġ (����, �⺻)

	// Ŭ��
	int							m_nSegmentIndexForHitCapsuleType;				// ���� ���׸�Ʈ
	int							m_nCapsuleIndexForHitCapsuleType;				// ���� ĸ��

	CSProjectileInfo()
	{
		m_nType										= TPT_NONE;
		m_nVisualType								= TPVT_STRAIGHT;

		m_strBoneName.clear();
		m_strEffectName.clear();
		m_strAttackEffectName.clear();
		m_strHitGroundEffectName.clear();
		m_strDefenceEffectName.clear();
		m_strFireEffectName.clear();

		m_vLocalStartPos							= vec3::ZERO;

		m_fSpeed									= 100.0f;
		m_fHitRadius								= 30.0f;
		m_fRange									= 0.0f;

		m_nSegmentIndexForHitCapsuleType			= -1;
		m_nCapsuleIndexForHitCapsuleType			= -1;
	}
};

/// �ŷ�Ʈ �̺�Ʈ ����
class CSTalentEventInfo
{
public:
	TALENT_EVENT		m_nEvent;
	float				m_fTime1;
	float				m_fTime2;
	tstring				m_strCustomEvent;
	tstring				m_strParam1;
	tstring				m_strParam2;
	int					m_nParam1;
	int					m_nParam2;
	bool				m_bFollow;
	bool				m_bServerDir;

	vec2				m_vLocal;

	CSProjectileInfo	m_ProjectileInfo;						// �߻�ü ������ �̺�Ʈ���� ó��

	CSTalentEventInfo() : m_nEvent(TE_NONE), m_fTime1(-1.0f), m_fTime2(-1.0f), m_nParam1(0), m_nParam2(0), m_bFollow(true), m_vLocal(0, 0), m_bServerDir(false) {}

	// map �����̳� �񱳿뵵
	bool operator() (CSTalentEventInfo& lhs, CSTalentEventInfo& rhs) const
	{
		return lhs.m_fTime1 < rhs.m_fTime1;
	}
};

/// �ŷ�Ʈ �̺�Ʈ
class CSTalentEvents
{
public:
	int							m_nRefID;
	int							m_nRefMode;

	vector<CSTalentEventInfo>	m_vecEvents;		///< �Ϲ����� �̺�Ʈ
	vector<CSTalentEventInfo>	m_vecActEvents;		///< Act�κ��� �ð� ���� �̺�Ʈ(�÷��̾� ���� �� ���)

	CSTalentEvents()
	{
		m_nRefID	= 0;
		m_nRefMode	= 0;
	}

	bool HasEvent()
	{
		return !(m_vecEvents.empty() && m_vecActEvents.empty());
	}

	bool HasEvent(TALENT_EVENT eEvent)
	{
		vector<CSTalentEventInfo>::iterator itEvents = m_vecEvents.begin();
		while(itEvents != m_vecEvents.end())
		{
			if((*itEvents).m_nEvent == eEvent)
				return true;

			itEvents++;
		}
		vector<CSTalentEventInfo>::iterator itActEvents = m_vecActEvents.begin();
		while(itActEvents != m_vecActEvents.end())
		{
			if((*itActEvents).m_nEvent == eEvent)
				return true;

			itActEvents++;
		}

		return false;
	}
};

/// �ŷ�Ʈ ������ ���� ����
struct GTalentResist
{
	TALENT_RESIST_METHOD		m_nMethod;		///< ���� ���
	TALENT_RESIST_TYPE			m_nType;		///< ���� Ÿ��
	uint8						m_nDifficulty;	///< ���� ���̵�

	GTalentResist() : m_nMethod(TRM_BINARY), m_nType(TRT_NONE), m_nDifficulty(255) {}
};

/// �ŷ�Ʈ ������ ��� ����
struct GTalentDrain
{
	float				m_fRate;
	TALENT_DRAIN_TYPE	m_nType;
	GTalentDrain() : m_fRate(0.0f), m_nType(TALENT_DRAIN_HP) {}
	bool IsDrainTalent() const { return (m_fRate != 0.0f); }
};

/// ���� ����
struct GTalentHitFilter
{
	bool	bMotionFactor[MF_SIZE];
	bool	bAltitude[AT_SIZE];
	GTalentHitFilter()
	{
		for (int i = 0; i < MF_SIZE; i++) bMotionFactor[i] = true;
		for (int i = 0; i < AT_SIZE; i++) bAltitude[i] = true;

		bMotionFactor[MF_KNOCKDOWN] = false;
		bMotionFactor[MF_THROWUP] = false;
		bMotionFactor[MF_UPPERED] = false;
		bAltitude[AT_SKY] = false;
	}
};

struct CSBuffEnchantInfo
{
	int						nID;	
	CSEffectInfo		EffectInfo;		// ������ ������ �Ǵ� ����� ����
	TALENT_CONDITION	nEnchantEvent;		// ������ ������ �Ǵ� ����
	GTalentResist		Resist;

	int					nRange;			// ���� ����

	CSBuffEnchantInfo() : 
	nID(INVALID_BUFF_ID), nEnchantEvent(TC_NONE) {}
};

// �ŷ�Ʈ �ߵ����� ����
struct CSTalentConditionInfo
{
	struct Release
	{
		// ���� ȿ���� �Ҵ� ���� Ÿ��
		TALENT_CONDITION	nType;
		// ���� ȿ���� �Ҵ� �߰� ���� Ÿ��
		vector<TALENT_CONDITION>	vecAdditionType;
		// ���� ȿ���� �Ҵ� ���� ����
		int	nParam;		

		Release()
		{
			nType  = TC_NONE;
			nParam = 0;
		}
	} infoRelease;

	// ���� ȿ���� ��� ���� Ÿ��
	TALENT_CONDITION	nEffectConditionType;	


	CSTalentConditionInfo()
	{
		nEffectConditionType	= TC_BUFF_GAIN_N_PERIOD;
	}
};

struct CSCOMMON_API CSTalentWeaponAllowdInfo
{
	set<int>		m_setWeapons;
	bool			m_bAllAllowed;
	bool			m_bNeedEquipShield;
	CSTalentWeaponAllowdInfo()
	{
		m_bAllAllowed = true;
		m_bNeedEquipShield = false;
	}
	bool IsAllowed(TALENT_WEAPON_ALLOWED nWeapon);
	bool IsAllowed(WEAPON_TYPE nWeaponType, bool bEquipShield);
};

struct CSTalentActAnimationTimeInfo
{
	WEAPON_TYPE		nWeaponType;
	float			fAniTime;

	CSTalentActAnimationTimeInfo()
	{
		nWeaponType	= WEAPON_NONE;
		fAniTime	= 0.0f;
	}
};

struct CSTalentActAnimationTime
{
	vector<CSTalentActAnimationTimeInfo> vecInfo;

	CSTalentActAnimationTime()
	{
		vecInfo.clear();
	}

	float GetTime(WEAPON_TYPE nWeaponType = WEAPON_NONE)
	{
		for(vector<CSTalentActAnimationTimeInfo>::iterator itTime = vecInfo.begin(); itTime != vecInfo.end(); itTime++)
		{
			if((*itTime).nWeaponType == nWeaponType)
				return (*itTime).fAniTime;
		}

		return 0.0f;
	}

	CSTalentActAnimationTimeInfo* GetActAnimationTimeInfo(WEAPON_TYPE nWeaponType = WEAPON_NONE)
	{
		for(vector<CSTalentActAnimationTimeInfo>::iterator itTime = vecInfo.begin(); itTime != vecInfo.end(); itTime++)
		{
			if((*itTime).nWeaponType == nWeaponType)
				return &(*itTime);
		}

		return NULL;
	}
};

enum TALENT_FINISH_TYPE
{
	TALENT_FINISH_NONE = 0,
	TALENT_FINISH_THROW_UP,
	TALENT_FINISH_KNOCK_BACK,
	TALENT_FINISH_RANDOM
};

struct CSTalentFinishEffect
{
	TALENT_FINISH_TYPE	m_nFinishType;
	bool				m_bBreakableParts;
	CSTalentFinishEffect() : m_nFinishType(TALENT_FINISH_NONE), m_bBreakableParts(false) {}
};

const int MAX_TALENT_MODE = WEAPON_MAX;

class CSCOMMON_API CSTalentInfoModes
{
public:
	bool			m_bExistMode;
	CSTalentInfo*	m_Modes[MAX_TALENT_MODE];

	CSTalentInfoModes();
	~CSTalentInfoModes();
	void ClearButNotDelete();
};

struct CSTalentInfoNPCChangeMode
{
	bool			m_bChangeMode;
	NPC_MODE		m_nModeTo;
	CSTalentInfoNPCChangeMode() : m_bChangeMode(false), m_nModeTo(NPC_MODE_DEFAULT) {}
};

struct RequireBuff
{
	vector<int> vecIncludeBuffID;		// �ش� ������ ������ ȿ�� ����
	vector<int> vecExcludeBuffID;		// �ش� ������ ������ ȿ�� ����
	bool		bKeepIncludeBuff;
	bool		bKeepExcludeBuff;

	RequireBuff(): bKeepIncludeBuff(true), bKeepExcludeBuff(true)	{}
};

enum WEAPON_REFRENCE
{
	WR_NONE = 0,
	WR_LEFT,
	WR_RIGHT,
	WR_BOTH,

	WR_MAX
};

enum MOVING_USE_TYPE
{
	MUT_NONE = 0,
	MUT_CASTING,
	MUT_USE,
	MUT_ALL
};

enum TARGETING_TYPE
{
	TTT_STICKY,
	TTT_LINE,
	TTT_GROUND
};

/// �ŷ�Ʈ ����
// ���ó�� (Ŭ��, ����, ���)�� ǥ���Ѵ�.
class CSCOMMON_API CSTalentInfo
{
protected:
	bool					m_bCooked;
	CSTalentInfoModes		m_Modes;
public:
	int						m_nID;							// id (���)
	int						m_nMode;						// mode - weapon ������ �� id�� �ŷ�Ʈ�� �������� TalentInfo�� ����ϰ� ������ �����(���)
	int						m_nOverride;


	// �Ϲ� ---------------------------
	tstring					m_szName;						// �ŷ�Ʈ �̸�(���)
	int						m_nHPCost;						// hp cost(Not Used) (���)
	int						m_nENCost;						// en cost(Not Used) (���)
	int						m_nSTACost;						// sta cost(Not Used) (���)
	TALENT_CATEGORY			m_nCategory;					// ī�װ�(���)
	TALENT_TYPE				m_nTalentType;					// �ŷ�Ʈ Ÿ�� (���)
	TALENT_SKILL_TYPE		m_nSkillType;					// �ŷ�Ʈ Ÿ���� ��ų�϶��� Ÿ�� (���)SwordTrailEffectForFocus
	TALENT_STYLE			m_nStyle;						// �ŷ�Ʈ ��Ÿ��
	TALENT_EXTRA_PASSIVE_TYPE	m_nExtraPassive;			// �ŷ�Ʈ Ÿ���� extra passive�� �� �� �׸��� �нú� �ɷ��� ������. (���)
	int							m_nExtraPassiveParam;		// �нú��� �߰����� ��ġ
	TALENT_EXTRA_PASSIVE_TYPE	m_nExtraPassive2;			// �ŷ�Ʈ Ÿ���� extra passive�� �� �� �׸��� �нú� �ɷ��� ������. (���)
	int							m_nExtraPassiveParam2;		// �нú��� �߰����� ��ġ

	TALENT_EXTRA_ACTIVE_TYPE	m_nExtraActive;				// �ŷ�Ʈ Ÿ���� extra active�� �� �ش� ��ų�� ����ϸ� Ư���� �ൿ�� �Ѵ�. (���)
	int							m_nExtraActiveParam1;		// ��Ƽ���� �߰����� ��ġ
	int							m_nExtraActiveParam2;		// ��Ƽ���� �߰����� ��ġ
	uint32					m_nAICategories;				// AI�� ���� �ŷ�Ʈ Ÿ�� (����)
	TALENT_CONDITION		m_nTiming;					// �ߵ� ����
	bool					m_bIgnoreMesmerize;				// Disabled ���¸� �����ϰ� ��� �������� ����(���)
	float					m_fCoolTime;					// ��Ÿ��(��)
	float					m_fDurationTime;				// �ŷ�Ʈ ���ӽð�

	int						m_nTalentLine;					// ��ũ ���� (���)
	int8					m_nRank;						// ��ũ (���)
	float					m_fCastingTime;					// ĳ�����ϴµ� �ɸ��� �ð� (���)
	unsigned int			m_nEnabledStanceFlag;			// �ŷ�Ʈ ��� ������ ���Ľ� (���)
	bool					m_bPerfectGuardOnly;			// �Ϻ����θ� �������� ���� (���)
	bool					m_bPartialGuardHit;				// �Ϻ���������� �κй��� ó������ ���� (���)
	bool					m_bCancelable;					// �ŷ�Ʈ ����� �ǰݽ� ��� ���� ���� (���)
	int						m_nHitCapsuleGroupIndex;		// �ŷ�Ʈ�� ���Ǵ� ���� �ٲ� NPC ĸ���׷��ȣ (���)
	int						m_nExtraHitCapsuleGroup;		// �ŷ�Ʈ�� ExtraPhase�� �Ǹ� �ٲ� NPC ĸ���׷��ȣ (���)
	int						m_nExtraHitCapsuleGroup2;		// �ŷ�Ʈ�� ExtraPhase2�� �Ǹ� �ٲ� NPC ĸ���׷��ȣ (���)
	int						m_nExtraHitCapsuleGroup3;		// �ŷ�Ʈ�� ExtraPhase3�� �Ǹ� �ٲ� NPC ĸ���׷��ȣ (���)

	bool					m_bUntrainable;					// �ŷ�Ʈ �ʱ�ȭ ��뿩�� (���)

	bool					m_bAvoidable;					// ȸ�� ����
	bool					m_bAvailableOnGuard;			// ����� ��� ���� ����(���)


	// ��ȿ�� -------------------------
	CSEffectInfo			m_EffectInfo;					// �ŷ�Ʈ�� ȿ���� �޴� ��� ����
	float					m_fRange;						// �߻�ü�� �����Ÿ�(����, m)
	float					m_fRequireMoveSpeed;			// �ŷ�Ʈ�� ����� �� �ʿ��� �̵��ӵ�

	// Art ----------------------------
	tstring					m_szCastingAnimation;			// ĳ���ý� �ִϸ��̼� (���)
	tstring					m_szCastingLoopAnimation;		// ĳ���� �ִϸ��̼� ������ ���� (Ŭ��)
	tstring					m_szUseAnimation;				// �ŷ�Ʈ ��� �ִϸ��̼� (���)
	tstring					m_szExtraActAnimation;			// �ŷ�Ʈ ��� �ִϸ��̼� (���)
	tstring					m_szExtraActAnimation2;			// �ŷ�Ʈ ��� �ִϸ��̼� (���)
	tstring					m_szExtraActAnimation3;			// �ŷ�Ʈ ��� �ִϸ��̼� (���)

	tstring					m_szIcon;						// ������
	tstring					m_szTargetArt;					// Ÿ�� ����Ʈ(Not Used)
	int						m_nTargetAttachmentPoint;		// Ÿ�� ����Ʈ �ٴ� ��ġ(Not Used)

	MOVING_USE_TYPE			m_nMovableType;				// �̵� ������ Ÿ���ΰ�

	// ���� ---------------------------
	bool					m_bUseAiming;					// ĳ������ Use�� Ÿ�ٿ� ���� ���� ��ȯ
	int						m_nHateAdj;	
	float					m_fHateFactor;

	float					m_fHitDelay;

	GTalentResist			m_Resist;

	// ����
	CSTalentHitInfo			m_HitInfo;						// ���� ����


	float					m_fHitStartTime;				// ���� ���� �ð�
	float					m_fHitDurationTime;				// ���� ���� �ð�
	vec3					m_vHitPosition;					// ���� ��ġ
	float					m_fHitSize;						// ���� ũ��(�ϴ� ���� ������)


	// �ǰ��� ȿ��
	int						m_nEffectTime;					// ȿ�� ���� �ð�
	float					m_fEffectAreaMin;				// ȿ�� �ּҰŸ� - melee���� ���(Not Used)
	float					m_fEffectAngle;					// ȿ�� ���� ���� - melee���� ���(Not Used)

	DAMAGE_ATTRIB			m_nDamageAttrib;					// ������ ���� (����)
	int						m_nMinDamage;					// �ּ� ������
	int						m_nMaxDamage;					// �ִ� ������
	uint8					m_nDodgeDifficulty;				///< ȸ�� ���̵�(0~255)

	int						m_nMinHeal;						// �ּ� ȸ����
	int						m_nMaxHeal;						// �ִ� ȸ����

	int						m_nHeal;						// ġ��(Not Used)
	float					m_fKnockBack;				// KnockBack(Not Used)

	MOTION_FACTOR_GROUP		m_MotionFactorGroup;
	CSTalentEvents			m_Events;						///< �̺�Ʈ ����

	bool					m_bUseBloodEffect;				///< �ŷ�Ʈ������� �ǰݽ� ���� ����Ʈ ���

	RequireBuff				m_RequireBuff;

	CSBuffEnchantInfo		m_Buff1;
	CSBuffEnchantInfo		m_Buff2;

	CSInstantModifier		m_InstantModifier;
	CSActorModifier			m_ActorModifier;
	CSPlayerModifier		m_PlayerModifier;

	CSTalentWeaponAllowdInfo	m_WeaponAllowedInfo;

	// �˱�
	tstring					m_strSwordTrailEffect;

	CSTalentFinishEffect	m_FinishEffect;			// �� �ŷ�Ʈ�� �׿��� �� ȿ��
	// NPC�� ���
	bool					m_bSetDirection;		// �ŷ�Ʈ ���� ��ǥ�� �ٶ��� ����
	set<int>				m_setNPCID;				// �� �ŷ�Ʈ�� ����ϴ� NPC ID ����
	CSTalentInfoNPCChangeMode	m_ChangeModeInfo;	// ��� ���� ����

	bool					m_bNeedTraining;		// ����߸� ����Ҽ� �ִ���

	// �н� �ʿ� ����
	int						m_nTNeedLevel;			// ����
	int						m_nTNeedTalent;			// ���� �ŷ�Ʈ
	int						m_nTNeedStyleTP;		// ���ڵ� Style TP

	// ��Ŀ�� -------------------------
	// �ŷ�Ʈ�� ���µ� �ʿ��� ��Ŀ��
	TALENT_FOCUS_TYPE		m_nRequireFocus;
	// �ŷ�Ʈ�� ���������� ���Ŀ� ��� �Ǵ� ��Ŀ��
	TALENT_FOCUS_TYPE		m_nNextFocus;


	bool					m_bValidation;			///< ResourceValidator ��ȿ���˻縦 ����, ���� ���� - �⺻�� true

	CSTalentActAnimationTime m_ActAnimationTime;

	bool					m_bByLicense;		// ������ ������� �н��ϴ� �ŷ�Ʈ
	bool					m_bUsableAtDead;		// ���� ���¿��� ���� �ִ� �ŷ�Ʈ

// CLIENT�� ��� -------------
public:
	tstring					m_szDesc;				///< ����(Ŭ��)
	float					m_fExtraMotionTime;		///< Extra ����� �÷��� �ð� - TOOL���� ���
	float					m_fExtraMotionTime2;	///< Extra2 ����� �÷��� �ð� - TOOL���� ���
	float					m_fExtraMotionTime3;	///< Extra3 ����� �÷��� �ð� - TOOL���� ���
//	float					m_fHitMotionDelay;		///< ��Ʈ ����� ������ - ��� ����
	bool					m_bAffectedMotionSpd;	///< ��� �ӵ��� ����޴� Ż��Ʈ���� ����

	TARGETING_TYPE			m_eTargetingType;		///< Ÿ���� Ÿ��
// CLIENT�� ��� -------------^

// SERVER�� ��� -------------
public:
	TALENT_GUARD_DIRECTION		m_nGuardDirection;		///< ��� ��ų�� ��� ��� ������ ����

	struct _TALENT_MOVE_INFO
	{
		vec3	vPos;
		float	fRot;
		_TALENT_MOVE_INFO() : vPos(vec3::ZERO), fRot(0.0f) {}
		_TALENT_MOVE_INFO(vec3 pos, float rot) : vPos(pos), fRot(rot) {}
	};

	vector<_TALENT_MOVE_INFO>	m_MovingInfoList;

	vec3					m_vCastingPos;			// Casting �ŷ�Ʈ ���� ��ġ
	float					m_fCastingRot;			// Casting �ŷ�Ʈ ���� ����
	vec3					m_vCastingLoopPos;		// Casting Loop �ŷ�Ʈ ���� ��ġ
	float					m_fCastingLoopRot;		// Casting Loop �ŷ�Ʈ ���� ����
	vec3					m_vExtraPos;			// Extra Ż��Ʈ�� ���� ��ġ
	float					m_fExtraRot;			// Extra Ż��Ʈ�� ���� ����
	vec3					m_vExtra2Pos;			// Extra2 Ż��Ʈ�� ���� ��ġ
	float					m_fExtra2Rot;			// Extra2 Ż��Ʈ�� ���� ����
	vec3					m_vExtra3Pos;			// Extra3 Ż��Ʈ�� ���� ��ġ
	float					m_fExtra3Rot;			// Extra3 Ż��Ʈ�� ���� ����

	// ���� ���� ���� -----------------
	GTalentDrain			m_Drain;
	bool					m_bCriticalEnable;
	float					m_fWeaponApplyRate;		// ���� ����� ���� (1 = 100%)
	WEAPON_REFRENCE			m_WeaponReference;		// ��� ���Ŀ��� ���� �Ǵ� ����

	float					m_fCriticalApplyRate;	// �߰� ġ�� ���� (1 = 100%)

	// ���� ���� ----------------------
	GTalentHitFilter		m_HitFilter;			// ���� ����
	MF_STATE				m_nForceMF;				// ���� ��� ���� - ������ NONE�̴�.
	short					m_nForceMFWeight;		// ���� ��� ���� Weight


	// AI -----------------------------
	float					m_fExtraPhaseTime;		///< extra phase �� �ð�
	float					m_fExtraPhaseTime2;		///< extra phase �� �ð�
	float					m_fExtraPhaseTime3;		///< extra phase �� �ð�

	// -----------------------------
	bool					m_bUseInvincibleTime;
	float					m_fInvincibleTimeStart;	// ���� ���� �ð�
	float					m_fInvincibleTimeEnd;	// ���� ���� �ð�
	bool					m_bUseSuperarmorTime;	// ���۾Ƹ�(������� �Ȱɸ�) ��� ����
	float					m_fSuperarmorTimeStart;	// ���۾Ƹ�(������� �Ȱɸ�) ���� �ð�
	float					m_fSuperarmorTimeEnd;	// ���۾Ƹ�(������� �Ȱɸ�) ���� �ð�
	bool					m_bSeparateHit;			///< ���� ���� ���� - �⺻�� �ش� �ŷ�Ʈ�� �ѹ� �����Ǹ� �������� �� ���� true�̸� ������ ĸ������ �����Ѵ�.
	bool					m_bHitCapsulePosSync;	///< ���� ĸ�� ��ġ�� �����ϰ� �ִ� ��ƼƼ�� ��ġ�� ������ ����
	float					m_fPostDelay;			// �ŷ�Ʈ ���� ������

	bool					m_bUseAvoidTime;
	float					m_fAvoidTimeStart;	// ȸ�� ���� �ð�
	float					m_fAvoidTimeEnd;	// ȸ�� ���� �ð�

	int						m_nCooltimeLine;	// ��Ÿ�� ���� (���)

	GUARD_CRASH_LEVEL		m_nGuardCrashLevel;	// ���� ũ���� ���� (���带 ���� ����)

	// Script -------------------------
	tstring					m_strLuaOnStart;
	tstring					m_strLuaOnAct;
	tstring					m_strLuaOnDelayedAct;
	tstring					m_strLuaOnFinish;
	tstring					m_strLuaOnCancel;
	


	bool IsToggle();
	int GetToggleBuffID();

// SERVER�� ��� -------------^

	CSTalentInfo();
	virtual ~CSTalentInfo()
	{
		m_MovingInfoList.clear();
	}
	virtual void Cooking();
	virtual void CopyAll(CSTalentInfo* pTalentInfo) {};		// ��ӹ��� Ŭ������ �˾Ƽ� ó���ϱ�

	bool IsPassiveSkill() const;
	bool IsNeedTarget() const;
	bool IsMagicDamage() const;
	bool HasRequireFocus();
	bool HasNextFocus();
	bool HasDamage();
	bool HasHealEffect();
	bool HasMotionfactor();
	bool ExistMode() { return m_Modes.m_bExistMode; }

	CSTalentInfo* GetMode(int nMode);
	void InsertMode(CSTalentInfo* pModeTalentInfo);
	bool IsCooked() const { return m_bCooked; }

	bool IsPhysicalAttack(void) const;
	bool IsSelfRebirth() const;
};