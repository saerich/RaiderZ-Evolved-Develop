/************************************************************************
MLayer

Prog. : Lim Dong Hwan
Date  : 29.JUN.2009
Desc. : Layer ���� Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MLayer.h"
#include "Mint.h"


// namespace : mint3
namespace mint3
{

/*	MLayer
	������
*/
MLayer::MLayer( const char* szName /*=NULL*/, MWidget* pParent /*=NULL*/, MListener* pListener /*=NULL*/)
: MWidget( szName, pParent, pListener)
{
	m_Rect = MRECT( 0, 0, MGetMint()->GetStandardWidth(), MGetMint()->GetStandardHeight());

	SetAnchor( MAnchorStyle( true, true, true, true));
}


// IsLayered
bool MLayer::IsLayered() const
{
	if ( m_pParent == NULL)			return false;
	if ( m_pParent->IsLayered())	return true;
	return false;
}


/*	OnEvent
	OnEvent �̺�Ʈ �ڵ鷯
*/
bool MLayer::OnEvent( MEvent* pEvent, MListener* pListener)
{
	return false;
}


/*	OnDraw
	OnDraw �̺�Ʈ �ڵ鷯
*/
void MLayer::OnDraw( MDrawContext* pDC)
{
}


/*	OnSize
	OnSize �̺�Ʈ �ڵ鷯
*/
void MLayer::OnSize(int w, int h)
{
	MWidget::OnSize( w, h);
}


} // namespace mint3
