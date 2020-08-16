#include "StdAfx.h"
#include "GItemInfoValidator.h"
#include "GServerSystems.h"
#include "GDef.h"
#include "GItemManager.h"
#include "GValidateLogger.h"
#include "CSSoul.h"
#include "CSStrings.h"
#include "GBuffInfo.h"

bool GItemInfoValidator::Check()
{
	if (gmgr.pItemManager == NULL)	return true;

	_VLI(FILENAME_ITEMINFO);
	for (GItemManager::GItemDataMap::iterator itor = gmgr.pItemManager->Begin(); itor != gmgr.pItemManager->End(); ++itor)
	{
		GItemData* pItemData = (*itor).second;
		int nItemID = (*itor).first;

		_ASSERT(NULL != pItemData);
		if (NULL == pItemData) continue;
		if (!pItemData->m_bValidation) continue;

		_VLE(_T(ITEM_XML_TAG_ITEM));
		_VLA(_T(ITEM_XML_ATTR_ID), nItemID);
		_VLA(_T(ITEM_XML_ATTR_NAME), pItemData->m_strName);
		_VLP;

		_VLOGGER->ValidateID(nItemID);

		CheckStackAmount(pItemData);
		CheckSoul(pItemData);
		CheckSellable(pItemData);
		CheckTier(pItemData);
		CheckUsableParam(pItemData);
		CheckGather(pItemData);
		CheckEnchant(pItemData);
		CheckArmorType(pItemData);
	}

	return (_VLOGGER->GetValidateResult() && GetReturnValue());
}

void GItemInfoValidator::CheckStackAmount(GItemData* pItemData)
{
	if (NULL == pItemData) return;

	if (0 < pItemData->m_nMaxDurability && 1 < pItemData->m_nStackAmount)
	{
		_VLOGGER->Log(L"�������� �ִ� �������� ���õ� �� �����ϴ�.\n");
	}

	if (true == pItemData->m_bUnique && 1 < pItemData->m_nStackAmount)
	{
		_VLOGGER->Log(L"�Ѱ��� ������ �� �ִ� �������� ���õ� �� �����ϴ�.\n");
	}

	if (true == pItemData->m_bClaimRequired && 1 < pItemData->m_nStackAmount)
	{
		_VLOGGER->Log(L"����� �ͼ� �Ǵ� �������� ���õ� �� �����ϴ�.\n");
	}
}

void GItemInfoValidator::CheckSoul( GItemData* pItemData )
{
	if (NULL == pItemData) return;

	if (ARMOR_AMULET != pItemData->m_nArmorType)
	{
		if (SOULT_NONE != pItemData->m_nSoulType)
		{
			_VLOGGER->Log(L"��� Ÿ���� �ֹķ��� �����۸� soul_type_T()�� ������ �޽��ϴ�.\n");
		}

		if (0 < pItemData->m_nSoulMaxQuantity)
		{
			_VLOGGER->Log(L"��� Ÿ���� �ֹķ��� �����۸� soul_maxquantity_T()�� ������ �޽��ϴ�.\n");
		}

		if (0 < pItemData->m_nSoulLinkedItem)
		{
			_VLOGGER->Log(L"��� Ÿ���� �ֹķ��� �����۸� soul_stoneitem_T()�� ������ �޽��ϴ�.\n");
		}			
	}
}

void GItemInfoValidator::CheckTier(GItemData* pItemData)
{
	if (NULL == pItemData) return;

	if (ITEM_TIER_VERY_COMMON == pItemData->m_nTier)
	{
		if (ITEMTYPE_WEAPON == pItemData->m_ItemType)
		{
			_VLOGGER->Log(L"���� �������� ����� �ſ� �������� ������ �� �����ϴ�.\n");
		}
		else if (ITEMTYPE_ARMOR == pItemData->m_ItemType)
		{
			_VLOGGER->Log(L"�� �������� ����� �ſ� �������� ������ �� �����ϴ�.\n");
		}
	}
}

void GItemInfoValidator::CheckSellable(GItemData* pItemData)
{
	if (NULL == pItemData) return;

	if (false == pItemData->m_bSellable)
	{
		if (ITEM_TIER_VERY_COMMON == pItemData->m_nTier)
		{
			_VLOGGER->Log(L"�ǸŰ� �Ұ����� �������� ����� �ſ� �������� ������ �� �����ϴ�.\n");
		}
	}
	else
	{
		if (true == pItemData->m_bQuestRelated)
		{
			_VLOGGER->Log(L"����Ʈ �������� �ǸŰ� �Ұ��� �ؾ� �մϴ�.\n");
		}
	}
}

void GItemInfoValidator::CheckUsableParam(GItemData* pItemData)
{   
	if (pItemData->m_ItemType != ITEMTYPE_USABLE)	return;

	if (pItemData->m_nUsableType != USABLE_TALENT_TRAIN &&
		pItemData->m_nUsableType != USABLE_INTERACTION &&
		pItemData->m_nUsableType != USABLE_INTERACTION_DEAD)
	{
		// ���� Ÿ�Ը�, �������� ���� ���� �� ����.
		if (1 < pItemData->m_vecUsableParam.size())
		{
			_VLOGGER->Log(L"�ش� Ÿ���� �������� usable_param_T()�� �Ѱ��� ���� �������մϴ�.\n");
		}
	}
	
	if (pItemData->m_nUsableType == USABLE_TALENT_TRAIN)
	{
		// �ŷ�Ʈ ���� �������� ���õ��� �ʽ��ϴ�.
		if (pItemData->m_nStackAmount != 1)
		{
			_VLOGGER->Log(L"UsableType�� train_talent�� �������� StackAmount�� �ݵ�� 1_T()�̿����մϴ�.\n");
		}

		// usable_param�� �ߺ����� ���� �� ����.
		vector<int> vecParam = pItemData->m_vecUsableParam;
		
		sort(vecParam.begin(), vecParam.end());
		vector<int>::iterator it = unique(vecParam.begin(), vecParam.end());
		vecParam.resize(it - vecParam.begin());

		if (vecParam.size() != pItemData->m_vecUsableParam.size())
		{
			_VLOGGER->Log(L"usable_param_T()�� �ߺ��� ���� �����ϰ� �ֽ��ϴ�.\n");
		}


		// ��� talent_train �������� usable_param�� ���ļ��� �ȵȴ�.
		for each(int nTalentID in pItemData->m_vecUsableParam)
		{
			map<talent_id, item_id>::iterator it;
			it = m_mapTrainTalent.find(nTalentID);

			if (it == m_mapTrainTalent.end())
			{
				m_mapTrainTalent.insert(make_pair(nTalentID, pItemData->m_nID));
				continue;
			}
		
			_VLOGGER->Log(L"�ش� �ŷ�Ʈ(%d)�� ���̼��� ������(%d)�� �̹� �ֽ��ϴ�.\n", nTalentID, pItemData->m_nID);
		}
	}
}

void GItemInfoValidator::CheckGather(GItemData* pItemData)
{
	if (pItemData->m_ItemType != ITEMTYPE_GATHER)	return;

	if (pItemData->m_nGatherType == GATHER_NONE)
	{		
		_VLOGGER->Log(L"%s�� �����ϴ�.\n", L"gather_type");
	}

	if (pItemData->m_nGatherType == GATHER_LUMBER)
	{
		_VLOGGER->Log(L"%s�� %s�� ������ ������ �ʽ��ϴ�.\n", L"gather_type", CSStrings::GatherTypeToString(GATHER_LUMBER));
	}
}

void GItemInfoValidator::CheckEnchant(GItemData* pItemData)
{
	int nBuffID = pItemData->m_EnchantStone.nLinkedBuffID;
	if (nBuffID == INVALID_BUFF_ID)	
		return;

	GBuffInfo* pBuffInfo = gmgr.pBuffInfoMgr->Get(nBuffID);
	if (pBuffInfo)
	{
		if (!pBuffInfo->IsEnchant())
		{
			_VLOGGER->Log(L"��ȭ���� ����� ������ ��ȭ�� ������ �ƴմϴ�.\n");
		}
	}
	else
	{
		_VLOGGER->Log(L"��ȭ�� ���� ���� ������ ã�� �� �����ϴ�.\n");
	}

	if (pItemData->m_EnchantStone.nColor == ENCHANT_NONE)
	{		
		_VLOGGER->Log(L"��ȭ�� ������ �����Ǿ����� �ʽ��ϴ�.\n");
	}

	if (!pItemData->m_vecEquipEnchantColor.empty())
	{		
		if (ENCHANT_WHITE != pItemData->m_vecEquipEnchantColor.front())
		{
			_VLOGGER->Log(L"ù��° ��ȭ ������ W�� ���� �����մϴ�.\n");
		}
		
		for (size_t i=1; i<pItemData->m_vecEquipEnchantColor.size(); ++i)
		{
			if (ENCHANT_WHITE == pItemData->m_vecEquipEnchantColor[i])
			{
				_VLOGGER->Log(L"ù��° ��ȭ ���Ը� W�� ���� �����մϴ�.\n");
			}
		}

		if (pItemData->m_vecEquipEnchantColor.size() > ENCHANT_MAX_COUNT)
		{
			_VLOGGER->Log(L"��ȭ ���� �ִ� ������ �ʰ��߽��ϴ�. (�ִ밹��: %d)\n", ENCHANT_MAX_COUNT);
		}
	}
}

void GItemInfoValidator::CheckArmorType(GItemData* pItemData)
{
	if (pItemData->m_ItemType != ITEMTYPE_ARMOR)	return;

	if (pItemData->m_nArmorType == ARMOR_NONE)
	{
		_VLOGGER->Log(L"������ Ÿ���� �Ƹ��� �������� �Ƹ�Ÿ���� ���� ���� �ʾҽ��ϴ�.\n");
	}
}