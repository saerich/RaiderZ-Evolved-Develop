#ifndef _REAL_INPUT_H
#define _REAL_INPUT_H

#include <windows.h>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

struct RInputKeyBuffer
{
	BYTE nScanCode;
	bool bPressed;
};

struct RMouseState
{
	LONG    lX;
	LONG    lY;
	LONG    lZ;
	BYTE    rgbButtons[8];
	bool	bMouseButtonStates[8];
};

// madduck [5/15/2006]
struct RMouseData		//DIDEVICEOBJECTDATA�� ����..
{
	DWORD       dwOfs;
	DWORD       dwData;
	DWORD       dwTimeStamp;
	DWORD       dwSequence;
#if(DIRECTINPUT_VERSION >= 0x0800)
	UINT_PTR    uAppData;
#endif // DIRECTINPUT_VERSION >= 0x0800 
};
#define MOUSEDATA_BUFFER_SIZE 16

class RJoystick
{
public:
	virtual void Update() {}

	// pov ���� ������
	virtual unsigned int GetJoystickPovCount()		{ return 0; }
	// ��ư ���� ������
	virtual unsigned int GetJoystickButtonCount()	{ return 0; }

	// TODO: ���̽�ƽ�� ������ �����Ҷ� ���� �������̽��� �Լ����� �߰�����
};

class RKeyboard
{
public:
	virtual const char* GetKeyName(unsigned long int nKey) { return NULL; }

	/// ��ĵ�ڵ� �� Down/Up �÷��� ����, ���� ���� ������ŭ �����Ѵ�.
	virtual DWORD		GetBufferedData(RInputKeyBuffer* pBuffer) { return 0; }
};

class RMouse
{
public:
	virtual RMouseState* GetState() { return NULL; }
	virtual RMouseData*	GetData() { return NULL; }
};

/// �Է� �ý���
class RealInput
{
public:
	enum CreateArgs
	{
		USE_DKEYBOARD	= 0x01,
		USE_DMOUSE		= 0x02,
		USE_DJOYSTICK	= 0x03
	};
protected:

	RKeyboard*				m_pKeyboard;
	RMouse*					m_pMouse;
	RJoystick*				m_pJoystick;
	unsigned long			m_nFlags;

public:
	RealInput();
	virtual ~RealInput();

	bool Create(HWND hWnd, unsigned long flags = USE_DKEYBOARD | USE_DMOUSE | USE_DJOYSTICK);
	void Destroy();
	void Update();


	RJoystick*				GetJoystick()		{ return m_pJoystick; }
	RKeyboard*				GetKeyboard()		{ return m_pKeyboard; }
	RMouse*					GetMouse()			{ return m_pMouse; }
};

#endif