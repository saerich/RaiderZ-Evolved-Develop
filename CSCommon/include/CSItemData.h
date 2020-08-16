#pragma once

#include <string>
#include "MTstring.h"
#include <vector>
#include "CSSoul.h"
#include "CSCommonLib.h"
#include "CSModifier.h"

#include "MTypes.h"

using namespace std;


/// DB���� ������ �������� �з��ϱ� ���ؼ� ���ȴ�.
const enum SH_ITEM_SLOT_TYPE
{
	SLOTTYPE_NONE			= -1,
	
	SLOTTYPE_DELETED		= 0,
	SLOTTYPE_INVENTORY		= 1,
	SLOTTYPE_EQUIP			= 2,
	SLOTTYPE_STORAGE		= 3,
	SLOTTYPE_GUILDSTORAGE	= 4,

	SLOTTYPE_MAIL			= 101,	// ���� ����.		
};

/// ĳ���� ������ ����
const enum SH_ITEM_SLOT 
{
	ITEMSLOT_NONE = 255,		// �����Ұ����� ������. 255���� �������� ����� �����.
	ITEMSLOT_ALL = ITEMSLOT_NONE,

	// DEFAULT =============================
	ITEMSLOT_ARMOR_START = 0,
	ITEMSLOT_HEAD = ITEMSLOT_ARMOR_START,	//����.
	ITEMSLOT_FACE,	//���۶󽺵�.
	ITEMSLOT_HANDS,
	ITEMSLOT_FEET,
	ITEMSLOT_BODY,
	ITEMSLOT_LEG,
	ITEMSLOT_ARMOR_END,
	
	ITEMSLOT_LFINGER = ITEMSLOT_ARMOR_END,
	ITEMSLOT_RFINGER,
	ITEMSLOT_NECK,
	ITEMSLOT_CHARM,
	ITEMSLOT_LWEAPON,
	ITEMSLOT_RWEAPON,
	ITEMSLOT_LWEAPON2,
	ITEMSLOT_RWEAPON2,
	ITEMSLOT_DEFAULT_BEGIN = ITEMSLOT_HEAD,
	ITEMSLOT_DEFAULT_MAX = ITEMSLOT_RWEAPON2,
	// DEFAULT ============================^

	// LOOK ================================
	ITEMSLOT_LOOK_HEAD,
	ITEMSLOT_LOOK_HANDS,
	ITEMSLOT_LOOK_FEET,
	ITEMSLOT_LOOK_BODY,
	ITEMSLOT_LOOK_LEG,
	ITEMSLOT_LOOK_BEGIN = ITEMSLOT_LOOK_HEAD,
	ITEMSLOT_LOOK_MAX = ITEMSLOT_LOOK_LEG,
	// LOOK ===============================^

	ITEMSLOT_INN_CRAFTING,
	ITEMSLOT_INN_BED,
	ITEMSLOT_INN_STEWARD,
	ITEMSLOT_INN_CARPET,
	ITEMSLOT_INN_FIREPLACE,
	ITEMSLOT_INN_MURAL,
	ITEMSLOT_INN_TABLE,
	ITEMSLOT_INN_FUNITURE,	
	
	ITEMSLOT_MAX
};

const enum SH_ITEM_SWITCH_WEAPON
{
	ITEMSWITCH_PRIMARY = 0,
	ITEMSWITCH_SECONDARY,
};


static const TCHAR* CSItemSlotStr[] =		// ���Ƿ� ������, xml�� �̰ɷ� ���.
{
	_T("head"),
	_T("face"),
	_T("hands"),
	_T("feet"),
	_T("body"),
	_T("leg"),
	_T("lfinger"),
	_T("rfinger"),
	_T("neck"),
	_T("charm"),

	_T("lweapon"),
	_T("rweapon"),
	_T("lweapon2"),
	_T("rweapon2"),

	_T("look_head"),
	_T("look_hands"),
	_T("look_feet"),
	_T("look_body"),
	_T("look_leg"),

	_T("inn_crafting"),
	_T("inn_bed"),
	_T("inn_steward"),
	_T("inn_carpet"),
	_T("inn_fireplace"),
	_T("inn_mural"),
	_T("inn_table"),
	_T("inn_funiture"),

	_T("none")
};


const enum SOUND_TYPE
{
	SOUND_TYPE_UNIVERSAL = 0,// 
	SOUND_TYPE_CLOTH,		// 
	SOUND_TYPE_LEATHER,		// 
	SOUND_TYPE_CHAIN,		// 
	SOUND_TYPE_IRON,		// 
	SOUND_TYPE_WOOD,		// 
	SOUND_TYPE_CUTLERY,		// 
	SOUND_TYPE_PAPER,		// 
	SOUND_TYPE_STONE,		// 
	SOUND_TYPE_LIQUID,		// 
	SOUND_TYPE_JEWEL,		// 
	SOUND_TYPE_ORGAN,		// 
	SOUND_TYPE_HAMMER,		// 
	SOUND_TYPE_COIN,		// 

	SOUND_TYPE_MAX
};

/// ������ Ÿ��
const enum SH_ITEM_TYPE
{
	ITEMTYPE_NONE = 0,

	ITEMTYPE_WEAPON,			// ����
	ITEMTYPE_ARMOR,				// �� �� �Ǽ��縮
	ITEMTYPE_HOUSING,			// �Ͽ�¡

	ITEMTYPE_USABLE,			// ��뼺
	
	ITEMTYPE_AMMUNITION,		// ��´ϼ�
	ITEMTYPE_INVENTORY,			// ������ �ʰ� �ڸ��� �����ϴ� ������	

	ITEMTYPE_GATHER,			// ä������

	ITEMTYPE_ENCHANTSTONE,		// ��ȭ
	ITEMTYPE_DYE,				// ����

	ITEMTYPE_MAX
};

/// ������ ���
enum ITEM_TIER
{
	ITEM_TIER_VERY_COMMON = 0,		// �ſ� ����
	ITEM_TIER_COMMON,				// ����
	ITEM_TIER_RARE,					// ���
	ITEM_TIER_TREASURE,				// ����
	ITEM_TIER_LEGENDARY,			// ����
	ITEM_TIER_EPIC,					// ����
	ITEM_TIER_MAX
};

/// ���� Ÿ��
enum WEAPON_TYPE
{
	WEAPON_NONE			= 0,
	WEAPON_1H_SLASH		= 1,	// ���� �Ӽ��� �Ѽ� ����
	WEAPON_1H_BLUNT		= 2,	// �ļ� �Ӽ��� �Ѽ� ����
	WEAPON_1H_PIERCE	= 3,	// ��� �Ӽ��� �Ѽ� ����
	WEAPON_TWO_HANDED	= 4,	// ��� ��
	WEAPON_STAFF		= 5,	// ������
	WEAPON_ARCHERY		= 6,	// Ȱ
	WEAPON_2H_BLUNT		= 7,	// ��� �б�

	WEAPON_DUAL_WIELD	= 8,	// ���Ⱑ �ٸ� ���	
	WEAPON_DUAL_PIERCE	= 9,	// ��� �ܰ�
	
	WEAPON_MAX,
	WEAPON_ALL,
};

// ���⺰ �̵��ӵ�
static const int MoveSpeedByWeapon[WEAPON_MAX] = 
{
	375,
	375,
	375,
	375,
	300,
	350,
	350,
	300,
	450,
	375,
};

/// �� Ÿ��
enum ARMOR_TYPE
{
	ARMOR_NONE		= 0,
	ARMOR_CLOTH		= 1,	// õ��
	ARMOR_LIGHT		= 2,	// ���� ����
	ARMOR_MEDIUM	= 3,	// ü�� ����
	ARMOR_HEAVY		= 4,	// �Ǳ� ����
	ARMOR_SHIELD	= 5,	// ����
	ARMOR_ACCESSORY	= 6,	// �Ǽ��縮
	ARMOR_AMULET	= 7,	// �ֹķ�(��ȥ ���)

	ARMOR_MAX
};

/// ��� Ÿ��
enum USABLE_TYPE
{
	USABLE_NONE				= 0,
	USABLE_TALENT_USE		= 1,	// �ŷ�Ʈ ���
	USABLE_TALENT_TRAIN		= 2,	// �ŷ�Ʈ �н�
	USABLE_TALENT_UNTRAIN	= 3,	// �ŷ�Ʈ �ʱ�ȭ
	USABLE_INTERACTION		= 4,	// NPC�� ���ͷ���
	USABLE_INTERACTION_DEAD	= 5,	// ���� NPC�� ���ͷ���
	USABLE_QUEST_ADD		= 6,	// ����Ʈ �߰�	

	USABLE_MAX
};

/// �������� ���� ������ ����
enum SH_FEATURE_ITEMSLOT
{
	FEATURE_ITEMSLOT_HEAD = 0,
	FEATURE_ITEMSLOT_FACE,
	FEATURE_ITEMSLOT_HANDS,
	FEATURE_ITEMSLOT_FEET,
	FEATURE_ITEMSLOT_BODY,
	FEATURE_ITEMSLOT_LEG,
	FEATURE_ITEMSLOT_LWEAPON,
	FEATURE_ITEMSLOT_RWEAPON,
	FEATURE_ITEMSLOT_LWEAPON2,
	FEATURE_ITEMSLOT_RWEAPON2,
	FEATURE_ITEMSLOT_MAX,
	FEATURE_ITEMSLOT_NONE = FEATURE_ITEMSLOT_MAX,
};

enum ENCHANT_COLOR
{
	ENCHANT_NONE = 0,
	ENCHANT_RED,
	ENCHANT_ORANGE,
	ENCHANT_BLUE,
	ENCHANT_GREEN,
	ENCHANT_WHITE,
	ENCHANT_YELLOW,
};

static const TCHAR* ENCHANT_COLOR_STR[] =		// ���Ƿ� ������, xml�� �̰ɷ� ���.
{
	_T("X"),
	_T("R"),
	_T("O"),
	_T("B"),
	_T("G"),
	_T("W"),
	_T("Y"),
	_T("S"), // MrFarbodD - Added this to prevent error
	_T("N"), // MrFarbodD - Added this to prevent error.
};



struct CSItemMaterial
{
	int				nID;		///< ���� Mtrl ������ Mesh �ε���
	int				nSubCount;	///< Multi/Sub Mtrl�� ����. Standard �̸� 1��.
	vector<string>	vecSubMtrls;///< ���� ������ RMaterial�� �ε���.
};

struct ITEM_SLOT_INFO
{
	SH_ITEM_SLOT_TYPE nSlotType;
	int nSlotID;

	ITEM_SLOT_INFO()
	: nSlotType(SLOTTYPE_NONE)
	, nSlotID(0)
	{}

	ITEM_SLOT_INFO(SH_ITEM_SLOT_TYPE nSlotType, int nSlotID)
	: nSlotType(nSlotType)
	, nSlotID(nSlotID)
	{}
};

enum DAMAGE_ATTRIB;
typedef vector<CSItemMaterial*>		CSItemMaterialVector;

/// ��� ������ �ν��Ͻ��� ������ �� �ִ� �Ķ����.
class CSCOMMON_API CSItemData
{
public:
		int				m_nID;				///< �������� ������!!! ID
		tstring			m_strName;			///< �������� �̸� - ���ڿ����̺� Ű����($$ITEM_NAME_#)
		tstring			m_strDesc;			///< ������ ���� - ���ڿ����̺� Ű����($$ITEM_DESC_#)
		ITEM_TIER		m_nTier;			///< ������ ���

		SH_ITEM_TYPE	m_ItemType;			///< �������� ����
		SH_ITEM_SLOT	m_nItemSlot;		///< �������� ��� �����Ǵ���
		SH_ITEM_SLOT	m_nSubSlot;			///< �߰��� ����ϴ� ����

		WEAPON_TYPE		m_nWeaponType;		///< ���� Ÿ��
		DAMAGE_ATTRIB	m_nDamageAttrib;	///< ������Ÿ��
		bool			m_bLeftWeaponEquipable;	///< ������ ��� �޼տ��� ��� �������� ����
		
		SOUND_TYPE		m_ItemSoundType;	///< ������ ���� ����
		ARMOR_TYPE		m_nArmorType;		///< �� Ÿ��

		USABLE_TYPE		m_nUsableType;		///< ��� Ÿ��
		vector<int>		m_vecUsableParam;	///< ��� Ÿ���� ����ϴ� �Ķ����
		
		bool			m_bUsableConsume;	///< ��� Ÿ���� ����� �Ҹ�Ǵ��� ����

		GATHER_TYPE		m_nGatherType;		///< ä�� ����

		int				m_nHousingNPC;		///< �Ͽ�¡ �������� �����ϸ� �濡 ����� NPC

		int				m_nStackAmount;		///< �� �������� ����� ��ĥ �� �ִ��� - �⺻���� 1

		int				m_nMaxDurability;	///< �ִ볻��

		int				m_nBaseBuyingPrice;		///< �÷��̾ �������� ��� �⺻ ����
		int				m_nBaseSellingPrice;	///< �÷��̾ ������ �Ĵ� �⺻ ����

		int				m_nMinDamage;		///< �ּ� ������
		int				m_nMaxDamage;		///< �ִ� �����
		int				m_nAP;				///< ����		

		bool			m_bTradable;		///< �ŷ� ���� ����
		bool			m_bSellable;		///< �Ǹ� ���� ����
		bool			m_bDiscardable;		///< ������ ���� ����		
		bool			m_bClaimRequired;	///< ����� �ͼ� �Ǵ���?
		bool			m_bUnique;			///< �ϳ��� ���� ��������?
		bool			m_bQuestRelated;	///< ����Ʈ ���� ����������?
		bool			m_bRecipeMaterial;	///< ������ �������?
		float			m_fCraftOptionRate;	///< ���� �ΰ� Ȯ��

		int				m_nRepairGrade;		///< ���� ���̵�
		int				m_bRepairable;		///< ���� ���� ����

		SOUL_TYPE		m_nSoulType;		///< ��ȥ ����
		uint8			m_nSoulMaxQuantity;	///< ��ȥ �ִ� �����
		int				m_nSoulLinkedItem;	///< ��ȥ�� �ִ�� ��������� ����Ǵ� ��ȥ�� ItemID

		tstring			m_strMeshName;		///< ������ �޽� �̸�, �� ���ǰ�� �ƴϴ��� �ٴڿ� �����ٴϰų� �Ҷ� �ʿ�? ...���� ������?
		bool			m_bMeshNameBySex;	///< ������ �ٸ� ������ �޽� �̸����� ����, ex) meshname= "01" �̸� female_01, male_01 �� ���� ��ü
		tstring			m_strParentName;	///< �������� ���� ��� �̸�
		tstring			m_strParentNameLeft;///< �޼տ� ���� ��� �������� ���� ��� �̸�(����)

		vector<tstring>	m_strNodeNames;		///< �������� ��� �̸�
		tstring			m_strMeshPath;		///< �޽� ���

		unsigned int	m_nTexColor;		///< �ؽ��� �÷�
		bool			m_bUseTexColor;		///< �ؽ��� �÷��� ����ϴ°�?
		tstring			m_strSwordTrailEffect; ///< �˱� ����Ʈ(���⿡�� ���Դϴ�.)

		float			m_fSpellPower;

		int				m_nConditionsInfoID;	///< ��� or ��� ����
		vector<int>		m_vecDyeColor;			///< ������ ����
		bool			m_bDyeable;				///< ���� ���� ����

		// 2008.03.17 by jun
		CSItemMaterialVector	m_vecMtrls;		///< Sub Material

		bool			m_bValidation;			///< ResourceValidator ��ȿ���˻縦 ����, ���� ���� - �⺻�� true
		tstring			m_strIconName;

		CSPlayerModifier	m_PlayerModifier;	///< Item �⺻ ����
		CSActorModifier		m_ActorModifier;	///< Item �⺻ ����		

		struct ENCHANT_STONE
		{
			int nLinkedBuffID; // ����� ����
			float fSuccessRate; // ���� Ȯ��
			int nLimitLevel;
			bool bAllowAllEquipments;
			vector<SH_ITEM_SLOT>	vecLimitEquipments;
			bool bAllowAllWeapons;
			vector<WEAPON_TYPE>		vecLimitWeapons;
			ENCHANT_COLOR nColor;
			int	nTier;

			ENCHANT_STONE(): nLinkedBuffID(INVALID_BUFF_ID), fSuccessRate(100.0f), nLimitLevel(0), bAllowAllEquipments(true), bAllowAllWeapons(true), nColor(ENCHANT_NONE), nTier(0) {}
		} m_EnchantStone;

		vector<ENCHANT_COLOR> m_vecEquipEnchantColor;
		
public:		
		CSItemData();
		virtual ~CSItemData();
		virtual void Cooking();

		bool IsEnchantStone() const;

		tstring GetMainNodeName() const
		{
			if (m_strNodeNames.empty()) return tstring(_T(""));
			return m_strNodeNames[0];
		}

		void CookNodeNames();
};





#define EQUIPMENT_ITEM_NODE_NAME_ADDITIVE_SUFFIX			_T("_item")