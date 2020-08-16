/************************************************************************/
/* 1. ĳ���������� ���ϴ�.                                              */
/************************************************************************/

#ifndef _XNPCINFO_H
#define _XNPCINFO_H

#include "CSChrInfo.h"
#include "MXml.h"
#include "CSDef_Interaction.h"
#include "XDef.h"
#include "XPartsSlotType.h"

#define NPC_XML_TAG_MAIET			"maiet"
#define NPC_XML_TAG_NPC				"NPC"
#define NPC_XML_TAG_ITEM			"ITEM"

#define NPC_XML_ATTR_ID				"id"
#define NPC_XML_ATTR_PARENT			"parent"
#define NPC_XML_ATTR_TYPE			"Type"
#define NPC_XML_ATTR_NAME			"Name"
#define NPC_XML_ATTR_MAX_HP			"MaxHP"
#define NPC_XML_ATTR_AP				"AP"
#define NPC_XML_ATTR_MODEL			"Model"
#define NPC_XML_ATTR_HITTEST_INDEX	"HitTestIndex"
#define NPC_XML_ATTR_MESH_NAME		"MeshName"
#define NPC_XML_ATTR_MESH_NODE_NAME "NodeName"
#define NPC_XML_ATTR_MESH_PATH		"MeshPath"
#define NPC_XML_ATTR_GRADE			"Grade"
#define NPC_XML_ATTR_HUGE			"Huge"
#define NPC_XML_ATTR_SCALE			"Scale"
#define NPC_XML_ATTR_ROTATE_SPEED	"RotateSpeed"
#define NPC_XML_ATTR_MODE			"Mode"
#define NPC_XML_ATTR_INTERACT		"Interact"
#define NPC_XML_ATTR_SHOW_NAME		"ShowName"
#define NPC_XML_ATTR_MONSTER		"Monster"
#define NPC_XML_ATTR_ALIGN_TERRAIN	"AlignTerrain"
#define NPC_XML_ATTR_ALIGN_ENEMY	"AlignEnemy"
#define NPC_XML_ATTR_TITLE_BONE		"TitleBone"
#define NPC_XML_ATTR_TITLE_HEIGHT	"TitleHeight"
#define NPC_XML_ATTR_BLOOD_EFFECT_NAME	"BloodEffectName"
#define NPC_XML_ATTR_DYNAMIC_SHADOW		"DynamicShadow"
#define NPC_XML_ATTR_INTERACTIONROTATE	"InteractionRotate"
#define NPC_XML_ATTR_INTERACTION_DISTANCE	"InteractionDistance"
#define NPC_XML_ATTR_ARMOR_TYPE		"ArmorType"
#define NPC_XML_ATTR_AA				"AA"
#define NPC_XML_ATTR_GATHER_TYPE	"GatherType"
#define NPC_XML_ATTR_SHOW_ENEMYINFO	"ShowEnemyInfo"

#define NPC_XML_ATTR_HITSOUND1		"HitSound1"
#define NPC_XML_ATTR_HITSOUND2		"HitSound2"
#define NPC_XML_ATTR_HITSOUND3		"HitSound3"
#define NPC_XML_ATTR_DYINGSOUND		"DyingSound"
#define NPC_XML_ATTR_ENTITYSOUND	"EntitySound"
#define NPC_XML_ATTR_COMBATBGM		"CombatMusic"
#define NPC_XML_ATTR_AGGROSOUND		"AggroSound"

#define NPC_XML_ATTR_MESH_PARTS_PATH	"MeshPartsPath"
#define NPC_XML_ATTR_PARTS_HEAD			"PartsHead"
#define NPC_XML_ATTR_PARTS_HAIR			"PartsHair"
#define NPC_XML_ATTR_PARTS_FACE			"PartsFace"
#define NPC_XML_ATTR_PARTS_CHEST		"PartsChest"
#define NPC_XML_ATTR_PARTS_LEG			"PartsLeg"
#define NPC_XML_ATTR_PARTS_HANDS		"PartsHands"
#define NPC_XML_ATTR_PARTS_FEET			"PartsFeet"
#define NPC_XML_ATTR_PARTS_WEAPON_R		"PartsWeaponR"
#define NPC_XML_ATTR_PARTS_WEAPON_L		"PartsWeaponL"
#define NPC_XML_ATTR_TEX_COLOR			"TexColor"
#define NPC_XML_ATTR_TEX_SKIN			"TexSkin"
#define NPC_XML_ATTR_CLAN				"Clan"
#define NPC_XML_ATTR_RARITY				"Rarity"
#define NPC_XML_ATTR_BPARTS_MATURE01	"BPartsMature01"
#define NPC_XML_ATTR_BPARTS_MATURE02	"BPartsMature02"
#define NPC_XML_ATTR_BPARTS_MATURE03	"BPartsMature03"
#define NPC_XML_ATTR_BPARTS_MATURE04	"BPartsMature04"
#define NPC_XML_ATTR_SKIN_COLOR			"SkinColor"
#define NPC_XML_ATTR_HAIR_COLOR			"HairColor"
#define NPC_XML_ATTR_USE_DIE_ANIMATION	"UseDieAni"

#define NPC_XML_ATTR_INTERACTION_LOOT_ANI	"ILootAni"
#define NPC_XML_ATTR_INTERACTION_LOOT_TIME	"ILootTime"

#define NPC_XML_ATTR_COL_TEST_OBJECT	"ColTestObject"
#define NPC_XML_ATTR_FACTION_LOSS_ID	 "f_loss_id"

#define NPC_XML_ATTR_SHOW_TARGEINFO		"ShowTargetInfo"

#define NPC_XML_ATTR_ANI_PREFIX			"AniPrefix"

#define NPC_XML_ATTR_L_ROTATION_ID		"LRotationID"
#define NPC_XML_ATTR_R_ROTATION_ID		"RRotationID"

#define NPC_XML_ATTR_VOICE_TYPE			"VoiceType"
#define NPC_XML_ATTR_ATTACKABLE		"Attackable"

#define NPC_XML_ATTR_BPARTS_HIT_EFFECT_1 "BParts1HitEffect"
#define NPC_XML_ATTR_BPARTS_HIT_EFFECT_2 "BParts2HitEffect"
#define NPC_XML_ATTR_BPARTS_HIT_EFFECT_3 "BParts3HitEffect"
#define NPC_XML_ATTR_BPARTS_HIT_EFFECT_4 "BParts4HitEffect"

#define NPC_XML_ATTR_NPC_IDLE_ANIMATION_NAME "IdleAni"

#define NPC_XML_ATTR_SHOW_HP_BAR		"ShowHpBar"
#define NPC_XML_ATTR_SHOW_AGGRO_INFO	"ShowAggroInfo"
#define NPC_XML_ATTR_SHOW_INDICATOR		"ShowIndicator"

#define NPC_XML_ATTR_IITEMID			 "IItemID"

struct XNPCMtrl
{
	int			nID;		///< ���� Mtrl ������ Mesh �ε���
	int			nSubCount;	///< Multi/Sub Mtrl�� ����. Standard �̸� 1��.
	vector<int>	vecSubMtrls;///< ���� ������ RMaterial�� �ε���.
};

class CSMeshInfo;
class CSMeshInfoMgr;

class XNPCInfo : public CSNPCInfo
{
protected:
	friend				class XNPCInfoMgr;
	using				CSNPCInfo::strName;			// ���� ���� �����ϴ�. ���ڿ����̺� �����ؾ��մϴ�.
	
	bool				m_bUseMeshParts;

	bool				m_bInteractionRotate;	///< ���ͷ��ǽ� ȸ������
	bool				m_bShowTargetInfo;
	bool				m_bShowIndicator;

	int					nLRotationTalentID;			// L ȸ�� �ŷ�Ʈ ID
	int					nRRotationTalentID;			// R ȸ�� �ŷ�Ʈ ID


	tstring				strClan;					// �ܺο��� ���ڿ����̺� �����ϵ��� ��


public:
	enum _CONST
	{
		BREKABLE_PARTS_MATURE	= 4
	};

	vector<XNPCInfo*>	vecModes;			///< NPC ���(�Ϲ�, �ϴ�, ���� ��)
	int					nPartsItemID[NPC_PARTS_SLOT_MAX];	
	bool				bAlignTerrain;
	bool				bAlignEnemy;		// �׻� ���� �ٶ󺸴��� ����
	bool				bUseDieAni;
	bool				m_bUseTexColor;
	unsigned int		nTexColor;
	tstring				strTexSkin;
	tstring				strInteractionLootAni;		///< ���ͷ��� ���� �Ҷ� �� �ִϸ��̼�
	bool				bDynamicShadow;
	unsigned int		nSkinColor;					///< �Ǻλ�(�ΰ����� ����)
	unsigned int		nHairColor;					///< �Ӹ���(�ΰ����� ����)
	unsigned int		nEyeColor;					///< �����ڻ�(�ΰ����� ����) -- (������, xml�� ���̺��� ���� �����ϴ�. �ʿ��ϸ� �߰��Ͻð� �Ľ��ϼ���)

	//tstring				strAniPrefix[NPC_MODE_MAX];	///< �ִϸ��̼� ���ξ�

	NPC_RARITY			nRarity;					///< ��͵�

	// 2008.03.17 by jun
	vector<XNPCMtrl*>	vecMtrls;			///< Sub Material
	

	INTERACTION_VOICE_TYPE	eVoiceType;
	INTERACTION_TYPE		eIT;		///<

	NPC_ATTACKABLE_TYPE	nInitAttackable;///< �ʱ� ���ݰ��ɿ���(������ ���)

	bool				bPartsMature[BREKABLE_PARTS_MATURE];			///< ���� ��� ����
	CSMeshInfo*			pMeshInfo;
	set<string>			setNotFakeBeatenAnimations;

	wstring				strHitBPartsEffectName[BREAKABLEPARTS_MAX_COUNT];

	tstring				strIdleAni;

	bool				bShowHpBar;
	bool				bShowAggroInfo;

	int					nIItemID;

public:
	XNPCInfo() : CSNPCInfo(), m_bUseMeshParts(false), bAlignTerrain(false), bAlignEnemy(false), m_bUseTexColor(false), bDynamicShadow(true), m_bInteractionRotate(true), bUseDieAni(false)
		, nTexColor(0xFFFFFFFF) , nSkinColor(0xFFFFFFFF), nHairColor(0xFFFFFFFF), nEyeColor(0xFFFFFFFF)
		, m_bShowTargetInfo(true)
		, nLRotationTalentID(0), nRRotationTalentID(0)
		, nRarity( NPC_RARITY_COMMON)
		, eVoiceType(VOICE_TYPE_NONE)
		, eIT(IT_NONE)
		, pMeshInfo(NULL)
		, nInitAttackable(NAT_NONE)
		, bShowHpBar(true)
		, bShowAggroInfo(true)
		, m_bShowIndicator(true)
		, nIItemID(0)
	{
		for (int i = 0; i < NPC_PARTS_SLOT_MAX; i++) nPartsItemID[i] = 0;
		for (int j = 0; j < BREKABLE_PARTS_MATURE; j++) bPartsMature[j] = false;
	}

	virtual void Cooking(CSMeshInfoMgr* pMeshInfoMgr);
	void Clear();
	bool IsUseMeshParts() { return m_bUseMeshParts; }
	bool IsUseTexColor()	{ return m_bUseTexColor; }
	bool IsInteractionRotate()	{ return m_bInteractionRotate;}
	bool IsShowTargetInfo()		{ return m_bShowTargetInfo;}
	bool IsRotationTalent(int nTalentID);
	bool IsShowIndicator();

	const TCHAR* GetName(void);
	const TCHAR* GetClan(void);
};

class XNPCInfoMgr : public CSNPCInfoMgr
{
private:

protected:
	void ParseNPC(MXmlElement* pElement, MXml* pXml);
	void ParseNPC_SubMtrl(MXmlElement* pElement, MXml* pXml);
	void ParseNPC_Parts(XNPCInfo* pNPCInfo, MXmlElement* pElement, MXml* pXml);

	unsigned int GetColorAttribute(MXmlElement* pElement, const char* szAttrName);
	void Cooking(CSMeshInfoMgr* pMeshInfoMgr);
public:
	XNPCInfoMgr();
	virtual ~XNPCInfoMgr();
	void Clear();
	bool Load(const TCHAR* szFileName, const TCHAR* szFileName_Sub, CSMeshInfoMgr* pMeshInfoMgr);
	XNPCInfo* Get(int nID);
	const TCHAR* GetName(int nID);
	int GetID(const TCHAR* pName);

	bool Exist(int nID);
	bool Reload(const TCHAR* szFileName, const TCHAR* szFileName_Sub, CSMeshInfoMgr* pMeshInfoMgr);
	virtual bool CreateNPCFromString(const std::tstring& _strXmlData) { return false; }	// mockNPCInfoMgr�� ���ؼ� ������
};

#endif