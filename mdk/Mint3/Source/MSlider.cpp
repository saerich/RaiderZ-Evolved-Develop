#include "stdafx.h"
#include "MSlider.h"


namespace mint3
{

/**	MSlider
	������
*/
MSlider::MSlider( const char* szName, MWidget* pParent, MListener* pListener)
: MScrollBar(szName, pParent, pListener)
{
	m_bShowArrow = false;
}

} // namespace mint3