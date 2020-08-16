#include "StdAfx.h"
#include "XWeaponBindingMenu.h"

// Define fade factor
const float FADE_FACTOR = 0.0045f;						// ���� Ŭ���� ���̵� �ð��� ª������

#define DEFAULT_DROP_HEIGHT	45

XWeaponListBox::XWeaponListBox( const char* szName, MWidget* pParent, MListener* pListener)
: MListBox( szName, pParent, pListener)
{
}


bool XWeaponListBox::OnEvent(MEvent* pEvent, MListener* pListener)
{
	if(MListBox::OnEvent(pEvent, pListener)==true) return true;

	MRECT r = GetClientRect();
	
	switch(pEvent->nMessage)
	{
	case MWM_LBUTTONDOWN:
	case MWM_MOUSEMOVE:
		{
// 			if(r.InPoint(pEvent->Pos)==false) return false;
// 			int nSelItem = FindItem(pEvent->Pos);
// 			if(nSelItem==-1)
// 				return true;
// 
// 			if ( Get( nSelItem)->IsEnabled() == false)
// 				return true;
// 
// 			SetSelIndex(nSelItem);
// 			OnItemSelected(pEvent);

			return true;
		}
	case MWM_LBUTTONUP:
		{
// 			if(r.InPoint(pEvent->Pos)==false)
// 			{
// 				if (pListener!=NULL) pListener->OnCommand(this,MEVENT_LB_ITEM_CLICKOUT);
// 				return false;
// 			}
// 
// 			int nSelItem = FindItem(pEvent->Pos);
// 			if(nSelItem==-1)
// 				return true;
// 
// 			if ( Get( nSelItem)->IsEnabled() == false)
// 				return true;
// 
// 			SetSelIndex(nSelItem);
// 			OnItemSelected(pEvent);
// 
// 			if(pListener!=NULL) pListener->OnCommand(this, MEVENT_LB_ITEM_SEL);
// 
// 			Show(false);
			return false;	// �ٸ� Widget���� �Ѱ��ش�.
		}
	case MWM_RBUTTONDOWN:
	case MWM_LBUTTONDBLCLK:
	case MWM_RBUTTONDBLCLK:
		Show(false);
		return false;	// �ٸ� Widget���� �Ѱ��ش�.
	}

	return false;
}



XWeaponBindingMenu::XWeaponBindingMenu( const char* szName/*=NULL*/, MWidget* pParent/*=NULL*/, MListener* pListener/*=NULL*/ )
: MWidget(szName, pParent, pListener)
{
//	m_AlignmentMode = MAM_LEFT|MAM_VCENTER;

	m_nDropHeight = DEFAULT_DROP_HEIGHT;		// ��ӹڽ� ���� ����

//	m_bComboButton = true;						// �޺� ��ư���� ����

	m_dwFadeTimer = timeGetTime();				// ����Ʈ Ÿ�̸� �ʱ�ȭ

	// ����Ʈ �ڽ� ����
	m_pListBox = new XWeaponListBox( NULL, pParent, this);
//	MFont* pFont = MFontManager::Get("fnt_DefUI");
//	m_pListBox->SetFont(pFont);
	m_pListBox->Show( false);
	m_pListBox->SetOpacity( 0.0f);

	char szTmp[ 64];
	sprintf_s( szTmp, "Primary");
	m_pListBox->AddString(szTmp);
	sprintf_s( szTmp, "second");
	m_pListBox->AddString(szTmp);
	sprintf_s( szTmp, "None");
	m_pListBox->AddString(szTmp);
}

XWeaponBindingMenu::~XWeaponBindingMenu( void )
{
	if (m_pListBox)
	{
		m_pListBox->ResetContent();
	}
	SAFE_DELETE(m_pListBox);
}

void XWeaponBindingMenu::OnMouseDown( MEvent* pEvent )
{
//	m_EventHandler.OnCommand(this, MEVENT_MOUSE_DOWN);
}

bool XWeaponBindingMenu::OnCommand( MWidget* pWidget, MEventMsg nMsg, const wchar_t* szArgs/*=NULL*/ )
{
	// ����Ʈ �ڽ��� ���� ������ �׳� ����
	if ( m_pListBox == NULL)
		return true;


	// ��ư �޽����� ���...
	if ( pWidget == this)
	{
// 		if ( nMsg == MEVENT_MOUSE_DOWN)
// 		{
// 			// üũ���� Ŭ���̺�Ʈ�� ���� ���� ������
// 			// ���� üũ ���¸� false�� �˻��Ѵ�.
// 			if ( m_bLButtonDown == true)
// 				m_pListBox->SetFocus();
// 			return true;
// 		}
// 		else if (nMsg == MEVENT_CB_SELECTED_INDEX_CHANGED)
// 		{
// 			int index = m_pListBox->GetSelIndex();
// 			gvar.MyInfo.PaletteList.Binding(index);
// 			return true;
// 		}
		return false;
	}


	// ����Ʈ�ڽ� �޽����� ���...
	else if ( pWidget == m_pListBox)
	{
		// ���콺�� ���� ����
		//if ( nMsg == MEVENT_LB_ITEM_SEL || nMsg == MEVENT_LB_ITEM_SEL2 || nMsg == MEVENT_LB_ITEM_DBLCLK)
// 		if ( nMsg == MEVENT_LEAVE)
// 		{
// 			// ��ư�� üũ ���¸� �����Ѵ�
// 			//SetCheck( false);
// 			m_bLButtonDown = false;
// 
// 
// 			// ��ư�� �ؽ�Ʈ�� �����Ѵ�
// 			SetText( m_pListBox->GetSelItemString());
// 
// 
// 			// �����ʿ� Ŀ�ǵ� ����
// 			OnSelectedItemChanged();
// 		}

		return true;
	}

	return false;
}

void XWeaponBindingMenu::OnRun( void )
{
	// ����Ʈ�ڽ��� ���� ������ ����
	if ( !m_pListBox)
		return;

	// 10ms���� ����
	DWORD curr = timeGetTime();
	DWORD diff = curr - m_dwFadeTimer;

	if ( diff < 10)
		return;

	m_dwFadeTimer = curr;

	// ����Ʈ �ڽ��� ��Ŀ���� ������ �����
	if ( m_bLButtonDown && !m_pListBox->IsFocus())
		m_bLButtonDown = false;


	// ����Ʈ �ڽ��� ��ġ �� ����� ���� �Ѵ�
	MRECT rect = GetRect();
	if ( m_pParent && ((rect.y + rect.h + m_nDropHeight) > m_pParent->GetClientRect().h))
		rect.y -= m_nDropHeight;
	else
		rect.y += rect.h;

	rect.h = m_nDropHeight;
	m_pListBox->SetRect( rect);


	// ����Ʈ �ڽ��� ���̵� ��Ų��
	float delta  = (float)diff * FADE_FACTOR;
	float fFade = m_pListBox->GetOpacity() + ( m_bLButtonDown ? delta : -delta);

	if ( fFade > 1.0f)			fFade = 1.0f;
	else if ( fFade < 0.0f)		fFade = 0.0f;

	m_pListBox->SetOpacity( fFade);
	m_pListBox->Show( (fFade > 0.0f) ? true : false);
}

void XWeaponBindingMenu::OnSelectedItemChanged()
{
	m_EventHandler.OnCommand(this, MEVENT_SELCHANGE);
}

void XWeaponBindingMenu::OnDraw( MDrawContext* pDC )
{
	MRECT r = GetInitialClientRect();
	//if (GetBackgroundColor().a != 0)
	//{
	//	pDC->SetColor(GetBackgroundColor());
	//	pDC->FillRectangle(r);
	//}

	//if (GetBorderStyle() == MBS_SINGLE)
	{
		pDC->SetColor(255, 255, 255);
		pDC->Rectangle(r);
		pDC->SetColor(255, 255, 255);
		pDC->Text(r, GetText(), MAM_LEFT|MAM_VCENTER);
	}
}

bool XWeaponBindingMenu::SetSelIndex( int i )
{
	bool bReturn = m_pListBox->SetCurSel(i) ? true : false;
	if(bReturn==true) SetText(m_pListBox->GetItemText( m_pListBox->GetCurSel()));
	return bReturn;
}

void XWeaponBindingMenu::glueSetSelectedIndex( int nIndex )
{
	SetSelIndex(nIndex);
}