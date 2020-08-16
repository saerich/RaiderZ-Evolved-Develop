#pragma once

#include "MWidget.h"
#include "MDrawContext.h"
#include "MLookNFeel.h"

namespace mint3 {

class MLabel;

/// Label�� Draw �ڵ尡 �ִ� Ŭ����, �� Ŭ������ ��ӹ޾Ƽ� Ŀ���ҷ��� ���� �� �ִ�.
class MLabelLook{
public:
	virtual void OnDraw(MLabel* pLabel, MDrawContext* pDC);
	virtual MRECT GetClientRect(MLabel* pLabel, MRECT& r);
};

/// Label
class MLabel : public MWidget{
protected:
	MCOLOR			m_TextColor;
	MAlignmentMode	m_AlignmentMode;

	DECLARE_LOOK(MLabelLook)
	DECLARE_LOOK_CLIENT()

protected:
	virtual bool OnEvent(MEvent* pEvent, MListener* pListener);

public:
	MLabel(const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);

	/// �ؽ�Ʈ �÷� ����
	virtual void SetTextColor(MCOLOR color);

	/// �ؽ�Ʈ �÷� ���
	MCOLOR GetTextColor(void);

	/// ���� ���
	MAlignmentMode GetAlignment(void);
	/// ���� ����
	virtual MAlignmentMode SetAlignment(MAlignmentMode am);

	void		glueSetTextColor(unsigned int color );

#define MINT_LABEL	"Label"
	virtual const char* GetClassName(void){ return MINT_LABEL; }
};

} // namespace mint3