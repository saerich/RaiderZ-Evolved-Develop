#pragma once

#include "GTalentFocus.h"

class GEntityPlayer;

class GTalentFocus_Precision : public GTalentFocus, public MTestMemPool<GTalentFocus_Precision>
{
public:
	GTalentFocus_Precision(GEntityPlayer* pOnwer);
	virtual ~GTalentFocus_Precision(void);

	// �ش� ��Ŀ�� Ÿ���� ��ȯ
	virtual TALENT_FOCUS_TYPE	GetType()				{ return TFT_PRECISION; }

	// ��Ŀ���� Ȱ��ȭ�� �� �ִ��� ����, Ȱ��ȭ �� �� �ִٸ� true�� ��ȯ�Ѵ�.
	virtual bool				CheckActivate();
};
