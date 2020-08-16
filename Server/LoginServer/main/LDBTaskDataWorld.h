#pragma once

struct DBTASK_WORLD_INFO
{
	int		nID;
	wstring	strName;
	wstring strIP;
	int		nType;					// ����. Ư�� ���� ���� ���п�. 0:�Ϲ�
	int		nCurrentPlayerCount;
	int		nMaxPlayerCount;
	bool	isServable;				// ���� ���� ����
	bool	isExpire;				// ���� ����

	DBTASK_WORLD_INFO()
		: nID(0), nType(0), nCurrentPlayerCount(0), nMaxPlayerCount(0), isServable(false), isExpire(false)
	{
	}
};
