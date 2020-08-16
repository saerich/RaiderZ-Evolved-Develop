#ifndef _XMYACTION_DYE_H
#define _XMYACTION_DYE_H

#include "XMyActionState.h"
#include "XTimer.h"
#include "XCommonData.h"

class XMyActionDye : public XMyActionState
{
private:
	MRegulator				m_Regulator;

	DYE_DATA				m_DyeData;

	bool					m_bActionDone;		///< �� �÷��װ� true�̾�� �� ActionState�� ��� �� �ִ�.
	bool					m_bPostDone;

	void					Init();
	void					DyeCancel();
	void					DyeEnd();

public:
	XMyActionDye( XModuleMyControl* pOwner );
	virtual bool			CheckEnterable(void* pParam);
	virtual bool			CheckExitable(int nNextStateID);
	virtual void			Enter(void* pParam=NULL);
	virtual void			Update(float fDelta);
	virtual void			Exit();;
	virtual XEventResult	Event(XEvent& msg);

	int						GetID() { return ACTION_STATE_DYE; }
	virtual	bool			IsPostMovement()	{ return false; }
};

#endif // _XMYACTION_HOLD_H