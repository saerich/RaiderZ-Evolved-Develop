#pragma once

// MRegulator�� ������
// IsReady()���� ElapsedTime�� 0���� �ٲٴ� ���,
// ������Ʈƽ�� �������� ƽ�� ����ϰ� ���������� �����Ѵ�.
class GTickRegulator
{
private:
	bool	m_bActive;
	float	m_fElapsed;
	float	m_fUpdateTick;
	int		m_nTickCounter;
public:
	GTickRegulator();
	GTickRegulator(float fUpdateTick);
	GTickRegulator& operator=(const GTickRegulator& other);

	void	SetTickTime(float fTime);
	float	GetTickTime();
	float	GetElapsedTime();
	bool	IsReady(float fDelta);
	void	Start();
	void	Reset(bool bImmediately=false);
	void	Stop();
	void	SetElapsedTime(float fElapsedTime);
	bool	IsActive();
	int		GetTickCounter();	
};