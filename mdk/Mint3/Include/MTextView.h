#pragma once

#include "MWidget.h"
#include "MDrawContext.h"
#include "MScrollBar.h"
#include <string>
#include <vector>


namespace mint3
{



/*************************************************************************
 (class) Chunk
**************************************************************************/
// Chunk types
enum _chunk_type
{
	_CT_NONE	= 0,
	_CT_STRING,		// String
	_CT_BITMAP,		// Bitmap
	_CT_SPACE,		// Null space
	_CT_FONT,		// Font
	_CT_RFONT,		// Restore font
	_CT_COLOR,		// Color
	_CT_RCOLOR,		// Restore color
	_CT_ALIGN,		// Align
	_CT_COLUMN,		// Height of column
	_CT_LINEFEED,	// Line feed
	_CT_CRETURN,	// Carriage return
	_CT_INDENT,		// Indent
	_CT_OUTDENT,	// Outdent
	_CT_CLRDENT		// Clear indent
};

// Chunk
class __chunk
{
public:
	__chunk ()				{}
	virtual ~__chunk ()		{}
	virtual int _gettype()			{ return _CT_NONE;			}		// Ÿ���� ����
	virtual const char* _getcdata()	{ return "";				}		// char �����͸� ����
	virtual size_t _getlength()		{ return 0;					}		// ���̸� ����
	virtual int _getwidth()			{ return 0;					}		// ���̸� ����
	virtual int _getheight()		{ return 0;					}		// ���̸� ����
	virtual int _getspace()			{ return 0;					}		// �����̽��� ���̸� ����
};

// String
class _str_chunk : public __chunk
{
public:
	string		_str;
	short		_w, _h;
	short		_space;

	_str_chunk( const string& str) : _str( str), _w( 0), _h( 0), _space( 0)  {}
	virtual int _gettype()			{ return _CT_STRING;		}
	const char* _getcdata()			{ return _str.c_str();		}
	size_t _getlength()				{ return _str.length();		}
	int _getwidth()					{ return _w;				}
	int _getheight()				{ return _h;				}
	int _getspace()					{ return _space;			}
};

// Bitmap
class _bmp_chunk : public __chunk
{
public:
	string		_bmp;
	short		_w, _h;

	_bmp_chunk( const string& bmp, short w, short h) : _bmp( bmp), _w( w), _h( h) {}
	virtual int _gettype()			{ return _CT_BITMAP;		}
	size_t _getlength()				{ return 0;					}
	int _getwidth()					{ return _w;				}
	int _getheight()				{ return _h;				}
};

// Null space
class _spc_chunk : public __chunk
{
public:
	short		_w, _h;

	_spc_chunk( short w, short h) : _w( w), _h( h) {}
	virtual int _gettype()			{ return _CT_SPACE;			}
	size_t _getlength()				{ return 0;					}
	int _getwidth()					{ return _w;				}
	int _getheight()				{ return _h;				}
};

// Font
class _fnt_chunk : public __chunk
{
public:
	string		_fnt;

	_fnt_chunk( const string& fnt) : _fnt( fnt) {}
	virtual int _gettype()			{ return _CT_FONT;			}
};

// Restore font
class _rfn_chunk : public __chunk
{
public:
	_rfn_chunk() {}
	virtual int _gettype()			{ return _CT_RFONT;			}
};

// Color
class _col_chunk : public __chunk
{
public:
	unsigned char	_r, _g, _b, _a;

	_col_chunk( unsigned char r, unsigned char g, unsigned char b, unsigned char a =255) : _r( r), _g( g), _b( b), _a( a) {}
	virtual int _gettype()			{ return _CT_COLOR;			}
};

// Restore color
class _rco_chunk : public __chunk
{
public:
	_rco_chunk() {}
	virtual int _gettype()			{ return _CT_RCOLOR;		}
};

// Align
class _aln_chunk : public __chunk
{
public:
	unsigned char	_align;

	_aln_chunk( unsigned char align) : _align( align) {}
	virtual int _gettype()			{ return _CT_ALIGN;			}
};

// Height of column
class _hco_chunk : public __chunk
{
public:
	unsigned char	_height;		// 0�̸� �ڵ�

	_hco_chunk( int height) : _height( (unsigned char)height) {}
	virtual int _gettype()			{ return _CT_COLUMN;		}
};

// Line feed
class _lnf_chunk : public __chunk
{
public:
	_lnf_chunk() {}
	virtual int _gettype()			{ return _CT_LINEFEED;		}
};

// Carriage return
class _crt_chunk : public __chunk
{
public:
	_crt_chunk() {}
	virtual int _gettype()			{ return _CT_CRETURN;		}
};

// Indent
class _idt_chunk : public __chunk
{
public:
	_idt_chunk() {}
	virtual int _gettype()			{ return _CT_INDENT;		}
};

// Outdent
class _odt_chunk : public __chunk
{
public:
	_odt_chunk() {}
	virtual int _gettype()			{ return _CT_OUTDENT;		}
};

// Clear indent
class _cdt_chunk : public __chunk
{
public:
	_cdt_chunk() {}
	virtual int _gettype()			{ return _CT_CLRDENT;		}
};

typedef vector< __chunk*>		MChunkList;




/*************************************************************************
 (class) MLineContents
**************************************************************************/
class MLineContents
{
protected:
	vector< __chunk*>	m_LineContents;				// ���� ������
	unsigned char		m_nCount;					// ������ ����
	unsigned short		m_nWidth;					// ���� ��ü ����
	unsigned char		m_nHeight;					// ���� ��ü ����
	unsigned char		m_nAlign;					// ���� ����
	unsigned char		m_nSpaceCount;				// �����̽��� ����
	unsigned char		m_nLastSpace;				// ������ �����̽��� ����
	unsigned char		m_nTrimHead;				// ó�� contents�� ���� ���ڿ� �ڸ� ��ġ
	unsigned char		m_nTrimTail;				// ������ contents�� ���� ���ڿ� �ڸ� ��ġ
	unsigned char		m_nIndent;					// �鿩����
	bool				m_bHyphen;					// �������� ������ ���ڰ� ������ ����


public:
	MLineContents();
	virtual ~MLineContents();

	bool AddContents( __chunk* pContents);
	bool AddTrimHeadContents( __chunk* pContents, int trim_pos, int trim_width);
	bool AddTrimTailContents( __chunk* pContents, int trim_pos, int trim_width, bool hyphen);

	__chunk* GetContents( int index);
	int GetCount();

	int GetWidth();									// ������ �����̽��� �� ��ü ����
	int GetWholeWidth();							// ��ü ���̸� ����
	
	void SetHeight( int height);					// ��ü ���̸� ������
	int GetHeight();								// ��ü ���̸� ����
	
	void SetAlign( unsigned char align);			// ������ ������
	unsigned char GetAlign();						// ������ ����

	int GetSpaceCount();							// ���ο� ����ִ� ó���� �����̽��� ����

	int GetTrimHead();								// ó�� contents�� ���ڿ� �ڸ� ��ġ�� ����
	int GetTrimTail();								// ������ contents�� ���ڿ� �ڸ� ��ġ�� ����

	void SetIndent( int indent);					// �鿩���� �Ÿ�
	int GetIndent();								// �鿩���� �Ÿ��� ����	

	bool IsHyphen();								// ������'-' ���ڰ� �ִ��� ���θ� ����

	void Clear();
};




/*************************************************************************
 (class) MTextView
**************************************************************************/
class MTextView : public MWidget
{
// Member variables
protected:
	MScrollBar*					m_pScrollbar;
	int							m_nScrollbarVisible;
	MChunkList					m_ChunkList;
	vector< MLineContents*>		m_LineContents;
	MSIZE						m_PageSize;
	bool						m_bWrap;
	bool						m_bAutoScroll;
	DWORD						m_dwAutoScroll;



// Member functions
public:
	MTextView( const char* szName =NULL, MWidget* pParent =NULL, MListener* pListener =NULL);
	virtual ~MTextView();

	// Scrollbar
	void ShowScrollbar( int mode);
	void SetScrollbarLook( MScrollBarLook* pLook);
	void SetScrollbarWidth( int nWidth);
	
	// �������� ��(-)/�Ʒ�(+)�� ��ũ���ϴ� �Լ�
	void ScrollPage( int delta);

	// ������ ����� ���ϴ� �Լ�
	MSIZE GetPageSize() const;

	// ������ �ڵ� ��ũ�� �Լ�
	void StartAutoScroll();
	void StopAutoScroll();

	// �ܾ� ������ ����(Wrapping)�� �ϴ��� ���θ� �����ϴ� �Լ�
	void SetWrapping( bool bWrap);
	bool IsWrapping();




protected:
	// �ؽ�Ʈ�� �Է¹����� ��Ʈ���� �±׸� �Ľ��Ͽ� chunk�� �����ϴ� �Լ�
	bool ParseText( const char* c);

	// �и��� chunk�� ����ϱ� ���ϵ��� ���� ������ �籸���ϴ� �Լ�. ���ϰ����� ȭ�� ������ �Ѿ���� ���θ� �Ѱ���
	bool UpdateLineContents( bool bTestMode =false);

	// ��ũ�ѹٸ� ������Ʈ �ϴ� �Լ�
	void UpdateScrollbar();


public:
	// �ؽ�Ʈ�� �����ϴ� �Լ�
	void Clear();
protected:
	void ClearChunkList();
	void ClearLineContents();


protected:
	bool _parse_tag( const char* c);
	bool _parse_bmp( const char* c);
	bool _parse_spc( const char* c);
	bool _parse_fnt( const char* c);
	bool _parse_rfn( const char* c);
	bool _parse_col( const char* c);
	bool _parse_rco( const char* c);
	bool _parse_aln( const char* c);
	bool _parse_hco( const char* c);
	bool _parse_crt( const char* c);
	bool _parse_idt( const char* c);
	bool _parse_odt( const char* c);
	bool _parse_cdt( const char* c);


protected:
	MRECT GetWorkRect();


protected:
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener);
	virtual bool OnCommand( MWidget* pWidget, MEventMsg nMsg, const char* szArgs =NULL);
	virtual void OnRun( void);
	virtual void OnDraw( MDrawContext* pDC);
	virtual void OnSize(int w, int h);
	virtual void OnScrollbarChanged(int nPos);

public:
	virtual void SetText( const char* szText);
	virtual const char* GetText( void);


public:
	void glueClear();
	void glueScrollPage( int delta);
	int glueGetPageWidth();
	int glueGetPageHeight();
	void glueStartAutoScroll();
	void glueStopAutoScroll();



#define MINT_TEXTVIEW	"TextView"
	virtual const char* GetClassName( void)		{ return MINT_TEXTVIEW;		}
};

} // namespace mint3