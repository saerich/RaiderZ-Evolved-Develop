#ifndef _XMESSAGE_ID_H
#define _XMESSAGE_ID_H


/// �޼��� ID
/// - �޼����� �̺�Ʈ�����θ� ����մϴ�. ������Ʈ�� ����� �޼����� ���� �ʵ��� �սô�. - birdkr(2007-02-02)
enum XEventID
{
	XEVT_UNKNOWN = 0,

	// ���� ���� -------------------------------------------------
	XEVTL_VIRTUAL_KEY_PRESSED,			///< Ű�Է�
	XEVTL_VIRTUAL_KEY_PRESSEDONCE,		///< Ű�Է� / ù �������� �ѹ��� ����
	XEVTL_VIRTUAL_KEY_RELEASED,			///< Ű�Է� ����

	// �ִϸ��̼�
	XEVTL_MOTION_PLAYONCE,				///< ���ϸ��̼� �ѹ� �÷��� �Ϸ��
	XEVTL_UPPER_MOTION_PLAYONCE,		///< ��ü ���ϸ��̼� �ѹ� �÷��� �Ϸ��

	XEVTL_ON_MOTION_EVENT,
	XEVTL_ON_CHANGE_MOTION,				///< ����� �ٲ���� ��
	XEVTL_ON_CHANGE_ANIMATION,			///< ��Ǿȿ��� �ִϸ��̼��� �ٲ�
	XEVTL_ON_CHANGED_MOTION_TO_IDLE,	///< ����� IDLE�� �ٲ����..

	// �׼�
	XEVTL_ON_CAN_CANCEL_ACTION,			///< �׼��� ĵ�� �� �� ������
	XEVTL_ON_RELEASE_TALENT_KEY,		///< �ŷ�Ʈ ���Ű ��

	XEVTL_JUMP_FAILED,					///< ���� ����
	XEVTL_SWIM_IN,						///< �����Ϸ� ����.
	XEVTL_SWIM_OUT,						///< ������ ������ �浹�Ͽ� ������ ������.

	// �ŷ�Ʈ ����
	XEVTL_TALENT_START,					///< �ŷ�Ʈ ����
	XEVTL_TALENT_CANCELED,				///< �ŷ�Ʈ ���
	XEVTL_TALENT_ACT_SPELL,				///< ���� �߻�
	XEVTL_TALENT_RECASTING,				///< ĳ���� �غ�

	XEVTL_USE_ITEM,						///< ������ ���

	// MyAction ����
	XEVTL_MYACTION_RELEASE_ACTION_HOLD,		///< Hold�� MyAction ���¸� Idle�� ���̽��Ѷ�.
	XEVTL_MYACTION_SWITCHING_WEAPON_BEGIN,	///< ���� ���� ����
	XEVTL_MYACTION_SWITCHING_WEAPON_START,	///< ���� ����
	XEVTL_MYACTION_USE_TALENT,				///< �ŷ�Ʈ�� ����϶�
	XEVTL_MYACTION_NPC_INTERACTION_END,			///< ���ͷ��� ����

	XEVTL_LOOT_END_READY,				///< ���� ���� �غ� �Ϸ�(�������� �޼����� ���´�.)
	XEVTL_LOOT_END,						///< ���� ����
	XEVTL_LOOT_GETITEM,					///< �����ۿ�û
	XEVTL_LOOT_GETALLITEM,				///< �������ۿ�û

	XEVTL_CHANGE_STANCE,				///< ���Ľ��� ������
	XEVTL_CHANGED_STANCE,				///< ���Ľ��� �����

	XEVTL_INTERACTION_CLICK,			///< ȭ�鿡 ���콺 Ŭ��
	XEVTL_MOUSE_MOVE_CLICK,				///< ȭ�鿡 ���콺 Ŭ��, �̵�ó��

	// ��� ����
	XEVTL_THROW,				///< �������� ����� ���۵�
	XEVTL_GRAPPLED_TERRAIN_COL,			///< ��ü �浹�ߴ�.
	XEVTL_GRAPPLED_NEW_START,			///< ��⸦ �ٽ� �����Ѵ�(�̹� ��� ���°� ������ �ʾҴµ� ��⸦ ���� ���)

	//XEVTL_HOLD_KEYABLE,				///< ���ͷ���Ŀ�ǵ忡 ���� ������ ���� Ű�������� �Ե���

	XEVTL_MOTION_PLAYONCE_NPC_DIE,		///< NPC_DIE �ִϸ��̼��� �Ϸ��.
	XEVTL_DIE_ANI_END,					///< ���� �ִϸ��̼� �Ϸ�Ǿ���...(XModuleMotion���� ó���ϴ� �̺�Ʈ�� �ƴ϶�... XActionDie���� �˸��� �̺�Ʈ)

	XEVTL_CRAFTING_REQ,					///< ũ������ ��û.

	// ����ó�� �߻�
	XEVTL_EXPCEPTION_HOLD,				///< �������� ����ó�� �߻�

	// ����
	XEVTL_GUARD_KNOCKBACK,				///< ���� �˹��̴�
	XEVTL_GUARDED,						///< �����̴�
	XEVTL_GUARD_FAILED,					///< ���� ����

	// �Ա� ����
	XEVTL_SWALLOWED_NEW_START,			///< �Ա⸦ �ٽ� �����Ѵ�(�̹� ��� ���°� ������ �ʾҴµ� ��⸦ ���� ���)
	XEVTL_SWALLOWED_DISAPPEAR,			///< ������ ������� �ð�...

	// �ɱ� ����
	XEVTL_SIT_RISE,
	
	// ��ȭ
	XEVTL_ENCHANT_ITEM_SELECT,			///< ��ȭ ������ ����
	XEVTL_ENCHANT_START,				///< ��ȭ ����
	XEVTL_ENCHANT_CANCEL,				///< ��ȭ ���
	XEVTL_ENCHANT_CHECK_REQ,			///< ��ȭ üũ
	XEVTL_ENCHANT_CHECK_SUCCESS,		///< ��ȭ üũ ����
	XEVTL_ENCHANT_RESULT,				///< ��ȭ ���

	//����
	XEVTL_DYE_ITEM_SELECT,				///< ���� ������ ����
	XEVTL_DYE_START_ITEM,				///< ���� ����
	XEVTL_DYE_END_ITEM,					///< ���� �Ϸ�
	XEVTL_DYE_CANCEL,					///< ���� ���

	// PvP ī�޶� ����
	XEVTL_CAMERA_PVP_VIEW_CLEAR	,		///< PvP ���� ī�޶� ����

	// Net ����
	XEVTL_NET_MOVE,						///< NetPlayer �̵�

	XEVTL_INTERPOLATION_MOVE_ARRIVED,	///< ���� �̵� ���� �޼���

	// ���� �޼���--------------------------------------------------------
	XEVTD_TALENT_HIT,					///< ��ų�� ����(������ �־� �޼����� ó����) (���� - XTalentInfo*)
	XEVTD_GUARD_DEFENSE,				///< ������(������ �־� �޼����� ó����) (���� - _DelayedGuardDefenseInfo*)

	XEVTD_END_MF_STUN,					///< ���� ������� ����
	XEVTD_END_MF_KNOCKDOWN,				///< �˴ٿ� ������� ����

	XEVTD_DISAPPEAR_ME,					///< õõ�� �������.

	XEVTD_MESH_LOADING_COMPLETE,		///< �� �޽� �ε� �Ϸ�

	XEVTD_NPC_SAY_END,					///< NPC SAY END - ��ǳ�� ������

	XEVTD_ACTION_SLEEP,					///< 

	XEVT_MAX = 65535
};







#endif // #ifndef _XMESSAGE_ID_H