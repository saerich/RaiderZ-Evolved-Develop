#pragma once

#include "GFrameworkLib.h"
#include "MCommandTable.h"
#include "CCommandTable.h"
#include "CCommandTable_Login.h"
#include "CCommandTable_Master.h"
#include "CCommandTable_App.h"
#include "SMinetInitScope.h"

// Ŀ�ǵ� ���� ////////////////////////////////////////////////////////////////
// Ŀ�ǵ� ��ȣ 20001 ~ 30000 ���� ���

// ���� ///////////////////////////////////////////////////////////////////////

// ���μ����� ���
enum GCommandTable
{
//	MC_LOCAL_GAME_START						= 20001,	///< �÷��̾ ���� �����Ѵ�
//	MC_LOCAL_PLAYER_LOGOUT					= 20003,	///< �÷��̾� �α׾ƿ�

};

GFRAMEWORK_API void GAddCommandTable();


class GMinetInitScope : public SMinetInitScope
{
public:
	GMinetInitScope() : SMinetInitScope()
	{
		GAddCommandTable();
	}
};

