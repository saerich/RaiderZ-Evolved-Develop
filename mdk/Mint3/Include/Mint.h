#pragma once

#ifdef WIN32
#include <windows.h>
#include <imm.h>
#endif

#include <map>
using namespace std;

#include "MintTypes.h"
#include "MEvent.h"
#include "MWidgetManager.h"
#include "MToolTip.h"

class MWLua;

namespace mint3 {

// madduck [4/4/2006] - �Ѱ��� ��������.
struct MBitmapRep
{
	string	strBitmapName;
	MRECT	m_rCoord[9];

	MBitmapRep()
	{
		memset(m_rCoord, 0, sizeof(MRECT)*9);
	}
};


struct MPickStrOnMouseMove
{
	MWidget*	m_pSender;
	MPOINT		m_ptPoint;
	string		m_strString;

	void Set( MWidget* pSender, MPOINT& ptPoint, string strString)
	{
		if ( m_pSender  ||  pSender == NULL  ||  strString.empty())
			return;

		m_pSender = pSender;
		m_ptPoint = ptPoint;
		m_strString = strString;
	}

	void Clear()				{ m_pSender = NULL;		m_strString.clear();	}
};

struct MTooltipInfo : public MPickStrOnMouseMove		{};
struct MHelpMsgInfo : public MPickStrOnMouseMove		{};


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

// madduck [4/5/2006]
class MLookManager;

#define MINT_VERSION	3
#define	MVersion()	MINT_VERSION

/// first KeyValue, second ActionID
typedef map<unsigned long int, int>	ACTIONKEYMAP;

/// Global Event Callback
typedef bool(MGLOBALEVENTCALLBACK)(MEvent* pEvent);


class MMainFrame;

/// Mint Global Object Class
class Mint{

protected:
	friend class MWidget;

	// madduck [4/4/2006]
	friend class MintLoader;
	MLookManager*			m_pLookManager;

	static Mint*			m_pInstance;				///< Instance
	MMainFrame*				m_pMainFrame;				///< Main Frame Widget
	MDrawContext*			m_pDC;						///< DrawContext
	MGLOBALEVENTCALLBACK*	m_fnGlobalEventCallBack;	///< Global Event Callback function
	MWidgetManager*			m_pWidgetManager;
	MScriptManager*			m_pScriptManager;			///< Lua ��ũ��Ʈ ����
	MButtonGroupManager*	m_pButtonGroupManager;		///< ��ư�� �׷� ������

	//////////////////////////////////////////////////////////////////////////
	// ���ҽ� �����ڵ�, 
	// TODO: ��Ʈ�� ���ʵ� �������� �������
	MAnimationTemplateManager*	m_pAnimationTemplateManager;	///< �ִϸ��̼� ���ø� ������

	MTooltip*				m_pTooltip;
	MTooltipInfo			m_TooltipInfo;

	// Drag & Drop
//	int						m_nDragObjectID;
	char					m_szDragObjectString[256];
	char					m_szDragObjectItemString[256];
	MBitmap*				m_pDragObjectBitmap;
	MPOINT					m_GrabPoint;
	bool					m_bVisibleDragObject;
	MWidget*				m_pDropableObject;
	MWidget*				m_pDragSourceObject;

	// Workspace Size
	int						m_nWorkspaceWidth;
	int						m_nWorkspaceHeight;
	// Standard Size
	int						m_nStandardWidth;
	int						m_nStandardHeight;
	float					m_fRatioToStandardScreenWidth;
	float					m_fRatioToStandardScreenHeight;

	/// Action Map
	ACTIONKEYMAP			m_ActionKeyMap;
#define ACTIONKEYMAP_IDCOUNT	256
	bool					m_ActionKeyPressedTable[ACTIONKEYMAP_IDCOUNT];

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
	/// ������ ����� ���� �ΰ������� �׸��� �͵��� �ִ�.
	void DrawDesignerMode( MDrawContext* pDC );
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

	/// Event Action Key
	bool EventActionKey(unsigned long int nKey, bool bPressed);

	/// Update(Redraw)
	virtual void Update(void){}

	/// Get MainFrame Widget
	MMainFrame* GetMainFrame(void);

	/// Get DrawContext
	MDrawContext* GetDrawContext(void);

	/// Singleton Instance
	static Mint* GetInstance(void);

	// Set tooltip
	void SetTooltip( MWidget* pWidget, MPOINT& pos, const string& strMsg)		{ m_TooltipInfo.Set( pWidget, pos, strMsg);		}
	void SetTooltipLook( MTooltipLook* pLook)									{ m_pTooltip->ChangeCustomLook( pLook);			}


#ifdef WIN32
	// ������ �ڵ� ����
	void SetHWND(HWND hWnd);
	HWND GetHWND(void);

	HIMC m_hImc;			// IME Handle, IME�� Enable/Disable�ϱ� ���� ������. IME���� �Լ��� ������ ImmGetContext()�� �� ����Ѵ�. ����, Disable�Ǿ� ������, ImmGetContext()�� NULL�� ������ ���̴�.
	//DWORD m_dwIMEConvMode;	// ������ IME ���
#endif

	void EnableIME(bool bEnable);
	bool IsEnableIME(void);
	void EnableCharInput(bool bEnable);
	bool IsEnableCharInput();

	/// ��Ű �߰�
	int RegisterHotKey(unsigned long int nModifier, unsigned long int nVirtKey);
	/// ��Ű ����
	void UnregisterHotKey(int nID);

	/// �׼Ǹ� �߰�
	bool RegisterActionKey(int nActionID, unsigned long int nKey);
	/// �׼Ǹ� ����
	bool UnregisterActionKey(int nActionID);

	/// Ű ��ȣ�� Ű �̸� �˾Ƴ���
	virtual const char* GetActionKeyName(unsigned long int nKey);
	/// ActionID�� Ű ��ȣ �˾Ƴ���
	unsigned long int GetActionKey(int nActionID);

	/// �׼� ��ư�� ���ȴ°�?
	bool IsActionKeyPressed(int nActionID);

	// Drag & Drop
	MWidget* SetDragObject(MWidget* pSender, MBitmap* pBitmap, const char* szString, const char* szItemString);
	MWidget* GetDragObject(void);

	virtual MWidget* NewWidget(const char* szClass, const char* szName, MWidget* pParent, MListener* pListener);

	MWidget* FindWidgetDropAble(MPOINT& p);
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
	virtual MFont*		NewFont(const char* pszAliasName, const char* pszFontName, const char* pszFileName, int nHeight, bool bOutline, float fOutlineOpacity =1.0f, bool bAntiAlias =false) = 0;
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
	// �������ϸ���
	virtual void BeginDrawWidget(const char* szClassName) {}
	virtual void EndDrawWidget(const char* szClassName) {}
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

// Designer Mode
void MSetDesignerMode(bool bEnable);
bool MIsDesignerMode(void);

int MGetDesignerModeSnapSize(void);
void MSetDesignerModeSnapSize(int s);

class MResourceMap;

bool MSaveDesignerMode(const char* szFileName=NULL, MResourceMap* pRM=NULL);
bool MLoadDesignerMode(const char* szFileName=NULL, MResourceMap* pRM=NULL);
void MResetDesignerMode(MResourceMap* pRM=NULL);
void MUpdateDesignerMode(void);
void MShowDesignerMenu(MPOINT& p, MWidget* pCaller);

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

#define MIsActionKeyPressed(_ActionID)	(Mint::GetInstance()->IsActionKeyPressed(_ActionID))

#define IsHangul(x) ((unsigned char)(x)>127) 

} // namespace mint3

