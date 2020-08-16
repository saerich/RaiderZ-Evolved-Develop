// SDIdocView.cpp : CCoreToolRS3View Ŭ������ ����
//

#include "stdafx.h"
//#include "CoreToolRS3.h"

#include "CoreToolRS3_Doc.h"
#include "CoreToolRS3_View.h"
#include "MainFrm.h"

#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperResource.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_StatusBarHandler.h"
#include "C_MintManager.h"
#include "C_KeyEventHandler.h"

#include "WS_WorkSpaceManager.h"
#include "M_ToolCmdMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCoreToolRS3View

IMPLEMENT_DYNCREATE(CCoreToolRS3View, CView)

BEGIN_MESSAGE_MAP(CCoreToolRS3View, CView)
	//ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()

	ON_WM_SIZE()
	//ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

// CCoreToolRS3View ����/�Ҹ�

CCoreToolRS3View::CCoreToolRS3View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_pControlMediator_ = NULL;

	m_bTimer = false;
	m_dwTimerElpasedTime = 0;
}

CCoreToolRS3View::~CCoreToolRS3View()
{
}

BOOL CCoreToolRS3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CCoreToolRS3View �׸���

void CCoreToolRS3View::OnDraw(CDC* /*pDC*/)
{
	CCoreToolRS3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}

// CCoreToolRS3View �μ�

//BOOL CCoreToolRS3View::OnPreparePrinting(CPrintInfo* pInfo)
//{
//	// �⺻���� �غ�
//	return DoPreparePrinting(pInfo);
//}
//
//void CCoreToolRS3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
//}
//
//void CCoreToolRS3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
//}


// CCoreToolRS3View ����

#ifdef _DEBUG
void CCoreToolRS3View::AssertValid() const
{
	CView::AssertValid();
}

void CCoreToolRS3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCoreToolRS3Doc* CCoreToolRS3View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCoreToolRS3Doc)));
	return (CCoreToolRS3Doc*)m_pDocument;
}
#endif //_DEBUG

//void CCoreToolRS3View::OnTimer(UINT nIDEvent)
//{
//	m_pControlMediator_->WB_OnTimeAction();
//	m_pControlMediator_->RunRS3();
//
//	CWnd::OnTimer(nIDEvent);
//}

void CCoreToolRS3View::SetUserTimer(DWORD dwElpasedTime)
{
	m_bTimer = true;
	m_dwTimerElpasedTime = dwElpasedTime;
}

void CCoreToolRS3View::KillUserTimer()
{
	m_bTimer = false;
}

void CCoreToolRS3View::OnUserTimer()
{
	static DWORD dwTick = 0;
	DWORD dwCurrentTick = GetTickCount();

	// Tick�� üũ�ؼ� �����ϴ� �κ�, ���� �Լ��� �и��ϰ� �;����� ������ �κ��� ã�� ������.
	// �� �Լ��� CCoreToolRS3App::OnIdle���� ȣ��ȴ�.
	if ( m_bTimer && dwCurrentTick - dwTick > m_dwTimerElpasedTime )
	{
		dwTick = dwCurrentTick;

		m_pControlMediator_->WB_OnTimeAction();
		m_pControlMediator_->RunRS3();
	}
}

int CCoreToolRS3View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	DragAcceptFiles(TRUE);

	RECT rt;
	GetClientRect(&rt);
	m_wbWidth_ = rt.right;
	m_wbHeight_ = rt.bottom;

	return 0;
}

//���콺 �׼�
void CCoreToolRS3View::OnMouseMove(UINT nFlags, CPoint point)
{
	_ASSERT(NULL != m_pControlMediator_);

	if( !CMintManager::GetReference().IsActiveQuadPopup() )
	{
		//���� ä���
		m_eventedMouseBuffer_.nFlags = nFlags;
		m_eventedMouseBuffer_.nEvent = MOUSE_ACT_EVENT::MOVE;
		m_eventedMouseBuffer_.x = point.x;
		m_eventedMouseBuffer_.y = point.y;
		m_eventedMouseBuffer_.zDelta = 0;

		//���콺�� ������Ʈ ����
		WB_DecideMouseState_();

		//�����϶� ���� ���콺�� ������ �浹�� ���
		if( !m_pControlMediator_->IsWBNavigation() )
		{
			if (m_pControlMediator_->GetWorkSpaceMgr()->IsAlwaysPickingWhenMouseMoved()
				&& !GetAsyncKeyState(VK_MENU) // ȸ���߿��� ��ŷ ���ϱ�
				&& !m_pControlMediator_->GetToolCmdMgr()->IsMouseCmdEntryM() //�д��߿��� ��ŷ ���ϱ�
				)
			{
				if(m_pControlMediator_->GetHelperAction()->SetMouseLineAndPickingPosition())
				{
					const rs3::RVector* vMousePos = m_pControlMediator_->GetHelperAction()->GetCurrentPickingPosition();
					if( vMousePos != NULL)
						CStatusBarHandler::GetInstance()->ShowCurrentPickingAndCamera(vMousePos->x, vMousePos->y, vMousePos->z, true);
				}
			}
			else
			{
				CStatusBarHandler::GetInstance()->ShowCurrentPickingAndCamera(0, 0, 0, false);
			}
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CCoreToolRS3View::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( CMintManager::GetReference().IsActiveQuadPopup() )
	{
		MEvent e;
		e.nMessage = MWM_LBUTTONDOWN;
		e.Pos.x = point.x;
		e.Pos.y = point.y;
		CMintManager::GetReference().ListenEvent(e);
	}
	else
	{
		SetFocus();
		m_eventedMouseBuffer_.nFlags = nFlags;
		m_eventedMouseBuffer_.nEvent = MOUSE_ACT_EVENT::L_BN | MOUSE_ACT_EVENT::BN_DOWN;
		m_eventedMouseBuffer_.x = point.x;
		m_eventedMouseBuffer_.y = point.y;
		m_eventedMouseBuffer_.zDelta = 0;

		//Set Mouse Command Entry Flag and Picking Position for Action
		m_pControlMediator_->GetToolCmdMgr()->InitMouseCmdFlagL();
		m_pControlMediator_->GetHelperAction()->OnLButtonDown();

		//Decide Mouse State And Callback Action
		WB_DecideMouseState_();

		// notify wb mouse action to observer
		m_pControlMediator_->GetHelperAction()->NotifyWBMouseAction( CControlMediatorHelperAction::WB_MOUSE_L_DOWN );
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CCoreToolRS3View::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_pControlMediator_->m_mouseInfo.m_bCaptured )
	{
		m_pControlMediator_->m_mouseInfo.m_bCaptured = false;
		ReleaseCapture();
	}

	if( CMintManager::GetReference().IsActiveQuadPopup() )
	{
		MEvent e;
		e.nMessage = MWM_LBUTTONUP;
		e.Pos.x = point.x;
		e.Pos.y = point.y;
		CMintManager::GetReference().ListenEvent(e);
	}
	else
	{
		m_eventedMouseBuffer_.nFlags = nFlags;
		m_eventedMouseBuffer_.nEvent = MOUSE_ACT_EVENT::L_BN | MOUSE_ACT_EVENT::BN_UP;
		m_eventedMouseBuffer_.x = point.x;
		m_eventedMouseBuffer_.y = point.y;
		m_eventedMouseBuffer_.zDelta = 0;

		// 
		m_pControlMediator_->GetHelperAction()->OnLButtonUp();

		WB_DecideMouseState_();

		//Reset Mouse Command Entry Flag
		m_pControlMediator_->GetToolCmdMgr()->ResetMouseCmdFlagL();

		// ���콺�� �����϶� ���� ������Ʈ �ϴ����� ���콺��ư�� ������ �Ѵ�. 
		//m_pControlMediator_->T_GetView< CEditObjectList* >( VIEW_ID::PANEL_OBJECT_LIST )->ObserveSelection( NULL, true, false );
		// notify wb mouse action to observer
		m_pControlMediator_->GetHelperAction()->NotifyWBMouseAction( CControlMediatorHelperAction::WB_MOUSE_L_UP );
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CCoreToolRS3View::OnMButtonDown(UINT nFlags, CPoint point)
{
	if( !CMintManager::GetReference().IsActiveQuadPopup() )
	{
		SetFocus();
		m_eventedMouseBuffer_.nFlags = nFlags;
		m_eventedMouseBuffer_.nEvent = MOUSE_ACT_EVENT::M_BN | MOUSE_ACT_EVENT::BN_DOWN;
		m_eventedMouseBuffer_.x = point.x;
		m_eventedMouseBuffer_.y = point.y;
		m_eventedMouseBuffer_.zDelta = 0;

		//Set Mouse Command Entry Flag
		m_pControlMediator_->GetToolCmdMgr()->InitMouseCmdFlagM();

		// by pok. �д��߿��� ��ŷ ���ϴ°ɷ� �����ϸ鼭 �Ʒ� �ڵ� �ּ�ó��
		// m_pControlMediator_->GetHelperAction()->SetActionPlaneAndContactPos();

		//Decide Mouse State And Callback Action
		WB_DecideMouseState_();
	}
	CWnd::OnMButtonDown(nFlags, point);
}

void CCoreToolRS3View::OnMButtonUp(UINT nFlags, CPoint point)
{
	if( m_pControlMediator_->m_mouseInfo.m_bCaptured )
	{
		m_pControlMediator_->m_mouseInfo.m_bCaptured = false;
		ReleaseCapture();
	}

	if( !CMintManager::GetReference().IsActiveQuadPopup() )
	{
		m_eventedMouseBuffer_.nFlags = nFlags;
		m_eventedMouseBuffer_.nEvent = MOUSE_ACT_EVENT::M_BN | MOUSE_ACT_EVENT::BN_UP;
		m_eventedMouseBuffer_.x = point.x;
		m_eventedMouseBuffer_.y = point.y;
		m_eventedMouseBuffer_.zDelta = 0;
		WB_DecideMouseState_();

		//Reset Mouse Command Entry Flag
		m_pControlMediator_->GetToolCmdMgr()->ResetMouseCmdFlagM();
	}

	CWnd::OnMButtonUp(nFlags, point);
}

BOOL CCoreToolRS3View::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	if( !CMintManager::GetReference().IsActiveQuadPopup() )
	{
		m_eventedMouseBuffer_.nFlags = nFlags;
		m_eventedMouseBuffer_.nEvent = MOUSE_ACT_EVENT::M_WHEEL;
		m_eventedMouseBuffer_.x = point.x;
		m_eventedMouseBuffer_.y = point.y;
		m_eventedMouseBuffer_.zDelta = zDelta;
		WB_DecideMouseState_();
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, point);
}

void CCoreToolRS3View::OnRButtonDown(UINT nFlags, CPoint point)
{
	if( CMintManager::GetReference().IsActiveQuadPopup() )
	{
		MEvent e;
		e.nMessage = MWM_RBUTTONDOWN;
		e.Pos.x = point.x;
		e.Pos.y = point.y;
		CMintManager::GetReference().ListenEvent(e);
	}
	else
	{
		SetFocus();
		m_eventedMouseBuffer_.nFlags = nFlags;
		m_eventedMouseBuffer_.nEvent = MOUSE_ACT_EVENT::R_BN | MOUSE_ACT_EVENT::BN_DOWN;
		m_eventedMouseBuffer_.x = point.x;
		m_eventedMouseBuffer_.y = point.y;
		m_eventedMouseBuffer_.zDelta = 0;
		WB_DecideMouseState_();

		if(m_pControlMediator_->m_mouseInfo.m_bPopAble)
			CMintManager::GetReference().ActivateQuadPopup(true, &point);
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CCoreToolRS3View::OnRButtonUp(UINT nFlags, CPoint point)
{
	if( m_pControlMediator_->m_mouseInfo.m_bCaptured )
	{
		m_pControlMediator_->m_mouseInfo.m_bCaptured = false;
		ReleaseCapture();
	}

	if( CMintManager::GetReference().IsActiveQuadPopup() )
	{
		MEvent e;
		e.nMessage = MWM_RBUTTONUP;
		e.Pos.x = point.x;
		e.Pos.y = point.y;
		CMintManager::GetReference().ListenEvent(e);
	}
	else
	{
		m_eventedMouseBuffer_.nFlags = nFlags;
		m_eventedMouseBuffer_.nEvent = MOUSE_ACT_EVENT::R_BN | MOUSE_ACT_EVENT::BN_UP;
		m_eventedMouseBuffer_.x = point.x;
		m_eventedMouseBuffer_.y = point.y;
		m_eventedMouseBuffer_.zDelta = 0;
		WB_DecideMouseState_();
	}
	CWnd::OnRButtonUp(nFlags, point);
}

void CCoreToolRS3View::WB_DecideMouseState_()
{
	/**
		m_eventedMouseBuffer_ : ���� �Ͼ �̺�Ʈ
		m_previousMouseBuffer_: �ٷ� ������ �Ͼ �̺�Ʈ

		->m_mouseInfo : ���޵�(����) �̺�Ʈ�� ��
	*/
	//evented �Ǿ���
	m_pControlMediator_->m_mouseInfo.mouseState.bEvented = true;
	m_pControlMediator_->m_mouseInfo.m_currentMouseX = m_eventedMouseBuffer_.x;
	m_pControlMediator_->m_mouseInfo.m_currentMouseY = m_eventedMouseBuffer_.y;
	m_pControlMediator_->m_mouseInfo.m_nWheelDelta = m_eventedMouseBuffer_.zDelta;
	if ( m_pControlMediator_->m_mouseInfo.m_nWheelDelta != 0 )
	{
		m_pControlMediator_->m_mouseInfo.mouseState.bMButton = false;
		m_pControlMediator_->m_mouseInfo.mouseState.bMDrag = false;
	}

	// ��ư ������, �� ���� -
	UINT nMouseEvent = m_eventedMouseBuffer_.nEvent;

	if ( nMouseEvent & MOUSE_ACT_EVENT::L_BN )
	{
		if ( nMouseEvent & MOUSE_ACT_EVENT::BN_DOWN )
		{
			m_pControlMediator_->m_mouseInfo.mouseState.bLButton = true;

			m_pControlMediator_->m_mouseInfo.m_orgMouseX = m_pControlMediator_->m_mouseInfo.m_currentMouseX;
			m_pControlMediator_->m_mouseInfo.m_orgMouseY = m_pControlMediator_->m_mouseInfo.m_currentMouseY;

			// ������ ��ǥ�� 0
			m_pControlMediator_->m_mouseInfo.m_movedMouseX = 0;
			m_pControlMediator_->m_mouseInfo.m_movedMouseY = 0;

			CKeyEventHandler::GetReference().CancelMenuKey();
		}
		else
			m_pControlMediator_->m_mouseInfo.mouseState.bLButton = false;
	}

	if ( nMouseEvent & MOUSE_ACT_EVENT::M_BN )
	{
		if ( nMouseEvent & MOUSE_ACT_EVENT::BN_DOWN )
		{
			m_pControlMediator_->m_mouseInfo.mouseState.bMButton = true;
			m_pControlMediator_->m_mouseInfo.m_orgMouseX = m_pControlMediator_->m_mouseInfo.m_currentMouseX;
			m_pControlMediator_->m_mouseInfo.m_orgMouseY = m_pControlMediator_->m_mouseInfo.m_currentMouseY;

			// ������ ��ǥ�� 0
			m_pControlMediator_->m_mouseInfo.m_movedMouseX = 0;
			m_pControlMediator_->m_mouseInfo.m_movedMouseY = 0;

			CKeyEventHandler::GetReference().CancelMenuKey();
		}
		else
			m_pControlMediator_->m_mouseInfo.mouseState.bMButton = false;
	}

	if ( nMouseEvent & MOUSE_ACT_EVENT::R_BN )
	{
		if ( nMouseEvent & MOUSE_ACT_EVENT::BN_DOWN )
		{
			m_pControlMediator_->m_mouseInfo.mouseState.bRButton = true;
			CKeyEventHandler::GetReference().CancelMenuKey();
		}
		else
			m_pControlMediator_->m_mouseInfo.mouseState.bRButton = false;
	}

	// Drag
	m_pControlMediator_->m_mouseInfo.mouseState.bDrag = false;
	m_pControlMediator_->m_mouseInfo.mouseState.bMDrag = false;

	if ( nMouseEvent & MOUSE_ACT_EVENT::MOVE )
	{
		m_pControlMediator_->m_mouseInfo.m_movedMouseX = m_eventedMouseBuffer_.x - m_previousMouseBuffer_.x;
		m_pControlMediator_->m_mouseInfo.m_movedMouseY = m_eventedMouseBuffer_.y - m_previousMouseBuffer_.y;

		if ( m_pControlMediator_->m_mouseInfo.mouseState.bLButton )
			m_pControlMediator_->m_mouseInfo.mouseState.bDrag = true;

		if ( m_pControlMediator_->m_mouseInfo.mouseState.bMButton )
			m_pControlMediator_->m_mouseInfo.mouseState.bMDrag = true;
	}

	m_previousMouseBuffer_ = m_eventedMouseBuffer_;

	m_pControlMediator_->WB_CallBackAction();
}

void CCoreToolRS3View::OnSize(UINT nType, int cx, int cy){
	CWnd::OnSize(nType, cx, cy);

	RECT rt;
	GetClientRect(&rt);
	m_wbWidth_ = rt.right;
	m_wbHeight_ = rt.bottom;

	bool bResoredFromGameRun = (nType == SIZE_RESTORED && cx == 0 && cy == 0);
	if(m_pControlMediator_ != NULL && bResoredFromGameRun == false )
		m_pControlMediator_->ResizeWBDevice(rt.right, rt.bottom);
}

BOOL CCoreToolRS3View::OnEraseBkgnd(CDC* pDC)
{
	//return CView::OnEraseBkgnd(pDC);

	if(m_pControlMediator_ != NULL && m_pControlMediator_->IsWBDeviceInited())
		m_pControlMediator_->RunRS3();

	return FALSE;
}

void CCoreToolRS3View::OnDropFiles(HDROP hDropInfo)
{
	char filename[256] = {0,};
	int cnt = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(cnt > 1)
		::MessageBox(NULL,"�ѹ��� �ϳ���~","info",MB_OK);

	for(int i=0;i<1;i++)
		DragQueryFile(hDropInfo,i,filename,sizeof(filename));

	DragFinish(hDropInfo);

	_ASSERT( GetDocument() );
	//�𷡽ð� ����

	HCURSOR hOld = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	//����
	MAP_EDIT_TYPE type = GetDocument()->OpenMapFile(filename);

	//�𷡽ð� ��
	::SetCursor(hOld);

	if (MAP_EDIT_TYPE_EXIT_SUCCESSED !=  type && MAP_EDIT_TYPE_EXIT_FAILD != type)
		CView::OnDropFiles(hDropInfo);
}
