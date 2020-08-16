#include "StdAfx.h"
#include "GBuffStack.h"
#include "GBuff.h"
#include "GBuffInfo.h"
#include "GEntityActor.h"
#include "CCommandTable.h"
#include "GCommand.h"

GBuffStack::GBuffStack(GBuff* pBuff)
: m_pOwner(pBuff)
, m_nCount(0)
{
}

GBuffStack::~GBuffStack(void)
{
}

void GBuffStack::Increase( float fDurationTime )
{
	const int nStackMaxCount = m_pOwner->GetInfo()->m_nStackMaxCount;

	if (nStackMaxCount != 0 && // 0�� ��쿡�� ���Ѵ�� ��ø
		m_nCount >= nStackMaxCount)
	{
		Decrease(false);
	}

	++m_nCount;

	if (m_pOwner->GetInfo()->m_nStackType == BUFFSTACK_STACK)
	{
		m_qDurations.push_back(fDurationTime);
		std::sort(m_qDurations.begin(), m_qDurations.end());
	}

	RouteStackIncrease();
}

void GBuffStack::Decrease(bool bRoute)
{
	--m_nCount;

	if (m_pOwner->GetInfo()->m_nStackType == BUFFSTACK_STACK)
	{
		// ���� ���� �ð����� ���ŵ�
		m_qDurations.pop_front();
	}

	if (bRoute)
	{
		RouteStackDecrease();
	}
}

int GBuffStack::GetCount() const
{
	return m_nCount;
}

bool GBuffStack::IsExpired() const
{
	return GetCount() < 1;
}

void GBuffStack::Update_Expired( float fDelta )
{
	for (deque<float>::iterator it = m_qDurations.begin(); 
		 it != m_qDurations.end();)
	{
		float& fDuration = *it;
		fDuration -= fDelta;

		if (fDuration != BUFF_DURATION_INFINITY && 
			fDuration <= 0.0f)
		{
			// �ð� ����
			it = m_qDurations.erase(it);
			--m_nCount;
			RouteStackDecrease();
		}
		else
		{
			++it;
		}
	}
}

void GBuffStack::RouteStackIncrease()
{
	if (m_nCount == 1)
		return; // ó�� ��� ������ GAIN ��Ŷ���� ó��

	GEntitySync* pOwnerActor = m_pOwner->GetOwner();
	VALID(pOwnerActor);

	if (!pOwnerActor->IsActor())	
		return;

	MCommand* pNewCommand = MakeNewCommand(MC_BUFF_STACK_INCREASE, 3, 
		NEW_USHORT(ToEntityActor(pOwnerActor)->GetUIID()), 
		NEW_INT(m_pOwner->GetBuffID()), 
		NEW_FLOAT(m_pOwner->GetDurationTime()));

	pOwnerActor->RouteToThisCell(pNewCommand);
}

void GBuffStack::RouteStackDecrease()
{
	if (m_nCount <= 0)
		return;  // �������� �Ҵ� ������ LOST ��Ŷ���� ó��

	GEntitySync* pOwnerActor = m_pOwner->GetOwner();
	VALID(pOwnerActor);

	if (!pOwnerActor->IsActor())	
		return;

	MCommand* pNewCommand = MakeNewCommand(MC_BUFF_STACK_DECREASE, 2,
		NEW_USHORT(ToEntityActor(pOwnerActor)->GetUIID()), 
		NEW_INT(m_pOwner->GetBuffID()));

	pOwnerActor->RouteToThisCell(pNewCommand);
}