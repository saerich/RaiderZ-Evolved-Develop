#ifndef _G_CONFIG_ARG_H
#define _G_CONFIG_ARG_H

#include "MTypes.h"
#include "GDef.h"
#include "GFrameworkLib.h"

/// �������� Ŀ�ǵ� ���� ����
class GFRAMEWORK_API GConfig_Arg
{
public:
	bool		bAutoTestActive;
	wstring		strAutoTestType;
	float		fLimitFrame;
	int			nRepeat;
	DWORD		dwTime;	
	int			nSpeed;

	GAITestType	nAITestType;
	GCommandRecordingType nCommandRecordingType;
	int			nNPCCount;
	int			nFieldID;
	int			nQPVPEventID;

	bool		bAutoStartCommandProfile;		// Ŀ�ǵ� �������ϸ� ����
	bool		bLimitRunTime;
	DWORD		dwRunTime;						// ���� �ð�(�ð� �� �ڵ� ����)

public:
	GConfig_Arg();
	void Create(const wchar_t* szCmd);	
};

#endif//_G_CONFIG_ARG_H
