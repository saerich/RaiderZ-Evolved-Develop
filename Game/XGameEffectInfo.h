#pragma once

#include "XEffectInfo.h"


// �̺�Ʈ ����
//////////////////////////////////////////////////////////////////////////

class XGameEffectInfoMgr : public XEffectInfoMgr
{
protected:
	virtual	void				GetCommaStringSplitter(const tstring& strValue, vector<tstring>& vecOut);

public:
	XGameEffectInfoMgr();
	virtual ~XGameEffectInfoMgr(void) {}
};
