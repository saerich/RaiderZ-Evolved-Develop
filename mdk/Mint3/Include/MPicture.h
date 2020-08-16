/************************************************************************
MPicture

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Picture ���� Ŭ���� �Դϴ�.
*************************************************************************/

#pragma once

#include "MWidget.h"


// namespace : mint3
namespace mint3
{

/*	MPicture
	Picture ���� Ŭ����
*/
class MPicture : public MWidget
{
// Member variables
protected:
	MBitmap*	m_pBitmap;			// ��Ʈ�� �̹��� ������

	bool		m_bStretch;			// ��Ʈ���� �÷� ����� �����ϴ� �Լ�

	DWORD		m_dwRotate;			// ��Ʈ�� ȸ�� ����(�¿� ����, ���Ϲ���, 90�� ȸ��, 90�� ��ȸ��)

	MCOLOR		m_Colorize;			// ��Ʈ�� �̹����� colorize ����

	bool		m_bTiled;			// ��Ʈ���� Ÿ�� �������� ����� �����ϴ� �Լ�




// Member functions
public:
	// ������
	MPicture( const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);


	// ��Ʈ�� ���ҽ��� �����ϴ� �Լ�
	void SetBitmap( MBitmap* pBitmap);

	// ��Ʈ�� ���ҽ��� ���ϴ� �Լ�
	MBitmap* GetBitmap();


	// ��Ʈ���� �÷� ����� �����ϴ� �Լ�
	void SetStretch( bool bStretch);


	// ��Ʈ���� �¿�� ������ �Լ�
	void SetFlipHorizontal();

	// ��Ʈ���� ���Ϸ� ������ �Լ�
	void SetFlipVertical();

	// ��Ʈ���� �ð� �������� 90�� ȸ����Ű�� �Լ�
	void SetRotate90CW();

	// ��Ʈ���� �ݽð� �������� 90�� ȸ����Ű�� �Լ�
	void SetRotate90CCW();

	// ��Ʈ���� �ݽð� �������� 180�� ȸ����Ű�� �Լ�
	void SetRotate180();

	// ��Ʈ�� ȸ���� �����ϴ� �Լ�
	void ResetRotate();

	// ��Ʈ���� Ÿ�� �������� ����� �����ϴ� �Լ�
	void SetTiled( bool bTiled)			{ m_bTiled = bTiled;	}


	// ��Ʈ�� �̹����� colorize�� �����ϴ� �Լ�
	void SetColorize( MCOLOR color);	


// Event handler
	void glueSetBitmapName( const char* szBitmapName );

protected:
	// OnDraw �̺�Ʈ �ڵ鷯
	virtual void OnDraw( MDrawContext* pDC);

	// OnEvent �̺�Ʈ �ڵ鷯
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener);


#define MINT_PICTURE	"Picture"
	virtual const char* GetClassName(void)		{ return MINT_PICTURE; }
};


} // namespace mint3














/*****************************************************************************
���⼭���� ���� �ڵ��Դϴ�


#pragma once

#include "MWidget.h"

namespace mint3 {

/// Picture
class MPicture : public MWidget{
protected:
	MBitmap* m_pBitmap;
	int		m_iStretch;
	int		m_iAnimType;			// 0:��->��, 1:��->��
	float	m_fAnimTime;			// �ִϸ��̼� �Ǵ� �ð�
	DWORD	m_dwCurrentTime;
	bool	m_bAnim;
	DWORD	m_DrawMode;

	MCOLOR	m_BitmapColor;
	MCOLOR	m_BitmapReseveColor;
	bool	m_bSwaped;

protected:
	/// Draw Handler
	virtual void OnDraw(MDrawContext* pDC);
	void OnAnimDraw(MDrawContext* pDC, int x, int y, int w, int h, int bx, int by, int bw, int bh );

public:
	MPicture(const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);

	/// ��Ʈ�� ����
	void SetBitmap(MBitmap* pBitmap);
	/// ��Ʈ�� ���
	MBitmap* GetBitmap(void);
	int		GetStretch() { return m_iStretch; }
	void SetStretch(int i) { m_iStretch = i; }

	void SetAnimation( int animType, float fAnimTime);

	void SetBitmapColor( MCOLOR color );	
	MCOLOR GetBitmapColor() const {
		return m_BitmapColor;
	}
	MCOLOR GetReservedBitmapColor() const{
		return m_BitmapReseveColor;	
	}

	void SetDrawMode( DWORD mode ) {
		m_DrawMode = mode;
	}
	DWORD GetDrawMode( ) const {
		return m_DrawMode;
	}

	bool IsAnim() const { return m_bAnim;}

#define MINT_PICTURE	"Picture"
	virtual const char* GetClassName(void){ return MINT_PICTURE; }
};


} // namespace mint3
*/