#pragma once

// --------------------------------------------------------------------
// MasterServer�� GameServer���� ��� ����Ÿ
// --------------------------------------------------------------------

#include "MTypes.h"
#include "SDef.h"
#include "CSDef.h"
#include "CSChrInfo.h"
#include "CSPalette.h"

#pragma pack(push, old)
#pragma pack(1)

/// ���� ������ �α��� �� �޴� ������ ���� �ʵ� ����
struct TDMG_MANAGED_SHARED_FIELD
{
	static const int MAX_SHARED_FIELD = 16;


	int					nFieldID;						///< �ʵ� ID
	int8				nChannelCount;					///< �Ҵ�� ä�� ��

	int					nChannelID[MAX_SHARED_FIELD];	///< �Ҵ�� ä�� ID ���
	MUID				uidField[MAX_SHARED_FIELD];		///< �Ҵ�� ä�κ� �ʵ� UID

	TDMG_MANAGED_SHARED_FIELD() : nFieldID(0), nChannelCount(0)
	{
		for (int i = 0; i < MAX_SHARED_FIELD; i++) 
		{
			nChannelID[i] = 0;
			uidField[i] = MUID::ZERO;
		}
	}
};

/// �ʵ� ����
struct TDMG_SHARED_FIELD_INFO
{
	short				nGameServerID;
	MUID				uidField;
	int					nFieldID;
	int					nChannelID;
};

/// ���� �ʵ� �׷� ����
struct TDMG_DYNAMIC_FIELD_GROUP_INFO
{
	MUID				uidDynamicFieldGroup;
	MUID				uidCreator;
	int					nFieldGroupID;
	int8				nType;
	short				nGameServerID;
};

/// ���� �ʵ� ���
struct TDMG_DYNAMIC_FIELD_NODE
{
	MUID uidFieldGroup;
	MUID uidDynamicField;
	int nFieldID;
};

// Proxy �ʵ� ���� ����
struct TDMG_PROXY_FIELD_UPDATE_INFO
{
	uint8 nPlayerCount;
};


// ���� �̵� �÷��̾� ����
struct TD_MOVE_PLAYER_INFO
{
	uint32					nEnterKey;			// �÷��̾� Ŭ���̾�Ʈ public IP
	uint32					nPlayerAID;			// �÷��̾� DB Index

	TD_MOVE_PLAYER_INFO()
	: nEnterKey(0)
	, nPlayerAID(0)
	{}
};

struct TD_PLAYER_GAME_DATA_CHARAINFO
{
	int8					nSex;						// SEX
	int						nCID;
	int						nLevel;
	int						nXP;
	int						nRemaindTP;
	int						nMoney;
	int						nStorageMoney;
	int						nHP;
	int						nEN;
	int						nSTA;
	int						nGID;
	int8					nWeaponSet;					// SH_ITEM_SWITCH_WEAPON
	int8					nFeatureHair;
	int8					nFeatureFace;
	short					nFeatureHairColor;
	short					nFeatureSkinColor;
	uint8					nEyeColor;
	uint8					nMakeUp;
	uint8					nTattooType;
	short					nTattooPosX;
	short					nTattooPosY;
	uint8					nTattooScale;
	SBID					nSoulBindingID;
	int8					nInnRoomID;
	int8					ActivePaletteID;			// PALETTE_NUM
	int8					PrimaryPaletteID;			// PALETTE_NUM
	int8					SecondaryPaletteID;			// PALETTE_NUM
	wchar_t					szName[PLAYER_NAME_LEN + 1];
	short					nFatigueQuantity;
	int						nBlankTimeSec;	
	MUID					uidParty;
	MUID					uidArena;
	MUID					uidArenaTeam;
	uint8					nPlayerGrade;
	int						nPlayTimeSec;
	int						nConnPlayTimeSec;
	int						nElapsedDeadTimeSec;
	uint64					nMailTopUID;				///< ������ �Ӽ� : �ֻ��� ���� UID
	uint64					nMailBottomUID;				///< ������ �Ӽ� : ������ ���� UID

	TD_PLAYER_GAME_DATA_CHARAINFO()
	: nSex(SEX_NA)
	, nCID(-1)
	, nLevel(0)
	, nXP(0)
	, nRemaindTP(0)
	, nMoney(0)
	, nStorageMoney(0)
	, nHP(0)
	, nEN(0)
	, nSTA(PLAYER_DEFAULT_STR)
	, nGID(0)
	, nWeaponSet(ITEMSWITCH_PRIMARY)
	, nFeatureHair(0)
	, nFeatureFace(0)
	, nFeatureHairColor(0)
	, nFeatureSkinColor(0)
	, nMakeUp(0)
	, nEyeColor(0)
	, nTattooType(0)
	, nTattooPosX(0)
	, nTattooPosY(0)
	, nTattooScale(0)
	, nSoulBindingID(0)
	, nInnRoomID(0)
	, ActivePaletteID(PALETTENUM_1)
	, PrimaryPaletteID(PALETTENUM_1)
	, SecondaryPaletteID(PALETTENUM_1)	
	, nFatigueQuantity(0)
	, nBlankTimeSec(0)
	, uidParty(MUID::ZERO)
	, uidArena(MUID::ZERO)
	, uidArenaTeam(MUID::ZERO)
	, nPlayerGrade(PLAYER_GRADE_NORMAL)
	, nPlayTimeSec(0)
	, nElapsedDeadTimeSec(0)
	, nMailTopUID(0)
	, nMailBottomUID(0)
	{
		szName[0] = L'\0';
	}
};

/// ���� �ʵ� ���� ��ġ ����
struct TD_DYNAMIC_FIELD_ENTER_POS_INFO
{
	int				nFieldID;
	int				nChannelID;
	vec3			vPos;
	vec3			vDir;
};

struct TD_PLAYER_GAME_DATA_EQUIP
{
	int						nType;
	int64					nIUID;

	TD_PLAYER_GAME_DATA_EQUIP(): nType(0), nIUID(0)
	{}
};

struct TD_PLAYER_GAME_DATA_ITEM_INSTANCE
{
	int						nItemID;
	int						nQuantity;
	int						nDurability;
	int						nDyedColor;
	int64					nIUID;		
	bool					bBind;
	uint8					nSoulQuantity;		
	uint8					nSlotType;
	int16					nSlotID;
	uint8					nUsageState;
	int						nCharPlayTimeSec;	
	bool					bUseUsagePeriod;
	bool					bStartCalculatingUsagePeriod;
	int						nUsagePeriodSec;
	bool					bUseExpirationTime;
	int						nExpirationTimeSec;
	wchar_t					szExpiDt[MAX_DB_TIMESTAMP_STR];
	int						nEnchants[ENCHANT_MAX_COUNT];	// ��æƮ ��� (ù��°�� Ȱ��ȭ ��æƮ)

	TD_PLAYER_GAME_DATA_ITEM_INSTANCE()
	: nItemID(0)
	, nQuantity(0)
	, nDurability(0)
	, nDyedColor(0)
	, nIUID(0)
	, bBind(false)
	, nSoulQuantity(0)
	, nSlotType(SLOTTYPE_INVENTORY)
	, nSlotID(0)
	, nUsageState(0)
	, nCharPlayTimeSec(0)
	, bUseUsagePeriod(false)
	, bStartCalculatingUsagePeriod(false)
	, nUsagePeriodSec(0)
	, bUseExpirationTime(false)
	, nExpirationTimeSec(0)
	{
		szExpiDt[0] = L'\0';
		for (int i=0; i<ENCHANT_MAX_COUNT; ++i)
		{
			nEnchants[i] = ENCHANT_UNUSED_SLOT;
		}
	}
};

struct TD_PLAYER_GAME_DATA_PALETTE
{
	uint8					nOrderID;
	int8 					nType;			// PALETTE_ITEM_TYPE		
	int						nID;

	TD_PLAYER_GAME_DATA_PALETTE()
	: nOrderID(0)
	, nType(PIT_NONE)
	, nID(0)
	{}
};

struct TD_PLAYER_GAME_DATA_QUEST_INSTANCE
{
	uint8				nSlotID;
	int					nQID;
	int					nVar;
	uint8				nDone;
	int					nDoneCount;
	uint32				nAcceptTime;
	wchar_t				szAcceptDt[MAX_DB_TIMESTAMP_STR];
	wchar_t				szExpiDt[MAX_DB_TIMESTAMP_STR];
	int16				nObj1Progress;
	int16				nObj2Progress;
	int16				nObj3Progress;
	int16				nObj4Progress;
	int16				nObj5Progress;
	bool				bObj1Complete;
	bool				bObj2Complete;
	bool				bObj3Complete;
	bool				bObj4Complete;
	bool				bObj5Complete;

	TD_PLAYER_GAME_DATA_QUEST_INSTANCE()
	: nSlotID(255)
	, nQID(0)
	, nVar(0)
	, nDone(0)
	, nDoneCount(0)
	, nAcceptTime(0)
	, nObj1Progress(0)
	, nObj2Progress(0)
	, nObj3Progress(0)
	, nObj4Progress(0)
	, nObj5Progress(0)
	, bObj1Complete(0)
	, bObj2Complete(0)
	, bObj3Complete(0)
	, bObj4Complete(0)
	, bObj5Complete(0)
	{
		szAcceptDt[0] = L'\0';
		szExpiDt[0] = L'\0';
	}
};

struct TD_PLAYER_GAME_DATA_QUEST_DONE_COUNT
{
	int nQID;
	int nDoneCount;

	TD_PLAYER_GAME_DATA_QUEST_DONE_COUNT()
	: nQID(0)
	, nDoneCount(0)
	{}
};

struct TD_PLAYER_GAME_DATA_FACTION
{
	uint8	nFactionID;
	uint16	nFaction;

	TD_PLAYER_GAME_DATA_FACTION()
	: nFactionID(0)
	, nFaction(0)
	{}
};


/// �ʵ� �̵��� ����ü
struct TD_GATE_INFO
{
	int nFieldID;
	vec3 vPosition;
	vec3 vDirection;
	bool bDynamicField;

	// Shared Field
	int nChannelID;

	// Dynamic Field
	MUID uidDynamicFieldGroup;

	TD_GATE_INFO()
	: nFieldID(0)
	, vPosition(vec3::ZERO)
	, vDirection(0.0f, 1.0f, 0.0f)
	, bDynamicField(false)
	, nChannelID(0)
	, uidDynamicFieldGroup(MUID::ZERO)
	{
	}

	TD_GATE_INFO& operator=(const TD_GATE_INFO& GateInfo)
	{
		nFieldID = GateInfo.nFieldID;
		vPosition = GateInfo.vPosition;
		vDirection = GateInfo.vDirection;
		bDynamicField = GateInfo.bDynamicField;
		nChannelID = GateInfo.nChannelID;
		uidDynamicFieldGroup = GateInfo.uidDynamicFieldGroup;

		return *this;
	}
};

/// �ʵ� �׷� ����/�̵� ��û�� ����ü
struct TD_GATE_FIELD_GROUP_INFO
{
	int nFieldGroupID;
	int8 nType;
	bool isSingleType;
	TD_GATE_INFO GateInfo;

	TD_GATE_FIELD_GROUP_INFO()
		: nFieldGroupID(0)
		, nType(0)
	{
	}

	TD_GATE_FIELD_GROUP_INFO& operator=(const TD_GATE_FIELD_GROUP_INFO& GateInfo)
	{
		nFieldGroupID = GateInfo.nFieldGroupID;
		nType = GateInfo.nType;

		return *this;
	}
};

// ��Ƽ ����ȭ ����ü
struct TD_PARTY_INFO
{
	MUID uidParty;
	int nLeaderIndex;
	int nMemberCount;
	bool bCreatedByAutoParty; // �ڵ���Ƽ�� ���� �������

	struct MEMBER_INFO
	{
		MUID uidMember;
		wchar_t szMemberName[PLAYER_NAME_LEN + 1];
		int	 nMemberCID;
	} members[MAX_PARTY_MEMBER_COUNT];
};


// ��Ƽ ����Ʈ ����ȭ ����ü
struct TD_PARTY_QUEST
{
	int nQuestID;
	int nQuestCount;

	TD_PARTY_QUEST(): nQuestID(0), nQuestCount(0)	{}
};


// ��Ƽ �ʵ� ����ȭ ����ü
struct TD_PARTY_FIELD
{
	int nFieldGroupID;
	MUID uidFieldGroup;

	TD_PARTY_FIELD(): nFieldGroupID(0), uidFieldGroup(MUID::Invalid())	{}
};


// ���� ���� �ð�
struct TD_REMAIN_BUFF_TIME
{
	int nID;
	float fRemainDurationSeconds;
	float fRemainNextPeriodSeconds;
	float fRemainBuffOffsetSeconds;
	int nStackedCount;

	TD_REMAIN_BUFF_TIME(): nID(INVALID_ID), fRemainDurationSeconds(0.0f), fRemainNextPeriodSeconds(0.0f), fRemainBuffOffsetSeconds(0.0f), nStackedCount(0) {}
};


// �ۼ��� ����
struct TD_APP_SERVER_INFO
{
	int		nAppServerMode;		// SAppServerMode
	uint32	nIP;
	int		nPort;	
};

// ���
struct TD_GUILD_ONLINE_MEMBER_INFO
{
	int		nCID;
	int		nFieldID;
	int		nChannelID;

	TD_GUILD_ONLINE_MEMBER_INFO(int nCID, int nFieldID, int nChannelID)
	: nCID(nCID), nFieldID(nFieldID), nChannelID(nChannelID) {}
};

#pragma pack(pop, old)
