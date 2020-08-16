//----------------------------------------------------------------------------------------------------
// ���ϸ� : RFont.h
// ��  �� : 2008. 01. 18 - Font �ȼ� �и��� ���� ���� �� 1�� �Ϸ�. -3DJun
// ��  �� : RealSpace3�� Font Class.
//			Mint3�� ���� �Ǿ� ���Ǹ�, Sample�̳� Tool ������Ʈ������ RTextSceneNode�� ���� ���� ������ ��� �Ѵ�.
//
// Copyright(c) 2008 Maiet Ent. Co.,Ltd. All Rights Reserved.
//----------------------------------------------------------------------------------------------------
#ifndef RFONT_H
#define RFONT_H

//#pragma warning(disable: 4786)

#include "RTypes.h"
//#include <d3dx9.h>
#include <map>
#include <list>
#include <string>
#include <wchar.h>
#include "MemPool.h"
//#include "RDeviceD3D.h"
#include "RVType.h"
#include "d3d9types.h"
#include "MLocale.h"

namespace rs3 {

/// �ѱ����� ������ ���´�
struct RCHARINFO : public CMemPoolSm<RCHARINFO> {		
	int nWidth;
	int nHeight;
	int nFontTextureID;
	int nFontTextureIndex;
};

typedef map<WORD,RCHARINFO*> RCHARINFOMAP;
struct RFONTTEXTURECELLINFO;
typedef list<RFONTTEXTURECELLINFO*> RFONTTEXTURECELLINFOLIST;

/// Ŀ�ٶ� �ؽ����� RFontTexture���� �� ���� ������ ����ִ� ����ü
struct RFONTTEXTURECELLINFO {
	int nID;
	int nIndex;
	RFONTTEXTURECELLINFOLIST::iterator itr;
};

//----------------------------------------------------------------------------------------------------
// RFontTexture
/**
* \brief �������ڸ� �����ϰ� �ִ� Ŀ�ٶ� �ؽ��� ����
* \li  Header  : RFontTexture.h
* \li  Library : RealSpace3.lib
*/
//----------------------------------------------------------------------------------------------------
class RFontTexture {

	RDevice*	m_pd3dDevice;		///< Renderer Device

	//////////////////////////////////////////////////////////////////////////
	// DC
	HDC			m_hDC;				///< �ؽ��Ŀ� �� ���ڸ� �׸��� �ӽ� DC & dibsection
	DWORD*		m_pBitmapBits_Font;		///< Texture�� Rect Pointer
	DWORD*		m_pBitmapBits_Outline;
	HBITMAP		m_hbmBitmap_Font;
	HBITMAP		m_hbmBitmap_Outline; 
	HBITMAP		m_hPrevBitmap;
	ULONG_PTR	m_pToken;
	
	//////////////////////////////////////////////////////////////////////////
	// Texture Info
	RTexture*	m_pTexture_Font;	///< CreateDIBSection���� ������ Texture Pointer.
									///< Rfont�� ���� �ϳ��� Texture_Font�� Texture_Outline �� ������. ( Width x Height ���� �̷���� �ϳ��� �ؽ��ĸ� ���۷� ���� )
	RTexture*	m_pTexture_Outline;	///< Outline Texture
	int			m_nWidth;			///< �ؽ����� �ʺ�
	int			m_nHeight;			///< �ؽ����� ��
	int			m_nX,m_nY;			///< ���� ����(�ʺ�, ��)
	int			m_nCell;			///< ���� ���� = nX * nY

	//////////////////////////////////////////////////////////////////////////
	//
	int			m_LastUsedID;		///< ���� �Ҵ�� ������ ����
	RFONTTEXTURECELLINFO*	 m_CellInfo;		///< Ŀ�ٶ� �ؽ����� RFontTexture���� �� ���� ������ ����ִ� ����ü
	RFONTTEXTURECELLINFOLIST m_PriorityQueue;	///< �� ������ ����Ʈ ���� �ֱٿ� ���� ���� �������� ���� �Ѵ�.
												///< �����ÿ� ���� ������ �ֱ� ���ټ��� ���� ������ �����Ѵ�.

public:
	RFontTexture();
	~RFontTexture();

	//////////////////////////////////////////////////////////////////////////
	// Create
	bool	Create(RDevice *pDevice);

	//////////////////////////////////////////////////////////////////////////
	// Destroy
	void	Destroy();

	//////////////////////////////////////////////////////////////////////////
	// Update
	bool	IsNeedUpdate(int nIndex, int nID);		///< ���ŵǾ�� �ϴ��� �˻�
	bool	MakeFontBitmap(HFONT hFont, RCHARINFO *pInfo, const wchar_t* szText, int nOutlineWidth, float fOutlineOpacity, DWORD nColorArg1, DWORD nColorArg2);

	void	Outline_GDIPlusPath(HFONT hFont, const wchar_t* szText, int nOutlineWidth);
	void	Outline_GDIOverDraw(const wchar_t* szText, int nOutlineWidth);

	//////////////////////////////////////////////////////////////////////////
	// Render
	bool	UploadTexture(RTexture* pTexture, RCHARINFO *pCharInfo, DWORD* pBitmapBits, int nIndex, int w,int h);
	void	BlitRect(BYTE* pDest, INT Pitch, int x1, int y1, int x2, int y2, int sw, int sh, DWORD* pSrcBitmapBits);

	//////////////////////////////////////////////////////////////////////////
	// Get / Set
	int		GetCharWidth(HFONT hFont, const wchar_t* szChar);
	int		GetCharABCWidth(const wchar_t* szText, SIZE size);

	HDC 	GetDC() { return m_hDC; }
	int 	GetWidth() { return m_nWidth; }
	int 	GetHeight() { return m_nHeight; }
	int 	GetCellCountX() { return m_nX; }
	int 	GetCellCountY() { return m_nY; }
	int 	GetLastUsedID() { return m_LastUsedID; }

	RTexture* GetTextureFont() { return m_pTexture_Font; }
	RTexture* GetTextureOutline() { return m_pTexture_Outline; }
};

//----------------------------------------------------------------------------------------------------
// RFont
/**
* \brief RealSpace3 Font
* \li  Header  : RFont.h
* \li  Library : RealSpace3.lib
*/
//----------------------------------------------------------------------------------------------------
class RS_API RFont {
	friend class RUnitTestRunner; //UnitTest�Ŀ� g_bFinalize �ʱ�ȭ���ֱ�
	static void ResetFinalizeFlag();

protected:
	static RDevice*		m_pDevice;		///< Renderer Device

	//////////////////////////////////////////////////////////////////////////
	// Font
	HFONT				m_hFont;			///< Font Handle
	RCHARINFOMAP		m_CharInfoMap;		///< ���ڿ��� ���� �ʸ���Ʈ // ó�� �ѹ��� ����ϰ� �������ʹ� ���� ����.
	RFontTexture*		m_pFontTexture;		///< ��Ʈ �ؽ���

	//////////////////////////////////////////////////////////////////////////
	// Begin, End
	static RMatrix		m_matOrgView;		///< VIEW Transform�� BeginFont���� �����ϰ� EndFont���� ���� ���ش�.
	static RMatrix		m_matOrgProj;		///< PROJECT Transform�� BeginFont���� �����ϰ� EndFont���� ���� ���ش�.
	static	bool		m_bInFont;			///< beginfont endfont ���̿� �ִ���.

	//////////////////////////////////////////////////////////////////////////
	// 
	//int					m_nHeight;			///< �ٰ� : �����ٰ��� ����. 
	int					m_nFontHeight;		///< ������ ��Ʈ ũ��
	int					m_nOutlineWidth;	///< �ܰ��� �β�
	float				m_fOutlineOpacity;	///< �ܰ��� ����
	bool				m_bAntiAlias;		///< ��Ƽ �ٸ����
	DWORD				m_ColorArg1;		
	DWORD				m_ColorArg2;

	
public:
	RFont(void);
	virtual ~RFont(void);

	//////////////////////////////////////////////////////////////////////////
	// Static
	static bool Initialize(RDevice *pDevice);
	static void Finalize();

	static bool BeginFont(/*DWORD _flag = NULL*/);
	static bool EndFont();
	static void	FlushFont();
	
	static bool AddFontResourceFromFile(const wchar_t* szFontResName);	///< �ܺ� ��Ʈ ������ �߰�
	static bool AddFontResourceFromMemory(const wchar_t* szFontResName);	///< �ܺ� ��Ʈ ������ �߰�

	//////////////////////////////////////////////////////////////////////////
	// Create
	bool		Create(RDevice *pDevice, const wchar_t* szFontName, int nHeight, bool bBold=false, bool bItalic=false, int nOutlineWidth=0, float fOutlineOpacity=1.0f, bool bAntiAlias=false, DWORD nColorArg1=0, DWORD nColorArg2=0);
	//////////////////////////////////////////////////////////////////////////
	// Destroy
	void		Destroy(void);

	//////////////////////////////////////////////////////////////////////////
	// Update
	HRESULT		OnLostDevice();
	HRESULT		OnResetDevice();

	//////////////////////////////////////////////////////////////////////////
	// Render

	//void		DrawText(float x, float y, const wchar_t* szText,  int iLength=-1, DWORD dwColorFont=0xFFFFFFFF, DWORD dwColorOutline=0xFF000000, float fScale=1.0f);	
	// DrawText �� �ٸ� �Լ� �̸��̶� ���ļ� �ܺο����� �̰����� ����ؾ� �մϴ�
	void		DrawText_UNICODE(float x, float y, const wchar_t* szText, int iLength=-1, DWORD dwColorFont=0xFFFFFFFF, DWORD dwColorOutline=0xFF000000, float fScale=1.0f);	
	bool		DrawTextIn3DSpace(const RMatrix& _refMat, const wchar_t* szText, int iLength = -1, DWORD dwColor = D3DCOLOR_XRGB(255,255,255));
	bool		DrawTextIn3DSpace(const RVector& _pos, const wchar_t* szText, int iLength = -1, DWORD dwColor = D3DCOLOR_XRGB(255,255,255));

	//////////////////////////////////////////////////////////////////////////
	// Get / Set
	int			GetHeight(void){ return m_nFontHeight; }
	//int			GetTexHeight(void) { return m_nHeight; }
	int			GetTextWidth(const wchar_t* szText, int nSize=-1);
	SIZE		GetTextSize(const wchar_t *szText, int iLength = -1);

	//////////////////////////////////////////////////////////////////////////
	// ��� ����
	void		Transform3DTo2D(const RVector& vPos, RVector& vOutCenter);
// 	bool		DrawTextToTexture(LPDIRECT3DTEXTURE9 lpTexture, int x, int y, const _TCHAR *szText, int iLength = -1, DWORD dwColor = D3DCOLOR_XRGB(255,255,255));
	bool		DrawTextByTexture(RTexture* pTextureText, SIZE& nTextSize, const RVector& vPos, int nWidth, int nHeight);

};

//////////////////////////////////////////////////////////////////////////
// Debug
bool DumpFontTexture();

}

#endif
