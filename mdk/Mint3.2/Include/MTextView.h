#pragma once

#include "MWidget.h"
#include "MScrollBar.h"
#include "MLookBase.h"
#include <string>
#include <vector>


namespace mint3
{


class MTextView;




/// class : __chunk

// Chunk types
enum _chunk_type
{
	_CT_NONE	= 0,
	_CT_STRING,			// String
	_CT_BITMAP,			// Bitmap
	_CT_SPACE,			// Null space
	_CT_FONT,			// Font
	_CT_RFONT,			// Restore font
	_CT_COLOR,			// Color
	_CT_RCOLOR,			// Restore color
	_CT_ALIGN,			// Align
	_CT_COLUMN,			// Height of column
	_CT_PRGSPC,			// Paragraph spacing
	_CT_LINESPC,		// Line spacing
	_CT_LINEFEED,		// Line feed
	_CT_CRETURN,		// Carriage return
	_CT_CRTHEIGHT,		// Carriage return with height
	_CT_INDENT,			// First line indent
	_CT_OUTDENT,		// First line outdent
	_CT_CLRDENT,		// Clear first line indent
	_CT_SECONDENT,		// Second line indent
	_CT_END				// End of chunk
};

// Chunk
class __chunk
{
public:
	__chunk ()				{}
	virtual ~__chunk ()		{}
	virtual int _gettype()				{ return _CT_NONE;			}		// Ÿ���� ����
	virtual const char* _getcdata()		{ return "";				}		// char �����͸� ����
	virtual size_t _getlength()			{ return 0;					}		// ���̸� ����
	virtual int _getwidth()				{ return 0;					}		// ���̸� ����
	virtual int _getheight()			{ return 0;					}		// ���̸� ����
	virtual int _getspace()				{ return 0;					}		// �����̽��� ���̸� ����
	virtual bool _isdummy()				{ return false;				}		// ���� chunk���� ���θ� ����
	virtual unsigned char _getref()		{ return 0;					}		// ���� �ε����� ����
};

// String
class _str_chunk : public __chunk
{
public:
	string			_str;
	short			_w, _h;
	short			_space;
	bool			_dummy;
	unsigned char	_ref;

	_str_chunk( const string& str, unsigned char tref) : _str( str), _w( 0), _h( 0), _space( 0), _dummy( false), _ref( tref) {}
	virtual int _gettype()				{ return _CT_STRING;		}
	const char* _getcdata()				{ return _str.c_str();		}
	size_t _getlength()					{ return _str.length();		}
	int _getwidth()						{ return _w;				}
	int _getheight()					{ return _h;				}
	int _getspace()						{ return _space;			}
	bool _isdummy()						{ return _dummy;			}
	unsigned char _getref()				{ return _ref;				}
};

// Bitmap
class _bmp_chunk : public __chunk
{
public:
	string			_bmp;
	short			_w, _h;
	unsigned char	_ref;


	_bmp_chunk( const string& bmp, short w, short h, unsigned char bref) : _bmp( bmp), _w( w), _h( h), _ref( bref) {}
	virtual int _gettype()				{ return _CT_BITMAP;		}
	size_t _getlength()					{ return 0;					}
	int _getwidth()						{ return _w;				}
	int _getheight()					{ return _h;				}
	unsigned char _getref()				{ return _ref;				}
};

// Null space
class _spc_chunk : public __chunk
{
public:
	short			_w, _h;
	unsigned char	_ref;

	_spc_chunk( short w, short h, unsigned char sref) : _w( w), _h( h), _ref( sref) {}
	virtual int _gettype()				{ return _CT_SPACE;			}
	size_t _getlength()					{ return 0;					}
	int _getwidth()						{ return _w;				}
	int _getheight()					{ return _h;				}
	unsigned char _getref()				{ return _ref;				}
};

// Font
class _fnt_chunk : public __chunk
{
public:
	string			_fnt;

	_fnt_chunk( const string& fnt) : _fnt( fnt) {}
	virtual int _gettype()				{ return _CT_FONT;			}
};

// Restore font
class _rfn_chunk : public __chunk
{
public:
	_rfn_chunk() {}
	virtual int _gettype()				{ return _CT_RFONT;			}
};

// Color
class _col_chunk : public __chunk
{
public:
	unsigned char	_r, _g, _b, _a;

	_col_chunk( unsigned char r, unsigned char g, unsigned char b, unsigned char a =255) : _r( r), _g( g), _b( b), _a( a) {}
	virtual int _gettype()				{ return _CT_COLOR;			}
};

// Restore color
class _rco_chunk : public __chunk
{
public:
	_rco_chunk() {}
	virtual int _gettype()				{ return _CT_RCOLOR;		}
};

// Align
class _aln_chunk : public __chunk
{
public:
	unsigned char	_horz;
	unsigned char	_vert;

	_aln_chunk( unsigned char horz, unsigned char vert) : _horz( horz), _vert( vert) {}
	virtual int _gettype()				{ return _CT_ALIGN;			}
};

// Height of column
class _hco_chunk : public __chunk
{
public:
	unsigned char	_h;			// 0�̸� �ڵ�

	_hco_chunk( int height) : _h( (unsigned char)height) {}
	virtual int _gettype()				{ return _CT_COLUMN;		}
};

// Paragraph spacing
class _prs_chunk : public __chunk
{
public:
	int				_spacing;

	_prs_chunk( int spacing =-1) : _spacing( spacing) {}
	virtual int _gettype()				{ return _CT_PRGSPC;		}
};

// Line spacing
class _lns_chunk : public __chunk
{
public:
	unsigned char	_spacing;

	_lns_chunk( int spacing) : _spacing( spacing) {}
	virtual int _gettype()				{ return _CT_LINESPC;		}
};

// Line feed
class _lnf_chunk : public __chunk
{
public:
	_lnf_chunk() {}
	virtual int _gettype()				{ return _CT_LINEFEED;		}
};

// Carriage return
class _crt_chunk : public __chunk
{
public:
	_crt_chunk() {}
	virtual int _gettype()				{ return _CT_CRETURN;		}
};

// Carriage return with height
class _crh_chunk : public __chunk
{
public:
	unsigned char	_h;

	_crh_chunk( int height) : _h( (unsigned char)height) {}
	virtual int _gettype()				{ return _CT_CRTHEIGHT;		}
};

// Indent
class _idt_chunk : public __chunk
{
public:
	int				_w;

	_idt_chunk( int w) : _w( w) {}
	virtual int _gettype()				{ return _CT_INDENT;		}
};

// Indent
class _odt_chunk : public __chunk
{
public:
	int				_w;

	_odt_chunk( int w) : _w( w) {}
	virtual int _gettype()				{ return _CT_OUTDENT;		}
};

// Clear indent
class _cdt_chunk : public __chunk
{
public:
	_cdt_chunk() {}
	virtual int _gettype()				{ return _CT_CLRDENT;		}
};

// Second indent
class _sid_chunk : public __chunk
{
public:
	int				_w;

	_sid_chunk( int w) : _w( w) {}
	virtual int _gettype()				{ return _CT_SECONDENT;		}
};

// End of chunk
class _end_chunk : public __chunk
{
public:
	_end_chunk() {}
	virtual int _gettype()				{ return _CT_END;			}
};

typedef vector< __chunk*>		MChunkList;








/// class : MLineContents
class MLineContents
{
/// Reference content info
public:
	struct MREFINFO
	{
		int				_ref;
		float			_x;
		float			_w;

		MREFINFO( int _ref_id, float x, float w) : _ref( _ref_id), _x( x), _w( w) {}
	};



/// Member variables
protected:
	vector< __chunk*>	m_LineContents;				// ���� ������
	unsigned char		m_nCount;					// ������ ����
	unsigned short		m_nWidth;					// ���� ��ü ����
	unsigned char		m_nColumnHeight;			// �� ����
	unsigned char		m_nHeight;					// ��ü ����
	unsigned char		m_nAlign;					// ���� ����
	unsigned char		m_nSpaceCount;				// �����̽��� ����
	unsigned char		m_nLastSpace;				// ������ �����̽��� ����
	unsigned char		m_nIndent;					// �鿩���� ��ġ
	float				m_fStartPos;				// ���� ��ġ
	float				m_fSpaceAddition;			// ���� ����ġ
	vector< MREFINFO*>	m_RefInfo;					// ���� ������ ���� ����



/// Member function
public:
	MLineContents();
	virtual ~MLineContents();

	bool AddContents( __chunk* pContents);

	__chunk* GetContents( int nIndex)				{ return m_LineContents[ nIndex];					}
	int GetCount() const							{ return (int)m_nCount;								}

	// ��ü ����/���̸� ����
	int GetWholeWidth() const						{ return (int)m_nWidth;								}

	// ������ �����̽��� �� ��ü ���̸� ����
	int GetWidth() const							{ return ((int)m_nWidth - (int)m_nLastSpace);		}
	
	// �� ���̸� ������
	void SetColumnHeight( int nHeight)				{ m_nColumnHeight = (int)nHeight;					}
	int GetColumnHeight() const						{ return (int)m_nColumnHeight;						}

	// ��ü ���̸� ������
	void SetHeight( int nHeight)					{ m_nHeight = (int)nHeight;							}
	int GetHeight() const							{ return (int)m_nHeight;							}

	// ������ ���� �� ����
	virtual void SetAlign( MAlignmentMode align)	{ m_nAlign = align;									}
	virtual MAlignmentMode GetAlign() const			{ return m_nAlign;									}

	// ���ο� ����ִ� ó���� �����̽��� ����
	int GetSpaceCount() const						{ return ( (m_nLastSpace == 0) ? m_nSpaceCount : (m_nSpaceCount - 1));	}

	// �鿩���⸦ ���� �� ����
	void SetIndent( int indent)						{ m_nIndent = (unsigned char)indent;				}
	int GetIndent() const							{ return m_nIndent;									}

	// ������ġ�� ����
	float GetStartPos() const						{ return m_fStartPos;								}

	// ���� ����ġ�� ����
	float GetSpaceAddition() const					{ return m_fSpaceAddition;							}

	// ���� ���� ������ ����
	size_t GetRefInfoCount() const					{ return m_RefInfo.size();							}
	int GetRefInfoRef( int i) const					{ return m_RefInfo[ i]->_ref;						}
	MRECT GetRefInfoRect( int i) const				{ return MRECT( (int)m_RefInfo[ i]->_x, 0, (int)m_RefInfo[ i]->_w, (int)m_nColumnHeight);	}

	// ���� ���� ������Ʈ
	void UpdateLineInfo( MRECT& r);

	// ��� ����
	void Clear();
};










/// class : MTextViewLook
class MTextViewLook : public MLookBase
{
/// Member variables
protected:
	int		m_nColumnHeight;
	int		m_nParagraphSpacing;
	int		m_nLineSpacing;
	int		m_nIndent;
	int		m_nSecondent;



/// Member functions
public:
	MTextViewLook();

	void SetColumnHeight( int height)				{ m_nColumnHeight = height;				}
	int GetColumnHeight() const						{ return m_nColumnHeight;				}

	void SetParagraphSpacing( int spacing)			{ m_nParagraphSpacing = spacing;		}
	int GetParagraphSpacing() const					{ return m_nParagraphSpacing;			}

	void SetLineSpacing( int spacing)				{ m_nLineSpacing = spacing;				}
	int GetLineSpacing() const						{ return m_nLineSpacing;				}

	void SetIndent( int indent)						{ m_nIndent = indent;					}
	int GetIndent() const							{ return m_nIndent;						}

	void SetSecondent( int secondent)				{ m_nSecondent = secondent;				}
	int GetSecondent() const						{ return m_nSecondent;					}

	virtual void OnDraw( MWidget* pWidget, MDrawContext* pDC) override;

protected:
	virtual void OnDrawText( MTextView* pTextView, MDrawContext* pDC);
};






/// struct MRefContent
struct MRefContect		{ int i;  MRECT r; };



/// class : MTextView
class MTextView : public MWidget
{
/// Structure of reference
protected:
	struct MTEXTVIEWREF
	{
		string		strText;
		string		strToolTip;

		MTEXTVIEWREF( const string& _text, const string& _tooltip) : strText( _text), strToolTip( _tooltip) {}
	};



/// Member variables
protected:
	MScrollBar*					m_pScrollbar;
	MChunkList					m_ChunkList;
	vector< MLineContents*>		m_LineContents;
	vector< MTEXTVIEWREF>		m_Reference;
	unsigned char				m_nReferenceIndex;
	unsigned char				m_nCaret;
	unsigned char				m_nMouseOver;
	MSIZE						m_PageSize;
	bool						m_bWrap;
	bool						m_bDoParseText;
	bool						m_bUpdateScrollBar;
	bool						m_bUpdateLineContents;
	bool						m_bDragAndDrop;



/// Member functions
public:
	MTextView( const char* szName =NULL, MWidget* pParent =NULL, MListener* pListener =NULL);
	virtual ~MTextView();

	// ������ ����� ���ϴ� �Լ�
	MSIZE GetPageSize();

	// �ؽ�Ʈ ���� �� ����
	virtual void SetText( const char* szText);
	virtual const char* GetText( void);

	/// ���� �޽��� ���� �� ����
	virtual void SetToolTipText( const char* szTooltip);
	virtual const char* GetToolTipText() const;

	// �ؽ�Ʈ�� �����ϴ� �Լ�
	void Clear();
	void ClearChunkList();
	void ClearLineContents();

	// ����ִ��� Ȯ���ϴ� �Լ�
	bool IsEmpty() const										{ return m_ChunkList.empty();						}

	// �ܾ� ������ ����(Wrapping)�� �ϴ��� ���θ� �����ϴ� �Լ�
	void SetWrapping( bool bWrap);
	bool IsWrapping() const										{ return m_bWrap;									}

	// ����
	size_t GetRefCount();
	const char* GetRefText( int index)							{ return m_Reference[ index].strText.c_str();		}
	const char* GetRefToolTip( int index)						{ return m_Reference[ index].strToolTip.c_str();	}
	MRECT GetRefRect( int index);

	// ĳ���� ��ġ�ϰ� �ִ� ���� �ε����� ����
	int SetCaretPos( int index);
	int GetCaretPos()											{ return (int)m_nCaret;								}

	// ���콺�� ��ġ�ϰ� �ִ� ���� �ε���
	int GetMouseOver() const									{ return (int)m_nMouseOver;							}

	// ���� ������ ����
	size_t GetLineContentsCount()								{ return m_LineContents.size();						}
	MLineContents* GetLineContents( size_t index)				{ return m_LineContents[ index];					}

	/// �巡�� �� ���
	virtual void EnableDragAndDrop( bool bEnable) override		{ m_bDragAndDrop = bEnable;							}
	virtual bool IsEnableDragAndDrop() const override			{ return m_bDragAndDrop;							}

	virtual MScrollBar* GetScrollBar() override					{ return m_pScrollbar;								}
	void ScrollPage( int delta);


protected:
	// �ؽ�Ʈ�� �Է¹����� ��Ʈ���� �±׸� �Ľ��Ͽ� chunk�� �����ϴ� �Լ�
	bool ParseText( const wchar_t* c);

	// �и��� chunk�� ����ϱ� ���ϵ��� ���� ������ �籸���ϴ� �Լ�. ���ϰ����� ȭ�� ������ �Ѿ���� ���θ� �Ѱ���
	bool UpdateLineContents( bool bTestMode =false);

	// �ش� ��ǥ�� �ش��ϴ� ���� �������� ã�´�
	MRefContect FindRefContent( MPOINT& pos);

	// ��ũ�ѹٸ� ������Ʈ �ϴ� �Լ�
	void UpdateScrollbar()										{ m_bUpdateScrollBar = true;						}



protected:
	bool _parse_tag( const wchar_t* c);
 	bool _parse_bmp( const wchar_t* c);
 	bool _parse_spc( const wchar_t* c);
 	bool _parse_fnt( const wchar_t* c);
 	bool _parse_rfn( const wchar_t* c);
 	bool _parse_col( const wchar_t* c);
 	bool _parse_rco( const wchar_t* c);
 	bool _parse_aln( const wchar_t* c);
	bool _parse_hco( const wchar_t* c);
	bool _parse_prs( const wchar_t* c);
	bool _parse_lns( const wchar_t* c);
	bool _parse_crt( const wchar_t* c);
	bool _parse_idt( const wchar_t* c);
	bool _parse_odt( const wchar_t* c);
	bool _parse_cdt( const wchar_t* c);
	bool _parse_sid( const wchar_t* c);
	bool _parse_ref( const wchar_t* c);
	bool _parse_erf( const wchar_t* c);




/// Event handler
protected:
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener) override;
	virtual void OnChangedLookAndFill() override;
	virtual void OnRun( DWORD dwCurrTime) override;
	virtual void OnSize(int w, int h) override;
	virtual MWidget* OnToolTip() override;
	virtual void OnLeave() override;



/// Lua script
public:
	void glueClear()											{ Clear();											}
	int glueGetRefCount( int index)								{ return (int)GetRefCount();						}
	const char* glueGetRefText( int index)						{ return GetRefText( index);						}
	const char* glueGetRefToolTip( int index)					{ return GetRefToolTip( index);						}
	MWLua::ret_int4 glueGetRefRect( int index);
	int glueSetCaretPos( int index)								{ return SetCaretPos( index);						}
	int glueGetCaretPos()										{ return GetCaretPos();								}
	int glueGetMouseOver()										{ return GetMouseOver();							}
	void glueScrollPage( int delta)								{ ScrollPage( delta);								}
	MWLua::ret_int2 glueGetPageSize();



#define MINT_TEXTVIEW	"TextView"
	virtual const char* GetClassName( void)						{ return MINT_TEXTVIEW;								}
};

} // namespace mint3