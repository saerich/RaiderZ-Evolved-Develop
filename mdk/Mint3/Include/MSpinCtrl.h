#pragma once

#include "MWidget.h"
#include "MLookNFeel.h"


// namespace : mint3
namespace mint3
{


// Ŭ���� ����
class MListBox;



// SpinCtrl Ÿ��
enum MSpinCtrlTypes
{
	MSPT_VERTICAL		= 0,		// ������
	MSPT_HORIZONTAL,				// ������
};



// ������Ʈ�� ����
enum MSPINCTRL_REGION
{
	MSPR_NONE			= -1,
	MSPR_UPARROW		= 0,		// �� ȭ��ǥ ����
	MSPR_DOWNARROW,					// �Ʒ� ȭ��ǥ ����
	MSPR_LEFTARROW,					// ���� ȭ��ǥ ����
	MSPR_RIGHTARROW,				// ������ ȭ��ǥ ����
	MSPR_MAX
};



class MSpinCtrl;


// ������Ʈ�� Ŀ���� Look
class MSpinCtrlLook
{
public:
	virtual void OnDraw( MSpinCtrl* pSpinCtrl, MDrawContext* pDC);
	virtual MRECT GetClientRect( MSpinCtrl* pSpinCtrl, MRECT& r);
};



// ������Ʈ�� Ŭ���� ����
class MSpinCtrl : public MWidget
{
// friend ����
friend MListBox;


// Member variables
protected:
	// ������Ʈ�� Ÿ��
	MSpinCtrlTypes		m_nType;

	// ���� ����
	MWidget*			m_pBuddy;

	// ���콺�� ���� ��ġ
	MSPINCTRL_REGION	m_nPushedRgn;

	// Ŀ�� ��ǥ
	MPOINT				m_ptCursorPos;

	// Ÿ�̸�
	DWORD				m_dwTimer;



// Member functions
public:
	// ������
	MSpinCtrl( const char* szName, MWidget* pParent=NULL, MListener* pListener=NULL);

	// �Ҹ���
	virtual ~MSpinCtrl();


	// Ÿ���� �����ϰų� ���ϴ� �Լ�
	void SetType( MSpinCtrlTypes type)			{ m_nType = type;		}
	MSpinCtrlTypes GetType()					{ return m_nType;		}


	// ���� �����ϴ� �Լ�
	void SetBuddy( MWidget* pBuddy)				{ m_pBuddy = pBuddy;	}

	// ���콺�� ���� ������ ���ϴ� �Լ�
	MSPINCTRL_REGION GetPushedRgn()				{ return m_nPushedRgn;	}


	// �� ȭ��ǥ ��ư�� ������ ���Ѵ�
	MRECT GetUpArrowRgn();

	// �Ʒ� ȭ��ǥ ��ư�� ������ ���Ѵ�
	MRECT GetDownArrowRgn();

	// ���� ȭ��ǥ ��ư�� ������ ���Ѵ�
	MRECT GetLeftArrowRgn();

	// ������ ȭ��ǥ ��ư�� ������ ���Ѵ�
	MRECT GetRightArrowRgn();


	// ��� ������ �������� ���θ� ���ϴ� �Լ�
	bool IsEnable();	


	// ��Ŀ���� ���� �������� ���θ� ���ϴ� �Լ�
	bool IsFocus();


	// Look�� �����Ѵ�
	DECLARE_LOOK( MSpinCtrlLook)
	DECLARE_LOOK_CLIENT()



// �޽��� �ڵ鷯
protected:
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener);
	virtual void OnRun( void);
	virtual void OnSize( int w, int h);


#define MINT_SPINCTRL	"SpinCtrl"
	virtual const char* GetClassName(void)		{ return MINT_SPINCTRL; }
};



} // namespace mint3