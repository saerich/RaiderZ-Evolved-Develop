#ifndef _RTOOL_HELPER_H
#define _RTOOL_HELPER_H

#include "RTypes.h"

namespace rs3 {

// ���� �����ϴ� ���� Ŭ�����Դϴ�. 
// DLL�� ������ �ʿ��� �͵� ������ �־����ϴ�. - bird
class RS_API RToolHelper
{
public:
// 	static void DrawLine(RVector* vec,int size,DWORD color);
// 	static void DrawGrid(const RVector pos,float w,float h,int cnt_x,int cnt_y,DWORD color);
	static void DrawCenter(float size_w,float size_h,DWORD color);
};


}






#endif
