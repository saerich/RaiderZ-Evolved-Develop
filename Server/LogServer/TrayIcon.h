#ifndef _TRAYICON_H
#define _TRAYICON_H

#include <shellapi.h>

/// ���ø����̼� Ʈ���� ������
class CTrayIcon
{
private:
protected:
	NOTIFYICONDATA	m_tnd;
public:
	CTrayIcon();
	virtual ~CTrayIcon();

	BOOL Create(HWND hWnd, UINT uCallbackMessage, LPCTSTR szToolTip, HICON icon, UINT uID);
	BOOL SetIcon(HICON hIcon);
	BOOL SetTooltipText(LPCTSTR pszTip);
	void RemoveIcon();
};
#endif
