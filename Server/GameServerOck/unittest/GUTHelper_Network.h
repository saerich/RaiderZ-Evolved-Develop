#pragma once

class MockLink;
class GEntityPlayer;

class GUTHelper_Network
{
public:
	// ���ο� �Ը�ũ ����
	MockLink* NewLink(GEntityPlayer* pPlayer, bool bIgnoreAllSendCommandID = false);
	// ������ ��� �Ը�ũ�� ����
	void ClearLinks();
};
