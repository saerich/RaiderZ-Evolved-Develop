#ifndef _GTASK_ID_H
#define _GTASK_ID_H

/// Task ID
enum GTaskID
{
	GTASK_NA		= 0,
	GTASK_IDLE,					///< ���
	GTASK_MOVE,					///< ���� ��ġ�� �̵�
	GTASK_COMBAT_MOVE,			///< ������ �̵�
	GTASK_TALENT,				///< ��ų ���
	GTASK_GUARD,				///< ���
	GTASK_LUA_CALL,				///< ����Լ� ȣ��
	GTASK_SPEAK,				///< ��ȭ�Լ� ȣ��
	GTASK_ADJUST,				///< ���� ���� ��ġ�� ���� (guard_rate��)
	GTASK_AGGRO,				///< ��׷� ���̺� ����
	GTASK_BREAKPART,			///< �극��Ŀ�� ���� �ı�
	GTASK_MOTIONFACTOR,			///< Motionfactor
	GTASK_CHANGE_CAPSULEGROUP,	///< �浹���� �׷� ����
	GTASK_DAMAGE,				///< ��������
	GTASK_FLEE,					///< ����ġ��
	GTASK_YELL,					///< �����û�ϱ�
	GTASK_WAITFOR,				///< Ư�� ���Ͱ� NextAction�Ҷ����� ����ϱ�

	GTASK_TEST,
};

enum GTaskEvent
{
	TE_NA				 = 0,
	TE_PATH_MOVE_BLOCKED,
	TE_PATH_FIND_FAILED,
};

/// task ���
enum GTaskResult
{
	TR_NONE,
	TR_RUNNING,
	TR_COMPLETED,
	TR_CANCELED
};

#endif	