#pragma once

class XObject;

// Ÿ�ݰ��� ���� �����ڰ� ���ݽ� �����Ǹ� ����� ������Ų��.
class XHitFrameFreezer
{
private:
	bool m_bReserve;
	float m_fReserveElapsedTime;
	wstring	m_strCurrAni;

	void FreezeFrame(XObject* pAttacker);
public:
	XHitFrameFreezer() : m_bReserve(false) {}
	void OnHit(XObject* pAttacker, const wchar_t* szCurrAni);
	void Update(float fDelta, XObject* pAttacker, const wchar_t* szCurrAni);
};
