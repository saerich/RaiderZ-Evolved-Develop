#pragma once

struct TD_ITEM_DROP;
struct TD_LOOT_MASTERLOOT_UI;
struct TD_LOOT_MASTERLOOT_NOTIFY;
enum DROP_ITEM_TYPE;
class GEntityNPC;

// ����� �������� ������ ����

class GDropItem : public MTestMemPool<GDropItem>
{
public:
	GDropItem(const MUID& nDropItemUID, int nItemID, int nItemAmount);
	~GDropItem();

	void InsertAuthorizedCID(int nCID);
	void DecraseAmount(int nDecreaseAmount);

	void SetFreeAuthority();
	void SetGettable();
	void SetMasterLootable();
	void SetRollable();

	void SetType(DROP_ITEM_TYPE nDIT);

	void Visible();
	void Unvisible();

	void GetViewableCID(set<int>& outsetViewableCID) const;
	int GetAmount() const;
	const MUID& GetUID() const;
	int GetID() const;

	bool IsViewableCID(int nCID) const;
	bool IsAuthorizedCID(int nCID) const;
	bool IsGettable() const;
	bool IsMasterLootable() const;
	bool IsRollable() const;
	bool IsVisible() const;
	DROP_ITEM_TYPE GetType() const;

	TD_ITEM_DROP				MakeTDDropItem() const;
	TD_LOOT_MASTERLOOT_UI		MakeTDMasterLootUI(GEntityNPC* pOwner) const;
	TD_LOOT_MASTERLOOT_NOTIFY	MakeTDMasterLootNotify() const;

	void ChangeGettableItem(int nCID);	///< Ÿ���� Gettable�� �ٲٰ� �����ڸ� �ѻ������ �����Ѵ�.

private:	
	MUID		m_nDropItemUID;
	int			m_nItemID;
	int			m_nItemAmount;
	set<int>	m_setAuthorizedCID;	///< ������ �ִ� ����� CID
	bool		m_bFreeAuthority;		///< �ƹ��� ������ �ִ��� ����

	DROP_ITEM_TYPE	m_nType;
	bool		m_bVisible;				///< �����ͷ��� �غ�Ǹ� �������� ������ �ʴ´�.
};