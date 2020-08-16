#include "stdafx.h"
#include "MScrollBar.h"
#include "MColorTable.h"


#define MSCROLLBAR_NAME				"Scroll Bar"



namespace mint3
{

IMPLEMENT_LOOK(MScrollBar, MScrollBarLook)



/**	OnDraw
	��ο� �Լ�
*/
void MScrollBarLook::OnDraw( MScrollBar* pScrollBar, MDrawContext* pDC)
{
	// ����� �׸���
	pDC->SetColor( 80, 80, 80);
	pDC->FillRectangle( pScrollBar->GetClientRect());


	// �� ���� ȭ��ǥ ��ư�� �׸���
	if ( pScrollBar->IsShowArrow())
	{
		if ( pScrollBar->GetType() == MSBT_VERTICAL)
		{
			// �� ȭ��ǥ ��ư�� �׸���
			pDC->SetColor( 160, 160, 160);
			pDC->FillRectangle( pScrollBar->GetUpArrowRgn());


			// �Ʒ� ȭ��ǥ ��ư�� �׸���
			pDC->SetColor( 160, 160, 160);
			pDC->FillRectangle( pScrollBar->GetDownArrowRgn());
		}
		else
		{
			// ���� ȭ��ǥ ��ư�� �׸���
			pDC->SetColor( 160, 160, 160);
			pDC->FillRectangle( pScrollBar->GetLeftArrowRgn());


			// ������ ȭ��ǥ ��ư�� �׸���
			pDC->SetColor( 160, 160, 160);
			pDC->FillRectangle( pScrollBar->GetRightArrowRgn());
		}
	}


	// Thumb�� �׸���
	pDC->SetColor( 255, 255, 255);
	pDC->FillRectangle( pScrollBar->GetThumbRgn());
}


/**	GetClientRect
	Ŭ���̾�Ʈ ������ ���ϴ� �Լ�
*/
MRECT MScrollBarLook::GetClientRect( MScrollBar* pScrollBar, MRECT& r)
{
	return r;
}








/**	MScrollBar
	������
*/
MScrollBar::MScrollBar( const char* szName, MWidget* pParent, MListener* pListener)
: MWidget(szName, pParent, pListener)
{
	LOOK_IN_CONSTRUCTOR()
		
	m_nType			= MSBT_HORIZONTAL;
	m_bShowArrow	= true;
	m_nValue		= 0;
	m_nRangeMin		= 0;
	m_nRangeMax		= 100;
	m_nWidth		= 15;
	m_nPushedRgn	= MSBR_NONE;
}


/**	MScrollBar
	������
*/
MScrollBar::MScrollBar( MWidget* pParent, MListener* pListener, MScrollBarTypes type)
: MWidget( MSCROLLBAR_NAME, pParent, pListener)
{
	LOOK_IN_CONSTRUCTOR()

	m_nType			= type;
	m_bShowArrow	= true;
	m_nValue		= 0;
	m_nRangeMin		= 0;
	m_nRangeMax		= 100;
	m_nWidth		= 15;
	m_nPushedRgn	= MSBR_NONE;
}


/**	~MScrollBar
	�Ҹ���
*/
MScrollBar::~MScrollBar()
{
}


/**	OnEvent
	OnEvent �޽��� �ڵ鷯
*/
bool MScrollBar::OnEvent(MEvent* pEvent, MListener* pListener)
{
	if ( MWidget::OnEvent( pEvent, pListener) == true)
		return true;


	// �θ� ������ �Ⱥ��̰ų� ����� �� ������ ���� ����
	if ( m_pParent && ( !m_pParent->IsEnable() || !m_pParent->IsVisible()) || !IsEnable())
	{
		m_nPushedRgn = MSBR_NONE;
		return false;
	}


	// �޽��� �˻�
	switch ( pEvent->nMessage)
	{
		case MWM_LBUTTONDOWN:
		{
			MRECT rect = GetInitialClientRect();

			m_nPushedRgn = MSBR_NONE;


			// ȭ��ǥ ��ư�� ���̴��� Ȯ��
			if ( m_bShowArrow)
			{
				// Vertical
				if ( m_nType == MSBT_VERTICAL)
				{
					// ���� ȭ��ǥ �������� Ȯ��
					if ( GetUpArrowRgn().InPoint( pEvent->Pos) == true)
						m_nPushedRgn = MSBR_UPARROW;

					// �Ʒ��� ȭ��ǥ �������� Ȯ��
					else if ( GetDownArrowRgn().InPoint( pEvent->Pos) == true)
						m_nPushedRgn = MSBR_DOWNARROW;
				}

				// Horizon
				else
				{
					// ���� ȭ��ǥ �������� Ȯ��
					if ( GetLeftArrowRgn().InPoint( pEvent->Pos) == true)
						m_nPushedRgn = MSBR_LEFTARROW;

					// ������ ȭ��ǥ �������� Ȯ��
					else if ( GetRightArrowRgn().InPoint( pEvent->Pos) == true)
						m_nPushedRgn = MSBR_RIGHTARROW;
				}
			}
			
			
			if ( m_nPushedRgn == MSBR_NONE)
			{
				// Thumb �������� Ȯ��
				if ( GetThumbRgn().InPoint( pEvent->Pos) == true)
					m_nPushedRgn = MSBR_THUMB;

				// ��� �������� Ȯ��
				else if ( rect.InPoint( pEvent->Pos) == true)
					m_nPushedRgn = MSBR_BKGRND;
			}


			// ���� ��ġ ����
			m_ptCursorPos = pEvent->Pos;


			// �ð� �缳��
			m_dwTimer = timeGetTime() - 1000;

			// �޽����� ó���Ǿ��ٰ� ����
			if(rect.InPoint(pEvent->Pos) )
				return true;

			break;
		}


		case MWM_LBUTTONUP:
			m_nPushedRgn = MSBR_NONE;
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
void MScrollBar::OnSize( int w, int h)
{
}


/**	OnRun
	OnRun �޽��� �ڵ鷯
*/
void MScrollBar::OnRun(void)
{
	if ( !IsVisible())
		return;


	// 20ms���� ����
	DWORD curr = timeGetTime();
	DWORD diff = curr - m_dwTimer;

	if ( diff < 20)
		return;

	m_dwTimer = curr;


	// �����ʸ� ����
	MListener* pListener = GetListener();



	// ���õ� ������ Ȯ����
	switch ( m_nPushedRgn)
	{
		// ���ÿ��� ����
		case MSBR_NONE :
			break;


		// �� ȭ��ǥ ����
		case MSBR_UPARROW :
		// ���� ȭ��ǥ ����
		case MSBR_LEFTARROW :
		{
			// �� ����
			m_nValue--;
			if ( m_nValue < m_nRangeMin)
			{
				m_nValue = m_nRangeMin;
				break;
			}


			// �޽��� ����
			if ( pListener)
				pListener->OnCommand( this, MEVENT_LIST_VALUE_CHANGED);

			break;
		}


		// �Ʒ� ȭ��ǥ ����
		case MSBR_DOWNARROW :
		// ������ ȭ��ǥ ����
		case MSBR_RIGHTARROW :
		{
			// �� ����
			m_nValue++;
			if ( m_nValue > m_nRangeMax)
			{
				m_nValue = m_nRangeMax;
				break;
			}


			// �޽��� ����
			if ( pListener)
				pListener->OnCommand( this, MEVENT_LIST_VALUE_CHANGED);

			break;
		}
		

		// Thumb ����
		case MSBR_THUMB :
		// ��� ����
		case MSBR_BKGRND :
		{
			// Ŀ�� ��ǥ�κ��� ���� ����
			SetValue( m_ptCursorPos);


			// �޽��� ����
			if ( pListener)
				pListener->OnCommand( this, MEVENT_LIST_VALUE_CHANGED);

			break;
		}
	}


	// �θ� �����ʰ� ������ ����
	if ( pListener == NULL)
	{
		MRECT rect = GetClientRect();

	
		if ( m_nType == MSBT_VERTICAL)
			m_nWidth = rect.w;
		else
			m_nWidth = rect.h;

		return;
	}


	// �θ� ������ ũ�⿡ ���缭 ����� �����Ѵ�. ������ ��ġ...
	if ( m_pParent)
	{
		MRECT r = m_pParent->GetClientRect();
		if ( m_nType == MSBT_VERTICAL)
			SetBounds( MRECT(r.x + r.w - m_nWidth, r.y, m_nWidth, r.h));
		else
			SetBounds( MRECT(r.x, r.y + r.h - m_nWidth, r.w, m_nWidth));
	}
}


/**	SetRange
	��ũ�ѹ��� �ִ�/�ּ� ������ �����ϰų� ���ϴ� �Լ�
*/
void MScrollBar::SetRange( int min, int max)
{
	if ( min > max)
		return;


	m_nRangeMin = min;
	m_nRangeMax = max;


	if ( m_nValue < min)			m_nValue = m_nRangeMin;
	else if ( m_nValue > max)		m_nValue = m_nRangeMax;
}


/**	SetValue
	���� �����ϴ� �Լ�
*/
void MScrollBar::SetValue( int nValue)
{
	if ( nValue < m_nRangeMin)			nValue = m_nRangeMin;
	else if ( nValue > m_nRangeMax)		nValue = m_nRangeMax;

	m_nValue = nValue;
}


/**	SetValue
	��ǥ�κ��� ���� �����ϴ� �Լ�
*/
void MScrollBar::SetValue( MPOINT point)
{
	MRECT r = GetClientRect();


	// Thumb�� �̵��� ���̸� ���Ѵ�
	int length = (m_nType == MSBT_VERTICAL) ? r.h : r.w;
	length -= m_nWidth;


	// ȭ��ǥ ��ư ��ŭ ���̸� ���ش�
	if ( m_bShowArrow)
		length -= m_nWidth + m_nWidth;


	// ���� ���Ѵ�
	int nValue;

	if ( m_nType == MSBT_VERTICAL)
		nValue = (int)( (float)((point.y - (m_nWidth / 2) - r.y - (m_bShowArrow ? m_nWidth : 0)) * (m_nRangeMax - m_nRangeMin)) / (float)length + 0.5f) + m_nRangeMin;
	else
		nValue = (int)( (float)((point.x - (m_nWidth / 2) - r.x - (m_bShowArrow ? m_nWidth : 0)) * (m_nRangeMax - m_nRangeMin)) / (float)length + 0.5f) + m_nRangeMin;


	// ���� �����Ѵ�
	SetValue( nValue);
}


/**	GetUpArrowRgn
	�� ȭ��ǥ ��ư�� ������ ���Ѵ�
*/
MRECT MScrollBar::GetUpArrowRgn()
{
	MRECT rect = GetClientRect();
	rect.h = m_nWidth;

	return rect;
}


/**	GetDownArrowRgn
	�Ʒ� ȭ��ǥ ��ư�� ������ ���Ѵ�
*/
MRECT MScrollBar::GetDownArrowRgn()
{
	MRECT rect = GetClientRect();
	rect.y += rect.h - m_nWidth;
	rect.h = m_nWidth;

	return rect;
}


/**	GetLeftArrowRgn
	���� ȭ��ǥ ��ư�� ������ ���Ѵ�
*/
MRECT MScrollBar::GetLeftArrowRgn()
{
	MRECT rect = GetClientRect();
	rect.w = m_nWidth;

	return rect;
}


/**	GetRightArrowRgn
	������ ȭ��ǥ ��ư�� ������ ���Ѵ�
*/
MRECT MScrollBar::GetRightArrowRgn()
{
	MRECT rect = GetClientRect();
	rect.x += rect.w - m_nWidth;
	rect.w = m_nWidth;

	return rect;
}


/**	GetThumbRgn
	Thumb ��ư�� ������ ���Ѵ�
*/
MRECT MScrollBar::GetThumbRgn()
{
	MRECT r = GetClientRect();


	// Thumb�� �̵��� ���̸� ���Ѵ�
	int length = (m_nType == MSBT_VERTICAL) ? r.h : r.w;
	length -= m_nWidth;


	// ȭ��ǥ ��ư ��ŭ ���̸� ���ش�
	if ( m_bShowArrow)
		length -= m_nWidth + m_nWidth;


	// ��ġ ��밪�� ���Ѵ�
	float fRatio = (float)( m_nValue - m_nRangeMin) / (float)( m_nRangeMax - m_nRangeMin);


	// Thumb�� ��ġ�� ���Ѵ�
	MRECT rect;

	// �������̸�...
	if ( m_nType == MSBT_VERTICAL)
	{
		rect.x = r.x;
		rect.y = r.y + (int)( (float)length * fRatio + 0.5f) + ( m_bShowArrow ? m_nWidth : 0);
		rect.w = r.w;
		rect.h = m_nWidth;
	}

	// �������̸�...
	else			
	{
		rect.x = r.x + (int)( (float)length * fRatio + 0.5f) + ( m_bShowArrow ? m_nWidth : 0);
		rect.y = r.y;
		rect.w = m_nWidth;
		rect.h = r.h;
	}


	return rect;
}


/**	GetBackgroundRgn
	��� ������ ���Ѵ�
*/
MRECT MScrollBar::GetBackgroundRgn()
{
	MRECT rect = GetClientRect();

	
	// ȭ��ǥ�� �Ⱥ��̸� �ٷ� ����
	if ( !m_bShowArrow)
		return rect;


	// �������̸�...
	if ( m_nType == MSBT_VERTICAL)
	{
		rect.y += m_nWidth;
		rect.h -= m_nWidth + m_nWidth;
	}

	// �������̸�...
	else
	{
		rect.x += m_nWidth;
		rect.w -= m_nWidth + m_nWidth;
	}


	return rect;
}


/*	IsEnable
	��� ������ �������� ���θ� ���ϴ� �Լ�
*/
bool MScrollBar::IsEnable()	
{
	if ( m_pParent && GetListener())
		return m_pParent->IsEnable();


	return MWidget::IsEnable();
}



/*	IsFocus
	��Ŀ���� ���� �������� ���θ� ���ϴ� �Լ�
*/
bool MScrollBar::IsFocus()
{
	if ( m_pParent)
		return m_pParent->IsFocus();


	return MWidget::IsFocus();
}

void MScrollBar::MoveEndLine()
{
	SetValue(m_nRangeMax);

	MListener* pListener = GetListener();
	if(pListener!=NULL) pListener->OnCommand(this, MEVENT_LIST_VALUE_CHANGED);
}

} // namespace mint3
