#pragma once

// AI LOD Event
struct GAIEvent
{
	enum AI_EVENT_ID
	{
		EVENT_ID_NONE = -1,
		EVENT_ID_FIELD_ENTER_PLAYER,		// �ƹ��� ���� �ʵ忡 ������ ����
		EVENT_ID_FIELD_LEAVE_PLAYER,		// �ʵ忡�� ��� �÷��̾� ����
		EVENT_ID_SECTOR_ENTER_PLAYER,		// �ƹ��� ���� ���Ϳ� ������ ����
		EVENT_ID_SECTOR_LEAVE_PLAYER,		// ���Ϳ��� ��� �÷��̾� ����
		EVENT_ID_COMBAT_BEGIN,				// ���� ����
		EVENT_ID_COMBAT_END,				// ���� ��
		EVENT_ID_SECTOR_MOVE_BE_PLAYER,		// NPC�� �������Ϳ� �÷��̾ �ִ� ���ͷ� �̵�
		EVENT_ID_SECTOR_MOVE_EMPTY_PLAYER,	// NPC�� �������Ϳ� �÷��̾ ���� ���ͷ� �̵�
		EVENT_ID_SPAWN_EMPTY_FIELD,			// NPC�� �ƹ��� ���� �ʵ忡 ����
		EVENT_ID_SPAWN_EMPTY_SECTOR,		// NPC�� �ƹ��� ���� ���Ϳ� ����
		EVENT_ID_SPAWN_PLAYER_SECTOR,		// NPC�� �÷��̾� �α� ���Ϳ� ����
		EVENT_ID_MAKE_MAJOR_LOD_NPC,		// �ֿ� LOD NPC ���� - ���� �ܿ� LOD�� ������ ���� �ʴ´�.

		MAX_EVENT_ID
	};

	AI_EVENT_ID eEventID;
	MUID uidField;
	int nSectorX;
	int nSectorY;
	MUID uidNPC;

	GAIEvent(): eEventID(EVENT_ID_NONE), uidField(MUID::ZERO), nSectorX(-1), nSectorY(-1), uidNPC(MUID::ZERO)	{}

	wstring GetName(void)
	{
		switch(eEventID)
		{
		case EVENT_ID_FIELD_ENTER_PLAYER:			return L"FIELD_ENTER_PLAYER";
		case EVENT_ID_FIELD_LEAVE_PLAYER:			return L"FIELD_LEAVE_PLAYER";
		case EVENT_ID_SECTOR_ENTER_PLAYER:			return L"SECTOR_ENTER_PLAYER";
		case EVENT_ID_SECTOR_LEAVE_PLAYER:			return L"SECTOR_LEAVE_PLAYER";
		case EVENT_ID_COMBAT_BEGIN:					return L"BATTLE_BEGIN";
		case EVENT_ID_COMBAT_END:					return L"BATTLE_END";
		case EVENT_ID_SECTOR_MOVE_BE_PLAYER:		return L"SECTOR_MOVE_BE_PLAYER";
		case EVENT_ID_SECTOR_MOVE_EMPTY_PLAYER:		return L"SECTOR_MOVE_EMPTY_PLAYER";
		case EVENT_ID_SPAWN_EMPTY_FIELD:			return L"SPAWN_EMPTY_FIELD";
		case EVENT_ID_SPAWN_EMPTY_SECTOR:			return L"SPAWN_EMPTY_SECTOR";
		case EVENT_ID_SPAWN_PLAYER_SECTOR:			return L"SPAWN_PLAYER_SECTOR";
		case EVENT_ID_MAKE_MAJOR_LOD_NPC:			return L"MAKE_MAJOR_LOD_NPC";
		}

		return L"EVENT_ID_NONE";
	}
};
