#include "stdafx.h"
#include "AutoEditValidator.h"

CString CGeneralValidate::validate(CString& str)
{
    return(_T(""));
}

CString CColorPickerValidate::validate(CString& str)
{
    if (atoi(str) > 255) {
        return("RGB�� ������ 255 �����Դϴ�");
    }

    return(_T(""));
}
