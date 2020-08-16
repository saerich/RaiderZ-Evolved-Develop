#ifndef _XINPUT_DEVICE_H
#define _XINPUT_DEVICE_H

#include "RealInput.h"
#include "RMouse.h"

class XInputDevice
{
private:
protected:
	RealInput*		m_pRealInput;
public:
	XInputDevice();
	virtual ~XInputDevice();
	bool Create(HWND hWnd);
	void Destroy();
	void Update(float fDelta);

	// Ű����
	bool IsKeyDown(int vKey);

	// ���콺
	bool GetLButtonState(void);
	bool GetMButtonState(void);
	bool GetRButtonState(void);


	RealInput* GetRealInput() { return m_pRealInput; }

	RMouseData*		GetMouseData();
	RMouseState*	GetMouseState();

};



#endif // _XINPUT_H