#include "stdafx.h"
#include "GMailItemPutUper.h"
#include "CTransData.h"
#include "GItemHolder.h"
#include "GEntityPlayer.h"
#include "GPlayerMailReserver.h"
#include "GGlobal.h"
#include "GItemActableChecker.h"
#include "GNPCInteractor.h"
#include "GInteractionSystem.h"
#include "GNPCInteractionInfo.h"

GMailItemPutUper::GMailItemPutUper()
: m_nLastFailure(CR_FAIL)
{
	// do nothing
}

GMailItemPutUper::~GMailItemPutUper()
{
	// do nothing
}

bool GMailItemPutUper::Do(GEntityPlayer* pPlayer, const TD_MAIL_RESERVE_ITEM* pReserveItem)
{
	VALID_RET(pPlayer != NULL, false);
	VALID_RET(pReserveItem != NULL, false);


	// ������ Ȯ��
	int nSlotID = pReserveItem->nInventorySlotID;
	int nAmount = pReserveItem->nItemAmount;

	GItemHolder* pItemHolder = pPlayer->GetItemHolder();
	GItem* pItem = pItemHolder->GetItem(SLOTTYPE_INVENTORY, nSlotID);

	if (pItem == NULL)
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_NO_INVENTORY;
		return false;
	}


	// ÷���� �� �ִ��� Ȯ���Ѵ�.
	if (!Check(pPlayer, pItem, nAmount))
	{
		return false;
	}


	// ÷�� ��Ͽ� �߰��Ѵ�.
	GPlayerMailReserver& mailReserver = pPlayer->GetMailReserver();		
	bool nResult = mailReserver.PutUpItem(nSlotID, nAmount);

	return nResult;
}

bool GMailItemPutUper::Check(GEntityPlayer* pPlayer, const GItem* pItem, int nAddAmount)
{
	VALID_RET(pPlayer != NULL, false);
	VALID_RET(pItem != NULL, false);


	// ���� ���ͷ��� ���¸� ��Ȯ���Ѵ�.
	if (!CheckMailInteraction(pPlayer))
	{
		return false;
	}

	// ������ �Ӽ��� ÷�ΰ��� ���������� Ȯ���Ѵ�.
	if (!CheckAppendable(pPlayer, pItem))
	{
		return false;
	}

	// ÷�ΰ����� ������ ���� �ִ��� Ȯ���Ѵ�.
	if (!CheckItemAmount(pPlayer, pItem, nAddAmount))
	{
		return false;
	}


	return true;
}

bool GMailItemPutUper::CheckItemAmount(GEntityPlayer* pPlayer, const GItem* pItem, int nAddAmount)
{
	VALID_RET(pPlayer != NULL, false);
	VALID_RET(pItem != NULL, false);
	VALID_RET(0 < nAddAmount, false);

	// ÷���� ������ ������ ���� ������ ���� �� ����.
	int nSlotID = pItem->m_nSlotID;
	int nTotalAmount = pItem->GetAmount();

	GPlayerMailReserver& reserver = pPlayer->GetMailReserver();
	int nReservedAmount = reserver.GetAmount(nSlotID);

	if (nTotalAmount < nReservedAmount + nAddAmount)
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_NOT_ENOUGH_AMOUNT_UP;
		return false;
	}

	return true;
}

bool GMailItemPutUper::CheckAppendable(GEntityPlayer* pPlayer, const GItem* pItem)
{
	// �ͼ� �������� ÷�� ���Ѵ�.
	if (pItem->m_bBind)
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_BIND;
		return false;
	}

	// �Ⱓ�� �������� ÷�� ���Ѵ�.
	if (pItem->m_UsagePeriod.IsPeriod() || pItem->m_ExpirationPeriod.IsPeriod())
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_PERIOD;
		return false;
	}

	// ��ȭ �������� ÷�� ���Ѵ�.
	if (0 < pItem->GetEnchantCount())
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_ENCHANT;
		return false;
	}

	// ���� �ٸ� ��ɿ��� ó������ �������� ÷�� ���Ѵ�.
	if (!gsys.pItemSystem->GetActableChecker().IsMailAppendable(pPlayer, pItem))
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_BUSY;
		return false;
	}


	return true;
}

bool GMailItemPutUper::CheckMailInteraction(GEntityPlayer* pPlayer)
{
	VALID_RET(pPlayer != NULL, false);

	
	// ��ü��� ���ͷ��� ���̾�� �Ѵ�.
	GNPCInteractor& interactor = gsys.pInteractionSystem->GetNPCInteractor();
	
	if (!interactor.CheckIProgress(pPlayer, IT_MAILBOX))
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_NOT_INTERATION;
		return false;
	}


	return true;
}

CCommandResultTable GMailItemPutUper::GetLastFailure(void)
{
	return m_nLastFailure;
}
