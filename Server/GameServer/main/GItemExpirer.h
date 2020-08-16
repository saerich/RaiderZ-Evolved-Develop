#pragma once

#include "GTickRegulator.h"

class GEntityPlayer;
class GItem;


typedef	vector<GItem*> GExprieItemVec;


class GItemExpirer : public MTestMemPool<GItemExpirer>
{
public:
	GItemExpirer(GEntityPlayer* pPlayer);
	~GItemExpirer(void);

	void Init();

	void Run(float fDelta);
	void RunImmediately();

	GExprieItemVec& GetExpiredItemList();		///< �Ⱓ ���� ������ ������ ���. Run(), RunImmediately()�� ȣ�� �� �Ŀ� ����ؾ� �Ѵ�.

private :
	void ExpireItems();
	void BuildExpiredItemList();
	void ExpireEachItem();

private:
	GEntityPlayer*	m_pOwnerPlayer;
	GTickRegulator	m_CheckTimeRegulator;
	GExprieItemVec	m_vecExpiredItem;
};
