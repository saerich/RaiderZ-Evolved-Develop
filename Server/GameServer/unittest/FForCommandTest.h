#pragma once

#include "FBaseFieldInfo.h"
#include "FBaseFieldMgr.h"
#include "FBaseMockLink.h"
#include "FBaseNetClient.h"
#include "FBasePlayer.h"
#include "GUTHelper.h"
#include "GCommandTable.h"
#include "GPlayerObjectManager.h"
#include "MockDBManager.h"
#include "MockField.h"
#include "FBaseAppNetClient.h"

/// Ŀ�ǵ� �׽�Ʈ�� ���� �Ƚ�ó
/// - ���� �ʵ� ���� 2�� : id = 1, 2
/// - ���� �ʵ� ���� 1�� : id = 100

class FForCommandTest : public FBaseMockLink, public FBaseNetClient, public FBasePlayer, public FBaseFieldInfo, public FBaseFieldMgr, public FBaseAppNetClient
{
public:
	FForCommandTest();
	virtual ~FForCommandTest();

	void InitField();
	void InitPlayerAndLink();

	MockField* m_pField;

	int m_nSharedFieldID;
	int m_nSharedFieldID2;
	int m_nDynamicFieldID;
	MUID m_uidDynamicFieldGroup;

	MockLink*		m_pLink;
	GEntityPlayer*	m_pPlayer;
};
