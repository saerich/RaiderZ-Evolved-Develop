#ifndef _G_LOG_ITEM_TRADE_H_
#define _G_LOG_ITEM_TRADE_H_

#include "PLog.h"

struct TDL_LOG_ITEM;

class PLog_ItemTrade : public PLog
{
private:
	int						m_nCID1;		///< ĳ���� ID
	vector<TDL_LOG_ITEM>	m_vecTDItem1;	///< ������
	int						m_nGold1;		///< ���

	int						m_nCID2;		///< ĳ���� ID
	vector<TDL_LOG_ITEM>	m_vecTDItem2;	///< ������
	int						m_nGold2;		///< ���

public:
	PLog_ItemTrade(const wstring& strDate,	int nCID1, const vector<TDL_LOG_ITEM>& vecTDItem1, int nGold1,
											int nCID2, const vector<TDL_LOG_ITEM>& vecTDItem2, int nGold2);
	~PLog_ItemTrade();

	virtual wstring MakeQuery();
};

#endif
