#pragma once

#include "GActorObserver.h"

class GEntityPlayer;

class GPlayerAFK : private GActorObserver, public MTestMemPool<GPlayerAFK>
{
public:
	GPlayerAFK(GEntityPlayer* pOwner);
	virtual ~GPlayerAFK(void);

	// ƽ ����
	void Update(float fDelta);

	// �ڸ���� ���� ��ȯ
	bool IsNowAFK() const;

	// �ڸ���� ���� ���
	void Toggle();

	// �ڸ���� �ɶ����� ��ٸ� �ð� ��ȯ
	float GetTimeoutSecond() const;

private:
	// �ڸ���� ����
	void OnAfkBegin();
	// �ڸ���� ����
	void OnAfkEnd();

	// �׽�Ʈ��: Ŭ�󿡰� �˷��ִ� �����̼�
	void TestNarration(const char* pszText);
private:
	// �̵�, ���ݵ� �������� �ൿ�� ���� �� (Player ����)
	virtual void OnDoSomething() override;
	// �ڽ��� �׾�����
	virtual void OnDie() override;
private:
	GEntityPlayer*	m_pOwner;
	MRegulator		m_rgrTimeout;
	bool			m_bNowAFK;
};
