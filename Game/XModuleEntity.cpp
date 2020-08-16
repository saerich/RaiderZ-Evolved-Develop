#include "stdafx.h"
#include "XModuleEntity.h"
#include "XEventID.h"
#include "XItem.h"
#include "XModuleMotion.h"
#include "XConst.h"
#include "XItemManager.h"
#include "XEffectManager.h"
#include "XMyPlayer.h"
#include "XNonPlayer.h"
#include "CSMeshInfo.h"

#include "XNaming.h"
#include "XAnimationEventHandler.h"

#include "RDebugPassRenderer.h"
#include "RCameraSceneNode.h"
#include "RBackgroundWorker.h"

#include "XCharacterSerializer.h"
#include "XSystem.h"
#include "XGameTransformControllerManager.h"
#include "MockAnimationController.h"
#include "MockCharacter.h"
#include "XEntityFader.h"
#include "XEntityDirectionInterpolator.h"

#include "MPhysX4R3/MPxAttacherTC.h"
#include "MPhysX4R3/MPxRagDollTC.h"
#include "MPhysX4R3/MPxDynamicActorNodeTC.h"
#include "MPhysX4R3/MPxAniLookUpTC.h"

#include "XSceneManager.h"
#include "XResourceManager.h"

#include "RMesh.h"

#include "XGameTransformControllerManager.h"
#include "MPhysX4R3/RPhysXActorController.h"
#include "XModuleCollision.h"

void XVictimFrameFreezer::FreezeFrame( XObject* pOwner )
{
	m_bReserve = false;

	const float HIT_FREEZE_TIME = 0.3f;

	if (HIT_FREEZE_TIME > 0.001f)
	{
		pOwner->GetModuleEntity()->GetAnimationController()->FreezeFrame(HIT_FREEZE_TIME);
	}
}

void XVictimFrameFreezer::OnHit( XObject* pOwner, const wchar_t* szCurrAni )
{
	const float HIT_FREEZE_DELAY = 0.0f;

	if (HIT_FREEZE_DELAY <= 0.001f)
	{
		FreezeFrame(pOwner);
		return;
	}

	m_bReserve = true;
	m_fReserveElapsedTime = 0.0f;
	m_strCurrAni = szCurrAni;
}

void XVictimFrameFreezer::Update( float fDelta, XObject* pOwner, const wchar_t* szCurrAni )
{
	if (m_bReserve == false) return;
	if (wcscmp(m_strCurrAni.c_str(), szCurrAni))
	{
		m_bReserve = false;
		return;
	}

	m_fReserveElapsedTime += fDelta;


	const float HIT_FREEZE_DELAY = 0.0f;

	if (m_fReserveElapsedTime >= HIT_FREEZE_DELAY)
	{
		m_bReserve = false;
		FreezeFrame(pOwner);
	}
}

//////////////////////////////////////////////////////////////////////////
XModuleEntity::XModuleEntity(XObject* pOwner) 
: XModule(pOwner)
, m_fScale(1.f)
, m_pNPCInfo(NULL)
, m_uidExtraTarget(MUID::ZERO)
, m_fMaxVisibility(1.0f)
, m_pFader(NULL)
, m_bAlpha2PassRendering(false)
{
	m_bLoadingCompleted = false;
	m_bInternalWarp = false;

	// Ư���� Transform�� �ϱ����� Transform Controller
	class XPxTcEntity : public MPxTcEntity
	{
	public:
		XPxTcEntity(XModuleEntity* pOwnerEntity) : m_pOwnerEntity(pOwnerEntity) {}
		virtual ~XPxTcEntity(){}

		virtual rs3::RActor* GetActor()
		{
			return m_pOwnerEntity->GetActor();
		}

		virtual MPxTransformController* GetTransformController(int nId)
		{
			return m_pOwnerEntity->GetTransformController(nId);
		}

		virtual bool IsAvailableActor(rs3::RActor* pActor)
		{
			return false;
		}

		virtual void OnDynamicObjectCollided(NxActor* _pNxActor, MPxDynamicObjectEntity* _pDynamicObjEntity)
		{
			XObject* pOwnerObject = m_pOwnerEntity->GetOwner();
			if (NULL == pOwnerObject)
				return;

			XModuleCollision* pModuleCollision = pOwnerObject->GetModuleCollision();
			if (NULL == pModuleCollision)
				return;

			CSMeshInfo* pMeshInfo = pModuleCollision->GetMeshInfo();
			if (NULL == pMeshInfo)
				return;

			RPhysXActorController* pActorController = _pDynamicObjEntity->GetPhysXActorController();
			if(NULL == pActorController)
				return;

			RActorNode* pRelatedActorNode = pActorController->GetActorNodeFromUpdateMap(_pNxActor);
			if (NULL == pRelatedActorNode)
				return;

			const std::string& rActorNodeName = pRelatedActorNode->GetNodeName();

			//b_parts_0.length = 9
			int nBPartIdx = atoi(rActorNodeName.substr(9, 1).c_str());
			if (nBPartIdx < 1 || nBPartIdx > 4)
				return;

			//������� ������ ��������Ʈ ���ְ� �ʿ����.
			const std::tstring& strPlaySoundName = pMeshInfo->m_strBPartsCollisionSound[nBPartIdx-1];
			if (strPlaySoundName.empty())
				return;

			// Mass Rank 1~8 ������ ��������Ʈ
			int nMassRank = _pDynamicObjEntity->GetMassRank(rActorNodeName);
			if (nMassRank < 1 || nMassRank > 8)
				return;

			bool bDo = false;
			MAP_DYN_OBJ::iterator itr = m_mapDynamicObject.find(pRelatedActorNode);
			if (itr == m_mapDynamicObject.end())
			{
				bDo = true;
				m_mapDynamicObject.insert(MAP_DYN_OBJ::value_type(pRelatedActorNode, pOwnerObject->GetUID()) );
			}
			else if (itr->second != pOwnerObject->GetUID())
			{
				bDo = true;
				itr->second = pOwnerObject->GetUID();
			}

			if (bDo)
			{
				if ( global.sound)
					global.sound->PlayDynamic3D( MLocale::ConvUTF16ToAnsi(strPlaySoundName.c_str()).c_str(), NULL, 0, pOwnerObject->GetUID(), rActorNodeName.c_str());
			}
		}

	protected:
		XModuleEntity* m_pOwnerEntity;

		//typedef std::map< MPxDynamicObjectEntity*, MUID > MAP_DYN_OBJ;
		//std::map< MPxDynamicObjectEntity*, MUID > m_mapDynamicObject;

		typedef std::map< RActorNode*, MUID > MAP_DYN_OBJ;
		std::map< RActorNode*, MUID > m_mapDynamicObject;
	};

	m_pTcEntity = new XPxTcEntity(this);
	m_bPauseTransformControl = false;

	int nTcID = 0;
	MPxTransformController* pTc = NULL;

	pTc = new MPxAttacherTC(m_pTcEntity);
	MPxAttacherTC::SetID(nTcID++);
	m_vecTransformControllers.push_back(pTc);

	pTc = new MPxRagDollTC(m_pTcEntity);
	MPxRagDollTC::SetID(nTcID++);
	m_vecTransformControllers.push_back(pTc);

	pTc = new MPxDynamicActorNodeTC(m_pTcEntity);
	MPxDynamicActorNodeTC::SetID(nTcID++);
	m_vecTransformControllers.push_back(pTc);

	pTc = new MPxAniLookUpTC(m_pTcEntity);
	MPxAniLookUpTC::SetID(nTcID++);
	m_vecTransformControllers.push_back(pTc);

	#ifdef _DEBUG
	for ( int i = 0; i < nTcID; ++i)
	{
		_ASSERT(m_vecTransformControllers[i] != NULL);
	}
	#endif

	// Actor�� AnimationController �����
	if (global.system->IsResourceLoading() == true)
	{
		m_pActor						= global.actor_loader->CreateActor();// new XCharacter();	�� ������ ����/������ interface�� ���� ��Ű�� ���� �̷��� �����Ѵ�. 
		m_pAnimationController			= new XGameTransformControllerManager(m_pActor, GetTransformController<MPxAniLookUpTC>());
	}
	else
	{
		// ���� ���ҽ� �ε��� ���ϸ� Mock���� ������. - birdkr
		m_pActor						= new MockCharacter();
		m_pAnimationController			= new MockAnimationController(m_pActor, GetTransformController<MPxAniLookUpTC>());
	}

	m_pActor->SetUsable(false);
	m_pActor->SetVisible(false);

	bEntitySoundPlaying				= false;

	// ������ ����
	m_pActor->SetActorListener(this);

	// Entity �ڿ�����
	m_pSerializer					= new XCharacterSerializer(this);
	m_pCharExtFeature				= NULL;

	m_pDirectionInterpolator = new XEntityDirectionInterpolator(this);

	m_RegMultiplyColor.Stop();

	// ���� �׸��� ���(��� ����)
	m_pActor->SetUseDoubleShadow(true);

	InitRenderPosDirUp();
}

XModuleEntity::~XModuleEntity(void)
{
	SAFE_DELETE(m_pDirectionInterpolator);

	if (m_pFader)
	{
		SAFE_DELETE(m_pFader)
	}

	SAFE_DELETE(m_pSerializer);

	// transform controller �����
	for (VEC_TC::iterator itr = m_vecTransformControllers.begin(); itr != m_vecTransformControllers.end(); ++itr)
	{
		delete *itr;
	}
	SAFE_DELETE(m_pTcEntity);

	SAFE_DELETE(m_pAnimationController);

	// ���� �����
	if (global.actor_loader)
		global.actor_loader->DeleteActor(m_pActor);
	else
	{
		// ���� �׽�Ʈ�� ���
		SAFE_DELETE( m_pActor );
	}

	// �����ִ� ����Ʈ ����
	DelAllLoopAniEffect();
}

void XModuleEntity::OnAdd()
{

}

void XModuleEntity::OnRemove()
{

}

void XModuleEntity::OnActivate()
{
	AddToSceneManager();
}

void XModuleEntity::OnDeactivate()
{
	// by pok. ���Ŵ��� ������
	RemoveFromSceneManager();
}

void XModuleEntity::OnUpdate(float fDelta)
{
	PFC_THISFUNC;

	//wstring name(GetOwner()->GetName());
	//if (  name  == L"��ޱ�� ���׽�")
	//{
	//	mlog("Here");
	//}

	//----------------------------------------------------------------------------
	// �ε� üũ ����
	CheckLoadingCompleted();

	PFI_BLOCK_THISFUNC(303);

	// module update
	XModule::OnUpdate(fDelta);

	// serialize update
	m_pSerializer->Update();

	//----------------------------------------------------------------------------
	// �ִϸ��̼� ������Ʈ ������ ó�� �ؾ� �ϴ� ����
	if (m_pFader)
	{
		m_pFader->Update(fDelta);
	}

	if (m_pDirectionInterpolator)
	{
		m_pDirectionInterpolator->Update(fDelta);
	}

	//----------------------------------------------------------------------------
	// �ִϸ��̼� ��Ʈ�� ������Ʈ ����
	m_pAnimationController->Update(fDelta);

	//----------------------------------------------------------------------------
	// �ִϸ��̼� ������Ʈ ���Ŀ� ó�� �ؾ� �ϴ� ����
	// Physx�� �ִϸ��̼� ��Ʈ�� ������Ʈ ���Ŀ� �ؾ� �Ѵ�.
	// special transform control update
	if ( m_bPauseTransformControl == false )
	{
		for (VEC_TC::iterator itr = m_vecTransformControllers.begin(); itr != m_vecTransformControllers.end(); ++itr)
		{
			MPxTransformController* pTc = *itr;
			_ASSERT(pTc);
			if (pTc->IsEnabled())
				pTc->Update(fDelta);
		}
	}

	m_pActor->UpdateAllSceneNodes();

	#ifdef _DEBUG
	m_debugInfos.clear();
	#endif

	if (m_pActor)
	{
		if(m_RegMultiplyColor.IsActive() && m_RegMultiplyColor.IsReady(fDelta))
		{
			m_pActor->DisableAddColorEffect();
			m_RegMultiplyColor.Stop();
		}

		for(int iBParts = 0; iBParts < BREAKABLEPARTS_MAX_COUNT; ++iBParts)
		{
			if(m_RegMultiplyColorForBParts[iBParts].regMultiplyColor.IsActive() && m_RegMultiplyColorForBParts[iBParts].regMultiplyColor.IsReady(fDelta))
			{
				m_pActor->DisableAddColorEffect(m_RegMultiplyColorForBParts[iBParts].strActorNodeName.c_str());
				m_RegMultiplyColorForBParts[iBParts].regMultiplyColor.Stop();
				m_RegMultiplyColorForBParts[iBParts].strActorNodeName.clear();
			}
		}
	}

	m_FrameFreezer.Update(fDelta, m_pOwner, m_pAnimationController->GetAnimationName().c_str());
}

void XModuleEntity::OnInitialized()
{
	m_vPos	= vec3::ZERO;
	m_vDir	= -vec3::AXISY;
	m_vUp	= UP_VECTOR;

	_SetPosition(m_vPos);
	_SetDirection(m_vDir);
}

void XModuleEntity::OnFinalized()
{

}



void XModuleEntity::_SetPosition(const vec3& pos)
{
	m_vPos.Set(pos);
	_UpdateTransform();
}

void XModuleEntity::_SetDirection(const vec3& dir)
{
	if (dir == vec3::ZERO)
		return;

	m_vDir.Set(dir);
	_UpdateTransform();
}


void XModuleEntity::SetVisible(bool bVisible)
{
	if (m_bLoadingCompleted == false) return;

	m_pActor->SetVisible(bVisible);
}

bool XModuleEntity::IsVisible()
{
	return m_pActor->GetVisible();
}

bool XModuleEntity::IsHaveDummy()
{
	return m_pActor->IsExistActorNode(L"dummy_loc");
}

bool XModuleEntity::GetMovingDummyPos(vec3& vAnimPos)
{
	if (m_pActor->GetMovingDummyPosition(vAnimPos))
		return true;

	return false;
}

bool XModuleEntity::GetMovingDummyDir(vec3& vAnimDir)
{
	if (m_pActor->GetMovingDummyDirection(vAnimDir))
		return true;

	return false;
}

vec3 XModuleEntity::GetAnimationPos()
{
	vec3 vAnimPos = vec3::ZERO;

	if (GetMovingDummyPos(vAnimPos))
	{
		MMatrix tm;
		tm.SetLocalMatrix(vec3::ZERO, -GetDirection(), vec3::AXISZ);
		vec3 ret = vAnimPos * tm;

		return ret;
	}

	return vec3::ZERO;
}

vec3 XModuleEntity::GetAnimationDir()
{
	vec3 vAnimDir = GetDirection();

	if (GetMovingDummyDir(vAnimDir))
	{
		mat4 tm;
		tm.SetLocalMatrix(vec3::ZERO, -GetDirection(), vec3::AXISZ);

		vec3 ret = vAnimDir * tm;
		ret.Normalize();

		return ret;
	}

	return (-vec3::AXISY);
}

void XModuleEntity::OnAnimationEvent(RActor* pActor, const RAnimationEvent* pEvent)
{
	XAnimationEventHandler::GetInstance().OnEvent(pActor, pEvent, *this);

}

vec3 XModuleEntity::GetPosition()
{
	return m_vPos; 
}


void XModuleEntity::SerializeMyPlayer(PLAYER_INFO& playerInfo, XMyEquipmentSlot* pEquipmentSlot)
{
	m_pSerializer->SerializeMyPlayer(playerInfo, pEquipmentSlot);

	XCharExtFeature _feature;
	_feature.nMakeUp = playerInfo.nMakeUp;
	_feature.nTattoo = playerInfo.nTattooType;
	_feature.fTattooScale = (float)playerInfo.nTattooScale * 0.02f;
	_feature.vTattooPos.x = (float)playerInfo.nTattooPosX * 0.01f;
	_feature.vTattooPos.y = (float)playerInfo.nTattooPosY * 0.01f;
	SetCharExtFeature( &_feature);
}


void XModuleEntity::SerializeNetPlayer( bool bLoadingAsync /*= true*/ )
{
	if (MIsExactlyClass(XNetPlayer, m_pOwner) == false) return;

	XNetPlayer* pNetPlayer = AsNetPlayer(m_pOwner);
	XPlayerInfoFeature& feature = pNetPlayer->GetFeature();

	// mesh serialize
	XResourceLoadingPolicy policy = LOADING_ASYNC;
	if(bLoadingAsync == false)
		policy = LOADING_SYNC;

	m_pSerializer->SerializeNetPlayer(pNetPlayer, feature, policy);

	XCharExtFeature _feature;
	_feature.nMakeUp = feature.nMakeUp;
	_feature.nTattoo = feature.nTattoo;
	_feature.fTattooScale = feature.fTattooScale;
	_feature.vTattooPos = feature.vTattooPos;
	SetCharExtFeature( &_feature);
}

void XModuleEntity::SerializeNPC( XNPCInfo* pNPCInfo, bool bLoadingAsync /*= true*/ )
{
	if (global.smgr == NULL) return;

	m_pNPCInfo = pNPCInfo;

	XResourceLoadingPolicy policy = LOADING_ASYNC;
	if(bLoadingAsync == false)
		policy = LOADING_SYNC;

	m_pSerializer->SerializeNPC(pNPCInfo, m_fScale, policy);
}

void XModuleEntity::OnSubscribeEvent()
{
	Subscribe(XEVTL_ON_CHANGE_MOTION);
	XModule::OnSubscribeEvent();	
}

XEventResult XModuleEntity::OnEvent(XEvent& msg)
{
	switch (msg.m_nID)
	{
	case XEVTL_ON_CHANGE_MOTION:
		{
			// TODO: 
			// [10/16/2007 madduck] �� ���� �ִ� ����Ʈ�� ������� �ϴ� ������ �ָ��ϴ�..... 
			//m_pOwner->GetModuleEntity()->DelAllLoopAniEffect();			
			DelAllLoopAniEffect();
		}
		break;
	}

	return MR_FALSE;
}

void XModuleEntity::DelAllLoopAniEffect()
{
	for( size_t i = 0; i < m_vLoopAniEffect.size(); i++)
	{
		if (global.emgr)
			global.emgr->DeleteXEffect(m_vLoopAniEffect[i].m_uid);
	}

	m_vLoopAniEffect.clear();
}
void XModuleEntity::_SetUpVector( const vec3& up )
{
	if (up == vec3::ZERO) 
		m_vUp.Set(UP_VECTOR);

	m_vUp.Set(up);
	m_vDir.z = 0.0f;
	_UpdateTransform();
}

void XModuleEntity::_SetRenderPosDir( const vec3& rpos, const vec3& rdir)
{
	m_vRenderPos.Set(rpos);
	m_vRenderDir.Set(rdir);

	_UpdateTransform();
}

void XModuleEntity::_SetRenderPosition( const vec3& rpos)
{
	m_vRenderPos.Set(rpos);

	_UpdateTransform();
}

void XModuleEntity::_SetRenderDirection( const vec3& rdir)
{
	m_vRenderDir.Set(rdir);

	_UpdateTransform();
}

void XModuleEntity::_SetRenderUpDir( const vec3& rup, const vec3& rdir )
{
	m_vRenderDir.Set(rdir);
	m_vRenderUp.Set(rup);

	_UpdateTransform();
}

void XModuleEntity::_UpdateTransform()
{
	// ���ﶧ�� update transform�� ���� �ʴ´�.
	_ASSERT(m_pOwner);
	if (m_pOwner->GetReferenceCount() == 0)
		return;

	if ( GetTransformController<MPxAttacherTC>()->IsEnabled())
		return;


	MMatrix mat;
	mat.SetTranslationMatrix( m_vRenderPos);
	mat.SetScale(m_fScale);

	MMatrix tm;
	if ( m_vRenderDir != vec3::ZERO)	tm.SetLocalMatrix( m_vPos, -m_vRenderDir, m_vRenderUp);
	else								tm.SetLocalMatrix( m_vPos, -m_vDir, m_vUp);
	mat *= tm;


	// �� ����� �ɸ���, UpdateForRender �� �Ҹ� ���Ŀ� �ٽ� ��ġ ������ �ϴ°��̸�, Į����� �������� �ʰ� ������Ե˴ϴ�.
	_ASSERT( !m_pActor->CheckAlreadyUpdatedForRender() );
	if (m_pActor->CheckAlreadyUpdatedForRender())
	{
//		mlog("From XModuleEntity::_UpdateTransform() : Already called UpdateForRender!\n");
	}

	// SetTransform���� �����ϰ��� ���� �����Ƿ� Matrix���� �������� �־�� �Ѵ�. 
	m_pActor->SetTransform( mat);

	// ���� �ε����϶� ����Ǹ� ���´�.
//	if(m_pActor->GetUsable())
	{
		m_pActor->UpdateTransform();
		m_pActor->UpdateVisibility();
	}
}

void XModuleEntity::AddToSceneManager()
{
	if (global.smgr)
	{
		// ���������� Warp�� �ѰͰ� ������ ȿ���� ��Ÿ����.
		OnWarp();
		global.smgr->AddSceneNode(m_pActor);
	}
}

bool XModuleEntity::RemoveFromSceneManager()
{
	if (m_pActor) return m_pActor->RemoveFromParent();
	return false;
}

void XModuleEntity::OnLoadingComplete()
{
	// Ȯ�� ������ ����
	SetCharExtFeature( m_pCharExtFeature);

	// �ִϸ��̼� ��Ʈ�ѷ�
	m_pAnimationController->OnLoadingComplete();

	// ���� Ʈ������ ��Ʈ�ѷ�
	//GetTransformController<MPxRagDollTC>()->InitRagDollInfoFromModelName( m_pSerializer->GetMeshName(), SH_PX_COL_RAGDOLL );
	//GetTransformController<MPxRagDollTC>()->InitRagDollInfoFromModelName( MLocale::ConvUTF16ToAnsi(m_pSerializer->GetMeshName().c_str()), NULL);

	// ���̳��� ������Ʈ Ʈ������ ��Ʈ�ѷ�
	//GetTransformController<MPxDynamicActorNodeTC>()->GrabDynamicActorShape();


	if (m_pActor && m_pActor->m_pMesh && global.res)
	{
		global.res->OnCreateResource(m_pActor->m_pMesh);

		//mlog("�޽��ε� ���ø�Ʈ? %d %d\n", m_pActor->m_pMesh->IsLoadingCompleted(), m_pActor->m_pMesh->IsLoadingSucceeded());
	}

	// �ε� �Ϸ� �̺�Ʈ �߻�
	XEvent evt;
	evt.m_nID = XEVTD_MESH_LOADING_COMPLETE;
	m_pOwner->Event(evt);

	// ���� 2 �н� üũ
	if(m_pActor->m_pMesh)
		m_bAlpha2PassRendering = m_pActor->m_pMesh->IsAlpha2PassRendering();
}

void XModuleEntity::SetTransform( const MMatrix& mat, const MPxTransformController* pCaller )
{
	// �� ����� �ɸ���, UpdateForRender �� �Ҹ� ���Ŀ� �ٽ� ��ġ ������ �ϴ°��̸�, Į����� �������� �ʰ� ������Ե˴ϴ�.
	_ASSERT( !m_pActor->CheckAlreadyUpdatedForRender() );
	if (m_pActor->CheckAlreadyUpdatedForRender())
	{
//		mlog("From XModuleEntity::SetTransform(mat, MPxTransformController) : Already called UpdateForRender!\n");
	}

	// Actor ����
	m_pActor->SetTransform(mat);
	m_pActor->UpdateTransform();

	// ModuleEntity�� ����ȭ
	m_vPos	= m_pActor->GetWorldPosition();
	m_vDir	= m_pActor->GetWorldDirection() * -1;
	m_vUp	= m_pActor->GetWorldUp();

	//REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugBox(m_pActor->GetWorldTransform(), 30, 10.0f, D3DCOLOR_XRGB(255, 255, 255));
}

void XModuleEntity::CheckLoadingCompleted()
{
	if (m_pActor == NULL) return;
	if (m_bLoadingCompleted == true) return;
	if (m_pActor->GetVisible() == true) return;

	if (m_pActor->IsLoadingCompleted())
	{
		m_bLoadingCompleted = true;
		m_pActor->SetVisible(true);
	}
}

void XModuleEntity::SetCharExtFeature( const XCharExtFeature* pExtFeature)
{
	if ( pExtFeature == NULL)
		return;

	if ( IsLoadingCompleted() == true)
	{
		RActorNode* pActorNode = m_pActor->GetActorNode( "face");
		if ( pActorNode != NULL  &&  pActorNode->m_pMeshNode != NULL  &&  pActorNode->m_pMeshNode->m_pParentMesh != NULL)
		{
			RMesh* pMesh = pActorNode->m_pMeshNode->m_pParentMesh;
			map< string/*material name*/, pair<int/*index*/, int/*count*/>> vMaterials;
			int _size = pMesh->GetMaterialResourceCount();
			for ( int i = 0;  i < _size;  i++)
			{
				string name = pMesh->GetMaterialResource( i)->m_pMaterial->GetName();

				map<string,pair<int,int>>::iterator _find = vMaterials.find( name);
				if ( _find == vMaterials.end())
					vMaterials.insert( map<string,pair<int,int>>::value_type( name, pair<int,int>( i, 1)));
				else
					(*_find).second.second++;
			}

			// Set make up
			int nMakeUp = min( max( 0, (int)vMaterials.size() - 1),  max( 0, pExtFeature->nMakeUp));
			if ( true)
			{
				map<string,pair<int,int>>::iterator itr = vMaterials.begin();
				for ( int i = 0;  i < nMakeUp;  i++, itr++);

  				for ( int i = 0;  i < min( 3, (*itr).second.second);  i++)
					m_pActor->ChangeMatetial( "face", i, (*itr).second.first + i, 0.0f);
			}

			// Set tattoo
			if ( true)
			{
				for ( map<string,pair<int,int>>::iterator itr = vMaterials.begin();  itr != vMaterials.end();  itr++)
					m_pActor->GetMaterialDecal()->RemoveDecalLayer( (*itr).first);

				if ( pExtFeature->nTattoo > 0)
				{
					RTextureDesc DiffuseMap;
					DiffuseMap.nMapChannel = 1;
					DiffuseMap.TextureName = XCONST::DEFAULT_TATTOO[ pExtFeature->nTattoo - 1];
					DiffuseMap.UVTransform.SetScale( RVector2( pExtFeature->fTattooScale, pExtFeature->fTattooScale));
					DiffuseMap.UVTransform.SetOffset( pExtFeature->vTattooPos);
					DiffuseMap.textureAddressU = RTADDRESS_CLAMP;
					DiffuseMap.textureAddressV = RTADDRESS_CLAMP;

					RTextureDesc OpacityMap;
					OpacityMap.TextureName = "tattoo_mask.dds";
					OpacityMap.nMapChannel = 0;

					for ( map<string,pair<int,int>>::iterator itr = vMaterials.begin();  itr != vMaterials.end();  itr++)
						m_pActor->GetMaterialDecal()->AddDecalLayer( (*itr).first, &DiffuseMap, NULL);
				}
			}
		}

		if ( m_pCharExtFeature != NULL)
		{
			delete m_pCharExtFeature;
			m_pCharExtFeature = NULL;
		}
	}
	else
	{
		if ( m_pCharExtFeature == NULL)
			m_pCharExtFeature = new XCharExtFeature();

		m_pCharExtFeature->nMakeUp = pExtFeature->nMakeUp;
		m_pCharExtFeature->nTattoo = pExtFeature->nTattoo;
		m_pCharExtFeature->fTattooScale = pExtFeature->fTattooScale;
		m_pCharExtFeature->vTattooPos = pExtFeature->vTattooPos;
	}
}

bool XModuleEntity::IsLoadingCompleted()
{
	// �ý��ۿ��� ���ҽ� �ε��� ���Ѵٸ� �׳� true�� �����Ѵ�(unit test ���� ���)
	if (global.system->IsResourceLoading() == false)
	{
		return true;
	}

	return m_bLoadingCompleted;
}

bool XModuleEntity::Pick( RPICKINFO& pickInfo )
{
	if (m_pActor == NULL) return false;
	return m_pActor->Pick(pickInfo);
}

void XModuleEntity::OnWarp()
{
	if (m_bInternalWarp)
		return;

	// Animation LookUp Controller���� Warp �Ǿ����� �˸�
	// GetTransformController<MPxAniLookUpTC>()->OnLookUpWarp();
}

void XModuleEntity::InitRenderPosDirUp()
{
	m_vRenderPos = vec3::ZERO;
	m_vRenderDir = vec3::ZERO;
	m_vRenderUp = vec3::AXISZ;
}

void XModuleEntity::SetVisibility( float fVisibility )
{
	if (m_pActor)
	{
		if (fVisibility >= 1.0f &&
			m_bAlpha2PassRendering == false &&
			(m_pActor->m_pMesh && m_pActor->m_pMesh->IsAlpha2PassRendering() == false)) // �޽ð� ����2�н� ���� �Ӽ��̸� ������ ����2�н���
		{
			m_pActor->SetAlpha2PassRendering(false);
		}
		else
		{
			// ����2�н��� �������ϰ� �׷����� �ȵȴ�.
			if (fVisibility >= 1.f)
				fVisibility = 0.9999f;

			m_pActor->SetAlpha2PassRendering(true);
		}

		if (fVisibility > m_fMaxVisibility) fVisibility = m_fMaxVisibility;
		m_pActor->SetVisibility(fVisibility);
	}
}

void XModuleEntity::SetMaxVisibility( float fMaxVisibility )
{
	m_fMaxVisibility = fMaxVisibility;
}

void XModuleEntity::StartFade( float fAlpha, float fTime )
{
	if (fAlpha > m_fMaxVisibility) fAlpha = m_fMaxVisibility;

	if (m_pFader == NULL)
	{
		m_pFader = new XEntityFader(m_pActor);
	}

	m_pFader->Start(fAlpha, fTime);
}

bool XModuleEntity::IsFading()
{
	if (m_pFader)
	{
		return m_pFader->IsDoing();
	}
	return false;
}

void XModuleEntity::StopFade()
{
	if (m_pFader)
	{
		m_pFader->Stop();
	}
}

float XModuleEntity::GetVisibility()
{
	if (m_pActor) return m_pActor->GetVisibility();
	return 0.0f;
}


void XModuleEntity::CreateFaceAnimation( bool bNPC /*= false*/ )
{
	m_pAnimationController->SetFaceAnimationNPC(bNPC);
	SetFaceAnimationActive(true);
}

void XModuleEntity::SetFaceAnimationActive( bool bActive )
{
	m_pAnimationController->SetFaceAnimationActive(bActive);
}

void XModuleEntity::SetDirectionSmoothly( const vec3& dir, float fRotateSpeed )
{
	vec3 vCurrDir = GetRenderDirection();
	if (vCurrDir == vec3::ZERO)
	{
		vCurrDir = GetDirection();
	}

	m_pDirectionInterpolator->Start(vCurrDir, dir, fRotateSpeed);
}

void XModuleEntity::StopDirectionInterpolation()
{
	m_pDirectionInterpolator->Stop();
}

bool XModuleEntity::IsDirectionInterpolatingNow()
{
	return m_pDirectionInterpolator->IsDoing();
}

bool XModuleEntity::IsAnimationUpdateCompleted()
{
	return m_pAnimationController->IsUpdated();
}

void XModuleEntity::ReSetAnimationUpdateCompleted()
{
	m_pAnimationController->ReSetUpdated();
}

void XModuleEntity::StartMultiplyColor()
{
	if (m_pActor)
	{
		m_RegMultiplyColor.SetTickTime(0.1f);
		m_RegMultiplyColor.Start();

		m_pActor->EnableAddColorEffect(vec3(XCONST::HIT_EFFECT_MODEL_ADD_COLOR.x, XCONST::HIT_EFFECT_MODEL_ADD_COLOR.y, XCONST::HIT_EFFECT_MODEL_ADD_COLOR.z));
	}
}

void XModuleEntity::StartFrameFreeze()
{
	m_FrameFreezer.OnHit(m_pOwner, m_pAnimationController->GetAnimationName().c_str() )	;
}

void XModuleEntity::CreateFakeBeatenAnimationController(tstring& strAniName, float fWeight, float fSpeed)
{
	m_pAnimationController->InitFakeBeatenAnimation(strAniName, fWeight, fSpeed);
}

void XModuleEntity::StartFakeBeaten(WEAPON_TYPE nAttackWeaponType, float fCriWeightRate, float fCriSpeedRate)
{
	// ���⺰ ������
	float fWeightRate = 1.0f;
	float fSpeedRate = 1.0f;

	if(nAttackWeaponType != WEAPON_NONE)
	{
		fWeightRate = XCONST::HIT_FAKE_BEATEN_WEAPON_TYPE_BLEND_WEIGHT[nAttackWeaponType - 1];
		fSpeedRate = XCONST::HIT_FAKE_BEATEN_WEAPON_TYPE_BLEND_SPEED[nAttackWeaponType - 1];
	}

	fWeightRate *= fCriWeightRate;
	fSpeedRate *= fCriSpeedRate;

	if (m_pOwner->IsNPC())
	{
		m_pAnimationController->StartFakeBeaten(GetAnimationController()->GetAnimationName().c_str(), fWeightRate, fSpeedRate, m_pOwner->AsNPC()->GetModeAnimationNameHeader().c_str());
	}
	else if (m_pOwner->IsPlayer())
	{
		WEAPON_TYPE nWeaponType = m_pOwner->AsPlayer()->GetCurrWeaponType();
		CHAR_STANCE nStance = m_pOwner->AsPlayer()->GetStance();

		m_pAnimationController->StartFakeBeaten(GetAnimationController()->GetAnimationName().c_str(), fWeightRate, fSpeedRate, XNaming::PlayerAnimationNameHeader(nWeaponType, nStance).c_str());
	}
}

void XModuleEntity::StartMultiplyColorForBParts( wstring strActorNodeName, int nBPartsIndex )
{
	if (m_pActor && nBPartsIndex < BREAKABLEPARTS_MAX_COUNT)
	{
		m_RegMultiplyColorForBParts[nBPartsIndex].regMultiplyColor.SetTickTime(0.2f);
		m_RegMultiplyColorForBParts[nBPartsIndex].regMultiplyColor.Start();
		m_RegMultiplyColorForBParts[nBPartsIndex].strActorNodeName = strActorNodeName;

		m_pActor->EnableAddColorEffect(strActorNodeName.c_str(), vec3(XCONST::HIT_EFFECT_BPARTS_ADD_COLOR.x, XCONST::HIT_EFFECT_BPARTS_ADD_COLOR.y, XCONST::HIT_EFFECT_BPARTS_ADD_COLOR.z));
	}
	
}

void XModuleEntity::SetFaceBlinkAnimationActive( bool bActive )
{
	m_pAnimationController->SetFaceBlinkAnimationActive(bActive);
}

void XModuleEntity::SerializeBPartNPC( XNPCInfo* pNPCInfo, std::wstring strMeshNodeName, std::wstring strActorNodeName, bool bLoadingAsync /*= true*/ )
{
	if (global.smgr == NULL ||
		pNPCInfo == NULL) 
		return;

	XResourceLoadingPolicy policy = LOADING_ASYNC;
	if(bLoadingAsync == false)
		policy = LOADING_SYNC;

	m_pSerializer->SerializeBPartNPC(pNPCInfo, strMeshNodeName, strActorNodeName, policy);
}
