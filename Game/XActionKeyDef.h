#pragma once

#include "XKeyBuffer.h"
#include "XMyActionID.h"

/// �� �׼� ����
enum SPECIAL_ACTION_COMMAND
{
	SPECIAL_KEY_NONE				= -1,
	SPECIAL_KEY_DODGE_FRONT			= 0,		///< ���� ȸ��
	SPECIAL_KEY_DODGE_BACK,						///< ���� ȸ��
	SPECIAL_KEY_DODGE_LEFT,						///< ���� ȸ��
	SPECIAL_KEY_DODGE_RIGHT,					///< ������ ȸ��
	SPECIAL_KEY_ADVANCE_ATTACK,					///< ���� ����
	SPECIAL_KEY_DOWNATTACK,						///< �ϴ� ����
	SPECIAL_KEY_UPPER,							///< ���� ����
	SPECIAL_KEY_SUPERATTACK,					///< Ư������
	SPECIAL_KEY_UPPERCOMBO,						///< ���� �� �޺�
	SPECIAL_KEY_SWORD_DRAWING,					///< �ߵ�
	SPECIAL_KEY_QUICK_RISE,						///< ���� ���
	SPECIAL_KEY_COUNTER_ATTACK,					///< ����� ����
	SPECIAL_KEY_GUARD,							///< ���

	SPECIAL_KEY_MAX
};

// �׼�Ű ����
struct XActionKey
{
	SPECIAL_ACTION_COMMAND		nSpecialActionCommand;
	float						m_fTotalTime;	// ����Ű�� �� �������� �ϴ� ���� �ð�
	vector<XVirtualKey>			sequenceKeys;	// ����Ű
	XVirtualKey					nExtKey;		// Ư�� Ű ���� ���� üũ

	size_t						GetKeyCount() 
	{
		return sequenceKeys.size();
	}
	XActionKey() : nSpecialActionCommand(SPECIAL_KEY_NONE), nExtKey(VKEY_NA)
	{
	}
};


class XActionKeyDef
{
protected:
	vector<XActionKey>		m_Table;

	void DefSeqKey(SPECIAL_ACTION_COMMAND nSpecialKey, int nKeyCount, XVirtualKey nKey, ... );
	void DefSeqKeyExt(SPECIAL_ACTION_COMMAND nSpecialKey, XVirtualKey nExtKey, int nKeyCount, XVirtualKey nKey, ... );
public:
	XActionKeyDef();
	~XActionKeyDef() {}

	XActionKey& GetTable(int nIndex) { return m_Table[nIndex]; }
	size_t GetSize() { return m_Table.size(); }
};