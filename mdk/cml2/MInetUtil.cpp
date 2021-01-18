#include "stdafx.h"
#include "Winsock2.h"
#include "MInetUtil.h"
#include <iostream>
#include <ws2tcpip.h>

void MConvertCompactIP(char* szOut, const char* szInputDottedIP)
{
	in_addr addr{};
	InetPton(AF_INET, szInputDottedIP, &addr.S_un.S_addr);
	sprintf(szOut, "%03u%03u%03u%03u", addr.S_un.S_un_b.s_b1, addr.S_un.S_un_b.s_b2, 
											addr.S_un.S_un_b.s_b3, addr.S_un.S_un_b.s_b4);
}