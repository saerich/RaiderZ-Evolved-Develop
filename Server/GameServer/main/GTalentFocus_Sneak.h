#pragma once

#include "GTalentFocus.h"

class GTalentFocus_Sneak : public GTalentFocus, public MTestMemPool<GTalentFocus_Sneak>
{
public:
	GTalentFocus_Sneak(GEntityPlayer* pOnwer);
	virtual ~GTalentFocus_Sneak(void);

	// �ش� ��Ŀ�� Ÿ���� ��ȯ
	virtual TALENT_FOCUS_TYPE	GetType()				{ return TFT_SNEAK; }

	// ��Ŀ���� Ȱ��ȭ�� �� �ִ��� ����, Ȱ��ȭ �� �� �ִٸ� true�� ��ȯ�Ѵ�.
	virtual bool				CheckActivate();

	// ��ȸ������ ����. ��Ŀ���ŷ�Ʈ�� ���������� ����ϰ� ��Ŀ���� ������ �����Ѵ�.
	virtual bool				IsOnce()				{ return true; }
};
