#include "stdafx.h"
#include "MComboBox.h"
#include "Mint.h"


// Define fade factor
const float FADE_FACTOR = 0.0045f;						// ���� Ŭ���� ���̵� �ð��� ª������



namespace mint3 {

MComboListBox::MComboListBox(MWidget* pParent, MListener* pListener)
: MListBox(pParent, pListener)
{
	LOOK_IN_CONSTRUCTOR()
	SetClipByParent( false);
}

bool MComboListBox::OnEvent(MEvent* pEvent, MListener* pListener)
{
	if(MListBox::OnEvent(pEvent, pListener)==true) return true;
	
	switch(pEvent->nMessage){
	case MWM_LBUTTONDOWN:
	case MWM_RBUTTONDOWN:
	case MWM_LBUTTONDBLCLK:
	case MWM_RBUTTONDBLCLK:
		Show(false);
		return false;	// �ٸ� Widget���� �Ѱ��ش�.
	}

	return false;
}

IMPLEMENT_LOOK(MComboListBox, MListBoxLook)


#define DEFAULT_DROP_HEIGHT	200

MComboBox::MComboBox(const char* szName, MWidget* pParent, MListener* pListener)
: MButton(szName, pParent, pListener)
{
	MButton::SetListener(this);					// Button �޼����� �ѹ� �ɷ��� �����ش�.

	m_bHighlight = false;						// �޺��ڽ������� ���̶���Ʈ(&)�� ������ �ʿ䰡 ����.

	m_pComboBoxListener = pListener;

	m_AlignmentMode = MAM_HCENTER|MAM_VCENTER;

	m_Type = MBT_PUSHBTN;						// ��ư�� Ǫ����ư ����

	m_nDropHeight = DEFAULT_DROP_HEIGHT;		// ��ӹڽ� ���� ����

	m_bComboButton = true;						// �޺� ��ư���� ����

	m_dwFadeTimer = timeGetTime();				// ����Ʈ Ÿ�̸� �ʱ�ȭ


	// ����Ʈ �ڽ� ����
	m_pListBox = new MComboListBox( pParent, this);
	m_pListBox->Show( false);
	m_pListBox->SetOpacity( 0.0f);
}

MComboBox::~MComboBox()
{
	if (!IsEmptyChildWidgets())
	{
		delete m_pListBox;
	}
	m_pListBox = NULL;
}

void MComboBox::SetDropSize(int nHeight)
{
	m_nDropHeight = nHeight;
}

void MComboBox::Add(const char* szItem)
{
	m_pListBox->Add(szItem);
}

void MComboBox::Add(MListItem* pItem)
{
	m_pListBox->Add(pItem);
}

const char* MComboBox::GetString(int i)
{
	return m_pListBox->GetString(i);
}

MListItem* MComboBox::Get(int i)
{
	return m_pListBox->Get(i);
}

void MComboBox::Remove(int i)
{
	m_pListBox->Remove(i);
}

void MComboBox::RemoveAll()
{
	m_pListBox->RemoveAll();
}

int MComboBox::GetCount()
{
	return m_pListBox->GetCount();
}

int MComboBox::GetSelIndex()
{
	return m_pListBox->GetSelIndex();
}

bool MComboBox::SetSelIndex(int i)
{
	bool bReturn = m_pListBox->SetSelIndex(i);
	if(bReturn==true) SetText(GetSelItemString());
	return bReturn;
}

const char* MComboBox::GetSelItemString()
{
	return m_pListBox->GetSelItemString();
}

MListItem* MComboBox::GetSelItem()
{
	return m_pListBox->GetSelItem();
}

void MComboBox::SetNextSel()
{
	int cnt = GetCount();
	int sel = GetSelIndex();

	if(cnt > 1) {
		if(sel+1==cnt) {
			SetSelIndex(0);
		}
		else {
			SetSelIndex(sel+1);
		}

		OnSelectedItemChanged();
	}
}

void MComboBox::SetPrevSel()
{
	int cnt = GetCount();
	int sel = GetSelIndex();

	if(cnt > 1) {
		if(sel==0) {
			SetSelIndex(cnt-1);
		} else {
			SetSelIndex( sel-1 );
		}

		OnSelectedItemChanged();
	}
}

bool MComboBox::OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs)
{
	// ����Ʈ �ڽ��� ���� ������ �׳� ����
	if ( m_pListBox == NULL)
		return true;


	// ��ư �޽����� ���...
	if ( pWidget == this)
	{
		if ( nMsg == MEVENT_BUTTON_CLICK)
		{
			// üũ���� Ŭ���̺�Ʈ�� ���� ���� ������
			// ���� üũ ���¸� false�� �˻��Ѵ�.
			if ( GetCheck() == false)
				m_pListBox->SetFocus();
			return true;
		}
		return false;
	}


	// ����Ʈ�ڽ� �޽����� ���...
	else if ( pWidget == m_pListBox)
	{
		// ���콺�� ���� ����
		if ( nMsg == MEVENT_LB_ITEM_SEL || nMsg == MEVENT_LB_ITEM_SEL2 || nMsg == MEVENT_LB_ITEM_DBLCLK)
		{
			// ��ư�� üũ ���¸� �����Ѵ�
			SetCheck( false);


			// ��ư�� �ؽ�Ʈ�� �����Ѵ�
			SetText( m_pListBox->GetSelItemString());


			// �����ʿ� Ŀ�ǵ� ����
			OnSelectedItemChanged();
		}
		
		return true;
	}


	return false;


/* 	if(pWidget==this && nMsg == MEVENT_BUTTON_CLICK) {

		if( GetComboDropped() ) 
		{
			m_pListBox->Show(false);
			SetComboDropped(false);
			ReleaseFocus();
			return true;
		}
		
		bool bSCheck = false;

 		if(m_nComboType == 0) { // 2�κ����� ������ó��

			if( GetNextComboBoxTypeSize() > m_ClickPos.x ) {
				bSCheck = true;
			}
		}

		// DropHeight�� �˳��ϸ� �ڵ����� ũ�⸦ ���δ�.

		if(bSCheck) {

			int cnt = GetCount();
			int sel = GetSelIndex();

			if(cnt > 1) {
				if(sel+1==cnt) {
					SetSelIndex(0);
				}
				else {
					SetSelIndex(sel+1);
				}

				if(m_pComboBoxListener!=NULL){
					m_pComboBoxListener->OnCommand(this, MEVENT_CB_CHANGED);
				}

			}
		}
		else {
		
			int nFrameHeight = m_pListBox->GetRect().h - m_pListBox->GetClientRect().h;
 			int nDropHeight	 = min(m_nDropHeight,m_pListBox->GetItemHeight()*m_pListBox->GetCount()+nFrameHeight);

			bool bDropUnder = true;

			MPOINT sp = MClientToScreen(this,MPOINT(r.x,r.y+r.h));

			if( ( GetRect().y + GetRect().h + nDropHeight + 10) > MGetWorkspaceHeight())
				bDropUnder = false;

			if(m_bAutoDrop)
			{
				if(bDropUnder==true)	m_pListBox->SetBounds(MRECT(r.x, r.y+r.h, r.w, nDropHeight));
				else					m_pListBox->SetBounds(MRECT(r.x, r.y-nDropHeight, r.w, nDropHeight));
			}
			else
			{
				if(m_bDropUnder)	m_pListBox->SetBounds(MRECT(r.x, r.y+r.h, r.w, nDropHeight));
				else				m_pListBox->SetBounds(MRECT(r.x, r.y-nDropHeight, r.w, nDropHeight));
			}

			m_pListBox->Show(true, true);
			SetComboDropped( true );
		}

		return true;
	}
	else if(pWidget==m_pListBox) {

		if(nMsg == MEVENT_LB_ITEM_SEL || 
		   nMsg == MEVENT_LB_ITEM_SEL2 || 
		   nMsg == MEVENT_LB_ITEM_DBLCLK)
		{
			m_pListBox->Show(false);
			SetComboDropped( false );
			SetText(m_pListBox->GetSelItemString());
			if(m_pComboBoxListener!=NULL){
				m_pComboBoxListener->OnCommand(this, MEVENT_CB_CHANGED);
			}
			return true;
		}
	}

	return false;
*/
}

void MComboBox::Sort()
{
	m_pListBox->Sort();
}

void MComboBox::SetListener(MListener* pListener)
{
	m_pComboBoxListener = pListener;
}
MListener* MComboBox::GetListener()
{
	return MWidget::GetListener();
}

void MComboBox::OnReleaseFocus()
{
//	if( GetCheck() && !m_pListBox->IsFocus())
//		SetCheck( false);

/*
	if( !m_pListBox->IsFocus() && GetComboDropped() )
	{
		SetComboDropped(false);
		m_pListBox->Show(false);
	}
*/
}


// Runtime Loop
void MComboBox::OnRun(void)
{
	// ����Ʈ�ڽ��� ���� ������ ����
	if ( !m_pListBox)
		return;


	// �޺� �ڽ��� ����Ʈ �ڽ��� ������ ������ ����
	// - �޺� �ڽ��� Ŭ������ �� ����Ʈ�� ������ �ʾ� �ּ�ó�� �մϴ�. - birdkr
	//if ( !IsVisible() || !m_pListBox->IsVisible())
	//{
	//	m_pListBox->SetOpacity( 1.0f);
	//	return;
	//}


	// 10ms���� ����
	DWORD curr = timeGetTime();
	DWORD diff = curr - m_dwFadeTimer;

	if ( diff < 10)
		return;

	m_dwFadeTimer = curr;


	// ����Ʈ �ڽ��� ��Ŀ���� ������ �����
	if ( GetCheck() && !m_pListBox->IsFocus())
		SetCheck( false);


	// ����Ʈ �ڽ��� ��ġ �� ����� ���� �Ѵ�
	if ( m_pListBox->IsVisible())
	{
		MRECT rect = GetRect();
		if ( m_pParent && ((rect.y + rect.h + m_nDropHeight) > m_pParent->GetClientRect().h))
			rect.y -= m_nDropHeight;
		else
			rect.y += rect.h;

		rect.h = m_nDropHeight;
		m_pListBox->SetBounds( rect);
	}


	// ����Ʈ �ڽ��� ���̵� ��Ų��
	float delta  = (float)diff * FADE_FACTOR;
	float fFade = m_pListBox->GetOpacity() + ( GetCheck() ? delta : -delta);

	if ( fFade > 1.0f)			fFade = 1.0f;
	else if ( fFade < 0.0f)		fFade = 0.0f;

	m_pListBox->SetOpacity( fFade);

	bool bShow = (fFade > 0.0f) ? true : false;

	if (bShow == false)
	{
		m_pListBox->Show( false );
	}
	else
	{
		if (m_pListBox->IsVisible() == false)
		{
			m_pListBox->Show( true, true );
		}
	}
}


// ��ƿ� ����ϴ� �Լ�
void MComboBox::glueAdd(const char* szItem)
{
	if (szItem == NULL)
	{
		return;
	}
	Add(szItem);
}

void MComboBox::glueRemove(int i)
{
	if (i > GetCount())
	{
		return;
	}
	Remove(i);
}

void MComboBox::glueRemoveAll()
{
	RemoveAll();
}

int MComboBox::glueGetCount()
{
	return GetCount();
}

const char* MComboBox::glueGetSelectedText()
{
	return GetSelItemString();
}

int MComboBox::glueGetSelelectedIndex()
{
	return GetSelIndex();
}

void MComboBox::glueSetSelectedIndex(int nIndex)
{
	SetSelIndex(nIndex);
}

void MComboBox::OnSelectedItemChanged()
{
	if(m_pComboBoxListener!=NULL)
	{
		m_pComboBoxListener->OnCommand(this, MEVENT_CB_SELECTED_INDEX_CHANGED);
	}
	m_EventHandler.OnCommand(this, MEVENT_CB_SELECTED_INDEX_CHANGED);
}

} // namespace mint3