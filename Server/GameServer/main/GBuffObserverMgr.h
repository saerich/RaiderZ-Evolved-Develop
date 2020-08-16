#pragma once

class GBuffObserver;
class GEntitySync;
class GBuffInfo;

class GBuffObserverMgr
{
public:
	GBuffObserverMgr(void);
	~GBuffObserverMgr(void);

	// �������� �����Ŵ
	void AttachObserver(GBuffObserver* pOvr);
	// �������� �и���Ŵ
	void DetachObserver(GBuffObserver* pOvr);

public:
	// ������ Triggered �� ��ϵ� ������ �ۿ����� ��
	void OnTriggeredBuffAffected();
protected:
	// ���� ������ ȿ��ó��
	void OnGain(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo);
	// ���� ������ ȿ��ó��
	void OnLost(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo, bool bRoute);
	// ������ ������� ��
	void OnCancelled();
	// ���� ������ ȿ��ó��
	void OnPeriod();
	// ������ ���� ���� ��
	void OnExpired(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo);
	// ������ ���� ���� ��
	void OnDispelled();
	// ������ �ߺ��Ǽ� �ɸ� ���
	void OnDuplicated(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo);
	// ������ �ִ� ���õ� ���
	void OnMaxStacked();
	// ������ ���õǼ� �ɸ� ���
	void OnStacked(float fDurationTime, float fPeriodTime);
private:
	// ������ ����
	vector<GBuffObserver*>	m_vecObservers;
};
