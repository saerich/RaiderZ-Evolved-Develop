#pragma once

#include "CTransData.h"
#include "MMockNetClient.h"
#include "XBaseUnitTestHelper.h"

class TestAniDataSet;
class XNonPlayer;
class XNPCInfo;

/// ���� �׽�Ʈ�� ���� �� �� �ֵ��� �����ִ� ���� Ŭ����
class XUnitTestHelper : public XBaseUnitTestHelper
{
public:
	XUnitTestHelper() : XBaseUnitTestHelper() {}
	~XUnitTestHelper() {}

	XNonPlayer* InNPC(MUID uidNPC, int nNPCID, vec3 vPos, TestAniDataSet* pAniDataSet=NULL, MCapsule* pColCapsule=NULL, const wchar_t* szMeshName=NULL);
	void InNPCs(TD_UPDATE_CACHE_NPC* pNpcNode, int nNPCCount, TestAniDataSet* pAniDataSet=NULL);

	// Talent
	XTalentInfo* CreateMeleeTalentInfo(int nTalentID);
	void CreateMagicAreaTalentInfo( int nTalentID );
	void CreateProjectileMagicTalentInfo( int nTalentID );
	static void CreateGuardTalentInfo(int nTalentID);
	void CreateTalentInfo(int nTalentID, const std::wstring& strInfo);

	// ������
	static void CreateWeaponItemData(int nItemID, WEAPON_TYPE nWeaponType);
	static void CreateItemAndEquip(XMyPlayer* pMyPlayer, int nItemID, SH_ITEM_SLOT nSlot);
	static void CreateItemSwordInfo( int nItemID);
	static void CreateItemBowInfo( int nItemID);
	static void CreateItemAndInven(XMyPlayer* pMyPlayer, int nItemID, SH_ITEM_SLOT nSlot);

	// �ִϸ��̼�
	void SetAniDataSet_NPCSample1(TestAniDataSet* pAniDataSet);
	void SetAniDataSet_Player(TestAniDataSet* pAniDataSet);
	void SetAniDataSet(XObject* pObject, TestAniDataSet* pAniDataSet);
	static void AddNPCMotions();

	// �浹
	static void SetMeshColCapsule(std::wstring strMeshName, MCapsule cap = MCapsule(vec3(0.0f, 0.0f, 50.0f), vec3(0.0f, 0.0f, 150.0f), 50.0f));
	// ����
	void SetMeshHitCapsule(std::wstring strMeshName, MCapsule* pCapsule = NULL);

	void Clear();


	// NPC
	static XNPCInfo* NewNPCInfo(int nNPCID = 1, const wchar_t* szName = L"SampleNPC", int nGrade = 3, const wchar_t* szMeshName = L"hm_adult", const wchar_t* szMeshPath = L"NPC/hm_adult", const wchar_t* szType = L"npc");

	// ��Ʈ��ũ
	
	


};