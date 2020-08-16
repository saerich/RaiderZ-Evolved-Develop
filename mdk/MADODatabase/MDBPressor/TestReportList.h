#pragma once

/*
 *	@brief	���� ����� ���� �α׷� ����ϸ�,
 *			�α� ������ ������ �Űܼ� ,(�޸�)�� �÷��� �з��� �� �ִ�.
 */

class TestReportList
{
public:
	TestReportList( CListCtrl* pListCtrl );
	virtual ~TestReportList();

	BOOL				Init();

	void				OutputTestSummaryHeader();
	void				OutputTestSummary(CString strDBIP, CString strCatalog, CString strPoolSize, CString strQueryString);

	void				OutputTimerInfoHeader();
	void				OutputTimerInfo(CString& strHour, CString& strMinute, CString& strSecond, CString& strTerm);

	void				OutputTestResultHeader();
	void				OutputTestResult(	int nTestNumber, const char* szStartTime, const char* szEndTime,
											double dElapsedTime, int nSuccess, int nFail, int nCountPerSecond );

protected:

private:
	CListCtrl*			m_pListCtrl;

	void				DumpTestResult();

};
