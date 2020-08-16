#ifndef _XMODULE_ID_H
#define _XMODULE_ID_H

enum ModuleID
{
	//----------------------------------------------------------------------------
	// 1.
	// �����Դϴ�. ���� ����� ���� �ٶ��ϴ�.
	//----------------------------------------------------------------------------
	// XModuleMessageQueue::OnUpdate(float fDelta) -> XObject::Event(XEvent & msg) -> XObject::OnEvent(XEvent & msg)
	// -> XModuleContainer::Event(XEvent & msg) -> XModule::Event(XEvent & msg) -> XModuleAction::OnEvent(XEvent msg)
	// -> XActor::DeadProcEnterField() -> XModuleAction::DoneDIe() -> XActionDie::DoneDie()
	// -> XModuleEntity::_SetUpVector(const MVector3 & up) -> XModuleEntity::_UpdateTransform()
	XMID_MESSAGE_QUEUE = 0,			// �ð������Ŀ� �޾ƾ� �ϴ� �޼����� ����
	XMID_PLAYER_CONTROL,	// MyControl, NetControl�� �������� �κ��� ���
	//XModuleMotion::OnUpdate(float fDelta) -> XMotionLogic::Update(float fDelta) -> MotionLogic::PlayDone()
	// -> XMotionLogic::SendEvent(XEventID nEventID=XEVTL_MOTION_PLAYONCE) -> XObject::Event(XEvent&)
	// -> XObject::OnEvent(XEvent&) -> XModuleContainer::Event(XEvent&) -> XModule::Event(XEvent&)
	// -> XModuleMyControl::OnEvent(XEvent&) -> XStateMachine<XModuleMyControl>::Event(XEvent&)
	// -> XMyActionNormalAttack::Event(XEvent&) -> XMyActionNormalAttack::doAttack(TALENT_NORMAL_ATTACK_TYPE nAttackType=NA_NORMAL2)
	// -> XModuleMyControl::SyncPlayerDirToCameraDir() -> XObject::SetDirection(const MVector3&)
	// -> XModuleEntity::SetDirection(const MVector3&) ������ ���� ����� ENTITY���� ���߿� ����Ǹ� Entity�� �ι�����Ǿ���մϴ�.
	XMID_MOTION,				// ������Ʈ�� �ִϸ��̼�, ����� ����
	// Move�� ���� ��ġ���� / Entity���� ���� �ҷ����Ѵ�.
	XMID_MOVABLE,				// �̵� ����

	//----------------------------------------------------------------------------
	// 2.
	// �������� ���谡 �����ϴ�.
	// �׷��� Entity ������ ó�� �ؾ� �մϴ�.
	//----------------------------------------------------------------------------
	XMID_ACTION,				// ������Ʈ�� �׼��� ���� - ��Ǻ��� ���� ����
	XMID_COLLISION,				// �浹 ����
	// Ż��Ʈ�� ���� ����Ʈ�߰�.
	// ����Ʈ �߰��� ���� UpdateForRender�� �����ɼ� �����Ƿ� Entity���� ���� �ҷ��ش�.
	XMID_TALENT,				// �ŷ�Ʈ ó��
	XMID_BREAKABLEPARTS,		// �극��Ŀ�� ����
	XMID_BUFF,					// ����
	XMID_SOUND,					// ����
	XMID_DATA,					// ����Ÿ
	XMID_BGW,					// ��׶��� ��ũ

	//----------------------------------------------------------------------------
	// 3.
	// �ִϸ��̼��� ó�� �մϴ�.
	//----------------------------------------------------------------------------
	XMID_ENTITY,				// �������� �Ͱ� ���õ� ���� ���(pos, mesh, animation ����)

	//----------------------------------------------------------------------------
	// 4.
	// �ִϸ��̼� ���Ŀ� ó�� �ؾ� �ϴ� �͵�...
	//----------------------------------------------------------------------------
	XMID_EFFECT,				//
	XMID_UI,					// Interaction
	XMID_POST,					// Ŀ�ǵ� ���� ����

	MAX_MODULE_ID
};


#endif