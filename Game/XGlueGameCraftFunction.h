#pragma once

class XGlueGameCraftFunction
{
public:
	//ũ����Ʈ
	int						GetItemIDWithRecipe(int nRecipeID);
	//int						GetRecipeRank(int nRecipeID);
	//int						GetOptionToolOfRecipe( int nRecipeID );
	const char*				GetMatOfRecipe(int nRecipeID);
	//const char*				GetSubMatOfRecipe(int nRecipeID);

	int						GetCraftableItemQuantity(int nRecipeID);
	//float					GetCraftableItemRate(int nRecipeID, float fToolOptionRate);
	void					StartCraftingItem( int nRecipeID , int nCount);
	void					CraftingCancelReq();
};
