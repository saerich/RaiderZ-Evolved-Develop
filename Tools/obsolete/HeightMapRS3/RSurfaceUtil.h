#pragma once

#include <d3d9.h>

class RSurfaceUtil
{
	// A, B�� ������ ũ���̾�� �Ѵ�.
	// A = A and B;
	static bool MaskingAnd(LPDIRECT3DSURFACE9 pSurfaceA, LPDIRECT3DSURFACE9 pSurfaceB);
};