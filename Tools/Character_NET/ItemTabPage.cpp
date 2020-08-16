#include "StdAfx.h"
#include "ItemTabPage.h"
#include "TItemData.h"

//�������� ���� �ε������ ����Ʈ�� �����ߵǳ�...
// TODO: �𵨿��� ������ ���� ������ �´� �����۸���Ʈ�� ����ϰ�
// �ε�� ���ص� ��� ���� �ʳ�???
//�������� �з��� �� �ְ� �����ؾ��ҵ�..
void Character_NET::ItemTabPage::InitItemList()
{
	ItemtreeView->Nodes->Clear();

	for( int i = 0;i < ITEMSLOT_MAX; ++i)
	{
		String^ str = gcnew String(CSItemSlotStr[i]);
		TreeNode^ node = ItemtreeView->Nodes->Add(str);

		SlotList* pSlotList = g_pMainApp->m_TabItem.SelectItem(i);
		if (pSlotList)
		{
			int size = pSlotList->m_Index.size();
			for(int j = 0; j < size; j++)
			{
				if (j == 0)
				{
					TreeNode^ tagnode0 = node->Nodes->Add("None");
					tagnode0->Tag = nullptr;
					continue;
				}

				String^ Name = gcnew String(pSlotList->m_Index[j]->m_strName.c_str());
				Name = Name  + "(" + pSlotList->m_Index[j]->m_nID + ")";
				TreeNode^ tagnode = node->Nodes->Add(Name);
				tagnode->Tag = pSlotList->m_Index[j]->m_nID; //������ ���̵��߰�
			}
		}
	}
}
