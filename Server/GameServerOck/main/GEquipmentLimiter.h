#pragma once

#include "CSItemData.h"

class GItemHolder;

// ��񽽷����κ��� ��ȿ�� ó���� �ϱ� ���� Ŭ����
class GEquipmentLimiter : public MTestMemPool<GEquipmentLimiter>
{
public:
	GEquipmentLimiter(GItemHolder* pItemHolder);
	~GEquipmentLimiter(void);

	// ��������� ���������� �߰�
	void AddEnableWeapon(WEAPON_TYPE nWeaponType);
	// ����������� ���������� ������Ű���� ����, �ƹ��� ���ǵ� ������� �ʾҴٸ� true�� ��ȯ
	bool IsEnable();
private:
	// ���⿡ ���� ���������� ������Ű���� ����
	bool IsEnableWeapon();
	// �ش� ���������� ��ϵǾ��ִ��� ����
	bool IsRegisterEnableWeapon(WEAPON_TYPE nWeaponType);
private:
	// �÷��̾��� �����۰�����
	GItemHolder*		m_pItemHolder;
	// ��밡���� ��������
	set<WEAPON_TYPE>	m_setEnableWeapons;
};
