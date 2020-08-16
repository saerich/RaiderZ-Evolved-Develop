//
//	MINT Draw Context
//
#pragma once

#include "CMList.h"
#include "MintTypes.h"
#include "MBitmap.h"
#include "MFont.h"
#include "MCursor.h"
#include "MResourceManager.h"
#include <vector>
#include "MVector3.h"

namespace mint3 {

struct MRECT;
struct MPOINT;

#define MINT_BASE_CLASS_TYPE	0x1030		///< RTTI�� ���� ����� _DEBUG��忡�� �����ϱ� ���� ID

using namespace std;

/// Draw Context, �������� �׸��� �ִ� ��� �Լ��� �����Ѵ�.
class MDrawContext{
protected:
#ifdef _DEBUG
	int		m_nTypeID;
#endif
	MCOLOR			m_Color;			///< ���� ���õǾ� �ִ� �÷�
	MCOLOR			m_HighlightColor;	///< ���� ���õǾ� �ִ� ���϶���Ʈ �÷�(&���� ������ ���� ���ڸ� ���϶��������ش�.)
	MCOLOR			m_ColorKey;			///< ��������Ʈ ����� ���� �÷�Ű
	MCOLOR			m_BitmapColor;	/// < ��Ʈ�ʿ� ���� �� (�⺻�� : White(255,255,255,255) )
	MBitmap*		m_pBitmap;			///< ���� ���õǾ� �ִ� ��Ʈ��
	MFont*			m_pFont;			///< ���� ���õǾ� �ִ� ��Ʈ
	MRECT			m_Clip;				///< ��ũ�� ��ǥ�迡���� Ŭ���� ����
	MPOINT			m_Origin;			///< ��ũ���� �׷����� ���� ���� ����
	MDrawEffect		m_Effect;			///< ȿ��
	float			m_fOpacity;
public:
	MDrawContext(void);
	virtual ~MDrawContext(void);

	// Color Setting
	MCOLOR SetBitmapColor(MCOLOR& color);
	MCOLOR SetBitmapColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
	MCOLOR GetBitmapColor(void);
	MCOLOR SetColor(MCOLOR& color);
	MCOLOR SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
	MCOLOR GetColor(void);
	MCOLOR SetHighlightColor(MCOLOR& color);
	MCOLOR SetHighlightColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
	MCOLOR GetHighlightColor(void);
	MCOLOR SetColorKey(MCOLOR& color);
	MCOLOR SetColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
	MCOLOR GetColorKey(void);

	// Font Setting
	MFont* SetFont(const char* szFontName);
	MFont* SetFont(MFont* pFont);
	MFont* GetFont(){ return m_pFont; }

	// Bitmap Setting
	MBitmap* SetBitmap(MBitmap* pBitmap);
	MBitmap* GetBitmap(){ return m_pBitmap; }

	// Clipping ( Screen Coordinate )
	virtual void SetClipRect(MRECT& r);
	void SetClipRect(int x, int y, int w, int h);
	MRECT GetClipRect(void);

	// Logical Origin
	void SetOrigin(int x, int y);
	void SetOrigin(MPOINT& p);
	MPOINT GetOrigin(void);

	// Draw Effect
	void SetEffect(MDrawEffect effect);
	MDrawEffect GetEffect() { return m_Effect; }

	// Basic Drawing Functions
	virtual void BeginDraw() {}	// �⺻ drawing function �� ���� ������Ʈ ����
	virtual void EndDraw() {}

	virtual void SetPixel(int x, int y, MCOLOR& color) = 0;
	virtual void HLine(int x, int y, int len) = 0;
	virtual void VLine(int x, int y, int len) = 0;
	virtual void Line(int sx, int sy, int ex, int ey) = 0;
	virtual void Rectangle(int x, int y, int cx, int cy);
	void Rectangle(MRECT& r);
	virtual void FillRectangle(int x, int y, int cx, int cy) = 0;
	void FillRectangle(MRECT& r);
	void FillPoly( MPOINT* pPoly, int nCount);

	// Bitmap Drawing
	void Draw(int x, int y);
	void Draw(int x, int y, int w, int h);
	void Draw(int x, int y, int sx, int sy, int sw, int sh);
	void Draw(MPOINT &p);
	void Draw(MRECT& r);
	void Draw(int x, int y, MRECT& s);
	virtual void Draw(MRECT& d, MRECT& s);
	virtual void DrawInverse( int x, int y, int w, int h );
	virtual void DrawInverse( int x, int y, int w, int h, int sx, int sy, int sw, int sh) {};
	virtual void DrawEx(int tx1, int ty1, int tx2, int ty2, 
		                int tx3, int ty3, int tx4, int ty4);
	virtual void Draw(int x, int y, int w, int h, int sx, int sy, int sw, int sh);
	virtual void DrawRotate( MVector3& pos, MVector3& dir, int x, int y, int w, int h, int sx, int sy, int sw, int sh);
	virtual void DrawRotate( int tx, int ty, int tw, int th, int rx, int ry, float ra, int sx, int sy, int sw, int sh);
	virtual void DrawTiled(int x, int y, int w, int h, int sx, int sy, int sw, int sh);

	// Text
//	virtual bool BeginFont() = 0;
//	virtual bool EndFont() = 0;
	int Text(int x, int y, const char* szText, int nLength = -1);
	int Text(MPOINT& p, const char* szText){ return Text(p.x, p.y, szText); }
	virtual int TextMultiLine(MRECT& r, const char* szText,int nLineGap=0,bool bAutoNextLine=true,int nIndentation=0,int nSkipLine=0, MPOINT* pPositions=NULL, bool bSkipColor = false);
	virtual int TextMultiLine2(MRECT& r, const char* szText,int nLineGap=0,bool bAutoNextLine=true,MAlignmentMode am=(MAM_HCENTER|MAM_VCENTER), bool bSkipColor = false );

	virtual int GetTextMultiLine(vector<string> *out_pvecStr, const char* szText, int nStrWidth, bool bSkipColor = false);		// Width ���̸�ŭ ���ڿ� ������

	/*
	/// nPos��° ���ڰ� ������ �����ġ�� ���Ѵ�
	static bool GetTextPosition(MPOINT *pOut,MFont *pFont,int nRectWidth, const char* szText,int nPos,bool bColorSupport=false);
	/// �����ġ�� ���° �������� ����
	static int GetCharPosition(MFont *pFont,int nX,int nLine,int nRectWidth,const char* szText,bool bColorSupport=false);
	*/

	/// &�� ����Ͽ� &�κ��� ���϶�����
	int TextWithHighlight(int x, int y, const char* szText);
	/// Ư�� �ؽ�Ʈ�� ���ĵǾ����� ���� ��ġ ���
	void GetPositionOfAlignment(MPOINT* p, MRECT& r, const char* szText, MAlignmentMode am, bool bAndInclude=true);
	int Text(MRECT& r, const char* szText, MAlignmentMode am=(MAM_HCENTER|MAM_VCENTER));
	int TextWithHighlight(MRECT& r, const char* szText, MAlignmentMode am=(MAM_HCENTER|MAM_VCENTER));

	// Multiple Color Support
	void TextMC(int x, int y, const char* szText);
	void TextMC(MRECT& r, const char* szText, MAlignmentMode am);
	
	/// Multiple Color ��ȣ�� ������ �ؽ�Ʈ�� ���Ѵ�.
	static char* GetPureText(const char *szText);

	/// Opacity�� �����Ѵ�.
	float SetOpacity(float fOpacity);
	float GetOpacity();

	/// MDrawContext, MBitmap, MFont�� State�� �����Ѵ�.
	virtual void SetState(MDrawState eState, MDrawEffect eEffect = MDE_NORMAL) = 0;

	// ��Ʈ�� ������ flush�Ѵ�
	virtual void FlushFont() =0;

private:
	virtual void Draw(MBitmap *pBitmap,int x, int y, int w, int h, int sx, int sy, int sw, int sh) = 0;		// User Defined Draw Function
	virtual void DrawRotate( MBitmap *pBitmap, MVector3& pos, MVector3& dir, int x, int y, int w, int h, int sx, int sy, int sw, int sh) = 0;
	virtual void DrawRotate( MBitmap *pBitmap, int tx, int ty, int tw, int th, int rx, int ry, float ra, int sx, int sy, int sw, int sh) = 0;
	virtual void DrawTiled(MBitmap *pBitmap,int x, int y, int w, int h, int sx, int sy, int sw, int sh) = 0;	// User Defined Draw Function
};


/// Draw Context for 3D
///		- 3D Context Object�� �پ缺�� ���� Ư�� ��Ǽ��� �̸� �������� �ʴ´�.
///		- �ܼ��� ��ü ������ ���� Ŭ�����̴�.
/// @deprecated	AceSaga�� ���� �ڵ��, 3D Rendering Pass�� ���� �����ϴ� ��쿡 ���Ǿ��� ������ �ڵ��̴�. �����δ� ���̻� ������ �ʴ´�.
class MDrawContext3D{
};


// multiline/color support ��ǵ�. TextMultiline �� ����ؼ� ����Ҷ� ������ �˼��ִ�.

// �ؽ�Ʈ�� ���� ��´�. (���ٿ����� �������� �����Ѵ�)
int MMGetWidth(MFont *pFont,const char* szText, int nSize, bool bColorSupport=false);

// ���� ���� ���� ��ġ�� �����Ѵ�
int MMGetNextLinePos(MFont *pFont,const char* szText, int nWidth, bool bAutoNextLine=true, bool bColorSupport=false);

/// nLine ���� �����ϴ� ��ġ�� ����. nIndentation�� 2����� �鿩���� �ȼ���
int MMGetLinePos(MFont *pFont,const char* szText, int nWidth, bool bAutoNextLine=true, bool bColorSupport=false,int nLine=1,int nIndentation=0);

/// ������ ���� ���� ���Ѵ�.  TextMultiLine�� ���� �Լ�.
int MMGetLineCount(MFont *pFont,const char* szText, int nWidth, bool bAutoNextLine=true, bool bColorSupport=false,int nIndentation=0);

} // namespace mint3
