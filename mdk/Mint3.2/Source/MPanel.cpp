#include "stdafx.h"
#include "MPanel.h"
#include <math.h>


namespace mint3
{


// MPanel
MPanel::MPanel( const char* szName, MWidget* pParent, MListener* pListener)
: MWidget( szName, pParent, pListener)
{
	m_bStatic = true;
}


// OnEvent
bool MPanel::OnEvent( MEvent* pEvent, MListener* pListener)
{
	return MWidget::OnEvent( pEvent, pListener);
}


// OnDraw
void MPanel::OnDraw( MDrawContext* pDC)
{
	MWidget::OnDraw( pDC);

#ifdef _DEBUG
	// ������ �ؽ�Ʈ�� "show rect"�̸� �ش� ������ ������ ǥ���Ѵ�
	if ( m_strText == string( "show rect"))
	{
		pDC->SetColor( 255, 255, 255, (int)(25.0 * sin( (double)timeGetTime() * 0.005) + 25.0));
		pDC->Rectangle( GetClientRect());
	}
#endif
}


} // namespace mint3