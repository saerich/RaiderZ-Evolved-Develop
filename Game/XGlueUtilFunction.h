#pragma once

#include "XGlueFunction.h"

/// lua���� ����ϴ� ��ƿ API
class XGlueUtilFunction : public XGlueFunction
{
private:
	char		m_szBuffer[1024];
public:
	virtual void DeclGlueFunc(MWLua* pLua);
	int ParamCount(const char* szParams);
	const char* Param(const char* szParams, int nIndex);
};
