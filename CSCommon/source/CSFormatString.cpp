#include "stdafx.h"
#include "CSFormatString.h"
#include "AString.h"
#include "MLocale.h"
#include "FSContext.h"

CStringResMgr* CSFormatString::sm_pDictionary = NULL;

tstring CSFormatString::Format(tstring formater, FSParam param)
{
	// �����ڵ�� ó���Ѵ�. TCHAR�� Ŭ���̾�Ʈ �� ���带 ���� �������̽����� ����Ѵ�.
	wstring strFormater = MLocale::ConvTCHARToUTF16(formater.c_str());
	wstring strParam = MLocale::ConvTCHARToUTF16(param.m_strParam.c_str());
	wstring strSeparator = MLocale::ConvTCHARToUTF16(FSP_SEPARATOR);

	
	// �ܺ� ���� ����	
	vector<wstring> vecParam;
	crl::AString::Split(strParam, strSeparator, vecParam);


	// �Ľ�
	FSContext context(vecParam, sm_pDictionary);

	for each(wchar_t token in formater)
	{
		context.Dispatch(token);
	}

	wstring strResult = context.GetResult();


	// �����ڵ� ��� tstring���� ��ȯ
	return MLocale::ConvUTF16ToTCHAR(strResult.c_str());
}

void CSFormatString::SetDictionary(CStringResMgr* pDictionary)
{
	sm_pDictionary = pDictionary;
}
