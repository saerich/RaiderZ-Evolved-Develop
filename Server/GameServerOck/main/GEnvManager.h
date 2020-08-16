#ifndef _GENV_MANAGER_H
#define _GENV_MANAGER_H

#include "CSWorldInfo.h"

struct GEnvParam
{
	int nStartHour[MAX_TIME_TYPE];

	int nLocalTimePerGameTimeOneHour;
	int nUpdateTickSec;
	int nWeatherTickSec;
	GEnvParam()
	{
		nStartHour[TIME_DAWN]		= 4;			// 4:00am ~ 7:00am
		nStartHour[TIME_DAYTIME]	= 7;			// 7:00am ~ 6:00pm
		nStartHour[TIME_SUNSET]		= 18;			// 6:00pm ~ 9:00pm
		nStartHour[TIME_NIGHT]		= 21;			// 9:00pm ~ 4:00am
		nStartHour[TIME_CUSTOM1]	= 0;
		nStartHour[TIME_CUSTOM2]	= 0;

		nLocalTimePerGameTimeOneHour = 60;
		nUpdateTickSec = 720;
		nWeatherTickSec = 120;
	}
};

#define MAX_ENV_HOUR	24

/// ȯ��(�ð�, ����) ������
class GEnvManager : public MTestMemPool<GEnvManager>
{
protected:
	// ������
	GAME_TIME_TYPE		m_nGameTimes[MAX_ENV_HOUR];				///< �ð��� ���� �ð�

	int					m_nLocalSecPerGametimeHour;		///< ���ӻ󿡼� 1�ð��� �ǽð����� ����? (�⺻��=120��)
	int					m_nGameTimeUpdateTickSec;		///< �ð�üũ ƽ��(��) - 60��

	float				m_fElapsed;
	float				m_fNextCheckTime;
	GAME_TIME_TYPE		m_nOldTime;
	GAME_TIME_TYPE		m_nCurrentTime;
	bool				m_bChagnedTime;
	int					m_nBaseHour;

	int					m_nOldTimeHour;
	int					m_nCurrentTimeHour;
	bool				m_bChagnedTimeHour;

protected:
	virtual tm*			GetLocalTime();

	GAME_TIME_TYPE		CheckGameTime(int nTime);
	int					CheckGameTimeHour();
	void				RouteGameTime(GAME_TIME_TYPE nGameTime);
	void				SetTimeHour(int nHour);		// ���Ƿ� ������ �ð��� �����Ѵ�.
public:
	GEnvManager();
	virtual ~GEnvManager();
	bool				Create(GEnvParam& envParam);
	void				Update(float fDelta);		// �ð��밡 ���ϸ� true ��ȯ
	void				SetTime(GAME_TIME_TYPE nTime);		// ���Ƿ� ������ �ð��븦 �����Ѵ�.
	void				ChangeTimeForced(GAME_TIME_TYPE nGameTime);
	bool				IsChangedTime();
	bool				IsChangedTimeHour();
	GAME_TIME_TYPE		GetOldTime()		{ return m_nOldTime; }
	GAME_TIME_TYPE		GetCurrentTime()	{ return m_nCurrentTime; }	///< ���ӽð��븦 ��ȯ
	
	int					GetCurrentTimeHour()	{ return m_nCurrentTimeHour; } ///< ��Ȯ�� ���ӽð��� ��ȯ (0~24)
};




#endif // _GENV_MANAGER_H