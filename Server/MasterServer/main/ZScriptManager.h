#pragma once

#include "MWLua.h"
#include <fstream>

//class GLuaRuntimeErrorReporter;

/// ��� ��ũ��Ʈ�� ����
class ZScriptManager
{
public:
			ZScriptManager();
	virtual	~ZScriptManager();
	bool	Init(bool bUseCallbackLogger=false);
	
	void	Fini();
	bool	IsRunning()		{ return m_bRunning; }

	inline MWLua*			GetLua();

private:
	bool	m_bRunning;
	MWLua	m_Lua;
};



inline 
MWLua* 
ZScriptManager::GetLua()					
{ 
	return &m_Lua; 
}

ZScriptManager* GetScriptManager();

#define WLUA GetScriptManager()->GetLua()

