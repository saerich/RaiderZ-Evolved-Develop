#pragma once

#include "XGlueFunction.h"

/// lua���� ����ϴ� ���� API
class XGlueGameFunction : public XGlueFunction
{
private:
	wstring					m_strStringBuffer;
public:
	virtual void DeclGlueFunc(MWLua* pLua);

	// ä��
	void					ChatGlobal(const char* szMsg);

	// ��ȭ
	void					DialogButtonSel(int nSel);
	const char*				XGetString(const char* nKey);
	const char*				XGetString2(const char* nKey);

	void					UnHoldMyCharacter();

	// �α���

	// ���� ����

	// ��Ʈ��ũ
	void					Connect(const char* szLoginID, const char* szPassword, const char* szIP);
	void					Disconnect();

	const char*				GetPlayerID( MUID uid);
	const char*				GetInteractivePlayerName();

	//��Ȱ
	void					RequireRebirth(int nSelect);

	// Buff ����
	const char*				GetBuffName(int nBuffID);
	bool					IsBuff(int nBuffID);

	unsigned int			GetDynamicFieldRemainTime();

	int						GameTime();
	int						GameWeather();

	int					GetStringWidth(const char* szWidgetName, const char* szMsg);

	// Feedback
	void					SendFeedback();
	void					ReloadFeedback();
};
