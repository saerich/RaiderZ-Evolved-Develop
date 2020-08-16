#pragma once

// MProfiler2�� AutoTest�� ����
//	��������: �߰� �ν��Ͻ��� MPROFILEITEM�� 10000�� �� ��������� ���ÿ����÷ο� �߻� -_-;

class GGameServerProfiler
{
private:
	friend void MInitProfile(bool bEnabled, bool _bRecordData);

	static const int	MAX_RECORD_SAMPLING = 10000;
	static const int	MAX_PROFILE_COUNT = 10000;

	struct MPROFILEITEM 
	{
		char szName[256];
		DWORD dwStartTime;
		DWORD dwTotalTime;
		DWORD dwMinTime;
		DWORD dwMaxTime;
		DWORD dwCalledCount;
		DWORD* arrayElapsedTimeData;
		int		nElapsedTimeDataIndex;
		double	nElapsedTimeStandardDeviation;		// ��� �ð� ǥ�� ����
	};

	bool				m_bEnabled;
	DWORD				m_dwEnableTime; // init���� enable�� Ȱ��ȭ���� �ð�
	vector<MPROFILEITEM>	m_vecProfileImtems;
	std::set<int>		m_setLineWhenAnalysis;
	// by pok
	bool				m_bRecordSampling;
	int					m_nMaxElapsedTimeDataIndex;

	DWORD				GetNowTime();
	void PutLine( FILE * file );
	void SaveSampling( const char* filename );
public:
	GGameServerProfiler();
	~GGameServerProfiler();

	void Init(bool bEnabled=true, bool bRecord = false);
	void Reset();
	void FinalAnalysis(const char* filename);

	void SetEnabled(bool bEnabled)	{ m_bEnabled = bEnabled; }
	void ResetSampling();		///< ��� �ð� ���ø� ����Ÿ �ʱ�ȭ

	void BeginProfile(int nIndex, const char* szName);
	void EndProfile(int nIndex);
	void AddLineForAnalysis(int nIndex);

	DWORD GetTotalTimeByIndex(int nIndex);
	float GetFrameByIndex(int nIndex);
};
