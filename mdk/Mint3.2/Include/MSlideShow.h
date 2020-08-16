#pragma once

#include "MWidget.h"


// namespace : mint3
namespace mint3
{


/// class : MSlideShow
class MSlideShow : public MWidget
{
/// Member variables
protected:
	MBitmap*		m_pBitmap;				///< ��Ʈ�� �̹��� ������
	int				m_nRow;					///< ���η� ���� ����
	int				m_nCol;					///< ���η� ���� ����
	MSIZE			m_SceneSize;			///< �� ������
	unsigned int	m_nMaxSceneCount;		///< �ִ� ������ ��� ����
	unsigned int	m_nSceneCount;			///< ��� ������ ��� ����
	unsigned int	m_nCurScene;			///< ���� ��� ��ȣ
	unsigned int	m_nPrevScene;			///< ���� ��� ��ȣ
	bool			m_bSlideShow;			///< �����̵� ��
	unsigned int	m_nDelayTime;			///< �����̵� �� ���� �ð�(ms)
	DWORD			m_dwSlideShowTimer;		///< �����̵� Ÿ�̸�
	bool			m_bPlayOnce;			///< �� ���� ����ϴ��� ����
	MTimer			m_tFade;				///< ���̵� Ÿ�̸�
	MDrawEffect		m_Effect;				///< ��Ʈ�� ����Ʈ(add, multiple)
	DWORD			m_dwRotate;				///< ��Ʈ�� ȸ�� ����(�¿� ����, ���Ϲ���, 90�� ȸ��, 90�� ��ȸ��)
	MCOLOR			m_Colorize;				///< ��Ʈ�� �̹����� colorize ����


	
	
/// Member functions
public:
	MSlideShow( const char* szName =NULL, MWidget* pParent =NULL, MListener* pListener =NULL);

	void SetBitmap( MBitmap* pBitmap);
	MBitmap* GetBitmap() const						{ return m_pBitmap;									}

	int SliceScene( int _row, int _col =0);
	
	MSIZE GetSceneSize() const						{ return m_SceneSize;								}

	int SetSceneCount( int nCount);
	int GetSceneCount() const						{ return m_nSceneCount;								}

	int SetCurScene( int nCurScene);
	int GetCurScene() const							{ return m_nCurScene;								}

	bool SlideShow( bool bSlideShow);
	bool IsSlideShow() const						{ return m_bSlideShow;								}

	void SetDelayTime( int nMilliSec)				{ m_nDelayTime = (unsigned int)nMilliSec;			}
	int GetDelayTime()								{ return (int)m_nDelayTime;							}

	void SetFadeTime( int nMilliSec)				{ m_tFade.SetArrivalTime( (DWORD)nMilliSec);		}
	int GetFadeTime()								{ return (int)m_tFade.GetArrivalTime();				}

	void SetPlayOnce( bool bSet)					{ m_bPlayOnce = bSet;								}
	bool IsPlayOnce() const							{ return m_bPlayOnce;								}

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



protected:
	MRECT GetSceneRect( int nSceneNum) const;



/// Event handler
protected:
	virtual void OnRun( DWORD dwCurrTime) override;
	virtual void OnDraw( MDrawContext* pDC) override;



/// LUA script
public:
	void glueSetImage( const char* szName);	
	const char* glueGetImage();
	MWLua::ret_int2 glueGetSceneSize() const		{ return MWLua::ret_int2( m_SceneSize.w, m_SceneSize.h);	}
	int glueSliceScene( int _row, int _col)			{ return SliceScene( _row, _col);							}
	int glueSetSceneCount( int nCount)				{ return SetSceneCount( nCount);							}
	int glueGetSceneCount() const					{ return GetSceneCount();									}
	int glueSetCurScene( int nCurScene)				{ return SetCurScene( nCurScene);							}
	int glueGetCurScene() const						{ return GetCurScene();										}
	bool glueSetSlideShow( bool bSlideShow)			{ return SlideShow( bSlideShow);							}
	bool glueIsSlideShow() const					{ return IsSlideShow();										}
	void glueSetDelayTime( int nMilliSec)			{ SetDelayTime( nMilliSec);									}
	int glueGetDelayTime()							{ return GetDelayTime();									}
	void glueSetFadeTime( int nMilliSec)			{ SetFadeTime( nMilliSec);									}
	int glueGetFadeTime()							{ return GetFadeTime();										}
	void glueSetPlayOnce( bool bSet)				{ SetPlayOnce( bSet);										}
	bool glueIsPlayOnce() const						{ return IsPlayOnce();										}
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



#define MINT_SLIDESHOW		"SlideShow"
	virtual const char* GetClassName() override		{ return MINT_SLIDESHOW;									}
};


} // namespace mint3


