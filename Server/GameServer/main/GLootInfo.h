#ifndef _GLOOT_INFO_H_
#define _GLOOT_INFO_H_

#include "CSSoul.h"

struct LOOT_ITEM_INFO : public MTestMemPool<LOOT_ITEM_INFO>
{
	int		m_nID;				// �������� ID
	float	m_fRate;			// �������� ���� Ȯ��
	int		m_nMin;				// �������� �������� �ּ� ����
	int		m_nMax;				// �������� �������� �ִ� ����
	int		m_nBreakPart;		// �극��Ŀ�� ������ �ı��Ǿ����� ����Ǵ� ������.
	int		m_nQuestPVPFieldID;	// ����ƮPVP ���������� ����Ǵ� ������
	vector<int>	m_vecQuestID;	// ����Ʈ�� �ϳ��� �ִ� ��Ƽ�����Ը� ����Ǵ� ������	

	LOOT_ITEM_INFO()
	: m_nID(0)
	, m_fRate(100.0f)
	, m_nMin(1)
	, m_nMax(1)
	, m_nBreakPart(0)
	, m_nQuestPVPFieldID(false)
	{

	}
};

struct LOOT_MONEY_INFO : public MTestMemPool<LOOT_MONEY_INFO>
{
	float	m_fRate;			// �ǹ��� ���� Ȯ��
	int		m_nMin;				// �ǹ��� �������� �ּҷ�
	int		m_nMax;				// �ǹ��� �������� �ִ뷮

	LOOT_MONEY_INFO()
	: m_fRate(100.0f)
	, m_nMin(1)
	, m_nMax(1)
	{

	}
};

struct LOOT_SOUL_INFO : public MTestMemPool<LOOT_SOUL_INFO>
{
	SOUL_TYPE m_nType;

	float	m_fRate;		// ��ȥ�� ���� Ȯ��
	float	m_fBPart1Rate;	// ����1 �ı��� Ȯ�� ������
	float	m_fBPart2Rate;	// ����2 �ı��� Ȯ�� ������
	float	m_fBPart3Rate;	// ����3 �ı��� Ȯ�� ������
	float	m_fBPart4Rate;	// ����4 �ı��� Ȯ�� ������

	int		m_nMin;				// ��ȥ�� �������� �ּҷ�
	int		m_nMax;				// ��ȥ�� �������� �ִ뷮

	LOOT_SOUL_INFO()
	: m_nType(static_cast<SOUL_TYPE>(0))
	, m_fRate(100.0f)
	, m_fBPart1Rate(0.0f)
	, m_fBPart2Rate(0.0f)
	, m_fBPart3Rate(0.0f)
	, m_fBPart4Rate(0.0f)
	, m_nMin(1)
	, m_nMax(1)
	{

	}
};

typedef vector<LOOT_ITEM_INFO*> LOOT_ITEM_VEC;
typedef vector<LOOT_MONEY_INFO*> LOOT_MONEY_VEC;
typedef vector<LOOT_SOUL_INFO*> LOOT_SOUL_VEC;
class GLootInfo : public MTestMemPool<GLootInfo>
{
public:
	bool m_bBPartLoot;
	LOOT_ITEM_VEC m_vecLootItems;
	LOOT_MONEY_VEC m_vecLootMoneys;
	LOOT_SOUL_VEC m_vecLootSouls;
	vector<int> m_vecLootBuffs;
	int	m_nID;
	int m_nLimitDropQuantity;		// ���ÿ� ���ü� �ִ� �������� �ִ� ����(����Ʈ�� �극��ũ��Ʈ�� ������� ����)
	bool m_bValidation;
public:
	GLootInfo();
	~GLootInfo();

	void Clear();
};

#endif //_GLOOT_INFO_H_
