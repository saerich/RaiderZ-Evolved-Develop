#pragma once

#include "PBaseApplication.h"
#include "MSingleton.h"
#include "PServerInfo.h"
#include "MDebug.h"

class PCmdHandler_App;

/// ���ø����̼� - ������ ������ �� ���� ����
class PConsole : public PBaseApplication, public MSingleton<PConsole>
{
private:
	PCmdHandler_App*		m_pAppCmdHandler;
protected:
	virtual	bool	OnCreate();
	virtual void	OnDestroy();
public:
					PConsole();
	virtual			~PConsole();

	static void		OutputLogProc(const char* szText, MLogLevel nLevel);
	void			OutputLog(const wchar_t* szText, MLogLevel nLevel);
};
