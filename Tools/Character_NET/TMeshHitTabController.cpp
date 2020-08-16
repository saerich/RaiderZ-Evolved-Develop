#include "stdafx.h"
#include "TMeshHitTabController.h"
#include "TPartsHitInfoViewer.h"
#include "TClipboard.h"


TMeshHitTabController::TMeshHitTabController() : m_pMeshColInfoViewer(NULL), m_pPartsHitInfoViewControl(NULL)
{

}

TMeshHitTabController::~TMeshHitTabController()
{

}

void TMeshHitTabController::DelCollision_Click( System::Object^ sender, System::EventArgs^ e )
{
	ArrayList^ arrSelectedCapsuleInfo = GlobalObjects::g_pMainForm->CollisionCapsulesView->SelectedNodes;

	int nCount = arrSelectedCapsuleInfo->Count;
	if(nCount == 0)
	{
		System::Windows::Forms::MessageBox::Show(gcnew String("���õ� �浹������ �����ϴ�."), "�浹 ���� ���� ����");
		return;
	}

	if (System::Windows::Forms::MessageBox::Show(String::Format("���õ� ���� {0}���� ���� �Ͻðڽ��ϱ�?", nCount), "�浹 ���� ����"
		,MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes)
	{
		vector<int> vecDelIndex;
		System::Collections::IEnumerator^ myEnumerator = arrSelectedCapsuleInfo->GetEnumerator();
		while ( (myEnumerator->MoveNext()) && (myEnumerator->Current != nullptr) )
		{
			TreeNode^ node = safe_cast<TreeNode^>(myEnumerator->Current);
			Object^ tag	= node->Tag;
			if(tag == nullptr)
				continue;

			int nDelIndex = (int)tag;
			vecDelIndex.push_back(nDelIndex);
		}

		std::sort(vecDelIndex.begin(), vecDelIndex.end(), descendingInt);
		for(vector<int>::iterator it = vecDelIndex.begin(); it != vecDelIndex.end(); ++it)
		{
			if(*it < COLLISION_BOX_START_INDEX)
				g_pMainApp->DelMeshColCapsuleInfo(*it);
			else
				g_pMainApp->DelMeshColBoxInfo(*it);
		}
	}
}

void TMeshHitTabController::AddCollisionCapsule_Click( System::Object^ sender, System::EventArgs^ e )
{
	// ���õ� ��尡 �ִٸ�...
	g_pMainApp->CreateMeshColCapsuleInfo();
}

void TMeshHitTabController::CopyColCapsule()
{
	ArrayList^ arrSelectedColInfo = GlobalObjects::g_pMainForm->CollisionCapsulesView->SelectedNodes;

	Character_NET::TClipboardGroupCapsuleBoxData^ clipboardColList = gcnew Character_NET::TClipboardGroupCapsuleBoxData();

	System::Collections::IEnumerator^ myEnumerator = arrSelectedColInfo->GetEnumerator();
	while ( (myEnumerator->MoveNext()) && (myEnumerator->Current != nullptr) )
	{
		TreeNode^ node = safe_cast<TreeNode^>(myEnumerator->Current);
		Object^ tag	= node->Tag;
		if(tag == nullptr)
			continue;

		if((int)tag < COLLISION_BOX_START_INDEX)
		{
			CSColCapsule* pColCapsule = m_pMeshColInfoViewer->GetMeshColCapsuleInfo((int)tag);
			if(pColCapsule == NULL)
				continue;

			Character_NET::TClipboardCapsuleData^ clipboardCapsuleData = gcnew Character_NET::TClipboardCapsuleData();
			clipboardCapsuleData->Capsule = pColCapsule->m_Capsule;
			clipboardCapsuleData->strBoneTop = gcnew String(pColCapsule->m_strBoneTop.c_str());
			clipboardCapsuleData->strBoneBottom = gcnew String(pColCapsule->m_strBoneBottom.c_str());

			clipboardColList->m_Capsule.CopyFrom(clipboardCapsuleData);
		}
		else
		{
			CSColBox* pColBox = m_pMeshColInfoViewer->GetMeshColBoxInfo((int)tag);
			if(pColBox == NULL)
				continue;

			Character_NET::TClipboardBoxData^ clipboardBoxData = gcnew Character_NET::TClipboardBoxData();
			clipboardBoxData->colBox = *pColBox;

			clipboardColList->m_Box.CopyFrom(clipboardBoxData);
		}

	}

	// Ŭ������� ����
	TClipboardHelper::CopyGroupCapsuleData(clipboardColList);
}

void TMeshHitTabController::PasteColCapsule()
{
	TClipboardCollisionData^ clipboardColList = TClipboardHelper::PasteGroupCapsuleData();

	if (clipboardColList == nullptr) return;

	int nCount = clipboardColList->m_Capsule.m_arrCapsule->Count;
	for(int i = 0; i < nCount; ++i)
	{
		Character_NET::TClipboardCapsuleData^ clipboardCapsuleData = gcnew Character_NET::TClipboardCapsuleData();
		clipboardColList->m_Capsule.CopyTo(i, clipboardCapsuleData);

		// ����
		g_pMainApp->CreateMeshColCapsuleInfo();

		int nSelCapsuleID = GlobalObjects::g_SelInfo.m_nMeshColID;

		CSColCapsule* pColCapsule = m_pMeshColInfoViewer->GetMeshColCapsuleInfo(nSelCapsuleID);
		if (pColCapsule == NULL) 
			continue;

		// ������ �Է�
		pColCapsule->m_Capsule = clipboardCapsuleData->Capsule;

		if (clipboardCapsuleData->strBoneTop != nullptr)
		{
			const char* cstr = MStringToCharPointer(clipboardCapsuleData->strBoneTop);
			pColCapsule->m_strBoneTop = cstr;
			MFreeCharPointer(cstr);
		}

		if (clipboardCapsuleData->strBoneBottom != nullptr)
		{
			const char* cstr = MStringToCharPointer(clipboardCapsuleData->strBoneBottom);
			pColCapsule->m_strBoneBottom = cstr;
			MFreeCharPointer(cstr);
		}
	}

	nCount = clipboardColList->m_Box.m_arrBox->Count;
	for(int i = 0; i < nCount; ++i)
	{
		Character_NET::TClipboardBoxData^ clipboardBoxData = gcnew Character_NET::TClipboardBoxData();
		clipboardColList->m_Box.CopyTo(i, clipboardBoxData);

		// ����
		g_pMainApp->CreateMeshColBoxInfo();

		int nSelBoxID = GlobalObjects::g_SelInfo.m_nMeshColID;

		CSColBox* pColBox = m_pMeshColInfoViewer->GetMeshColBoxInfo(nSelBoxID);
		if (pColBox == NULL) 
			continue;

		// ������ �Է�
		*pColBox = clipboardBoxData->colBox;
	}

	Serialize();
}

void TMeshHitTabController::Init( TMeshColInfoViewer* pMeshColInfoViewer, TPartsHitInfoViewer* pPartsHitInfoViewControl, TMeshHitEffectInfoViewer* pMeshHitEffectInfoViewer )
{
	m_pMeshColInfoViewer = pMeshColInfoViewer;
	m_pPartsHitInfoViewControl = pPartsHitInfoViewControl;
	m_pMeshHitEffectInfoViewer = pMeshHitEffectInfoViewer;
}

void TMeshHitTabController::Serialize()
{
	if (GlobalObjects::g_pMainForm->CollisionCapsulesView->Visible)
	{
		m_pMeshColInfoViewer->SetMeshColData(GlobalObjects::g_SelInfo.m_nMeshColID);
	}

	if (GlobalObjects::g_pMainForm->PartsHitCapsulesView->Visible)
	{
		m_pPartsHitInfoViewControl->SetPartsHitCapsuleData(GlobalObjects::g_SelInfo.m_nMeshHitGroupID, GlobalObjects::g_SelInfo.m_nMeshHitCapsuleID);
	}

	if(GlobalObjects::g_pMainForm->HitEffect_treeView->Visible)
	{
		m_pMeshHitEffectInfoViewer->SetMeshHitEffectCapsuleData(GlobalObjects::g_SelInfo.m_nMeshHitEffectCapsuleID);
	}
}

void TMeshHitTabController::CopyHitCapsule()
{
	ArrayList^ arrSelectedHitInfo = GlobalObjects::g_pMainForm->PartsHitCapsulesView->SelectedNodes;

	Character_NET::TClipboardGroupCapsuleBoxData^ clipboardCapsuleList = gcnew Character_NET::TClipboardGroupCapsuleBoxData();

	System::Collections::IEnumerator^ myEnumerator = arrSelectedHitInfo->GetEnumerator();
	while ( (myEnumerator->MoveNext()) && (myEnumerator->Current != nullptr) )
	{
		TreeNode^ node = safe_cast<TreeNode^>(myEnumerator->Current);
		Object^ tag	= node->Tag;
		if(tag == nullptr)
			continue;

		if(node->Level == 0)
		{
			// �׷�
			Character_NET::TClipboardCollisionData^ clipboardGroupData = gcnew Character_NET::TClipboardCollisionData();
			CSHitGroup* pCopyHitGroup = m_pPartsHitInfoViewControl->GetPartsHitGroupInfo((int)tag);
			if(pCopyHitGroup == NULL)
				continue;

			for(vector<CSHitCapsule>::iterator it = pCopyHitGroup->m_vecHitCapsules.begin(); it != pCopyHitGroup->m_vecHitCapsules.end(); ++it)
			{
				Character_NET::TClipboardCapsuleData^ clipboardCapsuleData = gcnew Character_NET::TClipboardCapsuleData();
				clipboardCapsuleData->Capsule = it->m_Capsule;
				clipboardCapsuleData->fActiveTimeBegin = it->GetActiveTimeBegin();
				clipboardCapsuleData->fActiveTimeEnd = it->GetActiveTimeEnd();
				clipboardCapsuleData->nLinkedBPart = it->nLinkedBPart;
				clipboardCapsuleData->nHitEffectCapsuleID = it->m_nHitEffectCapsuleID;
				
				clipboardGroupData->m_Capsule.CopyFrom(clipboardCapsuleData);
			}

			clipboardCapsuleList->AddGroupData(clipboardGroupData);
		}
		else
		{
			// ĸ��
			Object^ parentTag = node->Parent->Tag;

			CSHitGroup* pHitGroup = m_pPartsHitInfoViewControl->GetPartsHitGroupInfo((int)parentTag);
			if(pHitGroup == NULL)
				continue;

			CSHitCapsule* pHitCapsule = m_pPartsHitInfoViewControl->GetPartsHitCapsuleInfo((int)parentTag, (int)tag);
			if(pHitCapsule == NULL)
				continue;

			Character_NET::TClipboardCapsuleData^ clipboardCapsuleData = gcnew Character_NET::TClipboardCapsuleData();
			clipboardCapsuleData->Capsule = pHitCapsule->m_Capsule;
			clipboardCapsuleData->fActiveTimeBegin = pHitCapsule->GetActiveTimeBegin();
			clipboardCapsuleData->fActiveTimeEnd = pHitCapsule->GetActiveTimeEnd();
			clipboardCapsuleData->nLinkedBPart = pHitCapsule->nLinkedBPart;
			clipboardCapsuleData->nHitEffectCapsuleID = pHitCapsule->m_nHitEffectCapsuleID;

			clipboardCapsuleList->m_Capsule.CopyFrom(clipboardCapsuleData);
		}
	}

	TClipboardHelper::CopyGroupCapsuleData(clipboardCapsuleList);
}

void TMeshHitTabController::PasteHitCapsule()
{
	TClipboardGroupCapsuleBoxData^ clipboardCapsuleList = TClipboardHelper::PasteGroupCapsuleData();

	if (clipboardCapsuleList == nullptr) return;

	int nSelectGroupID = -1;
	TreeNode^ node = GlobalObjects::g_pMainForm->PartsHitCapsulesView->SelectedNode;
	if(node != nullptr)
	{
		Object^ tag = node->Tag;
		nSelectGroupID = (int)tag;
		if(node->Level == 1)
		{
			nSelectGroupID = (int)node->Parent->Tag;
		}
	}

	// �׷�
	int nGroupCount = clipboardCapsuleList->m_arrGroup->Count;
	for(int j = 0; j < nGroupCount; ++j)
	{
		TClipboardCollisionData^ pCopySegmentData = clipboardCapsuleList->GetGroupData(j);

		int nNewGroupID = g_pMainApp->CreateMeshHitGroupInfo();
		CSHitGroup* pNewHitGroup = m_pPartsHitInfoViewControl->GetPartsHitGroupInfo(nNewGroupID);
		if(pNewHitGroup == NULL)
			continue;

		for(int nCapsuleIndex = 0; nCapsuleIndex < pCopySegmentData->m_Capsule.m_arrCapsule->Count; ++nCapsuleIndex)
		{
			Character_NET::TClipboardCapsuleData^ clipboardCapsuleData = gcnew Character_NET::TClipboardCapsuleData();
			pCopySegmentData->m_Capsule.CopyTo(nCapsuleIndex, clipboardCapsuleData);

			int nNewCapsuleID = g_pMainApp->CreateMeshHitCapsuleInfo(nNewGroupID);
			CSHitCapsule* pNewHitCapsule = m_pPartsHitInfoViewControl->GetPartsHitCapsuleInfo(nNewGroupID, nNewCapsuleID);
			if(pNewHitCapsule == NULL)
				continue;

			pNewHitCapsule->m_Capsule = clipboardCapsuleData->Capsule;
			pNewHitCapsule->SetActiveTime(clipboardCapsuleData->fActiveTimeBegin, clipboardCapsuleData->fActiveTimeEnd);
			pNewHitCapsule->m_nHitEffectCapsuleID = clipboardCapsuleData->nHitEffectCapsuleID;
			pNewHitCapsule->nLinkedBPart = clipboardCapsuleData->nLinkedBPart;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ĸ��
	int nCapsuleCount = clipboardCapsuleList->m_Capsule.m_arrCapsule->Count;

	if(nCapsuleCount > 0 && nSelectGroupID == -1)
	{
		//���õ� �׷��� ������ ������...
		nSelectGroupID = g_pMainApp->CreateMeshHitGroupInfo();
	}

	for(int i = 0; i < nCapsuleCount; ++i)
	{
		Character_NET::TClipboardCapsuleData^ clipboardCapsuleData = gcnew Character_NET::TClipboardCapsuleData();
		clipboardCapsuleList->m_Capsule.CopyTo(i, clipboardCapsuleData);

		// ����
		int nNewCapsuleID = g_pMainApp->CreateMeshHitCapsuleInfo(nSelectGroupID);
		CSHitCapsule* pHitCapsule = m_pPartsHitInfoViewControl->GetPartsHitCapsuleInfo(nSelectGroupID, nNewCapsuleID);
		if(pHitCapsule == NULL)
			continue;


		// ������ �Է�
		pHitCapsule->m_Capsule = clipboardCapsuleData->Capsule;
		pHitCapsule->SetActiveTime(clipboardCapsuleData->fActiveTimeBegin, clipboardCapsuleData->fActiveTimeEnd);
		pHitCapsule->m_nHitEffectCapsuleID = clipboardCapsuleData->nHitEffectCapsuleID;
		pHitCapsule->nLinkedBPart = clipboardCapsuleData->nLinkedBPart;
	}

	Serialize();
}

void TMeshHitTabController::RefreshMeshHitColInfo()
{
	if(GlobalObjects::g_pMainForm->tabControl2->SelectedTab == GlobalObjects::g_pMainForm->PartsJudgment)
	{
		Serialize();
	}
}

void TMeshHitTabController::DelHitEffectCapsule_Click( System::Object^ sender, System::EventArgs^ e )
{
	ArrayList^ arrSelectedCapsuleInfo = GlobalObjects::g_pMainForm->HitEffect_treeView->SelectedNodes;

	int nCount = arrSelectedCapsuleInfo->Count;
	if(nCount == 0)
	{
		System::Windows::Forms::MessageBox::Show(gcnew String("���õ� ĸ���� �����ϴ�."), "���� ����Ʈ ĸ�� ���� ����");
		return;
	}

	if (System::Windows::Forms::MessageBox::Show(String::Format("���õ� ĸ�� {0}���� ���� �Ͻðڽ��ϱ�?", nCount), "���� ����Ʈ ĸ�� ����"
		,MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes)
	{
		vector<int> vecDelIndex;
		System::Collections::IEnumerator^ myEnumerator = arrSelectedCapsuleInfo->GetEnumerator();
		while ( (myEnumerator->MoveNext()) && (myEnumerator->Current != nullptr) )
		{
			TreeNode^ node = safe_cast<TreeNode^>(myEnumerator->Current);
			Object^ tag	= node->Tag;
			if(tag == nullptr)
				continue;

			int nDelIndex = (int)tag;
			vecDelIndex.push_back(nDelIndex);
		}

		std::sort(vecDelIndex.begin(), vecDelIndex.end(), descendingInt);
		for(vector<int>::iterator it = vecDelIndex.begin(); it != vecDelIndex.end(); ++it)
		{
			g_pMainApp->DelMeshHitEffectCapsuleInfo(*it);
		}
	}
}

void TMeshHitTabController::AddHitEffectCapsule_Click( System::Object^ sender, System::EventArgs^ e )
{
	// ���õ� ��尡 �ִٸ�...
	g_pMainApp->CreateMeshHitEffectCapsuleInfo();
}

void TMeshHitTabController::CopyHitEffectCapsule()
{
	ArrayList^ arrSelectedCapsuleInfo = GlobalObjects::g_pMainForm->HitEffect_treeView->SelectedNodes;

	Character_NET::TClipboardGroupCapsuleBoxData^ clipboardCapsuleList = gcnew Character_NET::TClipboardGroupCapsuleBoxData();

	System::Collections::IEnumerator^ myEnumerator = arrSelectedCapsuleInfo->GetEnumerator();
	while ( (myEnumerator->MoveNext()) && (myEnumerator->Current != nullptr) )
	{
		TreeNode^ node = safe_cast<TreeNode^>(myEnumerator->Current);
		Object^ tag	= node->Tag;
		if(tag == nullptr)
			continue;

		CSColCapsule* pHitEffectCapsule = m_pMeshHitEffectInfoViewer->GetMeshHitEffectCapsuleInfo((int)tag);
		if (pHitEffectCapsule == NULL)
			continue;

		Character_NET::TClipboardCapsuleData^ clipboardCapsuleData = gcnew Character_NET::TClipboardCapsuleData();
		clipboardCapsuleData->Capsule = pHitEffectCapsule->m_Capsule;
		clipboardCapsuleData->strBoneTop = gcnew String(pHitEffectCapsule->m_strBoneTop.c_str());
		clipboardCapsuleData->strBoneBottom = gcnew String(pHitEffectCapsule->m_strBoneBottom.c_str());

		clipboardCapsuleList->m_Capsule.CopyFrom(clipboardCapsuleData);
	}

	// Ŭ������� ����
	TClipboardHelper::CopyGroupCapsuleData(clipboardCapsuleList);
}

void TMeshHitTabController::PasteHitEffectCapsule()
{
	TClipboardGroupCapsuleBoxData^ clipboardCapsuleList = TClipboardHelper::PasteGroupCapsuleData();

	if (clipboardCapsuleList == nullptr) return;

	int nCount = clipboardCapsuleList->m_Capsule.m_arrCapsule->Count;
	for(int i = 0; i < nCount; ++i)
	{
		Character_NET::TClipboardCapsuleData^ clipboardCapsuleData = gcnew Character_NET::TClipboardCapsuleData();
		clipboardCapsuleList->m_Capsule.CopyTo(i, clipboardCapsuleData);

		// ����
		int nCapsuleID = g_pMainApp->CreateMeshHitEffectCapsuleInfo();

		CSColCapsule* pHitEffectCapsule = m_pMeshHitEffectInfoViewer->GetMeshHitEffectCapsuleInfo(nCapsuleID);
		if (pHitEffectCapsule == NULL) 
			continue;

		// ������ �Է�
		pHitEffectCapsule->m_Capsule = clipboardCapsuleData->Capsule;

		if (clipboardCapsuleData->strBoneTop != nullptr)
		{
			const char* cstr = MStringToCharPointer(clipboardCapsuleData->strBoneTop);
			pHitEffectCapsule->m_strBoneTop = cstr;
			MFreeCharPointer(cstr);
		}

		if (clipboardCapsuleData->strBoneBottom != nullptr)
		{
			const char* cstr = MStringToCharPointer(clipboardCapsuleData->strBoneBottom);
			pHitEffectCapsule->m_strBoneBottom = cstr;
			MFreeCharPointer(cstr);
		}
	}

	Serialize();
}

void TMeshHitTabController::DelPartsCapsule_Click( System::Object^ sender, System::EventArgs^ e )
{
	ArrayList^ arrSelectedHitInfo = GlobalObjects::g_pMainForm->PartsHitCapsulesView->SelectedNodes;

	int nCount = arrSelectedHitInfo->Count;
	if(nCount == 0)
	{
		System::Windows::Forms::MessageBox::Show(gcnew String("���õ� ĸ���̳� �׷��� �����ϴ�."), "���� ĸ�� ���� ����");
		return;
	}

	if (System::Windows::Forms::MessageBox::Show(String::Format("���õ� {0}���� �׷��̳� ĸ���� ���� �Ͻðڽ��ϱ�? �׷� �����ÿ��� ���Ե� ��� ĸ���� ���� �˴ϴ�.", nCount), "���� ĸ�� ���� ����"
		,MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes)
	{
		vector<delGroupCapsuleData> vecDelIndex;

		System::Collections::IEnumerator^ myEnumerator = arrSelectedHitInfo->GetEnumerator();
		while ( (myEnumerator->MoveNext()) && (myEnumerator->Current != nullptr) )
		{
			TreeNode^ node = safe_cast<TreeNode^>(myEnumerator->Current);
			Object^ tag	= node->Tag;
			if(tag == nullptr)
				continue;

			if(node->Level == 0)
			{
				// �׷�
				if((int)tag == 0)
				{
					// �׷� 0���� ������ �ȉ´�.
					System::Windows::Forms::MessageBox::Show(gcnew String("���õ� �׷�(0)�� ���� �� �� �����ϴ�."), "���� ĸ�� ���� ����");
					continue;
				}

				delGroupCapsuleData delData;
				delData.nGroupID = (int)tag;
				vecDelIndex.push_back(delData);
			}
			else
			{
				// ĸ��
				Object^ parentTag = node->Parent->Tag;
				if((int)parentTag == 0 &&
					node->Parent->GetNodeCount(true) == 1)
				{
					// �׷� 0���� ������ �ȉ´�.
					System::Windows::Forms::MessageBox::Show(gcnew String("���õ� �׷�(0) ĸ���� ���̻� ���� �� �� ���� �����Դϴ�."), "���� ĸ�� ���� ����");
					continue;
				}

				delGroupCapsuleData delData;
				delData.nGroupID = (int)parentTag;
				delData.nCapsuleID = (int)tag;
				vecDelIndex.push_back(delData);
			}
		}

		// ����
		std::sort(vecDelIndex.begin(), vecDelIndex.end(), delGroupCapsuleData::descending);
		for(vector<delGroupCapsuleData>::iterator it = vecDelIndex.begin(); it != vecDelIndex.end(); ++it)
		{
			if(it->nCapsuleID == -1)
				g_pMainApp->DelMeshHitGroupInfo(it->nGroupID);
			else
				g_pMainApp->DelMeshHitCapsuleInfo(it->nGroupID, it->nCapsuleID);
		}
	}
}

void TMeshHitTabController::AddCollisionBox_Click( System::Object^ sender, System::EventArgs^ e )
{
	g_pMainApp->CreateMeshColBoxInfo();
}
