#pragma once

#include "GTalentFocus.h"

class GEntityPlayer;

class GTalentFocus_Advent : public GTalentFocus, public MTestMemPool<GTalentFocus_Advent>
{
public:
	GTalentFocus_Advent(GEntityPlayer* pOnwer);
	virtual ~GTalentFocus_Advent(void);

	// ��Ŀ���� Ȱ��ȭ�� �� �ִ��� ����, Ȱ��ȭ �� �� �ִٸ� true�� ��ȯ�Ѵ�.
	virtual bool				CheckActivate();
	// ��Ŀ�� ���ӽð� ��ȯ, 0.0f�� ��ȯ�ϸ� �������� ���Ѵ�.
	virtual float				GetDurationTime();

	// �ش� ��Ŀ�� Ÿ���� ��ȯ
	virtual TALENT_FOCUS_TYPE	GetType()				{ return TFT_ADVENT; }
};
