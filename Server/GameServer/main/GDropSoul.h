#pragma once

class GDropSoul : public MTestMemPool<GDropSoul>
{
public:
	GDropSoul();

	void Clear();
	bool IsAuthorizedCID(int nCID);

public:
	SOUL_TYPE nSoulType;
	int nAmount;
	int nAuthorizedCID;	// ȹ�� ������ �ִ� ����� CID
};
