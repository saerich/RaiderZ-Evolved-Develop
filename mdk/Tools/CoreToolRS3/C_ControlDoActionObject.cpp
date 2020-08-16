#include "StdAfx.h"
#include "C_ControlDoAction.h"

#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_ControlMediatorHelperResource.h"
#include "C_ControlMediatorHelperRender.h"

#include "M_ToolLogicObjectManager.h"
#include "M_ToolCmdMgr.h"
#include "M_CommandBuffer.h"
#include "M_LogicObjectCommandBuffer.h"
#include "M_ToolSceneNodeInstance.h"
#include "M_LogicObject_SceneNodes.h"

#include "C_XMLConfig.h"
#include "C_ControlCursor.h"
#include "C_StatusBarHandler.h"

#include "VL_SceneNodeList.h"
#include "VR_ZoneObjectNew.h"
#include "VL_ToolBar.h"
#include "VG_Gizmo.h"
#include "VL_Objects.h"

#include "C_XMLEditInfoFile.h"
#include "RUtil.h"

#include "ext_lib/FooButton.h"

#include "WS_WorkSpaceManager.h"
#include "Gizmo.h"
#include "MainFrm.h"

#pragma warning( push )
#pragma warning( disable : 4996 )

using namespace rs3;

//edit ��尡 �ƴ� �ٸ� �׼ǵ��� �̸� �׼��� �����ش�.(pre-action)
const CControlDoAction::OBJECT_EDIT_TYPE CControlDoAction::GetObjectEditTypeAndPreAction(CControlMediator* _pController)
{
	static OBJECT_EDIT_TYPE edit_type = EDIT_INIT;

	BOOL isLocked = _pController->GetViewPtr<CCoreToolToolBar>()->m_btnObjectLock.isChecked();

	// ���� Ŀ����ġ�� ����� �ִ°����� ����üũ
	VG_Gizmo* pGizmo = _pController->GetViewPtr<VG_Gizmo>();
	bool bGizmoSelected = ( pGizmo->GetEnable() && (pGizmo->GetGizmoState() != AXIS_NULL) );

	//ctrlŰ�� �����ְ� ���� ����� �ȵǾ� �ִٸ�
	if(GetAsyncKeyState(VK_LCONTROL) && isLocked == FALSE)
	{
		edit_type = OBJECT_SELECT;
	}
	//�巡���϶�
	else if(_pController->m_mouseInfo.mouseState.bDrag)
	{
		//shift �巡�� - ����
		if(	GetAsyncKeyState(VK_SHIFT) )
		{
			rs3::RSceneNode* pSelectNodeAtMouse = NULL;
			CLogicObject* pLogicObject = NULL;
			//�����Ϸ��� ��ü�� �����ߴ��� Ȯ��
			_pController->GetHelperAction()->GetWBObjectAtCurrentMouse(&pSelectNodeAtMouse, &pLogicObject);

			// logic object ī�Ƿ� ������
			if( (pLogicObject != NULL &&
				_pController->GetHelperAction()->IsInSelectObjectList( pLogicObject) ) || //�����ѹ�ü
				bGizmoSelected || isLocked)
				edit_type = OBJECT_COPY;

			// TODO: ����
// 			if(_pController->m_mapInfo.m_bEditSceneObject)
// 			{
// 				//��ü�� �����ؼ� �����ϰų� ��������� ����� ��ü�� ����
// 				if( (pSelectNodeAtMouse != NULL &&
// 					_pController->GetHelperAction()->IsInSelectObjectList(pSelectNodeAtMouse, pLogicObject) ) || //�����ѹ�ü
// 					isLocked)
// 					edit_type = OBJECT_COPY;
// 			}
		}
	}
	//���콺 ��ư �ٿ��϶�
	else if(_pController->m_mouseInfo.mouseState.bLButton)
	{
		//��ü�� ��� �ִٸ�, ������ �������
		if(isLocked == TRUE)
		{
			// ������ ������� ������� ��ġ�� �׼��÷� ���������� ����
			//if(bGizmoSelected)
			{
				// action plane position �� ����
				RVector vecPosition = pGizmo->GetGizmoPosition();
				_pController->GetHelperAction()->SetActionPlanePosAs( vecPosition );
			}
			edit_type = OBJECT_EDIT;
			return OBJECT_EDIT;
		}

		// ����� �����ߴٸ� ������ �������
		if(bGizmoSelected)
		{
			RVector vecPosition = pGizmo->GetGizmoPosition();
			_pController->GetHelperAction()->SetActionPlanePosAs( vecPosition );

			edit_type = OBJECT_EDIT;
			return OBJECT_EDIT;
		}

		//������� �ʴٸ� ���� Ŀ�� ��ġ�� �ִ� ������Ʈ�� ��´�
		rs3::RSceneNode* pSelectNodeAtMouse = NULL;
		CLogicObject* pLogicObject = NULL;
		_pController->GetHelperAction()->GetWBObjectAtCurrentMouse(&pSelectNodeAtMouse, &pLogicObject);

		// ���� Ŀ�� ��ġ�� �ִ� ������Ʈ�� ������ ���� ����
		if(pSelectNodeAtMouse == NULL && pLogicObject == NULL && !bGizmoSelected )
		{
			_pController->GetHelperAction()->RemoveAllSelection(true);
			edit_type = OBJECT_SELECT;
		}
		else if( !GetAsyncKeyState(VK_SHIFT) ) // �巡�� ���縦 ���� ����Ʈ�� �ƴ϶��
		{
			// ���� Ŀ�� ��ġ�� �ִ°��� �̹� ���õǾ��ִ� �ƴ϶�� ���� ���� ����
			if ( _pController->GetHelperAction()->IsInSelectObjectList( pLogicObject) == false && !bGizmoSelected)
			{
				_pController->GetHelperAction()->RemoveAllSelection(true);
				CControlDoAction::ObjectSelect(_pController);
			}

			//Ŭ���Ŀ� �巡�� �ϸ� �ٷ� ������Ʈ ����Ʈ�� �� �� �ֵ���
			edit_type = OBJECT_EDIT;

			if(bGizmoSelected)
			{
				// action plane position �� ����
				RVector vecPosition = pGizmo->GetGizmoPosition();
				_pController->GetHelperAction()->SetActionPlanePosAs( vecPosition );
			}
		}
	}

	//selection �϶� �����ϱ�
	if(edit_type == OBJECT_SELECT)
	{
		CControlDoAction::ObjectSelect(_pController);
	}
	//copy ����϶�
	else if(edit_type == OBJECT_COPY)
	{
		//��Ʈ�� �׽�Ʈ, ���콺 Ŀ�ǵ� ��Ʈ���϶��� ����
		if(_pController->GetToolCmdMgr()->IsMouseCmdEntryL())
		{
//			bool bCopySucceed = _pController->GetHelperAction()->CopyDeepSelectedZoneObject();
			bool bCopySucceed = _pController->GetHelperAction()->CopyDeepSelectedObject();
			//���� ����
			if(!bCopySucceed)
			{
				//���¸� ��ü �������·� �ٲ��ش�.
				edit_type = OBJECT_EDIT;
			}	
			//Scale State ���ٸ� Move State �� �ٲ��ش�.
			else
			{
				if(_pController->GetCurrentWBState() == WORKBENCH_STATE::OBJECT_SCALE)
					_pController->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::OBJECT_MOVE, true);

				// move observer ���
				_pController->GetHelperAction()->AddToWBMouseObserver( CControlMediatorHelperAction::WB_MOUSE_L_UP, -1);
			}
			_pController->GetToolCmdMgr()->DoneMouseCmdL();
		}
		else
			edit_type = OBJECT_EDIT;
	}
	return edit_type;
}

bool CControlDoAction::IsEditableSceneNode( rs3::RSceneNode* pSceneNode )
{
	if ( pSceneNode->GetUserData() == NULL )
		return false;

	return true;
}

void CControlDoAction::UpdateDragText(CControlMediator* _pController, const char* szDragText)
{
	VG_Gizmo* pGizmo = _pController->GetViewPtr<VG_Gizmo>();
	Gizmo* pCurrentGizmo = pGizmo->GetCurrentGizmo();

	if( pCurrentGizmo == NULL ) return;

	// 2D ��ǥ ���Ѵ�.
	MVector2 vScreenPos = _pController->GetToolCamera()->GetScreenPosition( pCurrentGizmo->GetPosition() );
	
	vScreenPos.x -= 25;
	vScreenPos.y -= 85;

	_pController->GetHelperRender()->GetDragInfoText( CCoreToolToolBar::ROTATION_DRAG_INFO_ID,
		vScreenPos.x, vScreenPos.y) = szDragText;
}

bool CControlDoAction::MakeRotationMatrixAndInfoText(rs3::RMatrix& _rOut, CControlMediator* _pController, UINT nAxis)
{
	char szRotationInfo[256] = {0, };
	CCoreToolToolBar* pTopBar =  _pController->GetViewPtr<CCoreToolToolBar>();
	_rOut.MakeIdentity();

	float fDegree = (float)(_pController->m_mouseInfo.m_currentMouseY - _pController->m_mouseInfo.m_orgMouseY);

	VG_Gizmo* pGizmo = _pController->GetViewPtr<VG_Gizmo>();
	Gizmo* pCurrentGizmo = pGizmo->GetCurrentGizmo();

	// 2D ��ǥ ���Ѵ�.
	MVector2 vScreenPos = _pController->GetToolCamera()->GetScreenPosition( pCurrentGizmo->GetPosition() );

	// �巡�� ������ �ݴ�� �ݴ�������� ������ ?
	if( vScreenPos.x < _pController->m_mouseInfo.m_orgMouseX )		// ������ Ŭ��
		fDegree *= -1;

	CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	int nAngleSnapSize = pMainFrame->GetAngleSnapSize();
	if( nAngleSnapSize > 0 )
		fDegree = (int)((fDegree + .5f * float(nAngleSnapSize)) / nAngleSnapSize) * nAngleSnapSize;

	const RMatrix& localFrame = _pController->GetHelperAction()->GetSelectedObjectActionFrame();
	RVector vBaseXAxis(localFrame._11, localFrame._12, localFrame._13);
	RVector vBaseYAxis(localFrame._21, localFrame._22, localFrame._23);
	RVector vBaseZAxis(localFrame._31, localFrame._32, localFrame._33);

	switch ( nAxis )
	{
	case MOVEAXIS_X :
		_rOut.SetRotationMatrixAxis(vBaseXAxis, DegreeToRadian(fDegree));
		sprintf(szRotationInfo, "Rotation X : %.2f", fDegree);
		break;
	case MOVEAXIS_Y :
		_rOut.SetRotationMatrixAxis(vBaseYAxis, DegreeToRadian(fDegree));
		sprintf(szRotationInfo, "Rotation Y : %.2f", fDegree);
		break;
	case MOVEAXIS_Z :
		_rOut.SetRotationMatrixAxis(vBaseZAxis, DegreeToRadian(fDegree));
		sprintf(szRotationInfo, "Rotation Z : %.2f", fDegree);
		break;
	default:
		return false;
	}

	UpdateDragText( _pController, szRotationInfo );

	return true;
}

using namespace rs3;

rs3::RMatrix ns_matRotate;
std::map< DWORD_PTR, DWORD_PTR > ns_mapObjRotOrg;
rs3::RVector ns_vecPosition;
std::map< DWORD_PTR, DWORD_PTR > ns_mapObjPosOrg;


void CControlDoAction::ObjectNew(CControlMediator* _pController)
{
	// PreAction
	CDoActionObjectNew* pObjectNewAction = _pController->GetWorkSpaceMgr()->GetDoActionObjectNew();
	if (pObjectNewAction && pObjectNewAction->DoPreAction(_pController))
		return;

	RToolSceneManager* pSceneManager = _pController->GetToolSceneManager();
	if( pSceneManager == NULL )
		return;

	static string beforeString(""); //Ŀ���� ���� �޽� ���̱�
	std::string *pStrCurrentFile = NULL;
	std::string *pStrCurrentID = NULL;
	std::string *pStrCurrentPathName = NULL;

	static LONG beforeX = -1;
	static LONG beforeY = -1;

	//for logic
	static CLogicObject* pLogicReferenceObject = NULL;
	static CLogicObject* pLogicObjectCurrentSawed = NULL;
	if (false == _pController->m_mapInfo.m_bEditSceneObject && NULL == pObjectNewAction)
		return;

	//zone �� �̸鼭, Entity ���� ���õǾ� ���� ���� ��� : Scene �������
	if (_pController->m_mapInfo.m_bEditSceneObject)
	{
		pStrCurrentFile = &_pController->GetViewPtr<CRollupZoneObjectNew>()->m_strCurrentFile;
		pStrCurrentID = &_pController->GetViewPtr<CRollupZoneObjectNew>()->m_strCurrentType;
		pStrCurrentPathName = &_pController->GetViewPtr<CRollupZoneObjectNew>()->m_strFullPathName;
	}
	//field �̰ų�, Ȥ�� zone �ε� Entity ���� ���õ� ���
	else
	{
		_ASSERT(pObjectNewAction);
		pLogicReferenceObject = pObjectNewAction->OnGetReferenceObjectAndSetCursor(_pController, pLogicReferenceObject);
		if (pLogicReferenceObject)
			_pController->m_mouseInfo.m_bPopAble = false;
	}

	// RButton �� ������ ������, Unselect ���·� �����.
	if ( _pController->m_mouseInfo.mouseState.bRButton || GetAsyncKeyState(VK_ESCAPE) )
	{
		_pController->m_mouseInfo.m_bPopAble = true;
		_pController->m_mouseInfo.mouseState.bRButton = false;

		if(_pController->m_mapInfo.m_bEditSceneObject)
		{
			//���� �ʱ�ȭ
			beforeString = "";
			*pStrCurrentFile = "";
			*pStrCurrentID = "";

			//rollback action
			const WORKBENCH_STATE::STATE wbstate = _pController->GetPreviousWBState();
			if( wbstate == WORKBENCH_STATE::OBJECT_SELECT || wbstate == WORKBENCH_STATE::OBJECT_MOVE ||
				wbstate == WORKBENCH_STATE::OBJECT_ROTATE || wbstate == WORKBENCH_STATE::OBJECT_SCALE )
				_pController->GetWorkSpaceMgr()->TriggerWBState(wbstate, true);
			_pController->GetViewPtr<CRollupZoneObjectNew>()->NoneSelect();
		}
		else //Logic
		{
			pLogicReferenceObject = NULL;
			pLogicObjectCurrentSawed = NULL;
			_pController->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::OBJECT_SELECT, true);
		}

		L3DCursor::GetInstance()->SetType(L3DCursor::NONE, false);
		return;
	}

	const RVector* pMousePosVec = _pController->GetHelperAction()->GetCurrentPickingPosition();

	RVector vMousePosition = pMousePosVec ? *pMousePosVec : RVector(0,0,0);
	bool bIsVaildNavigation = true;
	if (_pController->m_mapInfo.m_bEditSceneObject)
	{
	}
	else	// field �� �� 
	{
		_ASSERT(pObjectNewAction);
		bIsVaildNavigation = pObjectNewAction->OnCheckValidLocatePosition(_pController, vMousePosition);
	}

	//���� ���� �������� ������ new ������ ���ٸ� ���ø��� ����.
	if( _pController->m_mouseInfo.mouseState.bEvented || _pController->m_mouseInfo.mouseState.bDrag)
	{
		bool bSelectedForNewObject = true;
		if(_pController->m_mapInfo.m_bEditSceneObject)
		{
			//zone ����� ���, pStrCurrentFile�� ���� ���� ���ÿ��θ� Ȯ���� �� �ִ�.
			if(pStrCurrentFile->empty())
				bSelectedForNewObject = false;
		}
		else
		{
			//field ����� ���, pEntityObject �� ���� ���η� ���� ���ÿ��θ� �� �� �ִ�.
			if(pLogicReferenceObject == NULL)
				bSelectedForNewObject = false;
		}

		//���� ���� ���� � ���õ� �Ǿ� ���� �ʴٸ�,
		if(bSelectedForNewObject == false)
		{
			//�ٸ����� �׳� Ŭ���������� �ʱ�ȭ ���ش�
			if ( !GetAsyncKeyState(VK_LCONTROL) && _pController->m_mouseInfo.mouseState.bEvented &&
				_pController->m_mouseInfo.mouseState.bLButton && !_pController->m_mouseInfo.mouseState.bDrag)
				_pController->GetHelperAction()->RemoveAllSelection(true);

			CControlDoAction::ObjectSelect(_pController);
			//�ݹ�׼��� ObjectSelect �׼��� OnTime�׼��� new���� �θ��� ������ ���ִ� �۾���
			_pController->m_mouseInfo.mouseState.bEvented = false;
			return;
		}

		//Field, Entity Object ���Ŀ� �ٷ� ��ü����
		if( !_pController->m_mapInfo.m_bEditSceneObject && _pController->m_mouseInfo.mouseState.bDrag )
		{
			if(pLogicObjectCurrentSawed != NULL)
			{
				_ASSERT(pObjectNewAction);
				pObjectNewAction->OnEditSawedObject(_pController, pLogicObjectCurrentSawed);
				_pController->m_mouseInfo.m_movedMouseX = 0; //moved �ʱ�ȭ
			}
		}
	}

	//������ �ٲ���ٸ� Ŀ���� �޸� ��ü�� �ٲ��ش�.
	if (_pController->m_mapInfo.m_bEditSceneObject && beforeString != *pStrCurrentFile && !pStrCurrentFile->empty() )
	{
		//������ ������ ��ư�� �˾��޴��� �ƴ϶� new ĵ���̾�� �Ѵ�.
		_pController->m_mouseInfo.m_bPopAble = false;
		// Ŀ���� ���� ������Ʈ�� �����Ѵ�.
		L3DCursor::GetInstance()->SetNodeFile( pStrCurrentFile->c_str(), pStrCurrentID->c_str());
		L3DCursor::GetInstance()->SetType(L3DCursor::NODE, false);
		L3DCursor::GetInstance()->SetColor(D3DCOLOR_XRGB(250, 0, 250));
		beforeString = *pStrCurrentFile;
	}

	//�̵��϶��� ���콺�� �޷��ִ� ��� ������Ʈ
	// View ���� �ȿ� ���� ������, Mouse ��ǥ�� ���ŵǱ� ������ - �̷��� �Ǵ��� �� �ִ�.
	if ( _pController->m_mouseInfo.m_currentMouseX != beforeX || _pController->m_mouseInfo.m_currentMouseY != beforeY )
	{
		if(_pController->m_mapInfo.m_bEditSceneObject)
		{
			L3DCursor::GetInstance()->SetTransform( vMousePosition, RVector(1, 1, 1));
		}
		else
		{
			// Ŀ�� ���ü�. �ٸ� �����ǿ� ������ ��ġ�� ������ ������ Ŀ������ ������ ��ģ��.
			RVector vPos(vMousePosition);
			vPos.z += 150.f;
			L3DCursor::GetInstance()->SetTransform(vPos, RVector(100, 100, 400));
		}
	}

	// ��� ���� ��ü�� ������ �÷�����
	CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
	if( pCommandManager->IsMouseCmdEntryL())
	{
		if(_pController->m_mapInfo.m_bEditSceneObject)
		{
			if(pSceneManager == NULL)
				return;

			RSceneNode *pCurrentNode = L3DCursor::GetInstance()->GetNode();
			if ( pCurrentNode == NULL )
				return;

			//////////////////////////////////////////////////////////////////////////
			// ����
			RSceneNode *pNode = pCurrentNode->Clone();	// �� ���� �ν��Ͻ� ����
			if( pNode == NULL )
				return;

			pSceneManager->AddSceneNode(pNode, true);

			//////////////////////////////////////////////////////////////////////////
			// update
			RSceneNodeUpdateVisitor visitor;
			pNode->Traverse(&visitor);

			CLogicObject* pLogicObject = pSceneManager->CreateLogicObject( pNode );
			if( !pLogicObject )
			{
				pNode->RemoveFromParent();
				delete pNode;
				return;
			}

			CDlgObjects* pDlgObjects = _pController->GetViewPtr<CDlgObjects>();
			pLogicObject->SetLayerName( pDlgObjects->GetDefaultLayerName() );

			
			// ������ ����
			CControlSceneProperty* pProperty = pLogicObject->GetProperty();
			if(pProperty)
				pProperty->SetNewInstance(pNode, pCurrentNode, false);

			// �̸� �ٽ� ����, CreateLogicObject �϶� �̸��� ���ٰ� SetNewInstance �� �̸��� �ٿ��ֱ� ������
			// LogicObject ������ ���İ� �ȵ� ����
			
			// TODO: ������ �Ϲ�ȭ �ǰ��� �̰����� SceneNode �����κ� ��ü�� ������
			pLogicObject->SetObjectName( pNode->GetNodeName().c_str() );

			//////////////////////////////////////////////////////////////////////////
			// SceneNodeInstance �̰� As Copy �̸� Convert���ش�
			RToolSceneNodeInstance* pSceneInstance = dynamic_cast<RToolSceneNodeInstance*>(pNode);
			CRollupZoneObjectNew* pNewView = _pController->GetViewPtr<CRollupZoneObjectNew>();
			if( pSceneInstance && pNewView->IsCreatingAsInstance() == false)
			{
				RSceneNodeList nodeList;

				// ���⼭ addtomanager
				pSceneManager->MakeSceneNodeInstanceEditable( pSceneInstance, &nodeList );

				//////////////////////////////////////////////////////////////////////////
				//Undo Buffer
				CGroupedCommandBuffer* pGroupCmdBuffer = new CGroupedCommandBuffer;
				for(RSceneNodeList::iterator itr = nodeList.begin(); itr != nodeList.end(); ++itr )
				{
					CObjectNewCmdBuffer* pCmdBuffer = new CObjectNewCmdBuffer( pSceneManager, *itr );
					pGroupCmdBuffer->AddBuffer( pCmdBuffer );
				}

				pCommandManager->RecordCommand(pGroupCmdBuffer);

				// ��� �����Ѱ͵��� ����
				for( RSceneNodeList::iterator itr = nodeList.begin(); itr != nodeList.end(); ++itr )
				{
					RSceneNode* pEachNode = *itr;
					CLogicObject_SceneNode* pEachLogicObject = dynamic_cast<CLogicObject_SceneNode*>((CCoreObject*)pEachNode->GetUserData());
					_pController->GetHelperAction()->AddSelectection( pEachLogicObject, true );
				}

			}else
			{
				_pController->GetToolLogicObjectMgr()->AddToManager( pLogicObject );

				//////////////////////////////////////////////////////////////////////////
				//Undo Buffer
				CObjectNewCmdBuffer* pCmdBuffer = new CObjectNewCmdBuffer(pSceneManager, pNode);
				pCommandManager->RecordCommand(pCmdBuffer);

				// ��� �����Ѱ��� ����
				_pController->GetHelperAction()->AddSelectection( pLogicObject, true );
			}


			//////////////////////////////////////////////////////////////////////////
			// cursor ��带 �ٽ� �����Ѵ�
			RSceneNode* pNewCursorNode = pCurrentNode->Clone();
			pNewCursorNode->SetUserData(pCurrentNode->GetUserData());

			// SceneNode ListBox �� Ŀ�� ��尡 ������ ������ �����
			L3DCursor::GetInstance()->SetNode(NULL);

			// ListBox�� ������Ʈ ��Ų��.
			_pController->GetViewPtr<CDlgSceneNodeList>()->UpdateTreeControl();


			L3DCursor::GetInstance()->SetNode( pNewCursorNode );	// ���ܳ��� Ŀ�� �ٽ� ����
		}
		else	// Logic ���� ����� ��
		{
			if(pLogicReferenceObject == NULL)
				return;

			pLogicObjectCurrentSawed = pLogicReferenceObject->Clone();
			if(pLogicObjectCurrentSawed)
			{
				//����� �̸��� �����ϰ� �ش޶�� ��û�� �־� �����մϴ�. JungChulHee(2011.06.07)
				//give unique name to new object
				//const char* szUniqueName = _pController->GetToolLogicObjectMgr()->GetUniqueObjectNameOf( pLogicReferenceObject->GetObjectName() );
				//pLogicObjectCurrentSawed->SetObjectName( szUniqueName );
				pLogicObjectCurrentSawed->SetObjectName( pLogicReferenceObject->GetObjectName() );

				pLogicObjectCurrentSawed->SetPosition( vMousePosition );

				CDlgObjects* pDlgObjects = _pController->GetViewPtr<CDlgObjects>();
				pLogicObjectCurrentSawed->SetLayerName( pDlgObjects->GetDefaultLayerName() );

				//For Undo
				ICommandBuffer* pNewCmdBuffer = pLogicObjectCurrentSawed->CreateCloneNewCmdBuffer();
				if(pNewCmdBuffer)
					pCommandManager->RecordCommand(pNewCmdBuffer);

				_pController->GetToolLogicObjectMgr()->AddToManager(pLogicObjectCurrentSawed);

				if(pObjectNewAction)
					pObjectNewAction->OnObjectCloned(_pController, pLogicObjectCurrentSawed);

				// ��� �����Ѱ��� ����
				_pController->GetHelperAction()->AddSelectection( pLogicObjectCurrentSawed, true );

				pLogicObjectCurrentSawed->OnLoadCompleted();
			}
		}
		pCommandManager->DoneMouseCmdL();
	}

	beforeX = _pController->m_mouseInfo.m_currentMouseX;
	beforeY = _pController->m_mouseInfo.m_currentMouseY;
}

void CControlDoAction::ObjectSelect(CControlMediator* _pController)
{
	// PreAction
	CDoActionObjectSelect* pObjectSelectAction = _pController->GetWorkSpaceMgr()->GetDoActionObjectSelect();
	if (pObjectSelectAction && pObjectSelectAction->DoPreAction(_pController))
		return;

	//for Zone static Object Selection
	RToolSceneManager* pManager = NULL;
	CToolLogicObjectManager* pLogicObjectManager = NULL;

	//���� �巡�� ���� ���� ���콺 Ŀ�� Ÿ�Ե� �����Ƿ�, �� �������ø��� Ŀ�� �ʱ�ȭ
	L3DCursor::GetInstance()->SetType(L3DCursor::NONE, false);

	//Lock Ȯ��
	BOOL isLocked = _pController->GetViewPtr<CCoreToolToolBar>()->m_btnObjectLock.isChecked();

	//////////////////////////////////////////////////////////////////////////
	//
	// ���������� ���־�� �� ����
	// �ϴ� �� ��������� Static Object�� ���ؼ��� Shift Copy�� �����Ѵ�.
	//
	if(_pController->m_mapInfo.m_bEditSceneObject)
	{
		pManager = _pController->GetToolSceneManager();

		//shitf�϶��� clone ���
		if(	GetAsyncKeyState(VK_SHIFT) && _pController->m_mouseInfo.mouseState.bLButton && _pController->GetToolCmdMgr()->IsMouseCmdEntryL())
		{
			const SCENEOBJECTMAP *pSelectList = _pController->GetHelperAction()->GetSelectedSceneObject();

			//return point
			if( pSelectList->empty() ) 
				return;

			if(!isLocked)
			{
				rs3::RSceneNode* pSelectNodeAtMouse = NULL;
				CLogicObject* pLogicObject = NULL;
				_pController->GetHelperAction()->GetWBObjectAtCurrentMouse(&pSelectNodeAtMouse, &pLogicObject);
				if( !_pController->GetHelperAction()->IsInSelectObjectList( pLogicObject) )
					return;
			}
			//Copy�� MOVE���·� ������ ����
			_pController->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::OBJECT_MOVE, true);
			return;
		}
	}
//	else //for field // scenenode/logicobject �Ѵ� �����ϵ��� ����
	{
		pLogicObjectManager = _pController->GetToolLogicObjectMgr();
	}

	//����ִٸ� Ŭ���� �����ϵ� �������� �׳� ����������
	if(isLocked == TRUE)
		return;

	//////////////////////////////////////////////////////////////////////////
	//
	// �巡�� / �巡�� �Ϸ�
	// Ctrl Selection, Ctrl + Alt Selection, Selection �� ���� ó��
	//

	//ctrl ����
	bool bCtrlPressd = GetAsyncKeyState(VK_LCONTROL) ? true : false;

	// �巡�� / �巡�� �ϷḦ ���� ���� �巡�� ���� üũ
	static bool bBeforeLDrag = false;
	if(_pController->m_mouseInfo.mouseState.bDrag && !bBeforeLDrag)
		bBeforeLDrag = true;

	//drag - drag�� drag�Ϸ��� ����
	static CPoint pointBegin, pointEnd; //�巡�� ���� �巡�� ����Ʈ
	if( _pController->m_mouseInfo.mouseState.bDrag)
	{
		//POINT pointBegin, pointEnd;
		pointBegin.x = _pController->m_mouseInfo.m_orgMouseX;
		pointBegin.y = _pController->m_mouseInfo.m_orgMouseY;
		pointEnd.x = _pController->m_mouseInfo.m_currentMouseX;
		pointEnd.y = _pController->m_mouseInfo.m_currentMouseY;

		// pointBegin�� �׻� �»�ܿ� �÷����´�.
		if ( pointBegin.x > pointEnd.x )
			std::swap(pointBegin.x, pointEnd.x);
		if ( pointBegin.y > pointEnd.y )
			std::swap(pointBegin.y, pointEnd.y);

		L3DCursor::GetInstance()->SetType( L3DCursor::BOX2D, false );
		L3DCursor::GetInstance()->SetColor( D3DCOLOR_XRGB(255, 255, 255));
		L3DCursor::GetInstance()->SetTransform(
			RVector((float)pointBegin.x,(float)pointBegin.y, 0.0f)
			,RVector((float)pointEnd.x ,(float)pointEnd.y, 0.0f));
	}

	//drag �Ϸ�
	if ( !_pController->m_mouseInfo.mouseState.bDrag && bBeforeLDrag)
	{
		bBeforeLDrag = false;

		if ( pointBegin.x == pointEnd.x || pointBegin.y == pointEnd.y )
			return;

		// ���콺�� �� ���� �̿��Ͽ� ���������� �������� ����� ,
		RECT rt;
		rt.left = 0;
		rt.top = 0;
		_pController->GetWBSize(rt.right, rt.bottom);

		RDevice *pDevice = _pController->GetCurrentDevice();
		RMatrix viewMat = pDevice->GetTransform(RST_VIEW);
		RMatrix projMat = pDevice->GetTransform(RST_PROJECTION);

		RVector vCameraPos = _pController->GetToolCamera()->GetPosition();
		float fFarZ = _pController->GetToolCamera()->GetFarZ();
		float fNearZ = _pController->GetToolCamera()->GetNearZ();

		RVector vp1, vq1;
		RVector vp2, vq2;
		RVector vp3, vq3;
		RVector vp4, vq4;

		RGetScreenLine(pointEnd.x, pointEnd.y, rt, viewMat, projMat		,&vp1);
		RGetScreenLine(pointBegin.x, pointBegin.y, rt, viewMat, projMat	,&vp2);
		RGetScreenLine(pointBegin.x, pointEnd.y, rt, viewMat, projMat	,&vp3);
		RGetScreenLine(pointEnd.x, pointBegin.y, rt, viewMat, projMat	,&vp4);

		vq1 = vCameraPos + vp1*fFarZ;
		vp1 = vCameraPos + vp1*fNearZ;
		vq2 = vCameraPos + vp2*fFarZ;
		vp2 = vCameraPos + vp2*fNearZ;
		vq3 = vCameraPos + vp3*fFarZ;
		vp3 = vCameraPos + vp3*fNearZ;
		vq4 = vCameraPos + vp4*fFarZ;
		vp4 = vCameraPos + vp4*fNearZ;

		// �װ��� �̿��Ͽ� Frustum �� �����. Plane ������ �������� -
		RViewFrustum kFrustum;

		RPlane nearPlane(vp2, vp1, vp3);
		RPlane farPlane(vq2, vq3, vq1);
		RPlane leftPlane(vp2, vp3, vq2);
		RPlane rightPlane(vp1, vp4, vq4);
		RPlane topPlane(vp4, vp2, vq2);
		RPlane bottomPlane(vp3, vp1, vq1);

		kFrustum.SetPlane(RV_NEAR,nearPlane);
		kFrustum.SetPlane(RV_FAR ,farPlane);
		kFrustum.SetPlane(RV_LEFT,leftPlane);
		kFrustum.SetPlane(RV_RIGHT,rightPlane);
		kFrustum.SetPlane(RV_TOP,topPlane);
		kFrustum.SetPlane(RV_BOTTOM,bottomPlane);

		//ctrl Ű�� ������ �ʾҴٸ� ���������� �ƴ�
		if (!bCtrlPressd)
			_pController->GetHelperAction()->RemoveAllSelection(false); //Update UI�� �������࿡��

		//ctrl + alt �̸� ���ÿ��� ������
		bool bDragRemoveMode = bCtrlPressd && GetAsyncKeyState(VK_MENU);

		//////////////////////////////////////////////////////////////////////////
		//
		// ���� ����
		// Ctrl Selection, Ctrl + Alt Selection, Selection �� ���� ó��
		//

		ASSERT(pLogicObjectManager);
		std::vector<CLogicObject*> vecPickEntityObject;
		pLogicObjectManager->PickLogicObjectFrustumForSelection(vecPickEntityObject, kFrustum);
		int nSize = (int)vecPickEntityObject.size();
		_pController->GetHelperAction()->SetSelectedLogicObjectCntForDoAction(nSize);

		bool bOnlyEntityObject = true;
		for ( int i = 0; i < nSize;)
		{
			CLogicObject* pLogicObject = vecPickEntityObject[i];

			if(bDragRemoveMode)
				_pController->GetHelperAction()->RemoveSelection( pLogicObject, true);
			else
				_pController->GetHelperAction()->AddSelectection( pLogicObject, true);

			if (pLogicObject->IsEntityLogic() == false)
			{
				bOnlyEntityObject = false;
			}

			++i;
			_pController->GetHelperAction()->SetSelectedLogicObjectCntForDoAction(nSize - i);
		}

		if (pObjectSelectAction)
			pObjectSelectAction->OnSelectChanged(_pController, bOnlyEntityObject);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// ���콺 ��ư Ŭ���� - callback �׼��̹Ƿ� �ٿ��� Ŭ���� �ǹ�
	//
	if (!_pController->m_mouseInfo.mouseState.bDrag && _pController->m_mouseInfo.mouseState.bLButton)
	{
		const RVector *pOrigin(NULL), *pTo(NULL);
		_pController->GetHelperAction()->GetCurrentMouseLine(&pOrigin, &pTo);
		if( pOrigin && pTo)
		{
			//������ ��ü
			CLogicObject* pLogicObject = NULL;

			ASSERT(pLogicObjectManager);
			pLogicObject = pLogicObjectManager->PickLogicObjectForSelection(*pOrigin, *pTo);

			// set selected logic object count
			_pController->GetHelperAction()->SetSelectedLogicObjectCntForDoAction(1);
			if ( !bCtrlPressd )
			{
				//���ϱ� ������ �ƴϹǷ� ���� �ʱ�ȭ
				_pController->GetHelperAction()->RemoveAllSelection(true);
				//���ϱ�
				_pController->GetHelperAction()->AddSelectection( pLogicObject, true);
			}
			else
			{
				//���ϱ� ����
				if(_pController->GetHelperAction()->IsInSelectObjectList( pLogicObject))
					_pController->GetHelperAction()->RemoveSelection( pLogicObject, true);
				else
					_pController->GetHelperAction()->AddSelectection( pLogicObject, true);
			}

			if( !_pController->m_mapInfo.m_bEditSceneObject )
			{
				if(pLogicObject)
				{
					if (pObjectSelectAction)
						pObjectSelectAction->OnSelectChanged(_pController, pLogicObject->IsEntityLogic());
				}
			}

			// reset selected logic object count
			_pController->GetHelperAction()->SetSelectedLogicObjectCntForDoAction(0);
		}
	}
}

RVector CControlDoAction::GetMoveVector(AXIS_STATE nAxisType, const RVector &vPosition, const rs3::RMatrix& localFrame
										, const RVector &p1, const RVector &q1, const RVector &q2)
{
	RVector vBaseXAxis(localFrame._11, localFrame._12, localFrame._13);
	RVector vBaseYAxis(localFrame._21, localFrame._22, localFrame._23);
	RVector vBaseZAxis(localFrame._31, localFrame._32, localFrame._33);

	RPlane XYPlane, YZPlane, XZPlane;
	XYPlane.SetPlane(vBaseZAxis, vPosition);
	YZPlane.SetPlane(vBaseXAxis, vPosition);
	XZPlane.SetPlane(vBaseYAxis, vPosition);

	// moveaxis_x,y,z �� ��쿡�� ���� �����ϴ� �� ����� ����ص� �Ǵµ�
	// ���࿡ ��������� ������ Ŀ���Ƿ� ���� ī�޶� ���⿡ ���� ������ ����� �������ش�
	RVector vecCamera = Normalize(q1-p1);

	RPlane *pPlane = NULL;
	switch ( nAxisType )
	{
	case MOVEAXIS_X :
		pPlane = fabs(vecCamera.z)>fabs(vecCamera.y) ? &XYPlane : &XZPlane;
 		break;
	case MOVEAXIS_Y :
		pPlane = fabs(vecCamera.z)>fabs(vecCamera.x) ? &XYPlane : &YZPlane;
 		break;
	case MOVEAXIS_Z :
		pPlane = fabs(vecCamera.y)>fabs(vecCamera.x) ? &XZPlane : &YZPlane;
 		break;
	case MOVEAXIS_XY:
		pPlane = &XYPlane;
		break;
	case MOVEAXIS_YZ:
		pPlane = &YZPlane;
		break;
	case MOVEAXIS_XZ:
		pPlane = &XZPlane;
		break;
	}

	RVector v1, v2;
	if ( !pPlane->Intersect(v1, p1, q1))
		return RVector(0, 0, 0);
	if ( !pPlane->Intersect(v2, p1, q2))
		return RVector(0, 0, 0);

	RVector vDirection = ( v2 - v1 );
	float t;

	switch ( nAxisType )
	{
	case MOVEAXIS_X :
		t = vDirection.DotProduct(vBaseXAxis);
		vDirection = vBaseXAxis*t;
		break;
	case MOVEAXIS_Y :
		t = vDirection.DotProduct(vBaseYAxis);
		vDirection = vBaseYAxis*t;
		break;
	case MOVEAXIS_Z :
		t = vDirection.DotProduct(vBaseZAxis);
		vDirection = vBaseZAxis*t;
		break;
	case MOVEAXIS_XY:
		vDirection.z = 0.0f;
		break;
	case MOVEAXIS_YZ:
		vDirection.x = 0.0f;
		break;
	case MOVEAXIS_XZ:
		vDirection.y = 0.0f;
		break;
	}

	return vDirection;
}

void CControlDoAction::ObjectMove(CControlMediator* _pController)
{
	// PreAction
	CDoActionObjectMove* pObjectMoveAction = _pController->GetWorkSpaceMgr()->GetDoActionObjectMove();
	if (pObjectMoveAction && pObjectMoveAction->DoPreAction(_pController))
		return;

	AXIS_STATE eAxis = _pController->GetViewPtr<CCoreToolToolBar>()->GetCurrentAxisToAXIS_STATE();
	if ( eAxis == AXIS_NULL )
		return;

	//Get Object Edit type and do pre-action
	OBJECT_EDIT_TYPE edit_type = GetObjectEditTypeAndPreAction(_pController);

	//object eidtion�� ����
	if(edit_type != OBJECT_EDIT || !_pController->m_mouseInfo.mouseState.bDrag || !_pController->m_mapInfo.m_bLoaded)
		return;

	// �������� ������ ����
	if (_pController->m_mouseInfo.m_movedMouseX == 0 && _pController->m_mouseInfo.m_movedMouseY == 0)
		return;

// 	const SCENEOBJECTMAP* pSelectList = _pController->GetHelperAction()->GetSelectedSceneObject();
	ENTITY_OBJECTLIST* pSelectedEntityObject = _pController->GetHelperAction()->GetSelectedEntityObject();
	LOGIC_OBJECTLIST* pSelectedLogicList = _pController->GetHelperAction()->GetSelectedLogicObject();

	// ���õǾ��ִ°� ������ ����
	if(/*pSelectList->empty() &&*/ pSelectedLogicList->empty() && pSelectedEntityObject->empty() )
		return;
// 	if ((_pController->m_mapInfo.m_bEditSceneObject && pSelectList->empty()) ||
// 		(!_pController->m_mapInfo.m_bEditSceneObject && (pSelectedEntityObject->empty() && pSelectedLogicList->empty()) ) ||
// 		(_pController->m_mouseInfo.m_movedMouseX == 0 && _pController->m_mouseInfo.m_movedMouseY == 0)
// 		)
// 		return;

	RVector p1, q1, p2, q2;
	// ���� Ŀ�� ��ġ
	LONG cx = _pController->m_mouseInfo.m_currentMouseX;
	LONG cy = _pController->m_mouseInfo.m_currentMouseY;

	// ���� ��ġ
	LONG ox = _pController->m_mouseInfo.m_orgMouseX;
	LONG oy = _pController->m_mouseInfo.m_orgMouseY;

	//GetCurrentMouseLine�� �� ��쿡 ����ϴ� �߰��� �ٲ�� �ִ�.
	_pController->GetHelperAction()->GetMouseLine(cx, cy, p2, q2);			// ���� ��ġ
	_pController->GetHelperAction()->GetMouseLine(ox, oy, p1, q1);	// �巡�� ��������

	// ���õ� ������ ��ư ( x, y, z, xy, yz ... ) �� ���� ������ �������� ���Ѵ�
	const RVector& vecActionPos = _pController->GetHelperAction()->GetActionPlanePos();
	const RMatrix& matLocalFrame = _pController->GetHelperAction()->GetSelectedObjectActionFrame();
	RVector vVec = GetMoveVector(eAxis, vecActionPos, matLocalFrame, p1, q1, q2);

	//���� ����κ�
	CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
	CGroupedCommandBuffer* pGroupCmdBuffer = NULL;
	bool bActionEntry = pCommandManager->IsMouseCmdEntryL();

	//�׷� Ŀ�ǵ� ���� ����
	if(bActionEntry)
	{
		pGroupCmdBuffer = new CGroupedCommandBuffer;
		ns_mapObjPosOrg.clear();
	}
	
	// Entity ���
	if ( !pSelectedEntityObject->empty() && pObjectMoveAction)
	{
		_ASSERT(pSelectedEntityObject);
		pObjectMoveAction->OnMove(_pController, pSelectedEntityObject, vVec, pGroupCmdBuffer);
	}

	// logic object
	if(!pSelectedLogicList->empty()) // scenenode/logicobject �Ѵ� �����ϵ��� ����
	{
		int nLogicIndex = 0;
		int nSelectedNum = pSelectedLogicList->size();

		// Logic ���� ����� ���
		for(LOGIC_OBJECTLIST::iterator it_logic = pSelectedLogicList->begin(); it_logic != pSelectedLogicList->end(); ++it_logic, ++nLogicIndex)
		{
			CLogicObject* pLogicObject = *it_logic;
			if (pLogicObject->GetEditable() == false)
				continue;

			if(bActionEntry)
			{
	 			RVector vPosOrg;
	 			if(pLogicObject->GetPosition(vPosOrg) == false)
	 				continue;

				CLogicMoveCmdBuffer* pCmdBuffer = new CLogicMoveCmdBuffer(pLogicObject, vPosOrg);
				pGroupCmdBuffer->AddBuffer(pCmdBuffer);

				ns_mapObjPosOrg[ DWORD_PTR(pLogicObject) ] = DWORD_PTR( pCmdBuffer );
			}

			CLogicMoveCmdBuffer* pCmdBuffer = (CLogicMoveCmdBuffer*)( ns_mapObjPosOrg[ DWORD_PTR(pLogicObject) ]);

			// �����̵�(-_- Shift������ �����ϸ鼭 �̵�) ������
			// �׼� ��Ʈ������ �ٷ� ����Ǿ��� ��쿡�� ����ؼ� ¥���Ѵ�.
			if (pCmdBuffer == NULL)
			{
				if (pGroupCmdBuffer == NULL)
				{
					pGroupCmdBuffer = new CGroupedCommandBuffer;
					ns_mapObjPosOrg.clear();

					// mouse observer ���
					_pController->GetHelperAction()->AddToWBMouseObserver( CControlMediatorHelperAction::WB_MOUSE_L_UP, CCoreToolToolBar::GetID() );
					_pController->GetHelperAction()->AddToWBMouseObserver( CControlMediatorHelperAction::WB_MOUSE_L_UP, CDlgSceneNodeList::GetID() );
				}

				RVector vPosOrg;
				if(pLogicObject->GetPosition(vPosOrg))
				{
					pCmdBuffer = new CLogicMoveCmdBuffer(pLogicObject, vPosOrg);
					pGroupCmdBuffer->AddBuffer(pCmdBuffer);
					ns_mapObjPosOrg[ DWORD_PTR(pLogicObject) ] = DWORD_PTR( pCmdBuffer );
				}

				if ( nLogicIndex == nSelectedNum - 1)
					pCommandManager->RecordCommand(pGroupCmdBuffer);
			}

			RVector vPos = pCmdBuffer->GetPosOrg();

			vPos = vPos + vVec;

			CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
			int nSnapToGridSize = pMainFrame->GetSnapToGridSize();
			if( nSnapToGridSize > 0 )
			{

#define SIGN_OF(x) (x>0 ? 1 : -1)

				vPos.x = (int)((vPos.x + SIGN_OF(vPos.x) * .5f * float(nSnapToGridSize)) / nSnapToGridSize) * nSnapToGridSize;
				vPos.y = (int)((vPos.y + SIGN_OF(vPos.y) * .5f * float(nSnapToGridSize)) / nSnapToGridSize) * nSnapToGridSize;
				vPos.z = (int)((vPos.z + SIGN_OF(vPos.z) * .5f * float(nSnapToGridSize)) / nSnapToGridSize) * nSnapToGridSize;
			}

			char szDragInfo[256] = {0, };
			sprintf(szDragInfo, "Move %.2f , %.2f , %.2f", 
				 vPos.x - pCmdBuffer->GetPosOrg().x, 
				 vPos.y - pCmdBuffer->GetPosOrg().y,
				 vPos.z - pCmdBuffer->GetPosOrg().z );

			UpdateDragText( _pController, szDragInfo );

			pLogicObject->SetPosition(vPos);
		}
	}

	//�׼� �Ϸ�
	if(bActionEntry)
	{
		pCommandManager->RecordCommand(pGroupCmdBuffer);
		pCommandManager->DoneMouseCmdL();

		// mouse observer ���
		_pController->GetHelperAction()->AddToWBMouseObserver( CControlMediatorHelperAction::WB_MOUSE_L_UP, CCoreToolToolBar::GetID() );
		_pController->GetHelperAction()->AddToWBMouseObserver(CControlMediatorHelperAction::WB_MOUSE_L_UP, -1);
	}
}

void CControlDoAction::ObjectRotate(CControlMediator* _pController)
{
	// PreAction
	CDoActionObjectRotate* pObjectRotateAction = _pController->GetWorkSpaceMgr()->GetDoActionObjectRotate();
	if (pObjectRotateAction && pObjectRotateAction->DoPreAction(_pController))
		return;

	AXIS_STATE nAxis = _pController->GetViewPtr<CCoreToolToolBar>()->GetCurrentAxisToAXIS_STATE();
	if ( nAxis == AXIS_NULL )
		return;

	//Get Object Edit type and do pre-action
	OBJECT_EDIT_TYPE edit_type = GetObjectEditTypeAndPreAction(_pController);

	//object eidtion�� ����
	if( edit_type != OBJECT_EDIT || !_pController->m_mapInfo.m_bLoaded ||
		!_pController->m_mouseInfo.mouseState.bDrag ||
		(_pController->m_mouseInfo.m_movedMouseX == 0 && _pController->m_mouseInfo.m_movedMouseY == 0)
		)
		return;

	//Command Manager
	CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
	CGroupedCommandBuffer* pGroupCmdBuffer = NULL;

	// ���� �׼�.
	bool bActionEntry = pCommandManager->IsMouseCmdEntryL();

// 	const SCENEOBJECTMAP *pSelectList = _pController->GetHelperAction()->GetSelectedSceneObject();
	ENTITY_OBJECTLIST* pSelectedEntityObject = _pController->GetHelperAction()->GetSelectedEntityObject();
	LOGIC_OBJECTLIST* pSelectedLogicObject = _pController->GetHelperAction()->GetSelectedLogicObject();

	if(bActionEntry && ( /*pSelectList->size() ||*/ pSelectedEntityObject->size() || pSelectedLogicObject->size() ) )
	{
		pGroupCmdBuffer = new CGroupedCommandBuffer;
		ns_mapObjRotOrg.clear();
	}

	// �����̼� ��Ʈ���� �����
	if (MakeRotationMatrixAndInfoText(ns_matRotate, _pController, nAxis ) == false)
	{
		// delete NULL�� ��ȿ�� �ڵ��. pGroupCmdBuffer�� NULL���� �ƴ����� �������.
		delete pGroupCmdBuffer;
		return;
	}

	if ( pSelectedEntityObject->size() )
	{
		if (pObjectRotateAction)
			pObjectRotateAction->OnRotate(_pController, pSelectedEntityObject, ns_matRotate, nAxis, pGroupCmdBuffer);
	}

	// Logic // scene/entity/logic ��� ����
//		else
	if( pSelectedLogicObject->size() )
	{
		int nLogicIndex = 0;
		int nSelectedNum = pSelectedLogicObject->size();

		// ȸ���� �߽� - ������ ��ü�� �߽��� ���ϱ� (�������� ���� ��� ���� �߽����� ȸ�� �̵�)
		RVector* pPivot = _pController->GetHelperAction()->GetSelectNodeCenter();
		_ASSERT( pPivot );

		// scene ���� ����϶��� �������� ������ �߽����� , logic �� ��쿡�� ������ �߽����� ȸ���Ѵ�

		RMatrix matTransform, matTranslation, matInverseTranslation;
		for(LOGIC_OBJECTLIST::iterator itr = pSelectedLogicObject->begin(); itr != pSelectedLogicObject->end(); ++itr, ++nLogicIndex)
		{
			CLogicObject* pLogicObject = *itr;
			if (pLogicObject->GetEditable() == false)
				continue;

			RVector vPos, vDir;
			if( pLogicObject->GetPosition(vPos) == false ||
				pLogicObject->GetDirection(vDir) == false )
				continue;

			RVector vUp(RVector::AXISZ);
			if ( pLogicObject->GetUp(vUp) )
			{
				if (MakeRotationMatrixAndInfoText(ns_matRotate, _pController, nAxis) == false)
					continue;
			}

			if(bActionEntry)
			{
				CLogicRotateCmdBuffer* pCmdBuffer = new CLogicRotateCmdBuffer(pLogicObject, vDir, vUp, vPos, *pPivot);
				pGroupCmdBuffer->AddBuffer(pCmdBuffer);

				ns_mapObjRotOrg[ DWORD_PTR(pLogicObject) ] = DWORD_PTR( pCmdBuffer );
			}

			// ���������� ���Ѹ�ŭ�� ȸ���Ѵ�.
			CLogicRotateCmdBuffer* pCmdBuffer = (CLogicRotateCmdBuffer*)( ns_mapObjRotOrg[ DWORD_PTR(pLogicObject) ]);
			if (pCmdBuffer == NULL)
			{
				if (pGroupCmdBuffer == NULL)
				{
					pGroupCmdBuffer = new CGroupedCommandBuffer;
					ns_mapObjRotOrg.clear();

					// mouse observer ���
					_pController->GetHelperAction()->AddToWBMouseObserver( CControlMediatorHelperAction::WB_MOUSE_L_UP, CCoreToolToolBar::GetID() );
					_pController->GetHelperAction()->AddToWBMouseObserver( CControlMediatorHelperAction::WB_MOUSE_L_UP, CDlgSceneNodeList::GetID() );
				}

				pCmdBuffer = new CLogicRotateCmdBuffer(pLogicObject, vDir, vUp, vPos, *pPivot);
				pGroupCmdBuffer->AddBuffer(pCmdBuffer);

				ns_mapObjRotOrg[ DWORD_PTR(pLogicObject) ] = DWORD_PTR( pCmdBuffer );

				if ( nLogicIndex == nSelectedNum - 1)
					pCommandManager->RecordCommand(pGroupCmdBuffer);
			}

			
			// ���������� ���Ѹ�ŭ�� ȸ���Ѵ�.
			matTransform.SetLocalMatrix( pCmdBuffer->GetRotOrgPos(), pCmdBuffer->GetRotOrgDir(), pCmdBuffer->GetRotOrgUp() );

			matTranslation.MakeIdentity();
			matInverseTranslation.MakeIdentity();

			RVector vecPivot = pCmdBuffer->GetRotPivot();
			matTranslation.SetTranslation(- vecPivot);
			matInverseTranslation.SetTranslation( vecPivot );

			matTransform = matTransform * matTranslation * ns_matRotate * matInverseTranslation;

			RVector vecNewDir, vecNewUp, vecNewPos;
			vecNewDir = RVector(matTransform._21,matTransform._22,matTransform._23);
			vecNewUp = RVector(matTransform._31,matTransform._32,matTransform._33);
			vecNewPos = RVector(matTransform._41,matTransform._42,matTransform._43);

			pLogicObject->SetDirectionUp(vecNewDir,vecNewUp);

			// scene ���� ����϶��� pivot �� �߽����� ȸ��
			pLogicObject->SetPosition( vecNewPos );
		}
	}

	if(bActionEntry)
	{
		// bActionEntry�ε� ��ݻ��¶� WB���°� ������ �ƴ϶� �����̰� ���õ� ��ü�� ���°���
		// pGroupCmdBuffer�� NULL ���´�.
		if (pGroupCmdBuffer)
		{
			if(pGroupCmdBuffer->IsEmpty())
				delete pGroupCmdBuffer;
			else
				pCommandManager->RecordCommand(pGroupCmdBuffer);
		}

		pCommandManager->DoneMouseCmdL();

		// mouse observer ���
		_pController->GetHelperAction()->AddToWBMouseObserver( CControlMediatorHelperAction::WB_MOUSE_L_UP, CCoreToolToolBar::GetID() );
		if (_pController->m_mapInfo.m_bEditSceneObject)
			_pController->GetHelperAction()->AddToWBMouseObserver( CControlMediatorHelperAction::WB_MOUSE_L_UP, CDlgSceneNodeList::GetID() );
	}
}

void CControlDoAction::ObjectScale(CControlMediator* _pController)
{
	CDoActionObjectScale* pObjectScaleAction = _pController->GetWorkSpaceMgr()->GetDoActionObjectScale();
	if (pObjectScaleAction && pObjectScaleAction->DoPreAction(_pController))
		return;

	//Get Object Edit type and do pre-action
	OBJECT_EDIT_TYPE edit_type = GetObjectEditTypeAndPreAction(_pController);

	//object eidtion�� ����
	if(edit_type != OBJECT_EDIT || !_pController->m_mouseInfo.mouseState.bDrag || !_pController->m_mapInfo.m_bLoaded)
		return;

	CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
	bool bActionEntry = pCommandManager->IsMouseCmdEntryL();

	{
		//scale adjust factor
		float f;
		static int nCumulFactor = 0;
		nCumulFactor += abs(_pController->m_mouseInfo.m_movedMouseY);
		if( nCumulFactor > 100 )
			f = (-_pController->m_mouseInfo.m_movedMouseY) * 0.03f;
		else
			f = (-_pController->m_mouseInfo.m_movedMouseY) * 0.01f;
		if(f == 0) return;

		ENTITY_OBJECTLIST* pSelectedEntityObject = _pController->GetHelperAction()->GetSelectedEntityObject();
		if(pSelectedEntityObject->size())
		{
			CGroupedCommandBuffer* pGroupCmdBuffer = NULL;
			if(bActionEntry)
				pGroupCmdBuffer = new CGroupedCommandBuffer;

			if (pObjectScaleAction)
				pObjectScaleAction->OnScale(_pController, pSelectedEntityObject, f, pGroupCmdBuffer);

			if(bActionEntry)
			{
				if(pGroupCmdBuffer->IsEmpty())
					delete pGroupCmdBuffer;
				else
					pCommandManager->RecordCommand(pGroupCmdBuffer);

				pCommandManager->DoneMouseCmdL();
			}
		}

		LOGIC_OBJECTLIST* pSelectedLogicObject = _pController->GetHelperAction()->GetSelectedLogicObject();
		if(pSelectedLogicObject->size())
		{
			CGroupedCommandBuffer* pGroupCmdBuffer = NULL;
			if(bActionEntry)
				pGroupCmdBuffer = new CGroupedCommandBuffer;

			size_t selected_size = pSelectedLogicObject->size();
			for(LOGIC_OBJECTLIST::iterator itr = pSelectedLogicObject->begin(); itr != pSelectedLogicObject->end(); ++itr)
			{
				CLogicObject* pLogicObject = *itr;
				if ( pLogicObject->GetEditable() == false)
					continue;

				float fRadius;
				if( pLogicObject->GetScale(fRadius) == false ) continue;

				float fNewRadius = fRadius * (1.0f + f);
				if( fNewRadius < 0.f ) fNewRadius = 0.f;

				pLogicObject->SetScale( fNewRadius );

				if(bActionEntry)
				{
					CLogicScaleCmdBuffer* pCmdBuffer = new CLogicScaleCmdBuffer(pLogicObject, fRadius);
					pGroupCmdBuffer->AddBuffer(pCmdBuffer);
				}
			}

			if(bActionEntry)
			{
				if(pGroupCmdBuffer->IsEmpty())
					delete pGroupCmdBuffer;
				else
					pCommandManager->RecordCommand(pGroupCmdBuffer);

				pCommandManager->DoneMouseCmdL();
			}
		}

	}

	if(bActionEntry)
	{
		// observer ���
		_pController->GetHelperAction()->AddToWBMouseObserver(CControlMediatorHelperAction::WB_MOUSE_L_UP, -1);

	}
}

#pragma warning( pop )
