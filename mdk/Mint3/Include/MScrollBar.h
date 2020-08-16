#pragma once

#include "MWidget.h"
#include "MLookNFeel.h"


// namespace : mint3
namespace mint3
{


// Ŭ���� ����
class MListBox;



// ScrollBar Ÿ��
enum MScrollBarTypes
{
	MSBT_VERTICAL		= 0,		// ���� ���θ鿡 ����
	MSBT_HORIZONTAL		= 1			// �ϴ� ���θ鿡 ����
};



// ��ũ�ѹ� ����
enum MSCROLLBAR_REGION
{
	MSBR_NONE			= -1,		// ����
	MSBR_UPARROW		= 0,		// �� ȭ��ǥ ����
	MSBR_DOWNARROW,					// �Ʒ� ȭ��ǥ ����
	MSBR_LEFTARROW,					// ���� ȭ��ǥ ����
	MSBR_RIGHTARROW,				// ������ ȭ��ǥ ����
	MSBR_THUMB,						// Thumb ����
	MSBR_BKGRND,					// ��� ����
	MSBR_MAX
};



class MScrollBar;


// ��ũ�ѹ� Ŀ���� Look
class MScrollBarLook
{
public:
	virtual void OnDraw( MScrollBar* pScrollBar, MDrawContext* pDC);
	virtual MRECT GetClientRect( MScrollBar* pScrollBar, MRECT& r);
};



// ��ũ�ѹ� Ŭ���� ����
class MScrollBar : public MWidget
{
// friend ����
friend MListBox;


// Member variables
protected:
	// ��ũ�ѹ� Ÿ��
	MScrollBarTypes		m_nType;

	// ȭ��ǥ ��ư�� ������ ����
	bool				m_bShowArrow;

	// ��ũ�ѹ��� ���� ������ ��
	int					m_nValue;

	// ��ũ�ѹ� �ִ�/�ּ� ����
	int					m_nRangeMin;
	int					m_nRangeMax;

	// ��ũ�ѹ� �β�
	int					m_nWidth;

	// ���콺�� ���� ��ġ
	MSCROLLBAR_REGION	m_nPushedRgn;

	// Ŀ�� ��ǥ
	MPOINT				m_ptCursorPos;

	// Ÿ�̸�
	DWORD				m_dwTimer;



// Member functions
public:
	// ������
	MScrollBar( const char* szName, MWidget* pParent=NULL, MListener* pListener=NULL);
	MScrollBar( MWidget* pParent=NULL, MListener* pListener=NULL, MScrollBarTypes type=MSBT_VERTICAL);

	// �Ҹ���
	virtual ~MScrollBar();


	// Ÿ���� �����ϰų� ���ϴ� �Լ�
	void SetType( MScrollBarTypes type)			{ m_nType = type;							}
	MScrollBarTypes GetType()					{ return m_nType;							}


	// ȭ��ǥ ��ư�� ������ ���θ� ���ϴ� �Լ�
	bool IsShowArrow()							{ return m_bShowArrow;						}


	// ��ũ�ѹ��� �ִ�/�ּ� ������ �����ϰų� ���ϴ� �Լ�
	void SetRange( int min, int max);
	int GetMin()								{ return m_nRangeMin;						}
	int GetMax()								{ return m_nRangeMax;						}


	// ���� �����ϰų� ���ϴ� �Լ�
	void SetValue( int nValue);
	void SetValue( MPOINT point);
	int GetValue()								{ return m_nValue;							}


	// ��ũ�ѹ� �β��� �����ϰų� ���ϴ� �Լ�
	void SetWidth( int nWidth)					{ m_nWidth = nWidth;						}
	int GetWidth()								{ return m_nWidth;							}


	// ���콺�� ���� ������ ���ϴ� �Լ�
	MSCROLLBAR_REGION GetPushedRgn()			{ return m_nPushedRgn;	}


	// �� ȭ��ǥ ��ư�� ������ ���Ѵ�
	MRECT GetUpArrowRgn();

	// �Ʒ� ȭ��ǥ ��ư�� ������ ���Ѵ�
	MRECT GetDownArrowRgn();

	// ���� ȭ��ǥ ��ư�� ������ ���Ѵ�
	MRECT GetLeftArrowRgn();

	// ������ ȭ��ǥ ��ư�� ������ ���Ѵ�
	MRECT GetRightArrowRgn();

	// Thumb ��ư�� ������ ���Ѵ�
	MRECT GetThumbRgn();

	// ��� ������ ���Ѵ�
	MRECT GetBackgroundRgn();


	// ��� ������ �������� ���θ� ���ϴ� �Լ�
	bool IsEnable();	


	// ��Ŀ���� ���� �������� ���θ� ���ϴ� �Լ�
	bool IsFocus();


	// Look�� �����Ѵ�
	DECLARE_LOOK( MScrollBarLook)
	DECLARE_LOOK_CLIENT()

	// Glue �Լ�
	int glueGetValue() { return GetValue(); }

// �޽��� �ڵ鷯
protected:
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener);
	virtual void OnRun( void);
	virtual void OnSize( int w, int h);

public:
	// �ӽ�
	int GetDefaultBreadth(void) { return 0; }
	void MoveStartLine() {}
	void MoveEndLine();

#define MINT_SCROLLBAR	"ScrollBar"
	virtual const char* GetClassName(void)		{ return MINT_SCROLLBAR; }
};



} // namespace mint3