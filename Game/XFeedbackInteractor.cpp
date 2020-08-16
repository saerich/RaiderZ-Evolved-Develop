#include "stdafx.h"
#include "XFeedbackInteractor.h"

#include "XPlayer.h"
#include "XModuleActorControl.h"

#include "XEffectInvoker.h"

//////////////////////////////////////////////////////////////////////////
// _cancelable_gesture_ + ����̸�
#define	ENCHANT_SUCCESS_MOTION	L"_cancelable_gesture_enchant_success"
#define ENCHANT_SUCCESS_EFFECT	L"enchant_success"
#define ENCHANT_FAIL_MOTION		L"_cancelable_gesture_enchant_fail"
#define ENCHANT_FAIL_EFFECT		L"enchant_fail"
//////////////////////////////////////////////////////////////////////////

void XFeedbackInteractor::Interact(XObject* pObject)
{
	if (NULL == pObject)
		return;

	// trigging gesture
	if (false == m_strMotionName.empty())
	{
		XModuleActorControl* pModuleActorControl = pObject->GetModuleActorControl();
		if (pModuleActorControl)
		{
			pModuleActorControl->DoActionGesture(m_strMotionName.c_str());
		}
	}

	// trigging effect
	if (false == m_strEffectName.empty())
	{
		XEffectInvoker effectInvoker;
		effectInvoker.Invoke(pObject, m_strEffectName, wstring(), wstring());
	}
}

void XFeedbackInteractor::DoEnchantSuccessAction( XObject* pObject )
{
	SetFeedbackEffectName(ENCHANT_SUCCESS_EFFECT);
	SetFeedbackMotionName(ENCHANT_SUCCESS_MOTION);
	Interact(pObject);
}

void XFeedbackInteractor::DoEnchantFailAction( XObject* pObject )
{
	SetFeedbackEffectName(ENCHANT_FAIL_EFFECT);
	SetFeedbackMotionName(ENCHANT_FAIL_MOTION);
	Interact(pObject);
}

void XFeedbackInteractor::GetCancelableFeedbackInteraction( std::set<wstring>& _out )
{
	_out.insert(ENCHANT_SUCCESS_MOTION);
	_out.insert(ENCHANT_FAIL_MOTION);
}