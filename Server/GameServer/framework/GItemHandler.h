#ifndef _GITEM_HANDLER_H
#define _GITEM_HANDLER_H

#include "MUID.h"
#include <map>
#include <set>
#include <vector>
using namespace std;

class IItemHandler
{
public:
	// TODO: MUID�� �ƴϰ� ���� ��ü�� �ٲ���.
	virtual void UseItem(MUID& uidEntity, MUID& uidItem) = 0;
	virtual vector<int>& GetItemIds() = 0;
};

/// ������ Ư������ ������ ��� �ڵ鷯�� �и��� ����..
class GItemHandler
{
private:
	map<int, IItemHandler*>		m_HandlerMap;
	set<IItemHandler*>			m_HandlerSet;
	static GItemHandler			m_stInstance;
public:
	GItemHandler();
	virtual ~GItemHandler();
	IItemHandler* GetHandler(int nItemID);
	void RegisterItemHandler(int nItemID, IItemHandler* pHandler);
};

class GPotionItemHandler : public IItemHandler
{
private:
	static vector<int>		m_stItemIds;
public:
	GPotionItemHandler();
	virtual ~GPotionItemHandler();
	virtual void UseItem(MUID& uidEntity, MUID& uidItem);
	vector<int>& GetItemIds()		{ return m_stItemIds; }
};


#endif