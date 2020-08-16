#pragma once

#include <set>

class XFeedbackInteractor
{
public:
	XFeedbackInteractor() {}
	virtual ~XFeedbackInteractor() {}

	void SetFeedbackMotionName(const wchar_t* _pSzMotionName)	{ m_strMotionName = _pSzMotionName; }
	void SetFeedbackEffectName(const wchar_t* _pSzEffectName)	{ m_strEffectName = _pSzEffectName; }

	void Interact(XObject* pObject);
	static void GetCancelableFeedbackInteraction(std::set<wstring>& _out);


// ������ �ǵ�� �Լ�
public:
	// ��ȭ
	void DoEnchantSuccessAction(XObject* pObject);
	void DoEnchantFailAction(XObject* pObject);

private:
	std::wstring m_strEffectName;
	std::wstring m_strMotionName;
};
