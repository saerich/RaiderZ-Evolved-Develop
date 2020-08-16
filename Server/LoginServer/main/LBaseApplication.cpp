#include "stdafx.h"
#include "LBaseApplication.h"
#include "SCommandTable_App.h"
#include "LServer.h"
#include "MProfiler.h"
#include "LConfig.h"
#include "MStatisticsProfiler.h"
//#include "LPMSSystem.h"
//#include "PMSCodes.h"
//#include "PMServerInitLogger.h"
#include "MLocale.h"

LBaseApplication::LBaseApplication()
: SServerApplication()
{

}

LBaseApplication::~LBaseApplication()
{

}

bool LBaseApplication::OnCreate()
{
	wstring strCmdLine = MLocale::ConvAnsiToUTF16(GetCommandLine().GetCommandLineString().c_str());
	mlog("application_commandline : %s\n", MLocale::ConvUTF16ToAnsi(strCmdLine.c_str()).c_str());


	AddAppCommandTable();

	m_Systems.Create();
	m_Managers.Create();

	m_Systems.pServer->Create();

#ifndef _PUBLISH
	StartServerMonitorEventListener(LConfig::m_strServerName.c_str());
#endif

	// MrFarbodD to do check this pmang shit
	//if (LConfig::m_bPMSEnable)
	//{
	//	DWORD nRet = gsys.pPMS->Connect(strCmdLine);
	//
	//	if (nRet != PMSC_ERR_OK)
	//	{
	//		mlog3("PMS Connect Failed. (code: %d)\n", nRet);
	//		return false;
	//	}
	//
	//	if (::IsServerInitFailed())
	//	{
	//		gsys.pPMS->SendWarningMsg();
	//	}
	//}

	//if (::IsServerInitFailed())
	//{
	//	mlog3("Server Init Failed: %s\n", MLocale::ConvUTF16ToAnsi(::GetServerInitWarningMsg()));
	//	return false;
	//}

	return true;
}

void LBaseApplication::OnDestroy()
{
	if (m_Systems.pServer)
	{
		m_Systems.pServer->Destroy();
	}

	m_Managers.Destroy();
	m_Systems.Destroy();

}

bool LBaseApplication::OnUpdate(float fDelta)
{
	if (!m_Systems.pServer->Update(fDelta))
		return false;

	return true;
}

void LBaseApplication::AnalysisProfiling()
{
	MSaveStatisticsProfile(MLocale::ConvUTF16ToAnsi(FILENAME_PROFILE).c_str(), false);
}

void LBaseApplication::StartProfiling()
{
#ifdef _PUBLISH
	return;
#endif

	MInitStatisticsProfile(true);
	MResetStatisticsProfile();
}