#include "StdAfx.h"
#include "TMtrl.h"
#include "RSceneManager.h"
#include "RMaterialResource.h"
#include "RMaterialResourceMgr.h"

TMtrl::TMtrl(void)
{
	Init();
}

TMtrl::~TMtrl(void)
{
	m_MaterialTable.clear();
}

void TMtrl::Init()
{
	m_nItemID = 0;
}

bool TMtrl::Export_NPC()
{
	TCharacter* pActor = g_pMainApp->GetActor();

	MXml xml;
	if (xml.LoadFile(FILENAME_NPC_CHANGEMTRL_TOOL) == false)
	{
		MXmlDeclaration* pDec = xml.CreateDeclaration("1.0", "euc-kr", "");
		xml.Doc()->LinkEndChild(pDec);

		MXmlElement *pRootElement = new MXmlElement(NPC_XML_TAG_MAIET);
		xml.Doc()->LinkEndChild(pRootElement);
	}

	MXmlHandle docHandle = xml.DocHandle();
	MXmlElement* pRootElement = docHandle.FirstChild(NPC_XML_TAG_MAIET).Element();

	MXmlElement* pElement = pRootElement->FirstChildElement();
	for( pElement; pElement != NULL; pElement=pElement->NextSiblingElement() )
	{
		int nNPCID;
		_Attribute(&nNPCID,	pElement, NPC_XML_ATTR_ID);

		if( g_pMainApp->m_NPC.IsValidNPCID(nNPCID) )
		{
			TNPCInfo* pNPCInfo = TNPCInfoMgr::GetInstance().Get(nNPCID);
			if (pNPCInfo == NULL)	return false;

			MXmlElement *pMeshElement = pElement->FirstChildElement();

			for( int i = 0; i < pActor->GetActorNodeCount(); ++i )
			{
				RActorNode *pActorNode = pActor->GetActorNodes()[i];

				// Dummy�� Bone�� ������� �ʴ´�.
				if( pActorNode->m_pMeshNode->isBone() || pActorNode->m_pMeshNode->isDummyMesh() )
					continue;

				MXmlElement *pMaterialElement = pMeshElement->FirstChildElement();
				for( int j = 0; j < pActorNode->m_pMeshNode->m_nMtrlCnt; ++j )
				{
					_SetAttribute(pMaterialElement,"SubID", j);
					_SetAttribute(pMaterialElement,"MtrlName", pActorNode->m_vecMaterialTable.at(j)->m_pMaterial->GetName().c_str());

					pMaterialElement = pMaterialElement->NextSiblingElement();
				}

				pMeshElement = pMeshElement->NextSiblingElement();
			}

			xml.SaveFile(FILENAME_NPC_CHANGEMTRL_TOOL);

			System::Windows::Forms::MessageBox::Show(gcnew String("NPC Export !!"), "�Ϸ�");
			TNPCInfoMgr::GetInstance().Load();

			return true;
		}
	}

	MXmlElement *pNPCElement = new MXmlElement(NPC_XML_TAG_NPC);
	pRootElement->LinkEndChild(pNPCElement);
	AddSubMtrl_NPC(pNPCElement, g_pMainApp->m_NPC.GetNPCID());

	xml.SaveFile(FILENAME_NPC_CHANGEMTRL_TOOL);

	System::Windows::Forms::MessageBox::Show(gcnew String("NPC Export !!"), "�Ϸ�");
	TNPCInfoMgr::GetInstance().Load();

	return true;
}

bool TMtrl::AddSubMtrl_NPC(MXmlElement* pNPCElement, int nNPCID)
{
	TCharacter* pActor = g_pMainApp->GetActor();

	//	if(pNPCInfo->bIsSubMtrl == false)	// ���� ���� ������ ������ �н�
	//		continue;

	TNPCInfo* pNPCInfo = TNPCInfoMgr::GetInstance().Get(nNPCID);
	if (pNPCInfo == NULL)	return false;

	_SetAttribute(pNPCElement,NPC_XML_ATTR_ID, pNPCInfo->nID);
	_SetAttribute(pNPCElement,NPC_XML_ATTR_NAME, pNPCInfo->strName.c_str());
	//	_SetAttribute(pNPCElement,NPC_XML_ATTR_MODEL, pNPCInfo->strModelFolder.c_str());
	_SetAttribute(pNPCElement,NPC_XML_ATTR_MESH_NAME, pNPCInfo->strMeshName.c_str());	// .elu file name

	//	MXmlElement *pMeshListElement = new MXmlElement("MESHLIST");
	//	pNPCElement->LinkEndChild(pMeshListElement);
	//	_SetAttribute(pMeshListElement,"Count", m_pActor->GetActorNodeCount());
	for( int i = 0; i < pActor->GetActorNodeCount(); ++i )
	{
		RActorNode *pActorNode = pActor->GetActorNodes()[i];
		// Dummy�� Bone�� ������� �ʴ´�.
		if( pActorNode->m_pMeshNode->isBone() || pActorNode->m_pMeshNode->isDummyMesh() )
			continue;

		MXmlElement *pMeshElement = new MXmlElement("MESH");
		pNPCElement->LinkEndChild(pMeshElement);
		_SetAttribute(pMeshElement, "index", i);

		// <Sub>
		_SetAttribute(pMeshElement, "SubCount", pActorNode->m_pMeshNode->m_nMtrlCnt);
		for( int j = 0; j < pActorNode->m_pMeshNode->m_nMtrlCnt; ++j )
		{
			MXmlElement *pMaterialElement = new MXmlElement("MATERIAL");
			pMeshElement->LinkEndChild(pMaterialElement);	
			_SetAttribute(pMaterialElement,"SubID", j);
			_SetAttribute(pMaterialElement,"MtrlName", pActorNode->m_vecMaterialTable[j]->m_pMaterial->GetName().c_str());
		}
	}

	return true;
}

bool TMtrl::Export_Item()
{
	MXml xml;
	if (xml.LoadFile(FILENAME_ITEM_CHANGEMTRL_TOOL) == false)
	{
		MXmlDeclaration* pDec = xml.CreateDeclaration("1.0", "euc-kr", "");
		xml.Doc()->LinkEndChild(pDec);

		MXmlElement *pRootElement = new MXmlElement(NPC_XML_TAG_MAIET);
		xml.Doc()->LinkEndChild(pRootElement);
	}

	MXmlHandle docHandle = xml.DocHandle();
	MXmlElement* pRootElement = docHandle.FirstChild(NPC_XML_TAG_MAIET).Element();

	MXmlElement* pElement = pRootElement->FirstChildElement();
	for( pElement; pElement != NULL; pElement=pElement->NextSiblingElement() )
	{
		int nItemID;
		_Attribute(&nItemID, pElement, NPC_XML_ATTR_ID);

		if( nItemID == m_nItemID )
		{
			CSItemData* pItem = g_pMainApp->GetItemMgr()->GetItemData(nItemID);
			if (pItem == NULL)	
				return false;

			// <ITEM id="1010" name="�ҿ� �����̸� ����" mesh_name="soul_primer" mesh_name_by_sex="True" node_name="hat" type="equipment" sex="All" slot="head" buyprice="1000" sellprice="500" weight="9" max_durability="50" AP="5" />
			MXmlElement *pMeshElement = pElement->FirstChildElement();
			MXmlElement *pMaterialElement = pMeshElement->FirstChildElement();
			for( size_t j = 0; j < g_pMainApp->m_pSelectedActorNode->m_vecMaterialTable.size(); ++j )
			{
				_SetAttribute(pMaterialElement,"SubID", (int)j);
				_SetAttribute(pMaterialElement,"MtrlName", g_pMainApp->m_pSelectedActorNode->m_vecMaterialTable.at(j)->m_pMaterial->GetName());

				pMaterialElement = pMaterialElement->NextSiblingElement();
			}

			pMeshElement = pMeshElement->NextSiblingElement();

			xml.SaveFile(FILENAME_ITEM_CHANGEMTRL_TOOL);

			System::Windows::Forms::MessageBox::Show(gcnew String("Item Export !!"), "�Ϸ�");
			TItemManager* pItemMgr = g_pMainApp->GetItemMgr();
			if ( !pItemMgr->LoadFromFile_SubMtrl(FILENAME_ITEM_CHANGEMTRL_TOOL) )
				mlog("���� : pItemMgr->LoadFromFile() - ������ ���� ���̺� �ҷ����� ����");

			return true;
		}
	}

	MXmlElement *pItemElement = new MXmlElement(NPC_XML_TAG_ITEM);
	if(AddSubMtrl_Item(pItemElement, m_nItemID) == false)
	{
		SAFE_DELETE(pItemElement);
		return false;
	}
	else
	{
		pRootElement->LinkEndChild(pItemElement);
	}

	xml.SaveFile(FILENAME_ITEM_CHANGEMTRL_TOOL);

	System::Windows::Forms::MessageBox::Show(gcnew String("Item Export !!"), "�Ϸ�");
	TItemManager* pItemMgr = g_pMainApp->GetItemMgr();
	if ( !pItemMgr->LoadFromFile_SubMtrl(FILENAME_ITEM_CHANGEMTRL_TOOL) )
		mlog("���� : pItemMgr->LoadFromFile() - ������ ���� ���̺� �ҷ����� ����");

	return true;
}

bool TMtrl::AddSubMtrl_Item(MXmlElement* pItemElement, int nItemID)
{
	CSItemData* pItem = g_pMainApp->GetItemMgr()->GetItemData(nItemID);
	if (pItem == NULL)	
		return false;

	if( g_pMainApp->m_pSelectedActorNode == NULL )
	{
		System::Windows::Forms::MessageBox::Show(gcnew String("���õ� ��尡 �����ϴ�. �ٽ� �������ּ���"), "����");
		return false;
	}

	_SetAttribute(pItemElement,NPC_XML_ATTR_ID, pItem->m_nID);
	_SetAttribute(pItemElement,NPC_XML_ATTR_NAME, pItem->m_strName.c_str());
	//	_SetAttribute(pItemElement,NPC_XML_ATTR_MESH_NAME, pItem->m_strMeshName.c_str());

	if (!pItem->m_strNodeNames.empty())
	{
		_SetAttribute(pItemElement,NPC_XML_ATTR_MESH_NODE_NAME, pItem->m_strNodeNames[1].c_str());
	}

	int nCurrentID = -1;	// ActorNode ID
	RActorNode *pActorNode = 0;
	for( int i = 0; i < g_pMainApp->GetActor()->GetActorNodeCount(); ++i )
	{
		pActorNode = g_pMainApp->GetActor()->GetActorNodes()[i];

		// Dummy�� Bone�� ������� �ʴ´�.
		if( pActorNode->m_pMeshNode->isBone() || pActorNode->m_pMeshNode->isDummyMesh() )
			continue;

		if( stricmp( pItem->m_strNodeNames[1].c_str(), pActorNode->m_strName.c_str() ) == 0 )
			nCurrentID = i;
	}

	if(nCurrentID < 0)
		return false;

	pActorNode = g_pMainApp->GetActor()->GetActorNodes()[nCurrentID];

	MXmlElement *pMeshElement = new MXmlElement("MESH");
	pItemElement->LinkEndChild(pMeshElement);
	_SetAttribute(pMeshElement, "index", nCurrentID);

	// <Sub>
	_SetAttribute(pMeshElement, "SubCount", (int)g_pMainApp->m_pSelectedActorNode->m_vecMaterialTable.size());
	for( size_t j = 0; j < g_pMainApp->m_pSelectedActorNode->m_vecMaterialTable.size(); ++j )
	{
		MXmlElement *pMaterialElement = new MXmlElement("MATERIAL");
		pMeshElement->LinkEndChild(pMaterialElement);	
		_SetAttribute(pMaterialElement,"SubID", (int)j);
		_SetAttribute(pMaterialElement,"MtrlName", g_pMainApp->m_pSelectedActorNode->m_vecMaterialTable.at(j)->m_pMaterial->GetName());
	}

	return true;
}

void TMtrl::SetItemDataSubMtrl( int nItemID )
{
	m_nItemID = nItemID;

	CSItemData* pItem = g_pMainApp->GetItemMgr()->GetItemData(nItemID);
	if (pItem == NULL)	
		return ;

	for ( int i = 0; i < (int)pItem->m_vecMtrls.size(); ++i )
	{
		for ( int j = 0; j < pItem->m_vecMtrls[i]->nSubCount; ++j )
		{
			g_pMainApp->GetActor()->ChangeMatetial(pItem->m_strNodeNames[1].c_str(), j, pItem->m_vecMtrls[i]->vecSubMtrls[j]);
		}
	}
}

void TMtrl::SetNPCInfoSubMtrl( int NPCID )
{
	TNPCInfo* pNPCInfo = TNPCInfoMgr::GetInstance().Get(NPCID);
	if (pNPCInfo == NULL)	return;

	//if( pNPCInfo->vecMtrls.size() == 0 )
	//{
	//	for( int i = 0; i < (int)g_pMainApp->GetActor()->GetActorNodeCount(); ++i )
	//	{
	//		RActorNode *pActorNode = g_pMainApp->GetActor()->GetActorNodes()[i];

	//		// Dummy�� Bone�� ������� �ʴ´�.
	//		if( pActorNode->m_pMeshNode->isBone() || pActorNode->m_pMeshNode->isDummyMesh() )
	//			continue;

	//		char szSubName[10];
	//		for( int j = 0; j < pActorNode->m_pMeshNode->m_nMtrlCnt; ++j )
	//		{
	//			sprintf(szSubName, ".%d", j);
	//			string szMeshName = pActorNode->m_pMeshNode->m_pBaseMesh->GetName() + szSubName;
	//			RMaterialResource* pMaterialResource = REngine::GetSceneManager().CreateResource< RMaterialResource >( szMeshName.c_str() );

	//			int nMaterialID = pActorNode->m_pMeshNode->m_pMtrlTableInfo[j].mtrlid;
	//			pActorNode->m_vecMaterialTable[j] = pMaterialResource;
	//			pActorNode->m_vecMaterialTable[j]->m_pMaterial->nUserIndex = nMaterialID;

	//			REngine::GetSceneManager().ReleaseResource( pMaterialResource );
	//		}
	//		pActorNode->OnCreateMaterials();
	//	}
	//}

	for ( int i = 0; i < (int)pNPCInfo->vecMtrls.size(); ++i )
	{
		for ( int j = 0; j < pNPCInfo->vecMtrls[i]->nSubCount; ++j )
		{
			// RActor �� ���ؼ� ȣ��Ǿ�� �Ѵ�
			g_pMainApp->GetActor()->ChangeMatetial(pNPCInfo->vecMtrls[i]->nID, j, pNPCInfo->vecMtrls[i]->vecSubMtrls[j]);
		}
	}
}

//************************************
// Method:    ExportMtrlSet
// FullName:  CMainApplication::ExportMtrlSet
// Access:    public 
// Returns:   void
// Qualifier: ��ü ������ Material ������ XML �����ͷ� ��� �Ѵ�.
//************************************
bool TMtrl::ExportMtrlSet()
{
	//if( g_pMainApp->m_SelectedTab == TAB_MTRLSTATE_ITEM )
	//{
	//	if(Export_Item())
	//		GlobalObjects::g_SaveNotifier.SetSaveChangeMtrlItem(false);
	//}
	//else 
	if( g_pMainApp->m_SelectedTab == TAB_MTRLSTATE_NPC )
	{
		if(Export_NPC())
			GlobalObjects::g_SaveNotifier.SetSaveChangeMtrlNPC(false);
	}
	else
	{
		if(Export_Item())
			GlobalObjects::g_SaveNotifier.SetSaveChangeMtrlItem(false);
	}

	return true;
}

bool TMtrl::SetMeshTreeView()
{
	GlobalObjects::g_pMainForm->treeViewMeshList->Nodes->Clear();

	//if( g_pMainApp->m_SelectedTab == TAB_MTRLSTATE_ITEM )
	if( g_pMainApp->m_SelectedTab2 == TAB_TABSTATE_ITEM )	// ITEM Page�ΰ�
	{
		return SetMeshTreeView_Item();
	}
	else if( g_pMainApp->m_SelectedTab == TAB_MTRLSTATE_NPC )
	{
		return SetMeshTreeView_NPC();
	}

	return true;
}

bool TMtrl::SetMeshTreeView_Item()
{
	TCharacter* pActor = g_pMainApp->GetActor();

	if(m_nItemID <= 0)
	{
		mlog("���� : ���õ� �������� �����ϴ�!!");
		return false;
	}

	CSItemData* pItem = g_pMainApp->GetItemMgr()->GetItemData(m_nItemID);
	if (pItem == NULL)	
	{
		mlog("���� : %s �������� �����ϴ�!!", m_nItemID);
		return false;
	}

	string strNodeName;
	// [0] : hat / [1] : hat_item
	// ���� ���� �ΰ��� �����ϴµ� ���� �޽��� �̸��� hat_item �� ����Ѵ�.
	// �׷��� ���� �޽� ���� ��쿡�� �����Ŀ� �޽��� �ٱ� ������ [0]hp_lweapon �� ���� 0�� �޽��� �����Ѵ�.
	if( pItem->m_strNodeNames.size() > 1 )	// �ϳ� �̻��϶� ��, [1]�� �����ϴ°�
		strNodeName = pItem->m_strNodeNames[1];
	else
		strNodeName = pItem->m_strNodeNames[0];

	RActorNode*		pActorNode	= pActor->GetActorNode( strNodeName.c_str() );
	if(pActorNode == NULL)
	{
		mlog("���� : %s ���ͳ�带 ã���� �����ϴ�!! ������ Ʋ���� ����!!", strNodeName.c_str());
		return false;
	}

	// �Ϲ����� Mesh ���
	String^ nodename = gcnew String(pActorNode->m_pMeshNode->m_Name.c_str());

	// ���� �θ� collection �� ����
	TreeNodeCollection^ tnParentCollection; 
	tnParentCollection = GlobalObjects::g_pMainForm->treeViewMeshList->Nodes;

	// �� Ʈ�� node �� ����Ѵ�
	TreeNode^ node = gcnew TreeNode( nodename );
	node->Tag = pActorNode->m_pMeshNode->m_nID;
	tnParentCollection->Add( node );

	TreeNode^ treeNode = SearchNode(GlobalObjects::g_pMainForm->treeViewMeshList->Nodes, nodename);
	if(treeNode == nullptr)
	{
		mlog("���� : tree node not found");
	}
	else
		tnParentCollection = treeNode->Nodes;

	for( int j = 0; j < pActorNode->m_pMeshNode->m_nMtrlCnt; ++j )
	{
		String^ strMtrlID = String::Format("MtrlID : {0}", j+1);
		TreeNode^ nodeMtrlID = gcnew TreeNode( strMtrlID );
		tnParentCollection->Add( nodeMtrlID );

		treeNode = SearchNode(GlobalObjects::g_pMainForm->treeViewMeshList->Nodes, strMtrlID);
		if (treeNode == nullptr)
		{
			continue;
		}

		TreeNode^ nodeTexname = gcnew TreeNode( gcnew String(pActorNode->m_vecMaterialTable.at(j)->m_pMaterial->GetName().c_str()) );
		nodeTexname->Tag = j;	// pMtrlTable�� Mtrl Index�� ����Ѵ�.
		treeNode->Nodes->Add( nodeTexname );
	}

	GlobalObjects::g_pMainForm->treeViewMeshList->ExpandAll();

	return true;
}

bool TMtrl::SetMeshTreeView_NPC()
{
	TCharacter* pActor = g_pMainApp->GetActor();

	if(pActor == NULL)
		return false;

	for( int i = 0; i < pActor->GetActorNodeCount(); ++i )
	{
		RActorNode *pActorNode = pActor->GetActorNodes()[i];

		// Dummy�� Bone�� ������� �ʴ´�.
		if( pActorNode->m_pMeshNode->isBone() || pActorNode->m_pMeshNode->isDummyMesh() )
			continue;

		// �Ϲ����� Mesh ���
		String^ nodename = gcnew String(pActorNode->m_pMeshNode->m_Name.c_str());

		// ���� �θ� collection �� ����
		TreeNodeCollection^ tnParentCollection; 
		tnParentCollection = GlobalObjects::g_pMainForm->treeViewMeshList->Nodes;

		// �� Ʈ�� node �� ����Ѵ�
		TreeNode^ node = gcnew TreeNode( nodename );
		node->Tag = pActorNode->m_pMeshNode->m_nID;

		TreeNode^ nodeExist = SearchNode(GlobalObjects::g_pMainForm->treeViewMeshList->Nodes, nodename);
		if (nodeExist != nullptr)
			continue;

		tnParentCollection->Add( node );

		TreeNode^ treeNode = SearchNode(GlobalObjects::g_pMainForm->treeViewMeshList->Nodes, nodename);
		if(treeNode == nullptr)
		{
			mlog("���� : tree node not found");
		}
		else
			tnParentCollection = treeNode->Nodes;

		for( int j = 0; j < pActorNode->m_pMeshNode->m_nMtrlCnt; ++j )
		{
			String^ strMtrlID = String::Format("MaterialID : {0}", j+1);	// ǥ��� 1����
			TreeNode^ nodeMtrl = gcnew TreeNode( strMtrlID );
			tnParentCollection->Add( nodeMtrl );

			//treeNode = SearchNode(GlobalObjects::g_pMainForm->treeViewMeshList->Nodes, strMtrlID);
			//if (treeNode == nullptr)
			//	continue;

			TreeNode^ nodeTexname = gcnew TreeNode( gcnew String(pActorNode->m_vecMaterialTable.at(j)->m_pMaterial->GetName().c_str()) );
			nodeTexname->Tag = j;	// pMtrlTable�� Mtrl Index�� ����Ѵ�.
			nodeMtrl->Nodes->Add( nodeTexname );
		}
	}

	GlobalObjects::g_pMainForm->treeViewMeshList->ExpandAll();	// ��� ��ħ

	return true;
}

bool TMtrl::ChangeMaterialSet()
{
	//if( g_pMainApp->m_SelectedTab == TAB_MTRLSTATE_ITEM )
	//if( g_pMainApp->m_SelectedTab2 == TAB_TABSTATE_ITEM )	// ITEM Page�ΰ�
	//{
	//	ChangeMaterialSet_Item();
	//	GlobalObjects::g_SaveNotifier.SetSaveChangeMtrlItem(true);
	//}
	//else 
	if( g_pMainApp->m_SelectedTab == TAB_MTRLSTATE_NPC )
	{
		ChangeMaterialSet_NPC();
		GlobalObjects::g_SaveNotifier.SetSaveChangeMtrlNPC(true);
	}
	else
	{
		ChangeMaterialSet_Item();
		GlobalObjects::g_SaveNotifier.SetSaveChangeMtrlItem(true);
	}

	return true;
}

bool TMtrl::ChangeMaterialSet_Item()
{
	if(!g_pMainApp->m_pSelectedActorNode)
		return false;

	TCharacter* pActor = g_pMainApp->GetActor();

	CSItemData* pItem = g_pMainApp->GetItemMgr()->GetItemData(m_nItemID);
	if (pItem == NULL)	return false;

	RActorNode* pActorNode = pActor->GetActorNode(pItem->m_strNodeNames[1].c_str());
	if(pActorNode == NULL) return false;

	// ���� ���õ� treeviewMeshList �� �׸��� ��´�.
	TreeNode^ treeNode = GlobalObjects::g_pMainForm->treeViewMeshList->SelectedNode;
	if( treeNode == nullptr || treeNode->GetNodeCount(false) ) // ������ ����ΰ�
		return false;

	// ���� ��ų Material�� SubMtrlListBox ���� ��´�. �̰��� ������ �̸� �����̴�.
	Object^ SelectedItem	= GlobalObjects::g_pMainForm->listBoxMtrlList->SelectedItem;
	String^ strSelectedItem = SelectedItem->ToString();

	// �̸� ������ ���� RMaterial ������ ��´�.
	int nIndex = GlobalObjects::g_pMainForm->listBoxMtrlList->Items->IndexOf(strSelectedItem);
	g_pMainApp->GetActor()->ChangeMatetial( pActorNode->m_strName, (int)treeNode->Tag, nIndex );


	return true;
}

bool TMtrl::ChangeMaterialSet_NPC()
{
	if(!g_pMainApp->m_pSelectedActorNode)
		return false;

	// ���� ���õ� treeviewMeshList �� �׸��� ��´�.
	TreeNode^ treeNode = GlobalObjects::g_pMainForm->treeViewMeshList->SelectedNode;
	if( treeNode == nullptr && treeNode->GetNodeCount(false) ) // ������ ��� �̾�� �Ѵ�.
		return false;

	// ���� ��ų Material�� SubMtrlListBox ���� ��´�. �̰��� ������ �̸� �����̴�.
	Object^ SelectedItem	= GlobalObjects::g_pMainForm->listBoxMtrlList->SelectedItem;
	String^ strSelectedItem = SelectedItem->ToString();

	// �̸� ������ ���� RMaterial ������ ��´�.
	int nIndex = GlobalObjects::g_pMainForm->listBoxMtrlList->Items->IndexOf(strSelectedItem);
	g_pMainApp->GetActor()->ChangeMatetial( g_pMainApp->m_pSelectedActorNode->m_strName, (int)treeNode->Tag, nIndex, 3.0f );

	return true;
}

bool TMtrl::SetMtrlList()
{
	GlobalObjects::g_pMainForm->listBoxMtrlList->Items->Clear();
	if(!g_pMainApp->m_pSelectedActorNode)
		return false;

	for( size_t i = 0; i < m_MaterialTable.size(); ++i )
	{
		// 08.04.21
		RMaterial* pMaterial = m_MaterialTable.at(i)->m_pMaterial;
		//RMaterial* pMaterial = m_pActor->m_pMesh->m_pMtrlList->at(i);
		mlog("���� : %d", pMaterial->IsUseUVAnimation());
		String^ nodename = gcnew String(pMaterial->GetName().c_str());
		//String^ nodename = gcnew String(pMaterial->GetTexLayerByType(RTT_DIFFUSE)->TextureName.c_str());

		GlobalObjects::g_pMainForm->listBoxMtrlList->Items->Add(nodename);
	}

	return true;
}

//************************************
// Method:    SetCurrentMaterialList
// FullName:  CMainApplication::SetCurrentMaterialList
// Access:    public 
// Returns:   void
// Qualifier: ���� ���õ� �������� BaseMesh�� m_pMaterialList�� XML �����͸� Material Resource�� �����Ѵ�.
//			  ������ Resource�� m_CurrentMaterialList���� �����Ѵ�.
//************************************
void TMtrl::SetCurrentMaterialList( int nItemID )
{
	// Material Data�� ������ ����
	// Material Resource ����
	m_MaterialTable.clear();
	//	for(int i = 0; i < (int)m_MaterialTable.size(); ++i)
	//	{
	//		m_pSceneManager->ReleaseResource( m_MaterialTable[i] );
	//	}	

	//if( g_pMainApp->m_SelectedTab == TAB_MTRLSTATE_ITEM )
	if( g_pMainApp->m_SelectedTab2 == TAB_TABSTATE_ITEM )	// ITEM Page�ΰ�
	{
		SetCurrentMaterialList_Item(nItemID);
	}
	else if( g_pMainApp->m_SelectedTab == TAB_MTRLSTATE_NPC )
	{
		SetCurrentMaterialList_NPC(nItemID);
	}
}

void TMtrl::SetCurrentMaterialList_Item( int nItemID )
{
	CSItemData* pItem = g_pMainApp->GetItemMgr()->GetItemData(nItemID);
	if(!pItem) return;

	TCharacter* pActor = g_pMainApp->GetActor();

	string strNodeName;
	// [0] : hat / [1] : hat_item
	// ���� ���� �ΰ��� �����ϴµ� ���� �޽��� �̸��� hat_item �� ����Ѵ�.
	// �׷��� ���� �޽� ���� ��쿡�� �����Ŀ� �޽��� �ٱ� ������ [0]hp_lweapon �� ���� 0�� �޽��� �����Ѵ�.
	if( pItem->m_strNodeNames.size() > 1 )	// �ϳ� �̻��϶� ��, [1]�� �����ϴ°�
		strNodeName = pItem->m_strNodeNames[1];
	else
		strNodeName = pItem->m_strNodeNames[0];

	RActorNode*		pActorNode	= pActor->GetActorNode( strNodeName.c_str() );
	if(pActorNode == NULL)
		return;
	mlog(pActorNode->m_strName.c_str());
	RMeshNode*		pMeshNode	= pActorNode->m_pMeshNode;

	char szSubName[10];
	for( int i = 0; i < (int)pMeshNode->m_pBaseMesh->GetMaterialResourceCount(); ++i )
	{
		sprintf(szSubName, ".%d", i);
		string szMeshName = pMeshNode->m_pBaseMesh->GetName() + szSubName;
		RMaterialResource* pMaterialResource = g_pMainApp->GetSceneMgr()->CreateResource< RMaterialResource >( szMeshName.c_str() );
		//pMaterialResource->BuildAll();
		// 		if( pMaterialResource->GetState() == RResource::RRS_EMPTY )
		// 		{
		// 			pMaterialResource->Fill();
		// 			pMaterialResource->Load();
		// 		}
		m_MaterialTable.push_back(pMaterialResource);
		g_pMainApp->GetSceneMgr()->ReleaseResource(pMaterialResource);
	}
}

void TMtrl::SetCurrentMaterialList_NPC( int nNPCID )
{
	TNPCInfo* pNPCInfo = TNPCInfoMgr::GetInstance().Get(nNPCID);
	if (pNPCInfo == NULL)	return;

	TCharacter* pActor = g_pMainApp->GetActor();

	RActorNode*		pActorNode	= pActor->GetActorNodes()[0];	// ���� �޽���带 �����ϱ� ������ �ƹ� ���ͳ�峪 ���´�.
	if(pActorNode == NULL)
		return;
	RMeshNode*		pMeshNode	= pActorNode->m_pMeshNode;

	char szSubName[10];
	for( int i = 0; i < (int)pMeshNode->m_pBaseMesh->GetMaterialResourceCount(); ++i )
	{
		sprintf(szSubName, ".%d", i);
		string szMeshName = pMeshNode->m_pBaseMesh->GetName() + szSubName;
		RMaterialResource* pMaterialResource = g_pMainApp->GetSceneMgr()->CreateResource< RMaterialResource >( szMeshName.c_str() );
		//pMaterialResource->BuildAll();
		// 		if( pMaterialResource->GetState() == RResource::RRS_EMPTY )
		// 		{
		// 			pMaterialResource->Fill();
		// 			pMaterialResource->Load();
		// 		}
		m_MaterialTable.push_back(pMaterialResource);
		g_pMainApp->GetSceneMgr()->ReleaseResource(pMaterialResource);
	}
}