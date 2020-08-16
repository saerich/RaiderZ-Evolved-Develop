#pragma once

#include "MWidget.h"
#include "MDrawContext.h"
#include "MLookNFeel.h"

namespace mint3 {

/// ��ũ��Ʈ���� ���� Draw�� ������ �� �ִ� ����
class MPaint : public MWidget{
protected:
	virtual void OnDraw(MDrawContext* pDC);
public:
	MPaint(const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);

#define MINT_PAINT	"Paint"
	virtual const char* GetClassName(void){ return MINT_PAINT; }
};

} // namespace mint3