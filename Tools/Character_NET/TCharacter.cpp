#include "StdAfx.h"
//#include "XDef.h"

#include "TCharacter.h"
#include "TNaming.h"
#include "RMeshNode.h"
//#include "RBlendAnimationController.h"
#include "RSceneManager.h"
#include "RMeshMgr.h"
#include "CSItemHelper.h"

MImplementRTTI(TCharacter, RActor);

//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
// XCharacter
TCharacter::TCharacter(void)
: m_pSpineController(new XCharacterSpineController)
, m_pQuiverNode(NULL)
, m_pBaseAnimationTC(NULL)
{
	for(int i=0;i<ITEMSLOT_MAX;i++)
	{
		m_equipments[i].pActorNode = NULL;
		m_equipments[i].pActorNode_b = NULL;
		m_equipments[i].pActorNode_w = NULL;
		m_equipments[i].pBaseMeshNode = NULL;
		m_equipments[i].pItemData = NULL;
	}
	AddController(m_pSpineController);

	m_bAnimationUpdated = true;

	m_pTC = new TTransformControllerManager(this);
}

TCharacter::~TCharacter(void)
{
	Destroy();

	RemoveController(m_pSpineController);
	delete m_pSpineController;

	SAFE_DELETE(m_pTC);
}

void TCharacter::Destroy()
{
	for(int i=0;i<ITEMSLOT_MAX;i++)
	{
		if(m_equipments[i].pActorNode)
		{
			RMesh* pMesh = m_equipments[i].pActorNode->m_pMeshNode->m_pBaseMesh;
			DeleteActorNode(m_equipments[i].pActorNode->GetNodeName().c_str());
			REngine::GetSceneManager().ReleaseResource(pMesh);

			m_equipments[i].pActorNode = NULL;
		}
	}

	// Quiver �� ������ Quiver ����
	RemoveQuiver();

	//RActor::Destroy();
}

void TCharacter::RemoveQuiver()
{
	if(m_pQuiverNode)
	{
		RMesh* pMesh = m_pQuiverNode->m_pMeshNode->m_pBaseMesh;

		DeleteActorNode(m_pQuiverNode->GetNodeName().c_str());
		REngine::GetSceneManager().ReleaseResource(pMesh);

		m_pQuiverNode = NULL;
	}
}

void TCharacter::AddQuiver()
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

bool TCharacter::Create(RMesh* pMesh, bool bBackgroundCreation)
{
	if(RActor::Create(pMesh, bBackgroundCreation))
	{
		// �����忡 ������ �κ�
		m_equipments[ITEMSLOT_HEAD].pBaseMeshNode = m_pMesh->GetNode(TNaming::GetPartsNodeName(ITEMSLOT_HEAD));
		m_equipments[ITEMSLOT_HANDS].pBaseMeshNode = m_pMesh->GetNode(TNaming::GetPartsNodeName(ITEMSLOT_HANDS));
		m_equipments[ITEMSLOT_BODY].pBaseMeshNode = m_pMesh->GetNode(TNaming::GetPartsNodeName(ITEMSLOT_BODY));
		m_equipments[ITEMSLOT_LEG].pBaseMeshNode = m_pMesh->GetNode(TNaming::GetPartsNodeName(ITEMSLOT_LEG));
		m_equipments[ITEMSLOT_FEET].pBaseMeshNode = m_pMesh->GetNode(TNaming::GetPartsNodeName(ITEMSLOT_FEET));
		m_pSpineController->Create();
		return true;
	}
	return false;
}

bool TCharacter::Create( const char* szMeshName, bool bBackgroundCreation /*= false*/ )
{
	m_toolAnimationInfoMgr.LoadInfo(string(szMeshName));
	return  RActor::Create(szMeshName, bBackgroundCreation);
}

void TCharacter::EquipItem( RMesh* _pMesh, const CSItemData& itemData)
{
	SH_ITEM_SLOT nSlot = itemData.m_nItemSlot;

	EquipItem(_pMesh, itemData, nSlot);
}

void TCharacter::EquipItem( RMesh* _pMesh, const CSItemData& itemData, SH_ITEM_SLOT nEquipedSlot)
{
	//if (_pMesh == NULL) return;
	if (itemData.m_strNodeNames.empty()) return;

	// ���� �ִ� ������ ����
//	if(m_equipments[nEquipedSlot].pItemData)
//		UnequipItem(nEquipedSlot, false);

	// ���ο� ������ ����
	m_equipments[nEquipedSlot].pItemData = &itemData;	// by pok. EquipItem�� CSItemData�� �����߿� ��ȿȭ���� �ʴ´ٰ� ������ �ڵ�

	//RActorNode* pPartsActorNode = SetPartsMesh(_pMesh, itemData.GetMainNodeName().c_str(), itemData.m_strParentName.c_str());
	//m_equipments[nEquipedSlot].pActorNode = pPartsActorNode;

	// LWEAPON �̰ų� RWEAPON �̸� body , world ���ͳ�� �߰�
	if(	nEquipedSlot == ITEMSLOT_LWEAPON || nEquipedSlot == ITEMSLOT_RWEAPON)
	{
		// _b ���� ���� ���
		//_pMesh->AddRef();
		//RActorNode* pPartsActorNode_b = SetPartsMesh(_pMesh, itemData.GetMainNodeName().c_str(), 
		//	(itemData.m_strParentName+"_b").c_str(), (itemData.GetMainNodeName()+"_b").c_str());
		//m_equipments[nEquipedSlot].pActorNode_b = pPartsActorNode_b;
		
		// _w ���� ���
		//_pMesh->AddRef();
		//RActorNode* pPartsActorNode_w = SetPartsMesh(_pMesh, itemData.GetMainNodeName().c_str(), 
		//	(itemData.m_strParentName+"_w").c_str(), (itemData.GetMainNodeName()+"_w").c_str());
		//m_equipments[nEquipedSlot].pActorNode_w = pPartsActorNode_w;
	}

	// item quiver ����
	if(itemData.m_nWeaponType==WEAPON_ARCHERY)
		AddQuiver();

	//// ���� ����
	//for ( int i = 0; i < (int)itemData.m_vecMtrls.size(); ++i )
	//{
	//	for ( int j = 0; j < itemData.m_vecMtrls[i]->nSubCount; ++j )
	//	{
	//		// RActor �� ���ؼ� ȣ��Ǿ�� �Ѵ�
	//		RActor::ChangeMatetial(itemData.m_strNodeNames[1].c_str(), j, itemData.m_vecMtrls[i]->vecSubMtrls[j] );
	//	}
	//}
}
//
//bool TCharacter::EquipItem(const CSItemData& itemData, int nSlot)
//{
//	return EquipItem(itemData, nSlot);
//}

bool TCharacter::EquipItem(const CSItemData& itemData, SH_ITEM_SLOT nEquipedSlot)
{
	if ( nEquipedSlot < ITEMSLOT_HEAD || nEquipedSlot >= ITEMSLOT_MAX)
	{
		mlog("���� : %s�� ���� ���̵� �̻��մϴ�.", itemData.m_strName.c_str());
		return false;
	}

	if (itemData.m_strMeshName.empty())
	{
		mlog("���� : %s�� �޽��̸��� �����ϴ�.", itemData.m_strName.c_str());
		return false;
	}

	string strItemMeshName;

	//����� ���� ��� ����.
 	if( nEquipedSlot == ITEMSLOT_LWEAPON || nEquipedSlot == ITEMSLOT_RWEAPON)
	{
		strItemMeshName = GetWeaponMeshName(itemData.m_strMeshName);
	}
	else
	{
		strItemMeshName = GetPartsMeshName(itemData.m_strMeshPath, itemData.m_strMeshName, itemData.m_bMeshNameBySex);
	}

	XCharacterSerializeStrategy_CreatePartsNodeAsync(nEquipedSlot, strItemMeshName, itemData);

	EquipItem( NULL, itemData, nEquipedSlot);

	return true;
}

void TCharacter::XCharacterSerializeStrategy_CreatePartsNodeAsync( SH_ITEM_SLOT nEquipedSlot, string &strItemMeshName, const CSItemData &itemData )
{
	XPlayerPartsSlotType nLookSlot = Slot2LookSlot(nEquipedSlot);

	XPlayerPartsParam partsParam;
	partsParam.Make(nLookSlot, strItemMeshName.c_str(), (CSItemData*)&itemData);

	string strWeaponAttachNodeName = (string)itemData.m_strParentName;
	if(nEquipedSlot == ITEMSLOT_LWEAPON &&
		itemData.m_bLeftWeaponEquipable)
	{
		strWeaponAttachNodeName = (string)itemData.m_strParentNameLeft;
	}

	for (size_t i = 0; i < itemData.m_strNodeNames.size(); i++)
	{
		unsigned int nColor = 0xFFFFFFFF;

		if (TNaming::IsSkinNodeName(itemData.m_strNodeNames[i].c_str()))
		{
			nColor = 0xfffcd6b8;
		}
		else if (TNaming::IsHairNodeName(itemData.m_strNodeNames[i].c_str()))
		{
			nColor = 0xff290a0a;
		}
		else
		{
			nColor = itemData.m_nTexColor;
		}

		string strNodeName = (string)itemData.m_strNodeNames[i];
		if(nEquipedSlot == ITEMSLOT_LWEAPON &&
			itemData.m_bLeftWeaponEquipable)
		{
			strNodeName = (string)itemData.m_strNodeNames[i] + "_left";
		}

		partsParam.AddPart((string)itemData.m_strNodeNames[i], strWeaponAttachNodeName, strNodeName, nColor);

		// weapon left
		if (itemData.m_ItemType == ITEMTYPE_WEAPON)
		{
			partsParam.AddPart((string)itemData.m_strNodeNames[i], (strWeaponAttachNodeName+"_w"), (strNodeName+"_w"));
			partsParam.AddPart((string)itemData.m_strNodeNames[i], (strWeaponAttachNodeName+"_b"), (strNodeName+"_b"));
			partsParam.AddPart((string)itemData.m_strNodeNames[i], (strWeaponAttachNodeName+"_c"), (strNodeName+"_c"));
		}
	}

	if (itemData.m_ItemType == ITEMTYPE_WEAPON)
	{
		// swordtrail_start
		partsParam.AddPart(string("swordtrail_start"), strWeaponAttachNodeName, string(""));

		// swordtrail_end
		partsParam.AddPart(string("swordtrail_end"), strWeaponAttachNodeName, string(""));

		// effect
		for(int iEffect = 0; iEffect < WEAPON_EFFECT_DUMMY_MAX_COUNT_TOOL; ++iEffect)
		{
			char chEffectName[256] = {0,};
			sprintf_s(chEffectName, "%s%02d", WEAPON_EFFECT_DUMMY_NAME_TOOL, iEffect);
			partsParam.AddPart(string(chEffectName), strWeaponAttachNodeName, string(""));
		}
	}

	CreatePartsNodeAsync( &partsParam );

	RSceneNodeUpdateVisitor visitor;
	Traverse(&visitor);
	//UpdateRecursively();
}

XPlayerPartsSlotType TCharacter::Slot2LookSlot( SH_ITEM_SLOT nSlot )
{
	switch(nSlot)
	{
	case ITEMSLOT_HEAD	: return PLAYER_PARTS_SLOT_HEAD;
	case ITEMSLOT_FACE	: return PLAYER_PARTS_SLOT_FACE;	// NPC �� �� �������� �ε��ϴ� ��찡 �ִ�
	case ITEMSLOT_HANDS : return PLAYER_PARTS_SLOT_HANDS;
	case ITEMSLOT_FEET	: return PLAYER_PARTS_SLOT_FEET;
	case ITEMSLOT_BODY	: return PLAYER_PARTS_SLOT_BODY;
	case ITEMSLOT_LEG	: return PLAYER_PARTS_SLOT_LEG;
	case ITEMSLOT_LWEAPON : return PLAYER_PARTS_SLOT_LWEAPON;
	case ITEMSLOT_RWEAPON : return PLAYER_PARTS_SLOT_RWEAPON;
	}
	return PLAYER_PARTS_SLOT_MAX;
}

// bResetToDefault : ������������ (�ʵ�)�⺻�𵨷� ���ư��� ����
void TCharacter::UnequipItem(int nSlot, bool bResetToDefault )
{
	if(m_equipments[nSlot].pItemData)
	{
		if(m_equipments[nSlot].pItemData->m_nWeaponType==WEAPON_ARCHERY)
			RemoveQuiver();
	}

	if(m_equipments[nSlot].pItemData)
	{
		XPlayerPartsSlotType nLookSlot = Slot2LookSlot((SH_ITEM_SLOT)nSlot);
		DestroyPartsNode(nLookSlot);
	}

	if(m_equipments[nSlot].pActorNode_b)
	{
		RMesh* pMesh = m_equipments[nSlot].pActorNode_b->m_pMeshNode->m_pBaseMesh;

		DeleteActorNode(m_equipments[nSlot].pActorNode_b->GetNodeName().c_str());
		REngine::GetSceneManager().ReleaseResource(pMesh);

		m_equipments[nSlot].pActorNode_b = NULL;
	}
	if(m_equipments[nSlot].pActorNode_w)
	{
		RMesh* pMesh = m_equipments[nSlot].pActorNode_w->m_pMeshNode->m_pBaseMesh;

		DeleteActorNode(m_equipments[nSlot].pActorNode_w->GetNodeName().c_str());
		REngine::GetSceneManager().ReleaseResource(pMesh);

		m_equipments[nSlot].pActorNode_w = NULL;
	}

	// �⺻ �޽ð� �ִ°��� ����
	if(bResetToDefault && m_equipments[nSlot].pBaseMeshNode)
	{
		AddMeshNode(m_equipments[nSlot].pBaseMeshNode,NULL);
	}

	if(bResetToDefault)
	{
		SetToDefaultPartsMesh(nSlot);
	}

	m_equipments[nSlot].Clear();
}

void TCharacter::SetToDefaultPartsMesh(int nSlot)
{
	XPlayerPartsSlotType nLookSlot = Slot2LookSlot((SH_ITEM_SLOT)nSlot);

	vector<XPartsParam::Part> vecParts;

	string strPartsNodeName = TNaming::GetPartsNodeName(SH_ITEM_SLOT(nSlot));
	if (strPartsNodeName.size() > 0)
	{
		XPartsParam::Part part;
		part.strNodeName = strPartsNodeName;
		part.dwPartsColor = 0xfffcd6b8;
		vecParts.push_back(part);

		XPartsParam::Part part_item;
		part_item.strNodeName = strPartsNodeName + string(EQUIPMENT_ITEM_NODE_NAME_ADDITIVE_SUFFIX);
		vecParts.push_back(part_item);
	}
	else
	{
		return;
	}

	string strDefaultPartsName;// = GetPartsMeshName("hf", false);//     GetDefaultPartsName(m_Feature.nSex, SH_ITEM_SLOT(nSlot));

	string strRealMeshName;

	// default part mesh �̸��� �����Ѱ��
	if (!strDefaultPartsName.empty())
	{
		TNaming::GetPartsMeshName(strRealMeshName, "", m_pMesh->GetName(), strDefaultPartsName, true);
	}
	else
	{
		// �ƴ϶�� ���� ���� parts �� �����Ѵ�
		if (m_pMesh)
		{
			strRealMeshName = m_pMesh->GetName();
		}
	}

	XPlayerPartsParam partsParam;
	partsParam.MakeForDefault(nLookSlot, strRealMeshName.c_str(), vecParts);
	CreatePartsNodeAsync( &partsParam );

	RSceneNodeUpdateVisitor visitor;
	Traverse(&visitor);
	//UpdateRecursively();
}

// [10/30/2007 madduck]  �׷��� �ִ� ����� �������?
void TCharacter::FindDummyHook(int nGroupID, vector<RActorNode*> &DummyHookNode)
{
	char chNodeName[32] = {0, };
	for( int i = 0; i < 10/*GetActorNodeCount()*/; i++) //�ϴ� 10��
	{
		sprintf_s(chNodeName, TARGET_HOOK_DUMMY_NAME, nGroupID+1, i);
		RActorNode* pActorNode = GetActorNode(chNodeName);
		if( pActorNode )
		{
			DummyHookNode.push_back(pActorNode);
		}
	}
}

vec3 TCharacter::GetDummyHookPos(int nGroupID, int nSlotID)
{
	char chNodeName[32] = {0, };
	sprintf_s(chNodeName, TARGET_HOOK_DUMMY_NAME, nGroupID+1, nSlotID+1);
	RActorNode* pActorNode = GetActorNode(chNodeName);
	if( pActorNode )
	{
		return pActorNode->GetWorldPosition(); //locla ��ǥ
	}

	return vec3::ZERO;
}

string TCharacter::GetPartsMeshName( const string& strMeshPath, const string& strMeshName, bool bMeshNameBySex )
{
	std::string strItemMeshName;

	//�����۰� �����۹̸������� ���̴� ���...��.��
	int nPos = strMeshName.find_last_of("/\\");
	if(nPos == -1)
	{
		if( bMeshNameBySex )
		{
			string strModelName( m_pMesh->GetName() );
			string strPureModelName( MGetPureFileName(strModelName) );

			if (strMeshPath.empty())
			{
				string strModelPurePath = MGetPurePath(strModelName);

				if (strPureModelName.length() >= 2 && strPureModelName[1] == 'm')
				{
					// �ΰ� ���� �÷��̾� ���
					strItemMeshName = PATH_PLAYER_MALE_TOOL + string("hm_");
				}
				else
				{
					// �ΰ� ���� �÷��̾� ���
					strItemMeshName = PATH_PLAYER_FEMALE_TOOL + string("hf_");
				}
			}
			else
			{
				strItemMeshName = strMeshPath;
			}
		}
		else
		{
			string strModelName( m_pMesh->GetName() );
			string strModelPurePath = MGetPurePath(strModelName);

			if (strMeshPath.empty())
			{
				strItemMeshName = strModelPurePath + strMeshPath;
			}
			else
			{
				strItemMeshName = strMeshPath;
			}
		}
	}

	return strItemMeshName + strMeshName + ".elu";
}

string TCharacter::GetWeaponMeshName(const string& strMeshName)
{
	return "../../Data/Model/weapons/" + strMeshName + ".elu";
}

CSItemData* TCharacter::GetItem(SH_ITEM_SLOT nSlot)
{
	return (CSItemData*)m_equipments[nSlot].pItemData;
}

WEAPON_TYPE TCharacter::GetCurrentWeaponType()
{
	CSItemData* pItemDataR = GetItem(ITEMSLOT_RWEAPON);
	CSItemData* pItemDataL = GetItem(ITEMSLOT_LWEAPON);

	WEAPON_TYPE nRType = WEAPON_NONE;
	WEAPON_TYPE nLType = WEAPON_NONE;
	bool bEquipShield = false;

	if (pItemDataR) nRType = pItemDataR->m_nWeaponType;
	if (pItemDataL) 
	{
		nLType = pItemDataL->m_nWeaponType;
		if (ARMOR_SHIELD == pItemDataL->m_nArmorType)
		{
			bEquipShield = true;
		}
	}

	return CSItemHelper::GetCurrentWeaponType(nRType, nLType, bEquipShield);
}

bool TCharacter::IsEquipShield()
{
	CSItemData* pItemDataL = GetItem(ITEMSLOT_LWEAPON);
	if (pItemDataL)
	{
		if (pItemDataL->m_nItemSlot == ITEMSLOT_LWEAPON &&
			pItemDataL->m_ItemType == ITEMTYPE_ARMOR) 
		{
			return true;
		}
	}
	return false;
}

bool TCharacter::SetAnimation( string& strAniName, bool bForce/*=false*/, bool bSameFrame /*= false*/, bool bTestPlay /*= false*/ )
{
	return m_pTC->SetAnimation(strAniName, bForce, bSameFrame, bTestPlay);
}

void TCharacter::CreateFaceAnimation( bool bNPC /*= false*/ )
{
	m_pTC->SetFaceAnimationNPC(bNPC, this);
}

void TCharacter::UpdateAnimationTC( float fDela )
{
	m_pTC->Update(fDela);
}

void TCharacter::SetAnimationSpeed( float fSpeed )
{
	m_pTC->SetAnimationSpeed(fSpeed);
}

float TCharacter::GetAnimationSpeed()
{
	return m_pTC->GetAnimationSpeed();
}

void TCharacter::InitCurrentAnimation()
{
	if(m_pBaseAnimationTC == NULL)
		return;

	m_pBaseAnimationTC->InitCurrentAnimation();
}

void TCharacter::PauseAnimation()
{
	if(m_pTC)
		m_pTC->PauseAnimation();
}

void TCharacter::StopAnimation()
{
	if(m_pTC)
		m_pTC->StopAnimation();
}

void TCharacter::PlayAnimation()
{
	if(m_pTC)
		m_pTC->PlayAnimation();
}

void TCharacter::ResetAnimationEventByFrame( int nFrame )
{
	if(m_pBaseAnimationTC)
		m_pBaseAnimationTC->ResetAnimationEventByFrame(nFrame);
}

bool TCharacter::IsAnimationLoop()
{
	return m_pTC->IsAnimationLoop();
}

bool TCharacter::IsPlayDone()
{
	return m_pTC->IsPlayDone();
}

bool TCharacter::IsOncePlayDone()
{
	return m_pTC->IsOncePlayDone();
}

rs3::RPLAYSTATE TCharacter::GetAnimationState()
{
	if(m_pBaseAnimationTC)
		return m_pBaseAnimationTC->GetState();

	return RPS_STOP;
}

unsigned int TCharacter::GetAnimationFrame()
{
	return m_pTC->GetAniFrame();
}

void TCharacter::SetAnimationFrame( unsigned int nFrame )
{
	m_pTC->SetAnimationFrame(nFrame);
}

float TCharacter::GetMaxFrameToSecond()
{
	return m_pTC->GetMaxFrameToSecond();
}	

float TCharacter::GetCurFrameToSecond()
{
	return m_pTC->GetCurFrameToSecond();
}

unsigned int TCharacter::GetAnimationMaxFrame()
{
	if(m_pBaseAnimationTC)
		return m_pBaseAnimationTC->GetAniMaxFrame();

	return 0;
}

void TCharacter::ClearAnimationEvent()
{
	if(m_pBaseAnimationTC)
		m_pBaseAnimationTC->ClearAnimationEvent();
}

void TCharacter::AddAnimationEvent( ANIMATION_EVENT& ani_event )
{
	if(m_pBaseAnimationTC)
		m_pBaseAnimationTC->AddAnimationEvent(ani_event);
}

bool TCharacter::ReSetAnimation()
{
	return m_pTC->ReSetAnimation();
}

void TCharacter::ResetAnimationControl()
{
	if(m_pBaseAnimationTC &&
		m_pBaseAnimationTC->GetCurrentAnimationController())
		m_pBaseAnimationTC->GetCurrentAnimationController()->Destroy();
}

bool TCharacter::SetUpperAnimation( string strAniName )
{
	return m_pTC->SetUpperAnimation(strAniName);
}

bool TCharacter::CheckExistNodeController( string& strNodeName )
{
	if(m_pBaseAnimationTC)
		return m_pBaseAnimationTC->CheckExistNodeController(strNodeName);

	return false;
}

RAnimation* TCharacter::GetCurAnimation()
{
	if(m_pBaseAnimationTC)
		return m_pBaseAnimationTC->GetAnimation();

	return NULL;
}

void TCharacter::GetSamplingActorNodeData( string strNodeName, unsigned int nStartFrame, unsigned int nEndFrame, int nSampleInterval, std::vector< RMatrix >& _rOUt )
{
	// ���� ���̸� �ȵ˴ϴ�.
	RActorNode * pActorNode = GetActorNode(strNodeName.c_str());
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

		string strCalNodeName = strNodeName;
		if(pActorNode->GetVisibility() == 0.0f)
			strCalNodeName += "_c";

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

RActorNode *  TCharacter::GetAnimatedActorNodeTransform( unsigned int nFrame, string& strNodeName, RMatrix& _rOut, E_TRANSFORMSPACE eTransformSpace )
{
	// ���� ���̸� �ȵ˴ϴ�.
	RActorNode * pActorNode = GetActorNode(strNodeName.c_str());
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

	m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);

	m_pBaseAnimationTC->GetAnimatedActorNodeTransform(strNodeName, _rOut, eTransformSpace);

	// �������� �����·� ������.
	SetAnimationFrame(nCurrentFrame);
	m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);

	PlayAnimation();
	return pActorNode;
}

RActorNode *  TCharacter::GetAnimatedActorNodeTransform2(unsigned int nFrame, string& strNodeName, RMatrix& _rOut, E_TRANSFORMSPACE eTransformSpace)
{
	// ���� ���̸� �ȵ˴ϴ�.
	RActorNode * pActorNode = GetActorNode(strNodeName.c_str());
	if(pActorNode == NULL)
		return NULL;

	// ���� ������ ��������
	int nCurrentFrame = GetCurFrame();

	// ���� �������� �� �����Ӻ��� ũ�� Max ����������...
	if(nFrame > GetMaxFrame())
		nFrame = GetMaxFrame();

	// ���ø�
	RMatrix matResult;
	SetAnimationFrame(nFrame);

	m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);

	m_pBaseAnimationTC->GetAnimatedActorNodeTransform(strNodeName, _rOut, eTransformSpace);

	// �������� �����·� ������.
	SetAnimationFrame(nCurrentFrame);
	m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);

	return pActorNode;
}

BLEND_DATA TCharacter::GetAnimationBlendData( string strSource, string strTarget )
{
	return m_toolAnimationInfoMgr.m_toolAnimationBlendInfo.GetAnimationTransData(strSource, strTarget, &m_pMesh->m_AniMgr);
}

XOverlayAnimationInfo* TCharacter::GetOverlayAnimaionInfo()
{
	return &(m_toolAnimationInfoMgr.m_toolOverlayAnimationInfo);
}

XAnimationHitSoundInfo* TCharacter::GetAnimationHitSoundInfo()
{
	return &(m_toolAnimationInfoMgr.m_AnimationHitSoundInfo);
}

void TCharacter::ForceUpdateBaseAnimationTC()
{
	m_pBaseAnimationTC->GetCurrentAnimationController()->Update(NULL);
}

void TCharacter::StartFaceAnimation( string& strAniName, bool bEye )
{
	m_pTC->StartFaceAnimation(strAniName, bEye);
}

void TCharacter::EndFaceAnimation( string& strAniName, float fEndBledTime )
{
	m_pTC->EndFaceAnimation(strAniName, fEndBledTime);
}

void TCharacter::StopFaceAnimation()
{
	m_pTC->StopFaceAnimation();
}

void TCharacter::InitFakeBeatenAnimation( tstring& strAniName, float fWeight, float fSpeed )
{
	m_pTC->InitFakeBeatenAnimation(strAniName, fWeight, fSpeed);
}

void TCharacter::StartFakeBeaten( const string& strCurAni, const float fWeightRate, const float fSpeedRate, const char* szAniPrefix/*=NULL*/ )
{
	m_pTC->StartFakeBeaten(strCurAni.c_str(), fWeightRate, fSpeedRate, szAniPrefix);
}

XReActionAnimationInfo* TCharacter::GetReActionAnimationInfo()
{
	return &(m_toolAnimationInfoMgr.m_toolReActionAnimationInfo);
}

void TCharacter::InitMeshNodeNoiseRef()
{
	for(int i = 0; i < GetActorNodeCount();i++)
	{
		RActorNode *pActorNode = GetActorNodes()[i];

		if(pActorNode && pActorNode->m_pMeshNode->m_nMtrlCnt > 0)
			pActorNode->SetNoiseRef(1.0f);
	}

	SetNoiseRef(1.0f);
}
