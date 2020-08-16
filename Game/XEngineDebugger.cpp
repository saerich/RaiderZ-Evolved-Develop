#include "stdafx.h"
#include "XEngineDebugger.h"
#include "XConsoleCmdsModifier.h"

#include "XSystem.h"
#include "XGame.h"
#include "XWorld.h"
#include "XController.h"
#include "XNetwork.h"

#include "MockNetClient.h"
#include "RCameraSceneNode.h"
#include "XModuleTalent.h"

#include "XCameraManager.h"

#include "XModuleEntity.h"

#include "XEngineDebugContextRagDoll.h"
#include "XEngineDebugContextCamera.h"
#include "XEngineDebugContextStun.h"
#include "XEngineDebugContextDynamicActorNode.h"
#include "XEngineDebugContextBuffEffect.h"
#include "XEngineDebugContextSwallowed.h"
#include "XEngineDebugContextHit.h"
#include "XEngineDebugContextTalent.h"

#include "XPost_GM.h"

XEngineDebugger::XEngineDebugger()
{
	m_pConsoleModifier = new XConsoleCmdsModifier;

	//if (global.net)
	{
		// global.net�� ���� ���� ������. m_pGlobalInstance->net�� �������� ���Դϴ�.
		//delete global.net;

		global.net = new MockNetwork();
		global.net->Create();
	}
	m_pNetAgent = new minet::MTestNetAgent(global.net->GetClient());


	//////////////////////////////////////////////////////////////////////////
	//
	// Debug Context �߰�
	//
	//////////////////////////////////////////////////////////////////////////

	AddDebugContext<XEngineDebugContextRagDoll>(L"r", L"RagDoll ����");
	AddDebugContext<XEngineDebugContextCamera>(L"c", L"Camera ��� �����°� �����");
	AddDebugContext<XEngineDebugContextStun>(L"s", L"���Ͻÿ� ����Ʈ ��� �پ��ִ��� �����");
	AddDebugContext<XEngineDebugContextDynamicActorNode>(L"d", L"���̳��� ���ͳ�� �����");
	AddDebugContext<XEngineDebugContextBuffEffect>(L"b", L"���� ����Ʈ �����");
	AddDebugContext<XEngineDebugContextSwallowed>(L"sw", L"�Ա� ��ũ �����");
	AddDebugContext<XEngineDebugContextHit>(L"hit", L"��Ʈ ���� �����");
	AddDebugContext<XEngineDebugContextTalent>(L"talent", L"�ŷ�Ʈ ���� �����");
}

XEngineDebugger::~XEngineDebugger()
{
	// ����� ���ؽ�Ʈ �����
	for (MAP_DEB_CONTEXT::iterator itr = m_mapDebugContexts.begin(); itr != m_mapDebugContexts.end(); ++itr)
	{
		delete itr->second;
	}
	m_mapDebugContexts.clear();

	delete m_pConsoleModifier;
	delete m_pNetAgent;
	SAFE_DELETE(global.net);
}

void XEngineDebugger::OnConsoleCmd( const wchar_t* line, const int argc, wchar_t **const argv )
{
	if (argc < 2)
		return;

	std::wstring strCmd = argv[1];

	// help
	if (argc == 2 && strCmd == L"help")
	{
		for (vector<wstring>::iterator itr = m_vecHelpText.begin(); itr != m_vecHelpText.end(); ++itr)
		{
			PrintToConsole(itr->c_str());
		}
		return;
	}

	// '��ɾ�' '�����ɾ�' ����
	else if (argc >= 3)
	{
		std::vector< std::wstring > vecStrCmd;
		for (int i = 2; i < argc; ++i)
		{
			vecStrCmd.push_back( argv[i] );
		}

		MAP_DEB_CONTEXT::iterator itr = m_mapDebugContexts.find(strCmd);
		if (itr != m_mapDebugContexts.end())
		{
			itr->second->OnCmd(vecStrCmd);
		}
		else
		{
			PrintToConsole(L"�������� �ʴ� ���");
		}
	}

	// ��ɾ�� ���� ��ɾ� ������ ���߾�� �Ѵ�.
	else
	{
		PrintToConsole(L"�߸��� ���");
	}
}

void XEngineDebugger::OnConsoleCmd( const wchar_t* _realspace_debug_str )
{
	REngine::m_strDebugFlags = MLocale::ConvUTF16ToAnsi(_realspace_debug_str);
}

void XEngineDebugger::Update( float fDelta )
{
	bool bNeedUpdateMyWorldCamera = false;

	// debug context update
	for (MAP_DEB_CONTEXT::iterator itr = m_mapDebugContexts.begin(); itr != m_mapDebugContexts.end(); ++itr)
	{
		XEngineDebugContext* pContext = itr->second;
		pContext->Update(fDelta);

		if (pContext->m_bContextUsable && pContext->m_eDebugContextType == XEngineDebugContext::DCT_MY_CHARACTER_WORLD)
			bNeedUpdateMyWorldCamera = true;
	}

	if (bNeedUpdateMyWorldCamera && gg.controller)
		gg.controller->UpdateCameraAfterGameUpdate(fDelta);
}

void XEngineDebugger::UpdateBeforeGame( float fDelta )
{
	//if (REngine::m_strDebugFlags == "a")
	//{
	//	XPostGM_RangeBreakPart(1000.f);
	//	//REngine::m_strDebugFlags = "b";
	//	//REngine::m_strDebugFlags.clear();
	//	//Sleep(1000);
	//}
	//else if (REngine::m_strDebugFlags == "b")
	//{
	//	Sleep(1000);
	//	REngine::m_strDebugFlags.clear();
	//}
}

void XEngineDebugger::PrintToConsole( const wchar_t* pSzText )
{
	m_pConsoleModifier->PrintToConsole(pSzText);
}
