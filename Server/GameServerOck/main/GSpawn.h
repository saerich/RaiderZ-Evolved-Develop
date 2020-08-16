#pragma once

#include "MTime.h"			// for MRegulator
#include "CSWorldInfo.h"	// for GAME_TIME_TYPE, GAME_WEATHER_TYPE

struct SPAWN_INFO;
class GField;
class GEntityNPC;

class GSpawn : public MTestMemPool<GSpawn>
{
public:
	GSpawn();

	// �ʵ尡 ó�� ����ɶ� ȣ��
	void Init(GField* pField, const SPAWN_INFO* pSpawnInfo);
	// �������� �ʱ�ȭ
	void Reset();
	// ��ƽ���� ȣ�� (��ȯ���� ���� �ƴ��� ����)
	bool Update(float fDelta);

	// ������ NPC���� Ȯ��
	bool IsSpawnedNPC(MUID uidNPC) const;

	// ������Ŵ
	GEntityNPC* Spawn();
	// ������Ŵ
	GEntityNPC* Despawn();
	// ������ NPC �ν��Ͻ� ��ȯ
	GEntityNPC* GetNPCInstance();

	// ������ Ȱ��ȭ
	void Enable(bool bEnable);
	// ������ Ȱ��ȭ ����
	bool IsEnable()	{ return m_bEnable; }

	// �ʵ��� �ð��� �ٲ�� �ݹ�
	void OnTimeChangedDetail( int nTime );
	// �ʵ��� �ð��밡 �ٲ�� �ݹ�
	void OnTimeChanged( GAME_TIME_TYPE nOldTime, GAME_TIME_TYPE nNewTime );
	// �ʵ��� ������ �ٲ�� �ݹ�
	void OnWeatherChanged( GAME_WEATHER_TYPE nOldWeather, GAME_WEATHER_TYPE nNewWeather );

	// ������ �������� �ð� ����
	void SetRespawnTime(float fNextTime);

	int GetGroupID();
	
	// ���� NPC ������Ʈ Ȱ��ȭ ����
	void ActivateUpdate();	
	bool IsActivateUpdate();

	const SPAWN_INFO* GetInfo() { return m_pSpawnInfo; }

private:
	// NPC Ÿ���� �������� ����
	int PickNpcType();
	// ������ǥ ��ŷ
	void MakeSpawnPosition(const SPAWN_INFO* pSpawnInfo, vec3& pos, vec3& dir);

	// ������ �������� �ð� ����
	void SetRespawnTime();	

	// m_pSpawnInfo�� ������ ����
	bool IsSpawnInfoNULL() { return (m_pSpawnInfo == NULL); }

	// �ʵ� �ð��� ������ �´��� ����
	bool IsMatchTime();
	// �ʵ� �ð� ������ �´��� ����
	bool IsMatchTimeDetail();
	// �ʵ� �ð��� ������ �´��� ����
	bool IsMatchWeather();
	// ä�� �������� ��� �ʵ� �ִ� �ν��Ͻ��� �Ѵ��� ����
	bool IsGatherEntitySpawnEnabled();

private:
	// ���� ���ַ�����
	MRegulator		m_rgrSpawn;
	// ���� ����
	const SPAWN_INFO*		m_pSpawnInfo;
	// ������ ��ġ�� �ʵ� (��ŷ�ϱ� ���� �ʿ�)
	GField*			m_pField;		
	// ������ NPC UID
	MUID			m_uidNPC;		
	// Ȱ��ȭ ����
	bool			m_bEnable;
	// ���� NPC ������Ʈ Ȱ��ȭ ����
	bool			m_isActivateUpdate;
};
