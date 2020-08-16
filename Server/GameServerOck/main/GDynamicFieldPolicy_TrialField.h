#ifndef _G_TRIAL_GROUP_POLICY_H_
#define _G_TRIAL_GROUP_POLICY_H_

#include "GDynamicFieldPolicy.h"

class GDynamicFieldPolicy_TrialField : public GDynamicFieldPolicy, public MTestMemPool<GDynamicFieldPolicy_TrialField>
{
private:
	bool		m_bTrialQuestCompleted;
	uint32		m_nExpiredTime;				// �ı��� �ð�			// TODO(pyo): Regulator�� ���� �� ��
	set<MUID>	m_setQuestRunner;			// ������ ������
	MUID		m_uidOwnParty;				// �����ϰ� �ִ� ��Ƽ	

private:
	bool CheckQuestRunner();
	void BeginExpiredCount(int nExpiredTime);

	bool CheckTrialQuestCompleted();	

	bool CheckExpiredCount();
	void CancelExpiredCount();
	void OnExpired();

	bool IsMatchedQuestRunner(MUID uidPlayer, const vector<int>* pVecQuestID);
	void InsertQuestRunner(MUID uidPlayer, MUID uidField);
	void EraseQuestRunner(MUID uidPlayer);
	bool HasQuestRunner();
	bool CheckBoundary();

	virtual FIELD_ENTER_FAIL_TYPE IsEnterable() override;
	bool IsChallengerQuestField() const;
protected:
	virtual void OnUpdate(float fDelta);
	virtual bool OnEnterPlayer(GEntityPlayer* pPlayer);
	virtual void OnLeavePlayer(GEntityPlayer* pPlayer);
	virtual void OnDestroy();

public:
	GDynamicFieldPolicy_TrialField(GDynamicFieldMaster* pOwner);
	virtual ~GDynamicFieldPolicy_TrialField();


	virtual DYNAMICFIELD_POLICY_TYPE GetType()  override { return DPT_TRIAL; }

	virtual void OnQuestSynch(MUID uidPlayer, MUID uidField, const vector<int>* pVecQuestID) override;
	virtual void OnQuestComplete(GEntityPlayer* pPlayer, int nQuestId) override;	
	virtual void OnQuestEraseRunner(MUID uidPlayer) override;
	virtual bool IsExpiredCounting(void) override;

	virtual void ChangePlayerUID(MUID uidOffline, MUID uidMember) override;
};

#endif
