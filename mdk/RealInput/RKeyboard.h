#ifndef _RKEYBOARD_H
#define _RKEYBOARD_H

#include "RealInput.h"
#include <string>

/// Keyboard Device
class RKeyboardImpl : public RKeyboard
{
private:

protected:
	friend class RealInput;
	RealInput*					m_pRealInput;
	LPDIRECTINPUTDEVICE8		m_pDevice;

#define KEYNAMETABLE_COUNT	256
	std::string					m_szKeyNameTable[KEYNAMETABLE_COUNT];
public:
								RKeyboardImpl();
	virtual						~RKeyboardImpl();

	bool						Create(LPDIRECTINPUT8 pDirectInput, HWND hWnd);
	void						Destroy();
	bool						IsEnabled()				{ return (m_pDevice != NULL); }

								/// 256���� ��ĵ�ڵ尡 �����ִ���(&0x80) �˻� 
	bool						ReadImmediateData(BYTE ScanCode[256]);	
								/// Ű���� ���۷� �о����
								/// ��ĵ�ڵ� �� Down/Up �÷��� ����, ���� ���� ������ŭ �����Ѵ�.
	DWORD						GetBufferedData(RInputKeyBuffer* pBuffer) override;

	const char* GetKeyName(unsigned long int nKey) override;
	
};


struct RAction
{

};

#endif // _RKEYBOARD_H