#include "StdAfx.h"
//#include "XDef.h"

#include "TECharacter.h"
#include "RMeshNode.h"
//#include "RBlendAnimationController.h"
#include "RSceneManager.h"
#include "RMeshMgr.h"
#include "CSItemHelper.h"

MImplementRTTI(TECharacter, RActor);

//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
// XCharacter
TECharacter::TECharacter(void)
: m_pSpineController(new XCharacterSpineController)
, m_pQuiverNode(NULL)
, m_pBaseAnimationTC(NULL)
{
	AddController(m_pSpineController);

	m_bAnimationUpdated = true;

	m_pTC = new TETransformControllerManager(this);
}

TECharacter::~TECharacter(void)
{
	Destroy();

	RemoveController(m_pSpineController);
	delete m_pSpineController;

	SAFE_DELETE(m_pTC);
}

void TECharacter::Destroy()
{
	// Quiver �� ������ Quiver ����
	RemoveQuiver();

	//RActor::Destroy();
}

void TECharacter::RemoveQuiver()
{
	if(m_pQuiverNode)
	{
		RMesh* pMesh = m_pQuiverNode->m_pMeshNode->m_pBaseMesh;

		DeleteActorNode(m_pQuiverNode->GetNodeName().c_str());
		REngine::GetSceneManager().ReleaseResource(pMesh);

		m_pQuiverNode = NULL;
	}
}

void TECharacter::AddQuiver()
{
	const char* szQuiverName = "quiver_01.elu";
	const char* szQuiverNodeName = "quiver_01";
	const char* szQuiverParentNodeName = "dummy_quiver";

	RMesh* pMesh = REngine::GetSceneManager().CreateResource< RMesh >( szQuiverName );
	bool bRet = pMesh->BuildAll();

	if( !bRet )
	{
		#ifdef _DEBUG
		mlog("���� : ȭ������ ���� - �׷� �޽� ����\r");
		#endif
		REngine::GetSceneManager().ReleaseResource( pMesh );
		return;
	}

	RMeshNode* pMeshNode = pMesh->GetNode(szQuiverNodeName);
	if(!pMeshNode)
	{
		#ifdef _DEBUG
		mlog("���� : ȭ������ ���� - �׷� �޽ó�� ����\r");
		#endif
		REngine::GetSceneManager().ReleaseResource( pMesh );
		return;
	}

	RActorNode* pParentNode = GetActorNode(szQuiverParentNodeName);
	m_pQuiverNode = AddMeshNode(pMeshNode,pParentNode);
}

bool TECharacter::Create(RMesh* pMesh, bool bBackgroundCreation)
{
	if(RActor::Create(pMesh, bBackgroundCreation))
	{
		return true;
	}
	return false;
}

bool TECharacter::Create( const TCHAR* szMeshName, bool bBackgroundCreation /*= false*/ )
{
	USES_CONVERSION_EX;
	return  RActor::Create(W2A_EX(szMeshName, 0), bBackgroundCreation);
}

bool TECharacter::SetAnimation( tstring& strAniName, bool bForce/*=false*/, bool bSameFrame /*= false*/, bool bTestPlay /*= false*/ )
{
	return m_pTC->SetAnimation(strAniName, bForce, bSameFrame, bTestPlay);
}

void TECharacter::UpdateAnimationTC( float fDela )
{
	m_pTC->Update(fDela);
}

void TECharacter::SetAnimationSpeed( float fSpeed )
{
	m_pTC->SetAnimationSpeed(fSpeed);
}

float TECharacter::GetAnimationSpeed()
{
	return m_pTC->GetAnimationSpeed();
}

void TECharacter::InitCurrentAnimation()
{
	if(m_pBaseAnimationTC == NULL)
		return;

	m_pBaseAnimationTC->InitCurrentAnimation();
}

void TECharacter::PauseAnimation()
{
	if(m_pTC)
		m_pTC->PauseAnimation();
}

void TECharacter::StopAnimation()
{
	if(m_pTC)
		m_pTC->StopAnimation();
}

void TECharacter::PlayAnimation()
{
	if(m_pTC)
		m_pTC->PlayAnimation();
}

void TECharacter::ResetAnimationEventByFrame( int nFrame )
{
	if(m_pBaseAnimationTC)
		m_pBaseAnimationTC->ResetAnimationEventByFrame(nFrame);
}

bool TECharacter::IsAnimationLoop()
{
	return m_pTC->IsAnimationLoop();
}

bool TECharacter::IsPlayDone()
{
	return m_pTC->IsPlayDone();
}

bool TECharacter::IsOncePlayDone()
{
	return m_pTC->IsOncePlayDone();
}

rs3::RPLAYSTATE TECharacter::GetAnimationState()
{
	if(m_pBaseAnimationTC)
		return m_pBaseAnimationTC->GetState();

	return RPS_STOP;
}

unsigned int TECharacter::GetAnimationFrame()
{
	return m_pTC->GetAniFrame();
}

void TECharacter::SetAnimationFrame( unsigned int nFrame )
{
	m_pTC->SetAnimationFrame(nFrame);
}

float TECharacter::GetMaxFrameToSecond()
{
	return m_pTC->GetMaxFrameToSecond();
}	

float TECharacter::GetCurFrameToSecond()
{
	return m_pTC->GetCurFrameToSecond();
}

unsigned int TECharacter::GetAnimationMaxFrame()
{
	if(m_pBaseAnimationTC)
		return m_pBaseAnimationTC->GetAniMaxFrame();

	return 0;
}

void TECharacter::ClearAnimationEvent()
{
	if(m_pBaseAnimationTC)
		m_pBaseAnimationTC->ClearAnimationEvent();
}

void TECharacter::AddAnimationEvent( ANIMATION_EVENT& ani_event )
{
	if(m_pBaseAnimationTC)
		m_pBaseAnimationTC->AddAnimationEvent(ani_event);
}

bool TECharacter::ReSetAnimation()
{
	return m_pTC->ReSetAnimation();
}

bool TECharacter::CheckExistNodeController( string& strNodeName )
{
	if(m_pBaseAnimationTC)
		return m_pBaseAnimationTC->CheckExistNodeController(strNodeName);

	return false;
}

RAnimation* TECharacter::GetCurAnimation()
{
	if(m_pBaseAnimationTC)
		return m_pBaseAnimationTC->GetAnimation();

	return NULL;
}

void TECharacter::GetSamplingActorNodeData( tstring strNodeName, unsigned int nStartFrame, unsigned int nEndFrame, int nSampleInterval, std::vector< RMatrix >& _rOUt )
{
	USES_CONVERSION_EX;
	// ���� ���̸� �ȵ˴ϴ�.
	RActorNode * pActorNode = GetActorNode(W2A_EX(strNodeName.c_str(), 0));
	if(pActorNode == NULL)
		return;

	// ���� ������ ��������
	int nCurrentFrame = GetCurFrame();

	// ���� �������� �� �����Ӻ��� ũ�� ����...
	if(nStartFrame > GetMaxFrame())
		return;

	// ���ø�
	RMatrix matResult;
	for(unsigned int n = nStartFrame; n < nEndFrame; n += nSampleInterval)
	{
		SetAnimationFrame(n);

		m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);

		tstring strCalNodeName = strNodeName;
		if(pActorNode->GetVisibility() == 0.0f)
			strCalNodeName += L"_c";

		m_pBaseAnimationTC->GetAnimatedActorNodeTransform(strCalNodeName, matResult, E_TS_WORLD);
		_rOUt.push_back(matResult);
	}

	// ������ �������� ���ø� ���ϹǷ� ���⼭ ó���Ѵ�.
	SetAnimationFrame(nEndFrame);
	m_pBaseAnimationTC->GetAnimatedActorNodeTransform(strNodeName, matResult, E_TS_WORLD);
	_rOUt.push_back(matResult);

	// �������� �����·� ������.
	SetAnimationFrame(nCurrentFrame);
	m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);
}

RActorNode *  TECharacter::GetAnimatedActorNodeTransform( unsigned int nFrame, tstring& strNodeName, RMatrix& _rOut, E_TRANSFORMSPACE eTransformSpace )
{
	USES_CONVERSION_EX;
	// ���� ���̸� �ȵ˴ϴ�.
	RActorNode * pActorNode = GetActorNode(W2A_EX(strNodeName.c_str(), 0));
	if(pActorNode == NULL)
		return NULL;

	PauseAnimation();

	// ���� ������ ��������
	int nCurrentFrame = GetCurFrame();

	// ���� �������� �� �����Ӻ��� ũ�� Max ����������...
	if(nFrame > GetMaxFrame())
		nFrame = GetMaxFrame();

	// ���ø�
	RMatrix matResult;
	SetAnimationFrame(nFrame);

	m_pBaseAnimationTC->GetAnimatedActorNodeTransform(strNodeName, _rOut, eTransformSpace);

	// �������� �����·� ������.
	SetAnimationFrame(nCurrentFrame);
	m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);

	PlayAnimation();
	return pActorNode;
}

void TECharacter::ForceUpdateBaseAnimationTC()
{
	m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);
}