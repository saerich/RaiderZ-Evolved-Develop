#include "stdafx.h"
#include "GBaseApplication.h"
#include "GConfig.h"
#include "CAppHelper.h"
#include "GConst.h"
#include "GValidateLogger.h"
#include "GSystem.h"
#include "GPMSSystem.h"
#include "PMServerInitLogger.h"
#include "PMSCodes.h"
#include "SRemoteProfiler.h"
#include "GGameGuard.h"
#include "GTestDynamicField.h"
#include "GAIMonitorPort.h"
#include "GCombatMonitorPort.h"
#include "GStrings.h"
#include "MemPoolObjCounter.h"

GBaseApplication::GBaseApplication()
: SServerApplication()
, m_bDoValidate(true)
, m_bDrawValidatorAlert(false)
{
	
}

bool GBaseApplication::OnCreate( )
{
	m_Systems.Create();
	m_Managers.Create();

	if (GStrings::Init() == false)
	{
		mlog3("Failed InitStrings\n");
		return false;
	}

	/// ��� �ʱ�ȭ
	gsys.pScriptManager->Init(true);
	GConst::Init(WLUA);

	m_ConfigLua.Create();
	GConfig::Init_Lua(m_ConfigLua);

	wstring strCmdLine = MLocale::ConvAnsiToUCS2(GetCommandLine().GetCommandLineString().c_str());
	mlog("commandline: %s\n", MLocale::ConvUTF16ToAnsi(strCmdLine.c_str()).c_str());
	
	m_ConfigArg.Create(strCmdLine.c_str());	
	GConfig::Init_Arg(m_ConfigArg);

	InitValidate();

	InitCommandLine();

	if (GConfig::m_bGGEnable)
	{
		if (!gsys.pGameGuard->Init())
		{
			SetServerInitResult(SERVERINIT_FAILED_INIT_GAMEGUARD);
		}
	}

	_RunRuntimeValidator();

	const wstring strServerVersion = _MakeServerVersion();

	if (false == gsys.pServer->Create(strServerVersion, !GConfig::m_bNoDB))
		return false;

	if (false == GConfig::m_bLogPoolCount)
	{
		MemPoolObjCounter::Instance().StopChecking();
	}

	ValidateResouces();

	if (RunAutoTest() == false)
	{
		SetServerInitResult(SERVERINIT_FAILED_DATAFILE_LOAD);
	}

	if (IsRunForTest())
	{
		MInitStatisticsProfile(GConst::AUTO_PROFILE);
		StartServerMonitorEventListener(GConfig::m_strServerName.c_str());
	}

	// ���� �۵� ���ѽð� ����; �������ϵ� �׽�Ʈ�� ���� ��
	m_LimitRunTimeRegulator.Stop();
	if (GConfig::m_bLimitRunTime)
	{
		m_LimitRunTimeRegulator.SetTickTime((float)GConfig::m_dwRunTime);
		m_LimitRunTimeRegulator.Start();
	}

	if (GConfig::m_bPMSEnable)
	{
		DWORD nRet = 
			gsys.pPMSSystem->Connect(strCmdLine);

		if (nRet != PMSC_ERR_OK)
		{
			mlog3("PMS Connect Failed. (code: %d)\n", nRet);
			return false;
		}

		if (::IsServerInitFailed())
		{
			gsys.pPMSSystem->SendWarningMsg();
		}
	}

	if (::IsServerInitFailed())
	{
		mlog3("Server Init Failed: %s\n", MLocale::ConvUTF16ToAnsi(::GetServerInitWarningMsg()).c_str());
		return false;
	}

	return true;
}

void GBaseApplication::OnDestroy()
{
	if (gsys.pServer)
		gsys.pServer->Destroy();

	m_Managers.Destroy();
	m_Systems.Destroy();

	m_AutoTestRunner.Destroy();
}

void GBaseApplication::CloseServer()
{
	if (gsys.pServer != NULL)
	{		
		// ������ �ִٸ�, ������ �����ϵ��� �Ѵ�.
		gsys.pServer->CloseServer();
	}
	else
	{		
		__super::CloseServer();
	}
}

void GBaseApplication::InitValidate()
{
	m_bDoValidate = true;

	// ��ȿ���˻� config Ȯ��
	if ((!GConst::RUN_FOR_TEST) || 
		(!GConst::RUN_RESOURCE_VALIDATOR))
	{
		m_bDoValidate = false;
		return;
	}

	// fieldlist_debug.xml�� ������ ��ȿ�� �˻縦 ���� �ʴ´�.
	wstring strDebugFIeldListPath = wstring(PATH_SYSTEM) + wstring(PURE_FILENAME_FIELDLIST_DEBUG);
	if (MIsExistFile(strDebugFIeldListPath.c_str()))
	{
		m_bDrawValidatorAlert = false;
		if (gsys.pValidateLogger)
		{
			gsys.pValidateLogger->SetLogging(false);
		}

		m_bDoValidate = false;
	}
}

void GBaseApplication::ValidateResouces()
{
	if (m_bDoValidate == false) return;

	m_bDrawValidatorAlert = false;

	// Server resource validator
	bool ret = _ValidateResouces();
	if (ret == false) 
	{
		m_bDrawValidatorAlert = true;
	}
}

void GBaseApplication::InitCommandLine()
{
	if (m_CommandLine.HasSwitch("standalone") || m_CommandLine.HasSwitch("-standalone") || m_CommandLine.HasSwitch("/standalone"))
	{
		GConfig::m_bStandAlone = true;
	}
}

bool GBaseApplication::RunAutoTest()
{
	if (IsRunForTest())
	{		
		if (true == GConfig::m_bAutoTestActive)
		{
			mlog("Begin Autotest... (type: %s, duration: %d second(s))\n", 
				MLocale::ConvUTF16ToAnsi(GConfig::m_strAutoTestType.c_str()).c_str(), GConfig::m_dwTime);

			if (false == m_AutoTestRunner.Create()) return false;
		}
	}

	return true;
}

bool GBaseApplication::OnUpdate(float fDelta)
{
//#ifdef _USE_PROFILER
	{
		// ����Ʈ �������Ϸ����� ����
		SRemoteProfiler::Instance().SendToProfiler();
		GAIMonitorPort::Inst().Update(fDelta);
		GCombatMonitorPort::Inst().Update(fDelta);
	}
//#endif

	PFC_THISFUNC;

	if (IsRunForTest())
	{
		if (GConst::DEBUG_GAME_TICK_MOD_SPEED != 1.0f &&
			 GConst::DEBUG_GAME_TICK_MOD_SPEED > 0.0f)
		{
			fDelta *= GConst::DEBUG_GAME_TICK_MOD_SPEED;
		}
	}

	if (false == GConfig::m_bAutoTestActive)
	{
		PFI_B(10, "GApplication::OnUpdate - GServer:Update");
		if (!gsys.pServer->Update(fDelta))
			return false;
		PFI_E(10);
	}
	else
	{
		if (!UpdateAutoTest(fDelta))
		{
			return false;
		}
	}
	

	GTestDynamicField::Instance().Update(fDelta);

	// ���� �۵� ���ѽð��� �ִٸ�, ó��
	if (m_LimitRunTimeRegulator.IsReady(fDelta))
	{
		_Quit(0);
	}

	return true;
}

void GBaseApplication::LogCommand( const wchar_t* szCmd )
{
	wchar_t szMsg[512]= L"";
	wchar_t szTime[256] = L"";

	//�ð�
	SYSTEMTIME stCurTime;
	GetLocalTime(&stCurTime);

	swprintf_s(szTime, L"[%02d:%02d:%02d.%03d]:"
		, stCurTime.wHour
		, stCurTime.wMinute
		, stCurTime.wSecond
		, stCurTime.wMilliseconds);

	swprintf_s(szMsg, L"%s %s\n", szTime, szCmd);

	_LogCommand(szCmd);
}

bool GBaseApplication::UpdateAutoTest(float fDelta)
{
	if (IsRunForTest())
	{
		if (!m_AutoTestRunner.Update(fDelta))
		{
			return false;
		}


		// ���� ������ ���� - ���� ������ �Ϸ�ɶ����� ������Ʈ ���ƾ��Ѵ�.
		static bool isEndAutoTest = false;
		
		if (!isEndAutoTest && m_AutoTestRunner.IsExit())
		{
			int nRetCode = 0;
			
			if (m_AutoTestRunner.IsFailure() == true)
			{
				nRetCode = 1;
			}

			_Quit(nRetCode);	

			isEndAutoTest = true;
		}
	}

	return true;
}

void GBaseApplication::StartProfiling()
{
	if (IsRunForTest())
	{
		MInitStatisticsProfile(true);
		MResetStatisticsProfile();
	}
}

void GBaseApplication::AnalysisProfiling()
{
	MSaveStatisticsProfile(MLocale::ConvUTF16ToAnsi(FILENAME_PROFILE).c_str(), false);
}

void GBaseApplication::OnMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	__super::OnMsg(hWnd, uMsg, wParam, lParam);

	if (GConst::RUN_FOR_TEST)
	{
		if (uMsg != WM_COPYDATA)
			return;

		COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
		if (!pCopyData)
			return;

		if(pCopyData->dwData == 1001)
		{
			GCombatMonitorPort::Inst().OnMsg((PCOPYDATASTRUCT) lParam);
		}
	}
}
