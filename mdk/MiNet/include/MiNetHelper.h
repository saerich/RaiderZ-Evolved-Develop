#pragma once

#include "MiNetLib.h"

namespace minet {

/// MiNet Helper Ŭ����
class MINET_API MiNetHelper
{
public:
	/// . �ִ� IP ���ڿ��� . ���� IP ���ڿ�(12����Ʈ)�� ��ȯ
	static void ConvertCompactIP(char* szOut, const char* szInputDottedIP);

	static unsigned long ConvertIPAddress(const char* szIP);
	static char* ConvertIPAddress(unsigned long nIP);

	static string GetHostIPString();
};

} // namespace minet