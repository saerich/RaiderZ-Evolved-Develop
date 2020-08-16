#ifndef _XWORLD_ENV_H
#define _XWORLD_ENV_H

#include "XDef.h"
#include "CSWorldInfo.h"
#include "REnvironmentSet.h"
#include "RSkyManager.h"
#include "REffectSceneNode.h"

class XFieldInfo;
class XWorld;
class XSceneNodeEnvModifier;

// ȯ�� ����
struct XWorldEnvInfo
{
	GAME_TIME_TYPE		nTime;			///< �ð���
	GAME_WEATHER_TYPE	nWeather;		///< ����
};

// ���� ȯ��
class XWorldEnv
{
private:
	bool				m_bCreated;
	XWorldEnvInfo		m_CurrentInfo;		///< �⺻ ȯ�� ����
	REnvironmentSet*	m_pEnvset;			///< ������ Envset
	XWorld*				m_pWorld;
	XSceneNodeEnvModifier*	m_pEnvironmentChanger;	// ȯ�溯ȭ�� ���� ������
	RWorldSceneNode*	m_pWorldSceneNode;

	void CreateEnvironmentSet();

	static GAME_TIME_TYPE ConvertTime(REnvironment::ETime nTime);
	static REnvironment::ETime ConvertTime(GAME_TIME_TYPE nTime);
	static GAME_WEATHER_TYPE ConvertWeather(REnvironment::EWeather nWeather);
	static REnvironment::EWeather ConvertWeather(GAME_WEATHER_TYPE nWeather);
public:
	XWorldEnv();
	~XWorldEnv();
	bool Create(XWorld* pWorld);		// ���̺� �� ���� �� �ʱ�ȭ


	void Destroy();
	bool LoadZoneEnv(const wchar_t* szFileName);
	void Update(float fDelta, RCollisionTree* pCollisionTree);
	void SetEnvSet(const wchar_t* szEnvname);
	bool SetEnv(GAME_TIME_TYPE nTime, GAME_WEATHER_TYPE nWeather, bool bFade = false, float fFadingTime = 0.0f);
	bool SetTime(GAME_TIME_TYPE nTime, bool bFade = false, float fFadingTime = 0.0f);
	bool SetWeather(GAME_WEATHER_TYPE nWeather, bool bFade = false, float fFadingTime = 0.0f);

	void OnWorldLoadingComplete( rs3::RWorldSceneNode* pLoadingRootSceneNode );

	REnvironmentSet*	GetEnvironmentSet()					{ return m_pEnvset;	}
	
	static const GAME_TIME_TYPE		GetDefaultTime()		{ return TIME_DAYTIME; }
	static const GAME_WEATHER_TYPE	GetDefaultWeather()		{ return WEATHER_SUNNY; }

	GAME_TIME_TYPE					GetCurrentTime()		{ return m_CurrentInfo.nTime; }
	GAME_WEATHER_TYPE				GetCurrentWeather()		{ return m_CurrentInfo.nWeather; }
	bool							IsCurrentNight();
};

#endif // _XWORLD_ENV_H