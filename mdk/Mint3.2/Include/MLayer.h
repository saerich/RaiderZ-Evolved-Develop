/************************************************************************
MLayer

Prog. : Lim Dong Hwan
Date  : 29.JUN.2009
Desc. : Layer ���� Ŭ���� �Դϴ�.
*************************************************************************/

#pragma once

#include "MWidget.h"


// namespace : mint3
namespace mint3
{

/*	MLayer
	Layer ���� Ŭ����
*/
class MLayer : public MWidget
{
// Member variables
protected:


// Member functions
public:
	// ������
	MLayer( const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);

	virtual bool IsLayered() const override;


protected:
	virtual void OnDraw( MDrawContext* pDC) override;
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener) override;
	virtual void OnSize( int w, int h) override;


#define MINT_LAYER		"Layer"
	virtual const char* GetClassName(void)		{ return MINT_LAYER;			}
};


} // namespace mint3














/*****************************************************************************
���⼭���� ���� �ڵ��Դϴ�


#pragma once

#include "MWidget.h"

namespace mint3 {

/// Picture
class MLayer : public MWidget{
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
	MLayer(const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);

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