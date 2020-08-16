#pragma once

#include "GTalentFocus.h"
#include "MTime.h"

class GTalentInfo;

class GTalentFocus_Enlighten : public GTalentFocus, public MTestMemPool<GTalentFocus_Enlighten>
{
public:
	GTalentFocus_Enlighten(GEntityPlayer* pOnwer);
	virtual ~GTalentFocus_Enlighten(void);

	// �ش� ��Ŀ�� Ÿ���� ��ȯ
	virtual TALENT_FOCUS_TYPE	GetType()				{ return TFT_ENLIGHTEN; }

	// ��Ŀ���� Ȱ��ȭ�� �� �ִ��� ����, Ȱ��ȭ �� �� �ִٸ� true�� ��ȯ�Ѵ�.
	virtual bool				CheckActivate();

protected:
	virtual bool IsGainViaBuff() { return true; }
};
