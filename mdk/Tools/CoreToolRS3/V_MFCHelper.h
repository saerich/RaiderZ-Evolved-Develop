#pragma once

#include "RTypes.h"
#include "d3d9.h"
#include <vector>
#include "DLL_Interface.h"

class CControlMediator;

class CStatusBarHandler;
class CCoreToolStatusBar;

class CORETOOL_API CMFCHelper
{
public:

	//////////////////////////////////////////////////////////////////////////
	//
	// Init / Destroy
	//
	//////////////////////////////////////////////////////////////////////////

	static void InitMFCHelper(CControlMediator* _pControlMediator, CCoreToolStatusBar* _pStatusBar);
	static void DestroyMFCHelper();


	//////////////////////////////////////////////////////////////////////////
	//
	// ���� ��ȭ����
	//
	//////////////////////////////////////////////////////////////////////////

	static void OpenToolOptionDlg();
	static void OpenShortCutList();
	static void OpenMapBatchScrip();

	template< typename T_Dlg >
	static T_Dlg* CreateDLLDlgModalOKed()
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg;
		return DoModal_(pDlg);
	}

	template< typename T_Dlg, typename T_Param >
	static T_Dlg* CreateDLLDlgModalOKed(T_Param _param)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg(_param);
		return DoModal_(pDlg);
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2 >
	static T_Dlg* CreateDLLDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2);
		return DoModal_(pDlg);
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2 >
	static T_Dlg* CreateAppDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2)
	{
		HINSTANCE hInstResourceDll = AfxGetResourceHandle();
		HINSTANCE hInstResourceClient = (HINSTANCE) ::GetModuleHandle(CORETOOL_DLL_NAME);

		AfxSetResourceHandle(hInstResourceClient);
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2);
		pDlg = DoModal_(pDlg);
		AfxSetResourceHandle(hInstResourceDll);
		return pDlg;
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2, typename T_param_3 >
	static T_Dlg* CreateDLLDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2, T_param_3 _param_3)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2, _param_3);
		return DoModal_(pDlg);
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2, typename T_param_3 >
	static T_Dlg* CreateAppDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2, T_param_3 _param_3)
	{
		HINSTANCE hInstResourceDll = AfxGetResourceHandle();
		HINSTANCE hInstResourceClient = (HINSTANCE) ::GetModuleHandle(CORETOOL_DLL_NAME);

		AfxSetResourceHandle(hInstResourceClient);
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2, _param_3);
		pDlg = DoModal_(pDlg);
		AfxSetResourceHandle(hInstResourceDll);
		return pDlg;
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2, typename T_param_3, typename T_Param_4 >
	static T_Dlg* CreateDLLDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2, T_param_3 _param_3, T_Param_4 _param_4)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2, _param_3, _param_4);
		return DoModal_(pDlg);
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2, typename T_param_3, typename T_Param_4, typename T_Param_5 >
	static T_Dlg* CreateDLLDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2, T_param_3 _param_3, T_Param_4 _param_4, T_Param_5 _param_5)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2, _param_3, _param_4, _param_5);
		return DoModal_(pDlg);
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2, typename T_param_3, typename T_Param_4, typename T_Param_5, typename T_Param_6 >
	static T_Dlg* CreateDLLDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2, T_param_3 _param_3, T_Param_4 _param_4, T_Param_5 _param_5, T_Param_6 _param_6)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2, _param_3, _param_4, _param_5, _param_6);
		return DoModal_(pDlg);
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2, typename T_param_3, typename T_Param_4, typename T_Param_5, typename T_Param_6, typename T_Param_7 >
	static T_Dlg* CreateDLLDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2, T_param_3 _param_3, T_Param_4 _param_4, T_Param_5 _param_5, T_Param_6 _param_6, T_Param_7 _param_7)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2, _param_3, _param_4, _param_5, _param_6, _param_7);
		return DoModal_(pDlg);
	}

	template< typename T_Dlg, typename T_Param_1, typename T_Param_2, typename T_param_3, typename T_Param_4, typename T_Param_5, typename T_Param_6, typename T_Param_7, typename T_Param_8 >
	static T_Dlg* CreateDLLDlgModalOKed(T_Param_1 _param_1, T_Param_2 _param_2, T_param_3 _param_3, T_Param_4 _param_4, T_Param_5 _param_5, T_Param_6 _param_6, T_Param_7 _param_7, T_Param_8 _param_8)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		T_Dlg* pDlg = new T_Dlg(_param_1, _param_2, _param_3, _param_4, _param_5, _param_6, _param_7, _param_8);
		return DoModal_(pDlg);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// XML ����Ʈ ��ȭ����
	//
	//////////////////////////////////////////////////////////////////////////

	// muti tab
	static void OpenZoneListEditorWithField();
	static void OpenFieldListEditorWithZone();

	// one tab
	static void OpenFieldListEditor();
	static void OpenZoneListEditor();
	static void OpenNpcListEditor();
	static void OpenPropertyListEditor();


	//////////////////////////////////////////////////////////////////////////
	//
	// DX Helper
	//
	//////////////////////////////////////////////////////////////////////////

	static bool D3DTextureToHDC(HDC hDC, LPDIRECT3DTEXTURE9 lpTexture, RECT *pTargetRect, bool bDrawAlpha = false);
	static void SetMFCSlider(CSliderCtrl *pCtrl, int iMin, int iMax, int iPos);
	static bool GetFileFromPathWithFilter(std::vector<std::string> &vecFilenames, const char *pPathname, const char *szFilter, bool bGetOnlyFileName = false);
	static bool FileExist(const char *szFilename);
	static bool EqualFloat(const float &fA, const float &fB);

	static void FillWnd(CWnd *pWnd, DWORD dwR, DWORD dwG, DWORD dwB);
	static void FillWnd(CWnd *pWnd, COLORREF _color);

	static inline BYTE GetAlpha( D3DCOLOR Color ) { return (BYTE)( Color >> 24 ); }
	static inline BYTE GetRed( D3DCOLOR Color ) { return (BYTE)( Color >> 16 ); }
	static inline BYTE GetGreen( D3DCOLOR Color ) { return (BYTE)( Color >> 8  ); }
	static inline BYTE GetBlue( D3DCOLOR Color ) { return (BYTE)( Color );       }

	// bitmap ���ҽ��κ��� �ؽ��ĸ� �����Ѵ�.
	static rs3::RTexture* CreateTextureFromResourceBitmap(rs3::RDevice *pDevice, UINT uiResID);

	template <typename T>
	static T BiLinearInterpolation(float dx, float dy, const T &vLT, const T &vRT, const T &vLB, const T &vRB)
	{
		T valueA = vLT*(1.0f-dx) + vRT*dx;
		T valueB = vLB*(1.0f-dx) + vRB*dx;

		return ( valueA*(1.0f-dy) + valueB*dy );
	}

	// �ܺ� ���α׷� ���� Wrapping �Լ�
	static BOOL Exec(LPCSTR lpExeFile, LPCSTR lpParam, BOOL bShow, DWORD dwWaitTime);

	// �Ϲ� �׷��� ������ DXT �� ��ȯ�Ѵ�. pcDestFilename �� NULL �̸�, ���� ���� �̸����� Overwrite �Ѵ�.
	static bool ConvertImageToDXT(const char *pcSrcFilename, const char *pcDestFilename = NULL);


	//////////////////////////////////////////////////////////////////////////
	//
	// Work Area ( Terrain Brush���� ��� )
	//
	//////////////////////////////////////////////////////////////////////////

	static void	RegisterBrushWorkArea(CSliderCtrl* _pCtrl, float* _pEditSize, CString* _pViewSize);
	static void	SetBrushWorkAreaSize(int _size);
	static int	GetBrushWorkAreaSize();

private:

	//�̱���
	CMFCHelper(CControlMediator* _pControlMediator);
	~CMFCHelper(void);

	// static
	static CMFCHelper* m_pMFCHelper_;

	// member
	CControlMediator* m_pControlMediator_;
	HINSTANCE m_hSciDll_;

	// work area
	typedef std::vector< std::pair<CSliderCtrl*, std::pair< float*, CString* > > > BRUSH_WORK_AREA;
	BRUSH_WORK_AREA m_vecRelatedBrushWorkArea_;

	// DoModal
	template< typename T_DlgPtr >
	static T_DlgPtr DoModal_(T_DlgPtr _dlg)
	{
		if (_dlg->DoModal() == IDOK)
			return _dlg;

		delete _dlg;
		return NULL;
	}
};

//////////////////////////////////////////////////////////////////////////
//
// �ð��� ��� Ŭ����
//
//////////////////////////////////////////////////////////////////////////

class CTimeCheck
{
private:
	DWORD m_dwConstructTime;
public:
	CTimeCheck(const char *pName);
	~CTimeCheck();
};
