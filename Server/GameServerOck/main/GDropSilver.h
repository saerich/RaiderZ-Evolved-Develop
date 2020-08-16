#pragma once

class GLootInfo;
class GEntityPlayer;

// ����� �ǹ��� �����ڰ���

class GDropMoney : public MTestMemPool<GDropMoney>
{
public:
	GDropMoney();		
	void Clear();

	void Drop(int nBeneficiaryCID, GLootInfo* pLootInfo);
	void Drop(const vector<int>& vecBeneficiaryCID, GLootInfo* pLootInfo);

	bool IsAuthorizedCID(int nCID);
	bool IsEmpty();
	int	 GetQuantity();

	void GetViewableCID(set<int>& outsetViewableCID);
	void GetAuthorizedCID(set<int>& outsetAuthorizedCID);

private:
	int			m_nMoney;
	set<int>	m_setAuhorizedCID;	// ȹ�� ������ �ִ� ����� CID
};
