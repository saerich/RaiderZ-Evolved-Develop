#ifndef _GNPCINFO_H
#define _GNPCINFO_H

#include "CSChrInfo.h"
#include "GColtInfo.h"
#include "GActorMeshInfo.h"
#include "GNPCInteractionInfo.h"
#include "GImmuneInfo.h"
#include "CSPalette.h"

class GLootInfo;
class GNPCShopInfo;

typedef vector<int> VEC_TEACHABLE_TALENT;
typedef vector<int> VEC_SELL_ITEM;



struct BPART_REWARD_INFO
{
	int nID;
	int nFlag;
	int nLootID;
	float fRate;
	USABLE_BPART_DIRECTION nDir;
	vector<int> vecPalettes;

	BPART_REWARD_INFO()
	{
		nLootID = INVALID_ID;
		nID = INVALID_ID;
		nFlag = INVALID_ID;
		fRate = 100.0f;
		nDir = UBD_NONE;

		vecPalettes.resize(PALETTESLOT_MAX, INVALID_ID);
	}

};

typedef map<int,vector<BPART_REWARD_INFO>> BPartRewardMap;

/// NPC ����
class GNPCInfo : public CSNPCInfo, public MTestMemPool<GNPCInfo>
{
	friend class GNPCInfoMgr;
	friend class GModuleAI;
public:
	list<int>		m_Talents;			///< ��밡���� ��ų ID	

	int				nSightRange;		///< �þ߰Ÿ�
	NPC_ATTACKABLE_TYPE	nInitAttackable;///< �ʱ� ���ݰ��ɿ���
	float			fStandupTime;		///< �˴ٿ�� �Ͼ������ �ɸ��� �ð�
	float			fChaseMaxDurationTime;	///< �ŷ�Ʈ�� �������� �����ϴ� �ִ� �ð�
	float			m_fHateExpiredTime; ///< ����Ʈ ���̺� ��Ʈ�� ���� ���ѽð�
	int				m_nColtLink;	///< ������ COLT�� NPC ID

	// ���� ���� ����
	float			m_fCriticalMelee;
	float			m_fCriticalRange;
	float			m_fCriticalMagic;
	float			m_fCriticalDmgAmp;			///< ġ�� ��������
	int				m_nMinDamage;
	int				m_nMaxDamage;
	float			m_fSpellPower;


	// ȸ��
	int				m_nLRotationID;
	int				m_nRRotationID;
	float			m_fRotationAngle;
	float			m_fRotationActAngle;

	// Auto Assist
	bool			m_bAutoAssist;
	float			m_fAutoAssistRange;
	bool			m_bAssistAfterCombat; ///< ���� ����� �ֺ��� ���� ������ �ٽ� �ο��� ����


	vector<GNPCInfo*>	vecModes;		///< NPC ���(�Ϲ�, �ϴ�, ���� ��)

	GLootInfo*		m_pLootInfo;	///< �Ϲ� ���� ����

	// �÷���
	bool			bRooted;		///< ������ �� �ִ��� ����
	bool			bCounterAttack; ///< �ݰ��ϴ��� ����
	bool			bCombatable;	///< ���� ���·� ���ϴ��� ����
	bool			bTauntable;	
	bool			bGainEXP;		///< NPC�� ���̴� �÷��̾�� ����ġ�� ���� ����

	// �鿪 ����
	immune::INFO	ImmuneInfo;

	// ��Ʈ ����
	GColtGroupInfo		ColtInfo;

	// MeshMeshInfo ����
	GActorMeshInfo*	m_pMeshInfo;					///< �޽� ����
	wstring			m_strOpeningStatement;		///< ������ ���
	GNPCInteractionInfo m_InteractionInfo;			///< ���ͷ��� ����

	bool			m_bObjectThinkable;			///< ObjectŸ���϶� AI��� ����

	uint8			m_nFactionGainID;			///< NPC�� �׿����� ��� �Ѽ��� ����
	uint16			m_nFactionGainQuantity;		///< NPC�� �׿����� ��� �Ѽ��� ��	
	uint16			m_nFactionLossQuantity;		///< NPC�� �׿����� �Ҵ� �Ѽ��� ��

	bool			m_bUseScriptTimer;
	bool			m_bEnableAILod;				///< AI LOD�� ������� ����
	bool			m_isMajorLodNPC;			///< AI LOD ������ NPC ���� - �ٸ� LOD �̺�Ʈ�� ������ ���� �ʰ� ���������� �����δ�.

	int				m_nLuaSpawnCount;			///< ��Ʒ� �����Ҷ� ���ʵ忡 �ִ�� �����ɼ� �ִ¼�

	BPartRewardMap m_mapBPartRewards;

	float			m_fKeepCorpseTime; // ��ü�����ð�
	float			m_fHateModifierForNPC; // NPC�� ���� ����Ʈ���̺� ������
public:
	GNPCInfo();
	~GNPCInfo();

	void Clear();
	
	bool HasTalent(int nTalentID);
	bool HasFaction();
	bool HasQuestIE();
	void ModeCopyFromParent(GNPCInfo* pParentInfo);
	void InitStandupTime();
	void InitMaxChaseDurationTime();
	void Cooking();

	void CheckUsingScriptTimer();
	bool IsBoss() const;
	bool IsUsingScriptTimer() const { return m_bUseScriptTimer; }
};

#define NPC_XML_ATTR_AGGRO_FLIP				L"flip"
#define NPC_XML_ATTR_AGGRO_RANDOM			L"random"
#define NPC_XML_ATTR_AGGRO_FAR				L"far"
#define NPC_XML_ATTR_AGGRO_NEAR				L"near"
#define NPC_XML_ATTR_AGGRO_SHORT			L"short"

#endif