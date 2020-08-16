#ifndef _XMYACTION_STATE_H
#define _XMYACTION_STATE_H

#include "XState.h"
#include "XVirtualKeyDef.h"
#include "XActionKeyDef.h"
#include "XMyActionID.h"

class XModuleMyControl;


///@brief ��� �� �׼� ������ �θ� Ŭ����
///@author �����
///@date 2007-01-05
class XMyActionState : public XState<XModuleMyControl>
{
public:
	XMyActionState(XModuleMyControl* pOwner) : XState<XModuleMyControl>(pOwner) {}
	virtual void			Enter(void* pParam=NULL);
	virtual void			Update(float fDelta);
	virtual void			PostUpdate(float fDelta);
	virtual void			Exit();
	virtual XEventResult	Event(XEvent& msg);
	virtual bool			OnSpecialAction(SPECIAL_ACTION_COMMAND nSpecialActionCommand) { return false; }
	virtual int				GetID() { return ACTION_STATE_NONE; }
	virtual const wchar_t*		GetDebugName();
	XPlayer*				GetOwnerPlayer();
	virtual	bool			IsPostMovement()	{ return true; }		// �̰��� false�� �ϸ� �ش� �׼� ���� ������ XPostMove�� ������ �ʴ´�.
};


class XMyActionFSM : public XStateMachine<XModuleMyControl>
{
public:
	XMyActionFSM(XModuleMyControl* pOwner);
	virtual ~XMyActionFSM();
	bool OnSpecialAction ( SPECIAL_ACTION_COMMAND nSpecialActionCommand );
	XMyActionState* GetCurrentState() { return static_cast<XMyActionState*>(XStateMachine<XModuleMyControl>::GetCurrentState()); }
};





#endif // _XMYACTION_STATE_H