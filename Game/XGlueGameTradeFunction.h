#pragma once

class XGlueGameTradeFunction
{
public:
	//Ʈ���̵�
	void					TradeStartReq();
	void					TradeRequiredAnswer(bool bAccept);
	void					TradeItemListUp( const char* szITEMUID );
	void					TradeItemListDel( const char* szITEMUID );
	void					TradePutUpSilver(int nPutUpSilver);
	void					TradeConfirmRequire();
	void					TradeCancel();
};
