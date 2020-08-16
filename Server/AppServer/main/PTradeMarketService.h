#pragma once

//////////////////////////////////////////////////////////////////////////
// ��ǰ�ŷ��� �˻� ����
// 
// * Ư�̻���
// - ������ ����� �����ͺ��̽� �ø��� ��ȣ(dbid)�� Ű�� �����Ѵ�.
// - �߰������� ���� ���� �� multiset���ε� �����ϸ� �� �����̳ʴ� �ٽ� ������������ȣ(item type id)�� Ű���ϴ� ������ �����Ѵ�.
// - �������� ������ ���� ������? ���� ��ȹ�� ������ �ʾ�����... ���� ���� ������ �˻��ϸ� �� ���� ���� ���� ���� ������? �ϴ� ��������
//
// * �����ٸ�? �߰� ���� ����
//  - tbb�� �ٿ�����

#include "STypes.h"
#include "MIOCP.h"
#include "CTradeMarketConstant.h"

struct TD_TRADEMARKETITEM;
namespace minet {
	class MCommand;
}

class PTradeMarketService : public MIOCP::IHandler
{
	struct SEARCH_FILTER;
	typedef std::tr1::shared_ptr<SEARCH_FILTER> FilterPtr;
protected:
	typedef int64	ITEM_DBID;
	typedef int		ITEM_TYPEID;
	typedef std::map<ITEM_DBID,TD_TRADEMARKETITEM>		ITEMS_BY_DBID;
	typedef std::multiset<TD_TRADEMARKETITEM>			ITEMS_ORDER_BY_PRICE;
	typedef std::map<ITEM_TYPEID,ITEMS_ORDER_BY_PRICE>	ITEMS_BY_TYPEID;
	typedef std::map<CID,FilterPtr>						PLAYER_FILTERS;

public:
	PTradeMarketService();
	virtual ~PTradeMarketService();

	virtual void NotifyAddItem(minet::MCommand* const cmd) const;
	virtual void NotifyDelItem(minet::MCommand* const cmd) const;
	virtual void RequestSearch(minet::MCommand* const cmd) const;
	virtual void RequestSearchNext(minet::MCommand* const cmd) const;

protected:
	void OnCmd(minet::MCommand* const cmd) const;

	void OnTradeMarketAdd(minet::MCommand& cmd);
	void OnTradeMarketDel(minet::MCommand& cmd);
	void OnTradeMarketSearch(minet::MCommand& cmd);
	void OnTradeMarketSearchNext(minet::MCommand& cmd);
	
	void Add(const TD_TRADEMARKETITEM& item);
	void Search(const CID& player);
	void Route(const CID& player,vector<TD_TRADEMARKETITEM>& findedItems);

	void OnIO(const DWORD& numOfByteTransfered,OVERLAPPED* const overlapped) override;
	void OnError(const int& errCode) override;

protected:
	MIOCP*				m_iocp;
	size_t				m_totalCnt;
	ITEMS_BY_DBID		m_itemsByDBID;

	int					m_curInsertPageIdx;
	ITEMS_BY_TYPEID		m_itemsByTypeID;
	PLAYER_FILTERS		m_playersFilters;

private:
	struct IOMSG : public OVERLAPPED
	{
		minet::MCommand* cmd;
	};

	struct SEARCH_FILTER : public MMemPool<SEARCH_FILTER>
	{
		int				lastFildedPrice;
		set<ITEM_DBID>	findedDBID;
		vector<int>		wantedSearchTypeIDs;

		SEARCH_FILTER()
		{
			lastFildedPrice = -1;
		}
	};
};