/************************************************************************
MPicture

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Picture ���� Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MPicture.h"



// namespace : mint3
namespace mint3
{

/*	MPicture
	������
*/
MPicture::MPicture( const char* szName /*=NULL*/, MWidget* pParent /*=NULL*/, MListener* pListener /*=NULL*/)
: MWidget( szName, pParent, pListener)
{
	// ��Ʈ�� ������ �ʱ�ȭ
	m_pBitmap = NULL;

	// ��Ʈ�� �÷� ��� ��� ����
	m_bStretch = false;	

	// ��Ʈ���� ȸ����Ű�� ����
	m_dwRotate = MBM_Normal;

	// Colorize ���� ����
	m_Colorize = MCOLOR( 255, 255, 255);
}


/*	SetBitmap
	��Ʈ�� ���ҽ��� �����ϴ� �Լ�
*/
void MPicture::SetBitmap( MBitmap* pBitmap)
{
	m_pBitmap = pBitmap;
}


/*	GetBitmap
	��Ʈ�� ���ҽ��� ���ϴ� �Լ�
*/
MBitmap* MPicture::GetBitmap()
{
	return m_pBitmap;
}



/*	SetStretch
	��Ʈ���� �÷� ����� �����ϴ� �Լ�
*/
void MPicture::SetStretch( bool bStretch)
{
	m_bStretch = bStretch;
}


/*	SetFlipHorizontal
	��Ʈ���� �¿�� ������ �Լ�
*/
void MPicture::SetFlipHorizontal()
{
	unsigned int nBit90L = MBM_RotL90;
	unsigned int nBit90R = MBM_RotR90;

	unsigned int nBit = ((m_dwRotate & nBit90L) || (m_dwRotate & nBit90R))  ?  MBM_FlipUD : MBM_FlipLR;


	// �̹� �¿츦 ������ ���¸�...
	if ( m_dwRotate & nBit)		m_dwRotate &= ~nBit;
	else						m_dwRotate |= nBit;
}


/*	SetFlipVertical
	��Ʈ���� ���Ϸ� ������ �Լ�
*/
void MPicture::SetFlipVertical()
{
	unsigned int nBit90L = MBM_RotL90;
	unsigned int nBit90R = MBM_RotR90;

	unsigned int nBit = ((m_dwRotate & nBit90L) || (m_dwRotate & nBit90R))  ?  MBM_FlipLR : MBM_FlipUD;


	// �̹� ���ϸ� ������ ���¸�...
	if ( m_dwRotate & nBit)		m_dwRotate &= ~nBit;
	else						m_dwRotate |= nBit;
}


/*	SetRotate90CW	
	��Ʈ���� �ð� �������� 90�� ȸ����Ű�� �Լ�
*/
void MPicture::SetRotate90CW()
{
	unsigned int nBit90L = MBM_RotL90;
	unsigned int nBit90R = MBM_RotR90;


	if ( m_dwRotate & nBit90L)
		m_dwRotate &= ~nBit90L;
	else if ( m_dwRotate & nBit90R)
	{
		SetRotate180();

		m_dwRotate &= ~nBit90R;
	}
	else
		m_dwRotate |= nBit90R;
}


/*	SetRotate90CCW
	��Ʈ���� �ݽð� �������� 90�� ȸ����Ű�� �Լ�
*/
void MPicture::SetRotate90CCW()
{
	unsigned int nBit90L = MBM_RotL90;
	unsigned int nBit90R = MBM_RotR90;


	if ( m_dwRotate & nBit90R)
		m_dwRotate &= ~nBit90R;
	else if ( m_dwRotate & nBit90L)
	{
		SetRotate180();

		m_dwRotate &= ~nBit90L;
	}
	else
		m_dwRotate |= nBit90L;
}


/*	SetRotate180
	��Ʈ���� �ݽð� �������� 180�� ȸ����Ű�� �Լ�
*/
void MPicture::SetRotate180()
{
	SetFlipHorizontal();
	SetFlipVertical();
}


/*	SetNormal
	��Ʈ�� ȸ���� �����ϴ� �Լ�
*/
void MPicture::ResetRotate()
{
	m_dwRotate = MBM_Normal;
}


/*	SetColorize
	��Ʈ�� �̹����� colorize�� �����ϴ� �Լ�
*/
void MPicture::SetColorize( MCOLOR color)
{
	m_Colorize = color;
}


/*	OnEvent
	OnEvent �̺�Ʈ �ڵ鷯
*/
bool MPicture::OnEvent( MEvent* pEvent, MListener* pListener)
{
	return false;
}


/*	OnDraw
	OnDraw �̺�Ʈ �ڵ鷯
*/
void MPicture::OnDraw( MDrawContext* pDC)
{
	// ��Ʈ���� ������ �׳� ����
	if ( m_pBitmap == NULL)
		return;


	// ȸ�� ����
	DWORD tmpDrawMode = m_pBitmap->GetDrawMode();
	m_pBitmap->SetDrawMode( m_dwRotate);

	
	// Colorize ���� ����
	MCOLOR tmpColorize = pDC->GetBitmapColor();
	pDC->SetBitmapColor( m_Colorize);


	// ��Ʈ�� ����
	pDC->SetBitmap( m_pBitmap);


	// ��Ʈ���� �÷���´ٸ� ���� Ŭ���̾�Ʈ ���� ��ü�� �׸���
	if ( m_bStretch)
	{
		MRECT rect = GetClientRect();
		pDC->Draw( rect.x, rect.y, rect.w, rect.h, 0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
	}
	else if ( m_bTiled)
	{
		MRECT rect = GetClientRect();
		pDC->DrawTiled( rect.x, rect.y, rect.w, rect.h, 0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
	}
	else
		pDC->Draw( 0, 0, 0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight());


	// Colorize ���� ����
	pDC->SetBitmapColor( tmpColorize);


	// �׸��� ��� ����
	m_pBitmap->SetDrawMode( tmpDrawMode);
}

void MPicture::glueSetBitmapName( const char* szBitmapName )
{
	MBitmap* pBitmap = MBitmapManager::Get(szBitmapName);
	if (pBitmap == NULL) return;

	SetBitmap(pBitmap);
}

} // namespace mint3


















/**********************************************************************************
���⼭���� ���� �ڵ��Դϴ�



#include "stdafx.h"
#include "MPicture.h"
#include "math.h"

namespace mint3 {

void MPicture::OnDraw(MDrawContext* pDC)
{
 	if(m_pBitmap==NULL) return;
	pDC->SetBitmap(m_pBitmap);

	MCOLOR tempColor = pDC->GetBitmapColor();
	pDC->SetBitmapColor( m_BitmapColor );
	DWORD tempDrawMode = m_pBitmap->GetDrawMode();
	m_pBitmap->SetDrawMode( m_dwRotate );

	int bw, bh;
	if( m_iStretch == 2 )	bw	= m_Rect.w;
	else		bw	= m_pBitmap->GetWidth();
	if( m_iStretch == 1 )	bh	= m_Rect.h; 
	else		bh	= m_pBitmap->GetHeight();
    
	int w, h;
	if (m_iStretch != 0)	{
		w = m_Rect.w;
		h = m_Rect.h;
	}
 	else	{
		w = bw;
		h = bh;
	}

 	if(m_bAnim)
		OnAnimDraw( pDC, 0,0, w, h, 0,0, bw, bh );
	else
 		pDC->Draw( 0, 0, w, h, 0, 0, bw, bh );

	pDC->SetBitmapColor( tempColor );
	m_pBitmap->SetDrawMode( tempDrawMode );
}

void MPicture::SetAnimation( int animType, float fAnimTime )
{
	m_bAnim = true;
	m_fAnimTime = fAnimTime;
	m_iAnimType = animType;
	m_dwCurrentTime = timeGetTime();
}

void MPicture::OnAnimDraw( MDrawContext* pDC, int x, int y, int w, int h, int bx, int by, int bw, int bh )
{
   	DWORD e_time = timeGetTime() - m_dwCurrentTime;
 	float ratio	= e_time / m_fAnimTime;
	
   	if( ratio >= 1.0f ) 
	{
		if(m_iAnimType == 2)
		{
 			m_iAnimType = 0;
			m_dwCurrentTime = timeGetTime();
			if(m_bSwaped)
			{
				MCOLOR c = m_BitmapColor;
				m_BitmapColor = m_BitmapReseveColor;
				m_BitmapReseveColor = c;
				m_bSwaped = false;
			}
			return;
		}
		else if(m_iAnimType == 3) 
		{
			m_iAnimType = 1;
			m_dwCurrentTime = timeGetTime();
			if(m_bSwaped)
			{
 				MCOLOR c = m_BitmapColor;
				m_BitmapColor = m_BitmapReseveColor;
				m_BitmapReseveColor = c;
				m_bSwaped = false;
			}
			return;
		}
		else
		{
			m_bAnim = false;
			pDC->Draw( 0, 0, w, h, 0, 0, bw, bh );
			return;
		}
	}

	int i_offset;
 	
  	if( m_iAnimType == 0 )
	{		
   		i_offset = (int)(bw*sqrtf(ratio));
		pDC->Draw( x,y,i_offset, h, bx+(bw-i_offset), by, i_offset, bh );
 	}
	else if( m_iAnimType == 1 )
	{
 		i_offset = (int)(bw*sqrtf(ratio));
		pDC->Draw( x+w-i_offset,y,i_offset, h, bx+(bw-i_offset), by, i_offset, bh );
	}
 	else if( m_iAnimType == 2 ) //<----
	{
//  		i_offset = w - w*ratio*ratio;	
		i_offset = (int)(bw - bw*sqrtf(ratio));	
		if(!m_bSwaped)
		{
			MCOLOR c = m_BitmapColor;
			m_BitmapColor = m_BitmapReseveColor;
			m_BitmapReseveColor = c;
			m_bSwaped = true;
			pDC->SetBitmapColor( m_BitmapColor );
		}
 		pDC->Draw( x,y,i_offset, h, bx+(bw-i_offset), by, i_offset, bh );
	}
	else if( m_iAnimType == 3 )
	{
		//i_offset = w - w*ratio*ratio;	
		i_offset = (int)(bw - bw*sqrtf(ratio));	
		if(!m_bSwaped)
		{
			MCOLOR c = m_BitmapColor;
			m_BitmapColor = m_BitmapReseveColor;
			m_BitmapReseveColor = c;
			m_bSwaped = true;
			pDC->SetBitmapColor( m_BitmapColor );
		}
		pDC->Draw( x+w-i_offset,y,i_offset, h, bx+(bw-i_offset), by, i_offset, bh );
	}
}

MPicture::MPicture(const char* szName, MWidget* pParent, MListener* pListener)
: MWidget(szName, pParent, pListener)
{
	m_pBitmap = NULL;
	m_bAnim = false;
	m_iStretch = 0;
	m_BitmapColor = 0xFFFFFFFF;
	m_BitmapReseveColor = 0xFFFFFFFF;
	m_dwRotate = MBM_Normal;
	m_bSwaped = false;
}

void MPicture::SetBitmap(MBitmap* pBitmap)
{
	m_pBitmap = pBitmap;

	if(m_pBitmap==NULL) return;

	if( m_iStretch == 0 ) SetSize(m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
}

MBitmap* MPicture::GetBitmap(void)
{
	return m_pBitmap;
}

void MPicture::SetBitmapColor( MCOLOR color )
{
	m_BitmapReseveColor = m_BitmapColor;
	m_BitmapColor.r = color.r;
	m_BitmapColor.g = color.g;
	m_BitmapColor.b = color.b;
}

} // namespace mint3
*/
