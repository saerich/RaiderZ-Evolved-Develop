#pragma once

#include "SFieldInfo.h"
#include "CSFieldInfo.h"

enum ZFieldType
{
	FT_SHARED = 0,
	FT_DYNAMIC
};

class ZFieldInfo : public SFieldInfo
{
public:
	ZFieldInfo() : SFieldInfo() {}
	virtual ~ZFieldInfo() {}
};

struct FIELD_GROUP_INFO
{
	FIELD_GROUP_INFO()
		: nID(0)
		, pStartFieldInfo(NULL)
	{
	}

	int					nID;
	ZFieldInfo*			pStartFieldInfo;
	vector<ZFieldInfo*>	vecFieldInfo;
	set<int>			setRunningQuest;	// �ش� �ʵ� �׷쿡�� ����Ǵ� ����Ʈ��
};
