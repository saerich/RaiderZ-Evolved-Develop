#include "stdafx.h"
#include "RMeshMgr.h"
#include "RActor.h"
#include "RScreenEffectManager.h"
#include "RCameraSceneNode.h"
#include "RDeviceD3D.h"
#include "MFileSystem.h"
#include "RAnimationResource.h"

namespace rs3 {

RScreenEffectActor::RScreenEffectActor() : REffectActor(RRP_NORMAL)
{
	m_bUsingActorPassRenderer = false;
	m_bUsingEnvironmentTiling = false;
	m_bUseShade = false;

	SetAutoUpdate(false);
	m_AutoUpdateContext.m_bMustNotAutoUpdate = true;
	m_bCullingNode = false;

	SetVisible(false);
}

////////////////////////////////////////////////////////////////////////////////////
// RScreenEffectUnit ///////////////////////////////////////////////////////////////
RScreenEffectUnit::RScreenEffectUnit() : m_bVisible( false )
{
	m_pMesh					= NULL;
	m_pAnimation			= NULL;
}

RScreenEffectUnit::~RScreenEffectUnit()
{

}

void RScreenEffectUnit::Create(RMesh *pMesh, 
								RAnimation* pAnimation, 
								vec3 offset)
{
	m_pMesh					= pMesh;
	m_Offset				= offset;
	m_pAnimation			= pAnimation;
	m_bVisible				= false;

	// �Ʒ��� �Լ����� ���ξ����忡�� �Ҹ��ٰ� �����մϴ�.

	// by pok. ���Ŵ��� ������
	//m_RActor.SetManager(RGetSceneManager());

	m_RActor.Create(pMesh);
//	m_RActor.SetAnimation("play");
	m_RActor.SetAnimation(m_pAnimation);
	m_RActor.SetListener(this);
	m_RActor.SetPosition(m_Offset);
	m_RActor.SetUsingPassRenderer(false);
}

void RScreenEffectUnit::Destroy()
{
	m_pMesh					= NULL;
	m_pAnimation			= NULL;

	m_RActor.Destroy();
}

bool RScreenEffectUnit::Render()
{
	return true;
}

void RScreenEffectUnit::Update(DWORD dwElapsed)
{
	//m_RActor.Update();
	//m_RActor.Traverse(&RSceneNode::UPDATOR);
	m_RActor.Traverse(&m_ActorUpdator);
	m_RActor.Update();
}

bool RScreenEffectUnit::IsAnimationEnd()
{
	if(m_RActor.IsPlayDone())
		return true;
	return false;
}

void RScreenEffectUnit::SetAnimation(char* pOrder)
{
//	m_RActor.GetAnimationFrameInfo().m_nFrame = 0;
	m_RActor.SetAnimation(pOrder);
}

void RScreenEffectUnit::SetAnimation(RAnimation* pAnimation)
{
	m_RActor.SetAnimation(pAnimation);
}

void RScreenEffectUnit::SetPosition(vec3 vPos)
{
	m_Offset = vPos;
	m_RActor.SetPosition(m_Offset);
}

void RScreenEffectUnit::SetAnimation(char * pOrder, AnimationLoopType nAniLoop)
{
	m_pMesh->GetAnimationMgr()->GetAnimation(pOrder)->SetAnimationLoopType(nAniLoop);
}

void RScreenEffectUnit::AnimationStop()
{
	m_RActor.Pause();
}

void RScreenEffectUnit::SetbVisible( bool bVisible )
{
	m_RActor.SetVisible(bVisible);
	m_bVisible = bVisible;
}

void RScreenEffectUnit::SetVisibility( float f )
{
	m_RActor.SetVisibility(f);
}

void RScreenEffectUnit::SetLastFrame()
{
	DWORD dwMaxFrame = m_RActor.GetAnimationController().GetMaxFrame();
	m_RActor.GetAnimationController().SetFrame(dwMaxFrame);
	m_RActor.Pause();
}
///////////////////////////////////////////////////////////////////////////////////////
// RScreenEffect ///////////////////////////////////////////////////////////////
RScreenEffect::RScreenEffect(RSceneManager* pSceneManager) :
	m_pSceneManager(pSceneManager),
	m_vPosEffect(0, 0, 0),
	m_ScreenEffectLocation(LOCATION_LEFT),
	m_ScreenEffectExist(EXIST_ONE),
	m_ScreenEffectChar(CHARACTER_NORMAL),
	m_ScreenEffectLoop(RAniLoopType_HoldLastFrame),
	m_ScreenEffectAniEndAtt(ANIMATION_END_DEL),
	m_bAnimationEnd(false)
{
}

RScreenEffect::~RScreenEffect()
{
}

void RScreenEffect::Create(string strScreenEffectName, 
							string strBaseFileName, 
							RSCREENEFFECT_EXIST_ATT existAtt,
							RSCREENEFFECT_CHARACTER_ATT charAtt,
							AnimationLoopType aniLoop)
{
	// �Ӽ� �Է�
	m_strScreenEffectName		= strScreenEffectName;
	m_strBaseFileName			= strBaseFileName;
	m_ScreenEffectExist			= existAtt;
	m_ScreenEffectChar			= charAtt;
	m_ScreenEffectLoop			= aniLoop;
}

void RScreenEffect::Destroy()
{

}

bool RScreenEffect::Render( RCameraSceneNode* pCurrenCamera)
{
	return true;
}

bool RScreenEffect::Update(DWORD dwElapsed)
{
	return true;
}

void RScreenEffect::SetString(string strContents)
{

}

void RScreenEffect::SetPosition(vec3 vPosEffect, RSCREENEFFECT_LOCATION_TYPE scrBase)
{

}

void RScreenEffect::ChangeAnimation(string strAnimation, RSCREENEFFECT_ANIMATION_END_ATT aniDel)
{

}

void RScreenEffect::SetDraw(bool bVisible)
{

}

// by pok, �̰��� ���� �����忡���� �Ҹ��ٰ� �����մϴ�.
bool RScreenEffect::SetScreenEffectUnit(RScreenEffectUnit* pUnit, string strFileName, vec3 vPosEffect)
{
	if(pUnit == NULL)
		return false;

	// �޽� �ε�
	RMesh* pMesh = m_pSceneManager->CreateResource< RMesh >( strFileName.c_str() );
	bool bRet = pMesh->BuildAll();
	if( !bRet )
	{
		_ASSERT( !"�޽ð� �ε��� �����߽��ϴ�." );
		return bRet;
	}

	// ���� �޽��� ������ �ִٸ�... //by pok. �� ���ذ� �Ȱ��ϴ�.
	if(pUnit->GetMesh() != pMesh)
	{
		pUnit->Destroy();
	}

	// �޽� �ִϸ��̼� ����
	RAnimation* pAnimation = pMesh->GetAnimationMgr()->GetAnimation("play");
	if(pAnimation == NULL)
	{
		// �ִϸ��̼� XML �ε�
		string strAniXMLFileName = strFileName + ".animation.xml";
		if(pMesh->GetAnimationMgr()->AddXmlDesc((char*)strAniXMLFileName.c_str()))
		{
			// XML �ε� ����
		}
		// �Ϲ� �ִϸ��̼� �ε�
		else
		{
			string strAniFileName = strFileName + ".ani";
			MFile mzf;

			// �ӽ� �������� Ȯ��. ���߿� �ؿ� Add���� ó���ؾ� �� ����
			if(!mzf.Open((char*)strAniFileName.c_str())) 
			{
				#ifdef _DEBUG
				mlog("%s file not found!! \n", strAniFileName.c_str() );
				#endif
			}
			else
			{
				pAnimation = pMesh->GetAnimationMgr()->Add("play", (char*)strAniFileName.c_str(), false, NULL );
				pAnimation->GetResource()->BuildAll();
				pAnimation->SetAnimationLoopType( m_ScreenEffectLoop );
			}
		}
	}
	else
	{
		if(pUnit->GetActor())
		{
			pUnit->GetActor()->GetAnimationController().SetFrame( 0 );
			pUnit->GetActor()->GetAnimationController().Play();
		}
	}

	if(pUnit->GetMesh() == NULL)
		pUnit->Create(pMesh, pAnimation, vPosEffect);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
//RSingleScreenEffect///////////////////////////////////////////////////////////////
RSingleScreenEffect::RSingleScreenEffect(RSceneManager* pSceneManager) : RScreenEffect(pSceneManager), m_pUnit( NULL )
{

}

RSingleScreenEffect::~RSingleScreenEffect()
{

}

void RSingleScreenEffect::Create(string strScreenEffectName, 
									string strBaseFileName, 
									RSCREENEFFECT_EXIST_ATT existAtt,
									RSCREENEFFECT_CHARACTER_ATT charAtt,
									AnimationLoopType aniLoop)
{
	RScreenEffect::Create(strScreenEffectName,
							strBaseFileName,
							existAtt,
							charAtt,
							aniLoop);

	m_pUnit = new RScreenEffectUnit;

	SetScreenEffect();
}

void RSingleScreenEffect::Destroy()
{
	m_pUnit->Destroy();
	SAFE_DELETE(m_pUnit);
}

bool RSingleScreenEffect::Render( RCameraSceneNode* pCurrentCamera)
{
	if(m_pUnit->GetbVisible() &&
		m_pUnit->GetActor())
	{
		m_pUnit->GetActor()->Render( pCurrentCamera, -1, RENDERING_INFERRED);	/// ��� ������ ����̵� ����� ���� �׸��� ����� �ϰ����� �³�?
	}

	return true;
}

bool RSingleScreenEffect::Update(DWORD dwElapsedTime)
{
	if(m_pUnit->GetbVisible())
	{
		m_pUnit->Update(dwElapsedTime);

		if(	m_pUnit->IsAnimationEnd() )
		{
			if( m_ScreenEffectAniEndAtt == ANIMATION_END_DEL)
			{
				return false;
			}
			else if( m_ScreenEffectAniEndAtt == ANIMATION_END_HIDE )
			{
				m_pUnit->SetbVisible(false);
				return true;
			}
		}
	}

	return true;
}

void RSingleScreenEffect::SetPosition(vec3 vPosEffect, RSCREENEFFECT_LOCATION_TYPE scrBase)
{
	m_vPosEffect = vPosEffect;
	m_ScreenEffectLocation = scrBase;

	m_pUnit->SetPosition(m_vPosEffect);
}

void RSingleScreenEffect::ChangeAnimation(string strAnimation, RSCREENEFFECT_ANIMATION_END_ATT aniDel)
{
	m_ScreenEffectAniEndAtt = aniDel;

	// �������� ���� �ʴ´ٸ�...
	if(m_pUnit->GetbVisible() == false)
	{
	}

	m_pUnit->SetAnimation((char*)strAnimation.c_str());
}

void RSingleScreenEffect::SetScreenEffect()
{
	// ��ũ�� ����Ʈ ���� Ȯ���� ���̱�
	string strFileName = m_strBaseFileName + ".elu";

	/// ��ũ�� ����Ʈ �߰� �� ����
	SetScreenEffectUnit(m_pUnit, strFileName, m_vPosEffect);
}

void RSingleScreenEffect::SetDraw(bool bVisible)
{
	m_pUnit->SetbVisible(bVisible);
}

void RSingleScreenEffect::AnimationStop()
{
	m_pUnit->AnimationStop();
}

bool RSingleScreenEffect::IsAnimationEnd()
{
	return m_pUnit->IsAnimationEnd();
}

void RSingleScreenEffect::SetVisibility( float f )
{
	m_pUnit->SetVisibility(f);
}

void RSingleScreenEffect::SetLastFrame()
{
	m_pUnit->SetLastFrame();
}
///////////////////////////////////////////////////////////////////////////////////////
//RCompositeScreenEffect///////////////////////////////////////////////////////////////
RCompositeScreenEffect::RCompositeScreenEffect(RSceneManager* pSceneManager) : RScreenEffect(pSceneManager)
{

}

RCompositeScreenEffect::~RCompositeScreenEffect()
{

}
void RCompositeScreenEffect::Create(string strScreenEffectName, 
									   string strBaseFileName, 
									   RSCREENEFFECT_EXIST_ATT existAtt,
									   RSCREENEFFECT_CHARACTER_ATT charAtt,
									   AnimationLoopType aniLoop)
{
	RScreenEffect::Create(strScreenEffectName,
							strBaseFileName,
							existAtt,
							charAtt,
							aniLoop);
}

void RCompositeScreenEffect::Destroy()
{
	ScreenEfeectUnitList_Iter it = m_vecUnit.begin();
	while(it != m_vecUnit.end())
	{
		(*it)->Destroy();
		SAFE_DELETE(*it);
		it++;
	}

	m_vecUnit.clear();
}

bool RCompositeScreenEffect::Render( RCameraSceneNode* pCurrentCamera)
{
	ScreenEfeectUnitList_Iter it = m_vecUnit.begin();
	while(it != m_vecUnit.end())
	{
		RScreenEffectUnit* pEffectUnit = *it;
		if(pEffectUnit->GetbVisible() &&
			pEffectUnit->GetActor())
		{
			pEffectUnit->GetActor()->Render( pCurrentCamera, -1, RENDERING_NULL);	/// ��� ������ ����̵� ����� ���� �׸��� ����� �ϰ����� �³�?
		}
		it++;
	}
	return true;
}

bool RCompositeScreenEffect::Update(DWORD dwElapsedTime)
{
	ScreenEfeectUnitList_Iter itUnit = m_vecUnit.begin();
	while(itUnit != m_vecUnit.end())
	{
		if((*itUnit)->GetbVisible())
		{
			(*itUnit)->Update(dwElapsedTime);
			if((*itUnit)->IsAnimationEnd() &&
				(m_ScreenEffectAniEndAtt == ANIMATION_END_DEL || m_ScreenEffectAniEndAtt == ANIMATION_END_HIDE) )
			{
				// EXIST_ONE�̴�.
				(*itUnit)->SetbVisible(false);
			}
		}

		itUnit++;
	}
	return true;
}

void RCompositeScreenEffect::SetPosition(vec3 vPosEffect, RSCREENEFFECT_LOCATION_TYPE scrBase)
{
	m_vPosEffect = vPosEffect;
	m_ScreenEffectLocation = scrBase;

	int nstrSize = m_strContents.size();
	int nLoopCount = 0;
	ScreenEfeectUnitList_Iter itEffecUnit = m_vecUnit.begin();
	while(itEffecUnit != m_vecUnit.end())
	{
		vec3 vNewPosEffect;

		// �ܾ� ��ũ�� ����Ʈ ��ġ ���� �ϱ�
		vNewPosEffect = SetWordPosition(m_vPosEffect, nLoopCount, nstrSize, m_ScreenEffectLocation);
		(*itEffecUnit)->SetPosition(vNewPosEffect);

		nLoopCount++;
		itEffecUnit++;
	}
}

void RCompositeScreenEffect::ChangeAnimation(string strAnimation, RSCREENEFFECT_ANIMATION_END_ATT aniDel)
{
	m_ScreenEffectAniEndAtt = aniDel;

	// �����ؼ� ����. �ִϸ��̼� ���� �� ó���� ��� �޽����� ������ �ȴ�.
	ScreenEfeectUnitList_Iter itUnit = m_vecUnit.begin();
	while(itUnit != m_vecUnit.end())
	{
		// �������� ���� �ʴ´ٸ�...
		if((*itUnit)->GetbVisible() == false)
		{
		}

		(*itUnit)->SetAnimation((char*)strAnimation.c_str());

		itUnit++;
	}
}

void RCompositeScreenEffect::SetString(string strContents)
{
	m_strContents = strContents;

	// ���� ���� ���� Ȯ��
	int nstrCurrentSize = m_vecUnit.size();
	int nstrGapSize = m_strContents.size() - nstrCurrentSize;

	// ���� ������ ���̰� �ִ���...
	if(nstrGapSize != 0)
	{
		// ���̰� �ִٸ� ��ü ���� �� �ٽ� ����
		Destroy();

		nstrGapSize = m_strContents.size();
	}

	// ���� �߰� ��ŭ �����Ѵ�.
	for(int i = 0; i < nstrGapSize; i++)
	{
		RScreenEffectUnit* pUnit = new RScreenEffectUnit;
		m_vecUnit.push_back(pUnit);
	}

	// ��Ʈ�� �޽� ���� �� ����
	SetScreenEffect();
}

void RCompositeScreenEffect::SetScreenEffect()
{
	// �ϳ��� �����ؾ� �Ѵٸ�...
	if(m_ScreenEffectExist == EXIST_ONE)
	{
		// ���� ��ũ�� ����Ʈ �缳��(������ ����Ʈ���� �����鼭 �ִϸ��̼� �ʱ�ȭ)
		ReSetScreenEffect();
	}

	// ���ο� ����
	// �ܾ� ����
	if(m_ScreenEffectChar == CHARACTER_WORD)
	{
		int nCount = 0;
		int nstrSize = m_strContents.size();
		ScreenEfeectUnitList_Iter itUnit = m_vecUnit.begin();
		while(itUnit != m_vecUnit.end())
		{
			vec3 vPosEffect;

			// �ܾ� ��ũ�� ����Ʈ ���� ��� �����
			string strFileName = m_strBaseFileName + '_' + m_strContents.c_str()[nCount] + ".elu";

			// �ܾ� ��ũ�� ����Ʈ ��ġ ���� �ϱ�
			vPosEffect = SetWordPosition(m_vPosEffect, nCount, nstrSize, m_ScreenEffectLocation);

			/// ��ũ�� ����Ʈ �߰� �� ����
			SetScreenEffectUnit((*itUnit), strFileName, vPosEffect);

			itUnit++;
			nCount++;
		}
	}
	else // �ϳ��� ���� ��
	{
		// ��ũ�� ����Ʈ ���� Ȯ���� ���̱�
		string strFileName = m_strBaseFileName + ".elu";

		/// ��ũ�� ����Ʈ �߰� �� ����
		SetScreenEffectUnit(m_vecUnit[0], strFileName, m_vPosEffect);
	}
}

void RCompositeScreenEffect::ReSetScreenEffect()
{
	ScreenEfeectUnitList_Iter itUnit = m_vecUnit.begin();

	while(itUnit != m_vecUnit.end())
	{
		if((*itUnit)->GetbVisible() == true)
		{
			(*itUnit)->SetbVisible(false);
		}

		itUnit++;
	}
}

vec3 RCompositeScreenEffect::SetWordPosition(const vec3& vBasePosEffect, 
											   int nCount, 
											   int nTotalCount,
											   RSCREENEFFECT_LOCATION_TYPE scrBase)
{
	vec3 vPosEffect = vBasePosEffect;

	const int letterSize = 50;
	switch(scrBase)
	{
		// ���� ����
	case LOCATION_LEFT:
		{
			vPosEffect.x = vPosEffect.x + (letterSize * (nCount));
		}
		break;
		// ������ ����
	case LOCATION_RIGHT:
		{
			vPosEffect.x = vPosEffect.x - (letterSize * (nTotalCount - nCount - 1));
		}
		break;
		// ��� ����
	case LOCATION_CENTER:
		{
			float fHalfLength = (letterSize * nTotalCount) / 2.0f;
			vPosEffect.x = (vBasePosEffect.x  - fHalfLength) + (letterSize * (nCount));
		}
		break;
	}

	return vPosEffect;
}

void RCompositeScreenEffect::SetDraw(bool bVisible)
{
	ScreenEfeectUnitList_Iter itUnit = m_vecUnit.begin();
	while(itUnit != m_vecUnit.end())
	{
		(*itUnit)->SetbVisible(bVisible);
		itUnit++;
	}
}

void RCompositeScreenEffect::AnimationStop()
{
	ScreenEfeectUnitList_Iter itUnit = m_vecUnit.begin();
	while(itUnit != m_vecUnit.end())
	{
		(*itUnit)->AnimationStop();
		itUnit++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// RScreenEffectManager ///////////////////////////////////////////////////////////////
RScreenEffectManager::RScreenEffectManager()
{
	m_mapCompositeScreenEffect.clear();
	m_vecSingleScreenEffect.clear();

	m_pSceneManager						= NULL;
	m_pLastInputSingleScreenEffect	= NULL;
}

RScreenEffectManager::~RScreenEffectManager()
{
	m_pSceneManager = NULL;
	m_pLastInputSingleScreenEffect	= NULL;
}

void RScreenEffectManager::Destroy()
{
	CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.begin();
	while(itComposite != m_mapCompositeScreenEffect.end())
	{
		itComposite->second->Destroy();
		SAFE_DELETE(itComposite->second);
		itComposite++;
	}

	SingleScreenEfeectList_Iter itSingle = m_vecSingleScreenEffect.begin();
	while(itSingle != m_vecSingleScreenEffect.end())
	{
		(*itSingle)->Destroy();
		SAFE_DELETE(*itSingle);
		itSingle++;
	}

	m_mapCompositeScreenEffect.clear();
	m_vecSingleScreenEffect.clear();
}

void RScreenEffectManager::Update()
{
	DWORD dwElapsedTime = m_pSceneManager->GetTimer().GetElapsedTime();

	CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.begin();
	while(itComposite != m_mapCompositeScreenEffect.end())
	{
		if(itComposite->second->Update(dwElapsedTime) == false)
		{
			// ����
			itComposite->second->Destroy();
			SAFE_DELETE(itComposite->second);
			itComposite = m_mapCompositeScreenEffect.erase(itComposite);
			continue;
		}

		itComposite++;
	}

	SingleScreenEfeectList_Iter itSingle = m_vecSingleScreenEffect.begin();
	while(itSingle != m_vecSingleScreenEffect.end())
	{
		if((*itSingle)->Update(dwElapsedTime) == false)
		{
			// ����
			(*itSingle)->Destroy();
			SAFE_DELETE(*itSingle);
			itSingle = m_vecSingleScreenEffect.erase(itSingle);
			continue;
		}

		itSingle++;
	}
}

void RScreenEffectManager::Render()
{
	RCameraSceneNode screenEffectCamera;
	screenEffectCamera.SetActive(true);
	screenEffectCamera.Set(RVector(0,0,0),RVector(0,1,0));
	screenEffectCamera.SetFov(14.32f * MMath::PI);
	screenEffectCamera.SetFogEnable(false);
	screenEffectCamera.BeginRender();

	REngine::GetDevice().SetDepthEnable(false, false);

	CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.begin();
	while(itComposite != m_mapCompositeScreenEffect.end())
	{
		itComposite->second->Render( &screenEffectCamera );
		itComposite++;
	}

	SingleScreenEfeectList_Iter itSingle = m_vecSingleScreenEffect.begin();
	while(itSingle != m_vecSingleScreenEffect.end())
	{
		(*itSingle)->Render( &screenEffectCamera );
		itSingle++;
	}

	REngine::GetDevice().SetDepthEnable(true, true);

	screenEffectCamera.EndRender();
}

void RScreenEffectManager::SetPosition(string strScreenEffectName, vec3 vPosEffect, RSCREENEFFECT_LOCATION_TYPE scrBase)
{
	if(strScreenEffectName.size() > 0)
	{
		CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.find(strScreenEffectName);
		if(itComposite != m_mapCompositeScreenEffect.end())
		{
			itComposite->second->SetPosition(vPosEffect, scrBase);
		}
	}
	else if(m_pLastInputSingleScreenEffect)
	{
		// �̱� ��ũ�� ����Ʈ�� �̸��� ����.
		m_pLastInputSingleScreenEffect->SetPosition(vPosEffect, scrBase);
	}
}

void RScreenEffectManager::SetString(string strScreenEffectName, string strContents)
{
	CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.find(strScreenEffectName);
	if(itComposite != m_mapCompositeScreenEffect.end())
	{
		itComposite->second->SetString(strContents);
	}
}

void RScreenEffectManager::ChangeAnimation(string strScreenEffectName, string strAnimation, RSCREENEFFECT_ANIMATION_END_ATT aniDel)
{
	CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.find(strScreenEffectName);
	if(itComposite != m_mapCompositeScreenEffect.end())
	{
		itComposite->second->ChangeAnimation(strAnimation, aniDel);
	}
}

void RScreenEffectManager::AddScreenEffect(string strListFileName, RSCREENEFFECT_CHARACTER_ATT charAtt, AnimationLoopType aniLoop)
{
	// AddScreenEffect������ EXIST_MULTI�� ����Ѵ�.
	RSingleScreenEffect * pScreenEffect = new RSingleScreenEffect(m_pSceneManager);
	m_pLastInputSingleScreenEffect = pScreenEffect;				// ���������� �Է��� ��ũ�� ����Ʈ �ּ�
	m_vecSingleScreenEffect.push_back(pScreenEffect);

	pScreenEffect->Create("",
							strListFileName,
							EXIST_MULTI,
							charAtt,
							aniLoop);
}

void RScreenEffectManager::RegisterScreenEffect(string strScreenEffectName, 
									   string strListFileName,
									   RSCREENEFFECT_CHARACTER_ATT charAtt,
									   AnimationLoopType aniLoop)
{
	_ASSERT( m_pSceneManager );
	// AddRegister������ EXIST_ONE�� ����Ѵ�.
	RScreenEffect * pScreenEffect = NULL;

	// EXIST_ONE�̸� ����Ʈ�� ���� �ϴ��� �˻�
	CompositeScreenEfeectList_Iter itList = m_mapCompositeScreenEffect.find(strScreenEffectName);
	if(itList != m_mapCompositeScreenEffect.end())
		pScreenEffect = (RCompositeScreenEffect *)itList->second;

	// �����
	if(pScreenEffect == NULL)
	{
		// ��ũ�� ����Ʈ ���
		if(charAtt == CHARACTER_WORD)
		{
			// �ܾ� ����
			pScreenEffect = new RCompositeScreenEffect(m_pSceneManager);
		}
		else if(charAtt == CHARACTER_NORMAL)
		{
			// �Ϲ�
			pScreenEffect = new RSingleScreenEffect(m_pSceneManager);
		}

		m_mapCompositeScreenEffect.insert(CompositeScreenEfeectList::value_type(strScreenEffectName, pScreenEffect));

		pScreenEffect->Create(strScreenEffectName,
								strListFileName,
								EXIST_ONE,
								charAtt,
								aniLoop);
	}
}
void RScreenEffectManager::SetDraw(string strScreenEffectName, bool bVisible)
{
	if(strScreenEffectName.size() > 0)
	{
		CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.find(strScreenEffectName);
		if(itComposite != m_mapCompositeScreenEffect.end())
		{
			itComposite->second->SetDraw(bVisible);
		}
	}
	else if(m_pLastInputSingleScreenEffect)
	{
		// �̱� ��ũ�� ����Ʈ�� �̸��� ����.
		m_pLastInputSingleScreenEffect->SetDraw(bVisible);
	}
}

void RScreenEffectManager::AnimationStop(string strScreenEffectName)
{
	if(strScreenEffectName.size() > 0)
	{
		CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.find(strScreenEffectName);
		if(itComposite != m_mapCompositeScreenEffect.end())
		{
			itComposite->second->AnimationStop();
		}
	}
	else if(m_pLastInputSingleScreenEffect)
	{
		// �̱� ��ũ�� ����Ʈ�� �̸��� ����.
		m_pLastInputSingleScreenEffect->AnimationStop();
	}
}

bool RScreenEffectManager::IsAnimationEnd( string strScreenEffectName )
{
	CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.find(strScreenEffectName);
	if(itComposite != m_mapCompositeScreenEffect.end())
	{
		return itComposite->second->IsAnimationEnd();
	}

	return false;
}

void RScreenEffectManager::SetVisibility( string strScreenEffectName, float f )
{
	if(strScreenEffectName.size() > 0)
	{
		CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.find(strScreenEffectName);
		if(itComposite != m_mapCompositeScreenEffect.end())
		{
			itComposite->second->SetVisibility(f);
		}
	}
	else if(m_pLastInputSingleScreenEffect)
	{
		// �̱� ��ũ�� ����Ʈ�� �̸��� ����.
		m_pLastInputSingleScreenEffect->SetVisibility(f);
	}
}

void RScreenEffectManager::SetLastFrame( string strScreenEffectName )
{
	if(strScreenEffectName.size() > 0)
	{
		CompositeScreenEfeectList_Iter itComposite = m_mapCompositeScreenEffect.find(strScreenEffectName);
		if(itComposite != m_mapCompositeScreenEffect.end())
		{
			itComposite->second->SetLastFrame();
		}
	}
	else if(m_pLastInputSingleScreenEffect)
	{
		// �̱� ��ũ�� ����Ʈ�� �̸��� ����.
		m_pLastInputSingleScreenEffect->SetLastFrame();
	}
}
}