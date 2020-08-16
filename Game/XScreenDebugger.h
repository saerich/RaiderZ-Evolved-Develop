#ifndef _XSCREEN_DEBUGGER_H
#define _XSCREEN_DEBUGGER_H

#pragma push_macro("_SECURE_SCL")
#pragma push_macro("_HAS_ITERATOR_DEBUGGING")
#pragma warning ( disable : 4005)

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0


#include "XGlobal.h"
#include <map>
#include <string>
using namespace std;
using namespace mint3;

/// ȭ�鿡 ����� ������ ����ϱ����� Ŭ����
/// 4���� ���̾ ���� ������ ����� �� �ִ�.
class XScreenDebugger
{
public:
	enum _Layer {
		LAYER1 = 0,
		LAYER2,
		LAYER3,
		LAYER4,
		LAYER5,
		LAYER6,
		LAYER7,
		LAYER8,			// MovableDebugger ����
		LAYER9,			// HDR Monitor

		LAYER_MAX,
		LAYER_ALL	= LAYER_MAX
	};
private:
	static const int		LINE_PIXEL = 20;				// �Ѷ����� 20�ȼ��� ����.

	list< pair<int, wstring>>	m_BufferMap[LAYER_ALL+1];		// �� ���̾��..
	bool					m_bVisible;
	int						m_nLayer;
	int						m_nEndLine[LAYER_ALL+1];
	bool					m_bReleaseTextOnDraw[LAYER_ALL+1];
	MPOINT					GetPosFromKey(int key);
	int						MakeKey(int x, int y);
	void					DrawLayer(MDrawContext *pDC, _Layer layer);
	void					InitEndLine();
public:
							/// ������
							XScreenDebugger();
							/// �Ҹ���
	virtual					~XScreenDebugger();
							/// ���ۿ� ��Ƴ��� ������ ���
	void					Draw(MDrawContext *pDC);
							/// ���̾� ����
	void					SelLayer(_Layer layer);
							/// ���� �����ִ� ���̾�
	_Layer					GetSelLayer();
							/// ȭ�鿡 ���������� ����
	void					Show(bool b=true, _Layer layer=LAYER1) { InitEndLine(); m_bVisible = b; m_nLayer=layer; }
							/// ����� ������ ����ϰ� �ִ��� ����
	bool					IsVisible()	{ return m_bVisible; }
							/// �ؽ�Ʈ ���. �ؽ�Ʈ�� ���ۿ� ��Ƴ��� ���� Draw�Լ����� ������ ȭ�鿡 ����Ѵ�.
	void					Print(int x, int y, const wchar_t* szText, _Layer layer=LAYER_ALL);
							/// �ؽ�Ʈ ���. y���� ������� ���پ� ����Ѵ�.
	void					PrintLine(int x, const wchar_t* szText, _Layer layer=LAYER_ALL);
	void					NextLayer();
	void					PrevLayer();
	void					SetLine(int y, _Layer layer=LAYER_ALL);
	void					SetReleaseTextOnDraw(bool b, _Layer layer);
	void					ReleaseText(_Layer layer);

							/// �ν��Ͻ� ��ȯ
	static XScreenDebugger*	GetInstance();
};

// ���Ǹ� ���� �Լ�

#define _MAKE_TEXT_SCRTEXT_		wchar_t text[1024];					\
								va_list args;						\
								va_start(args, pFormat);			\
								vswprintf(text, pFormat, args);		\
								va_end(args);						\
								
inline void _ScrText(int x, int y, const wchar_t *pFormat, ...) 
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->Print(x, y, text, XScreenDebugger::LAYER_ALL);
}
inline void _ScrText1(int x, int y, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->Print(x, y, text, XScreenDebugger::LAYER1);
}
inline void _ScrText2(int x, int y, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->Print(x, y, text, XScreenDebugger::LAYER2);
}
inline void _ScrText3(int x, int y, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;	
	XScreenDebugger::GetInstance()->Print(x, y, text, XScreenDebugger::LAYER3);
}
inline void _ScrText4(int x, int y, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->Print(x, y, text, XScreenDebugger::LAYER4);
}
inline void _ScrText5(int x, int y, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->Print(x, y, text, XScreenDebugger::LAYER5);
}
inline void _ScrText6(int x, int y, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->Print(x, y, text, XScreenDebugger::LAYER6);
}
inline void _ScrText7(int x, int y, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->Print(x, y, text, XScreenDebugger::LAYER7);
}
inline void _ScrLine(int x, const wchar_t *pFormat, ...) 
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER_ALL);
}
inline void _ScrLine1(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER1);
}
inline void _ScrLine2(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER2);
}
inline void _ScrLine3(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;	
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER3);
}
inline void _ScrLine4(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER4);
}
inline void _ScrLine5(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER5);
}
inline void _ScrLine6(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER6);
}
inline void _ScrLine7(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER7);
}
inline void _ScrLine8(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER8);
}
inline void _ScrLine9(int x, const wchar_t *pFormat, ...)
{
	_MAKE_TEXT_SCRTEXT_;
	XScreenDebugger::GetInstance()->PrintLine(x, text, XScreenDebugger::LAYER9);
}
inline XScreenDebugger::_Layer XScreenDebugger::GetSelLayer()		{ return XScreenDebugger::_Layer(m_nLayer); }

#pragma pop_macro("_SECURE_SCL")
#pragma pop_macro("_HAS_ITERATOR_DEBUGGING")

#endif // _XSCREEN_DEBUGGER_H