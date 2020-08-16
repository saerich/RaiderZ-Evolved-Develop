#pragma once

class GEntitySync;
class GBuffInfo;

class GBuffObserver
{
public:
	// ���� ������ ȿ��ó��
	virtual void OnGain(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo) {}
	// ���� ������ ȿ��ó��
	virtual void OnLost(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo, bool bRoute) {}
	// ������ ������� ��
	virtual void OnCancelled() {}
	// ���� ������ ȿ��ó��
	virtual void OnPeriod() {}
	// ������ ���� ���� ��
	virtual void OnExpired(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo) {}
	// ������ ���� ���� ��
	virtual void OnDispelled() {}
	// ������ �ߺ��Ǽ� �ɸ� ���
	virtual void OnDuplicated(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo) {}
	// ������ �ִ� ���õ� ���
	virtual void OnMaxStacked() {}
	// Triggered �� ��ϵ� ������ �ۿ����� ��
	virtual void OnAffected() {}
	// ������ ���õǼ� �ɸ� ���
	virtual void OnStacked(float fDurationTime, float fPeriodTime) {}
};