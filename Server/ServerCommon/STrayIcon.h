#pragma once

#include <shellapi.h>
#include "ServerCommonLib.h"

/// ���ø����̼� Ʈ���� ������
class SCOMMON_API STrayIcon
{
private:
protected:
	NOTIFYICONDATA	m_tnd;
public:
	STrayIcon();
	virtual ~STrayIcon();

	BOOL Create(HWND hWnd, UINT uCallbackMessage, const wchar_t* szToolTip, HICON icon, UINT uID);
	BOOL SetIcon(HICON hIcon);
	BOOL SetTooltipText(LPCTSTR pszTip);
	void RemoveIcon();
};

