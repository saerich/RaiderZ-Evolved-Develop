#pragma once

#include "MWidget.h"
#include "MDrawContext.h"
#include "MPanel.h"
#include "MScrollBar.h"


namespace mint3
{
	class MView;



/// View
class MView : public MWidget
{
protected:
	MPanel*			m_pPanel;
	MScrollBar*		m_pScrollbarVer;


public:
	MView( const char* szName =NULL, MWidget* pParent =NULL, MListener* pListener =NULL);
	virtual ~MView();

	/// Panel ������ ����
	MPanel* GetPanel()									{ return m_pPanel;		}

	/// ��ũ�ѹ� �β� ����
	void SetScrollBarWidth( int width)					{ m_pScrollbarVer->SetWidth( width);			}

	/// ��ũ�ѹ� Look ����
	void SetScrollBarLook( MScrollBarLook* pLook)		{ m_pScrollbarVer->ChangeCustomLook( pLook);	}


protected:
	virtual void OnDraw( MDrawContext* pDC);
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener);



#define MINT_VIEW	"View"
	virtual const char* GetClassName()					{ return MINT_VIEW;		}
};

} // namespace mint3
