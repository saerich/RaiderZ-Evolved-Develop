#ifndef _XCOMMAND_TABLE_H
#define _XCOMMAND_TABLE_H

#include "MCommandTable.h"
#include "CCommandTable.h"
#include "CCommandTable_Login.h"

// Ŀ�ǵ� ���� ////////////////////////////////////////////////////////////////
// Ŀ�ǵ� ��ȣ 30001 ~ 40000 ���� ���

enum XCommandTable
{

	MC_LOCAL_RELOAD_UI				= 30001,		///< UI ���ε�
	MC_LOCAL_CHANGE_GAMEFRAME,						///< ���� ������ ��ȯ


	// �׽�Ʈ ����
	MC_LOCAL_TEST_CHANGE_ENV,						///< ȯ�� ����
	MC_LOCAL_TEST_SELF_MOTION_FACTOR,				///< �� ĳ���� ��� ���� �׽�Ʈ

};



void XAddCommandTable();

#endif