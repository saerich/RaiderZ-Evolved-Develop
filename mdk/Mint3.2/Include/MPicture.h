#pragma once

#include "MWidget.h"


// namespace : mint3
namespace mint3
{


/// class : MPicture
class MPicture : public MWidget
{
/// Member variables
protected:
	MBitmap*		m_pBitmap;			///< ��Ʈ�� �̹��� ������
	bool			m_bStretch;			///< ��Ʈ���� �÷� ����� �����ϴ� �Լ�
	MDrawEffect		m_Effect;			///< ��Ʈ�� ����Ʈ(add, multiple)
	DWORD			m_dwRotate;			///< ��Ʈ�� ȸ�� ����(�¿� ����, ���Ϲ���, 90�� ȸ��, 90�� ��ȸ��)
	MCOLOR			m_Colorize;			///< ��Ʈ�� �̹����� colorize ����




/// Member functions
public:
	MPicture( const char* szName =NULL, MWidget* pParent =NULL, MListener* pListener =NULL);

	void SetBitmap( MBitmap* pBitmap)				{ m_pBitmap = pBitmap;			}
	MBitmap* GetBitmap()							{ return m_pBitmap;				}

	bool SetStretch( bool bStretch);
	bool GetStretch() const							{ return m_bStretch;			}

	MDrawEffect SetEffect( MDrawEffect _effect);
	MDrawEffect GetEffect() const					{ return m_Effect;				}

	MCOLOR SetColorize( MCOLOR& color);
	MCOLOR GetColorize() const						{ return m_Colorize;			}

	void SetFlipHorizontal();
	void SetFlipVertical();
	void SetRotate90CW();
	void SetRotate90CCW();
	void SetRotate180();
	void ResetRotate()								{ m_dwRotate = MBM_NORMAL;		}



/// Event handler
protected:
	virtual void OnDraw( MDrawContext* pDC) override;



/// LUA script
public:
	void glueSetImage( const char* szImageName);
	const char* glueGetImage();
	int glueGetImageWidth()							{ return (m_pBitmap ? m_pBitmap->GetWidth() : 0);	}
	int glueGetImageHeight()						{ return (m_pBitmap ? m_pBitmap->GetHeight() : 0);	}
	bool glueSetStretch( bool bStretch)				{ return SetStretch( bStretch);						}
	bool glueGetStretch()							{ return GetStretch();								}
	void glueSetEffect( const char* szEffect);
	const char* glueGetEffect();
	MWLua::ret_int4 glueSetColorize( int r, int g, int b);
	MWLua::ret_int4 glueGetColorize();
	void glueSetFlipHorizontal()					{ SetFlipHorizontal();								}
	void glueSetFlipVertical()						{ SetFlipVertical();								}
	void glueSetRotate90CW()						{ SetRotate90CW();									}
	void glueSetRotate90CCW()						{ SetRotate90CCW();									}
	void glueSetRotate180()							{ SetRotate180();									}
	void glueResetRotate()							{ ResetRotate();									}


#define MINT_PICTURE	"Picture"
	virtual const char* GetClassName()				{ return MINT_PICTURE;			}
};


} // namespace mint3


