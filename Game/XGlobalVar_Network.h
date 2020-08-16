#pragma once

#include "CTransData.h"
#include "CSDef.h"

/// �Ǹ� �α��� ����
//struct XPmangLoginInfo
//{
//	bool				isLoginOnPmang;					// �Ǹ����� �α����ϴ��� ����
//	wstring				strLauncherParameter;			// ��ó�� ���޹��� �α��� ���ڿ�
//	wstring				strStatIndex;					// ��ó�� ���� ��� �ڵ�		
//	bool				isAdult;						// 18�� �̻󿩺�
//	int					nPCCafeID;
//
//	XPmangLoginInfo() : isLoginOnPmang(false), isAdult(false), nPCCafeID(0) 
//	{
//	}
//};


/// ���������� ����ϴ� ��Ʈ��ũ ����
struct GlobalVar_Network
{
	bool				bDirectLogin;					// �ٷ� �α��Ҳ���
	bool				bWaitDirectLogin;				// �ٷ� �α��� ������ - �����˾��� ���ִ� ������ ���
	bool				bIsAlreadyDirectLogined;		// �̹� �ٷ� �α����� �ѹ� �ߴ��� üũ
	bool				bDirectSelChar;					// �ٷ� ĳ���� �����Ұ���
	wchar_t				szLoginID[64];					// �ٷ� �α��� �� ����� �α� ID
	wchar_t				szPassword[64];					// �ٷ� �α��� �� ����� �н�����
	wchar_t				szIP[64];						// �ٷ� �α��� �� ����� IP	- ���� IP
	bool				bLoginToGameServer;				// �α��� ������ �������� �ʰ� ���Ӽ����� ������ �� ����

	wchar_t				szStartZone[128];				// �ٷ� �α��� �� ���� ��
	vec3				vStartPos;						// �ٷ� �α��� �� ���� ��ġ
	vec3				vStartDir;						// �ٷ� �α��� �� ���� ����
	int					nDirectLoginFieldID;			// �ٷ� �α��� �� ������ �ʵ� ID
	wstring				strSelChar;						// �ٷ� �α��� �� ������ ĳ���� �̸�
	wstring				szWorldName;					// �ٷ� �α��� �� ������ ���� �̸�

	CSServerMode		nServerMode;					// ���� ���� ���
	bool				bForceServerModeNormal;			// ����Ÿ ������ ����ϴ�.

	//XPmangLoginInfo		PmangInfo;						// �Ǹ� �α��� ����
	
	vector<wstring>		vecLocatorIPList;				// �������� IP ���
	vector<CSWorldInfo>	vecWorldInfo;					// ���� ���
	

	GlobalVar_Network()
	{
		bDirectLogin = false;
		bWaitDirectLogin = false;
		bIsAlreadyDirectLogined = false;

		bDirectSelChar = false;
		szLoginID[0] = szPassword[0] = szIP[0] = 0;
		bLoginToGameServer = true;
		nDirectLoginFieldID = -1;	
		szStartZone[0] = 0;
		nServerMode = SERVER_MODE_NORMAL;
		bForceServerModeNormal = false;
	}

	bool IsExistDirectLoginFieldInfo()
	{
		return (0 < nDirectLoginFieldID);
	}

	void ResetDirectLoginFieldInfo()
	{
		nDirectLoginFieldID = -1;
		vStartPos = vec3(0,0,0);
	}

	bool IsServerModeExpo()
	{
		return (nServerMode == SERVER_MODE_EXPO);
	}
};
