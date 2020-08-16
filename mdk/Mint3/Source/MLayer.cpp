/************************************************************************
MLayer

Prog. : Lim Dong Hwan
Date  : 29.JUN.2009
Desc. : Layer ���� Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MLayer.h"



// namespace : mint3
namespace mint3
{

/*	MLayer
	������
*/
MLayer::MLayer( const char* szName /*=NULL*/, MWidget* pParent /*=NULL*/, MListener* pListener /*=NULL*/)
: MWidget( szName, pParent, pListener)
{
	Enable( true);
	Show( true);
	SetAnchor( MAnchorStyle( true, true, true, true));
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
