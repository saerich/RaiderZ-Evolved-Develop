// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// ValidatorApp.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"

int CalLoagingCompletePercent( int nCurrentComplete, int nFullComplete, int nLoadingFullPercent /*= 100*/ )
{
	float fCompleteRatio = (float)nCurrentComplete / (float)nFullComplete;
	return (int)(fCompleteRatio * nLoadingFullPercent);
}
