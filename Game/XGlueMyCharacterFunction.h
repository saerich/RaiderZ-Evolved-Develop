#pragma once

#include "XGlueFunction.h"

/// lua���� ����ϴ� ��ĳ������ API
class XGlueMyCharacterFunction : public XGlueFunction
{
public:
	virtual void DeclGlueFunc(MWLua* pLua);

	// ĳ���� ����
	const char*				GetID() { return gvar.MyInfo.ChrInfo.szName; }					// �̸�(ID)
	const char*				GetSurName() { return gvar.MyInfo.ChrInfo.szSurname; }			// First Name�� �ش��ϴ� �̸�
	int						GetLevel() { return gvar.MyInfo.nLevel; }				// ����
	int						GetBounty() { return gvar.MyInfo.ChrInfo.nBounty; }				// �ٿ�Ƽ(��)
	int						GetExpPercent() { return gvar.MyInfo.nExpPercent; }				// ����ġ �ۼ�Ʈ
	int						GetExp() { return gvar.MyInfo.ChrInfo.nXP; }					// ����ġ

	int						GetItemMaxWeight() { return gvar.MyInfo.ChrInfo.nMaxWeight; }	// ���� ������ ������ �� ����
	int						GetItemWeight() { return gvar.MyInfo.ChrInfo.nWeight; }			// ������ ����

	// �ɷ� ����
	int						GetSTR() { return gvar.MyInfo.ChrInfo.nSTR; }					// �Ϸ�
	int						GetDEX() { return gvar.MyInfo.ChrInfo.nDEX; }					// ȸ��
	int						GetINT() { return gvar.MyInfo.ChrInfo.nINT; }					// ����
	int						GetCON() { return gvar.MyInfo.ChrInfo.nCON; }					// HP�� �ִ뷮�� ����
	int						GetCHA() { return gvar.MyInfo.ChrInfo.nCHA; }					// ī������

	// MOD
	int						GetModSTR() { return gvar.MyInfo.ChrInfo.nModSTR; }				// +�Ϸ�
	int						GetModDEX() { return gvar.MyInfo.ChrInfo.nModDEX; }				// +ȸ��
	int						GetModINT() { return gvar.MyInfo.ChrInfo.nModINT; }				// +����
	int						GetModCON() { return gvar.MyInfo.ChrInfo.nModCON; }				// +HP�� �ִ뷮�� ����
	int						GetModCHA() { return gvar.MyInfo.ChrInfo.nModCHA; }				// +ī������

	// ���� ����
	int						GetMR() { return gvar.MyInfo.ChrInfo.nMR; }						// �⺻ ���� ���׷�
	int						GetFR() { return gvar.MyInfo.ChrInfo.nFR; }						// ȭ�Ӽ� ���׷�
	int						GetCR() { return gvar.MyInfo.ChrInfo.nCR; }						// ���Ӽ� ���׷�
	int						GetLR() { return gvar.MyInfo.ChrInfo.nLR; }						// ǳ�Ӽ� ���׷�
	int						GetPR() { return gvar.MyInfo.ChrInfo.nPR; }						// ���Ӽ� ���׷�

	// ���� ����
	int						GetHP() { return gvar.MyInfo.ChrStatus.nHP; }					// �����
	int						GetEN() { return gvar.MyInfo.ChrStatus.nEN; }					// ���ŷ�
	int						GetSTA() { return gvar.MyInfo.ChrStatus.nSTA; }					// ���׹̳�

	int						GetMaxHP() { return gvar.MyInfo.ChrInfo.nMaxHP; }				// �ִ� �����
	int						GetMaxEN() { return gvar.MyInfo.ChrInfo.nMaxEN; }				// �ִ� ���ŷ�
	int						GetMaxSTA() { return gvar.MyInfo.ChrInfo.nMaxSTA; }				// �ִ� ���׹̳�

	// �κ� ����
	void					UseItem(unsigned int nItemID_High, unsigned int nItemID_Low);	// ������ ���
	void					GetItemInfo(unsigned int nItemID_High, unsigned int nItemID_Low);// ������ ����
	const char*				GetItemName(unsigned int nItemID_High, unsigned int nItemID_Low);

	const char*				GetInventoryFirstItem();										// �κ��丮 ù��° ������
	const char*				GetInventoryNextItem();											// �κ��丮 ���� ������

	// ������� ����
	const char*				GetEquippedItem(const char* slot);

	// �������� ����

	// �ŷ�Ʈ ����

	// ������ ����

	// �ɸ� �ŷ�Ʈ

	// ���� ��뿡 ���� ó��

	// �� ����
	const char*				GetFieldName();
};
