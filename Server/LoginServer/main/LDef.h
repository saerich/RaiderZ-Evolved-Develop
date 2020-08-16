#pragma once

#include <vector>
#include <set>
#include <map>
#include <string>
#include "MTypes.h"
#include "MMath.h"
#include "MiNet.h"
#include "CSDef.h"

using namespace std;
using namespace minet;

#define APP_CLASS_NAME					L"LoginServer"
#define APP_NAME						L"LoginServer"
#define APPLICATION_WIDTH				500
#define APPLICATION_HEIGHT				700

#define DEFAULT_SERVER_NAME				L"LoginServer"
#define DEFAULT_MAX_PLAYER				1000			///< �⺻ ���� ����������

#define PATH_DATA						L"./data/"


#define FILENAME_CONFIG					L"./server.ini"
#define FILENAME_CONFIG_REL				L"./server_rel.ini"
#define FILENAME_PROFILE				L"profile.txt"
#define FILENAME_LOG					L"mlog.txt"

#define FILENAME_ABUSE					PATH_DATA L"abuse.txt"
#define FILENAME_GAMEWORDS				PATH_DATA L"gamewords.txt"
#define FILENAME_NAME_ILLEGALWORDS		PATH_DATA L"name_illegalwords.txt"
#define FILENAME_NAME_ALLOWEDCHARS		PATH_DATA L"name_allowedchars.txt"

#define DEFAULT_UPDATE_TICK			20			///< ������Ʈ ƽ
#define MAX_TICK_DELTA				(1.0f)		///< �ƹ��� ���� �ɷ��� delta���� 1�ʸ� �ʰ����� �ʴ´�.
#define APP_SERVERVIEW_UPDATE_TICK	(1.0f)		// ����â�� �����ִ� ���� ���� ƽ(�ϴ� 1��)


namespace INITIAL_ITEM_SET
{
	enum _
	{
		WEAPON_ID	= 1,	// ������ ��
		BODY_ID		= 1011,	// ũ�ҷ��� �߰� ����Ʈ
		LEG_ID		= 1012,	// ũ�ҷ��� �߰� ����
		HANDS_ID	= 1013,	// ũ�ҷ��� �߰� ��Ʋ��
		FEET_ID		= 1014	// ũ�ҷ��� �߰� �׸���
	};
}
