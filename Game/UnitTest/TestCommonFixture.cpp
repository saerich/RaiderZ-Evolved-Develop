#include "stdafx.h"
#include "TestCommonFixture.h"
#include "TestGame.h"
#include "XUnitTestUtil.h"
#include "XMotion.h"
#include "XMotionMgr.h"

SimpleTestFixture::SimpleTestFixture()
{
	info.motion->Init();

	m_pGame = new TestGame();

	m_Helper.SetAniDataSet_NPCSample1(&m_NPCAniDataSet);
	m_Helper.SetAniDataSet_Player(&m_PCAniDataSet);

	// npc ���� �߰�
	XUnitTestHelper::NewNPCInfo(1, L"sample");

	// �ŷ�Ʈ ���� �߰�
	int ID = XUnitTestUtil::NewID();
	m_Helper.CreateMeleeTalentInfo(ID);
}

SimpleTestFixture::~SimpleTestFixture()
{
	SAFE_DELETE(m_pGame);

	gvar.MyInfo.Talent.Init();
}