#pragma once

enum XGuideMsgEnum
{
	GUIDE_MSG_NA					= 0,
	GUIDE_MSG_LEVEL_UP				= 1,	///< ������ ���� ��
	GUIDE_MSG_CAN_LOOT				= 2,	///< ���� ������ ��
	GUIDE_MSG_CAN_INTERACT_TO_NPC	= 3,	///< NPC �տ��� ���ͷ��� ������ ��

	// ������ ��ƿ��� ȣ���ϴ� �޼���
	GUIDE_MSG_LUA_RECV_INVITE_PARTY		= 101,	///< ��Ƽ�ʴ� �˾� �޼��� �޾��� ��
	GUIDE_MSG_LUA_RECV_INVITE_GUILD		= 102,	///< ����ʴ� �˾� �޼��� �޾��� ��
	GUIDE_MSG_LUA_RECV_TRADE_REQUESTED	= 103,	///< �ŷ���û �˾� �޼��� �޾��� ��
	GUIDE_MSG_LUA_RECV_DUEL_CHALLENGED	= 104	///< ������û �˾� �޼��� �޾��� ��
};

/*
	XGuideMsgEnum�� �߰��� �� game_defines.lua�� GUIDEMSG_TYPE�� �Բ� �߰��ؾ� �մϴ�. - birdkr
*/