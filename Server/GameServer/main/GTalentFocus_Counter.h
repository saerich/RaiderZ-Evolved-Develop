#pragma once

#include "GTalentFocus.h"

class GEntityActor;

class GTalentFocus_Counter : public GTalentFocus, public MTestMemPool<GTalentFocus_Counter>
{
public:
	GTalentFocus_Counter(GEntityPlayer* pOnwer);
	virtual ~GTalentFocus_Counter(void);

	// �ش� ��Ŀ�� Ÿ���� ��ȯ
	virtual TALENT_FOCUS_TYPE	GetType()				{ return TFT_COUNTER; }
	// ��Ŀ���� Ȱ��ȭ�� �� �ִ��� ����, Ȱ��ȭ �� �� �ִٸ� true�� ��ȯ�Ѵ�.
	virtual bool				CheckActivate();
	// ��Ŀ�� ���ӽð� ��ȯ, 0.0f�� ��ȯ�ϸ� �������� ���Ѵ�.
	virtual float				GetDurationTime();
	// ��ȸ������ ����. ��Ŀ���ŷ�Ʈ�� ���������� ����ϰ� ��Ŀ���� ������ �����Ѵ�.
	virtual bool				IsOnce()				{ return true; }
	
	// ���̼����� �ִ��� ����
	virtual bool				CheckLicense() const override;

	// ��� �̺�Ʈ
	void OnGuard(GEntityActor* pAttacker);
	void OnAbsoluteGuard(GEntityActor* pAttacker);
private:
	// ���带 ���������� �� Ƚ��
	int m_nGuardCount;
};
