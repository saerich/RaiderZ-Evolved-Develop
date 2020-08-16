#include "stdafx.h"
#include "TTalentEventViewer.h"
#include "TTalentEventMgr.h"
#include "Xmlseral.h"

//////////////////////////////////////////////////////////////////////////
TTalentEventViewer::TTalentEventViewer()
{
	m_pSelectedTalentInfo = NULL;
	m_pEventMgr								= NULL;

	
	// �ŷ�Ʈ �̺�Ʈ �Լ� ������
	TalentEventFuncToProperty[0]			= &TTalentEventViewer::SetTalentEventInfoToProperty;
	TalentEventFuncToTalentEventInfo[0]		= &TTalentEventViewer::SetPropertyToTalentEventInfo;

	TalentEventFuncToProperty[1]			= &TTalentEventViewer::SetTalentEventFireToProperty;
	TalentEventFuncToTalentEventInfo[1]		= &TTalentEventViewer::SetPropertyToTalentEventFire;

	TalentEventFuncToProperty[2]			= &TTalentEventViewer::SetTalentEventProjectileToProperty;
	TalentEventFuncToTalentEventInfo[2]		= &TTalentEventViewer::SetPropertyToTalentEventProjectile;

	TalentEventFuncToProperty[3]			= &TTalentEventViewer::SetTalentEventEffectToProperty;
	TalentEventFuncToTalentEventInfo[3]		= &TTalentEventViewer::SetPropertyToTalentEventEffect;

	TalentEventFuncToProperty[4]			= &TTalentEventViewer::SetTalentEventSoundToProperty;
	TalentEventFuncToTalentEventInfo[4]		= &TTalentEventViewer::SetPropertyToTalentEventSound;

	TalentEventFuncToProperty[5]			= &TTalentEventViewer::SetTalentEventCameraToProperty;
	TalentEventFuncToTalentEventInfo[5]		= &TTalentEventViewer::SetPropertyToTalentEventCamera;

	TalentEventFuncToProperty[6]			= &TTalentEventViewer::SetTalentEventDamageToProperty;
	TalentEventFuncToTalentEventInfo[6]		= &TTalentEventViewer::SetPropertyToTalentEventDamage;

	TalentEventFuncToProperty[7]			= &TTalentEventViewer::SetTalentEventCustomToProperty;
	TalentEventFuncToTalentEventInfo[7]		= &TTalentEventViewer::SetPropertyToTalentEventCustom;

	TalentEventFuncToProperty[8]			= &TTalentEventViewer::SetTalentEventDelayedActToProperty;
	TalentEventFuncToTalentEventInfo[8]		= &TTalentEventViewer::SetPropertyToTalentEventDelayedAct;

	TalentEventFuncToProperty[9]			= &TTalentEventViewer::SetTalentEventExtraActive_1ToProperty;
	TalentEventFuncToTalentEventInfo[9]		= &TTalentEventViewer::SetPropertyToTalentEventExtraActive_1;

	TalentEventFuncToProperty[10]			= &TTalentEventViewer::SetTalentEventExtraActive_2ToProperty;
	TalentEventFuncToTalentEventInfo[10]	= &TTalentEventViewer::SetPropertyToTalentEventExtraActive_2;

	TalentEventFuncToProperty[11]			= &TTalentEventViewer::SetTalentEventCameraLockToProperty;
	TalentEventFuncToTalentEventInfo[11]	= &TTalentEventViewer::SetPropertyToTalentEventCameraLock;

	TalentEventFuncToProperty[12]			= &TTalentEventViewer::SetTalentEventGroundFireToProperty;
	TalentEventFuncToTalentEventInfo[12]	= &TTalentEventViewer::SetPropertyToTalentEventGroundFire;
}

PropertyGrid^ TTalentEventViewer::GetEventViewerPropertGrid()
{
	return GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventAtt;
}

TreeView^ TTalentEventViewer::GetEventViewerTreeNode()
{
	return GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventTreeView;
}

string TTalentEventViewer::GetTextEventType( TALENT_EVENT nEventType )
{
	if(nEventType == TE_FIRE_EFFECT)
	{
		return "�߻�";
	}
	else if(nEventType == TE_LAUNCH_PROJECTILE)
	{
		return "�߻�ü";
	}
	else if(nEventType == TE_EFFECT)
	{
		return "����Ʈ";
	}
	else if(nEventType == TE_SOUND)
	{
		return "����";
	}
	else if(nEventType == TE_CAMERA)
	{
		return "ī�޶�";
	}
	else if(nEventType == TE_DAMAGE)
	{
		return "������";
	}
	else if(nEventType == TE_CUSTOM)
	{
		return "sh_custom";
	}
	else if(nEventType == TE_DELAYED_ACT)
	{
		return "�ð���";
	}
	else if(nEventType == TE_EXTRAACTIVE_1)
	{
		return "extra_active_1";
	}
	else if(nEventType == TE_EXTRAACTIVE_2)
	{
		return "extra_active_2";
	}
	else if(nEventType == TE_CAMEAR_LOCK)
	{
		return "ī�޶� ��";
	}
	else if(nEventType == TE_GROUND_FIRE_EFFECT)
	{
		return "���� �߻�";
	}

	// ��.
	return "����";
}

TALENT_EVENT TTalentEventViewer::GetDataEventType( char* pchrBuffer )
{
	TALENT_EVENT eEventType = TE_NONE;

	if(!stricmp(pchrBuffer, "�߻�"))
	{
		eEventType = TE_FIRE_EFFECT;	
	}
	else if(!stricmp(pchrBuffer, "�߻�ü"))
	{
		eEventType = TE_LAUNCH_PROJECTILE;	
	}
	else if(!stricmp(pchrBuffer, "����Ʈ"))
	{
		eEventType = TE_EFFECT;
	}
	else if(!stricmp(pchrBuffer, "����"))
	{
		eEventType = TE_SOUND;
	}	
	else if(!stricmp(pchrBuffer, "ī�޶�"))
	{
		eEventType = TE_CAMERA;
	}
	else if(!stricmp(pchrBuffer, "������"))
	{
		eEventType = TE_DAMAGE;
	}
	else if(!stricmp(pchrBuffer, "sh_custom"))
	{
		eEventType = TE_CUSTOM;
	}
	else if(!stricmp(pchrBuffer, "�ð���"))
	{
		eEventType = TE_DELAYED_ACT;
	}
	else if(!stricmp(pchrBuffer, "extra_active_1"))
	{
		eEventType = TE_EXTRAACTIVE_1;
	}
	else if(!stricmp(pchrBuffer, "extra_active_2"))
	{
		eEventType = TE_EXTRAACTIVE_2;
	}
	else if(!stricmp(pchrBuffer, "ī�޶� ��"))
	{
		eEventType = TE_CAMEAR_LOCK;
	}
	else if(!stricmp(pchrBuffer, "���� �߻�"))
	{
		eEventType = TE_GROUND_FIRE_EFFECT;
	}

	return eEventType;
}

string TTalentEventViewer::GetTextProjectileType( TALENT_PROJECTILE_TYPE nProjectileType )
{
	if(nProjectileType == TPT_MISSILE)
	{
		return TALENT_XML_VALUE_PROJECTILETYPE_MISSILE;
	}
	else if(nProjectileType == TPT_MISSILE_GUIDED)
	{
		return TALENT_XML_VALUE_PROJECTILETYPE_GUIDED;
	}
	else if(nProjectileType == TPT_HITCAPSULE_GUIDED)
	{
		return TALENT_XML_VALUE_PROJECTILETYPE_HITCAPSULE_GUIDED;
	}

	return "none";
}

TALENT_PROJECTILE_TYPE TTalentEventViewer::GetDataProjectileType( string strBuffer )
{
	TALENT_PROJECTILE_TYPE nProjectileType = TPT_NONE;

	if(strBuffer == TALENT_XML_VALUE_PROJECTILETYPE_MISSILE)
	{
		nProjectileType = TPT_MISSILE;	
	}
	else if(strBuffer == TALENT_XML_VALUE_PROJECTILETYPE_GUIDED)
	{
		nProjectileType = TPT_MISSILE_GUIDED;	
	}
	else if(strBuffer == TALENT_XML_VALUE_PROJECTILETYPE_HITCAPSULE_GUIDED)
	{
		nProjectileType = TPT_HITCAPSULE_GUIDED;
	}

	return nProjectileType;
}

string TTalentEventViewer::GetTextProjectileVisualType( TALENT_PROJECTILE_VISUAL_TYPE nProjeceilVisualType )
{
	if(nProjeceilVisualType == TPVT_STRAIGHT)
	{
		return TALENT_XML_VALUE_PROJECTILE_VISUAL_TYPE_STRAIGHT;
	}
	else if(nProjeceilVisualType == TPVT_CURVE)
	{
		return TALENT_XML_VALUE_PROJECTILE_VISUAL_TYPE_CURVE;
	}
	else if(nProjeceilVisualType == TPVT_PARABOLA)
	{
		return TALENT_XML_VALUE_PROJECTILE_VISUAL_TYPE_PARABOLA;
	}

	return "none";
}

TALENT_PROJECTILE_VISUAL_TYPE TTalentEventViewer::GetDataProjectileVisualType( string strBuffer )
{
	TALENT_PROJECTILE_VISUAL_TYPE nProjectileVisualType = TPVT_CURVE;

	if(strBuffer == TALENT_XML_VALUE_PROJECTILE_VISUAL_TYPE_STRAIGHT)
	{
		nProjectileVisualType = TPVT_STRAIGHT;
	}
	else if(strBuffer == TALENT_XML_VALUE_PROJECTILE_VISUAL_TYPE_CURVE)
	{
		nProjectileVisualType = TPVT_CURVE;
	}
	else if(strBuffer == TALENT_XML_VALUE_PROJECTILE_VISUAL_TYPE_PARABOLA)
	{
		nProjectileVisualType = TPVT_PARABOLA;
	}

	return nProjectileVisualType;
}

CSTalentEventInfo* TTalentEventViewer::GetCurrentTalentEventInfo()
{
	if (m_pSelectedTalentInfo == NULL) return NULL;

	TreeNode^ tn = GetEventViewerTreeNode()->SelectedNode;
	// ��尡 root���... ���� �� �� ����.
	if(tn == nullptr ||
		tn->Text == TALENT_EVENT_NORMAL ||
		tn->Text == TALENT_EVENT_ACT)
	{
		return NULL;
	}

	
	CSTalentEventInfo * pTalentEventInfo = NULL;
	CSTalentInfo * pTalentInfo = m_pSelectedTalentInfo;

	// ���۷�������...?
	if(m_pSelectedTalentInfo->m_Events.m_nRefID > 0)
	{
		pTalentInfo = TTalentHelper::GetTalentInfo(m_pSelectedTalentInfo->m_Events.m_nRefID, m_pSelectedTalentInfo->m_Events.m_nRefMode);
		if(pTalentInfo == NULL)
			pTalentInfo = m_pSelectedTalentInfo;
	}

	if(tn->Parent->Text == TALENT_EVENT_NORMAL)
	{
		pTalentEventInfo = m_pEventMgr->GetTalentEventInfo(pTalentInfo, tn->Index, TES_NORMAL);
	}
	else if(tn->Parent->Text == TALENT_EVENT_ACT)
	{
		pTalentEventInfo = m_pEventMgr->GetTalentEventInfo(pTalentInfo, tn->Index, TES_ACT);
	}

	return pTalentEventInfo;
}

CSTalentInfo* TTalentEventViewer::GetCurrentTalentInfo()
{
	return m_pSelectedTalentInfo;
}

TreeNode^ TTalentEventViewer::AddTalentEventView(int nStart)
{
	// ����
	char chIndex[64]		= {0, };
	String^ strEventStart;
	TreeNode^ tn			= nullptr;
	if(nStart == 0)
	{
		strEventStart = gcnew String(TALENT_EVENT_NORMAL);
		tn = g_pMainApp->SearchNode(GetEventViewerTreeNode()->Nodes, strEventStart);

		if(tn == nullptr)
		{
			// ������ ����.
			return nullptr;
		}

		sprintf(chIndex, "Event_%d", ++(m_pEventMgr->m_nMaxNormalIndex));

		tn->Nodes->Add(gcnew String(chIndex));
	}
	else if(nStart == 1)
	{
		strEventStart = gcnew String(TALENT_EVENT_ACT);
		tn = g_pMainApp->SearchNode(GetEventViewerTreeNode()->Nodes, strEventStart);

		if(tn == nullptr)
		{
			// ������ ����.
			return nullptr;
		}

		sprintf(chIndex, "ActEvent_%d", ++(m_pEventMgr->m_nMaxActIndex));

		tn->Nodes->Add(gcnew String(chIndex));
	}

	return g_pMainApp->SearchNode(tn->Nodes, gcnew String(chIndex));
}

bool TTalentEventViewer::DeleteTalentEvent()
{
	if (m_pSelectedTalentInfo == NULL) return false;

	TreeNode^ tn = GetEventViewerTreeNode()->SelectedNode;
	// ��尡 root���... ���� �� �� ����.
	if(tn == nullptr ||
		tn->Text == TALENT_EVENT_NORMAL ||
		tn->Text == TALENT_EVENT_ACT)
	{
		return false;
	}

	// �θ� ��� ���� Ȯ��
	TALENT_EVENT_STARTTYPE nStartType;
	if(tn->Parent->Text == TALENT_EVENT_NORMAL)
		nStartType = TES_NORMAL;
	else if(tn->Parent->Text == TALENT_EVENT_ACT)
		nStartType = TES_ACT;

	// ������ ����
	if(m_pEventMgr)
	{
		m_pEventMgr->DeleteTalentEventInfo(m_pSelectedTalentInfo, tn->Index, nStartType);
	}

	GetEventViewerTreeNode()->Nodes->Remove( tn );
	GetEventViewerTreeNode()->SelectedNode = nullptr;
	GetEventViewerPropertGrid()->SelectedObject = nullptr;

	return true;
}

void TTalentEventViewer::SetCurrentPropertyTalentEvent(bool bNewData)
{
	// ���� ���õ� ��忡�� �����͸� ã�´�.
	CSTalentEventInfo * pEventInfo	= GetCurrentTalentEventInfo();
	if(pEventInfo)
	{
		CSTalentInfo * pTalentInfo		= GetCurrentTalentInfo();

		// ������Ƽ���� ����
		// 1. ���ο� ������ �Է�
		// 2. ����� ������ �Է�

		int nEventTypeIndex = 0;
		XmlSerializable::TALENTEVENT^ xevent	= (XmlSerializable::TALENTEVENT^)(GetEventViewerPropertGrid()->SelectedObject);

		// 1 ���ο� ������ �Է�
		if(bNewData ||
			xevent == nullptr)
		{
			// �̺�Ʈ �������� Ÿ���� �����´�.
			nEventTypeIndex = (int)pEventInfo->m_nEvent;
			(this->*TalentEventFuncToProperty[nEventTypeIndex])(pEventInfo, pTalentInfo);
		}
		// 2 ����� ������ �Է�
		else
		{
			NotifyChanging();

			// ������Ƽ���� Ÿ����  �����´�.
			nEventTypeIndex = GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));

			// �ŷ�Ʈ �������� Ÿ���� �����´�.
			int nEventTypeFromInfo		= (int)pEventInfo->m_nEvent;

			// Ÿ���� �ٸ��� ��
			if(nEventTypeFromInfo != nEventTypeIndex)
			{
				// �ٸ���
				// �켱 �ٲ� Ÿ������ ���� ���� �Ѵ�.
				pEventInfo->m_nEvent	= (TALENT_EVENT)nEventTypeIndex;
				(this->*TalentEventFuncToProperty[nEventTypeIndex])(pEventInfo, pTalentInfo);
			}

			// �̺�Ʈ ���� �ʱ�ȭ
			pEventInfo->m_nEvent	= TE_NONE;
			pEventInfo->m_fTime1	= -1;
			pEventInfo->m_fTime2	= -1;
			pEventInfo->m_nParam1	= 0;
			pEventInfo->m_nParam2	= 0;
			pEventInfo->m_strParam1.clear();
			pEventInfo->m_strParam2.clear();
			pEventInfo->m_bFollow	= false;

			(this->*TalentEventFuncToTalentEventInfo[nEventTypeIndex])(pEventInfo, pTalentInfo);		
		}

		ChangeEventTreeNodeName(GetTextEventType(pEventInfo->m_nEvent));
	}
	// ������... nullptr
	else
	{
		GetEventViewerPropertGrid()->SelectedObject = nullptr;
	}
}

// �Ϲ�
bool TTalentEventViewer::SetTalentEventInfoToProperty(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_NONE^ xevent = gcnew XmlSerializable::TALENTEVENT_NONE;

	xevent->�̺�ƮŸ��		= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());
	xevent->���۽ð�		= pTalentEventInfo->m_fTime1;
	xevent->���ڿ�1			= gcnew String(pTalentEventInfo->m_strParam1.c_str());
	xevent->���ڿ�2			= gcnew String(pTalentEventInfo->m_strParam2.c_str());

	xevent->����1			= pTalentEventInfo->m_nParam1;
	xevent->����2			= pTalentEventInfo->m_nParam2;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventInfo(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_NONE^ xevent = (XmlSerializable::TALENTEVENT_NONE^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->���ڿ�1));
	pTalentEventInfo->m_strParam2	= string(MStringToCharPointer(xevent->���ڿ�2));

	pTalentEventInfo->m_nParam1		= xevent->����1;
	pTalentEventInfo->m_nParam2		= xevent->����2;

	return true;
}

// �߻�ü
bool TTalentEventViewer::SetTalentEventProjectileToProperty(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	if(pTalentEventInfo == NULL)
	{
		GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventAtt->SelectedObject = nullptr;
		return false;
	}

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_PROJECTILE^ xevent = gcnew XmlSerializable::TALENTEVENT_PROJECTILE;

	xevent->�̺�ƮŸ��		= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());
	xevent->�߻�ð�		= pTalentEventInfo->m_fTime1;
	xevent->�߻�üŸ��		= gcnew String(GetTextProjectileType(pTalentEventInfo->m_ProjectileInfo.m_nType).c_str());
	xevent->�����Ÿ��		= gcnew String(GetTextProjectileVisualType(pTalentEventInfo->m_ProjectileInfo.m_nVisualType).c_str());

	xevent->���ǵ�			= pTalentEventInfo->m_ProjectileInfo.m_fSpeed;
	xevent->����ũ��		= pTalentEventInfo->m_ProjectileInfo.m_fHitRadius;
	xevent->��Ÿ�			= pTalentEventInfo->m_ProjectileInfo.m_fRange / 100.0f;		// ��Ÿ��� �����̴�.

	xevent->x				= pTalentEventInfo->m_ProjectileInfo.m_vLocalStartPos.x;
	xevent->y				= pTalentEventInfo->m_ProjectileInfo.m_vLocalStartPos.y;
	xevent->z				= pTalentEventInfo->m_ProjectileInfo.m_vLocalStartPos.z;

	xevent->���̸�			= gcnew String(pTalentEventInfo->m_ProjectileInfo.m_strBoneName.c_str());

	xevent->ĸ���ε���		= pTalentEventInfo->m_ProjectileInfo.m_nCapsuleIndexForHitCapsuleType;
	xevent->�����ε���		= pTalentEventInfo->m_ProjectileInfo.m_nSegmentIndexForHitCapsuleType;

	xevent->�߻�ü			= gcnew String(pTalentEventInfo->m_ProjectileInfo.m_strEffectName.c_str());
	xevent->�ǰ�			= gcnew String(pTalentEventInfo->m_ProjectileInfo.m_strAttackEffectName.c_str());
	xevent->�����ǰ�		= gcnew String(pTalentEventInfo->m_ProjectileInfo.m_strHitGroundEffectName.c_str());
	xevent->���			= gcnew String(pTalentEventInfo->m_ProjectileInfo.m_strDefenceEffectName.c_str());
	xevent->�߻�			= gcnew String(pTalentEventInfo->m_ProjectileInfo.m_strFireEffectName.c_str());

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventProjectile(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	if(pTalentEventInfo == NULL)
	{
		GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventAtt->SelectedObject = nullptr;
		return false;
	}

	XmlSerializable::TALENTEVENT_PROJECTILE^ xevent = (XmlSerializable::TALENTEVENT_PROJECTILE^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->�߻�ð�;

	pTalentEventInfo->m_ProjectileInfo.m_nType			= GetDataProjectileType(MStringToCharPointer(xevent->�߻�üŸ��));
	pTalentEventInfo->m_ProjectileInfo.m_nVisualType	= GetDataProjectileVisualType(MStringToCharPointer(xevent->�����Ÿ��));

	pTalentEventInfo->m_ProjectileInfo.m_fSpeed			= xevent->���ǵ�;
	pTalentEventInfo->m_ProjectileInfo.m_fHitRadius		= xevent->����ũ��;
	pTalentEventInfo->m_ProjectileInfo.m_fRange			= xevent->��Ÿ� * 100.0f;		// ��Ÿ��� �����̴�.

	pTalentEventInfo->m_ProjectileInfo.m_vLocalStartPos.x	= xevent->x;
	pTalentEventInfo->m_ProjectileInfo.m_vLocalStartPos.y	= xevent->y;
	pTalentEventInfo->m_ProjectileInfo.m_vLocalStartPos.z	= xevent->z;

	pTalentEventInfo->m_ProjectileInfo.m_strBoneName		= string(MStringToCharPointer(xevent->���̸�));

	pTalentEventInfo->m_ProjectileInfo.m_nCapsuleIndexForHitCapsuleType	= xevent->ĸ���ε���;
	pTalentEventInfo->m_ProjectileInfo.m_nSegmentIndexForHitCapsuleType	= xevent->�����ε���;


	pTalentEventInfo->m_ProjectileInfo.m_strEffectName			= string(MStringToCharPointer(xevent->�߻�ü));
	pTalentEventInfo->m_ProjectileInfo.m_strAttackEffectName	= string(MStringToCharPointer(xevent->�ǰ�));
	pTalentEventInfo->m_ProjectileInfo.m_strHitGroundEffectName	= string(MStringToCharPointer(xevent->�����ǰ�));
	pTalentEventInfo->m_ProjectileInfo.m_strDefenceEffectName	= string(MStringToCharPointer(xevent->���));
	pTalentEventInfo->m_ProjectileInfo.m_strFireEffectName		= string(MStringToCharPointer(xevent->�߻�));

	return true;
}

// ����Ʈ
bool TTalentEventViewer::SetTalentEventEffectToProperty(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_EFFECT^ xevent = gcnew XmlSerializable::TALENTEVENT_EFFECT;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());
	xevent->���۽ð�			= pTalentEventInfo->m_fTime1;
	xevent->����Ʈ�̸�			= gcnew String(pTalentEventInfo->m_strParam1.c_str());
	xevent->���̸�				= gcnew String(pTalentEventInfo->m_strParam2.c_str());

	xevent->����ð�			= pTalentEventInfo->m_fTime2;
	xevent->�������			= pTalentEventInfo->m_nParam2;

	xevent->Follow				= pTalentEventInfo->m_bFollow;

	xevent->ĸ���ε���			= pTalentEventInfo->m_ProjectileInfo.m_nCapsuleIndexForHitCapsuleType;
	xevent->�����ε���			= pTalentEventInfo->m_ProjectileInfo.m_nSegmentIndexForHitCapsuleType;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventEffect(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_EFFECT^ xevent = (XmlSerializable::TALENTEVENT_EFFECT^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->����Ʈ�̸�));
	pTalentEventInfo->m_strParam2	= string(MStringToCharPointer(xevent->���̸�));

	pTalentEventInfo->m_fTime2		= xevent->����ð�;
	pTalentEventInfo->m_nParam2		= xevent->�������;

	pTalentEventInfo->m_bFollow		= xevent->Follow;

	pTalentEventInfo->m_ProjectileInfo.m_nCapsuleIndexForHitCapsuleType	= xevent->ĸ���ε���;
	pTalentEventInfo->m_ProjectileInfo.m_nSegmentIndexForHitCapsuleType	= xevent->�����ε���;

	return true;
}

// �߻�
bool TTalentEventViewer::SetTalentEventFireToProperty(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_FIRE_EFFECT^ xevent = gcnew XmlSerializable::TALENTEVENT_FIRE_EFFECT;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());
	xevent->���۽ð�			= pTalentEventInfo->m_fTime1;
	xevent->����Ʈ�̸�			= gcnew String(pTalentEventInfo->m_strParam1.c_str());
	xevent->���̸�				= gcnew String(pTalentEventInfo->m_strParam2.c_str());

	xevent->����ð�			= pTalentEventInfo->m_fTime2;
	xevent->�������			= pTalentEventInfo->m_nParam2;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventFire(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_FIRE_EFFECT^ xevent = (XmlSerializable::TALENTEVENT_FIRE_EFFECT^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->����Ʈ�̸�));
	pTalentEventInfo->m_strParam2	= string(MStringToCharPointer(xevent->���̸�));

	pTalentEventInfo->m_fTime2		= xevent->����ð�;
	pTalentEventInfo->m_nParam2		= xevent->�������;

	return true;
}

// ����
bool TTalentEventViewer::SetTalentEventSoundToProperty(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_SOUND^ xevent = gcnew XmlSerializable::TALENTEVENT_SOUND;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());	
	xevent->���۽ð�			= pTalentEventInfo->m_fTime1;
	xevent->�����̸�			= gcnew String(pTalentEventInfo->m_strParam1.c_str());
	xevent->���̸�				= gcnew String(pTalentEventInfo->m_strParam2.c_str());

	xevent->����ð�			= pTalentEventInfo->m_nParam1;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventSound(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_SOUND^ xevent = (XmlSerializable::TALENTEVENT_SOUND^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->�����̸�));
	pTalentEventInfo->m_strParam2	= string(MStringToCharPointer(xevent->���̸�));

	pTalentEventInfo->m_nParam1		= xevent->����ð�;
	pTalentEventInfo->m_nParam2		= 0;

	return true;
}

// ī�޶�
bool TTalentEventViewer::SetTalentEventCameraToProperty(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_CAMERA^ xevent = gcnew XmlSerializable::TALENTEVENT_CAMERA;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());	
	xevent->���۽ð�			= pTalentEventInfo->m_fTime1;
	xevent->����ð�			= pTalentEventInfo->m_fTime2;
	xevent->ī�޶�ȿ���̸�		= gcnew String(pTalentEventInfo->m_strParam1.c_str());

	xevent->�ݰ�				= pTalentEventInfo->m_nParam1;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventCamera(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_CAMERA^ xevent = (XmlSerializable::TALENTEVENT_CAMERA^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_fTime2		= xevent->����ð�;

	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->ī�޶�ȿ���̸�));
	pTalentEventInfo->m_strParam2	= string("");

	pTalentEventInfo->m_nParam1		= xevent->�ݰ�;

	if(pTalentEventInfo->m_fTime2 != -1 &&
		(pTalentEventInfo->m_fTime1 > pTalentEventInfo->m_fTime2))
	{
		// ���
		System::Windows::Forms::MessageBox::Show(L"����ð��� ���۽ð����� �۽��ϴ�.", "����");
	}
	return true;
}

// ������
bool TTalentEventViewer::SetTalentEventDamageToProperty(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_DAMAGE^ xevent = gcnew XmlSerializable::TALENTEVENT_DAMAGE;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());	
	xevent->�������ð�			= pTalentEventInfo->m_fTime1;
	xevent->����Ʈ				= gcnew String(pTalentEventInfo->m_strParam1.c_str());
	xevent->��					= gcnew String(pTalentEventInfo->m_strParam2.c_str());
	xevent->Rate				= pTalentEventInfo->m_nParam1;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventDamage(CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo)
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_DAMAGE^ xevent = (XmlSerializable::TALENTEVENT_DAMAGE^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->�������ð�;
	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->����Ʈ));
	pTalentEventInfo->m_strParam2	= string(MStringToCharPointer(xevent->��));
	pTalentEventInfo->m_nParam1		= xevent->Rate;

	return true;
}

// Custom
bool TTalentEventViewer::SetTalentEventCustomToProperty( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_CUSTOM^ xevent = gcnew XmlSerializable::TALENTEVENT_CUSTOM;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());	
	xevent->�̺�Ʈ�̸�			= gcnew String(pTalentEventInfo->m_strCustomEvent.c_str());	
	xevent->PARAM				= gcnew String(pTalentEventInfo->m_strParam1.c_str());
	xevent->��					= gcnew String(pTalentEventInfo->m_strParam2.c_str());
	xevent->���۽ð�			= pTalentEventInfo->m_fTime1;
	xevent->����ð�			= pTalentEventInfo->m_fTime2;

	xevent->ĸ���ε���			= pTalentEventInfo->m_ProjectileInfo.m_nCapsuleIndexForHitCapsuleType;
	xevent->�����ε���			= pTalentEventInfo->m_ProjectileInfo.m_nSegmentIndexForHitCapsuleType;

	xevent->ServerDir			= pTalentEventInfo->m_bServerDir;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventCustom( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_CUSTOM^ xevent = (XmlSerializable::TALENTEVENT_CUSTOM^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_strCustomEvent = string(MStringToCharPointer(xevent->�̺�Ʈ�̸�));
	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->PARAM));
	pTalentEventInfo->m_strParam2	= string(MStringToCharPointer(xevent->��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_fTime2		= xevent->����ð�;

	pTalentEventInfo->m_ProjectileInfo.m_nCapsuleIndexForHitCapsuleType	= xevent->ĸ���ε���;
	pTalentEventInfo->m_ProjectileInfo.m_nSegmentIndexForHitCapsuleType	= xevent->�����ε���;

	pTalentEventInfo->m_bServerDir	= xevent->ServerDir;

	return true;
}

void TTalentEventViewer::NotifyChanging()
{
	GlobalObjects::g_SaveNotifier.SetSaveTalentEvent(true);
}

void TTalentEventViewer::ChangeEventTreeNodeName( string strEventTypeName, TreeNode^ tn /*= nullptr*/ )
{
	if (m_pSelectedTalentInfo == NULL) return;

	if(tn == nullptr)
	{
		tn = GetEventViewerTreeNode()->SelectedNode;
		if(tn == nullptr ||
			tn->Parent == nullptr)
		{
			return;
		}
	}

	string strNodeName = MStringToCharPointer(tn->Text);
	string strChangeNodeName = strNodeName.substr(0, strNodeName.find("(") - 1) + " (" +strEventTypeName + ")";
	tn->Text = gcnew String(strChangeNodeName.c_str());
}

void TTalentEventViewer::GetTalentHitInfoSegmentIndex( ArrayList^ indexList )
{
	CSTalentInfo* pTalentInfo = g_pMainApp->GetTalentInfo();
	if(pTalentInfo)
	{
		for(int i = -1; i < (int)pTalentInfo->m_HitInfo.m_vSegments.size(); i++)
		{
			indexList->Add(i);
		}
	}
}

void TTalentEventViewer::GetTalentHitInfoCapsuleIndex( ArrayList^ indexList, TALENT_EVENT nEventType )
{
	CSTalentInfo* pTalentInfo = g_pMainApp->GetTalentInfo();
	if(pTalentInfo)
	{
		int nSegmentIndex = 0;

		if(nEventType == TE_LAUNCH_PROJECTILE)
		{
			XmlSerializable::TALENTEVENT_PROJECTILE^ xevent = (XmlSerializable::TALENTEVENT_PROJECTILE^)(GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventAtt->SelectedObject);
			if(xevent == nullptr) 
				return;
			nSegmentIndex = xevent->�����ε���;
		}
		else if(nEventType == TE_EFFECT)
		{
			XmlSerializable::TALENTEVENT_EFFECT^ xevent = (XmlSerializable::TALENTEVENT_EFFECT^)(GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventAtt->SelectedObject);
			if(xevent == nullptr) 
				return;
			nSegmentIndex = xevent->�����ε���;
		}
		else if(nEventType == TE_CUSTOM)
		{
			XmlSerializable::TALENTEVENT_CUSTOM^ xevent = (XmlSerializable::TALENTEVENT_CUSTOM^)(GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventAtt->SelectedObject);
			if(xevent == nullptr) 
				return;
			nSegmentIndex = xevent->�����ε���;
		}

		if(nSegmentIndex == -1)
			return;

		if((int)pTalentInfo->m_HitInfo.m_vSegments.size() > nSegmentIndex)
		{
			for(int i = -1; i < (int)pTalentInfo->m_HitInfo.m_vSegments[nSegmentIndex].m_vCapsules.size(); i++)
			{
				indexList->Add(i);
			}
		}
	}
}

void TTalentEventViewer::GetProjectileVisualType( ArrayList^ typeList )
{
	CSTalentInfo* pTalentInfo = g_pMainApp->GetTalentInfo();
	if(pTalentInfo)
	{
		XmlSerializable::TALENTEVENT_PROJECTILE^ xevent = (XmlSerializable::TALENTEVENT_PROJECTILE^)(GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventAtt->SelectedObject);
		if(xevent == nullptr) 
			return;

		TALENT_PROJECTILE_TYPE eCurrentProjectileType					= GetDataProjectileType(MStringToCharPointer(xevent->�߻�üŸ��));

		if(eCurrentProjectileType == TPT_MISSILE)
		{
			typeList->Add("straight");
		}
		else if(eCurrentProjectileType == TPT_MISSILE_GUIDED ||
			eCurrentProjectileType == TPT_HITCAPSULE_GUIDED)
		{
			typeList->Add("straight");
			typeList->Add("curve");
			typeList->Add("parabola");
		}
	}
}

bool TTalentEventViewer::SetTalentEventExtraActive_1ToProperty( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_EXTRA_ACTIVE^ xevent = gcnew XmlSerializable::TALENTEVENT_EXTRA_ACTIVE;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());
	xevent->���۽ð�			= pTalentEventInfo->m_fTime1;
	xevent->x					= pTalentEventInfo->m_vLocal.x;
	xevent->y					= pTalentEventInfo->m_vLocal.y;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventExtraActive_1( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_EXTRA_ACTIVE^ xevent = (XmlSerializable::TALENTEVENT_EXTRA_ACTIVE^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_vLocal.x	= xevent->x;
	pTalentEventInfo->m_vLocal.y	= xevent->y;

	return true;
}

bool TTalentEventViewer::SetTalentEventExtraActive_2ToProperty( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	return SetTalentEventExtraActive_1ToProperty(pTalentEventInfo, pTalentInfo);
}

bool TTalentEventViewer::SetPropertyToTalentEventExtraActive_2( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	return SetPropertyToTalentEventExtraActive_1(pTalentEventInfo, pTalentInfo);
}

bool TTalentEventViewer::SetTalentEventDelayedActToProperty( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	return SetTalentEventExtraActive_1ToProperty(pTalentEventInfo, pTalentInfo);
}

bool TTalentEventViewer::SetPropertyToTalentEventDelayedAct( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	return SetPropertyToTalentEventExtraActive_1(pTalentEventInfo, pTalentInfo);
}

void TTalentEventViewer::GetTalentHitInfoSegmentIndexAndCapsuleIndex( int& nSegIndex, int& nCapsuleIndex )
{
	if(GlobalObjects::g_pMainForm->m_EventTabPage->TalentEventAtt->SelectedObject == nullptr)
		return;

	XmlSerializable::TALENTEVENT^ xevent	= (XmlSerializable::TALENTEVENT^)(GetEventViewerPropertGrid()->SelectedObject);
	int nEventType = GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
			
	nSegIndex = -1;
	nCapsuleIndex =  -1;


	if(nEventType == TE_LAUNCH_PROJECTILE)
	{
		XmlSerializable::TALENTEVENT_PROJECTILE^ xevent = (XmlSerializable::TALENTEVENT_PROJECTILE^)(GetEventViewerPropertGrid()->SelectedObject);
		if(xevent == nullptr) 
			return;
		nSegIndex = xevent->�����ε���;
		nCapsuleIndex = xevent->ĸ���ε���;
	}
	else if(nEventType == TE_EFFECT)
	{
		XmlSerializable::TALENTEVENT_EFFECT^ xevent = (XmlSerializable::TALENTEVENT_EFFECT^)(GetEventViewerPropertGrid()->SelectedObject);
		if(xevent == nullptr) 
			return;
		nSegIndex = xevent->�����ε���;
		nCapsuleIndex = xevent->ĸ���ε���;
	}
	else if(nEventType == TE_CUSTOM)
	{
		XmlSerializable::TALENTEVENT_CUSTOM^ xevent = (XmlSerializable::TALENTEVENT_CUSTOM^)(GetEventViewerPropertGrid()->SelectedObject);
		if(xevent == nullptr) 
			return;
		nSegIndex = xevent->�����ε���;
		nCapsuleIndex = xevent->ĸ���ε���;
	}
}

bool TTalentEventViewer::SetTalentEventCameraLockToProperty( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_CAMERA_LOCK^ xevent = gcnew XmlSerializable::TALENTEVENT_CAMERA_LOCK;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());	
	xevent->���۽ð�			= pTalentEventInfo->m_fTime1;
	xevent->���ӽð�			= pTalentEventInfo->m_fTime2;

	xevent->�ݰ�				= pTalentEventInfo->m_nParam1;

	xevent->��					= gcnew String(pTalentEventInfo->m_strParam1.c_str());

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventCameraLock( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_CAMERA_LOCK^ xevent = (XmlSerializable::TALENTEVENT_CAMERA_LOCK^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_fTime2		= xevent->���ӽð�;

	pTalentEventInfo->m_nParam1		= xevent->�ݰ�;

	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->��));

	return true;
}

bool TTalentEventViewer::SetTalentEventGroundFireToProperty( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	// �ŷ�Ʈ �̺�Ʈ ������ ������Ƽ���� ����

	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEVENT_GROUND_FIRE_EFFECT^ xevent = gcnew XmlSerializable::TALENTEVENT_GROUND_FIRE_EFFECT;

	xevent->�̺�ƮŸ��			= gcnew String(GetTextEventType(pTalentEventInfo->m_nEvent).c_str());
	xevent->���۽ð�			= pTalentEventInfo->m_fTime1;
	xevent->����Ʈ�̸�			= gcnew String(pTalentEventInfo->m_strParam1.c_str());

	xevent->����ð�			= pTalentEventInfo->m_fTime2;

	//����Ÿ ���ε� ?
	GetEventViewerPropertGrid()->SelectedObject = xevent;

	return true;
}

bool TTalentEventViewer::SetPropertyToTalentEventGroundFire( CSTalentEventInfo * pTalentEventInfo, CSTalentInfo * pTalentInfo )
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����

	XmlSerializable::TALENTEVENT_GROUND_FIRE_EFFECT^ xevent = (XmlSerializable::TALENTEVENT_GROUND_FIRE_EFFECT^)(GetEventViewerPropertGrid()->SelectedObject);

	pTalentEventInfo->m_nEvent		= GetDataEventType((char*)MStringToCharPointer(xevent->�̺�ƮŸ��));
	pTalentEventInfo->m_fTime1		= xevent->���۽ð�;
	pTalentEventInfo->m_strParam1	= string(MStringToCharPointer(xevent->����Ʈ�̸�));

	pTalentEventInfo->m_fTime2		= xevent->����ð�;

	return true;
}
