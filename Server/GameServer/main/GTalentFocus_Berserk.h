#pragma once

#include "MTime.h"
#include "GTalentFocus.h"

class GTalentInfo;

class GTalentFocus_Berserk : public GTalentFocus, public MTestMemPool<GTalentFocus_Berserk>
{
public:
	GTalentFocus_Berserk(GEntityPlayer* pOnwer);
	virtual ~GTalentFocus_Berserk(void);

	// �ش� ��Ŀ�� Ÿ���� ��ȯ
	virtual TALENT_FOCUS_TYPE	GetType()				{ return TFT_BERSERK; }
	// ��Ŀ���� Ȱ��ȭ�� �� �ִ��� ����, Ȱ��ȭ �� �� �ִٸ� true�� ��ȯ�Ѵ�.
	virtual bool				CheckActivate();
	// ��Ŀ���� Ȱ��ȭ�� �� ȣ��
	virtual void				Active();
	// ��ƽ���� ȣ���, ��ȯ���� ��Ŀ���� ����ƴ��� ����
	virtual bool				Update(float fDelta);
	// ��Ŀ�� ���ӽð� ��ȯ, 0.0f�� ��ȯ�ϸ� �������� ���Ѵ�.
	virtual float				GetDurationTime();
		
	// ���̼����� �ִ��� ����
	virtual bool				CheckLicense() const override;

	// �����ŷ�Ʈ�� �������� �̺�Ʈ
	void OnHitMeleeTalent(GTalentInfo* pHitTalentInfo);
	// �ŷ�Ʈ�� ���� Ƚ�� ��ȯ
	int GetHitCount();
	// ��Ŀ���� �ɸ��� ���� �ʿ��� ���� Ƚ�� ��ȯ
	int GetMaxHitCount();
private:
	// �ŷ�Ʈ�� ���� Ƚ���� �ʱ�ȭ��
	void ResetCounter();

private:
	// �ŷ�Ʈ�� ���� Ƚ��
	int			m_nHitTalentCounter;
	// �ŷ�Ʈ ��Ʈ ���� Ÿ�̸�, ��Ŀ���� ��� ���� �ŷ�Ʈ�� ��Ʈ�Ҷ� ���� �ð����� �ľ߸� �ϰ��Ҷ� ����
	MRegulator	m_rgrRelayHit;
};
