#pragma once

#include "XGlueFunction.h"

/// lua���� ����ϴ� ����� API
class XGlueDebugFunction : public XGlueFunction
{
private:

public:
	virtual void DeclGlueFunc(MWLua* pLua);
	void AddLogFilter(const char* szFilter);
	void Console(const char* szCommand);

	void ConsoleCmd(const char* szCmd);			// �ܼ� ��ɾ� �Է�
	void Log(const char* szMsg);


};
