// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "EffectTool.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CRView*			g_rView;
extern CControlView*	g_ControlView;

BEGIN_MESSAGE_MAP(CSplitterX, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitterX)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSplitterX::CSplitterX() {}
CSplitterX::~CSplitterX(){}

LRESULT CSplitterX::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	return HTNOWHERE;
	//return CSplitterWnd::OnNcHitTest(point);

}

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE( WM_MODIFY_MSG, OnModifyParticle)

	ON_COMMAND(ID_VIEW_START, OnViewStart)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_PAUSE, OnViewPause)
	ON_COMMAND(ID_VIEW_BGCOLOR, OnViewBgcolor)

	ON_COMMAND(ID_TB_WIREFRAME, OnWireFrame)
	ON_COMMAND(ID_TB_AXIS, OnViewAxis)
	ON_COMMAND(ID_TB_GRID, OnViewGrid)
	ON_COMMAND(ID_TB_EMITTER, OnViewEmitter)
	ON_COMMAND(ID_VIEW_PATH, OnViewPath)

	ON_UPDATE_COMMAND_UI(ID_TB_WIREFRAME, OnWireFrameUpdateUI)
	ON_UPDATE_COMMAND_UI(ID_TB_AXIS, OnViewAxisUpdateUI)
	ON_UPDATE_COMMAND_UI(ID_TB_GRID, OnViewGridUpdateUI)
	ON_UPDATE_COMMAND_UI(ID_TB_EMITTER, OnViewEmitterUpdateUI)

	ON_COMMAND(ID_FILE_NEW, OnNew)
	ON_COMMAND(ID_FILE_OPEN, OnEffectFileOpen)
	ON_COMMAND(ID_FILE_SAVEQ, OnEffectFileSave)
	ON_COMMAND(ID_FILE_SAVEAS, OnEffectFileSaveAs)

	ON_COMMAND(ID_FILE_OPEN_MESH, OnMeshEffectFileOpen)
	ON_COMMAND(ID_FILE_OPEN_ELU, OnCharFileOpen)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

ULONG_PTR m_gdiplusToken;

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	//_CrtSetBreakAlloc(679);
}

CMainFrame::~CMainFrame()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		
//	ShowWindow(SW_SHOWMAXIMIZED);
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// 
//	cs.style ^= WS_MAXIMIZEBOX;
	
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.cx = 1024;
	cs.cy = 900;

	return TRUE;
}


// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame �޽��� ó����
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{

	HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	int width = GetDeviceCaps(hDC, HORZRES);

	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE("���� �����츦 �����ϴµ� ���� �߽��ϴ�.\n");
		return FALSE;
	}
	//���� Size 800, 0
	if (!m_wndSplitter.CreateView(0,0, RUNTIME_CLASS(CRView), CSize(620,0), pContext))
	{
		//�����ϴ� �����ϸ� ����
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	//������  size 0,0 -> ������ �������� ������ �κ��� �����Ѵ�.
	if (!m_wndSplitter.CreateView(0,1, RUNTIME_CLASS(CControlView), CSize(0,0), pContext))
	{
		// �����ϴ� �����ϸ� ����..
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	//	�Ʒ��� ���� ����	
	return TRUE;
}

LRESULT CMainFrame::OnModifyParticle(WPARAM wParam, LPARAM lParam)
{
	g_rView->ModifyParticle(wParam, lParam);
	return 0;
}

void CMainFrame::OnViewStart()
{
	g_rView->OnViewStart();
}

void CMainFrame::OnViewStop()
{
	g_rView->OnViewStop();
}

void CMainFrame::OnViewPause()
{
	g_rView->OnViewPause();
}

void CMainFrame::OnViewBgcolor()
{
	g_rView->OnViewBgcolor();
}

void CMainFrame::OnWireFrame()
{
	g_rView->OnWireFrame();
}

void CMainFrame::OnWireFrameUpdateUI(CCmdUI *pCmdUI)
{
	g_rView->OnWireFrameUpdateUI(pCmdUI);
}
void CMainFrame::OnViewAxis()
{
	g_rView->OnViewAxis();
}

void CMainFrame::OnViewAxisUpdateUI(CCmdUI *pCmdUI)
{
	g_rView->OnViewAxisUpdateUI(pCmdUI);
}

void CMainFrame::OnViewGrid()
{
	g_rView->OnViewGrid();
}

void CMainFrame::OnViewGridUpdateUI(CCmdUI *pCmdUI)
{
	g_rView->OnViewGridUpdateUI(pCmdUI);
}

void CMainFrame::OnViewEmitter()
{
	g_rView->OnViewEmitter();
}

void CMainFrame::OnViewPath()
{
	g_rView->OnViewPath();
}

void CMainFrame::OnViewEmitterUpdateUI(CCmdUI *pCmdUI)
{
	g_rView->OnViewEmitterUpdateUI(pCmdUI);
}

void CMainFrame::OnNew()
{
	g_rView->OnNew();
}

void CMainFrame::OnEffectFileOpen()
{
	g_rView->OnEffectFileOpen();
}

void CMainFrame::OnEffectFileSave()
{
	g_rView->OnEffectFileSave();
}

void CMainFrame::OnEffectFileSaveAs()
{
	g_rView->OnEffectFileSaveAs();
}

void CMainFrame::OnMeshEffectFileOpen()
{
	g_rView->OnMeshEffectFileOpen();
}

void CMainFrame::OnCharFileOpen()
{
	g_rView->OnCharFileOpen();
}
