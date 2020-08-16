#include "stdafx.h"
#include "XArgumentsParser.h"
#include "XCrashReporter.h"
#include "XApplicationConfig.h"
#include <iomanip>
#include <regex>


XArgumentsParserResult XArgumentsParser::Parse( const wchar_t* szArgs, const cml2::MCommandLine& commandLine, XGameFrameManager* pGameFrameManager )
{
	XArgumentsParserResult ret;

	// autotest üũ
	{
		//XCrashReporter::bAutoReport = commandLine.HasSwitch("auto_report_crashdump");
	}

	// �Ķ���Ͱ� ���÷��� ���ϸ����� Ȯ���Ѵ�
	if (ParseReplayFile(szArgs, ret))
	{
		return ret;
	}

#ifdef _PUBLISH

#if defined( _KOREA)
	if ( ParseLoginPmang( szArgs, ret))						return ret;
	if ( ParseLogin( szArgs, ret))							return ret;		// (�׽�Ʈ��) ���� ���� ����
#elif defined( _BUYER)
	wstring strArgs;
	strArgs += szArgs;
	strArgs += L" ";
	strArgs += L"218.50.2.254";
	strArgs += L" skip_select_world";					// ���弱�� ��ŵ�ϱ� - DirectLogin
	if ( ParseLogin( strArgs.c_str(), ret))				return ret;
#else
	if ( ParseLogin( szArgs, ret))							return ret;
#endif

#else

	if ( ParseZoneTest( szArgs, ret))						return ret;		// local zone-name posx posy posz dirx diry dirz time weather
	if ( ParseLogin( szArgs, ret))							return ret;
	if ( ParseFrameName( szArgs, pGameFrameManager, ret))	return ret;

#endif

	return ret;
}

wstring XArgumentsParser::ParseFileNameFromArguments( const wchar_t* szArgs )
{
	wchar_t szFileName[MAX_PATH] = L"";
	size_t nLength;

	// ����ǥ ������ �����Ѵ�
	if(szArgs[0]==L'"') 
	{
		wcscpy_s(szFileName,szArgs+1);

		nLength = wcslen(szFileName);
		if(szFileName[nLength-1]==L'"')
		{
			szFileName[nLength-1]=0;
		}
	}
	else
	{
		wcscpy_s(szFileName,szArgs);

		wchar_t seps[] = L" ";
		wchar_t* token;
		wchar_t* nextToken;

		token = wcstok_s(szFileName, seps, &nextToken);
		if(token!=NULL)
		{
			wcscpy_s(szFileName, token);
		}
	}
	return wstring(szFileName);
}

bool XArgumentsParser::ParseReplayFile( const wchar_t* szArgs, XArgumentsParserResult& ret )
{
	wstring strFileName = ParseFileNameFromArguments(szArgs);
	size_t nLength = strFileName.length();

	if(wcsicmp(strFileName.c_str() + nLength - wcslen(FILEEXT_REPLAY),FILEEXT_REPLAY)==0)
	{
		mlog("MODE=REPLAY!!\n");

		ret.nExecuteMode = EM_REPLAY;
		ret.nStartGameFrameID = REPLAY_FRAME_ID;
		ret.strGameFrameArgs = strFileName;

		return true;
	}

	return false;
}

bool XArgumentsParser::CheckPrefix( const wchar_t* szArgs, const wchar_t* szStr )
{
	return (!wcsnicmp(szArgs, szStr, wcslen(szStr)));
}

bool XArgumentsParser::ParseZoneTest( const wchar_t* szArgs, XArgumentsParserResult& ret )
{
#ifdef _PUBLISH
	return false;
#endif

	if (CheckPrefix(szArgs, L"zonetest") == false && 
		CheckPrefix(szArgs, L"local") == false)
	{
		return false;
	}

	mlog("MODE=LOCAL_TEST!!\n");

	ret.nExecuteMode = EM_LOCAL_TEST;
	ret.nStartGameFrameID = PLAY_FRAME_ID;

	rs3::REngine::GetConfig().m_bFullScreen = false;	// ������ â���

	if (wcslen(szArgs) > 5)
	{
		wchar_t temp[256]=L"";
		wchar_t posx[256]=L"";
		wchar_t posy[256]=L"";
		wchar_t posz[256]=L"";
		wchar_t dirx[256]=L"";
		wchar_t diry[256]=L"";
		wchar_t dirz[256]=L"";
		wchar_t time[256]=L"";
		wchar_t weather[256]=L"";

		int nRet = swscanf(szArgs, L"%s %s %s %s %s %s %s %s %s %s", temp, gvar.Network.szStartZone, posx, posy, posz, dirx, diry, dirz, time, weather);
		if (nRet == 10)
		{
			gvar.Network.bDirectLogin = true;
			gvar.Network.bDirectSelChar = true;
			gvar.Network.vStartPos.x = _wtof(posx);
			gvar.Network.vStartPos.y = _wtof(posy);
			gvar.Network.vStartPos.z = _wtof(posz);
			gvar.Network.vStartDir.x = _wtof(dirx);
			gvar.Network.vStartDir.y = _wtof(diry);
			gvar.Network.vStartDir.z = _wtof(dirz);

			gvar.World.SetNextMapInfo(gvar.Network.vStartPos, gvar.Network.vStartDir);

			gvar.World.nGameTime = (GAME_TIME_TYPE)_wtoi(time);
			gvar.World.nWeather = (GAME_WEATHER_TYPE)_wtoi(weather);
		}
		else if (nRet == 2)
		{
			gvar.Network.bDirectLogin = true;
			gvar.Network.bDirectSelChar = true;
			gvar.Network.vStartPos.x = 0.0f;
			gvar.Network.vStartPos.y = 0.0f;
			gvar.Network.vStartPos.z = 0.0f;
			gvar.Network.vStartDir.x = 0.0f;
			gvar.Network.vStartDir.y = 1.0f;
			gvar.Network.vStartDir.z = 0.0f;

			gvar.World.SetNextMapInfo(gvar.Network.vStartPos, gvar.Network.vStartDir);

			gvar.World.nGameTime = TIME_DAWN;
			gvar.World.nWeather = WEATHER_SUNNY;
		}
	}

	gvar.MyInfo.MyUID = MUID(345,234234);
	gvar.MyInfo.ChrStatus.nHP = PLAYER_DEFAULT_HPMAX;

	return true;
}

bool XArgumentsParser::ParseLogin(const wchar_t* szArgs, XArgumentsParserResult& ret)
{
	//return ParseLoginOld(L"launch admin 127.0.0.1", ret);

	ret.nExecuteMode = EM_GAME;
	ret.nStartGameFrameID = LOGIN_FRAME_ID;

	int len = wcslen(szArgs);
	wchar_t szIP[256] = L"";
	wchar_t szUser[256] = L"", szPW[256] = L"";

	if (wcslen(szArgs) > 5)
	{
		wchar_t temp[256] = L"";

		int ret = swscanf(szArgs, L"%s %s %s", szIP, szUser, szPW);

		if (ret == 1)
		{
			wcsncpy_s(gvar.Network.szIP, szIP, _TRUNCATE);
			gvar.Network.vecLocatorIPList.push_back(szIP);
			gvar.Network.bDirectLogin = false;
			gvar.Network.bDirectSelChar = false;
		}
		else if (ret == 3)
		{
			wcsncpy_s(gvar.Network.szIP, szIP, _TRUNCATE);
			gvar.Network.vecLocatorIPList.push_back(szIP);
			gvar.Network.bDirectLogin = true;
			gvar.Network.bDirectSelChar = false;
			wcsncpy_s(gvar.Network.szLoginID, szUser, _TRUNCATE);
			wcsncpy_s(gvar.Network.szPassword, szPW, _TRUNCATE);
		}
	}

	gvar.Network.bLoginToGameServer = false;

	return true;
}

bool XArgumentsParser::ParseLoginOld( const wchar_t* szArgs, XArgumentsParserResult& ret )
{
	// ����Ÿ ������ ����ϴ�.
	if (CheckPrefix(szArgs, L"notgstar"))
	{
		gvar.Network.bForceServerModeNormal = true;
	}

	if (CheckPrefix(szArgs, L"login") == false && 
		CheckPrefix(szArgs, L"launch") == false &&
		CheckPrefix(szArgs, L"notgstar") == false &&		// ����Ÿ ������ ����ϴ�.
		CheckPrefix(szArgs, L"standalone") == false
		)
	{
		return false;
	}

	ret.nExecuteMode = EM_GAME;
	ret.nStartGameFrameID = LOGIN_FRAME_ID;

	int len = wcslen(szArgs);
	wchar_t szIP[256]=L"";
	
	if (wcslen(szArgs) > 5)
	{
		wchar_t temp[256]=L"";

		int ret = swscanf(szArgs, L"%s %s %s %d %f %f %f", temp, gvar.Network.szLoginID, szIP,
			&gvar.Network.nDirectLoginFieldID, &gvar.Network.vStartPos.x, &gvar.Network.vStartPos.y, &gvar.Network.vStartPos.z);

		if (ret == 3)
		{
			gvar.Network.bDirectLogin = false;

			wchar_t szWorldName[256] = L"";
			int ret = swscanf(szArgs, L"%s %s %s %s", temp, gvar.Network.szLoginID, szIP, szWorldName);

			if (ret == 4)
			{
				gvar.Network.bDirectLogin = true;				
				gvar.Network.szWorldName = szWorldName;				
			}
		}
		else if (ret == 7)
		{
			gvar.Network.bDirectLogin = true;
			gvar.Network.bDirectSelChar = true;
		}

		// IP�� ��Ͽ� �߰�
		wcsncpy_s(gvar.Network.szIP, szIP, _TRUNCATE);
		gvar.Network.vecLocatorIPList.push_back(szIP);
	}


	// ���� ����Ÿ�� ����
	if (CheckPrefix(szArgs, L"launch"))				// LoginServer
	{
		gvar.Network.bLoginToGameServer = false;
	}
	else if (CheckPrefix(szArgs, L"standalone"))		// GameServer
	{
		gvar.Network.bLoginToGameServer = true;
	}
	else												// Auto Detect
	{			
		if (wcsicmp(szIP, L"localhost") && wcsicmp(szIP, L"127.0.0.1"))
		{
			gvar.Network.bLoginToGameServer = false;
		}
	}

	return true;
}

bool XArgumentsParser::ParseFrameName( const wchar_t* szArgs, XGameFrameManager* pGameFrameManager, XArgumentsParserResult& ret )
{
#ifdef _PUBLISH
	return false;
#endif

	if (!pGameFrameManager) return false;


	// ���ڸ� Frame Name���� üũ�ؼ� ��ġ�ϴ� �������� ������ �ش� ���������� �ٷ� �̵�

	if (wcslen(szArgs) > 0)
	{
		wchar_t args[1024] = L"";
		wcscpy_s(args, szArgs);

		wchar_t* token;
		wchar_t seps[] = L" ,\t";
		token = wcstok(args, seps);
		if ((token != NULL) && (wcslen(token) > 0))
		{
			XBaseFrame* pFrame = pGameFrameManager->GetFrameFromName(token);
			if (pFrame)
			{
				int nFrameID = pFrame->GetID();

				mlog("MODE=%s\n", MLocale::ConvUTF16ToAnsi(token).c_str());
				ret.nExecuteMode = EM_CUSTOM_FRAME;
				ret.nStartGameFrameID = XGameFrameID(nFrameID);
				ret.strGameFrameArgs = szArgs;

				return true;
			}
		}
	}

	return false;
}

//bool XArgumentsParser::ParseLoginPmang( const wchar_t* szArgs, XArgumentsParserResult& ret )
//{
//	enum TOKEN
//	{
//		COMMAND_LINE = 0,
//		LOGINSTREAM_FRONT,
//		USER_ID,
//		TIME_STAMP,
//		NEOWIZ_PCBID,				
//		LOGINSTREAM_REAR,
//		REGION_CODE,
//		AGE_CODE,
//		GENDER_CODE,		
//		YEAR_OF_BIRTH,
//
//		MAX_TOKEN
//	};
//
//	// -key"PmangLoginStream"
//	wregex pattern(
//						L"-key\""						// �Ǹ� �Ķ���� ���� ǥ��. version.ini ������ ���� ���� ���ſ���
//							L"("						// LOGINSTREAM_FRONT
//								L"\\d+"					// �����ø���
//								L"\\|"
//								L"([^| ]{1,12})"		// USER_ID
//								L"\\|"
//								L"(\\d+)"				// TIME_STAMP							
//								L"\\|"
//								L"(?:_|([^| ]{1,12}))"	// PC�� 
//								L"(?:\\|[^| ]+)*"		// ������, ä�θ�								
//								L"\\|"
//								L"[a-f\\d]{32}"			// MD5 �ؽ�
//								L" "
//							L")"
//							L"("						// LOGINSTREAM_REAR
//								L"(\\d{2})"				// REGION_CODE
//								L"(\\d{1})"				// AGE_CODE
//								L"(\\d{1})"				// GENDER_CODE
//								L"(\\d{4})"				// YEAR_OF_BIRTH
//							L")"
//						L"\""
//		);
//
//
//	match_results<wstring::const_iterator> result;
//	wstring strCommandLine = szArgs;
//
//	if (!regex_match(strCommandLine, result, pattern))
//	{
//		return false;
//	}
//
//	if (MAX_TOKEN != result.size())
//	{
//		return false;
//	}
//
//	gvar.Network.PmangInfo.strLauncherParameter = result[LOGINSTREAM_FRONT];
//
//
//	// StatIndex ������. RegionCode�� �Ǹ����� ��ӵ� ������ 1 ū���� ���´�. -1 �ؼ� �ٽ� �����Ѵ�.
//	int nRegionCode = _wtoi(wstring(result[REGION_CODE]).c_str());
//	int nAgeCode = _wtoi(wstring(result[AGE_CODE]).c_str());
//	int nGenderCode = _wtoi(wstring(result[GENDER_CODE]).c_str());
//	int nYearOfBirth = _wtoi(wstring(result[YEAR_OF_BIRTH]).c_str());
//
//	nRegionCode = max(0, nRegionCode - 1);
//
//	// RegionCode�� ũ��, �ؿ�(REGION_MAX)�� �����Ѵ�.
//	const int REGION_MAX = 16;
//	if (REGION_MAX < nRegionCode) nRegionCode = REGION_MAX;
//
//
//	wchar_t szStatIndex[16];
//	wsprintf(szStatIndex, L"%02d%d%d%04d", nRegionCode, nAgeCode, nGenderCode, nYearOfBirth);
//
//	gvar.Network.PmangInfo.strStatIndex = szStatIndex;
//	gvar.Network.PmangInfo.strLauncherParameter += szStatIndex;
//
//
//	// PC�濩�� �Ľ�
//	int nPCCafeID = _wtoi(wstring(result[NEOWIZ_PCBID]).c_str());
//	gvar.Network.PmangInfo.nPCCafeID = nPCCafeID;
//
//
//	// ����Ȯ��, �Ķ���Ϳ� ���Ե� �¾ �ؿ� Ÿ�ӽ������� �������� ����Ѵ�.
//	time_t timestamp = _wtoi(wstring(result[TIME_STAMP]).c_str());
//	tm* tmLocalTime = localtime(&timestamp);
//
//	if (nYearOfBirth + 18 <= tmLocalTime->tm_year + 1900)
//	{
//		gvar.Network.PmangInfo.isAdult = true;		
//	}
//	else
//	{
//		gvar.Network.PmangInfo.isAdult = false;
//	}
//
//
//	// �Ǹ� �α��� ����
//	wcsncpy_s(gvar.Network.szLoginID, wstring(result[USER_ID]).c_str(), _TRUNCATE);
//	gvar.Network.PmangInfo.isLoginOnPmang = true;
//
//	ret.nExecuteMode = EM_GAME;
//	ret.nStartGameFrameID = LOGIN_FRAME_ID;	
//
//	gvar.Network.bDirectLogin = true;
//	gvar.Network.bLoginToGameServer = false;
//
//
//	// IP ����
//	gvar.Network.vecLocatorIPList = XApplicationConfig::m_vecIPString;
//	
//	if (!gvar.Network.vecLocatorIPList.empty())
//	{
//		// ���� ���� IP�� ����� ù��° IP�� �ʱ�ȭ.
//		wstring strIP = gvar.Network.vecLocatorIPList.front();
//		wcsncpy_s(gvar.Network.szIP, strIP.c_str(), _TRUNCATE);
//	}
//
//
//	return true;
//}
