#ifndef _G_CONDITIONS_SYSTEM_H_
#define _G_CONDITIONS_SYSTEM_H_

#include "GConditionChecker.h"

class GConditionsInfo;
class GCondition_Leaf;
enum CONDITION_TYPE;
class GEntityPlayer;

class GConditionsSystem : public MTestMemPool<GConditionsSystem>
{
public:
	GConditionsSystem();
	~GConditionsSystem();

	virtual bool Check(GEntityPlayer* pPlayer, int nConditionID);
	bool CheckGroup(vector<GEntityPlayer*> vecPlayer, int nConditionID, bool isDisjunction);
	virtual bool CheckForPartyConjunction(GEntityPlayer* pPlayer, int nConditionID);	// ���� - ��� ����� ���϶� ��
	virtual bool CheckForPartyDisjunction(GEntityPlayer* pPlayer, int nConditionID);	// ���� - �� ����� ���̸� ��

	bool Check(GEntityPlayer* pPlayer, GConditionsInfo* pConditionsInfo, vector<GCondition_Leaf*>& vecFailConditionLeaf=vector<GCondition_Leaf*>());
	bool CheckForItem(GEntityPlayer* pPlayer, int nConditionID, vector<GCondition_Leaf*>& vecFailConditionLeaf=vector<GCondition_Leaf*>());

private:
	

	GConditionChecker	m_ConditionChecker;
};

#endif//_G_CONDITIONS_SYSTEM_H_
