#ifndef _CS_CHRINFO_H
#define _CS_CHRINFO_H

#include "CSCommonLib.h"
#include "CSDef.h"
#include <string>
#include "MTstring.h"
#include <map>
#include <set>
#include <deque>
#include "MUID.h"
using namespace std;

#include "CSItemData.h"
#include "CSModifier.h"
#include "MTypes.h"
#include "CSTalentInfoEnum.h"
#include "CSTypes.h"

/// ����
enum RACE
{
	RACE_NA		= -1,	///< ��������
	RACE_HUMAN,			///< �ΰ�
	RACE_VOLO,			///< ����
	RACE_NEBIT,			///< �׺�
	RACE_MAX
};

/// ����
enum CLASS
{
	ADVENTURER = 0,			// �⺻ Ŭ����
	WARRIOR,				// �ٰŸ� ������ ������
	HUNTER,					// ȭ��� ��ǥ�� �븰��
	MAGE,					// ������ ���� �ٷ�� ��
	DEFENDER,				// ������ ü���� ������
	FENCER,					// ���� ������ ����
	RANGER,					// ���Ÿ� ������ ����
	ASSASSIN,				// �������� ������ �ѹ�
	SORCERER,				// ���Ұ� ������ ������
	GUARDIAN,				// �ż��� ���� ������ ������
};

/// ����
enum SEX
{
	SEX_NA		= -1,	///< ���� ����
	SEX_MALE,			///< ����	
	SEX_FEMALE,			///< ����
	SEX_SEXLESS,		///< ����
	SEX_MAX
};

/// ��ƼƼ Ÿ��
enum ENTITY_TYPE
{
	// ���� Ŭ���̾�Ʈ ����
	ET_ENTITY		= 0x1,
	ET_SYNC			= 0x2,
	ET_ACTOR		= 0x4,
	ET_PC			= 0x10,		///< �÷��̾�
	ET_NPC			= 0x20,		///< NPC - NPC, ���� �� ��� NPC
	ET_ITEM			= 0x40,		///< ������
	ET_PROJECTILE	= 0x80,		///< �߻�ü
	ET_BUFF_ENTITY	= 0x100,	///< ���ӽð��ִ� ��������
	ET_ENTITY_BPART	= 0x200,	///< breakable part
	// ������ ���

	// Ŭ���̾�Ʈ�� ���
	ET_EFFECT		= 0x100,
};

enum ENTITY_TYPEID
{
	ETID_ENTITY		= 0,
	ETID_SYNC		= 1,
	ETID_ACTOR		= 2,
	ETID_PLAYER		= 3,
	ETID_NPC		= 4,
	ETID_ITEM		= 5,
	ETID_PROJECTILE	= 6,
	ETID_BUFF_ENTITY= 7,
	ETID_CUTSCENE	= 8,

	ETID_MAX
};

// ĳ���� ���Ľ�
enum CHAR_STANCE
{
	CS_NORMAL = 0,
	CS_BATTLE = 1
};

enum CHAR_DOING
{
	CD_NORMAL = 0,
	CD_INTERACTING,					///< ���ͷ���
	CD_TRADING,						///< ��ȯ
	CD_LOOTING,						///< ����
	CD_ENCHANTING,					///< ��ȭ
	CD_DYEING,						///< ����
	CD_CUTSCENING,					///< �ƾ�
};

enum ALTITUDE_TYPE
{
	AT_NORMAL=0,			// ����
	AT_FLOATING,			// ������ �پ������� ���ִ�(����)
	AT_SKY,					// �ϴ� ��
	AT_UNDERGROUND,			// ����

	AT_SIZE
};

#define PLAYER_DEFAULT_HPMAX		100
#define PLAYER_DEFAULT_ENMAX		100
#define PLAYER_DEFAULT_SPMAX		100
#define PLAYER_DEFAULT_SPEED		100.0f

#define PLAYER_DEFAULT_FR			0
#define PLAYER_DEFAULT_CR			0
#define PLAYER_DEFAULT_PR			0
#define PLAYER_DEFAULT_LR			0
#define PLAYER_DEFAULT_HR			0
#define PLAYER_DEFAULT_UR			0

#define PLAYER_DEFAULT_STR			50
#define PLAYER_DEFAULT_DEX			50
#define PLAYER_DEFAULT_INT			50
#define PLAYER_DEFAULT_CHA			50
#define PLAYER_DEFAULT_CON			50

#define BASE_HP_RACE_HUMAN		(50)		// HP race - �ΰ� �⺻ ü��
#define BASE_EN_RACE_HUMAN		(50)		// EN race - �ΰ� �⺻ ������
#define BASE_SP_RACE_HUMAN		(50)		// SP race - �ΰ� �⺻ ���¹̳�

/// ĳ������ ����ġ
struct CSActorResistances
{
	uint8	nValue[TRT_MAX];	// TRT_CHA�� ������� ����.

	CSActorResistances();
};

struct CSActorMFModifier
{
	PerMulVar<short>	Weight[MF_PRIORITY_SIZE];
	PerMulVar<short>	Value[MF_PRIORITY_SIZE];
};

/// ĳ����(PC,NPC) ���� ����
class CSCOMMON_API CHR_INFO
{
public:
	int		nLevel;			///< ����(1~60)

	// Attributes
	int		nMaxHP;			///< Max HP
	int		nMaxEN;			///< Max EN
	int		nMaxSTA;		///< Max STA
	int		nAP;			///< �Ƹ� ����Ʈ
	int		nABS;			///< ���� ������ �����
	int		nHPRegen;		///< �ʴ� HP �ڿ� ȸ����
	int		nENRegen;		///< �ʴ� EN �ڿ� ȸ����
	int		nSTARegen;		///< �ʴ� STA �ڿ� ȸ����
	float	fSpeed;			///< �̵� �ӵ�
	
	CSActorMFModifier		MFModifier;
	CSActorResistances	Resist;	

	bool	bRooted;		///< ������ �� ����.

	ALTITUDE_TYPE		nAltitude;

	CHR_INFO();
};

struct CSCOMMON_API CHR_STATUS
{
public:
	int nHP;
	int nEN;
	int nSTA;
	CSInstantModifier	InstantModifier;
	CSActorModifier		ActorModifier;

	CHR_STATUS();
};

/// �̵� �÷��� - 2byte�� ����ϹǷ� �÷��װ� 2����Ʈ�� ������ �ȵȴ�.
enum MOVEMENT_FLAG
{
	MOVEMENTFLAG_FORWARD		= 0x1,
	MOVEMENTFLAG_BACKWARD		= 0x2,
	MOVEMENTFLAG_LEFT			= 0x4,
	MOVEMENTFLAG_RIGHT			= 0x8,
	MOVEMENTFLAG_JUMPING		= 0x10,
	MOVEMENTFLAG_FALLING		= 0x20,
	MOVEMENTFLAG_USING_TALENT	= 0x40,
	MOVEMENTFLAG_MOTION_FACTOR	= 0x80,
	MOVEMENTFLAG_GUARD			= 0x100,
};

/////////////////////////////////////////////////////////////////////////////////////
// PC ///////////////////////////////////////////////////////////////////////////////

/// �÷��̾� ���
enum GPlayerGrade
{
	PLAYER_GRADE_NORMAL			= 0,		///< �Ϲ����� �÷��̾�
	PLAYER_GRADE_GM				= 1,		///< GM
	PLAYER_GRADE_TESTER			= 2,		///< �׽���
	PLAYER_GRADE_DEVELOPER		= 3			///< ������
};

// �÷��̾� ����
class CSCOMMON_API PLAYER_INFO : public CHR_INFO
{
private:
	bool	m_bGod;					///< ���ظ� ���� ����
	int		m_nMoney;				///< ������ ��
	int		m_nStorageMoney;		///< �����Կ� ������ ��

public:
	bool	m_bGhost;					///< �ٸ� �÷��̾�� �Ⱥ����� ����

	int				nCID;				///< CID, DB���� ���
	GPlayerGrade	nPlayerGrade;		///< �÷��̾� ���
	
	RACE	nRace;				///< ����
	SEX		nSex;				///< ����
	int8	nFeatureHair;		///< �Ӹ�
	int8	nFeatureFace;		///< ��
	short	nFeatureHairColor;	///< �Ӹ���
	short	nFeatureSkinColor;	///< �Ǻλ�
	uint8	nEyeColor;			///< ����
	uint8	nMakeUp;			///< ȭ��
	uint8	nTattooType;			///< ����
	short	nTattooPosX;			///< ���� ��ǥ x
	short	nTattooPosY;			///< ���� ��ǥ y
	uint8	nTattooScale;		///< ���� ũ��

	TCHAR	szName[PLAYER_NAME_LEN+1];				///< �̸�
	TCHAR	szSurname[PLAYER_SUR_NAME_LEN+1];		///< ��
	int		nXP;					///< ���� ����ġ

	SBID	nSoulBindingID;			///< �ҿ� ���ε� ��ġ ID
	
	// Attributes
	int		nSTR;					///< �Ϸ�
	int		nDEX;					///< ���� ���� ���ǵ� ���� �� Ȯ���� ȸ��
	int		nINT;					///< ����
	int		nCHA;					///< ī������?
	int		nCON;					///< HP�� �ִ뷮�� ����
	
	int		nGrab;					///< ��� �ɷ�ġ
	int		nBodyRun;				///< ���� ������ �� �� �ִ� �ɷ�ġ
	float	fMerchandize;			///< ������ ��� �ȶ��� ����, ������
	short	nFatigueQuantity;		///< �Ƿε�
	uint8	nInnRoomID;				///< ����� ������
	int		nGID;					///< ��� ID

	int		nBlankTimeSec;			///< �α׾ƿ� ���� �ٽ� �α����� �ð������� ��� �ð�(�ʴ���).
	int		nPlayTimeSec;			///< ������ �����ؼ� ������ ������ �ð�(�ʴ���).
	int		nConnPlayTimeSec;		///< ���� ���ӽ� �÷��� �ð�. nPlayTimeSec - nConnnPlayTimeSec�� ���������� ��� �ð��� ����.
	int		nDeadTime;				///< ���������� ���� �ð�(�ʴ���).
	int		nElapsedDeadTimeSec;	///< ���������� ���� �ð� ���ķ� ���� �ð�

	int		nLootGainMoney;		///< ������ ���ݱ��� �������� ȹ���� ���

	// Item
	SH_ITEM_SWITCH_WEAPON	m_nWeaponSet;

	// GM
	void SetGod(bool god) { m_bGod = god; }
	bool IsGod() { return m_bGod; }

	void SetMoney(int nMoney);
	int GetMoney();
	void SetStorageMoney(int nMoney);
	int GetStorageMoney();

	int GetDeltaPlayTime();

	float CalcAutoRebirthRemainTime();

	PLAYER_INFO();
};

/////////////////////////////////////////////////////////////////////////////////////
// NPC //////////////////////////////////////////////////////////////////////////////

enum NPC_TYPE
{
	NPC_TYPE_MONSTER = 0,					// ����
	NPC_TYPE_NPC,						// NPC
	NPC_TYPE_OBJECT,					// �� ������Ʈ
	NPC_TYPE_GATHER,					// ä��
	NPC_TYPE_MAX
};

static const TCHAR* NPC_TYPE_STR[NPC_TYPE_MAX] = 
{
	_T("monster"),
	_T("npc"),
	_T("object"),
	_T("gather")
};	

enum NPC_MODE
{
	NPC_MODE_DEFAULT	= 0,
	NPC_MODE_1			= 1,
	NPC_MODE_2			= 2,
	NPC_MODE_3			= 3,
	NPC_MODE_4			= 4,
	NPC_MODE_5			= 5,
	NPC_MODE_6			= 6,
	NPC_MODE_7			= 7,
	NPC_MODE_8			= 8,
	NPC_MODE_9			= 9,
	NPC_MODE_10			= 10,

	NPC_MODE_MAX
};

enum NPC_ATTACKABLE_TYPE
{
	NAT_INVALID			= -1,
	NAT_NONE			= 0,
	NAT_ALWAYS			= 1,
	NAT_FACTION			= 2,

	NAT_MAX
};

enum NPC_RARITY
{
	NPC_RARITY_COMMON = 0,				// �Ϲ�
	NPC_RARITY_RARE,
	NPC_RARITY_EPIC,

	NPC_RARITY_MAX
};


#define DEFAULT_NPC_GRADE		3
#define PLAYER_GRADE			3

#define MIN_NPC_GRADE			0
#define MAX_NPC_GRADE			10

// �� ������ ������ ��꿡 �ʿ��� ����� �Ӽ�
enum ARMOR_ATTRIB
{
	ARMOR_ATT_NEUTRAL = 0,		// �÷��̾�� ������ Neutral�̴�.
	ARMOR_ATT_DRAGON1,
	ARMOR_ATT_DRAGON2,
	ARMOR_ATT_FIRE,
	ARMOR_ATT_WATER,
	ARMOR_ATT_LIGHTNING,
	ARMOR_ATT_POISON,
	ARMOR_ATT_HOLY,
	ARMOR_ATT_UNHOLY,
	ARMOR_ATT_IRON,
	ARMOR_ATT_STONE,
	ARMOR_ATT_WOOD,
	ARMOR_ATT_ANIMAL,
	ARMOR_ATT_UNDEAD,
	ARMOR_ATT_BUG,
	ARMOR_ATT_MAX
};

static const TCHAR* ARMOR_ATTRIB_STR[ARMOR_ATT_MAX] = 
{
	_T("neutral"),
	_T("dragon1"),
	_T("dragon2"),
	_T("fire"),
	_T("water"),
	_T("lightning"),
	_T("poison"),
	_T("holy"),
	_T("unholy"),
	_T("iron"),
	_T("stone"),
	_T("wood"),
	_T("animal"),
	_T("undead"),
	_T("bug")
};	

struct NPCSoundInfo
{
	tstring			HitSound1;
	tstring			HitSound2;
	tstring			HitSound3;
	tstring			DyingSound;
	tstring			EntitySound;
	tstring			CombatBGM;
	tstring			Aggro;

	void Clear();
};

enum NPC_AA_TYPE
{
	NAAT_INVALID = -1,
	NAAT_NONE = 0,
	NAAT_ALWAYS,
	NAAT_FACTION,

	NAAT_MAX
};

static const TCHAR* NPC_AA_TYPE_STRING[NAAT_MAX] = 
{
	_T("AA_NONE"),
	_T("AA_ALWAYS"),
	_T("AA_FACTION"),
};	

/// NPC ����
class CSCOMMON_API CSNPCInfo : public CHR_INFO
{
public:
	int				nID;				///< NPC ������ ID
	NPC_TYPE		nNpcType;			///< NPC Ÿ��
	tstring			strName;			///< �̸�
	tstring			strNameInLua;		///< Lua���� �� �̸�
	tstring			strSubtitle;		///< 
	tstring			strMeshPath;		///< �� ����
	tstring			strMeshName;		///< �޽� �̸�
	tstring			strBloodEffectName; ///< ���� ����Ʈ �̸�
	int				nCHA;				///< NPC�� ������ CHA�� ������.

	int				nMaxLevel;			///< NPC�� ������ �� nLevel(Min)~nMaxLevel ������ �������� �������� ���������. 
	int8			nGrade;				///< ���
	bool			bHuge;
	float			fRunSpeed;			///< �޸��� �ӵ�
	float			fFleeSpeedAmp;		///< ����ġ�� �ӵ� ����ġ
	float			fRotateSpeed;		///< ȸ�� �ӵ� 0�̸� ȸ���ϴ� ������ ���� ����, ������ ����
	float			fScale;				///< �޽� ������ ��
	bool			bShowName;			///<
	bool			bInteract;			///< Interactive ���ɿ���
	ARMOR_ATTRIB	nArmorType;			///< ����� �Ӽ�
	GUARD_TYPE		nDefaultGuardType;	///< �⺻ ��� Ÿ�� (�⺻�� �������)

	float			fTitleHeight;		///< �̸� ���� �������� ���̰�
	tstring			strTitleBone;		///< �̸� ���� ���� �̸�
	
	tstring			strAniPrefix;				///< �ִϸ��̼� ���ξ�

	bool			m_bColTestObject;	///< �浹 ����

	uint8			nHitTestIndex;		///< �⺻���� ����� ���� �ε���
	NPC_MODE		nMode;				///< NPC �ʱ� ����ȣ(�Ϲ�, �ϴ�, ���� ��)
	NPC_MODE		nSpawningMode;		///< NPC�� ������ �� ������ Mode
	NPC_AA_TYPE		nAA;				///< Always Agressive on PC

	float			fInteractionLootTime;		///< ���ͷ������� ���� �Ҷ� �ɸ��� �ð�
	int				nInteractionDistance;		///< ���ͷ��� ���� �Ÿ�. �� ���� 0�̸� ���Ŀ� ���ؼ�, 0�� �ƴϸ� �� ������ �Ÿ��� üũ�Ѵ�.

	bool			bValidation;				///< ResourceValidator ��ȿ���˻縦 ����, ���� ���� - �⺻�� true

	NPCSoundInfo	SoundInfo;			///< ���� ����

	int				nEatOwnerBuffID;	///< �������� ��� ���� ���̵�
	int				nEatTargetBuffID;	///< �������� ��� ���� ���̵�
	float			fEatPeriod;			///< �Ա� ȿ���ֱ�ð�

	uint8			m_nFactionLossID;			///< NPC�� �׿����� �Ҵ� �Ѽ��� ����

	GATHER_TYPE		nGatherType;		///< ä������
	bool			bShowEnemyInfo;

	CSNPCInfo();

	void Clear();
};

/// NPC ���� ������ - ����, Ŭ���̾�Ʈ ���ڿ��� ��ӹ޾� ����ؾ��Ѵ�.
class CSCOMMON_API CSNPCInfoMgr : public map<int, CSNPCInfo*>
{
private:

protected:

public:
	CSNPCInfoMgr();
	virtual ~CSNPCInfoMgr();
};


inline CHAR_STANCE NegativeStance(CHAR_STANCE nStance)
{
	if (nStance == CS_NORMAL) return CS_BATTLE;
	return CS_BATTLE;
}

class CSCOMMON_API CSAltitudeHelper 
{
public:
	// Area ������ ������ �� �ִ� ������ ����
	static bool IsAreaHittable(ALTITUDE_TYPE nAltitude);

	// ��� ���� ������ �ϴ��� ����
	static bool IsApplicableToMotionFactor(ALTITUDE_TYPE nAltitude);

	// �浹�� ������ ������ ����
	static bool IsColliable(ALTITUDE_TYPE nAltitude);
};
#endif // _CS_CHRINFO_H
