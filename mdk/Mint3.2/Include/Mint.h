#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <map>
using namespace std;


#include "MintTypes.h"
#include "MEvent.h"
#include "MWidgetManager.h"
#include "MDragManager.h"
#include "MToolTipManager.h"
#include "MTimerManager.h"

class MWLua;

namespace mint3 {



class MDrawContext;
class MWidget;
class MFont;
class MBitmap;
class MCursor;
class MIDLResource;
class MListener;
class MEvent; 
class MintLoader;
class MScriptManager;
class MButtonGroupManager;
class MAnimationTemplateManager;
class MLookBase;
class MLookManager;

#define MINT_VERSION	3
#define	MVersion()	MINT_VERSION


/// Global Event Callback
typedef bool(MGLOBALEVENTCALLBACK)(MEvent* pEvent);


class MMainFrame;

/// Mint Global Object Class
class Mint{

protected:
	friend class MWidget;

	// madduck [4/4/2006]
	friend class MintLoader;

	HWND					m_hWnd;

	static Mint*			m_pInstance;				///< Instance
	MMainFrame*				m_pMainFrame;				///< Main Frame Widget

	MDrawContext*			m_pDC;						///< DrawContext
	DWORD					m_dwUpdateTime;				///< Update time
	MGLOBALEVENTCALLBACK*	m_fnGlobalEventCallBack;	///< Global Event Callback function

	MLookManager*			m_pLookManager;				///< Look manager
	MWidgetManager*			m_pWidgetManager;			///< Widget manager
	MScriptManager*			m_pScriptManager;			///< Lua ��ũ��Ʈ ����
	MDragManager			m_DragManager;				///< Drag and Drop manager
	MToolTipManager			m_ToolTipManager;			///< ToolTip Manager
	MTimerManager			m_TimerManager;				///< Timer manager
	MButtonGroupManager*	m_pButtonGroupManager;		///< ��ư�� �׷� ������
	MWidget*				m_pPopupMenu;				///< Popup menu

	bool					m_bShowCursor;				///< Show cursor
	MPOINT					m_ptLastCursorPos;			///< ������ Ŀ�� ��ġ

	//////////////////////////////////////////////////////////////////////////
	// ���ҽ� �����ڵ�, 
	// TODO: ��Ʈ�� ���ʵ� �������� �������
	MAnimationTemplateManager*	m_pAnimationTemplateManager;	///< �ִϸ��̼� ���ø� ������

	// Workspace Size
	int						m_nWorkspaceWidth;
	int						m_nWorkspaceHeight;
	// Standard Size
	int						m_nStandardWidth;
	int						m_nStandardHeight;
	float					m_fRatioToStandardScreenWidth;
	float					m_fRatioToStandardScreenHeight;

	// Mint profiler
	bool					m_bStartMintProfiler;

	// IME ����
	void*	m_pCandidateList;		// Candidate List, LPCANDIDATELIST�� ĳ�����ؼ� ����Ѵ�.
	int		m_nCandidateListSize;	// Candidate List Size
	MPOINT	m_CandidateListPos;		// Candidate List�� ������ ��ġ
	bool	m_bEnableIME;
	bool	m_bEnableCharInput;		// ä�� Ȱ��
public:
	DWORD	m_nCompositionAttributeSize;				// Composition ���ڿ��� �Ӽ� ����
	BYTE	m_nCompositionAttributes[MIMECOMPOSITIONSTRING_LENGTH];		// Composition ���ڿ��� �Ӽ�
	//DWORD	m_dwCompositionClauses[MIMECOMPOSITIONSTRING_LENGTH];		// Composition ���ڿ��� �Ӽ��� ���� ���ڿ� ����
	int		m_nCompositionCaretPosition;				// Composition ���ڿ������� ĳ�� ��ġ


protected:
	/// Candidate List �׸���, �������� �������� �ʰ� ���� �׸���
	virtual void DrawCandidateList(MDrawContext* pDC, MPOINT& p);


public:
	Mint(void);
	virtual ~Mint(void);

	/// Mint Initialize
	bool Initialize(int nWorkspaceWidth, int nWorkspaceHeight, int nStandardWidth, int nStandardHeight,
					MDrawContext* pDC, MFont* pDefaultFont, MWLua* pLua=NULL);
	/// Mint Finalize
	void Finalize(void);

#ifdef WIN32
	/// Event Processing
	bool ProcessEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
#endif
	bool ProcessEvent(MEvent& e);

	/// Mint Run
	virtual void Run(void);
	/// Mint Draw
	virtual void Draw(void);

	/// Update(Redraw)
	virtual void Update(void){}

	/// Get MainFrame Widget
	MMainFrame* GetMainFrame(void);

	/// Get DrawContext
	MDrawContext* GetDrawContext(void);

	/// Singleton Instance
	static Mint* GetInstance(void);



	// ������ �ڵ� ����
	void SetHWND(HWND hWnd);
	HWND GetHWND(void);

	HIMC m_hImc;


	void EnableIME(bool bEnable);
	bool IsEnableIME(void);
	void EnableCharInput(bool bEnable);
	bool IsEnableCharInput();
	void ClearImeCompose();

	MToolTipManager* GetToolTipManager()			{ return &m_ToolTipManager;			}
	void ClearToolTip()								{ m_ToolTipManager.Reset();			}
	void ChangedToolTipText( MWidget* pWidget);

	MTimerManager* GetTimerManager()				{ return &m_TimerManager;			}

	// Popup Menu
	bool SetPopupMenu( MWidget* pPopupMenu);
	bool ClearPopupMenu();
	MWidget* GetPopupMenu()							{ return m_pPopupMenu;				}

	// Drag and drop manager
	MDragManager* GetDragManager()					{ return &m_DragManager;			}

	bool ShowCursor( bool bShow);
	bool IsShowCursor() const						{ return m_bShowCursor;				}
	// Get last cursor position
	MPOINT GetLastCursorPos() const					{ return m_ptLastCursorPos;			}

	// Reset mouse state
	void ResetMouseState();

	// Start mint profiler
	void StartMintProfiler();

	// Drag and drop manager
	virtual MWidget* NewWidget(const char* szClass, const char* szName, MWidget* pParent, MListener* pListener);

	MWidget* FindWidget(MPOINT& p);
	MWidget* FindWidget(int x, int y);
	MWidget* FindWidget(const char* szName);

	/// Mint�� ���۵� ȭ���� ���� ũ�� ���
	int GetWorkspaceWidth(void);
	/// Mint�� ���۵� ȭ���� ���� ũ�� ���
	int GetWorkspaceHeight(void);
	/// Mint�� ������ �Ǵ� ȭ���� ���� ũ�� ���
	int GetStandardWidth(void);
	/// Mint�� ������ �Ǵ� ȭ���� ���� ũ�� ���
	int GetStandardHeight(void);
	/// Mint�� ���۵� ȭ���� ũ�� ����
	void SetWorkspaceSize(int w, int h);
	/// Mint�� ������ �Ǵ� ȭ�� ũ��
	void SetStandardSize(int w, int h);

	/// �ý��ۿ� �´� Bitmap Open Function�� �����ؾ� �Ѵ�.
	virtual MBitmap*	NewBitmap(const char* szName, const char* szFileName) = 0;
	/// �ý��ۿ� �´� Font Open Function�� �����ؾ� �Ѵ�.
	virtual MFont*		NewFont( const char* szName, const char* szFontName, const char* szFileName, int nHeight, bool bBold =false, bool bItalic =false, bool bAntiAlias =false, float fOutline =0.0f, float fScale =1.0f) = 0;
	virtual MFont*		NewFontMemory( const char* szName, const char* szFontName, const char* szFileName, int nHeight, bool bBold =false, bool bItalic =false, bool bAntiAlias =false, float fOutline =0.0f, float fScale =1.0f) = 0;
	/// �ý��ۿ� �´� cursor
	virtual MCursor*	NewCursor(const char* pszAliasName);

	void SetGlobalEvent(MGLOBALEVENTCALLBACK pGlobalEventCallback);

	// IME ���� �Լ���
	/// ���� �� ���� �⺻ ��Ʈ ���
	const char* GetDefaultFontName(void) const;

	/// �� ��� ID ���
	/// LANG_KOREAN, LANG_JAPANESE, LANG_CHINESE, etc... in "winnt.h"
	int GetPrimaryLanguageIdentifier(void) const;

	/// �� ��� ID ���
	/// �߱��� ��ü, ��ü�� ��� ���ؼ��� �� ������ �ʿ�
	/// SUBLANG_KOREAN, SUBLANG_CHINESE_TRADITIONAL, SUBLANG_CHINESE_SIMPLIFIED, ... in "winnt.h"
	int GetSubLanguageIdentifier(void) const;

	/// �ε�������(En, ��, ��, �� ��) ���
	const char* GetLanguageIndicatorString(void) const;

	/// Native ���� �����Ǿ� �ִ°�?
	bool IsNativeIME(void) const;

	/// WM_IME_NOTIFY�� IMN_OPENCANDIDATE ����, Candidate List �˾��� �� ȣ��
	void OpenCandidateList(void);
	/// WM_IME_NOTIFY�� IMN_CLOSECANDIDATE ����, Candidate List �˾��� ���� �� ȣ��
	void CloseCandidateList(void);

	// Candidate List ���� �Լ���
	/// �ε��������� Candidate ���ڿ� ���
	const char* GetCandidate(int nIndex) const;
	/// Candidate ���ڿ� �� ���� ���
	int GetCandidateCount(void) const;
	/// Candidate ���ڿ��� �� ���� ���õ� ���ڿ� �ε��� ���
	int GetCandidateSelection(void) const;
	/// ���� �������� ������ Candidate ���ڿ��� ù��° �ε��� ���
	int GetCandidatePageStart(void) const;
	/// �� �������� ������ Candidate ���ڿ��� ���� ���
	int GetCandidatePageSize(void) const;

	/// Candidate List�� ������ ��ġ �����ϱ�
	/// p�� ���յǴ� ������ ���� ��ġ
	/// �Ʒ��� ������ �����ϸ� ���� �������� �ϹǷ� ������ ���̸� �˾ƾ� �Ѵ�.
	void SetCandidateListPosition(MPOINT& p, int nWidgetHeight);
	/// Candidate List �ʺ�
	int GetCandidateListWidth(void);
	/// Candidate List ����
	int GetCandidateListHeight(void);

	/// Composition �Ӽ� ���� ���
	DWORD GetCompositionAttributeSize(void) const { return m_nCompositionAttributeSize; }
	/// Composition �Ӽ� ���
	const BYTE* GetCompositionAttributes(void) const { return m_nCompositionAttributes; }
	//const DWORD* GetCompositionClauses(void) const { return m_dwCompositionClauses; }

	/// �������� �Ӽ� �׸���
	/// p�� szComposition[i]�� ��ġ
	int DrawCompositionAttribute(MDrawContext* pDC, MPOINT& p, const char* szComposition, int i);
	/// p�� Composition�� �����ϴ� ��ġ
	void DrawCompositionAttributes(MDrawContext* pDC, MPOINT& p, const char* szComposition);
	/// ���ʿ� �ε������� �׸���
	void DrawIndicator(MDrawContext* pDC, MRECT& r);

	/// DC ��ȯ
	MDrawContext* GetDC()		{ return m_pDC; }

	/// ������Ʈ �ð� ��ȯ
	DWORD GetUpdateTime() const	{ return m_dwUpdateTime;	}


	// XML �Ľ� ���� ---------
	void Clear();

	// Lua ���� --------------
	MScriptManager*		GetScriptManager()	{ return m_pScriptManager; }

	MWidgetManager*		GetWidgetManager()	{ return  m_pWidgetManager;}

	MWidget* FindWidget(string& strName)	{ return m_pWidgetManager->FindWidget(strName); }
	void FindWidgets(MWidgetList& widgetList, string& strName) { m_pWidgetManager->FindWidgets(widgetList, strName); }

	// �ػ󵵿� ���� ���� ���, �ػ󵵰� ���ϸ� ���� �������� �̷���� ���
	void TransformScreenSizeRatioToStandardSize(int &nx, int &ny, int &nWidth, int &nHeight);
	// �ػ󵵿� ���� ���� ��
	float GetRatioToStandardScreenWidth() { return m_fRatioToStandardScreenWidth; }
	float GetRatioToStandardScreenHeight() { return m_fRatioToStandardScreenHeight; }

	MLookManager *		GetLookManager() { return m_pLookManager; };
	MAnimationTemplateManager* GetAnimationTemplateManager() { return m_pAnimationTemplateManager; }
	//////////////////////////////////////////////////////////////////////////

	// Enabled lua script
	void EnableCallScript( bool bEnable);
	bool IsEnableCallScript();
};

/// Mint �ν��Ͻ� ��ȯ
inline Mint* MGetMint(void) {
	return Mint::GetInstance();
}

/// Mint�� ���۵� ȭ���� ���� ũ�� ���
inline int MGetWorkspaceWidth(void){
	return Mint::GetInstance()->GetWorkspaceWidth();
}
/// Mint�� ���۵� ȭ���� ���� ũ�� ���
inline int MGetWorkspaceHeight(void){
	return Mint::GetInstance()->GetWorkspaceHeight();
}


class MResourceMap;

// Sample
void MCreateSample(void);
void MDestroySample(void);


#ifdef WIN32
#define MMODIFIER_ALT	MOD_ALT
#define MMODIFIER_CTRL	MOD_CONTROL
#define MMODIFIER_SHIFT	MOD_SHIFT
#else
#define MMODIFIER_ALT	1
#define MMODIFIER_CTRL	2
#define MMODIFIER_SHIFT	4
#endif

#define IsHangul(x) ((unsigned char)(x)>127) 


 
// TODO: MLocale�� ��ü�մϴ�. ���� �����ϰڽ��ϴ�.  [2010/9/28 praptor]
//std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc = std::locale());
//std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale());

bool SetClipboard( const char* text);
bool GetClipboard( char* text, size_t size);

} // namespace mint3

