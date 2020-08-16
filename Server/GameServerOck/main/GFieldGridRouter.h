#pragma once

#include "GFieldGrid.h"

class GEntityPlayer;
class GEntitySync;
class GField;

TEST_FORWARD_FT(FieldGrid, FGrid, RouteToHugeSector);
TEST_FORWARD_FT(FieldGrid, FGrid, CheckHeight);

class GFieldGridRouter
{
	TEST_FRIEND_FT(FieldGrid, FGrid, RouteToHugeSector);
	TEST_FRIEND_FT(FieldGrid, FGrid, CheckHeight);
public:
	void SendToMe(GEntityPlayer* pPlayer, const vector<GFieldGrid::Sector*>& vecNewCells);

	static void RouteToCell(const GEntitySync& sender, GField& field, MCommand* const pCommand, const MPoint& sectorPt, int nCellSize, float fBaseHeight, const MUID& uidExcept=MUID::Invalid());
	void RouteToField(GField* pField, MCommand* pCommand);

	template<typename T>
	MCommandParameter* NewSplitParameter(vector<T> vecValue, int nSplitIndex, int nDivideNumber)
	{
		// ��� ���
		int nSplitCount = vecValue.size() / nDivideNumber;
		int nModCount = vecValue.size() % nDivideNumber;

		if (0 < nModCount)
		{
			nSplitCount++;
		}

		// �ε����� �ʰ��ϸ� �� �Ķ����
		if (nSplitCount <= nSplitIndex)
		{
			return NEW_BLOB();
		}

		// ������ �ε����� �������� ������.
		int nCount = nDivideNumber;
		if (nModCount != 0)
		{
			// nDivedeNumber��ŭ �߶󺸳��� ���� ¥���� �ε����� �ִٸ�, ¥������ŭ�� ������.
			if (nSplitIndex == nSplitCount - 1)
			{
				nCount = nModCount;
			}
		}

		return NEW_BLOB(&vecValue[nSplitIndex * nDivideNumber], sizeof(vector<T>::value_type), nCount);
	}

private:
	static bool CollectCellReceivers( vector<MUID>& outvecEntities, const GEntitySync& sender, GField& field, MCommand* const pCommand, const MPoint& sectorPt, int nCellSize, float fBaseHeight, const MUID& uidExcept=MUID::Invalid() );
	static void CollectEssentialReceivers(bool bCrowdFiltered, vector<MUID>& outvecEntities, const GEntitySync& sender, MCommand* pCommand, const MUID& uidExcept);
	static bool CheckHeight(float nBaseHeight, const MUID& uidTargetActor, GField& field);
};


template<typename T>
MCommandParameter* NewSplitParameter(vector<T> vecValue, int nSplitIndex, int nDivideNumber)
{
	// ��� ���
	int nSplitCount = vecValue.size() / nDivideNumber;
	int nModCount = vecValue.size() % nDivideNumber;

	if (0 < nModCount)
	{
		nSplitCount++;
	}

	// �ε����� �ʰ��ϸ� �� �Ķ����
	if (nSplitCount <= nSplitIndex)
	{
		return NEW_BLOB();
	}

	// ������ �ε����� �������� ������.
	int nCount = nDivideNumber;
	if (nModCount != 0)
	{
		// nDivedeNumber��ŭ �߶󺸳��� ���� ¥���� �ε����� �ִٸ�, ¥������ŭ�� ������.
		if (nSplitIndex == nSplitCount - 1)
		{
			nCount = nModCount;
		}
	}

	return NEW_BLOB(&vecValue[nSplitIndex * nDivideNumber], sizeof(vector<T>::value_type), nCount);
}