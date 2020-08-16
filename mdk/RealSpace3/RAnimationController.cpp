#include "StdAfx.h"
#include "RAnimationController.h"
#include "RAnimation.h"
#include "RAnimationNode.h"
#include "RAnimationControllerNode.h"
#include "RActor.h"
#include "RActorNodePhysique.h"
#include "RAnimationResource.h"
#include "RMesh.h"
#include "RMeshNode.h"

namespace rs3 {

//////////////////////////////////////////////////////////////////////////
// RActorNodeSelection
RActorNodeSelection::RActorNodeSelection()
{
}

bool RActorNodeSelection::IsExistNode(const char* szNodeName)
{
	RACTORNODESET::iterator itr = m_nodes.find( szNodeName );
	if( itr!= m_nodes.end() )
		return true;

	return false;
}

bool RActorNodeSelection::SelectNode(RActor* pActor, const char* szNodeName)
{
	if( IsExistNode(szNodeName) )
		return false;

	m_nodes.insert( szNodeName );
	return true;
}

bool RActorNodeSelection::SelectNodeAndChildren( RActor* pActor, const char* szNodeName, bool bExceptSelectNode /*= false*/ )
{
	// Root�� �����Ұ��ΰ�?
	if(bExceptSelectNode == false)
	{
		if(!SelectNode(pActor, szNodeName)) 
			return false;
	}

	RActorNode* pActorNode = pActor->GetActorNode(szNodeName);
	for(int i=0;i<pActor->GetActorNodeCount();++i)
	{
		RActorNode* pNode = pActor->GetActorNodes()[i];
		if(pActorNode==pNode->GetParent())
			SelectNodeAndChildren( pActor, pNode->GetNodeName().c_str());
	}
	return true;
}

RAnimationFrameController::RAnimationFrameController( RSNCTRL_TYPE eType /*= RSNCTRL_UPDATE*/ )
: RActorController( eType )
, m_fSpeed(1.f), m_nState(RPS_STOP)
, m_nMaxFrame(0), m_nFrame(0)
, m_bPlayDone(false), m_bOncePlayDone(false), m_bEventActive(true)
{
}

void RAnimationFrameController::Update(RCameraSceneNode* pCamera)
{
	DWORD dwElapsedTime = REngine::GetSceneManager().GetTimer().GetElapsedTime();

	if(m_nMaxFrame)
		UpdateFrame(dwElapsedTime);

	if( m_bPlayDone && m_bLoop)
	{
		m_bPlayDone = false;
		ResetEvent();
	}
}

void RAnimationFrameController::GetNextFrameAndState(DWORD dwElapsedTime, DWORD& _nOutNextFrame, bool& _bPlayDone) const
{
	if(m_nState==RPS_PLAY)
		_nOutNextFrame += DWORD(4.8f*m_fSpeed*dwElapsedTime);	// Max�� ���� 4800 �������� 1��

	if(_nOutNextFrame >= m_nMaxFrame)
	{
		if(m_bLoop && m_nState==RPS_PLAY && m_nMaxFrame > 0 )
			_nOutNextFrame = _nOutNextFrame % m_nMaxFrame;
		else
			_nOutNextFrame = m_nMaxFrame;
		_bPlayDone = true;
	}
}

void RAnimationFrameController::UpdateFrame(DWORD dwElapsedTime)
{
	GetNextFrameAndState(dwElapsedTime, m_nFrame, m_bPlayDone);
	if (m_bPlayDone)
		m_bOncePlayDone = true;

	if( !m_pActor->GetListener() ) return;

	// �ִϸ��̼� �̺�Ʈ �߻�
	if (m_bEventActive)
	{
		for(size_t i=0; i<m_AniamtionEvents.size(); ++i)
		{
			ANIMATION_EVENT &aniEvent = m_AniamtionEvents[i];
			if(!aniEvent.bEventDone && aniEvent.pAnimationEvent->m_nFrame <= m_nFrame)
			{
				aniEvent.bEventDone = true;
				m_pActor->GetListener()->OnAnimationEvent(m_pActor, aniEvent.pAnimationEvent);
			}
		}
	}
}

void RAnimationFrameController::ResetEvent()
{
	for(vector<ANIMATION_EVENT>::iterator itEvent = m_AniamtionEvents.begin(); itEvent != m_AniamtionEvents.end(); ++itEvent)
	{
		(*itEvent).bEventDone = false;
	}
}

void RAnimationFrameController::CloseEvent()
{
	for(vector<ANIMATION_EVENT>::iterator itEvent = m_AniamtionEvents.begin(); itEvent != m_AniamtionEvents.end(); ++itEvent)
	{
		(*itEvent).bEventDone = true;
	}
}

void RAnimationFrameController::SetAnimation( RAnimation *pAnimation )
{
	// ���� ������ ó���ؾ� �� �̺�Ʈ
	CheckAnimationEventSend();

	RAnimationResource* pResource = pAnimation->GetResource();
	m_nMaxFrame = pResource->GetMaxFrame();
	m_bLoop = (pAnimation->GetAnimationLoopType()==RAniLoopType_Loop);
	m_fSpeed = pAnimation->GetSpeed();


	// �ִϸ��̼� �̺�Ʈ ����
	m_AniamtionEvents.clear();
	for(vector<RAnimationEvent*>::iterator itAniEvent = pAnimation->m_animationEvents.begin(); itAniEvent != pAnimation->m_animationEvents.end(); ++itAniEvent)
	{
		ANIMATION_EVENT aniEvent;
		aniEvent.pAnimationEvent = *itAniEvent;
		m_AniamtionEvents.push_back(aniEvent);
	}
}

void RAnimationFrameController::Reset()
{
	m_nFrame = 0;
	m_nMaxFrame = 0;
	m_bPlayDone = false;
	m_bOncePlayDone = false;
	m_nState = RPS_STOP;
	m_bLoop = false;

	m_AniamtionEvents.clear();
}

void RAnimationFrameController::Play()
{
	if( m_nMaxFrame <= 0 ) return;

	m_nState = RPS_PLAY;
	m_bPlayDone	= false;
	m_bOncePlayDone = false;
}

void RAnimationFrameController::Stop() 
{
	Reset();
}

void RAnimationFrameController::Pause() 
{
	m_nState = RPS_PAUSE;
}

void RAnimationFrameController::SetAnimationEvent(vector<ANIMATION_EVENT>& aniamtionEvents)
{
	m_AniamtionEvents.clear();
	m_AniamtionEvents = aniamtionEvents;
}

void RAnimationFrameController::SetForceAnimationControlPlayDoneForTest( bool bPlayDone )
{
	m_bPlayDone = bPlayDone;
	m_bOncePlayDone = bPlayDone;
}

void RAnimationFrameController::CheckAnimationEventSend()
{
	if( !m_pActor->GetListener() ) return;

	for(int i=0;i<(int)m_AniamtionEvents.size();++i)
	{
		ANIMATION_EVENT &aniEvent = m_AniamtionEvents[i];
		if(aniEvent.bEventDone == false && aniEvent.pAnimationEvent->m_bUnconditional)
		{
			aniEvent.bEventDone = true;
			m_pActor->GetListener()->OnAnimationEvent(m_pActor, aniEvent.pAnimationEvent);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// RAnimationController_New
RAnimationController::RAnimationController( RSNCTRL_TYPE eType /*= RSNCTRL_POSTANIMATION*/, RSNCTRL_TYPE eFrameControllerType /*= RSNCTRL_UPDATE*/ )
: RActorController(eType)
, m_frameController(eFrameControllerType)
, m_pAnimation(NULL)
{
	// ���������ʴ� ��Ʈ�ѷ��� ����
	m_frameController.AddAttribute( RCA_STICKY );
}

RAnimationController::~RAnimationController(void)
{
	Destroy();
}

void RAnimationController::OnAdd()
{
	RActorController::OnAdd();
	m_pActor->AddController( &m_frameController );
	m_frameController.SetAutoUpdate(true);
}

void RAnimationController::OnRemove()
{
	m_frameController.SetAutoUpdate(false);
	m_pActor->RemoveController( &m_frameController );
	RActorController::OnRemove();
}

void RAnimationController::ReNew()
{
	RAnimation* pAnimation = GetAnimation();
	DWORD dwFrame = GetFrame();
	RPLAYSTATE ePlayState = GetState();

	// �̺�Ʈ ����
	vector<ANIMATION_EVENT> aniamtionEvents = m_frameController.GetAnimationEvent();

	// once play done ����
	bool bOncePlayDone = m_frameController.IsOncePlayDone();

	/*RPLAYINFO playInfo;
	playInfo.fPlaySpeed = GetSpeed();
	playInfo.fWeight = GetWeight();
	playInfo.fBlendIn = m_fBlendIn;
	playInfo.fBlendOut = m_fBlendOut;
	playInfo.blendMode =  GetBlendMode();
	playInfo.pNodeSet = m_pNodeSelection;*/
	// ����
	//SetAnimation(pAnimation, NULL);
	SetAnimation(pAnimation, &m_playInfo);
	SetFrame(dwFrame);

	// �̺�Ʈ �缳��
	m_frameController.SetAnimationEvent( aniamtionEvents );

	// ���� �������� �ִϸ��̼��� ���� ��Ű�Ƿ� Play�� ��ŵ�ϴ�.
	if (RPS_PLAY == ePlayState)
	{
		Play();
		Update(NULL);
	}

	// once play done �缳��
	m_frameController.SetOncePlayDone( bOncePlayDone );
}

void RAnimationController::Destroy()
{
	Reset();
}

void RAnimationController::Reset()
{
	Disconnect();

	m_frameController.Reset();

	m_nodeControllers.erase(m_nodeControllers.begin(),m_nodeControllers.end());
	m_activeControllers.erase(m_activeControllers.begin(),m_activeControllers.end());
}

void RAnimationController::ActivateNodes( RActorNodeSelection* pSelection )
{
	vector<RActorNode*>& lodBoneSet = m_pActor->GetAnimationReferrencedNodes();

	// 0 �� ������ �׽�Ʈ��
	m_activeControllers.erase(m_activeControllers.begin(),m_activeControllers.end());
	for(size_t i=0;i<lodBoneSet.size();++i)
	{
		RActorNode* pNode = lodBoneSet[i];
		RAnimationControllerNode* pControllerNode = GetNodeController( pNode->GetNodeName() );
		if(!pControllerNode)
			continue;

		// Ư�� ��� ���� ���������� ������
		if( pSelection && !pSelection->IsExistNode( pNode->GetNodeName().c_str() ) )
			continue;

		if(pControllerNode->GetAnimationNode()==NULL)
			continue;

		// ������ �ʰ�, ���� �ִϸ��̼ǵ� ������ ������� �ʴ´�
		if( (pNode->m_pMeshNode->m_fBaseVisibility==0 || pNode->m_pMeshNode->m_fBaseNoiseRef==0) && pControllerNode->GetAnimationNode()->m_visKeyTrack.empty()) 
			continue;

		m_activeControllers.push_back(pControllerNode);
	}
}

void RAnimationController::ConnectAnimation(RAnimation *pAnimation, RActorNodeSelection* pSelection )
{
	Disconnect();

	RAnimationResource* pResource = pAnimation->GetResource();
	m_pAnimation = pAnimation;
	m_nodeControllers.reserve( pResource->GetAniNodeCount() );

	//mlog("ConnectAnimation \n");
	//mlog("%s , speed : %f \n", pAnimation->GetName(), pAnimation->GetSpeed());

	// actor ���� �ʱ�ȭ
	// �ϸ鼭.. ���� ��尡 �ִ����� üũ����
	bool bUsePhysique = false;
	for(int i=0;i<m_pActor->GetActorNodeCount();++i) {
		RActorNode* pNode = m_pActor->GetActorNodes()[i];

		_ASSERT(pNode);

		// ������ ���¸� ������ �ִ���..
		if( dynamic_cast<RActorNodePhysique*>(pNode))
			bUsePhysique = true;

		// Ư�� ��� ���� ���������� ������
		if( pSelection && !pSelection->IsExistNode( pNode->GetNodeName().c_str() ) ) continue;

		// ������ ��Ÿ�� ���� �ʱ�ȭ ������ �ʴ´�.
		if (pNode->QueryAttribute(RSNA_NO_HIERARCHY_UPDATE) == false)
		{
			pNode->SetVisibility( pNode->m_pMeshNode->m_fBaseVisibility );
			pNode->SetNoiseRef( pNode->m_pMeshNode->m_fBaseNoiseRef);
		}
	}

	m_nodeControllers.resize( pResource->GetAniNodeCount() );

	for(int i=0;i<pResource->GetAniNodeCount();++i)
	{
		RAnimationNode* pAnimationNode = pResource->GetAniNode(i);
		RAnimationControllerNode* pControllerNode = &m_nodeControllers[i];

		const RMatrix &matAnimationLocal = pAnimationNode->m_matLocal;

		RActorNode* pActorNode = m_pActor->GetActorNode(pAnimationNode->GetName());

		pControllerNode->SetAnimationNode(pAnimationNode);
		pControllerNode->m_matBase = matAnimationLocal;
		pControllerNode->m_matCurrent = matAnimationLocal;

		// �⺻����� scale, rotation, translation ���� �����ؼ� �����ص�
		pControllerNode->m_matBase.Decompose(
			pControllerNode->m_currentScale,pControllerNode->m_currentTranslation,pControllerNode->m_currentRotation);

		pControllerNode->m_pActorNode = pActorNode;
		// ������ ���¸� ������ ������ ��Ʈ�ѷ��� ��� �Ǵ� ���� ���� ���� ���ϸ��̼ǿ� ���̴� ���� ���·� ����.
		// do u understand?? ������? ���߿� ���� �о ���ذ� ����.. ��..
		// ������ ���� ��� ������ ��Ʈ������ ���� �ʰ� ���ʹϾ�� Ʈ�������̼Ǹ�..
#ifdef USING_RSC_PHYSIQUE_QT
		if( bUsePhysique && pControllerNode->m_pActorNode)
			pControllerNode->m_pActorNode->SetUseForPhysiqueBone( true);
#endif
	}

	m_frameController.SetAnimation( pAnimation );
}

void RAnimationController::Disconnect()
{
	if(!m_pAnimation)
		return ;

	m_pAnimation = NULL;
	m_nodeControllers.clear();

	m_frameController.Reset();
}

void RAnimationController::SetActorUseVertexAnimation( RAnimation* pAnimation )
{
	m_pActor->SetUseVertexAnimation( false );
	if(	pAnimation->GetResource()->GetAnimationType() == RAniType_Vertex )
		m_pActor->SetUseVertexAnimation( true );
}

bool RAnimationController::SetAnimation( const char* szAnimation, RPLAYINFO* pPlayInfo )
{
	if( !m_pActor->GetUsable() )
	{
		_ASSERT( FALSE );	// �ε��� ?
		return false;
	}

	if(!m_pActor->m_pMesh)
		return false;

	RAnimation* pAniSet = m_pActor->m_pMesh->GetAnimationMgr()->GetAnimation( szAnimation );
	if(!pAniSet) return false;

	return SetAnimation( pAniSet, pPlayInfo );
}

bool RAnimationController::SetAnimation(RAnimation* pAnimation, RPLAYINFO* pPlayInfo )
{
	Reset();

	if(!pAnimation)
		return false;

	SetActorUseVertexAnimation( pAnimation );

	// reset play information
	if (pPlayInfo)
	{
		// ���ο� play info �̸� ���� �������ش�.
		if (pPlayInfo != &m_playInfo)
		{
			m_playInfo = *pPlayInfo;
		}

		// ���ο� node set �̸� ���� �����Ѵ�.
		if (m_playInfo.pNodeSet && m_playInfo.pNodeSet != &m_nodeSelection)
		{
			m_nodeSelection = *m_playInfo.pNodeSet;
			m_playInfo.pNodeSet = &m_nodeSelection;
		}
	}
	else
	{
		//m_playInfo.fPlaySpeed = GetSpeed();
		//m_playInfo.fWeight = GetWeight();
		//m_playInfo.blendMode = GetBlendMode();

		m_nodeSelection.clear();
		m_playInfo.pNodeSet = NULL;
		m_playInfo.fBlendIn = 0;
		m_playInfo.fBlendOut = 0;
	}

	ConnectAnimation(pAnimation, m_playInfo.pNodeSet );
	ActivateNodes(m_playInfo.pNodeSet);

	//SetSpeed( m_playInfo.fPlaySpeed );
	//SetWeight( m_playInfo.fWeight );
	//SetBlendMode( m_playInfo.blendMode );
	//m_fBlendIn = m_playInfo.fBlendIn;
	//m_fBlendOut = m_playInfo.fBlendOut;

	return true;
}

RAnimationControllerNode* RAnimationController::GetNodeController(int i)
{
	return &m_nodeControllers[i];
}

RAnimationControllerNode* RAnimationController::GetNodeController(const string& strName)
{
	if( m_pAnimation==NULL ) return NULL;
	RAnimationResource* pResource = m_pAnimation->GetResource();
	int nIndex = pResource->GetNodeIndex(strName);
	if(nIndex<0 || nIndex>=(int)m_nodeControllers.size()) return NULL;

	return &m_nodeControllers[nIndex];
}

// ������ ���������� �ִϸ��̼� �����͸� �����Ѵ�
void RAnimationController::UpdateAnimation()
{
	RPFC_THISFUNC;
	for(size_t i=0;i<GetActiveNodeCount();++i)
	{
		RAnimationControllerNode* pControllerNode = m_activeControllers[i];
		pControllerNode->Update( m_frameController.GetFrame());
	}
}

void RAnimationController::Update(RCameraSceneNode* pCamera)
{
	RPFC_THISFUNC;
	UpdateAnimation();

	for(unsigned int i=0;i<m_activeControllers.size();++i)	// TODO: lod set ���� ����
	{
		RAnimationControllerNode* pController = m_activeControllers[i];
		if(!pController)
			continue;

		RActorNode* pActorNode = m_pActor->GetActorNode(pController->GetNodeName());

		if (!pActorNode)
			continue;

		// �θ� �ִ�, ������ Ÿ�� �ʴ� ���ͳ��� ������Ʈ ���� �ʴ´�.
		if (pActorNode->QueryAttribute(RSNA_NO_HIERARCHY_UPDATE) && pActorNode->GetParent() != NULL)
			continue;

		RQuaternion targetRotation		= pController->GetCurrentRotation();
		RVector		targetTranslation	= pController->GetCurrentTranslation();
		RVector		targetScale			= pController->GetCurrentScale();
		float		targetVisibility	= pController->GetVisibility();

		// overwrite ����̸� ����������
		if(m_playInfo.blendMode==RBLENDMODE_OVERWRITE)
		{
			pActorNode->SetRotation( targetRotation );
			pActorNode->SetPosition( targetTranslation );
			pActorNode->SetScale( targetScale );
			pActorNode->SetVisibility( targetVisibility );

		}else if ( m_playInfo.blendMode==RBLENDMODE_BLEND )
		// �����ؾ��Ѵ�
		{
			RQuaternion curRotation = pActorNode->GetRotation();	/// TODO: �ӵ��� ���� �ɸ��� �۾�, ����ȭ ����
			RVector	curTranslation = pActorNode->GetPosition();
			RVector	curScale = pActorNode->GetScale();
			float	curVisibility = pActorNode->GetVisibility();

/* // ĳ���� �⺻ ����� ���� ����
			RQuaternion curRotation;
			RVector	curTranslation;
			RVector	curScale;
			pActorNode->m_pMeshNode->m_matLocal.Decompose(curScale,curTranslation,curRotation);
*/

			float fCurrentWeight = m_playInfo.fWeight;

			if( m_playInfo.fBlendIn>0 )
			{
				float fBlendInWeight = (float(m_frameController.GetFrame())/4800.f / m_playInfo.fBlendIn);
				fCurrentWeight = min( fCurrentWeight, fBlendInWeight );
			}

			if( m_playInfo.fBlendOut>0 )
			{
				float fBlendOutWeight = (float(m_frameController.GetMaxFrame() - m_frameController.GetFrame())/4800.f / m_playInfo.fBlendOut);
				fCurrentWeight = min( fCurrentWeight, fBlendOutWeight );
			}

// 			if( fCurrentWeight> 0 )
			{
				RQuaternion blendedRotation;
				blendedRotation.Slerp(curRotation,targetRotation,fCurrentWeight);

				RVector blendedPosition = fCurrentWeight* targetTranslation + (1.f-fCurrentWeight)*curTranslation;

				RVector blendedScale = fCurrentWeight* targetScale + (1.f-fCurrentWeight)*curScale;
				
				float blendedVisibility = fCurrentWeight* targetVisibility + (1.f-fCurrentWeight)*curVisibility;

				pActorNode->SetRotation(blendedRotation);
				pActorNode->SetPosition(blendedPosition);
				pActorNode->SetScale( blendedScale );
				pActorNode->SetVisibility( blendedVisibility );
			}

		}else
		{
			_ASSERT(FALSE);
		}
	}
}

}