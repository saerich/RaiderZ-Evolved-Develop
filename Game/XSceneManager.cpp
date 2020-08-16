#include "stdafx.h"
#include "XSceneManager.h"
#include "XDef.h"
#include "XConst.h"
#include "XProfiler.h"
#include "RShaderCompositeManager.h"
#include "RBackgroundWorker.h"

#include "MPhysX.h"
#include "MPhysX4R3/MPhysX4R3.h"
#include "XCharacter.h"

const char *RSID_XCHARACTER	= "XCHARACTER";

MImplementRTTI(XSceneManager, RSceneManager);

XSceneManager::XSceneManager()
{
	m_vecReserveRemoveToDelete.clear();
}

XSceneManager::~XSceneManager()
{

}

bool XSceneManager::Create(RDevice* pDevice)
{
	if(!RSceneManager::Create(pDevice))
		return false;

	//////////////////////////////////////////////////////////////////////////
	// ���̴� �̸� �ε� �׽�Ʈ
	//
	// ������ ��忡�� 864 �� 27.5 �� -_-;;
	//
	// ���� �ִ� ������ �ƴ϶� ����. ���Ŀ� �̸� �������ϴ� ������ ������ �ؾ߰���
	//

	// PreloadActorShaders();
	return true;
}

void XSceneManager::Clear()
{
	// ��׶��� ��Ŀ�� �ִ� ť ����

	// CLEAR_QUEUE_DEBUG
	//REngine::GetBackgroundWorker().ClearQueues();

	// ���Ŵ��� clear
	RSceneManager::Clear();
}

void XSceneManager::Update()
{
	PFI_BLOCK_THISFUNC( 1001 );
	RSceneManager::Update();

	//////////////////////////////////////////////////////////////////////////
	for( size_t i = 0; i < m_vecReserveRemoveToDelete.size(); ++i )
	{
		XCharacter* pDeleteActor = m_vecReserveRemoveToDelete[i];

		SAFE_DELETE(pDeleteActor);
	}

	m_vecReserveRemoveToDelete.clear();
	_ASSERT( m_vecReserveRemoveToDelete.size() == 0 );
}

void XSceneManager::RenderPrimaryCamera()
{
	PFI_BLOCK_THISFUNC( 1002 );
	RSceneManager::RenderPrimaryCamera();
}

//////////////////////////////////////////////////////////////////////////
// �� �Ŵ��� Factory
//
RSceneNode* XSceneManager::CreateSceneNode( const char* pChar )
{
	RSceneNode* pSceneNode = NULL;

	// XCharacter
	if (_stricmp(pChar, RSID_XCHARACTER) == 0)
	{
		return new XCharacter();
	}

	if (NULL == pSceneNode)
	{
		//CPU Ŭ�ν� 
		pSceneNode = RSceneManager::CreateSceneNode(pChar);
	}

	return pSceneNode;
}

RSceneNodeController* XSceneManager::CreateSceneNodeController( const char* _pSzControllerName )
{
	RSceneNodeController* pController = NULL;

	//pController = physx::CreateRPhysXController(_pSzControllerName);

	if (NULL == pController)
	{
		pController = RSceneManager::CreateSceneNodeController(_pSzControllerName);
	}

	return pController;
}

void XSceneManager::DeleteActor(XCharacter* pActor)
{
	ReserveActorToDelete(pActor);
}

void XSceneManager::ReserveActorToDelete(XCharacter* pActor)
{
	m_vecReserveRemoveToDelete.push_back(pActor);
}
