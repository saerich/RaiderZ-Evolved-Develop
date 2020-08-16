#pragma once

#include "XEngineDebugContext.h"

class XEngineDebugContextCamera : public XEngineDebugContext
{
public:
	XEngineDebugContextCamera()
		: XEngineDebugContext(XEngineDebugContext::DCT_MY_CHARACTER_WORLD)
	{
		m_vStartPosOgre.Set(5000,5000,0);
		m_vStartDirOgre.Set(1,1,0);
		m_vStartDirOgre.Normalize();

		m_vStartPosMy = m_vStartPosOgre + (300.f * m_vStartDirOgre);
		m_vStartDirMy = -m_vStartDirOgre;

		m_nTalentID = 211000115;
		m_uidEffect.SetZero();
	}

	virtual ~XEngineDebugContextCamera(){}

	virtual void OnUpdate(float fDelta)
	{
	}

	virtual void OnCmd(const std::vector< std::wstring >& _refConsoleCmd)
	{
		_ASSERT( _refConsoleCmd.empty() == false );

		if (_refConsoleCmd[0] == L"init")
		{
			InitCameraDebuggingWorld();
		}
		else
		{
			if (false == m_bContextUsable)
			{
				m_pEngineDebugger->PrintToConsole(L"ī�޶� �׽�Ʈ �Ҽ� �ִ� ȯ�� �ƴ� - 'ed c init' ���");
				return;
			}

			//////////////////////////////////////////////////////////////////////////
			// ���� Ŀ�ǵ�
			//
			if (_refConsoleCmd[0] == L"a")
			{
				minet::MTestNetAgent* pNetAgent = m_pEngineDebugger->GetNetAgent();
				XActor* pOgre = gg.omgr->FindActor(m_OgreID);
				pNetAgent->OnRecv( MC_ACTION_USE_TALENT, 4,
					NEW_USHORT( m_OgreUIID ),
					NEW_INT( m_nTalentID ),
					NEW_SVEC(pOgre->GetDirection() ),
					NEW_VEC(pOgre->GetPosition() )
					);
				mlog("�׼�!");
				return;
			}
			/*else if (_refConsoleCmd[0] == "scale" && _refConsoleCmd.size() == 2)
			{
				float fScale = atof(_refConsoleCmd[1].c_str());
				global.emgr->SetScaleXEffect(m_uidEffect, false, fScale);
			}*/
		}
	}

	void InitCameraDebuggingWorld()
	{
		// uid ����
		m_OgreID = global.system->GenerateLocalUID();
		m_MyID = global.system->GenerateLocalUID();

		m_OgreUIID = XUnitTestUtil::NewUIID();

		// ���ӻ���(���� + ĳ����)
		gvar.World.SetFieldID(9001);

		TD_MYINFO MyInfo;
		wcsncpy_s(MyInfo.szName, L"pok", _TRUNCATE);
		wcsncpy_s(MyInfo.szSurname, L"test_pok", _TRUNCATE);

		MyInfo.nSex	= SEX_FEMALE;
		MyInfo.nLevel	= 34;
		MyInfo.nMoney	= 999;
		MyInfo.nMaxHP	= 70;
		MyInfo.nMaxEN	= 90;
		MyInfo.nMaxSTA = 140;
		MyInfo.nXP		= 11053;
		MyInfo.nSTR	= 3;
		MyInfo.nDEX	= 1;
		MyInfo.nINT	= 1;
		MyInfo.nCHA	= 3;
		MyInfo.nCON	= 7;

		MyInfo.nCurHP	= 70;
		MyInfo.nCurEN	= 90;
		MyInfo.nCurSTA	= 140;

		MyInfo.fExpPercent	= 70;
		MyInfo.nRemainTP	= 99;

		vector<TD_ITEM> vecTDInventory;
		vector<TD_ITEM> vecTDEquipment;
		vector<int> vecTalentID;	
		vector<TD_PLAYERQUEST> vecTDPlayerQuest;
		TD_PALETTE tdPalette;
		vector<TD_FACTION> vecTDFaction;

		gvar.MyInfo.SetFrom_TD_MYINFO(MyInfo, vecTDInventory, vecTDEquipment, vecTalentID, vecTDPlayerQuest, tdPalette, vecTDFaction, vector<int>());
		gvar.MyInfo.MyUID = m_MyID;

		gg.game->Create();

		// NPC
		TD_UPDATE_CACHE_NPC td_update_cache_npc;
		td_update_cache_npc.uid = m_OgreID;
		td_update_cache_npc.nUIID = m_OgreUIID;
		td_update_cache_npc.nNPCID = 110001;
		td_update_cache_npc.vPos = m_vStartPosOgre;
		td_update_cache_npc.svDir = m_vStartDirOgre;

		gg.currentgamestate->InNPCs(&td_update_cache_npc, 1);

		// �� ĳ���� �̵�
		minet::MTestNetAgent* pNetAgent = m_pEngineDebugger->GetNetAgent();
		pNetAgent->OnRecv( MC_ENTITY_WARP, 3, NEW_UID(XGetMyUID()), NEW_VEC(m_vStartPosMy), NEW_VEC(m_vStartDirMy));

		// ī�޶� ����
		global.camera->SetPrimaryCamera(CAMERA_MAIN_BACKVIEW);
		XCameraBackViewController* pCameraController = global.camera->GetCameraController<XCameraBackViewController>();
		if (pCameraController)
			pCameraController->Init(m_MyID);

		// ����� ���ؽ�Ʈ Ȱ��ȭ
		m_bContextUsable = true;

		// ��Ŀ���� �ֿܼ� �״�� �����ϱ�
		global.ui->ToggleConsole();
		global.ui->ToggleConsole();

		// ��ƼŬ ������ ���� �׽�Ʈ
		//XObject* pMyPlayer = gg.omgr->FindActor(XGetMyUID());
		//RActorNode* pActorNode = pMyPlayer->GetActor()->GetActorNode("Bip01 HeadNub");
		////m_uidEffect = global.emgr->AddXEffect("ef_holy_cast_s2", pMyPlayer->GetActor(), pActorNode, mat);
		//m_uidEffect = global.emgr->AddXEffect("ef_soul", pMyPlayer->GetActor(), pActorNode);
		//global.emgr->SetScaleXEffect(m_uidEffect, false, 1.0f);
	}

	MUID m_OgreID;
	MUID m_MyID;
	MUID m_uidEffect;

	UIID m_OgreUIID;

	int m_nTalentID;

	RVector m_vStartPosOgre;
	RVector m_vStartDirOgre;

	RVector m_vStartPosMy;
	RVector m_vStartDirMy;
};
