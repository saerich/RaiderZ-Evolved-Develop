#pragma once

#include "RD3DApplication.h"
#include "XConst.h"
#include "RMeshMgr.h"
#include "XInputDevice.h"
#include "XGameFrame.h"
#include "XGameFrameManager.h"
#include "XBandiCapturer.h"
#include "MCommandLine.h"
#include "Minet.h"

#ifdef _KOREA
	#include "XYouthSelfRegulation.h"
#endif // _KOREA

class XGlobalInstance;
class XGlobalInfoInstance;
class XGameInitializer;

using namespace rs3;
using namespace mint3;
using namespace minet;

enum _EXECUTE_MODE
{
	EM_GAME = 0,		///< ���� ����
	EM_LOCAL_TEST,		///< ���� �׽�Ʈ
	EM_REPLAY,			///< ���÷���
	EM_DUMMYCLIENT,		///< ��Ʈ��ũ �׽�Ʈ�� ���� Ŭ���̾�Ʈ
	EM_ENGINE_DEBUG,	///< ����(�׷���/���� ���� ��) ����׿� ���
	EM_CUSTOM_FRAME,
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ���ø����̼� Ŭ����
//
class XBaseApplication : public RD3DApplication
{
protected:
	XGameInitializer*		m_pInitializer;

	wchar_t					m_szCmdLine[256];		///< ���ڰ�
	cml2::MCommandLine		m_CommandLine;			///< ���ڰ�

	_EXECUTE_MODE			m_nMode;				///< ���� ���

	XGameFrameManager		m_GameFrameMgr;			///< ���������� ������


	XGlobalInstance*		m_pGlobalInstance;
	XGlobalInfoInstance*	m_pGlobalInfoInstance;

	float					m_fLossElapsedTime;

	HANDLE					m_Mutex;

	RFILLMODE				m_fillMode;

#ifdef _KOREA
//	XYouthSelfRegulation	m_YouthSelfRegulation;
#endif // _KOREA

	void					DeclGlobalVals();
	void					InitPath();
	void					InitDebug();
	void					InitWindowCaption();
	virtual bool			InitGlobalInstance();

	bool					StartGameFrame();

	virtual bool			ParseArguments();
	wstring					ParseFileNameFromArguments();

	void					CheckMoveSecondTestWindow();
	virtual void			CreateGlobal();
	static void				UpdateProfileInfoCallback(const wchar_t* szText,void* pUserPointer);

	virtual void			CreateGameFrames();

	bool					CheckSecondWindow();

	virtual DWORD			GetWindowStyle(bool bFullScreen);
protected:

	virtual RSceneManager*	CreateSceneManager();	// �� �Ŵ��� ����
	virtual bool			OnCreateSceneManager();
	virtual void			OnDestroySceneManager();

	virtual void			UpdateTimer();

	virtual bool			OnCreate();
	virtual void			OnDestroy();
	virtual void			OnActive(bool bActive);

	virtual void			OnUpdate();
	virtual void			OnPreRender();
	virtual void			OnRender();
	virtual void			OnPreFlip();
	virtual bool			OnWndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pReturn);

public:
	XBaseApplication();
	virtual					~XBaseApplication();

	void					Quit(int nRet=0);
	void					Init(LPWSTR lpCmdLine);

	// frame ����
	void					ChangeFrame(int nFrameID);
	int						GetCurrGameFrame();
	void					MessageToFrame(const wchar_t* szMsg);

	void					SetClipboard(const wchar_t* szBuf);

	virtual void			DeviceSetting();
	virtual bool			ResetDevice();


	int						TestRun();
	void					TestDestroy();

	virtual char*			GetName();
	const wchar_t*			GetArguments()			{ return m_szCmdLine; }
	_EXECUTE_MODE			GetMode()				{ return m_nMode; }
	MUID					GenerateLocalUID();

	const cml2::MCommandLine&		GetCommandLine()		{ return m_CommandLine; }

	virtual const wchar_t*		GetProductVersion() { return L"1.0"; }
	virtual const wchar_t*		GetSVNRevision()	{ return L"1"; }
	virtual const wchar_t*		GetSVNDate()		{ return L"2010-02-18 07:04:14"; }

	void					ToggleSolidWire();

	XGameInitializer*		GetInitializer()	{ return m_pInitializer; }
	XGlobalInstance*		GetGlobalInstance()	{ return m_pGlobalInstance; }
	XGlobalInfoInstance*	GetGlobalInfoInstance() { return m_pGlobalInfoInstance; }

};

