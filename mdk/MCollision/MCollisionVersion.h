#pragma once

struct RHEADER 
{
	DWORD dwID;
	DWORD dwVersion;

	RHEADER() {}
	RHEADER(DWORD id, DWORD version)
		: dwID(id)
		, dwVersion(version)
	{
	}
};

const DWORD R_COL2_ID		= 0x59249834;	// .cl2 ���� ( �浹�� AABBTree )
const DWORD R_COL2_VERSION	= 1;			// save ����, ���� ���� ������ impl Ŭ������ ���
