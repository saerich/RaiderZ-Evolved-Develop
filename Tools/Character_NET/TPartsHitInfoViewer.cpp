#include "stdafx.h"
#include "TPartsHitInfoViewer.h"
#include "Xmlseral.h"
#include "RRenderHelper.h"
#include "RFont.h"

#define PARTS_HIT_INFO_GROUP_ID			"Group : {0}"
#define PARTS_HIT_INFO_CAPULE_ID		"Capsule : {0}"

#define PARTS_HIT_CAPSULE_SELECT		0x20ffff00
#define PARTS_HIT_CAPSILE_NONE_SELECT	0x20C0C0C0

//////////////////////////////////////////////////////////////////////////
void TPartsHitInfoViewer::InitPartsHitInfoViewer()
{
	// Viewer ���� �ʱ�ȭ
	GlobalObjects::g_pMainForm->PartsHitCapsulesView->ClearTreeView();
	GetPartsHitInfoViewerPropertGrid()->SelectedObject = nullptr;

}

TreeView^ TPartsHitInfoViewer::GetPartsHitInfoViewerTreeNode()
{
	return GlobalObjects::g_pMainForm->PartsHitCapsulesView;
}

PropertyGrid^ TPartsHitInfoViewer::GetPartsHitInfoViewerPropertGrid()
{
	return GlobalObjects::g_pMainForm->PartsHitCapsuleAtt;
}

void TPartsHitInfoViewer::ChangePartsHitInfoViewer( System::String^ ModelName )
{
	InitPartsHitInfoViewer();

	TMeshInfoMgr& MeshInfoMgr		= TMeshInfoMgr::GetInstance();

	// �� Hit ������ �����´�.
	m_strModelName = (char*)MStringToCharPointer(ModelName);

	// ���̸��� Ȯ���ڰ� ���Դ��� üũ
	string strModelName = string(m_strModelName);
	m_strModelName = strModelName.substr(0, strModelName.rfind("."));

	CSMeshInfo * pMeshInfo = MeshInfoMgr.GetInfo(m_strModelName);

	// ������ ����� �ش�.
	if(pMeshInfo == NULL)
	{
		if(MeshInfoMgr.UseModelHitColCapsule(m_strModelName))
		{
			if(MeshInfoMgr.CreateMeshInfo(m_strModelName))
			{
				pMeshInfo = MeshInfoMgr.GetInfo(m_strModelName);
			}
		}
	}

	if(pMeshInfo)
	{
		// �� �̸� �����ֱ�
		GlobalObjects::g_pMainForm->PartsHitModelName->Text = ModelName;		

		// �ƹ��͵� ���ų�, 0�� �ε����� ���� ���� ĸ���� ���ٸ�... �⺻ ���� ĸ���� �����.
		if(pMeshInfo->m_mapHitGroupInfo.size() == 0 ||
			MeshInfoMgr.GetMeshHitInfo()->IsHitGroup(m_strModelName, 0) == false)
		{
			MeshInfoMgr.GetMeshHitInfo()->CreateSimpleHitCapsule(pMeshInfo, g_pMainApp->GetActor());

			NotifyChanging();
		}

		// Viewer ���� ä���
		for(map<int, CSHitGroup>::iterator itParts = pMeshInfo->m_mapHitGroupInfo.begin();
			itParts != pMeshInfo->m_mapHitGroupInfo.end(); itParts++)
		{
			// �׷� ���
			TreeNode^ nnode = GetPartsHitInfoViewerTreeNode()->Nodes->Add(String::Format(PARTS_HIT_INFO_GROUP_ID, itParts->second.m_nGroupID));
			nnode->Tag		= itParts->first;

			int nCIndex = 0;
			for(vector<CSHitCapsule>::iterator itCapsule = itParts->second.m_vecHitCapsules.begin();
				itCapsule != itParts->second.m_vecHitCapsules.end(); itCapsule++)
			{
				if(nnode != nullptr)
				{
					// ĸ�� ���
					TreeNode^ cnnod = nnode->Nodes->Add(String::Format(PARTS_HIT_INFO_CAPULE_ID, nCIndex));
					cnnod->Tag		= nCIndex;
				}

				nCIndex++;
			}
		}

		GetPartsHitInfoViewerTreeNode()->ExpandAll();
	}
}

void TPartsHitInfoViewer::CreatePartsHitInfoViewer(int nGroupID, int nCapsuleID /* = -1 */)
{
	if(nGroupID == -1)
		return;

	TreeNode^ pGroupNode = g_pMainApp->SearchNode(GetPartsHitInfoViewerTreeNode()->Nodes, String::Format(PARTS_HIT_INFO_GROUP_ID, nGroupID));
	if(pGroupNode == nullptr)
	{
		// ������ ����
		pGroupNode = GetPartsHitInfoViewerTreeNode()->Nodes->Add(String::Format(PARTS_HIT_INFO_GROUP_ID, nGroupID));
		pGroupNode->Tag									= nGroupID;
		GetPartsHitInfoViewerTreeNode()->SelectedNode	= pGroupNode;
		GlobalObjects::g_SelInfo.m_nMeshHitGroupID					= nGroupID;
	}

	if(nCapsuleID == -1)
		return;

	// ĸ�� �߰�
	TreeNode^ pCapsuleNode = pGroupNode->Nodes->Add(String::Format(PARTS_HIT_INFO_CAPULE_ID, nCapsuleID));

	// �߰��� ĸ�� ���ó��� ����
	pCapsuleNode->Tag								= nCapsuleID;
	GetPartsHitInfoViewerTreeNode()->SelectedNode	= pCapsuleNode;
	GlobalObjects::g_SelInfo.m_nMeshHitCapsuleID				= nCapsuleID;
}

void TPartsHitInfoViewer::Reload()
{
	GlobalObjects::g_pMainForm->PartsHitCapsulesView->ClearTreeView();
	GetPartsHitInfoViewerPropertGrid()->SelectedObject = nullptr;

	if(m_strModelName.size())
	{
		ChangePartsHitInfoViewer(gcnew String(m_strModelName.c_str()));
	}
}

CSHitCapsule * TPartsHitInfoViewer::GetPartsHitCapsuleInfo( int nGroupIndex, int nCapsuleIndex )
{
	if(nGroupIndex == -1 || nCapsuleIndex == -1)
	{
		return NULL;
	}

	// �� Hit ������ �����´�.
	CSMeshInfo * pMeshInfo = TMeshInfoMgr::GetInstance().GetMeshInfo(m_strModelName);
	if(pMeshInfo)
	{
		for(map<int, CSHitGroup>::iterator itParts = pMeshInfo->m_mapHitGroupInfo.begin();
			itParts != pMeshInfo->m_mapHitGroupInfo.end(); itParts++)
		{
			if(itParts->first == nGroupIndex)
			{
				int nCIndex = 0;
				for(vector<CSHitCapsule>::iterator itCapsule = itParts->second.m_vecHitCapsules.begin();
					itCapsule != itParts->second.m_vecHitCapsules.end(); itCapsule++)
				{
					if(nCIndex == nCapsuleIndex)
					{
						return &(*itCapsule);
					}

					nCIndex++;
				}
			}
		}
	}

	return NULL;
}

void TPartsHitInfoViewer::SetPartsHitCapsuleData( int nGroupIndex, int nCapsuleIndex )
{
	// �κ� ���� ������ �����´�.
	CSHitCapsule * pCapsuleInfo = GetPartsHitCapsuleInfo(nGroupIndex, nCapsuleIndex);
	if(pCapsuleInfo != NULL)
	{
		SetPartsHitCapsuleDataToProperty(pCapsuleInfo);
	}
	else
	{
		// ������ ����. �׷��Ƿ� ���� �κ� ���� ĸ�� ���� �����
		GetPartsHitInfoViewerPropertGrid()->SelectedObject = nullptr;
	}
}

void TPartsHitInfoViewer::ChangePartsHitCapsuleInfo( int nGroupID, int nCapsuleID )
{
	// �κ� ���� ������ �����´�.
	CSHitCapsule * pCapsuleInfo = GetPartsHitCapsuleInfo(nGroupID, nCapsuleID);
	if(pCapsuleInfo != NULL)
	{
		// ����� �����͸� ĸ�� ������ �Է�
		SetPropertyToPartsHitCapsuleData(pCapsuleInfo);

		// ����� �����͸� ������ ���� �ٽ� ���
		pCapsuleInfo->m_Capsule.CalcHeight();

		// ����� ���̸� ������Ƽ���� �ٽ� �Է�
		SetPartsHitCapsuleDataToProperty(pCapsuleInfo);

		NotifyChanging();
	}
	else
	{
		// ������ ����. �׷��Ƿ� ���� �κ� ���� ĸ�� ���� �����
		GetPartsHitInfoViewerPropertGrid()->SelectedObject = nullptr;
	}
}

void TPartsHitInfoViewer::SetPartsHitCapsuleDataToProperty(CSHitCapsule * pPartsHitCapsule)
{
	XmlSerializable::PARTSHITCAPSULE^ capsule = (XmlSerializable::PARTSHITCAPSULE^)(GetPartsHitInfoViewerPropertGrid()->SelectedObject);

	if( capsule == nullptr)
	{
		capsule = gcnew XmlSerializable::PARTSHITCAPSULE;
	}

	capsule->������		= pPartsHitCapsule->m_Capsule.radius;
	capsule->����		= pPartsHitCapsule->m_Capsule.height;
	capsule->������1_X	= pPartsHitCapsule->m_Capsule.top.x;
	capsule->������1_Y	= pPartsHitCapsule->m_Capsule.top.y;
	capsule->������1_Z	= pPartsHitCapsule->m_Capsule.top.z;
	capsule->������2_X	= pPartsHitCapsule->m_Capsule.bottom.x;
	capsule->������2_Y	= pPartsHitCapsule->m_Capsule.bottom.y;
	capsule->������2_Z	= pPartsHitCapsule->m_Capsule.bottom.z;
	capsule->�ε���		= pPartsHitCapsule->m_nHitEffectCapsuleID;

	capsule->a��ȿ���۽ð�	= pPartsHitCapsule->GetActiveTimeBegin();
	capsule->b��ȿ���ð�	= pPartsHitCapsule->GetActiveTimeEnd();

	capsule->LinkedBPart	= pPartsHitCapsule->nLinkedBPart;

	GetPartsHitInfoViewerPropertGrid()->SelectedObject = capsule;

}

void TPartsHitInfoViewer::SetPropertyToPartsHitCapsuleData(CSHitCapsule * pPartsHitCapsule)
{
	XmlSerializable::PARTSHITCAPSULE^ capsule = (XmlSerializable::PARTSHITCAPSULE^)(GetPartsHitInfoViewerPropertGrid()->SelectedObject);
	
	if( capsule == nullptr)
	{
		return;
	}

	pPartsHitCapsule->m_Capsule.radius		= capsule->������;
	pPartsHitCapsule->m_Capsule.height		= capsule->����;
	pPartsHitCapsule->m_Capsule.top.x		= capsule->������1_X;
	pPartsHitCapsule->m_Capsule.top.y		= capsule->������1_Y;
	pPartsHitCapsule->m_Capsule.top.z		= capsule->������1_Z;
	pPartsHitCapsule->m_Capsule.bottom.x	= capsule->������2_X;
	pPartsHitCapsule->m_Capsule.bottom.y	= capsule->������2_Y;
	pPartsHitCapsule->m_Capsule.bottom.z	= capsule->������2_Z;
	pPartsHitCapsule->m_nHitEffectCapsuleID	= capsule->�ε���;
	pPartsHitCapsule->nLinkedBPart			= capsule->LinkedBPart;

	float fActiveTimeBegin = capsule->a��ȿ���۽ð�;
	float fActiveTimeEnd = capsule->b��ȿ���ð�;

	pPartsHitCapsule->SetActiveTime(fActiveTimeBegin, fActiveTimeEnd);

}

void TPartsHitInfoViewer::RenderEditionCapsule(MCapsule & capsuleInfo, DWORD dwColor)
{
	// �κ� ���� ������ �����´�.
	MCapsule cap = capsuleInfo;

	//------------------------------------------------------------------------
	// ĸ�� ������
	RMatrix matActor			= g_pMainApp->GetActor()->GetWorldTransform();
	vec3 vTop, vBottom;
	matActor.TransformVect(cap.top, vTop);
	matActor.TransformVect(cap.bottom, vBottom);
	cap.top = vTop;
	cap.bottom = vBottom;

	RenderCapsule(capsuleInfo, dwColor);

	g_pMainApp->SetCapsuleRenderState();
	//------------------------------------------------------------------------
	// ĸ�� �߾� ��
	RBoundingBox box = RBoundingBox(RVector(-5,-5,-5),RVector(5,5,5));
	RMatrix matTM;
	matTM.MakeIdentity();

	RRenderHelper::RenderLine(RMatrix::IDENTITY, cap.bottom, cap.top, 0xFFFF0000);

	RVector t1 = cap.top;
	t1.z -= (cap.height + cap.radius * 1.5f);
	RRenderHelper::RenderLine(RMatrix::IDENTITY, cap.top, t1, 0xFFFF00FF);
	matTM.SetTranslation(t1);
	RRenderHelper::RenderBox(matTM, box, 0xffffffff);
	RRenderHelper::RenderAxis(matTM,30,0.5f);
	g_pMainApp->RenderAxisText(matTM, 30);

	RVector t2 = cap.bottom;
	t2.z -= cap.radius * 2.0f;
	RRenderHelper::RenderLine(RMatrix::IDENTITY, cap.bottom, t2, 0xFFFF00FF);
	matTM.SetTranslation(t2);
	RRenderHelper::RenderBox(matTM, box, 0xffffffff);
	RRenderHelper::RenderAxis(matTM,30,0.5f);
	g_pMainApp->RenderAxisText(matTM, 30);

	//------------------------------------------------------------------------
	// ��ȣ ���
	// billboard
	rs3::RFont::BeginFont(/*D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE*/);

	g_pMainApp->GetDevice()->SetTransform(RST_VIEW, g_pMainApp->GetCamera()->GetViewMatrix());

	rs3::RMatrix matInverse;
	g_pMainApp->GetDevice()->GetTransform( rs3::RST_VIEW ).GetInverse( &matInverse );

	g_pMainApp->GetCamera()->GetViewMatrix().GetInverse( &matInverse );

	rs3::RMatrix matTextReflectAndScale
		(
			2.0f,    0.f,    0.f,   0.f,
			0.f,     -2.0f,  0.f,   0.f,
			0.f,      0.f,    2.0f, 0.f,
			0.f,      0.f,    0.f,   1.f
		);

	matInverse._41 = t1.x;
	matInverse._42 = t1.y;
	matInverse._43 = t1.z;

	RMatrix matT = matTextReflectAndScale * matInverse;

	if( GlobalObjects::g_pMainForm->Bottom_toolStripButton->Checked ||
		GlobalObjects::g_pMainForm->TopBottom_toolStripButton->Checked)
		g_pMainApp->GetFont()->DrawTextIn3DSpace( matT, L"1", -1, 0xFFFF0000);
	else
		g_pMainApp->GetFont()->DrawTextIn3DSpace( matT, L"1" );

	matInverse._41 = t2.x;
	matInverse._42 = t2.y;
	matInverse._43 = t2.z;

	matT = matTextReflectAndScale * matInverse;

	if( GlobalObjects::g_pMainForm->Top_toolStripButton->Checked ||
		GlobalObjects::g_pMainForm->TopBottom_toolStripButton->Checked)
		g_pMainApp->GetFont()->DrawTextIn3DSpace( matT, L"2", -1, 0xFFFF0000);
	else
		g_pMainApp->GetFont()->DrawTextIn3DSpace( matT, L"2" );

	rs3::RFont::EndFont();

	g_pMainApp->EndCapsuleRenderState();
}

void TPartsHitInfoViewer::RenderCapsule(MCapsule & capsuleInfo, DWORD dwColor /* = 0x200000ff */)
{
	MCapsule cap = capsuleInfo;

	//------------------------------------------------------------------------
	// ĸ�� ������
	RMatrix matActor			= g_pMainApp->GetActor()->GetWorldTransform();
	vec3 vTop, vBottom;
	matActor.TransformVect(cap.top, vTop);
	matActor.TransformVect(cap.bottom, vBottom);
	cap.top = vTop;
	cap.bottom = vBottom;

	g_pMainApp->SetCapsuleRenderState();

	RRenderHelper::RenderCapsule(cap, dwColor);

	g_pMainApp->EndCapsuleRenderState();
}

void TPartsHitInfoViewer::RenderGroup( int nGroupIndex, int nSelectCapsuleID /*= -1*/ )
{
	if(nGroupIndex == -1 || m_strModelName.size() <= 0)
	{
		return;
	}

	// �� Hit ������ �����´�.
	CSMeshInfo * pMeshInfo = TMeshInfoMgr::GetInstance().GetMeshInfo(m_strModelName);
	if(pMeshInfo)
	{
		for(map<int, CSHitGroup>::iterator itParts = pMeshInfo->m_mapHitGroupInfo.begin();
			itParts != pMeshInfo->m_mapHitGroupInfo.end(); itParts++)
		{
			if(itParts->first == nGroupIndex)
			{
				bool bSelect = false;
				MCapsule renderCapsule;
				MCapsule selectCapsule;
				int nCIndex = 0;
				for(vector<CSHitCapsule>::iterator itCapsule = itParts->second.m_vecHitCapsules.begin();
					itCapsule != itParts->second.m_vecHitCapsules.end(); itCapsule++)
				{
					renderCapsule = (*itCapsule).m_Capsule;

					if(nSelectCapsuleID != -1)
					{
						// ĸ�� �����̶��... ���� ĸ�� �ܴ̿� ȸ������
						if(nCIndex == nSelectCapsuleID)
						{
							bSelect = true;
							selectCapsule = renderCapsule;
						}
						else
						{
							RenderCapsule(renderCapsule, PARTS_HIT_CAPSILE_NONE_SELECT);
						}
					}
					else
					{
						// �׷켱���̶�� ��ü�� �Ķ���
						RenderCapsule(renderCapsule);
					}

					nCIndex++;
				}

				if(bSelect)
				{
					RenderEditionCapsule(selectCapsule, PARTS_HIT_CAPSULE_SELECT);
				}
			}
		}
	}
}

MCapsule * TPartsHitInfoViewer::GetCurrentSelectCapsule()
{
	CSHitCapsule * pPartsHitCapsule = GetPartsHitCapsuleInfo(GlobalObjects::g_SelInfo.m_nMeshHitGroupID, GlobalObjects::g_SelInfo.m_nMeshHitCapsuleID);

	if(pPartsHitCapsule)
	{
		return &(pPartsHitCapsule->m_Capsule);
	}

	return NULL;
}

void TPartsHitInfoViewer::NotifyChanging()
{
	GlobalObjects::g_SaveNotifier.SetSaveMeshInfo(true);
}

CSHitGroup * TPartsHitInfoViewer::GetPartsHitGroupInfo( int nGroupIndex )
{
	if(nGroupIndex == -1)
	{
		return NULL;
	}

	// �� Hit ������ �����´�.
	CSMeshInfo * pMeshInfo = TMeshInfoMgr::GetInstance().GetMeshInfo(m_strModelName);
	if(pMeshInfo)
	{
		for(map<int, CSHitGroup>::iterator itParts = pMeshInfo->m_mapHitGroupInfo.begin();
			itParts != pMeshInfo->m_mapHitGroupInfo.end(); itParts++)
		{
			if(itParts->first == nGroupIndex)
			{
				return &(itParts->second);
			}
		}
	}

	return NULL;
}
