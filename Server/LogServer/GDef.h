#ifndef _GDEF_H
#define _GDEF_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include "MTypes.h"
#include "MMath.h"

#include "CSDef.h"

using namespace std;
using namespace minet;

#define DEFAULT_SERVER_NAME					"�׽�Ʈ"	///< �⺻ ���� �̸�
#define DEFAULT_MAX_PLAYER			1000				///< �⺻ ���� ����������

#define FILENAME_SERVER_CONFIG		"./server.ini"
#define FILENAME_SERVER_CONFIG_REL	"./server_rel.ini"
#define FILENAME_PROFILE			"profile.txt"
#define FILENAME_LOG				"mlog.txt"

#define DEFAULT_GAME_TICK			20			///< ����ƽ
#define MAX_TICK_DELTA				(1.0f)		///< �ƹ��� ���� �ɷ��� delta���� 1�ʸ� �ʰ����� �ʴ´�.
#define APP_SERVERVIEW_UPDATE_TICK	(1.0f)		///< ����â�� �����ִ� ���� ���� ƽ(�ϴ� 1��)

#define MAX_EXECUTE_QUERY			1			///< ��ƽ�� �����ϴ� �ִ� ����

#define LOG_POINT_TOP_PRIORITY		100			///< �ְ� ���� �α� ����
#define LOG_POINT_SECOND_PRIORITY	10			///< �ι�° ���� �α� ����
#define LOG_POINT_THIRD_PRIORITY	1			///< ����° ���� �α� ����


#endif