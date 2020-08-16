#pragma once

class MWLua;

#include "MScriptHelper.h"

namespace mint3 {

/// Mint�� ��� ��ũ��Ʈ�� ����
class MScriptManager
{
private:
protected:
	MWLua*			m_pLua;
	MScriptHelper	m_ScriptHelper;
	bool			InitDeclWidgets();
	void			InitDeclEvents();
public:
				MScriptManager();
	virtual		~MScriptManager();
	bool		Initialize(MWLua* pLua);

	void		DeclWidget(const char* szClass, const char* szName, MWidget* pWidget);
	void		ReleaseWidget(const char* szName);
	void		AddWidgetEvent(string& strWidgetName, string& strEventName, string& strScript);

	void		RunScriptFile(const char *szFileName);
	void		RunScriptString(const char* szStream);
	void		RunScriptWidgetEvent(MWidget* pWidget, const char* szEventName);
	MWLua* GetLua() { return m_pLua; }
};



} // namespace mint3