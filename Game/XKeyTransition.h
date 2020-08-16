#pragma once

// ���� ����
enum : unsigned int
{
	WC_NONE = 1,
	WC_1HS	= (1<<1),
	WC_1HB	= (1<<2),
	WC_1HP	= (1<<3),
	WC_2HD	= (1<<4),
	WC_STF	= (1<<5),
	WC_ARC	= (1<<6),
	WC_2HB	= (1<<7),
	WC_DWD	= (1<<8),
	WC_DWP	= (1<<9),

	WC_ALL	= 0xFFFFFFFF
};

// Ű ����
enum XConditionKeyState
{
	_PRESSED		= 1,			///< ��������
	_PRESSEDONCE	= (1<<1),		///< �ѹ��� ��������
	_RELEASED		= (1<<2),		///< ��������
	_KEY_FORWARD	= (1<<3),		///< ��Ű�� �Բ� ��������
	_KEY_DOWN		= (1<<4),		///< ��Ű�� �Բ� ��������
	_KEY_LEFT		= (1<<5),		///< ����Ű�� �Բ� ��������
	_KEY_RIGHT		= (1<<6),		///< ������Ű�� �Բ� ��������
};

// �ڼ� ����
enum XConditionStance : unsigned int
{
	_STANCE_NORMAL	= 1,
	_STANCE_BATTLE	= (1<<1),
	_STANCE_ALL		= 0xFFFFFFFF
};

// �� �׼� ���� ���� ����
struct XMyActionTransCondition
{
	XVirtualKey		nKey;				///< Ű
	unsigned int	nKeyState;			///< ����Ű�� ����� ����(���� �Ⱦ���-����Ű�� ����..-_-)
	unsigned int	nWeaponState;		///< ��� �ִ� ���� Ÿ��(WEAPON_NONE�̸� �ƹ����⳪ �������)
	unsigned int	nStances;			///< �ڼ�
};

// �� �׼� ����
struct XMyActionTransition
{
	ACTION_STATE				nNextState;		///< ���� ����
	XMyActionTransCondition		Condition;		///< ����
};


class XKeyConditionChecker
{
private:
	vector<XMyActionTransition>		m_vecMyActionTransitions;
public:
	void AddCondition(XMyActionTransition& transition);
	ACTION_STATE CheckKeyCondition(XVirtualKey nVirtualKey, unsigned int nKeyState, CHAR_STANCE nCurrStance);
};
