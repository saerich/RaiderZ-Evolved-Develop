#include "stdafx.h"
#include "GEnvManager.h"

SUITE(EnvManager)
{
	/// �׽�Ʈ�� ���� LocalTime�� �������̵��� EnvManager Ŭ����
	class TestEnvManager : public GEnvManager
	{
	protected:
		tm			m_LocalTime;
		virtual tm*			GetLocalTime()
		{
			return &m_LocalTime;
		}
	public:
		TestEnvManager() : GEnvManager() {}
		virtual ~TestEnvManager() {}
		using GEnvManager::CheckGameTimeHour;

		void ExpectLocalTime(int nHour, int nMin, int nSec)
		{
			m_LocalTime.tm_hour = nHour;
			m_LocalTime.tm_min = nMin;
			m_LocalTime.tm_sec = nSec;
		}
	};
	class FEnvManager
	{
	public:
		FEnvManager()
		{

		}
		~FEnvManager()
		{

		}
		void Init(int nSecPerGameTimeOneHour)
		{
			m_EnvManager.ExpectLocalTime(0, 0, 0);

			GEnvParam envParam;
			envParam.nStartHour[TIME_DAWN]		= 4;
			envParam.nStartHour[TIME_DAYTIME]	= 7;
			envParam.nStartHour[TIME_SUNSET]	= 18;
			envParam.nStartHour[TIME_NIGHT]		= 21;
			envParam.nUpdateTickSec = 1;			// 1�ʸ��� ƽ üũ�ϵ��� ��
			envParam.nLocalTimePerGameTimeOneHour = nSecPerGameTimeOneHour;	

			m_EnvManager.Create(envParam);
		}

		TestEnvManager	m_EnvManager;
	};

	TEST_FIXTURE(FEnvManager, TestEnvManager_UpdateGameTime)
	{
		const float ENV_TICK = 1.1f;

		Init(3600);	// ���� �ð��� ����.. (1�ð� == 3600��)

		m_EnvManager.ExpectLocalTime(0, 0, 0);

		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_NIGHT, m_EnvManager.GetCurrentTime());

		// ���� 3�ö� �ð��� �ٲ��� ����
		m_EnvManager.ExpectLocalTime(3, 0, 0);

		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_NIGHT, m_EnvManager.GetCurrentTime());

		// 5�ö� DAWN���� �ٲ�
		m_EnvManager.ExpectLocalTime(5, 0, 1);

		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_DAWN, m_EnvManager.GetCurrentTime());

		// 7�ö� DAYTIME���� �ٲ�
		m_EnvManager.ExpectLocalTime(7, 0, 1);
		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_DAYTIME, m_EnvManager.GetCurrentTime());

		// 18�ö� TIME_SUNSET���� �ٲ�
		m_EnvManager.ExpectLocalTime(18, 0, 1);
		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_SUNSET, m_EnvManager.GetCurrentTime());

		// 21�ö� TIME_NIGHT���� �ٲ�
		m_EnvManager.ExpectLocalTime(21, 0, 1);
		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_NIGHT, m_EnvManager.GetCurrentTime());

	}

	TEST_FIXTURE(FEnvManager, TestEnvManager_CheckGameTimeHour)
	{
		const float ENV_TICK = 1.1f;
		Init(3600);	// ���� �ð��� ����..

		m_EnvManager.ExpectLocalTime(0, 0, 0);
		int nCurrentGameTimeHour = m_EnvManager.CheckGameTimeHour();
		CHECK_EQUAL(0, nCurrentGameTimeHour);

		m_EnvManager.ExpectLocalTime(1, 0, 0);
		nCurrentGameTimeHour = m_EnvManager.CheckGameTimeHour();
		CHECK_EQUAL(1, nCurrentGameTimeHour);

		m_EnvManager.ExpectLocalTime(17, 0, 0);
		nCurrentGameTimeHour = m_EnvManager.CheckGameTimeHour();
		CHECK_EQUAL(17, nCurrentGameTimeHour);

		m_EnvManager.ExpectLocalTime(17, 30, 0);
		nCurrentGameTimeHour = m_EnvManager.CheckGameTimeHour();
		CHECK_EQUAL(17, nCurrentGameTimeHour);

	}

	TEST_FIXTURE(FEnvManager, TestEnvManager_CheckGameTimeHour_ByChangeTimeForced)
	{
		const float ENV_TICK = 1.1f;
		Init(3600);	// ���� �ð��� ����..

		m_EnvManager.ExpectLocalTime(10, 0, 0);
		m_EnvManager.ChangeTimeForced(TIME_DAWN);	// 10�ð� TIME_DAWN ����

		CHECK_EQUAL(4, m_EnvManager.CheckGameTimeHour());		// Dawn�� 4:00 am����

		m_EnvManager.ExpectLocalTime(20, 0, 0);					// 10�ð� �Ĵϱ� 10+4
		CHECK_EQUAL(14, m_EnvManager.CheckGameTimeHour());

		m_EnvManager.ExpectLocalTime(3, 0, 0);					// 17�ð� �Ĵϱ� 17+4
		CHECK_EQUAL(21, m_EnvManager.CheckGameTimeHour());


		// --
		m_EnvManager.ExpectLocalTime(3, 0, 0);
		m_EnvManager.ChangeTimeForced(TIME_SUNSET);	// 18�ð� TIME_SUNSET ����

		CHECK_EQUAL(18, m_EnvManager.CheckGameTimeHour());	// Sunset�� 6:00 pm����

		m_EnvManager.ExpectLocalTime(4, 0, 0);
		CHECK_EQUAL(19, m_EnvManager.CheckGameTimeHour());

		m_EnvManager.ExpectLocalTime(5, 0, 0);
		CHECK_EQUAL(20, m_EnvManager.CheckGameTimeHour());

		m_EnvManager.ExpectLocalTime(6, 0, 0);
		CHECK_EQUAL(21, m_EnvManager.CheckGameTimeHour());

		m_EnvManager.ExpectLocalTime(7, 0, 0);
		CHECK_EQUAL(22, m_EnvManager.CheckGameTimeHour());

		m_EnvManager.ExpectLocalTime(2, 0, 0);
		CHECK_EQUAL(17, m_EnvManager.CheckGameTimeHour());

	}

	TEST_FIXTURE(FEnvManager, TestEnvManager_ChangeTimeForced)
	{
		const float ENV_TICK = 1.1f;

		Init(3600);	// ���� �ð��� ����..

		m_EnvManager.ExpectLocalTime(5, 0, 0);
		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_DAWN, m_EnvManager.GetCurrentTime());

		// ������ �ð� ���� - 5������ SUNSET���� ����
		m_EnvManager.ChangeTimeForced(TIME_SUNSET);
		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_SUNSET, m_EnvManager.GetCurrentTime());
		CHECK_EQUAL(18, m_EnvManager.GetCurrentTimeHour());

		m_EnvManager.ExpectLocalTime(6, 0, 0);
		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_SUNSET, m_EnvManager.GetCurrentTime());
		CHECK_EQUAL(19, m_EnvManager.GetCurrentTimeHour());
		

		// night
		m_EnvManager.ExpectLocalTime(5+3, 0, 0);
		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_NIGHT, m_EnvManager.GetCurrentTime());

		// dawn
		m_EnvManager.ExpectLocalTime(5+3+7, 0, 0);
		m_EnvManager.Update(ENV_TICK);
		CHECK_EQUAL(TIME_DAWN, m_EnvManager.GetCurrentTime());

	}
}
