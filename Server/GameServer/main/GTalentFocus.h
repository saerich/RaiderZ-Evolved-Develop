#pragma once

#include "CSItemData.h"
#include "CSDef.h"

class GEntityPlayer;
class GEquipmentLimiter;


// Ż��Ʈ ��Ŀ���� �ֻ��� Ŭ����
class GTalentFocus
{
public:
	GTalentFocus(GEntityPlayer* pOnwer);
	virtual ~GTalentFocus();

	// �ش� ��Ŀ�� Ÿ���� ��ȯ
	virtual TALENT_FOCUS_TYPE	GetType()=0;
	// ��Ŀ���� Ȱ��ȭ�� �� �ִ��� ����, Ȱ��ȭ �� �� �ִٸ� true�� ��ȯ�Ѵ�.
	virtual bool				CheckActivate()=0;
	// ��Ŀ���� Ȱ��ȭ�� �� ȣ��
	virtual void				Active();
	// ��Ŀ���� ��Ȱ��ȭ�� �� ȣ��
	virtual void				Deactive();
	// Ȱ��ȭ �ƴ��� ����
			bool				IsActive()				{ return m_bActive; }
	// ��ȸ������ ����. ��Ŀ���ŷ�Ʈ�� ���������� ����ϰ� ��Ŀ���� ������ �����Ѵ�.
	virtual bool				IsOnce()				{ return false; }
	// ��ƽ���� ȣ���, ��ȯ���� ��Ŀ���� ����ƴ��� ����
	virtual bool				Update(float fDelta)	{ return false; }
	// ���������� ��Ŀ���� ���� �� �ִ� ������ �Ǵ��� ����
			bool				CheckEquipItem();
	// ��Ŀ�� ���ӽð� ��ȯ, 0.0f�� ��ȯ�ϸ� �������� ���Ѵ�.
	virtual float				GetDurationTime()		{ return BUFF_DURATION_INFINITY; }
	// �ش� ��Ŀ���� �Բ��� ���� ���̵� ��ȯ
			int					GetFocusBuffID();
	// ��Ŀ�� ������ ������ ����
			void				SetBuffType(int nBuffType);

	// ���̼����� �ִ��� ����
	virtual bool				CheckLicense() const	{ return true; }
protected:
	// ��������� ���������� �߰�
	void	AddEnableWeapon(WEAPON_TYPE nWeaponType);

	// ������ ���� ��Ŀ���� �ɸ����� ����
	virtual bool IsGainViaBuff() { return false; }
private:
	// ��Ŀ�� ���� ���
	void GainFocusBuff();
	// ��Ŀ�� ���� �ұ�
	void LostFocusBuff();
private:
	// ��Ŀ���� Ȱ��ȭ�ƴ��� ����
	bool			m_bActive;
	// ��Ŀ�� ������ ����
	int				m_nBuffType;
protected:
	// ��Ŀ�� ������
	GEntityPlayer*		m_pOwner;
	// ��������ۿ� ���� ���ѻ��� ������
	GEquipmentLimiter*	m_pEquipmentLimiter;
};
