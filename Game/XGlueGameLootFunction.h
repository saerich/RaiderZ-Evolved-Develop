#pragma once

class XGlueGameLootFunction
{
public:
	//����
	void					LootItem(const char* szItemID, const char* szItemCount, bool bPass = false);
	void					LootEndRequest();
};
