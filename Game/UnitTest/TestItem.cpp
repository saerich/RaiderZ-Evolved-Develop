#include "stdafx.h"
#include "XItemContainer.h"
#include "XEquipmentSlot.h"
#include "CTransData.h"
#include "XItemManager.h"
#include "XStrings.h"
#include "XUnitTestList.h"

SUITE(Item)
{
	TEST(TestItemDataCook_NodeNames)
	{
		// ��� �������� ��� Node �̸��� '_item' �� ���� Node�� Cook�� �� �ڵ������� �߰����ش�.
		{
			CSItemData itemData;
			itemData.m_strNodeNames.push_back(wstring(L"hat"));
			itemData.Cooking();

			CHECK_EQUAL(2, itemData.m_strNodeNames.size());
			CHECK(wstring(L"hat") == itemData.m_strNodeNames[0]);
			CHECK(wstring(L"hat_item") == itemData.m_strNodeNames[1]);
		}

		// ��ҹ��� ���о���
		{
			CSItemData itemData;
			itemData.m_strNodeNames.push_back(wstring(L"Chest"));		
			itemData.Cooking();

			CHECK_EQUAL(2, itemData.m_strNodeNames.size());
			CHECK(wstring(L"Chest") == itemData.m_strNodeNames[0]);
			CHECK(wstring(L"Chest_item") == itemData.m_strNodeNames[1]);
		}


		// �̹� _item �� ������ �߰����� ����
		{
			CSItemData itemData;
			itemData.m_strNodeNames.push_back(wstring(L"feet_item"));
			itemData.m_strNodeNames.push_back(wstring(L"feet"));
			
			itemData.Cooking();

			CHECK_EQUAL(2, itemData.m_strNodeNames.size());
			CHECK(wstring(L"feet_item") == itemData.m_strNodeNames[0]);
			CHECK(wstring(L"feet") == itemData.m_strNodeNames[1]);
		}

	}
}
