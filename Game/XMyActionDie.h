#ifndef _XMYACTION_DIE_H
#define _XMYACTION_DIE_H

#include "XMyActionState.h"
#include "MTime.h"

//#define _MADDUCK3
///@brief Die Action State
class XMyActionDie : public XMyActionState
{
protected:
	float					m_fRebirthTime;							//< �ڵ���Ȱ�����ִ� �ð�
	MRegulator				m_RebirthRegulator;						//< ��Ȱâ �߿��� �ִ� �ð�Ÿ�̸�
	bool					m_bChangeState;							//< ���ִ̾ϸ��̼��� ����Ǿ�� ��Ȱ�� �ǵ���

	void					RebirthWindowShow(bool bForceDieAniEnd = false);
	void					RebirthRequireWindowShow();
	void					Rebirth();
	void					CheckRebirthWindow();

public:
	XMyActionDie(XModuleMyControl* pOwner);
	virtual void			Enter(void* pParam=NULL);
	virtual void			Exit();
	virtual bool			CheckEnterable(void* pParam);
	virtual bool			CheckExitable(int nNextStateID);
	virtual XEventResult	Event(XEvent& msg);
	virtual void			Update(float fDelta);
	virtual int				GetID() { return ACTION_STATE_DIE; }
	virtual	bool			IsPostMovement()	{ return false; }

	float					GetRebirthRemainTime();

#ifdef _MADDUCK3
	// test
	bool					m_bTestRebirth;
	void					Debug_AutoRebirth();	//< �׽�Ʈ�� �ڵ� ��Ȱ
#endif
};


#endif // _XMYACTION_DIE_H