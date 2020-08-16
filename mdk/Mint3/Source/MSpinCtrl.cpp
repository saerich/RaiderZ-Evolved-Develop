#include "stdafx.h"
#include "MSpinCtrl.h"
#include "MColorTable.h"


#define MSPINCTRL_NAME				"Spin Ctrl"



namespace mint3
{

IMPLEMENT_LOOK(MSpinCtrl, MSpinCtrlLook)



/**	OnDraw
	��ο� �Լ�
*/
void MSpinCtrlLook::OnDraw( MSpinCtrl* pSpinCtrl, MDrawContext* pDC)
{
	// ����� �׸���
	pDC->SetColor( 80, 80, 80);
	pDC->FillRectangle( pSpinCtrl->GetClientRect());


	// �� ���� ȭ��ǥ ��ư�� �׸���
	if ( pSpinCtrl->GetType() == MSPT_VERTICAL)
	{
		// �� ȭ��ǥ ��ư�� �׸���
		pDC->SetColor( 160, 160, 160);
		pDC->FillRectangle( pSpinCtrl->GetUpArrowRgn());


		// �Ʒ� ȭ��ǥ ��ư�� �׸���
		pDC->SetColor( 160, 160, 160);
		pDC->FillRectangle( pSpinCtrl->GetDownArrowRgn());
	}
	else
	{
		// ���� ȭ��ǥ ��ư�� �׸���
		pDC->SetColor( 160, 160, 160);
		pDC->FillRectangle( pSpinCtrl->GetLeftArrowRgn());


		// ������ ȭ��ǥ ��ư�� �׸���
		pDC->SetColor( 160, 160, 160);
		pDC->FillRectangle( pSpinCtrl->GetRightArrowRgn());
	}
}


/**	GetClientRect
	Ŭ���̾�Ʈ ������ ���ϴ� �Լ�
*/
MRECT MSpinCtrlLook::GetClientRect( MSpinCtrl* pSpinCtrl, MRECT& r)
{
	return r;
}








/**	MSpinCtrl
	������
*/
MSpinCtrl::MSpinCtrl( const char* szName, MWidget* pParent, MListener* pListener)
: MWidget(szName, pParent, pListener)
{
	LOOK_IN_CONSTRUCTOR()
		
	m_nType			= MSPT_VERTICAL;
	m_nPushedRgn	= MSPR_NONE;
	m_pBuddy		= NULL;
}


/**	~MSpinCtrl
	�Ҹ���
*/
MSpinCtrl::~MSpinCtrl()
{
}


/**	OnEvent
	OnEvent �޽��� �ڵ鷯
*/
bool MSpinCtrl::OnEvent(MEvent* pEvent, MListener* pListener)
{
	if ( MWidget::OnEvent( pEvent, pListener) == true)
		return true;


	// �θ� ������ �Ⱥ��̰ų� ����� �� ������ ���� ����
	if ( m_pParent && ( !m_pParent->IsEnable() || !m_pParent->IsVisible()) || !IsEnable())
	{
		m_nPushedRgn = MSPR_NONE;
		return false;
	}


	// �޽��� �˻�
	switch ( pEvent->nMessage)
	{
		case MWM_LBUTTONDOWN:
		{
			MRECT rect = GetInitialClientRect();

			m_nPushedRgn = MSPR_NONE;


			// Vertical
			if ( m_nType == MSPT_VERTICAL)
			{
				// ���� ȭ��ǥ �������� Ȯ��
				if ( GetUpArrowRgn().InPoint( pEvent->Pos) == true)
					m_nPushedRgn = MSPR_UPARROW;

				// �Ʒ��� ȭ��ǥ �������� Ȯ��
				else if ( GetDownArrowRgn().InPoint( pEvent->Pos) == true)
					m_nPushedRgn = MSPR_DOWNARROW;
			}

			// Horizon
			else
			{
				// ���� ȭ��ǥ �������� Ȯ��
				if ( GetLeftArrowRgn().InPoint( pEvent->Pos) == true)
					m_nPushedRgn = MSPR_LEFTARROW;


				// ������ ȭ��ǥ �������� Ȯ��
				else if ( GetRightArrowRgn().InPoint( pEvent->Pos) == true)
					m_nPushedRgn = MSPR_RIGHTARROW;
			}



			// �� ����
			if ( (m_nPushedRgn == MSPR_UPARROW) || (m_nPushedRgn == MSPR_RIGHTARROW))
			{
				if ( m_pBuddy)
				{
					int num = atoi( m_pBuddy->GetText());
					num++;

					char szTmp[ 256];
					_itoa( num, szTmp, 10);
					m_pBuddy->SetText( szTmp);
				}
			}

			// �� ����
			else if ( (m_nPushedRgn == MSPR_DOWNARROW) || (m_nPushedRgn == MSPR_LEFTARROW))
			{
				if ( m_pBuddy)
				{
					int num = atoi( m_pBuddy->GetText());
					num--;

					if ( num < 0)
						num = 0;

					char szTmp[ 256];
					_itoa( num, szTmp, 10);
					m_pBuddy->SetText( szTmp);
				}
			}
			
			
			// ���� ��ġ ����
			m_ptCursorPos = pEvent->Pos;


			// �ð� �缳��
			m_dwTimer = timeGetTime();


			break;
		}


		case MWM_LBUTTONUP:
			m_nPushedRgn = MSPR_NONE;
			break;


		case MWM_MOUSEMOVE:
			m_ptCursorPos = pEvent->Pos;


		default :
			break;
	}


	return false;
}


/**	OnSize
	OnSize �޽��� �ڵ鷯
*/
void MSpinCtrl::OnSize( int w, int h)
{
}


/**	OnRun
	OnRun �޽��� �ڵ鷯
*/
void MSpinCtrl::OnRun(void)
{
	// 250ms���� ����
	DWORD curr = timeGetTime();
	DWORD diff = curr - m_dwTimer;

	if ( diff < 250)
		return;

	m_dwTimer = curr;


	// �����ʸ� ����
	MListener* pListener = GetListener();



	// ���õ� ������ Ȯ����
	switch ( m_nPushedRgn)
	{
		// ���ÿ��� ����
		case MSPR_NONE :
			break;


		// �� ȭ��ǥ ����
		case MSPR_UPARROW :
		// ���� ȭ��ǥ ����
		case MSPR_RIGHTARROW:
		{
			// �޽��� ����
			if ( pListener)
				pListener->OnCommand( this, MEVENT_LIST_VALUE_CHANGED);


			// �� ����
			if ( m_pBuddy)
			{
				int num = atoi( m_pBuddy->GetText());
				num++;

				char szTmp[ 256];
				_itoa( num, szTmp, 10);
				m_pBuddy->SetText( szTmp);
			}


			break;
		}


		// �Ʒ� ȭ��ǥ ����
		case MSPR_DOWNARROW :
		// ������ ȭ��ǥ ����
		case MSPR_LEFTARROW:
		{
			// �޽��� ����
			if ( pListener)
				pListener->OnCommand( this, MEVENT_LIST_VALUE_CHANGED);


			// �� ����
			if ( m_pBuddy)
			{
				int num = atoi( m_pBuddy->GetText());
				num--;

				if ( num < 0)
					num = 0;

				char szTmp[ 256];
				_itoa( num, szTmp, 10);
				m_pBuddy->SetText( szTmp);
			}


			break;
		}
	}


	// �θ� ������ ũ�⿡ ���缭 ����� �����Ѵ�. ������ ��ġ...
	if ( m_pBuddy)
	{
		MRECT r = m_pBuddy->GetRect();

		int width = GetClientRect().w;
		SetBounds( MRECT( r.x + r.w, r.y, width, r.h));
	}
}


/**	GetUpArrowRgn
	�� ȭ��ǥ ��ư�� ������ ���Ѵ�
*/
MRECT MSpinCtrl::GetUpArrowRgn()
{
	MRECT rect = GetClientRect();
	rect.h /= 2;

	return rect;
}


/**	GetDownArrowRgn
	�Ʒ� ȭ��ǥ ��ư�� ������ ���Ѵ�
*/
MRECT MSpinCtrl::GetDownArrowRgn()
{
	MRECT rect = GetClientRect();
	rect.y += rect.h / 2;
	rect.h = rect.h - (rect.h / 2);

	return rect;
}


/**	GetLeftArrowRgn
	���� ȭ��ǥ ��ư�� ������ ���Ѵ�
*/
MRECT MSpinCtrl::GetLeftArrowRgn()
{
	MRECT rect = GetClientRect();
	rect.w /= 2;

	return rect;
}


/**	GetRightArrowRgn
	������ ȭ��ǥ ��ư�� ������ ���Ѵ�
*/
MRECT MSpinCtrl::GetRightArrowRgn()
{
	MRECT rect = GetClientRect();
	rect.x += rect.w / 2;
	rect.w = rect.w - (rect.w / 2);

	return rect;
}


/*	IsEnable
	��� ������ �������� ���θ� ���ϴ� �Լ�
*/
bool MSpinCtrl::IsEnable()	
{
	if ( m_pParent && GetListener())
		return m_pParent->IsEnable();


	return MWidget::IsEnable();
}



/*	IsFocus
	��Ŀ���� ���� �������� ���θ� ���ϴ� �Լ�
*/
bool MSpinCtrl::IsFocus()
{
	if ( m_pParent && GetListener())
		return m_pParent->IsFocus();


	return MWidget::IsFocus();
}


} // namespace mint3
