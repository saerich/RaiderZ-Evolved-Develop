#pragma once

#include "MScrollBar.h"


// namespace : mint3
namespace mint3
{

#define MINT_SLIDER	"Slider"


// �����̴� Ŭ���� ����
class MSlider : public MScrollBar
{
// Member functions
public:
	// ������
	MSlider( const char* szName, MWidget* pParent=NULL, MListener* pListener=NULL);
};

} // namespace mint3
