#pragma once

#include "MTypes.h"
#include "MXml.h"
#include <string>
#include <map>
#include <vector>
#include "CSFieldInfo.h"
#include "CSWorldInfo.h"
#include "CSTriggerInfo.h"
#include "SFieldInfo.h"

using namespace std;

#define MAX_SPAWN_RATE	1000

class GFieldInfoMgr;
class GEntityActor;
class GEntityNPC;
class GField;
class GCollisionTree;
class GFieldInfoLoader;
class GFieldAttrGrid;
class GPickingZTable;

// �� �ʿ� �� ������(DB�� xml�κ��� �о ��)

struct SPAWN_NPC_INFO
{
	bool bDefaultSet;	///< �ƹ��͵� ���õ��� ������ �� ���� ����
	int nPickRate;		///< Pick rate
	int nID;			///< �ʵ忡���� NPC �ĺ���

	SPAWN_NPC_INFO(): bDefaultSet(false), nID(0), nPickRate(0) 									{}
	SPAWN_NPC_INFO(int nID): bDefaultSet(true), nID(nID), nPickRate(MAX_SPAWN_RATE) 			{}
	SPAWN_NPC_INFO(int nID, int nPickRate): bDefaultSet(false), nID(nID), nPickRate(nPickRate) 	{}
};


/// ���� ����
struct SPAWN_INFO : public MARKER_INFO
{
	bool			bEnable;								///< ���������� ����
	int				nStartTime;								///< �ʵ尡 �������� ������ ������ �ð� (sec)
	unsigned int	nRespawnTime;							///< ������������ �ð�
	bool			bRandomDir;								///< ������ �������� ����
	float			fRadius;								///< ���� ����
	float			fRoamRadius;							///< ��ȸ ����
	float			fRoamDepth;								///< ��ȸ ������ Z����
	int				nGroupID;								///< NPC Group ID
	bool			bEnableTimeList[MAX_TIME_TYPE];			///< ���� ������ �ð��� ���
	bool			bEnableWeatherList[MAX_WEATHER_TYPE];	///< ���� ������ ���� ���
	pair<int,int>	pairEnableTime;							///< ������ �ð������� (���۽ð�, ���ð�)
	bool			bUseConditionTime;						///< �ð��� ���� ���
	bool			bUseConditionWeather;					///< ���� ���� ���
	GATHER_TYPE		nGatherType;							///< ä���� ��� ä�� Ÿ�� - ä���� �ƴϸ� GATHER_NONE
	bool			bCheckSpawnPointValidation;				///< �����Ǵ� ��ġ ��ȿ�� �Ǵ����� ����(�׺���̼� �޽�)
	vector<SPAWN_NPC_INFO> vecNPCInfoList;					///< ������ NPC ����	
	float			fCombatRadius;							///< ���� ������ ���� (�������� ����� ���� ����)
	

	SPAWN_INFO()
		: bRandomDir(true)
		, bEnable(true)
		, nStartTime(0)
		, fRadius(0.0f)
		, fRoamRadius(0.0f)
		, nRespawnTime(0)
		, nGroupID(INVALID_SPAWN_GROUP_ID)
		, fRoamDepth(200.f)
		, bCheckSpawnPointValidation(true)
		, bUseConditionTime(false)
		, bUseConditionWeather(false)
		, pairEnableTime(-1, -1)
		, nGatherType(GATHER_NONE)
		, fCombatRadius(0.0f)
	{
		memset(bEnableTimeList, 0, sizeof(bEnableTimeList));
		memset(bEnableWeatherList, 0, sizeof(bEnableWeatherList));
	}
};

/// �ʵ� ���� ���� ���
struct GFieldWeatherInfoNode
{
	GAME_WEATHER_TYPE	nWeather;
	int					nPercent;

	GFieldWeatherInfoNode() : nWeather(DEFAULT_GAME_WEATHER), nPercent(0) {}
};

/// �ʵ��� ���� ����
struct GFieldWeatherInfo
{
	GAME_WEATHER_TYPE				nDefault;
	vector<GFieldWeatherInfoNode>	vecNodes;
	vector<GAME_WEATHER_TYPE>		vecRandomData;
	
	GFieldWeatherInfo() : nDefault(DEFAULT_GAME_WEATHER) {}
	void Clear();
};


typedef map<unsigned long, SPAWN_INFO> SPAWN_INFO_MAP;
typedef map<unsigned long, MARKER_INFO> MARKER_INFO_MAP_BY_ID;
typedef map<unsigned long, SENSOR_INFO> SENSOR_INFO_MAP_BY_ID;
typedef map<int, vec3> SPAWNPOS_INFO_MAP;

struct FIELD_GROUP_INFO;

/// �ʵ� ����
/// - �����ϰ� �ʵ� �������� ������ �ִ�.
class GFieldInfo : public SFieldInfo, public MTestMemPool<GFieldInfo>
{
	friend class GFieldInfoMgr;
	friend class GField;
	friend class GFieldInfoLoader;
	friend class GFieldInfoValidator;
public:
	int						m_nZoneID;		///< �� ID
	wstring					m_strZoneName;	///< �� �̸�
	wstring					m_strZoneFile;	///< �� ���� �̸�
	int						m_nParentID;	// �θ��id
	GFieldWeatherInfo		m_WeatherInfo;	// ���� ����
	FIELD_TYPE				m_nType;
	float					m_fSectorHeight;	///< ���Ͷ���� �� ��׷� ��������
	int						m_nMaxGatherInstance[MAX_GATHER_TYPE];		///< �ʵ�� �ִ� ä�� ������Ʈ �ν��Ͻ� ���� - 0�̸� ������
	FIELD_GROUP_INFO*		m_pFieldGroup;
	int						m_nQPVPNPCID1;
	int						m_nQPVPNPCID2;
	int						m_nQPVPRebirthMarkerID1;
	int						m_nQPVPRebirthMarkerID2;
	int						m_nIntroCutsceneID;

private:
	float					m_fMinX;
	float					m_fMaxX;
	float					m_fMinY;
	float					m_fMaxY;
public:
	GFieldInfo();
	virtual ~GFieldInfo();

	void Init(const wstring& strZonePath, const wstring& strFile);
	void						Reset();
	void						RemoveSpawnTime();

	const GAME_WEATHER_TYPE		GetRandomWeather() const;
	const SPAWN_INFO_MAP&		GetSpawnInfo() const{ return m_mapSpawn; }
	vec3						GetSpawnPos(int nNPCID);

	const SENSOR_INFO_MAP_BY_ID& GetSensorInfoContainer() const	{ return m_mapSensorByID; }	

	const MARKER_INFO*			FindMarker(unsigned long nID) const;
	const SENSOR_INFO*			FindSensor(int nID) const;
	const SPAWN_INFO*			FindSpawnInfo(unsigned long nID) const;

	const MARKER_INFO*			GetFirstMarker(void) const;
	void						InsertMarkerInfo(const MARKER_INFO& markerInfo);
	void						InsertSpawnInfo(const SPAWN_INFO& info);
	void						InsertSensor( SENSOR_INFO& info );

	void						AutoAssign_IsSensorScript();
	GCollisionTree*				GetCollisionTree() { return m_pCollisionTree; }
	const bool					IsWeatherNotChanged() const { return m_WeatherInfo.vecRandomData.empty(); }

	bool						IsSectorHeightLimitField() const;							///< �ʵ忡 ��� �������� ����Ǿ����� ����
	bool						IsAllowSectorHeightLimit(float fDifferenceHeight) const;	///< �ش� �������� �ʵ忡�� ����ϴ� ���������� ���´�

	void ChangeSensorSetting( SENSOR_INFO& source_info);

	bool IsLoadingCollision();
	bool IsLoadingPath();

	GFieldAttrGrid*				GetAttrGrid() const			{ return m_pAttrGrid; }
	void SetPickingZTable(GPickingZTable* pTable)			{ m_pPickingZTable = pTable; }
	GPickingZTable*				GetPickingZTable() const	{ return m_pPickingZTable; }

	float MinX() const;
	float MinY() const;
	float MaxX() const;
	float MaxY() const;
	void SetFieldMinXY_ForTest(float x, float y);
	void SetFieldMaxXY_ForTest(float x, float y);

	void Cooking();

	bool IsInWarpSensor(vec3 vPos);
	const MARKER_INFO* GetLeaveMarker(int nToFieldID) const;

	bool IsPortalTalentUsable() const	{ return m_bPotralTalentUsable; }

	const set<int>& CollectQuestNPC() const	{ return m_setQuestNPC; }

protected:
	GCollisionTree*		m_pCollisionTree;		// �浹��
	GFieldAttrGrid*		m_pAttrGrid;
	GPickingZTable*		m_pPickingZTable;

	// �ߺ����� �ƴϴ�... ���߿� ����.

	SPAWN_INFO_MAP			m_mapSpawn;
	MARKER_INFO_MAP_BY_ID	m_mapMarkerByID;
	SENSOR_INFO_MAP_BY_ID	m_mapSensorByID;

	SPAWNPOS_INFO_MAP		m_mapSpawnPos;		// SPAWN_INFO_MAP�� �ٸ��� Ű���� NPCID, ù��° ��ġ�� ������ �ִ�.
	set<int>				m_setQuestNPC;		// ����Ʈ ���õ� NPC(����, ����)

	// �� ���� ������

	bool LoadCollision(wstring& strZoneFilename);
	bool LoadPath(const wstring& strZonePath, const wstring& strPathFile);
	void InitWeather();
	bool LoadCommon(wstring& strZoneFilename);
};

struct FIELD_GROUP_INFO : public MTestMemPool<FIELD_GROUP_INFO>
{
	FIELD_GROUP_INFO()
	: nID(0)
	, pStartFieldInfo(NULL)
	, pStartMarkerInfo(NULL)
	, bValidation(true)
	, bChallengerQuest(false)
	{
	}

	int					nID;
	const GFieldInfo*	pStartFieldInfo;
	const MARKER_INFO*	pStartMarkerInfo;
	vector<const GFieldInfo*>	vecFieldInfo;
	set<int>			setRunningQuest;	// �ش� �ʵ� �׷쿡�� ����Ǵ� ����Ʈ��
	bool				bValidation;
	bool				bChallengerQuest; // ����������Ʈ�� ����Ǵ� ������ ����
};

