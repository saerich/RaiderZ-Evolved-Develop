#include "stdafx.h"
#include "PmAuthenticator.h"
#include "MMD5.h"
#include "MStringUtil.h"
#include "MLocale.h"

const int MD5_DATA_SIZE	= 16;

PmAuthenticator::PmAuthenticator(wstring strServerKey)
{	
	m_strServerKey = strServerKey;
}

bool PmAuthenticator::Verify(wstring strExpected, wstring strHashKeyStream) const
{
	// ���޹��� ���� ����Ű�� ���� MD5�� ��ȯ�Ѵ�.
	// ��ȯ���� ��ǥ���� �����ϸ� ����.
	wstring strInput = strHashKeyStream + L"|" + m_strServerKey;
	BYTE szOutput[MD5_DATA_SIZE];	
	
	string strAnsi = MLocale::ConvTCHARToAnsi(strInput.c_str());

	MMD5::md5_string((BYTE*)strAnsi.c_str(), strAnsi.length(), szOutput);
	wstring strHexString = cml2::DataToHexStringL(szOutput, MD5_DATA_SIZE);

	return strExpected == strHexString;
}
