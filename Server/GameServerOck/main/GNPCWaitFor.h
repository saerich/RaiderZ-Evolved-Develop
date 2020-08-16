#pragma once

class GEntityNPC;

class GNPCWaitFor : public MTestMemPool<GNPCWaitFor>
{
public:
	GNPCWaitFor(GEntityNPC* pOwner);
	~GNPCWaitFor(void);

	// ��� �޴� ��� --------------------
	void AddWaiter(MUID uidWaiter);
	void RunNextAction();

	// ��� �ϴ� ��� --------------------
	void AddWaiting(MUID uidTarget);
	bool IsWaiting();
	void OnNextAction(MUID uidActor);
private:
	GEntityNPC*		m_pOwner;
	// ��� �޴� ��� --------------------
	vector<MUID>	m_vecWaiters;	// ����� ���
	// ��� �ϴ� ��� --------------------
	set<MUID>		m_setWaitings;	// ����ϰ� �ִ� ���� ���
};
