#ifndef _GSPAWN_H
#define _GSPAWN_H

#include "MMemPool.h"
#include "CSWorldInfo.h"
#include "CSTriggerInfo.h"


class GSpawn;
class GField;
class GNPCInfo;
class GEntityNPC;
class GSpawnDelay;
class GDespawnAuto;
struct SPAWN_INFO;

class GSpawnManager : public MMemPool<GSpawnManager>
{
public:

public:
				GSpawnManager(GField* pField);
	virtual		~GSpawnManager();

	void		Init();
	void		Fini();
	void		AddSpawn(const SPAWN_ID nID, const SPAWN_INFO* pSpawnInfo);

	void		Update(float fDelta);
	void		Update_Spawn(float fDelta);
	void		Update_SpawnDelay(float fDelta);
	void		Update_DespawnAuto(float fDelta);

	// NPC�� �׾��� �� ȣ��
	void		OnNPCDie(SPAWN_ID nID);

	// SPAWN_ID�� ����
	GEntityNPC* Spawn(SPAWN_ID nSpawnID, float fSpawnDelayTime=0.0f, float fDespawnTime=0.0f);
	// NPC ID�� ����
	GEntityNPC*	Spawn(int nNPCID, const vec3& vPos, const vec3& vDir, float fSpawnDelayTime=0.0f, float fDespawnTime=0.0f, SpawnDetailInfo* pDetailInfo=NULL);
	
	// ������Ŵ
	GEntityNPC* Despawn(SPAWN_ID nSpawnID, bool bSpawnEnable=true);
	// �������� ����
	void		Reset(SPAWN_ID nSpawnID);
	// ������ NPC �ν��Ͻ� ��ȯ
	GEntityNPC* GetNPCInstance(SPAWN_ID nSpawnID);
	// Ȱ��ȭ���� ����
	void		Enable(SPAWN_ID nSpawnID, bool bSwitch);
	// Ȱ��ȭ���� ��ȯ
	bool		IsEnable(SPAWN_ID nSpawnID);

	// �ʵ��� �ð��� �ٲ�� �ݹ�
	void OnTimeChangedDetail( int nTime );
	// �ʵ��� �ð��밡 �ٲ�� �ݹ�
	void OnTimeChanged( GAME_TIME_TYPE nOldTime, GAME_TIME_TYPE nNewTime );
	// �ʵ��� ������ �ٲ�� �ݹ�
	void OnWeatherChanged( GAME_WEATHER_TYPE nOldWeather, GAME_WEATHER_TYPE nNewWeather );

	// ���� �ð��ڿ� ������ų�� ������
	bool		IsSpawnDelayEmpty();

	// ������ NPC���� Ȯ��
	bool IsSpawnedNPC( SPAWN_ID nSpawnID, MUID uidNPC );
	
	void		ActivateSpawnGroup(int nGroupID);

	const SPAWN_INFO* GetSpawnInfo( unsigned long nSpawnID );

private:
	GSpawn*		NewSpawn();
	void		AddSpawnDelay(int nNPCID, const vec3& vPos, const vec3& vDir, float fDelayTime);
	void		AddSpawnDelay(SPAWN_ID nSpawnID, float fDelayTime);
	void		AddDespawnAuto(MUID uidNPC, float fDespawnTime);
	void		AddDespawnAuto(SPAWN_ID nSpawnID, float fDespawnTime);
	GSpawn*		Find(SPAWN_ID nSpawnID);	

private:
	typedef map<SPAWN_ID, GSpawn*>	SPAWN_MAP;
	typedef list<GSpawnDelay*>		SPAWNDELAY_LIST;
	typedef list<GDespawnAuto*>		DESPAWNAUTO_LIST;
	SPAWN_MAP					m_mapSpawns;
	SPAWNDELAY_LIST				m_listSpawnDelays;
	DESPAWNAUTO_LIST			m_listDespawnAuto;
	GField*						m_pField;
	vector<GSpawn*>				m_vecUnspawneds;
};


#endif