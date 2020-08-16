#ifndef _GMESSAGE_ID_H
#define _GMESSAGE_ID_H

#include "GDef.h"

class GLuaResult;
class GEntityPlayer;

/// �޼��� ID
enum GMessageID
{
	GMSG_UNKNOWN = 0,

	// AI���� ����ϴ� �̺�Ʈ
	GMSG_AI_EVT_THINK,				///< ���� ������Ȳ�� �ٲ������ �� AI�� ���� ���� �׼��� ���ϱ⸦ ���Ҷ� �߻�
	GMSG_AI_EVT_TRY_ATTACKED,		///< ������ �õ� ���ߴ�.
	GMSG_AI_EVT_ATTACK,				///< �����ߴ�.
	GMSG_AI_EVT_ATTACKED,			///< ���ݴ��ߴ�.
	GMSG_AI_EVT_ARRIVED,			///< ��ǥ�� �ϴ� ������ �����ߴ�.
	GMSG_AI_EVT_BLOCKED,			///< ���̻� �̵��� �� ����.
	GMSG_AI_EVT_DEAD,				///< �׾���
	GMSG_AI_EVT_FORGET_TARGET,		///< Ÿ���� �Ҿ���ȴ�
	GMSG_AI_EVT_FIND_TARGET,		///< Ÿ�� �߰�
	GMSG_AI_EVT_LISTEN,				///< �Ҹ��� �����
	GMSG_AI_EVT_YELL,				///< ������ ���� ��û
	GMSG_AI_EVT_HATETABLECHANGED,	///< ����Ʈ ���̺� ���� �ٲ����.
	GMSG_AI_EVT_STARTCOMBAT,		///< ���� ����
	GMSG_AI_EVT_ENDCOMBAT,			///< ���� ����
	GMSG_AI_EVT_FLEE,				///< ���� ĥ��
	GMSG_AI_EVT_ENDPROLOGUE,		///< ���� �ʱ� ���� ����

	GMSG_AI_EVT_START_VICTORY,		///< �¸� ���� ����
	GMSG_AI_EVT_GRABBED,			///< ������ ��
	GMSG_AI_EVT_CHANGETARGET,		///< Ÿ���� �ٲ���� ��
	GMSG_AI_EVT_ATTACKTURN,			///< �� ���ݰ��ɽ�������
	GMSG_AI_EVT_PEACE,				///< Peace ���·� ����
	GMSG_AI_EVT_SESSION,			///< Session ���·� ����
	GMSG_AI_EVT_SESSION_CHANGE_SCENE,	///< ���� �� ����
	GMSG_AI_EVT_SESSION_PUSHJOB,	///< ������ �߰�
	GMSG_AI_EVT_SESSION_FINISHED,	///< ��������
	GMSG_AI_EVT_JOB_CLEAR,			///< JobMgr �ʱ�ȭ
	GMSG_AI_EVT_JOB_COMPLETED,		///< JobMgr���� ���� ���������� �Ϸ�
	GMSG_AI_EVT_JOB_CANCELED,		///< JobMgr���� ���� ���
	GMSG_AI_EVT_JOB_TIMEDOUT,		///< JobMgr���� ���� �ð�����
	GMSG_AI_EVT_JOB_FINISHED,			///< JobMgr���� ���� �Ϸ� (Finished, Canceld ���)

	GMSG_AI_EVT_MISSION,			///< �̼� ���� �߰� �Ǿ���.

	// ��ų ����
	GMSGL_HIT_TALENT,			///< ��ų�� �¾Ҵ�


	GMSG_MAX
};

static const wchar_t* GMessageIDStr[GMSG_MAX] =
{
	L"GMSG_UNKNOWN",
	L"GMSG_AI_EVT_THINK",
	L"GMSG_AI_EVT_TRY_ATTACKED",
	L"GMSG_AI_EVT_ATTACK",
	L"GMSG_AI_EVT_ATTACKED",
	L"GMSG_AI_EVT_ARRIVED",
	L"GMSG_AI_EVT_BLOCKED",
	L"GMSG_AI_EVT_DEAD",
	L"GMSG_AI_EVT_FORGET_TARGET",
	L"GMSG_AI_EVT_FIND_TARGET",
	L"GMSG_AI_EVT_LISTEN",
	L"GMSG_AI_EVT_YELL",
	L"GMSG_AI_EVT_HATETABLECHANGED",
	L"GMSG_AI_EVT_STARTCOMBAT",
	L"GMSG_AI_EVT_ENDCOMBAT",
	L"GMSG_AI_EVT_FLEE",

	L"GMSG_AI_EVT_START_VICTORY",
	L"GMSG_AI_EVT_GRABBED",
	L"GMSG_AI_EVT_CHANGETARGET",
	L"GMSG_AI_EVT_ATTACKTURN",
	L"GMSG_AI_EVT_PEACE",

	L"GMSG_AI_EVT_MISSION",

	L"GMSGL_HIT_TALENT",
};


// �޼��� �Ķ��Ÿ

struct EVENT_ATTACKED_INFO
{
	EVENT_ATTACKED_INFO()
	: uidAttacker(MUID::ZERO)
	, nType(DA_NONE)
	, nDamage(0)
	, nEffectSourceType(EST_NONE)
	, nEffectSourceID(INVALID_ID)
	{
	}

	MUID				uidAttacker;
	DAMAGE_ATTRIB		nType;
	uint32				nDamage;

	EFFECT_SOURCE_TYPE	nEffectSourceType;
	int					nEffectSourceID;
};

struct EVENT_ATTACK_INFO
{
	MUID				uidTarget;
	uint32				nDamage;
};

struct EVENT_FOUND_ENEMY
{
	MUID				uidTarget;
};

struct EVENT_DIALOG_EXIT
{
	GEntityPlayer*		pPlayer;
	int					nDialogID;
	int					nExit;
};

struct EVENT_YELL_INFO
{
	MUID				uidSender;
	MUID				uidTarget;
};

struct EVENT_START_COMBAT
{
	MUID				uidEnemy;
};

struct EVENT_FLEE_INFO
{
	FleeType nFleeType;
	float fDurationTime;

	EVENT_FLEE_INFO()
		: fDurationTime(0.0f)
		, nFleeType(Flee_FromEnemy)
	{

	}
};

#endif
