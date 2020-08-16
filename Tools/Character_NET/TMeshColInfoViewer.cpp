#include "stdafx.h"
#include "TMeshColInfoViewer.h"
#include "Xmlseral.h"
#include "RRenderHelper.h"
#include "RFont.h"

#define MESH_COL_INFO_CAPULE_ID			"Capsule : {0}"
#define MESH_COL_INFO_BOX_ID			"Box : {0}"

#define MESH_COL_CAPSULE_SELECT			0x20FF0000
#define MESH_COL_CAPSILE_NONE_SELECT	0x20C0C0C0

//////////////////////////////////////////////////////////////////////////
void TMeshColInfoViewer::InitMeshColInfoViewer()
{
	// Viewer ���� �ʱ�ȭ
	GlobalObjects::g_pMainForm->CollisionCapsulesView->ClearTreeView();
	GetMeshColInfoViewerPropertGrid()->SelectedObject = nullptr;

	GlobalObjects::g_SelInfo.m_nMeshColID				= -1;
}

TreeView^ TMeshColInfoViewer::GetMeshColInfoViewerTreeNode()
{
	return GlobalObjects::g_pMainForm->CollisionCapsulesView;
}

PropertyGrid^ TMeshColInfoViewer::GetMeshColInfoViewerPropertGrid()
{
	return GlobalObjects::g_pMainForm->CollisionCapsuleAtt;
}

void TMeshColInfoViewer::ChangeMeshColInfoViewer( System::String^ ModelName )
{
	InitMeshColInfoViewer();
	
	TMeshInfoMgr& MeshInfoMgr		= TMeshInfoMgr::GetInstance();
	TMeshColInfoMgr* MeshColInfoMgr = MeshInfoMgr.GetMeshColInfo();

	// �� Hit ������ �����´�.
	m_strModelName = (char*)MStringToCharPointer(ModelName);
	m_MeshColInfoBoxEditor.SetModelName(m_strModelName);

	// ���̸��� Ȯ���ڰ� ���Դ��� üũ
	string strModelName = string(m_strModelName);
	m_strModelName = strModelName.substr(0, strModelName.rfind("."));

	CSMeshInfo * pMeshInfo = MeshInfoMgr.GetMeshInfo(m_strModelName);

	// ������ ����� �ش�.
	if(pMeshInfo == NULL)
	{
		if(MeshInfoMgr.UseModelHitColCapsule(m_strModelName))
		{
			if(MeshInfoMgr.CreateMeshInfo(m_strModelName))
			{
				pMeshInfo = MeshInfoMgr.GetMeshInfo(m_strModelName);
			}
		}
	}

	if(pMeshInfo)
	{
		// �� �̸� �����ֱ�
		GlobalObjects::g_pMainForm->CollisionModelName->Text = ModelName;		

		// �ƹ��͵� ���ų�, 0�� �ε����� ���� ���� ĸ���� ���ٸ�... �⺻ ���� ĸ���� �����.
		if(MeshColInfoMgr->GetNextBoxIndex(m_strModelName) == COLLISION_BOX_START_INDEX &&				// �浹�ڽ��� ����...
			MeshColInfoMgr->GetNextCapsuleIndex(m_strModelName) == 0)
		{
			MeshColInfoMgr->CreateSimpleColCapsule(pMeshInfo, g_pMainApp->GetActor());
			NotifyChanging();
		}

		// Viewer ���� ä���
		// ĸ��
		int nCIndex = 0;
		for(vector<CSColCapsule>::iterator itCapsule = pMeshInfo->m_vecColCapsules.begin();
			itCapsule != pMeshInfo->m_vecColCapsules.end(); itCapsule++)
		{
			// ĸ�� ���
			TreeNode^ cnnod = GetMeshColInfoViewerTreeNode()->Nodes->Add(String::Format(MESH_COL_INFO_CAPULE_ID, nCIndex));
			cnnod->Tag		= nCIndex;

			nCIndex++;
		}

		// �ڽ�
		nCIndex = 0;
		for(vector<CSColBox>::iterator itBox = pMeshInfo->m_vecColBox.begin();
			itBox != pMeshInfo->m_vecColBox.end(); itBox++)
		{
			// ĸ�� ���
			TreeNode^ cnnod = GetMeshColInfoViewerTreeNode()->Nodes->Add(String::Format(MESH_COL_INFO_BOX_ID, nCIndex));
			cnnod->Tag		= nCIndex + COLLISION_BOX_START_INDEX;

			nCIndex++;
		}

		GetMeshColInfoViewerTreeNode()->ExpandAll();
	}
}

void TMeshColInfoViewer::CreateMeshColCapsuleInfoViewer(int nCapsuleID /* = -1 */)
{
	if(nCapsuleID == -1)
		return;

	// ĸ�� �߰�
	TreeNode^ pCapsuleNode = GetMeshColInfoViewerTreeNode()->Nodes->Add(String::Format(MESH_COL_INFO_CAPULE_ID, nCapsuleID));

	// �߰��� ĸ�� ���ó��� ����
	pCapsuleNode->Tag								= nCapsuleID;
	GetMeshColInfoViewerTreeNode()->SelectedNode	= pCapsuleNode;
	GlobalObjects::g_SelInfo.m_nMeshColID	= nCapsuleID;
}

void TMeshColInfoViewer::Reload()
{
	GlobalObjects::g_pMainForm->CollisionCapsulesView->ClearTreeView();
	GetMeshColInfoViewerPropertGrid()->SelectedObject = nullptr;

	if(m_strModelName.size())
	{
		ChangeMeshColInfoViewer(gcnew String(m_strModelName.c_str()));
	}
}

CSColCapsule * TMeshColInfoViewer::GetMeshColCapsuleInfo( int nCapsuleIndex )
{
	if(nCapsuleIndex == -1)
	{
		return NULL;
	}

	// �� Hit ������ �����´�.
	CSMeshInfo * pMeshInfo = TMeshInfoMgr::GetInstance().GetMeshInfo(m_strModelName);
	if(pMeshInfo)
	{
		int nCIndex = 0;
		for(vector<CSColCapsule>::iterator itCapsule = pMeshInfo->m_vecColCapsules.begin();
			itCapsule != pMeshInfo->m_vecColCapsules.end(); itCapsule++)
		{
			if(nCIndex == nCapsuleIndex)
			{
				return &(*itCapsule);
			}

			nCIndex++;
		}
	}

	return NULL;
}

void TMeshColInfoViewer::SetMeshColData( int nColIndex )
{
	// �浹 ĸ�� ������ �����´�.
	CSColCapsule * pCapsuleInfo = GetMeshColCapsuleInfo( nColIndex);
	CSColBox * pBoxInfo = GetMeshColBoxInfo(nColIndex);
	if(pCapsuleInfo != NULL)
	{
		SetMeshColCapsuleDataToProperty(pCapsuleInfo);
	}
	else if(pBoxInfo != NULL)
	{
		SetMeshColBoxDataToProperty(pBoxInfo);
	}
	else
	{
		// ������ ����. �׷��Ƿ� ���� �浹 ĸ�� ���� �����
		GetMeshColInfoViewerPropertGrid()->SelectedObject = nullptr;
	}
}

void TMeshColInfoViewer::ChangeMeshCoInfo( int nColIndex )
{
	// �浹 ĸ�� ������ �����´�.
	CSColCapsule * pCapsuleInfo = GetMeshColCapsuleInfo(nColIndex);
	CSColBox * pBoxInfo = GetMeshColBoxInfo(nColIndex);
	if(pCapsuleInfo != NULL)
	{
		// ����� �����͸� ĸ�� ������ �Է�
		SetPropertyToMeshColCapsuleData(pCapsuleInfo);

		// ����� �����͸� ������ ���� �ٽ� ���
		pCapsuleInfo->m_Capsule.CalcHeight();

		// ����� ���̸� ������Ƽ���� �ٽ� �Է�
		SetMeshColCapsuleDataToProperty(pCapsuleInfo);

		NotifyChanging();
	}
	else if(pBoxInfo != NULL)
	{
		// ����� �����͸� ĸ�� ������ �Է�
		SetPropertyToMeshColBoxData(pBoxInfo);

		// ����� ���̸� ������Ƽ���� �ٽ� �Է�
		SetMeshColBoxDataToProperty(pBoxInfo);

		NotifyChanging();
	}
	else
	{
		// ������ ����. �׷��Ƿ� ���� �浹 ĸ�� ���� �����
		GetMeshColInfoViewerPropertGrid()->SelectedObject = nullptr;
	}
}

void TMeshColInfoViewer::SetMeshColCapsuleDataToProperty(CSColCapsule * pMeshColCapsule)
{
	XmlSerializable::MESHCOLCAPSULE^ capsule = gcnew XmlSerializable::MESHCOLCAPSULE;

	capsule->������		= pMeshColCapsule->m_Capsule.radius;
	capsule->����		= pMeshColCapsule->m_Capsule.height;
	capsule->������1_X	= pMeshColCapsule->m_Capsule.top.x;
	capsule->������1_Y	= pMeshColCapsule->m_Capsule.top.y;
	capsule->������1_Z	= pMeshColCapsule->m_Capsule.top.z;
	capsule->������2_X	= pMeshColCapsule->m_Capsule.bottom.x;
	capsule->������2_Y	= pMeshColCapsule->m_Capsule.bottom.y;
	capsule->������2_Z	= pMeshColCapsule->m_Capsule.bottom.z;
	capsule->��1		= gcnew String(pMeshColCapsule->m_strBoneTop.c_str());
	capsule->��2		= gcnew String(pMeshColCapsule->m_strBoneBottom.c_str());

	GetMeshColInfoViewerPropertGrid()->SelectedObject = capsule;
}

void TMeshColInfoViewer::SetPropertyToMeshColCapsuleData(CSColCapsule * pMeshColCapsule)
{
	XmlSerializable::MESHCOLCAPSULE^ capsule = (XmlSerializable::MESHCOLCAPSULE^)(GetMeshColInfoViewerPropertGrid()->SelectedObject);
	
	if( capsule == nullptr)
	{
		return;
	}

	pMeshColCapsule->m_Capsule.radius		= capsule->������;
	pMeshColCapsule->m_Capsule.height		= capsule->����;
	pMeshColCapsule->m_Capsule.top.x		= capsule->������1_X;
	pMeshColCapsule->m_Capsule.top.y		= capsule->������1_Y;
	pMeshColCapsule->m_Capsule.top.z		= capsule->������1_Z;
	pMeshColCapsule->m_Capsule.bottom.x		= capsule->������2_X;
	pMeshColCapsule->m_Capsule.bottom.y		= capsule->������2_Y;
	pMeshColCapsule->m_Capsule.bottom.z		= capsule->������2_Z;
	pMeshColCapsule->m_strBoneTop			= string(MStringToCharPointer(capsule->��1));
	pMeshColCapsule->m_strBoneBottom		= string(MStringToCharPointer(capsule->��2));
}

void TMeshColInfoViewer::RenderEditionCapsule(MCapsule & capsuleInfo, DWORD dwColor)
{
	// �κ� ���� ������ �����´�.
	MCapsule cap = capsuleInfo;

	//------------------------------------------------------------------------
	// ĸ�� ������
	RenderCapsule(cap, dwColor);

	g_pMainApp->SetCapsuleRenderState();
	//------------------------------------------------------------------------
	// ĸ�� �߾� ��
	RBoundingBox box = RBoundingBox(RVector(-5,-5,-5),RVector(5,5,5));
	RMatrix matTM;
	matTM.MakeIdentity();

	RRenderHelper::RenderLine(RMatrix::IDENTITY, cap.bottom, cap.top, 0xFFFFFF00);

	float fMinZ = (cap.top.z < cap.bottom.z) ? cap.top.z : cap.bottom.z;
	RVector t1 = cap.top;
	t1.z = fMinZ - cap.radius * 1.5f;
	RRenderHelper::RenderLine(RMatrix::IDENTITY, cap.top, t1, 0xFFFF00FF);
	matTM.SetTranslation(t1);
	RRenderHelper::RenderBox(matTM, box, 0xffffffff);
	RRenderHelper::RenderAxis(matTM,30,0.5f);
	g_pMainApp->RenderAxisText(matTM, 30);


	RVector t2 = cap.bottom;
	t2.z = fMinZ - cap.radius * 2.0f;
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

void TMeshColInfoViewer::RenderCapsule(MCapsule & capsuleInfo, DWORD dwColor /* = 0x200000ff */)
{
	g_pMainApp->SetCapsuleRenderState();

	RRenderHelper::RenderCapsule(capsuleInfo, dwColor);

	g_pMainApp->EndCapsuleRenderState();
}

void TMeshColInfoViewer::RenderGroup(int nSelectColID /*= -1*/ )
{
	if(m_strModelName.size() <= 0)
		return;

	// �� Hit ������ �����´�.
	CSMeshInfo * pMeshInfo = TMeshInfoMgr::GetInstance().GetMeshInfo(m_strModelName);
	if(pMeshInfo)
	{
		//////////////////////////////////////////////////////////////////////////
		// ĸ��
		bool bSelectCapsule = false;
		MCapsule renderCapsule;
		MCapsule selectCapsule;
		int nCIndex = 0;

		for(vector<CSColCapsule>::iterator itCapsule = pMeshInfo->m_vecColCapsules.begin();
			itCapsule != pMeshInfo->m_vecColCapsules.end(); itCapsule++)
		{
			renderCapsule = (*itCapsule).m_Capsule;
			
			// �ִϸ��̼��� �����ϰ� ���� �����Ǿ� �ִ°�?
			CalAnimationRenderCapsule(&(*itCapsule), renderCapsule);

			if(nSelectColID != -1)
			{
				// ĸ�� �����̶��... ���� ĸ�� �ܴ̿� ȸ������
				if(nCIndex == nSelectColID)
				{
					bSelectCapsule = true;
					selectCapsule = renderCapsule;
				}
				else
				{
					RenderCapsule(renderCapsule, MESH_COL_CAPSILE_NONE_SELECT);
				}
			}
			else
			{
				// �׷켱���̶�� ��ü�� ��ȫ��
				RenderCapsule(renderCapsule);
			}

			nCIndex++;
		}
		
		if(bSelectCapsule)
		{
			RenderEditionCapsule(selectCapsule, MESH_COL_CAPSULE_SELECT);
		}

		//////////////////////////////////////////////////////////////////////////
		// �ڽ�
		m_MeshColInfoBoxEditor.RenderColBox(nSelectColID);
	}
}

MCapsule * TMeshColInfoViewer::GetCurrentSelectCapsule()
{
	CSColCapsule * pMeshColCapsule = GetMeshColCapsuleInfo(GlobalObjects::g_SelInfo.m_nMeshColID);

	if(pMeshColCapsule)
	{
		return &(pMeshColCapsule->m_Capsule);
	}

	return NULL;
}

void TMeshColInfoViewer::CalAnimationRenderCapsule( CSColCapsule * pColCapsule, MCapsule & outCapsule )
{
	RActorNode* pTopNode		= g_pMainApp->GetActorNode(pColCapsule->m_strBoneTop);
	RActorNode* pBottomNode		= g_pMainApp->GetActorNode(pColCapsule->m_strBoneBottom);
	RMatrix matActor			= g_pMainApp->GetActor()->GetWorldTransform();

	outCapsule.radius *= g_pMainApp->GetActor()->GetScaleUniform();
	matActor.TransformVect(outCapsule.top);
	matActor.TransformVect(outCapsule.bottom);
	outCapsule.CalcHeight();

	if(pTopNode)
	{
		vec3 vLocalTop	= pTopNode->GetWorldPosition() - g_pMainApp->GetActor()->GetWorldPosition();
		vec3 vTopPos;
		matActor.TransformVect(pColCapsule->m_Capsule.top, vTopPos);

		outCapsule.top = vTopPos + vLocalTop;
	}

	if(pBottomNode)
	{
		vec3 vLocalBottom	= pBottomNode->GetWorldPosition() - g_pMainApp->GetActor()->GetWorldPosition();
		vec3 vBottomPos;
		matActor.TransformVect(pColCapsule->m_Capsule.bottom, vBottomPos);

		outCapsule.bottom = vBottomPos + vLocalBottom;
	}

	outCapsule.CalcHeight();
}

void TMeshColInfoViewer::NotifyChanging()
{
	GlobalObjects::g_SaveNotifier.SetSaveMeshInfo(true);
}

int TMeshColInfoViewer::GetMeshColCapsuleCount()
{
	// �� Hit ������ �����´�.
	CSMeshInfo * pMeshInfo = TMeshInfoMgr::GetInstance().GetMeshInfo(m_strModelName);
	if(pMeshInfo)
	{
		return pMeshInfo->m_vecColCapsules.size();
	}

	return 0;
}

void TMeshColInfoViewer::CreateMeshColBoxInfoViewer( int nBoxID /*= -1*/ )
{
	if(nBoxID == -1)
		return;

	// ĸ�� �߰�
	TreeNode^ pBoxNode = GetMeshColInfoViewerTreeNode()->Nodes->Add(String::Format(MESH_COL_INFO_BOX_ID, nBoxID - COLLISION_BOX_START_INDEX));

	// �߰��� ĸ�� ���ó��� ����
	pBoxNode->Tag									= nBoxID;
	GetMeshColInfoViewerTreeNode()->SelectedNode	= pBoxNode;
	GlobalObjects::g_SelInfo.m_nMeshColID			= nBoxID;
}

CSColBox * TMeshColInfoViewer::GetMeshColBoxInfo( int nBoxIndex )
{
	if(nBoxIndex == -1)
	{
		return NULL;
	}

	// �� Hit ������ �����´�.
	CSMeshInfo * pMeshInfo = TMeshInfoMgr::GetInstance().GetMeshInfo(m_strModelName);
	if(pMeshInfo)
	{
		int nCIndex = COLLISION_BOX_START_INDEX;
		for(vector<CSColBox>::iterator itBox = pMeshInfo->m_vecColBox.begin();
			itBox != pMeshInfo->m_vecColBox.end(); itBox++)
		{
			if(nCIndex == nBoxIndex)
			{
				return &(*itBox);
			}

			nCIndex++;
		}
	}

	return NULL;
}

void TMeshColInfoViewer::SetMeshColBoxDataToProperty( CSColBox * pMeshColBox )
{
	XmlSerializable::COLLISION_BOX^ box = gcnew XmlSerializable::COLLISION_BOX;

	vec3 vPos = pMeshColBox->m_matWorld.GetTranslation();
	vec3 vDir = vec3(pMeshColBox->m_matWorld._21, pMeshColBox->m_matWorld._22, pMeshColBox->m_matWorld._23);
	vec3 vUp = vec3(pMeshColBox->m_matWorld._31, pMeshColBox->m_matWorld._32, pMeshColBox->m_matWorld._33);

	vDir.Normalize();
	vUp.Normalize();

	box->center_x			= vPos.x;
	box->center_y			= vPos.y;
	box->center_z			= vPos.z;
	box->dir_x				= vDir.x;
	box->dir_y				= vDir.y;
	box->dir_z				= vDir.z;
	box->up_x				= vUp.x;
	box->up_y				= vUp.y;
	box->up_z				= vUp.z;

	box->min_x				= pMeshColBox->vmin.x;	
	box->min_y				= pMeshColBox->vmin.y;	
	box->min_z				= pMeshColBox->vmin.z;	
	box->max_x				= pMeshColBox->vmax.x;	
	box->max_y				= pMeshColBox->vmax.y;	
	box->max_z				= pMeshColBox->vmax.z;	

	GetMeshColInfoViewerPropertGrid()->SelectedObject = box;

}

void TMeshColInfoViewer::SetPropertyToMeshColBoxData( CSColBox * pMeshColBox )
{
	XmlSerializable::COLLISION_BOX^ box = (XmlSerializable::COLLISION_BOX^)(GetMeshColInfoViewerPropertGrid()->SelectedObject);

	if( box == nullptr)
	{
		return;
	}

	vec3 vPos;
	vec3 vDir;
	vec3 vUp;

	vPos.x							= box->center_x;
	vPos.y							= box->center_y;
	vPos.z							= box->center_z;
	vDir.x							= box->dir_x;
	vDir.y							= box->dir_y;
	vDir.z							= box->dir_z;
	vUp.x							= box->up_x;
	vUp.y							= box->up_y;
	vUp.z							= box->up_z;

	vDir.Normalize();
	vUp.Normalize();
	pMeshColBox->m_matWorld.SetLocalMatrix(vPos, vDir, vUp);

	pMeshColBox->vmin.x				= box->min_x;
	pMeshColBox->vmin.y				= box->min_y;
	pMeshColBox->vmin.z				= box->min_z;
	pMeshColBox->vmax.x				= box->max_x;
	pMeshColBox->vmax.y				= box->max_y;
	pMeshColBox->vmax.z				= box->max_z;
}

bool TMeshColInfoViewer::CheckMousePick( int stat, RVector2& pos, RVector2 prePos )
{
	if(m_MeshColInfoBoxEditor.CheckMousePick(stat, pos, prePos))
	{
		GlobalObjects::g_SaveNotifier.SetSaveMeshInfo(true);

		return true;
	}

	return false;
}

bool TMeshColInfoViewer::CheckMouseDown( int stat, RVector2& pos, RVector2 prePos )
{
	return m_MeshColInfoBoxEditor.CheckMouseDown(stat, pos, prePos);
}