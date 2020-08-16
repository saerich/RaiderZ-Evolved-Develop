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

#define APP_CLASS_NAME					"LoginServer"
#define APP_NAME						"LoginServer"
#define APPLICATION_WIDTH				500
#define APPLICATION_HEIGHT				700

#define DEFAULT_SERVER_NAME				"LoginServer"


#define PATH_DATA						"./data/"


#define FILENAME_CONFIG					"./server.ini"
#define FILENAME_CONFIG_REL				"./server_rel.ini"
#define FILENAME_PROFILE				"profile.txt"
#define FILENAME_LOG					"mlog.txt"

#define DEFAULT_UPDATE_TICK			20			///< ������Ʈ ƽ
#define MAX_TICK_DELTA				(1.0f)		///< �ƹ��� ���� �ɷ��� delta���� 1�ʸ� �ʰ����� �ʴ´�.
#define APP_SERVERVIEW_UPDATE_TICK	(1.0f)		// ����â�� �����ִ� ���� ���� ƽ(�ϴ� 1��)

