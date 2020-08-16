#pragma once

class GQuestInfo;
class GEntityPlayer;
struct GITEM_STACK_AMT;

class GQuestRemoveItemCollector : public MTestMemPool<GQuestRemoveItemCollector>
{
public:
	void CollectForCancel(GEntityPlayer* pPlayer, GQuestInfo* pQuestInfo, vector<GITEM_STACK_AMT>& vecItemStackAmt);
	bool CollectForReward(GEntityPlayer* pPlayer, GQuestInfo* pQuestInfo, vector<GITEM_STACK_AMT>& vecItemStackAmt);
	void CollectForGive(GEntityPlayer* pPlayer, GQuestInfo* pQuestInfo, vector<GITEM_STACK_AMT>& vecItemStackAmt);

private:
	void CollectDestroyItem(GEntityPlayer* pPlayer, GQuestInfo* pQuestInfo, vector<GITEM_STACK_AMT>& vecItemStackAmt);		// ����Ʈ ���ŵɶ� �����Ǵ� ������
	void CollectAddItem(GEntityPlayer* pPlayer, GQuestInfo* pQuestInfo, vector<GITEM_STACK_AMT>& vecItemStackAmt);			// ����Ʈ �����Ҷ� ���� ������
	void CollectRemoveItem(GEntityPlayer* pPlayer, GQuestInfo* pQuestInfo, vector<GITEM_STACK_AMT>& vecItemStackAmt);		// ����Ʈ �����Ҷ� ���ŵǴ� ������
	bool CollectQObjectiveItem(GEntityPlayer* pPlayer, GQuestInfo* pQuestInfo, vector<GITEM_STACK_AMT>& vecItemStackAmt);	// ����Ʈ ���� ������

	void FilterSameItem(vector<GITEM_STACK_AMT>& vecItemStackAmt);
	bool CheckAlreadyExistItem(vector<GITEM_STACK_AMT>& vecItemStackAmt, const int64 nIUID );
};
