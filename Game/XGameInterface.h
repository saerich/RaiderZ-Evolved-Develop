/*
	XGameInterface

	���� �������̽��� ���õ� �ڵ带 �����մϴ�.
	������ �������� UI�� �ٷ�� ���ٴ� ������ ��ü ��� ���� ������ ������ �ΰ� �ۼ��մϴ�.
*/


#pragma once

#ifndef _XGAME_INTERFACE
#define _XGAME_INTERFACE


#include "XGameInterfaceOption.h"



/// class XGameInterface
class XGameInterface
{
// Member variables
public:
	XGameInterfaceOption		OPTIONS;



// Member functions
public:
	XGameInterface();
	virtual ~XGameInterface();

	static XGameInterface& GetInstance();
};




// Inline
inline XGameInterface&	XGetGameInterface()
{
	return XGameInterface::GetInstance();
}


#endif
