#include "stdafx.h"
#include "MPxAniLookUpTC.h"

#include "MPxRagDollTC.h"
#include "MPxDynamicCollisionTC.h"

namespace physx
{

using namespace rs3;

MPxAniLookUpTC::MPxAniLookUpTC(MPxTcEntity* pTcEntity)
: MPxTcImpl(pTcEntity)
{
	m_bCollisionTcHooking = false;
}

bool MPxAniLookUpTC::HookOnAnimationSetting( string& strAniName )
{
	// ������ �ִϸ��̼��� �����̶�� ��ŷ�Ѵ�.
	MPxRagDollTC* pRagDollTransform = GetTransformController<MPxRagDollTC>();
	if (pRagDollTransform)
	{
		const char* pSzRagDollPrefix = strstr(strAniName.c_str(), "ragdoll_");
		if ( NULL != pSzRagDollPrefix )
		{
			// ���� ���¸� ��û�޾�����.
			const char* pSzRagDollContext = pSzRagDollPrefix + 8; //���ڿ� ragdoll_ �� ũ�Ⱑ 8�̴�
			if (pRagDollTransform->IsRagDollAvailable())
			{
				// �̹� ���������϶��� Context�κ��� ������ �����Ų��.
				pRagDollTransform->ChangeRagDollFromContext(pSzRagDollContext);
			}
			else
			{
				// ���� ���� Ʈ����
				pRagDollTransform->StartRagDoll(pSzRagDollContext);
			}
			return true;
		}
		else
		{
			// ���� ���¾ƴ� Ʈ����
			pRagDollTransform->EndRagDoll();

			// �ٸ� �ִϸ��̼� �̸� ��ŷ : col�� uncol
			if (m_bCollisionTcHooking)
			{
				const char* pSzCollisionPrefix = strstr(strAniName.c_str(), "uncol_");
				if(pSzCollisionPrefix)
				{
					strAniName = pSzCollisionPrefix + 6; //���ڿ� uncol_ �� 6����
					MPxDynamicCollisionTC* pCollisionTc = GetTransformController<MPxDynamicCollisionTC>();
					if (pCollisionTc)
					{
						pCollisionTc->DeActivateCollision();
					}
				}
				else if ( (pSzCollisionPrefix = strstr(strAniName.c_str(), "col_")) )
				{
					strAniName = pSzCollisionPrefix + 4; //���ڿ� col_ �� 4����
					MPxDynamicCollisionTC* pCollisionTc = GetTransformController<MPxDynamicCollisionTC>();
					if (pCollisionTc)
					{
						pCollisionTc->ActivateCollision();
					}
				}
			}
		}
	}

	return false;
}

}
