#include "stdafx.h"
#include "SCrashReporter.h"
#include <ShellAPI.h>
#include "MCrashReporter.h"
#include "SConfig.h"
#include "MLocale.h"

std::wstring SCrashReporter::m_strProgName;
bool SCrashReporter::m_bReportLog = true;
wstring SCrashReporter::m_strDumpDestServerIP = L"icecream";
int SCrashReporter::m_nDumpDestServerPort = 21;

void SCrashReporter::ExecuteBAReport(const wchar_t* szProgName, const wchar_t* szFileName, bool bReportLog, wstring strServerIP/*="icecream"*/, int nServerPort/*=21*/ )
{
	size_t converted;
	char app_name[64];
	char host_name[128];
	char file_name[1024];

	wcstombs_s(&converted, app_name, szProgName, _TRUNCATE);
	wcstombs_s(&converted, host_name, strServerIP.c_str(), _TRUNCATE);
	wcstombs_s(&converted, file_name, szFileName, _TRUNCATE);
	
	MCrashReporter reporter(app_name, host_name, nServerPort, "raiderz", "ihatebug", SConfig::m_isCrashDumpAutoAgree);
	vector<MCrashReportFilePair> to_report;

	to_report.push_back(MCrashReportFilePair(file_name, reporter.GetRemotePrefix() + ".dmp"));
	if (bReportLog)
	{
		// ���� �������� mlog.txt�� �������� �Ǿ� �ִµ�, ������ �αװ� ����Ǵ� ��δ� Log ���丮 �Ʒ��̸� ������ non-debug ���忡���� �Ͻñ��� ���� �̸��� ��ϵǾ� �ֽ��ϴ�.
		// ����ư ���� �ǵ��� �״�� ���Ӵϴ�.
		to_report.push_back(MCrashReportFilePair("mlog.txt", reporter.GetRemotePrefix() + "_mlog.txt"));
	}

	reporter.Report(to_report, false);
}

void SCrashReporter::Init( const wchar_t* szProgName, bool bReportLog/*=true*/, wstring strServerIP/*="icecream"*/, int nServerPort/*=21*/ )
{
	m_strProgName = szProgName;
	m_strDumpDestServerIP = strServerIP;
	m_nDumpDestServerPort = nServerPort;
}

void SCrashReporter::CrashDumpHandler( const char* szFileName )
{
	ExecuteBAReport(m_strProgName.c_str(), MLocale::ConvAnsiToTCHAR(szFileName).c_str(), m_bReportLog, m_strDumpDestServerIP, m_nDumpDestServerPort);
}
