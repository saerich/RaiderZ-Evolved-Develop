#pragma once

#include "MWidget.h"
#include "MLookBase.h"


// namespace : mint3
namespace mint3
{


class MEdit;
class MSpinCtrl;



// SpinCtrl Ÿ��
enum MSpinCtrlTypes
{
	MSPT_VERTICAL		= 0,		// ������
	MSPT_HORIZONTAL,				// ������
};


// ������Ʈ�� ����
enum MSPINCTRL_PART
{
	MSPP_NONE			= -1,
	MSPP_UPARROW		= 0,		// �� ȭ��ǥ ����
	MSPP_DOWNARROW,					// �Ʒ� ȭ��ǥ ����
	MSPP_LEFTARROW,					// ���� ȭ��ǥ ����
	MSPP_RIGHTARROW,				// ������ ȭ��ǥ ����
	MSPP_MAX
};






/// class : MSpinCtrlLook
class MSpinCtrlLook : public MLookBase
{
/// Member variables
protected:
	int			m_nWidth;


/// Member functions
public:
	MSpinCtrlLook();

	void SetWidth( int width)				{ m_nWidth = width;				}
	int GetWidth()							{ return m_nWidth;				}

	virtual void OnDrawBorder( MWidget* pWidget, MDrawContext* pDC) override;
	virtual void OnDraw( MWidget* pWidget, MDrawContext* pDC) override;

protected:
	virtual void OnDrawUpArrow( MSpinCtrl* pSpin, MRECT& r, MDrawContext* pDC);
	virtual void OnDrawDownArrow( MSpinCtrl* pSpin, MRECT& r, MDrawContext* pDC);
	virtual void OnDrawLeftArrow( MSpinCtrl* pSpin, MRECT& r, MDrawContext* pDC);
	virtual void OnDrawRightArrow( MSpinCtrl* pSpin, MRECT& r, MDrawContext* pDC);
};







/// class : MSpinCtrl
class MSpinCtrl : public MWidget
{
/// Member variables
protected:
	int					m_nType;
	MEdit*				m_pBuddy;
	int					m_nMouseOver;
	int					m_nBtnPressed;
	MPOINT				m_ptCursorPos;
	DWORD				m_dwTimer;



/// Member functions
public:
	MSpinCtrl( const char* szName, MWidget* pParent =NULL, MListener* pListener =NULL);

	const char* GetButtonState( int button);

	void SetType( int type)						{ m_nType = type;				}
	int GetType()								{ return m_nType;				}

	void SetBuddy( MWidget* pBuddy);

	int GetWidth()								{ return (HasLookAndFill() ? ((MSpinCtrlLook*)m_pLookAndFill)->GetWidth() : 15);	}


	MRECT GetUpArrowRgn();
	MRECT GetDownArrowRgn();
	MRECT GetLeftArrowRgn();
	MRECT GetRightArrowRgn();



/// Event handler
protected:
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener) override;
	virtual void OnRun( DWORD dwCurrTime) override;
	virtual void OnSetFocus() override;
	virtual void OnLeave() override;
	virtual bool OnSetCursor( MEvent* pEvent, string* pCursor) override;



#define MINT_SPINCTRL	"SpinCtrl"
	virtual const char* GetClassName()			{ return MINT_SPINCTRL;			}
};



} // namespace mint3