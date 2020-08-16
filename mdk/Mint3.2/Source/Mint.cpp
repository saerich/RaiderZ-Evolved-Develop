#include "stdafx.h"
#include "MWLua.h"
#include "MScriptManager.h"
#include "MFileSystem.h"
#include "Mint.h"
#include "MWidget.h"
#include "MWidgetManager.h"
#include "MColorTable.h"
#include "TChar.h"
#include "MLabel.h"
#include "MLabelEx.h"
#include "MFrame.h"
#include "MEdit.h"
#include "MEditCtrl.h"
#include "MListBox.h"
#include "MListCtrl.h"
#include "MComboBox.h"
#include "MGroup.h"
#include "MSlider.h"
#include "MSpinCtrl.h"
#include "MResource.h"
#include "MPicture.h"
#include "MAnimation.h"
#include "MTabCtrl.h"
#include "MPanel.h"
#include "MPaint.h"
#include "MSheet.h"
#include "MLinePrinter.h"
#include "MPopupMenu.h"
#include "MintLoader.h"
#include "MMainFrame.h"
#include "MButtonGroup.h"
#include "MLayer.h"
#include "MViewCtrl.h"
#include "MTextFrame.h"
#include "MTextView.h"
#include "MTreeCtrl.h"
#include "MCategoryCtrl.h"
#include "MProgressCtrl.h"
#include "MSelector.h"
#include "MSlideShow.h"
#include "MLookBase.h"
#include "MLookManager.h"
#include "MintProfiler.h"

namespace mint3 {



////////////////////////////////////////////////////
Mint* Mint::m_pInstance = NULL;

void Mint::DrawCandidateList(MDrawContext* pDC, MPOINT& p)
{
	if(GetCandidateCount()>0){
		MCOLOR c = pDC->GetColor();

		MRECT r(p.x, p.y, GetCandidateListWidth(), GetCandidateListHeight());

//		pDC->SetColor(MCOLOR(0x30000000));		// �׸���
//		pDC->FillRectangle( r.x+5, r.y+5, r.w, r.h);
		
		pDC->SetColor(MCOLOR(0xFF050505));		// ������ ����
		pDC->FillRectangle(r);

		pDC->SetColor(MCOLOR(0xFF505050));		// ������ ��ο� �κ�
		pDC->Rectangle( r.x+1, r.y+1, r.w,   r.h);

		pDC->SetColor(MCOLOR(0xFFB0B0B0));		// ������ ���� �κ�
		pDC->Rectangle(r);

		MFont* pFont = pDC->GetFont();
		pDC->SetFont( MFontManager::Get( "Default"));		// ������ ��Ʈ�� ����Ʈ�� �����Ѵ�.

		int nStart = GetCandidatePageStart();

//		if (pDC->BeginFont())
		{
			char temp[MIMECOMPOSITIONSTRING_LENGTH+4];
			for(int i=nStart; i<(int)min(GetCandidateCount(), nStart+GetCandidatePageSize()); i++)
			{
				const char* szCandidate = GetCandidate(i);
				if(i==GetCandidateSelection())	pDC->SetColor( MColorTable::TEXT[ MCOLOR_FOCUS]);
				else							pDC->SetColor( MColorTable::TEXT[ MCOLOR_NORMAL]);
				int nIndexInPage = i-nStart;
				sprintf(temp, "%d: %s", nIndexInPage+1, szCandidate);
				pDC->Text(p.x+4, p.y + nIndexInPage*pDC->GetFont()->GetHeight() + 4, temp);
			}

			// ���� ���� �ε��� �� �� ���� ���
			sprintf(temp, "(%d/%d)", GetCandidateSelection()+1, GetCandidateCount());
			pDC->SetColor( MColorTable::TEXT[ MCOLOR_NORMAL]);
			pDC->Text(p.x + 4, p.y + GetCandidatePageSize()*pDC->GetFont()->GetHeight() + 4, temp);

//			pDC->EndFont();
		}

		pDC->SetColor(c);
		pDC->SetFont( MFontManager::Get( pFont->GetName()));		// ���� ��Ʈ�� ����
	}
}


Mint::Mint()
{
	m_hWnd = NULL;
	_ASSERT(m_pInstance==NULL);	// Singleton!!!
	m_pInstance = this;
	m_pMainFrame = NULL;
	m_pDC = NULL;
	m_pScriptManager = NULL;
	m_hImc = NULL;

	m_nWorkspaceWidth = 640;
	m_nWorkspaceHeight = 480;
	m_nStandardWidth = m_nWorkspaceWidth;
	m_nStandardHeight = m_nWorkspaceHeight;
	m_fRatioToStandardScreenWidth = 1.0f;
	m_fRatioToStandardScreenHeight = 1.0f;

	m_fnGlobalEventCallBack = NULL;

	m_pPopupMenu = NULL;

	m_pAnimationTemplateManager = NULL;

	m_pCandidateList = NULL;
	m_nCandidateListSize = 0;

	m_nCompositionAttributeSize = 0;
	memset(m_nCompositionAttributes, 0, sizeof(BYTE)*(MIMECOMPOSITIONSTRING_LENGTH));
	//memset(m_dwCompositionClauses, 0, sizeof(DWORD)*(MIMECOMPOSITIONSTRING_LENGTH));

	m_nCompositionCaretPosition = 0;

	m_bStartMintProfiler = false;

	m_bEnableIME = false;
	m_bEnableCharInput = false;

	m_pWidgetManager = new MWidgetManager(this);
	m_pScriptManager = new MScriptManager();
	m_pButtonGroupManager = new MButtonGroupManager();

	// madduck [4/5/2006]
	m_pLookManager	= new MLookManager();

	m_bShowCursor = true;
	m_ptLastCursorPos = MPOINT( 0, 0);
}

Mint::~Mint()
{
	m_ToolTipManager.Reset();
	m_DragManager.Clear();
	m_pPopupMenu = NULL;

	MWidget* pMainFrame = GetMainFrame();
	if (pMainFrame)
		pMainFrame->DeleteChildren();

	m_pWidgetManager->Clear();
	m_pLookManager->Clear();

	m_fnGlobalEventCallBack = NULL;
	m_pInstance = NULL;

	delete m_pWidgetManager;m_pWidgetManager=NULL;
	delete m_pScriptManager;
	m_pScriptManager = NULL;
	delete m_pButtonGroupManager; m_pButtonGroupManager=NULL;

	// madduck [4/5/2006]
	delete m_pLookManager; m_pLookManager = NULL;

	m_DragManager.Clear();
	m_TimerManager.Clear();
}

Mint* Mint::GetInstance(void)
{
	_ASSERT(m_pInstance!=NULL);
	return m_pInstance;
}

bool Mint::Initialize(int nWorkspaceWidth, int nWorkspaceHeight, int nStandardWidth, int nStandardHeight, 
					  MDrawContext* pDC, MFont* pDefaultFont, MWLua* pLua)
{
	_ASSERT(m_pMainFrame==NULL);

	m_pDC = pDC;

	if (m_pScriptManager->Initialize(pLua) == false)
	{
		assert(!"Script Manager Init ����");
	}

	MFontManager::SetDefaultFont(pDefaultFont);
	m_pAnimationTemplateManager = new MAnimationTemplateManager;

	m_pMainFrame = new MMainFrame("Mint");
	m_pMainFrame->SetRect(0, 0, nWorkspaceWidth, nWorkspaceHeight);
	m_pScriptManager->DeclWidget(MINT_WIDGET, "Mint", m_pMainFrame);	// ��� ������Ʈ ����

	SetWorkspaceSize(nWorkspaceWidth, nWorkspaceHeight);
	SetStandardSize(nStandardWidth, nStandardHeight);

	// ȭ�� �������� �̸� ��� �صд�.
	m_fRatioToStandardScreenWidth = (float)nWorkspaceWidth / (float)nStandardWidth;
	m_fRatioToStandardScreenHeight = (float)nWorkspaceHeight / (float)nStandardHeight;

	return true;
}

void Mint::Finalize(void)
{
	m_ToolTipManager.Reset();
	m_DragManager.Clear();

	if ( m_pPopupMenu != NULL)
	{
		m_pPopupMenu->Show( false);
		m_pPopupMenu = NULL;
	}

	if(m_pMainFrame!=NULL){
		delete m_pMainFrame;
		m_pMainFrame = NULL;
	}

	// LookManager ����
	m_pLookManager->Clear();

	MFontManager::Destroy();

	MCursorSystem::Destroy();

	if(m_pAnimationTemplateManager)
	{
		m_pAnimationTemplateManager->Destroy();
		delete m_pAnimationTemplateManager;
		m_pAnimationTemplateManager = NULL;
	}

	m_pDC = NULL;
}

bool Mint::ProcessEvent( MEvent& e)
{
	MWidget::m_EventArgs.Reset();

	if ( m_pMainFrame == NULL)		return false;

	if ( e.nMessage > MWM_MOUSEFIRST  &&  e.nMessage < MWM_MOUSELAST)
	{
		e.Pos.x = max( 0, min( m_nWorkspaceWidth, e.Pos.x));
		e.Pos.y = max( 0, min( m_nWorkspaceHeight, e.Pos.y));

		m_ptLastCursorPos = e.Pos;


		if ( m_hWnd != NULL)
		{
			switch ( e.nMessage)
			{
			case MWM_LBUTTONDOWN :
			case MWM_RBUTTONDOWN :
				::SetCapture( m_hWnd);
				break;

			case MWM_LBUTTONUP :
			case MWM_RBUTTONUP :
				::ReleaseCapture();
				break;
			}
		}
	}


	// Drag and drop ó��
	if ( m_DragManager.PostProcessDragEvent( m_pMainFrame, &e) == true)
		return true;

	// ToolTip ó��
	if ( m_ToolTipManager.PostProcessToolTipEvent( m_pMainFrame, &e) == true)
		return true;


	// ���� �̺�Ʈ ó��
	if ( m_fnGlobalEventCallBack)
	{
		if ( m_fnGlobalEventCallBack( &e) == true)
			return true;
	}


	// �˾� �޴� ó��
	if ( m_pPopupMenu != NULL)
	{
		MRESULT nResult = m_pPopupMenu->Event( &e);
		if ( nResult != MNOTPROCESSED)	return true;
	}


	// �Ϲ� Ű �̺�Ʈ ó��
	if ( e.nMessage > MWM_KEYFIRST  &&  e.nMessage < MWM_KEYLAST)
	{
		MRESULT nResult = MNOTPROCESSED;

		// ��Ŀ�̵� �������� ����
		MWidget* pFocused = m_pMainFrame->GetFocus();
		if ( pFocused != NULL)
		{
			// TabŰ ó�� - ���� �������� ��Ŀ���� �ѱ�
			if ( e.nMessage == MWM_KEYDOWN  &&  e.nKey == VK_TAB)
			{
				if ( pFocused->GetParent() != NULL)
				{
					list< MWidget*>& _children = pFocused->GetParent()->GetChilList();

					list< MWidget*>::iterator itr = _children.begin();
					while ( (*itr) != pFocused)		itr++;
					itr++;

					while ( 1)
					{
						if ( itr == _children.end())		itr = _children.begin();
						if ( (*itr) == pFocused)			break;
						if ( (*itr)->IsEnableFocus())
						{
							(*itr)->SetFocus();
							break;
						}

						itr++;
					}
				}

				nResult = MTRANSLATED;
			}

			// ��Ŀ�� ������ Ű �̺�Ʈ ó��
			else
			{
				nResult = pFocused->Event( &e);
			}
		}


		// Hot key ó��
		if ( nResult == MNOTPROCESSED  &&  e.nMessage == MWM_KEYDOWN)
		{
			nResult = m_pMainFrame->EventHotKey( e.nKey);
		}

		return ( (nResult != MNOTPROCESSED) ? true : false);
	}

	// ���콺 �̺�Ʈ ó��
	else if ( e.nMessage > MWM_MOUSEFIRST  &&  e.nMessage < MWM_MOUSELAST)
	{
		MRESULT nResult = MNOTPROCESSED;
		if ( m_pMainFrame->GetCapture())	nResult = m_pMainFrame->GetCapture()->Event( &e);
		else								nResult = m_pMainFrame->Event( &e);

		if ( nResult != MNOTPROCESSED)		return true;

		// ó������ ���� �޽��� ��ó��
		switch ( e.nMessage)
		{
		case MWM_LBUTTONDOWN :
		case MWM_LBUTTONDBLCLK :
		case MWM_RBUTTONDOWN :
		case MWM_RBUTTONDBLCLK :
			if ( m_pMainFrame->GetFocus())		m_pMainFrame->GetFocus()->ReleaseFocus();
			if ( m_pMainFrame->GetCapture())	m_pMainFrame->GetCapture()->ReleaseCapture();
			break;
		}

		if ( m_pMainFrame->GetMouseOver())		m_pMainFrame->GetMouseOver()->ReleaseMouseOver();
	}

	// �� �� �̺�Ʈ ó��
	else
	{
		MRESULT nResult = m_pMainFrame->Event( &e);
		return ( (nResult != MNOTPROCESSED) ? true : false);
	}

	return false;
}

bool Mint::ProcessEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	MEvent e;
	int nResult = e.TranslateEvent( hwnd, message, wparam, lparam);

	bool bRetVal = false;
	if ( nResult & EVENT_MINT_TRANSLATED)		bRetVal = ProcessEvent(e);
	else if ( nResult & EVENT_PROCESSED)		bRetVal = true;

	return bRetVal;
}

void Mint::Run(void)
{
	if ( m_bStartMintProfiler == true)
	{
		MGetMintProfiler()->Create( "MintProfiler.txt");
		MGetMintProfiler()->BeginUpdateProfile();
	}

	m_dwUpdateTime = timeGetTime();

	// ToolTip
	m_ToolTipManager.PostProcessToolTipRun( m_pMainFrame, m_dwUpdateTime);

	// Timer
	m_TimerManager.Update( m_dwUpdateTime);


	// Update
	if ( m_pMainFrame != NULL)
		m_pMainFrame->Run( m_dwUpdateTime);


	if ( m_bStartMintProfiler == true)		MGetMintProfiler()->EndUpdateProfile();
}

void Mint::Draw(void)
{
#define RESET_DC_SETTING()			{	pDC->SetClipRect( 0, 0, MGetWorkspaceWidth()-1, MGetWorkspaceHeight()-1);	\
										pDC->SetOrigin( 0, 0);														\
										pDC->SetOpacity(1.0f);														\
										pDC->SetColor( 255, 255, 255);	}
#define RESTORE_DC_SETTING()		{	pDC->SetOpacity(1.0f);	pDC->SetColor( 255, 255, 255);	}


	if ( m_bStartMintProfiler == true)		MGetMintProfiler()->BeginRenderProfile();

	if(!m_pMainFrame) return;

	MDrawContext* pDC = m_pDC;
	if ( pDC == NULL)		return;
	pDC->BeginDraw();


	// Reset dc setting
	RESET_DC_SETTING();

	// Draw widgets
	pDC->SetFont( MFontManager::Get( NULL));
	m_pMainFrame->Draw(pDC);


	// Reset dc setting
	RESET_DC_SETTING();


	// Tooltip
	{
		// Restore dc setting
		RESTORE_DC_SETTING();

		// Draw tooltip
		m_ToolTipManager.DrawToolTipImage( pDC);
	}


	
	// Drag and drop object
	{
		// Restore dc setting
		RESTORE_DC_SETTING();

		// Draw drag object
		m_DragManager.DrawDragImage( pDC, m_ptLastCursorPos);
	}


	// Candidate list
	{
		// Restore dc setting
		RESTORE_DC_SETTING();

		// Draw candidate list
		DrawCandidateList( pDC, m_CandidateListPos);
	}



	// Cursor
	{
		// Restore dc setting
		RESTORE_DC_SETTING();

		// Draw cursor
		MPOINT p = MEvent::LatestPos;
		MCursorSystem::Draw(pDC, p.x, p.y);
	}


	pDC->EndDraw();


	// �Ϻ� �׷��� ī�忡�� viewport�� �ʱ�ȭ������ �����Ƿ� ���⼭ ���� viewport�� �ʱ�ȭ�Ѵ�.
	// �����ϸ� ���� �ʿ��� ���� �ٶ�
	RESET_DC_SETTING();


	if ( m_bStartMintProfiler == true)
	{
		MGetMintProfiler()->EndRenderProfile();
		MGetMintProfiler()->OutputProfileText();

		m_bStartMintProfiler = false;
	}
}

MMainFrame* Mint::GetMainFrame(void)
{
	return m_pMainFrame;
}

MDrawContext* Mint::GetDrawContext(void)
{
	return m_pDC;
}

void Mint::SetHWND(HWND hWnd)
{
	m_hWnd = hWnd;

	HWND hImeWnd = ImmGetDefaultIMEWnd( hWnd);
	if ( hImeWnd != INVALID_HANDLE_VALUE)
		SendMessage( hImeWnd, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0);

	HIMC hImc = ImmGetContext( hWnd);
	if ( hImc)
	{
		DWORD dwIMEConvMode, dwSentMode;
		ImmGetConversionStatus( hImc, &dwIMEConvMode, &dwSentMode);
		ImmSetConversionStatus( hImc,IME_CMODE_ALPHANUMERIC, dwSentMode);
		ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext( hWnd, hImc);

		m_bEnableIME = false;
	}

	m_hImc = ImmAssociateContext( hWnd, NULL);

	SetFocus( hWnd);
}

HWND Mint::GetHWND(void)
{
	return m_hWnd;
}

void Mint::EnableIME( bool bEnable)
{
	if ( MEvent::GetIMESupport() == false)		return;
	if ( m_bEnableIME == bEnable)				return;

	HWND hWnd = GetHWND();
	if ( hWnd == INVALID_HANDLE_VALUE)			return;

	if ( bEnable == true)
	{
		if ( m_hImc == NULL)					return;

		ImmAssociateContext( hWnd, m_hImc);
		ImmNotifyIME( m_hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	}
	else
	{
		m_hImc = ImmAssociateContext( hWnd, NULL);
		if ( m_hImc == NULL)					return;
	}

	SetFocus( hWnd);

	m_bEnableIME = bEnable;
}

bool Mint::IsEnableIME(void)
{
	return m_bEnableIME;
}

void Mint::EnableCharInput(bool bEnable)
{
	m_bEnableCharInput = bEnable;
}

bool Mint::IsEnableCharInput()
{
	return m_bEnableCharInput;
}

void Mint::ClearImeCompose()
{
	HWND hWnd = GetInstance()->GetHWND();
	if ( hWnd == NULL)		return;

	HIMC hImc = ImmGetContext( hWnd);
	if ( hImc == NULL)		return;

	ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmReleaseContext( hWnd, hImc);
}


void Mint::ChangedToolTipText( MWidget* pWidget)
{
	m_ToolTipManager.ChangedToolTipText( pWidget);
}


bool Mint::SetPopupMenu( MWidget* pPopupMenu)
{
	ClearPopupMenu();

	if ( pPopupMenu == NULL)	return false;

	m_pPopupMenu = pPopupMenu;

	m_pPopupMenu->SetListener( NULL);
	m_pPopupMenu->BringToTop();
	m_pPopupMenu->Show( true);
	m_pPopupMenu->SetFocus();

	return true;
}


bool Mint::ClearPopupMenu()
{
	if ( m_pPopupMenu != NULL)
	{
		m_pPopupMenu->Show( false);
		m_pPopupMenu = NULL;

		return true;
	}
	return false;
}


bool Mint::ShowCursor( bool bShow)
{
	bool bRetVal = m_bShowCursor;
	if ( m_bShowCursor != bShow)
	{
		m_bShowCursor = bShow;


		// Clear UI
		m_ToolTipManager.Reset();
		m_DragManager.Clear();

		if ( m_pPopupMenu != NULL)
		{
			m_pPopupMenu->Show( false);
			m_pPopupMenu = NULL;
		}


		MWidget* pCapture = m_pMainFrame->GetCapture();
		if ( pCapture != NULL)
			pCapture->InitMouseState();


		if ( bShow == true)
		{
			MCursorSystem::Set( "arrow");
			::ShowCursor( TRUE);
		}
		else
			::ShowCursor( FALSE);
	}

	return bRetVal;
}


void Mint::ResetMouseState()
{
	m_DragManager.Clear();

	m_ToolTipManager.Reset();
	m_ToolTipManager.ResetCursor();

	if(m_pMainFrame)
	{
		m_pMainFrame->ReleaseFocus();
		m_pMainFrame->ReleaseMouseOver();
	}
}


void Mint::StartMintProfiler()
{
	m_bStartMintProfiler = true;
}


void Mint::SetGlobalEvent(MGLOBALEVENTCALLBACK pGlobalEventCallback)
{
	m_fnGlobalEventCallBack = pGlobalEventCallback;
}



MWidget* Mint::NewWidget(const char* szClass, const char* szName, MWidget* pParent, MListener* pListener)
{
	MWidget* pNewWidget = NULL;

	if(strcmp(szClass, MINT_WIDGET)==0) pNewWidget = new MWidget(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_LABEL)==0) pNewWidget = new MLabel(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_LABELEX)==0) pNewWidget = new MLabelEx(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_BUTTON)==0) pNewWidget = new MButton(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_EDIT)==0) pNewWidget = new MEdit(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_EDITCTRL)==0) pNewWidget = new MEditCtrl(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_LISTBOX)==0) pNewWidget = new MListBox(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_LISTCTRL)==0) pNewWidget = new MListCtrl(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_COMBOBOX)==0) pNewWidget = new MComboBox(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_FRAME)==0) pNewWidget = new MFrame(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_GROUP)==0) pNewWidget = new MGroup(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_SCROLLBAR)==0) pNewWidget = new MScrollBar(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_SPINCTRL)==0) pNewWidget = new MSpinCtrl(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_SLIDER)==0) pNewWidget = new MSlider(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_TABCTRL)==0) pNewWidget = new MTabCtrl(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_PICTURE)==0) pNewWidget = new MPicture(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_ANIMATION)==0) pNewWidget = new MAnimation(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_PANEL)==0) pNewWidget = new MPanel(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_PAINT)==0) pNewWidget = new MPaint(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_SHEET)==0) pNewWidget = new MSheet(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_TEXTVIEW)==0) pNewWidget = new MTextView(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_TREECTRL)==0) pNewWidget = new MTreeCtrl(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_CATEGORYCTRL)==0) pNewWidget = new MCategoryCtrl(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_LAYER)==0) pNewWidget = new MLayer(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_VIEWCTRL)==0) pNewWidget = new MViewCtrl(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_SLIDESHOW)==0) pNewWidget = new MSlideShow(szName, pParent, pListener);
	else if(strcmp(szClass, MINT_LINEPRINTER)==0) pNewWidget = new MLinePrinter( szName, pParent, pListener);
	else if(strcmp(szClass, MINT_TEXTFRAME)==0) pNewWidget = new MTextFrame( szName, pParent, pListener);
	else if(strcmp(szClass, MINT_PROGRESSCTRL)==0) pNewWidget = new MProgressCtrl( szName, pParent, pListener);
	else if(strcmp(szClass, MINT_SELECTOR)==0) pNewWidget = new MSelector( szName, pParent, pListener);
	else if(strcmp(szClass, MINT_POPUPMENU)==0) pNewWidget = new MPopupMenu( szName, pParent, pListener);

	return pNewWidget;
}


MWidget* Mint::FindWidget(MPOINT& p)
{
	return m_pMainFrame->WindowFromPoint(p);
}

MWidget* Mint::FindWidget(int x, int y)
{
	return m_pMainFrame->WindowFromPoint( MPOINT(x, y));
}

MWidget* Mint::FindWidget(const char* szName)
{
	return m_pWidgetManager->FindWidget(string(szName));
}

int Mint::GetWorkspaceWidth(void)
{	
	return m_nWorkspaceWidth;
}

int Mint::GetWorkspaceHeight(void)
{
	return m_nWorkspaceHeight;
}

int Mint::GetStandardWidth(void)
{	
	return m_nStandardWidth;
}

int Mint::GetStandardHeight(void)
{
	return m_nStandardHeight;
}

void Mint::SetWorkspaceSize(int w, int h)
{
	m_nWorkspaceWidth = w;
	m_nWorkspaceHeight = h;
}

void Mint::SetStandardSize(int w, int h)
{
	m_nStandardWidth = w;
	m_nStandardHeight = h;
}

const char* Mint::GetDefaultFontName(void) const
{
	static char* szFontName[] = {
		"Tahoma",		// Default
		"Gulim",		// �ѱ�
		"MS PGothic",	// �Ϻ�
		"MingLiU",		// ��ü
		"NSimSun"		// ��ü
	};

	int nFont = 0;
	if(GetPrimaryLanguageIdentifier()==LANG_KOREAN) nFont = 1;
	else if(GetPrimaryLanguageIdentifier()==LANG_JAPANESE) nFont = 2;
	else if(GetPrimaryLanguageIdentifier()==LANG_CHINESE){
		if(GetSubLanguageIdentifier()==SUBLANG_CHINESE_TRADITIONAL) nFont = 3;
		else nFont = 4;
	}

	return szFontName[nFont];
}

int Mint::GetPrimaryLanguageIdentifier(void) const
{
	/*
	Language Identifiers
	+-------------------------+-------------------------+
	|      SubLanguage ID     |   Primary Language ID   |
	+-------------------------+-------------------------+
	15                    10  9                         0   bit

	ms-help://MS.VSCC.2003/MS.MSDNQTR.2003FEB.1042/intl/nls_8xo3.htm
	ms-help://MS.VSCC.2003/MS.MSDNQTR.2003FEB.1042/intl/nls_238z.htm
	*/
	HKL hKeyboardLayout = GetKeyboardLayout(0);
	WORD nLanguageIdentifier = LOWORD(hKeyboardLayout);
	return PRIMARYLANGID(nLanguageIdentifier);
}

int Mint::GetSubLanguageIdentifier(void) const
{
	HKL hKeyboardLayout = GetKeyboardLayout(0);
	WORD nLanguageIdentifier = LOWORD(hKeyboardLayout);
	return SUBLANGID(nLanguageIdentifier);
}

const char* Mint::GetLanguageIndicatorString(void) const
{
	/*
	static char* szIndicator[] = {
	"EN", "KO", "JP", "CH", "?",
	};
	*/
	static char* szIndicator[] = {
		"EN", "��", "��", "��", "?",
	};
	switch(GetPrimaryLanguageIdentifier()){
	case LANG_ENGLISH:
		return szIndicator[0];
	case LANG_KOREAN:
		return szIndicator[1];
	case LANG_JAPANESE:
		return szIndicator[2];
	case LANG_CHINESE:
		return szIndicator[3];
	default:
		return szIndicator[4];
	}
}

bool Mint::IsNativeIME(void) const
{
	HIMC hImc = ImmGetContext(GetInstance()->GetHWND());
	if(hImc==NULL) return false;

	bool bNative = false;

	if(GetPrimaryLanguageIdentifier()==LANG_JAPANESE){
		// �Ϻ����ΰ�� ConversionMode�� �ƴ� OpenSatus�� �Է±Ⱑ Ȱ��ȭ �Ǿ����� �� �� �ִ�.
		// �Ϻ��� IME �̻���!
		bNative = (ImmGetOpenStatus(hImc)==TRUE);
	}
	else{
		// ����(IME_CMODE_ALPHANUMERIC) �Ǵ� ����Ƽ��(IME_CMODE_NATIVE) ��������� �Ǻ�
		DWORD dwConvMode, dwSentMode;
		ImmGetConversionStatus(hImc, &dwConvMode, &dwSentMode);

		bNative = (dwConvMode&IME_CMODE_NATIVE);
	}

	ImmReleaseContext(GetInstance()->GetHWND(), hImc);

	return bNative;
}

void Mint::OpenCandidateList(void)
{
	// Candidate List ����
	HIMC hImc = ImmGetContext(GetInstance()->GetHWND());
	if(hImc==NULL) return;

	m_nCandidateListSize = ImmGetCandidateList(hImc, 0, NULL, 0);

	//if(m_pCandidateList!=NULL) delete[] m_pCandidateList;
	//m_pCandidateList = new unsigned char[m_nCandidateListSize];
	if(m_pCandidateList!=NULL){ delete[] m_pCandidateList; m_pCandidateList = NULL; }
	if(m_nCandidateListSize>0) {
		m_pCandidateList = new unsigned char[m_nCandidateListSize];
		ImmGetCandidateList(hImc, 0, (LPCANDIDATELIST)m_pCandidateList, m_nCandidateListSize);
	}

	ImmReleaseContext(GetInstance()->GetHWND(), hImc);
}

void Mint::CloseCandidateList(void)
{
	// Candidate List ����
	if(m_pCandidateList!=NULL) delete[] m_pCandidateList;
	m_pCandidateList = NULL;
	m_nCandidateListSize = 0;
}

const char* Mint::GetCandidate(int nIndex) const
{
	if(m_pCandidateList==NULL) return NULL;

	CANDIDATELIST* pCandidateList = (CANDIDATELIST*)m_pCandidateList;

	if(nIndex>=(int)pCandidateList->dwCount) return NULL;

	char* pCandidate = (char*)((BYTE*)pCandidateList+pCandidateList->dwOffset[nIndex]);
	return pCandidate;
}

int Mint::GetCandidateCount(void) const
{
	if(m_pCandidateList==NULL) return 0;

	CANDIDATELIST* pCandidateList = (CANDIDATELIST*)m_pCandidateList;

	return pCandidateList->dwCount;
}

int Mint::GetCandidateSelection(void) const
{
	if(m_pCandidateList==NULL) return 0;

	CANDIDATELIST* pCandidateList = (CANDIDATELIST*)m_pCandidateList;

	return pCandidateList->dwSelection;
}

int Mint::GetCandidatePageStart(void) const
{
	if(m_pCandidateList==NULL) return 0;

	// GetCandidatePageStart(); �� �Ϻ���� ���װ� �����Ƿ�, �������� ���
	int nStart = GetCandidatePageSize() * (GetCandidateSelection()/GetCandidatePageSize());

	return nStart;

	/*
	// �Ϻ�� �����ϰ� �۵��Ǵ� ���� �ڵ�
	if(m_pCandidateList==NULL) return 0;

	CANDIDATELIST* pCandidateList = (CANDIDATELIST*)m_pCandidateList;

	return pCandidateList->dwPageStart;
	*/
}

int Mint::GetCandidatePageSize(void) const
{
	if(m_pCandidateList==NULL) return 0;

	CANDIDATELIST* pCandidateList = (CANDIDATELIST*)m_pCandidateList;

	return pCandidateList->dwPageSize;
}

void Mint::SetCandidateListPosition(MPOINT& p, int nWidgetHeight)
{
	MPOINT cp = p;

	// ���� ���� üũ
	if((cp.x+GetCandidateListWidth())>=MGetWorkspaceWidth()){
		cp.x = MGetWorkspaceWidth()-GetCandidateListWidth();
	}
	else{
//		cp.x -= 4;
	}
	// ���� ���� üũ
	if((cp.y+GetCandidateListHeight()+nWidgetHeight+8)>=MGetWorkspaceHeight()){
		cp.y -= GetCandidateListHeight() + 6;
	}
	else{
		cp.y += (nWidgetHeight+6);
	}

	m_CandidateListPos = cp;
}

int Mint::GetCandidateListWidth(void)
{
	int w = 60;
	if(GetCandidateCount()>0){
		const char* szCandidate = GetCandidate(0);
		w = max(w, MFontManager::Get( "Default")->GetWidth(szCandidate)+100);	// �ٸ� ���ڿ��� �ʺ� �� Ŭ �� �����Ƿ� �������� ����� �ش�.
	}
	return w + 4;
}

int Mint::GetCandidateListHeight(void)
{
	return (MFontManager::Get( "Default")->GetHeight()*(GetCandidatePageSize()+1) + 6);
}

// ��Ƽ���� ������ ���� ���� ���� ���
int Mint::DrawCompositionAttribute(MDrawContext* pDC, MPOINT& p, const char* szComposition, int i)
{
	if(i>=(int)strlen(szComposition)) return 0;
	//if(pMint->GetPrimaryLanguageIdentifier()!=LANG_JAPANESE) return;

	const BYTE* pCompAttr = GetCompositionAttributes();
	DWORD nCompAttrSize = GetCompositionAttributeSize();

	if(i>=(int)nCompAttrSize) return 0;	// Composition Attribute ������ ����� ���

	MFont* pFont = pDC->GetFont();
	int nFontHeight = pFont->GetHeight();
	MCOLOR c = pDC->GetColor();

	int nCharSize = 1;
	bool bTwoByteChar = IsHangul(szComposition[i]);
	if(bTwoByteChar) nCharSize = 2;
	int nWidth = pFont->GetWidth(&(szComposition[i]), nCharSize);

	if(pCompAttr[i]==ATTR_TARGET_CONVERTED)				// ��ȯ�� ����
		pDC->SetColor(MCOLOR(255, 0, 0, 128));
	else if(pCompAttr[i]==ATTR_TARGET_NOTCONVERTED)		// ��ȯ���� �ʴ� ����
		pDC->SetColor(MCOLOR(0, 196, 0, 128));
	else												// ��ȯ ���� ��
		pDC->SetColor(MCOLOR(128, 128, 128, 128));

	pDC->FillRectangle(p.x, p.y, nWidth, nFontHeight);

	pDC->SetColor(c);

	return nWidth;
}

void Mint::DrawCompositionAttributes(MDrawContext* pDC, MPOINT& p, const char* szComposition)
{
	if(strlen(szComposition)==0) return;

	int nWidth = 0;

	for(int i=0; szComposition[i]!=NULL; i++){
		bool bTwoByteChar = IsHangul(szComposition[i]);

		nWidth += DrawCompositionAttribute(pDC, MPOINT(p.x+nWidth, p.y), szComposition, i);

		if(bTwoByteChar) i++;
	}
}

void Mint::DrawIndicator(MDrawContext* pDC, MRECT& r)
{
	MFont* pFont = pDC->GetFont();
	int nFontHeight = pFont->GetHeight();

	// �ش� �� ���� ǥ��
	const char* szLanguageIndicator = GetLanguageIndicatorString();

	int nIdicatorWidth = pFont->GetWidth(szLanguageIndicator);
	MCOLOR c = pDC->GetColor();

	MRECT fr(r.x+r.w-nIdicatorWidth-4, r.y+2, nIdicatorWidth+3, r.h-4);

	if(IsNativeIME()==true)
	{
		pDC->SetColor(MCOLOR(130,130,130));
		pDC->FillRectangle(fr);
		pDC->SetColor(MCOLOR(0, 0, 0));
//		if (pDC->BeginFont())
		{
			pDC->Text(r.x+r.w-nIdicatorWidth-2, r.y + (r.h-nFontHeight)/2, szLanguageIndicator);
//			pDC->EndFont();
		}
	}
	else{
//		pDC->SetColor(MCOLOR(0, 0, 0));
//		pDC->FillRectangle(fr);
//		pDC->SetColor( MColorTable::NORMAL );
	}

//	pDC->Text(r.x+r.w-nIdicatorWidth-2, r.y + (r.h-nFontHeight)/2, szLanguageIndicator);
	pDC->SetColor(c);
}


void Mint::Clear()
{
	m_ToolTipManager.Reset();
	m_DragManager.Clear();
	m_pPopupMenu = NULL;


	MWidget* pMainFrame = GetMainFrame();
	if (pMainFrame)
		pMainFrame->DeleteChildren();

	m_pWidgetManager->Clear();
	m_pLookManager->Clear();
	
	MBitmapManager::Clear();
}

void Mint::TransformScreenSizeRatioToStandardSize(int &nx, int &ny, int &nWidth, int &nHeight)
{
	nWidth = (int)(nWidth * m_fRatioToStandardScreenWidth);
	nHeight = (int)(nHeight * m_fRatioToStandardScreenHeight);

	nx = (int)(nx * m_fRatioToStandardScreenWidth);
	ny = (int)(ny * m_fRatioToStandardScreenHeight);
}


MCursor* Mint::NewCursor(const char* pszAliasName)
{
	return new MDefaultCursor(pszAliasName);
}


void Mint::EnableCallScript( bool bEnable)
{
	m_pScriptManager->EnableCallScript( bEnable);
}

bool Mint::IsEnableCallScript()
{
	return m_pScriptManager->IsEnableCallScript();
}

// Sample Widgets


TCHAR* szStar[] = {
	_T("������ �������� �ϴÿ���"),
	_T("������ ���� �� �ֽ��ϴ�."),
	_T("���� �ƹ� ������ ����"),
	_T("���� ���� ������ �� ���� ���մϴ�. "),
	_T("���� �ӿ� �ϳ� �� �������� ����  "),
	_T("���� �� ����� ����"),
	_T("���� ��ħ�� ���� ����̿�, "),
	_T("���Ϲ��� ���� ����̿�,"),
	_T("���� ���� û���� ������ ���� ����Դϴ�. "),
	_T("�� �ϳ��� �߾��"),
	_T("�� �ϳ��� �����"),
	_T("�� �ϳ���  �����԰�"),
	_T("�� �ϳ��� �����"),
	_T("�� �ϳ��� �ÿ�"),
	_T("�� �� ���� ��Ӵ�, ��Ӵ�"),
	_T("��Ӵ�, ���� �� �ϳ��� �Ƹ� �ٿ� �� �� ���� �ҷ� ���ϴ�. ���б� �� å���� "),
	_T("�����ߴ� ���̵��� �̸���, ��, ��, ��, �̷� �̱� �ҳ���� �̸���,"),
	_T("���� �Ʊ� ��Ӵ� �� �����ֵ��� �̸���, ������ �̿� ������� �̸���,"),
	_T("��ѱ�, ������, �䳢, ���,  ���, �����ý� ��, ���̳� ������ ����,"),
	_T("�̷� ������ �̸��� �ҷ� ���ϴ�."),
	_T("�̳׵��� �ʹ��� �ָ� �ֽ��ϴ�."),
	_T("���� �ƽ����� �� ����,"),
	_T("��Ӵ�,"),
	_T("�׸��� ����� �ָ� �ϰ����� ��ʴϴ�."),
	_T("���� �������� �׸���"),
	_T("�� ���� ������ ���� ��� ����"),
	_T("�� �̸��ڸ� �� ����,"),
	_T("������ ���� ���������ϴ�."),
	_T("����, ���� ���� ��� ������"),
	_T("�β����� �̸��� �����ϴ� ����Դϴ�."),
	_T("�׷��� �ܿ��� ������ ���� ������ ���� ����,"),
	_T("���� ���� �Ķ� �ܵ� �Ǿ����"),
	_T("�� �̸��� ���� ��� ������,"),
	_T("�ڶ�ó�� Ǯ�� ������ �ſܴ�. "),
};


void MCreateSample(void)
{
}

void MDestroySample(void)
{
}


// TODO: MLocale�� ��ü�մϴ�. ���� �����ϰڽ��ϴ�.  [2010/9/28 praptor]
//// mbs_to_wcs
//std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc)
//{
//	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
//	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
//	std::mbstate_t state = std::mbstate_t();
//	std::vector<wchar_t> buf(str.size() + 1);
//	char const* in_next = str.c_str();
//	wchar_t* out_next = &buf[0];
//	std::codecvt_base::result r = codecvt.in(state, 
//		str.c_str(), str.c_str() + str.size(), in_next, 
//		&buf[0], &buf[0] + buf.size(), out_next);
//	if (r == std::codecvt_base::error)
//		throw std::runtime_error("can't convert string to wstring");   
//	return std::wstring(&buf[0]);
//}

// TODO: MLocale�� ��ü�մϴ�. ���� �����ϰڽ��ϴ�.  [2010/9/28 praptor]
//// wcs_to_mbs
//std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc)
//{
//	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
//	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
//	std::mbstate_t state = std::mbstate_t();
//	std::vector<char> buf((str.size() + 1) * codecvt.max_length());
//	wchar_t const* in_next = str.c_str();
//	char* out_next = &buf[0];
//	std::codecvt_base::result r = codecvt.out(state, 
//		str.c_str(), str.c_str() + str.size(), in_next, 
//		&buf[0], &buf[0] + buf.size(), out_next);
//	if (r == std::codecvt_base::error)
//		throw std::runtime_error("can't convert wstring to string");   
//	return std::string(&buf[0]);
//}


// SetClipboard
bool SetClipboard( const char* text)
{
	HANDLE hData = GlobalAlloc( GMEM_DDESHARE, strlen( text) + 2);
	if ( hData == NULL)
		return false;

	char* lpData = (char*)GlobalLock( hData);
	if ( lpData == NULL)
		return false;

	strcpy( lpData, text);
	GlobalUnlock( hData);

	if ( OpenClipboard( NULL))
	{
		EmptyClipboard();
		SetClipboardData( CF_TEXT, hData);
		CloseClipboard();
	}

	return true;
}


// GetClipboard
bool GetClipboard( char* text, size_t size)
{
	if ( OpenClipboard( NULL) == FALSE)
		return false;

	HANDLE hClipData = GetClipboardData( CF_TEXT);
	if ( hClipData == NULL)
	{
		CloseClipboard();
		return false;
	}

	HANDLE hText = GlobalAlloc( GMEM_MOVEABLE, GlobalSize( hClipData));
	if ( hText == NULL)
	{
		CloseClipboard();
		return false;
	}

	char* lpClipData = (char *)GlobalLock( hClipData);
	if ( lpClipData == NULL)
	{
		CloseClipboard();
		return false;
	}

	if ( strlen( lpClipData) > (size - 2))
	{
		memcpy( text, lpClipData, size - 1);
		text[ size - 1] = 0;
	}
	else
		strcpy( text, lpClipData);

	GlobalUnlock( hClipData);
	CloseClipboard();
	GlobalUnlock( hText);

	return true;
}





/*
TODO: Edit ������ ����
TODO: Text Area IME ����
*/

} // namespace mint3