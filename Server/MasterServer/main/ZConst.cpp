#include "StdAfx.h"
#include "ZConst.h"
#include "ZScriptManager.h"


int				ZConst::GAME_SERVER_DYNAMIC_FIELD_COUNT_LIMIT = 200;	///< ���� ������ ������ �� �ִ� �����ʵ��� �ִ� ����
int				ZConst::GAME_SERVER_PLAYER_COUNT_LIMIT = 1000;			///< ���� ������ ������ �� �ִ� �÷��̾��� �ִ� ����

int				ZConst::FITNESS_ADDITIONAL_VALUE_TO_ALLOC_FIELD = 10;	///< ���� �ʵ带 �Ҵ��� ������ �� �� Ư�� �������� �ִ� ������
float			ZConst::TIME_UPDATE_CHANNEL_BEHOLDER_SEC = 1.0f;		///< ChannelBeholder �� ������Ʈ �ֱ� (��)
unsigned int	ZConst::TICK_EXPIRE_SILENT_CHANNEL = 300000;			///< ChannelBeholder ���� ������� �ʴ� ä���� �����ϱ���� ��� �ð� (ms)
int				ZConst::RATIO_CHANNEL_CROWD_FOR_NEW_CHANNEL = 90;		///< ��� ä���� ��% �̻� ��ȭ�����̸� ���ο� ä���� �������� ���� ����

bool			ZConst::SHOW_LUA_ERROR_LOG					= false;
bool			ZConst::ALLOC_DYNAMIC_SERVER_ANOTHER_SERVER	= false;

bool			ZConst::DEBUG_CONSOLE_SHOW_GAMETICK = false;
int				ZConst::DEBUG_CONSOLE_GAMETICK_TICK = 60;

bool			ZConst::INCLUDE_DEV_FIELD_LIST = false;
bool			ZConst::INSERT_WORLD_INFO = false;

#define GET_LUA_VAR(RETTYPE, NAME, VAR)		if (WLUA->IsExistGlobal(NAME)) VAR = WLUA->GetVar<RETTYPE>(NAME)

void ZConst::Init()
{
	GET_LUA_VAR(int,			"GAME_SERVER_DYNAMIC_FIELD_COUNT_LIMIT",	GAME_SERVER_DYNAMIC_FIELD_COUNT_LIMIT);
	GET_LUA_VAR(int,			"GAME_SERVER_PLAYER_COUNT_LIMIT",			GAME_SERVER_PLAYER_COUNT_LIMIT);

	GET_LUA_VAR(int,			"FITNESS_ADDITIONAL_VALUE_TO_ALLOC_FIELD",	FITNESS_ADDITIONAL_VALUE_TO_ALLOC_FIELD);
	GET_LUA_VAR(float,			"TIME_UPDATE_CHANNEL_BEHOLDER_SEC",			TIME_UPDATE_CHANNEL_BEHOLDER_SEC);
	GET_LUA_VAR(unsigned int,	"TICK_EXPIRE_SILENT_CHANNEL",				TICK_EXPIRE_SILENT_CHANNEL);
	GET_LUA_VAR(int,			"RATIO_CHANNEL_CROWD_FOR_NEW_CHANNEL",		RATIO_CHANNEL_CROWD_FOR_NEW_CHANNEL);

	GET_LUA_VAR(bool,			"INCLUDE_DEV_FIELD_LIST",					INCLUDE_DEV_FIELD_LIST);

	GET_LUA_VAR(bool,			"INSERT_WORLD_INFO",						INSERT_WORLD_INFO);

	/// ����׿� ------------------------------------------------------------------

	GET_LUA_VAR(bool,			"SHOW_LUA_ERROR_LOG",						SHOW_LUA_ERROR_LOG);
	GET_LUA_VAR(bool,			"ALLOC_DYNAMIC_SERVER_ANOTHER_SERVER",		ALLOC_DYNAMIC_SERVER_ANOTHER_SERVER);

	GET_LUA_VAR(bool,			"DEBUG_CONSOLE_SHOW_GAMETICK",				DEBUG_CONSOLE_SHOW_GAMETICK);
	GET_LUA_VAR(int,			"DEBUG_CONSOLE_GAMETICK_TICK",				DEBUG_CONSOLE_GAMETICK_TICK);
}
