#pragma once

#include "MWidget.h"
#include "MDrawContext.h"
#include "MLookNFeel.h"

namespace mint3 {

/// ��Ʈ������ �׸� ���̺�
/// ��Ʈ�ʿ� 0~9 , A ~ Z , a ~ z������ �׷��� �־�� �Ѵ�.
class MBmLabel : public MWidget{
protected:
	MBitmap*		m_pLabelBitmap;
	MSIZE			m_CharSize;

	virtual void OnDraw(MDrawContext* pDC);
public:
	MBmLabel(const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);

	void SetLabelBitmap(MBitmap* pLabelBitmap);
	void SetCharSize(MSIZE &size);
#define MINT_BMLABEL	"BmLabel"
	virtual const char* GetClassName(void){ return MINT_BMLABEL; }
};


} // namespace mint3