#pragma once

// #include "STypes.h"

struct GDBT_TRADEMARKET_PUT_ON
{
	GDBT_TRADEMARKET_PUT_ON()
		: slotFromInventory(-1)
		, price(-1)
		, quantity(-1)
		, expiryDate(-1)
		, expectItemCount(-1)
	{}

	GDBT_TRADEMARKET_PUT_ON(const MUID& puid,const CID& pcid,int slot,int p,int	q,int date,const IUID& iuid,const int& expectItemCnt)
		: playerUID(puid)
		, playerCID(pcid)
		, slotFromInventory(slot)
		, price(p)
		, quantity(q)
		, expiryDate(date)
		, itemIUID(iuid)
		, expectItemCount(expectItemCnt)
	{}

	MUID	playerUID;
	CID		playerCID;
	int		slotFromInventory;
	int		price;
	int		quantity;
	int		expiryDate;	// ���밪. ��: 3 - 3��, 5 - 5��, 10 - 10��
	IUID	itemIUID;
	int		expectItemCount;
};