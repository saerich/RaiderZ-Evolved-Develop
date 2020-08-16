#ifndef _GJOB_ID_H
#define _GJOB_ID_H

/// Job ID
enum GJobID
{
	GJOB_NA				= 0,
	GJOB_IDLE,					///< ���
	GJOB_MOVE_TO_POS,			///< ���� ��ġ�� �̵�
	GJOB_MOVE_TO_ENTITY,		///< ���� ��ƼƼ�� �̵�
	GJOB_ESCAPE_FROM_AREA,		///< ���� �������� ���

	GJOB_PATROL,				///< ����
	GJOB_ROAM,					///< ��ȸ
	GJOB_TALENT,				///< ��ų ���
	GJOB_GUARD,

	GJOB_TEST,

	GJOB_FLEE_FROM_ENTITY,		///< ���� ��ƼƼ�κ��� ����
	GJOB_FLEE_TO_HOME,			///< ���� ������������ ����
//	GJOB_ROUND_TO_POS,			///< ������ ��ġ���� �ڽ��� �̵�
	GJOB_SCRIPT,				///< ��ũ��Ʈ
	GJOB_SPEAK,					///< ��ȭ
	GJOB_ADJUST,				///< ���� ���� ��ġ�� ���� (guard_rate��)
	GJOB_AGGRO,				///< ��׷� ���̺� ����
	GJOB_BREAKPART,			///< �극��Ŀ�� ���� �ı�
	GJOB_MOTIONFACTOR,			///< Motionfactor
	GJOB_CHANGE_CAPSULEGROUP,	///< �浹���� �׷� ����
	GJOB_DAMAGE,				///< ��������
	GJOB_FLEE,					///< ����ġ��
	GJOB_YELL,					///< �����û�ϱ�

	GJOB_WARP,					///< Ư�� ��Ŀ ��ġ�� ���� �̵�		
	GJOB_INSTANTBUFF,			///< ���� ȿ���� ������ �����.
	GJOB_DESPAWN,				///< ����
	GJOB_DIE,					///< �׵��� �����.
	GJOB_ASSIST,				///< ����� ���� �����Ѵ�.
	GJOB_ATTACK,				///< ����� �����Ѵ�.	
	GJOB_FACETO,				///< ����� �ٶ󺻴�.
	GJOB_WAITFOR,				///< ��� ���Ͱ� NextAction�Ҷ����� ���.
	GJOB_NEXTACTION,			///< �ڽ��� ����ϴ� ���͵��� ���� �ൿ�� �� �� �ְ� ó��
	GJOB_SUMMON,				///< ���� ��ȯ

	GJOB_SESSION_BLOCKING,		///< ���� ��ŷ��� ����
	GJOB_SESSION_WAIT,			///< ���� ���
	GJOB_SESSION_ADD_NPC,		///< ���ǿ� NPC �߰�
	GJOB_SESSION_REMOVE_NPC,	///< ���ǿ��� NPC ����
	GJOB_SESSION_END,			///< ���� ����
	GJOB_SESSION_CHANGE_STATE,	///< ���� ���� ����
	GJOB_SESSION_MAKE_PEACE,	///< ���ǿ� ���õ� NPC ��� ��������
	GJOB_SESSION_MAKE_COMBAT,	///< ���ǿ� ���õ� NPC ��� ��������

	GJOB_MAX
};

/// task ���
enum GJobResult
{
	JR_NONE,
	JR_RUNNING,
	JR_COMPLETED,
	JR_CANCELED,
	JR_TIMEOUT,
};


#endif