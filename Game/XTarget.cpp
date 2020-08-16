#include "stdafx.h"
#include "XTarget.h"
#include "XNonPlayer.h"
#include "XEffectManager.h"
#include "XGame.h"
#include "XWorld.h"
#include "XCameraManager.h"
#include "XCharacter.h"

#include "RUtil.h"
#include "XHeadIcon.h"
#include "XModuleUI.h"

//#include "RDebugPassRenderer.h"

// XTargetRender ////////////////////////////////////////////////////
MUID XTargetRenderer::SetIndicatorEffect( XObject* pObject )
{
	MUID uid = MUID::ZERO;
	if (pObject == NULL) return uid;

	if(pObject->GetActor()->CheckLoadingCompletedFastly() == false)
	{
	}
	else
	{
		XModuleUI* pModuleUI = pObject->GetModuleUI();
		if (pModuleUI)
		{
			XHeadIconPlacer* pHeadIconPlacer = pModuleUI->GetHeadIconPlacer();
			if (pHeadIconPlacer)
			{
				RMatrix matLocal;
				matLocal.MakeZero();
				if(pHeadIconPlacer->MakeIndicatorLocalMatrix(matLocal, pObject))
				{
					RActorNode* pActorNode = pHeadIconPlacer->FindTitleBone(pObject);
					uid = global.emgr->AddXEffect(ICON_EFFECT_MARKER, pObject->GetActor(), pActorNode, matLocal, false, true);
				}
				else
				{
					uid = global.emgr->AddXEffect(ICON_EFFECT_MARKER, pObject->GetActor(), NULL, matLocal);
				}
			}
		}
	}

	return uid;
}

MUID XTargetRenderer::SetTargetBoneEffect(XObject* pObject, RActorNode * pPartsTargetNode, vec3& pos)
{
	// ����Ʈ ����
	vec3 tagetpos	= pPartsTargetNode->GetPosition();
	vec3 lockdir	= pPartsTargetNode->GetPosition() - pos;
	lockdir.Normalize();

	XCharacter* pActor	= pObject->GetActor();
	if(pActor == NULL)
	{
		return MUID::ZERO;
	}

	vec3 pObjpos	= vec3::ZERO;
	RMatrix matLocal;
	matLocal.SetLocalMatrix(pObjpos, lockdir, vec3(0, 0, 1));
	MUID uid = global.emgr->AddXEffect(TARGET_EFFECT_FILE_NAME, pActor, pPartsTargetNode, matLocal, true);
	return uid;
}

void XTargetRenderer::DelTargetEffect( MUID uidEffect )
{
	global.emgr->DeleteXEffect(uidEffect);
}

MUID XTargetRenderer::SetTargetHitPartsEffect( XObject* pObject, vec3& centerPos, vec3& pos )
{
	vec3 targetpos	= pObject->GetPosition();
	vec3 lockdir	= centerPos - pos;
	lockdir.Normalize();

	XCharacter* pActor	= pObject->GetActor();
	if(pActor == NULL)
	{
		return MUID::ZERO;
	}

	vec3 pObjpos;
	{
		RMatrix matInverse;
		pActor->GetWorldTransform().GetInverse(&matInverse);
		matInverse.TransformVect(centerPos, pObjpos);
	}

	RMatrix matLocal;
	matLocal.SetLocalMatrix(pObjpos, lockdir, vec3(0, 0, 1));
	MUID uid		= global.emgr->AddXEffect(TARGET_EFFECT_FILE_NAME, pActor, NULL, matLocal, true);
	return uid;
}

// XTarget //////////////////////////////////////////////////////////
XTarget::XTarget( XObject* pObject )
: XMouseBase(pObject)
{
	m_fTargetRange	= 0.0f;
	m_nOneTargetEn	= 0;

	m_bTargetUI = false;
}

XTarget::~XTarget()
{

}

void XTarget::InitTarget( bool bTargetUI )
{
	DelTargetAll();

	m_bTargetUI = bTargetUI;

	if(bTargetUI)
	{
		StartTargetUI();
	}
}

void XTarget::ExitTarget()
{
	DelTargetAll();
	EndTargetUI();
}

void XTarget::GetCameraInfo( vec3& vPos, vec3& vDir )
{
	XBaseCameraSceneNode* pCameraNode = global.camera->GetCamera();
	if(pCameraNode)
	{
		// ī�޶� ��ġ ����
		if(m_bTargetUI)
		{
			vec2 vUIPos = global.ui->GetTargetUIPos();
			GetTrans3DCoordinates(vUIPos.x, vUIPos.y, vPos, vDir);
		}
		else
		{
			vDir = pCameraNode->GetDirection();
			vPos = pCameraNode->GetPosition();
		}
	}
	else 
	{
		// ī�޶� ������... �׽�Ʈ������ ���� ���Ѱ�... (UnitTest)
		vDir = m_pOwner->GetDirection();
		vPos = m_pOwner->GetPosition();
		vPos.z += 100.0f;
	}
}

bool XTarget::CheckTargetAllowed( XObject* pObject )
{
	PFC_THISFUNC;

	// Ÿ���� NPC�� PLAYER�� �ƴ϶�� Ÿ�ٿ��� �����Ѵ�.
	if(pObject->GetEntityType() != ETID_NPC && pObject->GetEntityType() != ETID_PLAYER)
	{
		return false;
	}

	//------------------------------------------------------------------------
	switch(m_nCurrentTargetAllowed)
	{
	case CSEffectInfo::RELATION_NONE:
		break;
	case CSEffectInfo::RELATION_ALL:
		return true;
	case CSEffectInfo::RELATION_ALLIED:
		{
			if(CheckMe(pObject) ||								// ���� ����
				IsActorDead(pObject))							// ������ ����
			{
				return false;
			}

			// �켱 NPC�� ����
			if(pObject->GetEntityType() == ETID_PLAYER)
			{
				return true;
			}
		}
		break;
	case CSEffectInfo::RELATION_ALLIED_DEAD:
		{
			// ���� �Ʊ����Ը�...
			// ���� ����
			if(CheckMe(pObject))
			{
				return false;
			}

			// �켱 NPC�� ����
			if(pObject->GetEntityType() == ETID_PLAYER &&
				IsActorDead(pObject))							// �׾� �ִ°�?
			{
				return true;
			}
		}
		break;
	case CSEffectInfo::RELATION_ENEMY:
		{
			// ���� ����
			if(CheckMe(pObject) ||								// ���� ����
				IsActorDead(pObject))							// ������ ����
			{
				return false;
			}

			// �������� �켱 NPC�� ��� ���´�.
			if(pObject->GetEntityType() == ETID_NPC &&
				CheckNPC(pObject) == false)						// NPC��� ���߿����� ���Ϳ� NPC�� �����Ѵ�.
			{
				return true;
			}
			// PVP���̳�
			else if(pObject->GetEntityType() == ETID_PLAYER)
			{
				XPlayer* pPlayer = static_cast<XPlayer*>(pObject);

				if (CheckEqualFaction(pPlayer) == false)
				{
					return true;
				}
			}
		}
		break;
	case CSEffectInfo::RELATION_PARTY:
		{
			// ��Ƽ����... 
			if(gvar.MyInfo.Party.Find(pObject->GetUID()))
			{
				return true;
			}
		}
		break;
	}

	return false;
}

bool XTarget::CheckMe( XObject* pObject )
{
	if(pObject->GetUID() == m_pOwner->GetUID() ||
		pObject == m_pOwner) 
	{
		return true;
	}
	return false;
}

bool XTarget::CheckNPC( XObject* pObject )
{
	// NPC��� ���߿����� ���Ϳ� NPC�� �����Ѵ�.
	if(pObject->GetEntityType() == ETID_NPC)
	{	
		XNonPlayer * pNPC = pObject->AsNPC();
		if(	pNPC->IsAttackable() == false)						// ������ NPC�� �ƴϴ�. �������̸� ���ͷ� ����
		{
			// NPC�̴�.
			return true;
		}
	}

	// �����̴�.
	return false;
}

bool XTarget::CheckExistTargetObjOnScreen( XObject* pObject )
{
	// ȭ�鿡�� �������� ���� �ʰ� �ִٸ�...
	if(pObject->GetActor())
	{
		return pObject->GetActor()->IsPrimaryCamRenerable();
	}

	return false;
}

bool XTarget::CheckColTerrain( vec3& vStart, vec3& vTarget, float& fOutDistance, vec3* vOutTargetPos /*= NULL*/ )
{
	PFC_THISFUNC;

	vec3 vOutPos;
	vec3 vOutNor;
	if(gg.currentgamestate->GetWorld()->PickCollision(vStart, vTarget, &vOutPos, &vOutNor, CollisionAttribute::MESH | CollisionAttribute::TERRAIN))
	{
		vec3 vCheckDir = (vTarget - vStart).Normalize();
		if(vCheckDir.DotProduct(vOutNor) > 0.0f)
			return false;

		fOutDistance = (vStart - vOutPos).Length();
		if(vOutTargetPos)
		{
			*vOutTargetPos = vOutPos;
		}

		return true;
	}

	return false;
}

void XTarget::SetTargetInfo( float fRange, int nEn, CSEffectInfo::RELATION nAllowed /*= CSEffectInfo::RELATION::RELATION_ENEMY*/ )
{
	m_fTargetRange				= fRange;
	m_nOneTargetEn				= nEn;
	
	m_nCurrentTargetAllowed		= nAllowed;
}

bool XTarget::CheckTotalEN( int nTargetCount )
{
	// En�� ��, ��� �� �� �ִ� En�� �ִ°�?
	// m_pOwner�� My �÷��̾��ΰ�? My �÷��̾ �˻�
	if(m_pOwner->GetUID() == XGetMyUID())
	{
		int nTotalEn = nTargetCount * m_nOneTargetEn;
		if(gvar.MyInfo.ChrStatus.nEN < nTotalEn)
		{
			return false;
		}
	}

	return true;
}

void XTarget::StartTargetUI()
{
	if (global.ui)
		global.ui->TargetUIStart();
}

void XTarget::EndTargetUI()
{
	if (global.ui)
		global.ui->TargetUIEnd();
}

bool XTarget::CheckEqualFaction( XPlayer* pObject )
{
	if (gg.omgr == NULL) return true;
	XPlayer* pOwnerPlayer = gg.omgr->FindPlayer(m_pOwner->GetUID());
	if (pOwnerPlayer == NULL) return true;

	return (pOwnerPlayer->IsAlly(pObject));
}

bool XTarget::CheckTargetHitCapsule( XObject* pObject )
{
	// ��Ʈ ĸ���� ���ٸ� Ÿ���� ���� �ʴ´�.
	if(pObject && pObject->GetModuleCollision())
	{
		CSMeshInfo * pMeshInfo = pObject->GetModuleCollision()->GetMeshInfo();
		if(pMeshInfo && pMeshInfo->m_mapHitGroupInfo.size() > 0)
		{
			return true;
		}
	}

	return false;
}
