#include "stdafx.h"
#include "MPxRagDollTC.h"
#include "../MPxObjRagDoll.h"

#include "MPhysX.h"

#include "RTypes.h"
#include "RSceneNode.h"

#include "RPhysXActorController.h"
#include "RPhysXRagDollBuilder.h"

#include "RActor.h"
#include "RCameraSceneNode.h"
#include "RDebugPassRenderer.h"

namespace physx
{

using namespace rs3;

MPxRagDollTC::MPxRagDollTC( MPxTcEntity* pRActorWrapper )
: MPxTcImpl(pRActorWrapper)
{
	m_pPxObjectRagDoll = NULL;
	m_pRagDollFactor = new RPhysXRagDollFactor;
	m_RagDollEntity.RegisterRagDollFactor(m_pRagDollFactor);
}

MPxRagDollTC::~MPxRagDollTC()
{
	// ���� / ���� ����ѷ� �����
	DeleteRagDoll();

	// ���� ���� �����
	m_RagDollEntity.DestroyRagDollBuilder();
	delete m_pRagDollFactor;
}

void MPxRagDollTC::Update( float fDelta )
{
	m_RagDollEntity.UpdateRagDollTransform(GetActor());
}

void MPxRagDollTC::InitRagDollInfoFromModelName( const std::string& strModelName, const char* _pSzCollisionGroup )
{
	m_strRagDollInfoName = strModelName + ".phx.xml";
	if (_pSzCollisionGroup)
		m_strCollisionGroup = _pSzCollisionGroup;

	if (MPhysX::GetInstance())
	{
		// �ߺ������� ���ο��� �˻����ش�.
		MPhysX::GetInstance()->CreatePxInfoXML(m_strRagDollInfoName, false);
	}
}

bool MPxRagDollTC::IsRagDollAvailable()
{
	return m_RagDollEntity.IsRagDollAvailable();
}

void MPxRagDollTC::StartRagDoll(const char* pSzRagdollContextName)
{
	CreateRagDoll(pSzRagdollContextName);
	m_bEnabled = true;
}

void MPxRagDollTC::EndRagDoll()
{
	DeleteRagDoll();
	m_bEnabled = false;
}

void MPxRagDollTC::CreateRagDoll(const char* pSzRagdollContextName)
{
	if (NULL == MPhysX::GetInstance())
		return;

	//////////////////////////////////////////////////////////////////////////
	//
	// Factor, Information -> Builder -> Entity -> MPxObjRagDoll ������ ����
	//
	//////////////////////////////////////////////////////////////////////////

	// RagDoll Factor�� XML�� �����ص� Context�κ��� �ε��ϱ�
	MXmlElement* pInfoElement = NULL;
	MXmlElement* pRagDollContextElement = MPhysX::GetInstance()->GetPxInfoXML<MPxObjRagDoll>(m_strRagDollInfoName, pSzRagdollContextName);
	if (pRagDollContextElement)
		m_pRagDollFactor->LoadRagDollFactorFromContext(pRagDollContextElement);

	// ���� ���� ����. ���� ���� ������ �̹� �����Ǿ� �־��ٸ� �˾Ƽ� �ߺ������� �����Ѵ�.
	m_RagDollEntity.CreateRagDollBuilder();

	// ������ ������������ �ִ� ���� �����. �Ϲ������δ� EndRagDoll���� ��������.
	DeleteRagDoll();

	// RagDoll Entity ����
	m_RagDollEntity.CreateRagDollEntity(GetActor());

	// MPhysX ���� ������Ʈ ��������, ������� �ִٸ� �ٽ� ������ �ʴ´�.
	if (NULL == m_pPxObjectRagDoll)
	{
		m_pPxObjectRagDoll = MPhysX::GetInstance()->CreatePxObject<physx::MPxObjRagDoll>();
		m_pPxObjectRagDoll->RegisterRagDollEntity(&m_RagDollEntity);

		if (m_strCollisionGroup.empty() == false)
			m_pPxObjectRagDoll->AddToCollisionGroup(m_strCollisionGroup.c_str());
	}
}

void MPxRagDollTC::DeleteRagDoll()
{
	m_RagDollEntity.DestroyRagDollEntity(GetActor());

	if (NULL != m_pPxObjectRagDoll)
	{
		m_pPxObjectRagDoll->UnRegisterRagDollEntity();
		MPhysX::GetInstance()->RemovePxObject(m_pPxObjectRagDoll);
		m_pPxObjectRagDoll = NULL;
	}
}

void MPxRagDollTC::ChangeRagDollFromContext( const char* pSzRagdollContextName )
{
	MXmlElement* pRagDollContextElement = MPhysX::GetInstance()->GetPxInfoXML<MPxObjRagDoll>(m_strRagDollInfoName, pSzRagdollContextName);
	if (pRagDollContextElement)
	{
		m_pRagDollFactor->LoadRagDollFactorFromContext(pRagDollContextElement);
		m_RagDollEntity.ChangeRagDollFromFactor();
	}
}

//////////////////////////////////////////////////////////////////////////
//
// RagDoll Entity
//
//////////////////////////////////////////////////////////////////////////

MPxRagDollTC::MPxTcRagDollEntity::MPxTcRagDollEntity()
: m_matCustomAttacherOffsetTM(MMatrix::IDENTITY)
{
	m_pPhysXActorController = NULL;
	m_pPhysXRagDollBuilder = NULL;
	m_pRagDollFactor = NULL;

	m_bUsingCustomTMOffset = true;
	m_matCustomAttacherOffsetTM._43 = 116.f;

	//RMatrix matRot;
	//matRot.SetRotationZ(180);
	//m_matCustomAttacherOffsetTM = matRot * m_matCustomAttacherOffsetTM;
}

bool MPxRagDollTC::MPxTcRagDollEntity::IsActive()
{
	if (m_pPhysXActorController && m_pPhysXActorController->IsNxObjectUpdatable() )
	{
		return true;
	}
	return false;
}

const MBox& MPxRagDollTC::MPxTcRagDollEntity::GetAreaBox()
{
	_ASSERT( m_pPhysXActorController != NULL );
	return m_pPhysXActorController->GetControlledActorAABB();
}

void MPxRagDollTC::MPxTcRagDollEntity::OnSimulated()
{
	if (m_pPhysXActorController && m_pPhysXActorController->IsEnable())
	{
		m_pPhysXActorController->Update(NULL);
	}
}

void MPxRagDollTC::MPxTcRagDollEntity::BuildNxObject(RActor* pActor)
{
	_ASSERT(m_pPhysXActorController != NULL);
	_ASSERT(m_pPhysXRagDollBuilder != NULL);

	pActor->AddController(m_pPhysXActorController);
	if( m_pPhysXActorController->BuildNxObject(m_pPhysXRagDollBuilder) )
	{
		ChangeRagDollFromFactor();
		m_pPhysXActorController->SetEnableKinematicMode( RSkeleton::GetBipName(RBIPID_SPINE1) );
	}
	else
	{
		pActor->RemoveController(m_pPhysXActorController);
		m_pPhysXActorController->ReleaseNxObject();
	}
}

void MPxRagDollTC::MPxTcRagDollEntity::CreateRagDollEntity( RActor* pActor )
{
	_ASSERT(NULL == m_pPhysXActorController);

	if (pActor)
	{
		m_pPhysXActorController = new RPhysXActorController( MPhysX::GetInstance()->GetNxScene(), RSNCTRL_CUSTOMUPDATE );
		BuildNxObject(pActor);
	}
}

void MPxRagDollTC::MPxTcRagDollEntity::DestroyRagDollEntity( RActor* pActor )
{
	if (NULL != m_pPhysXActorController)
	{
		if (pActor)
		{
			pActor->RemoveController(m_pPhysXActorController);
		}

		m_pPhysXActorController->ReleaseNxObject();
		delete m_pPhysXActorController;
		m_pPhysXActorController = NULL;
	}
}

bool MPxRagDollTC::MPxTcRagDollEntity::IsRagDollAvailable()
{
	if (m_pPhysXActorController && m_pPhysXActorController->IsNxObjectUpdatable() )
		return true;

	return false;
}

void MPxRagDollTC::MPxTcRagDollEntity::UpdateRagDollTransform( RActor* pActor )
{
	if (m_pPhysXActorController && pActor)
	{
		_ASSERT(MPhysX::GetInstance());

		if (m_pPhysXActorController->IsNxObjectUpdatable())
		{
			m_pPhysXActorController->SetKinematicModeTransform(pActor->GetWorldTransform(), false);

			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBoxAndAxis( pActor->GetWorldTransform(), 10, 1000.f);
			//RMatrix mattt = pActor->GetWorldTransform();
			//mlog("11 %f, %f, %f\n", mattt._41, mattt._42, mattt._43);
		}
		else
		{
			// �����ض�� �ؼ� ���� �����Ǵ°��� �ƴϴ�.. �x.
			// �׷��� ������Ʈ�� �������� ������ ��������ش�.
			BuildNxObject(pActor);
		}
	}
}

void MPxRagDollTC::MPxTcRagDollEntity::RegisterRagDollFactor( rs3::RPhysXRagDollFactor* pRagDollFactor )
{
	m_pRagDollFactor = pRagDollFactor;
}

void MPxRagDollTC::MPxTcRagDollEntity::ChangeRagDollFromFactor()
{
	m_pRagDollFactor->ChangeRagDollControllerFromFactor(m_pPhysXActorController);
}

bool MPxRagDollTC::MPxTcRagDollEntity::IsNeedCreateRagDollBuilder()
{
	return (NULL == m_pPhysXRagDollBuilder);
}

void MPxRagDollTC::MPxTcRagDollEntity::CreateRagDollBuilder()
{
	// �̹� �ִٸ� ���� ����� ���� �ʴ´�.
	if (NULL == m_pPhysXRagDollBuilder)
		m_pPhysXRagDollBuilder = new rs3::RPhysXRagDollBuilder;
}

void MPxRagDollTC::MPxTcRagDollEntity::DestroyRagDollBuilder()
{
	SAFE_DELETE(m_pPhysXRagDollBuilder);
}

void MPxRagDollTC::MPxTcRagDollEntity::OnRefreshCollisionGroup( int nCollGroup )
{
	if (m_pPhysXActorController)
	{
		m_pPhysXActorController->SetResouceNxActorCollisionGroup(nCollGroup);
	}
}

const MMatrix* MPxRagDollTC::MPxTcRagDollEntity::GetAttacherOffsetTM()
{
	if (m_bUsingCustomTMOffset)
	{
		return &m_matCustomAttacherOffsetTM;
	}

	if (m_pPhysXActorController)
	{
		return &(m_pPhysXActorController->GetKinematicLocalTM());
	}

	return NULL;
}

void MPxRagDollTC::MPxTcRagDollEntity::SetAttacherOffsetTM( bool bUsingCustomOffset, const MMatrix& _rTM )
{
	m_bUsingCustomTMOffset = bUsingCustomOffset;
	m_matCustomAttacherOffsetTM = _rTM;
}

}