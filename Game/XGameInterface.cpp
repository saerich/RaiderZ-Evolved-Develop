/*
	XGameInterface

	���� �������̽��� ���õ� �ڵ带 �����մϴ�.
	������ �������� UI�� �ٷ�� ���ٴ� ������ ��ü ��� ���� ������ ������ �ΰ� �ۼ��մϴ�.
*/


#include "stdafx.h"
#include "XGameInterface.h"




XGameInterface::XGameInterface()
{
}


XGameInterface::~XGameInterface()
{
}

XGameInterface& XGameInterface::GetInstance()
{
	static XGameInterface	instance;
	return instance;
}
