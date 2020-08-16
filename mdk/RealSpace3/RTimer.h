#pragma once

#include "RLibTypes.h"

// DXUTTimer�� ����, QueryPerformance ��� TimeGetTime�� ����Ͽ� �������.
namespace rs3 {

class RS_API RTimer
{
protected:
	bool m_bTimerStopped;		///< Ÿ�̸Ӱ� �����ߴ��� ����

	DWORD m_dwLastUpdateTime;	///< ������ ������Ʈ �� �ð�
	DWORD m_dwElapsedTime;		///< ������ ������Ʈ���� �帥 �ð� ����
	DWORD m_dwStopTime;			///< ������ �ð�
	DWORD m_dwBaseTime;			///< Start ( or reset) �� �ð�

public:
	RTimer();

	void	Reset();		///< resets the timer
	void	Start();		///< starts the timer
	void	Stop();			///< stop (or pause) the timer
	void	Update();		///< ElapsedTime�� ���ϱ� ���� ������Ʈ
	void	UpdateCustom(DWORD dwElapsedTime);	///< ElapsedTime �����Ͽ� ������Ʈ

	// get the absolute system time
	DWORD	GetAbsoluteTime() const;
	
	// get the current time
	DWORD	GetTime() const;

	// get the time that elapsed between GetElapsedTime() calls
	DWORD	GetElapsedTime() const;

	// returns true if timer stopped
	bool	IsStopped() const;

	// set/get application elapsed time
// 	void	SyncToApplicationElapsedTime(float fDelta);
// 	void	SyncToApplicationElapsedTime(DWORD dwDelta);
};

class RS_API RTimeChecker
{
private:
	unsigned int m_nLastTime;
public:
	RTimeChecker() : m_nLastTime(0) {}
	void Start();
	unsigned int Check();
};

class RS_API RTimeChecker100us
{
private:
	__int64 m_i64LastTime;
public:
	RTimeChecker100us(){}
	void Start();
	unsigned int Check();
};

}
