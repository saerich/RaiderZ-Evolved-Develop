#pragma once

/// NPC �׼� ����
enum NPC_ACTION_STATE
{
	NPC_ACTION_STATE_NONE			= -1,
	NPC_ACTION_STATE_NA				= 0,		///< �̺з�
	NPC_ACTION_STATE_MOVE			= 1,		///< �̵�
	NPC_ACTION_STATE_TALENT			= 2,		///< �ŷ�Ʈ ���
	NPC_ACTION_STATE_HIT			= 3,		///< �ǰ�
	NPC_ACTION_STATE_DIE			= 4,		///< ����

	MAX_NPC_ACTION_STATE
};
