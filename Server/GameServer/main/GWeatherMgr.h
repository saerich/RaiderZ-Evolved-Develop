#pragma once

#include <queue>
#include "CSWorldInfo.h"
using namespace std;


class GFieldInfo;

typedef queue<GAME_WEATHER_TYPE, std::list<GAME_WEATHER_TYPE> >		GWeatherQueue;

class GWeatherMgr : public MTestMemPool<GWeatherMgr>
{
private:
	float				m_fTick;
	const GFieldInfo*	m_pFieldInfo;
	float				m_fElapsedTime;
	GAME_WEATHER_TYPE	m_nOldWeather;
	GAME_WEATHER_TYPE	m_nCurrentWeather;
	GWeatherQueue		m_Queue;
	bool				UpdateQueue();
	void				TransWeather(GWeatherQueue& _queue, GAME_WEATHER_TYPE nCurrentWeather, GAME_WEATHER_TYPE nNextWeather);

public:
						GWeatherMgr();
						~GWeatherMgr();
	void				Init(const GFieldInfo* pFieldInfo, int nTick);
	bool				Update(float fDelta);		// �ٲ� �Ͽ� true ��ȯ
	void				ChangeWeather(GAME_WEATHER_TYPE nWeatherType);			///< ������ ������ �����Ѵ�.
	GAME_WEATHER_TYPE	GetOldWeather()			{ return m_nOldWeather; }
	GAME_WEATHER_TYPE	GetCurrentWeather()		{ return m_nCurrentWeather; }
};



