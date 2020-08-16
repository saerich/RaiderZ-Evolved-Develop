#pragma once
#include "RApplication.h"
#include "RDebuggingInfo.h"

namespace rs3 {

class RS_API RD3DApplication : public RApplication
{

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	int			m_nIconResID;		///< ������ ���ҽ� ID. ��� ���� Application �����ڿ��� �������ָ� �ȴ�.
	DWORD		m_dwWindowStyle;
	RECT		m_rcWindowBounds;

	void			AdjustWindowSizeAndShowWindow( int x, int y, int nWidth, int nHeight, bool bFullScreen);

	virtual RSceneManager* CreateSceneManager();
	virtual bool	CreateDevice();
	virtual void	DestroyDevice();
	virtual DWORD	GetWindowStyle(bool bFullScreen);
public:
	RD3DApplication(void);
	virtual ~RD3DApplication(void);


	virtual bool	ProcessMessage(MSG&  msg);
	// ���̻� �޽����� ó���� �ʿ䰡 ������ pReturn�� ������޽����� ���ϰ��� �ְ� true�� ����
	virtual bool	OnWndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pReturn) { return false; }

	virtual int		MainLoop();
	virtual void	Activate(bool bActive);

	virtual void	OnLostDevice();
	virtual void	OnResetDevice();

	void			ToggleFullScreen();


private:
	RDebuggingInfo	m_DebuggingInfo;

};

}
