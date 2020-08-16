#ifndef _XMYACTION_ID_H
#define _XMYACTION_ID_H

/// �� �׼� ����
enum ACTION_STATE
{
	ACTION_STATE_NONE				= -1,
	ACTION_STATE_IDLE				= 0,		///< IDLE
	ACTION_STATE_JUMP,							///< ����
	ACTION_STATE_FALLING,						///< �������� ��
	ACTION_STATE_CHANGE_STANCE,					///< �ڼ� ��ȯ
	ACTION_STATE_HIT,							///< �ǰ�
	ACTION_STATE_GUARD,							///< ���
	ACTION_STATE_NORMAL_ATTACK,					///< ��Ÿ
	ACTION_STATE_USE_TALENT,					///< �ڷ�Ʈ ���
	ACTION_STATE_SHOOT_ARROW,					///< Ȱ�� �⺻ ����

	ACTION_STATE_DIE,							///< ����
	ACTION_STATE_USE_ITEM,						///< ������ ���
	ACTION_STATE_WEAPON_CHANGE,					///< ���� ����
	ACTION_STATE_LOOTINGITEM,					///< ��ü����, ���ͷ��Ǿ�����
	ACTION_STATE_HOLD,							///< �ƹ� �͵� �������̴� ���� - �ַ� UI ���ͷ��ǿ����� ���

	ACTION_STATE_REBIRTH,						///< ��Ȱ

	ACTION_STATE_GRAPPLED,						///< ���

	ACTION_STATE_MAPOBJ_INTERACTION,			///< �ʿ�����Ʈ�� ���ͷ���
	ACTION_STATE_SIT,							///< �ɱ�

	ACTION_STATE_CHANGE_CHANNEL,				///< ä�� �̵�
	ACTION_STATE_QUEST_INTERACTION,				///< ����Ʈ ���ͷ���
	ACTION_STATE_DEACTIVATIVE,					///< Ȱ�� �Ҵ� ����

	ACTION_STATE_SWALLOWED,						///< �Ա�

	ACTION_STATE_GESTURE,						///< ���۵�

	ACTION_STATE_INTERACTION,					///< NPC�� ���ͷ���

	ACTION_STATE_ENCHANT,						///< ��þƮ

	ACTION_STATE_DYE,							///< ����

	MAX_ACTION_STATE
};









#endif // _XMYACTION_IDLE_H