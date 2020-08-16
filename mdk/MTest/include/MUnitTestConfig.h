#pragma once


#define UNITTEST_CONFIG_FILTER_ITEM_COUNT	4

/// ������ ������
class MUnitTestConfig
{
private:
	bool GetPrivateProfileBool(const char* szAppName, const char* szKeyName, bool bDefault, const char* szFileName);
	void GetFilter( const char* szName, string (&outString)[UNITTEST_CONFIG_FILTER_ITEM_COUNT], const char* filename );

	bool IsExistFile(const char *filename);
public:
	MUnitTestConfig();

	bool Load();

	bool m_bExecuteUnitTest;										///< ���� ���� �����ϱ� ���� ���� �׽�Ʈ�� �������� ����
	string m_strStageFilter;										///< ���� �׽�Ʈ�� �������� (��ü�̸� all)
	string m_strSuite[UNITTEST_CONFIG_FILTER_ITEM_COUNT];			///< �׽�Ʈ�� Suite, �ƹ��͵� ������ ��� �׽�Ʈ�Ѵ�.
	string m_strDisabledSuite[UNITTEST_CONFIG_FILTER_ITEM_COUNT];	///< �׽�Ʈ���� ���� Suite
	string m_strTest[UNITTEST_CONFIG_FILTER_ITEM_COUNT];			///< Ư�� �׽�Ʈ�� �׽�Ʈ�� �� �׽�Ʈ �̸�
	string m_strSuiteAnyWords;										///< Suite �̸��߿� �� �ܾ ������ �׽�Ʈ�Ѵ�.
	string m_strCodeCoverageFilter;				///< �ڵ� Ŀ������ üũ ����
	bool m_bBreakOnFailure;						///< �׽�Ʈ ���н�
	int m_nRepeat;								///< �ݺ� ȸ��. 
	bool m_bLogSuiteName;						///< �׽�Ʈ ����� Suite �̸��� �α׷� ������� ����
	bool m_bCheckCodeCoverage;					///< �ڵ� Ŀ������ üũ�� �� �� ����
	bool m_bLogTimeElapsed;						///< �׽�Ʈ�� �ҿ�ð��� ������� ����

	// dbtest
	string	m_strDBServer;
	string	m_strDBName;
	string	m_strDBUserName;
	string	m_strDBPassword;
	bool	m_bExecuteDB;
	string	m_strOdbcDriver;
	
	bool CheckStageFilter(const char* szStageName);
};





