#pragma once

#include "../../Game/GameCommon/XEffectInfo.h"


// �̺�Ʈ ����
//////////////////////////////////////////////////////////////////////////

class CoreEffectInfoMgr : public XEffectInfoMgr
{
protected:
	virtual	void				GetCommaStringSplitter(const tstring& strValue, vector<tstring>& vecOut);

public:
	CoreEffectInfoMgr();
	virtual ~CoreEffectInfoMgr(void) {}
};
