#pragma once

#include "RActor.h"
#include "RActorController.h"
#include "CSItemData.h"
#include "XCharacter.h"
#include "TTransformControllerManager.h"
#include "TBaseAnimationTC.h"
#include "TAnimationInfoMgr.h"

using namespace rs3;

/// SoulHunt �� ĳ���� �޽�, RActor�� ����Ŭ����
// item������ ������� �������� ĳ������ ��Ģ�� ���⿡ ��� ���ǵȴ�.
// Character_Net ������ ���� ��� �ϹǷ� *** ����*** �ƹ��ų� include ���� ����

struct TCharacterEquipment
{
	//string			strDefaultPartsName;	// �⺻ ����(�ٸ� �޽��� �� ����)
	RMeshNode*			pBaseMeshNode;	// �ƹ��͵� ������ �⺻���
	RActorNode*			pActorNode;		// ���� ������ ���
	RActorNode*			pActorNode_b;	// ���⿡ ���� �߰��� parent�� body �� ��� 
	RActorNode*			pActorNode_w;	// ���⿡ ���� �߰��� parent�� world �� ��� 
	const CSItemData*	pItemData;		// ���� ������ ������

	TCharacterEquipment()
	{
		Clear();
	}

	void Clear()
	{
		pBaseMeshNode = NULL;
		pActorNode = NULL;
		pActorNode_b = NULL;
		pActorNode_w = NULL;
		pItemData = NULL;
	}
};

class TCharacter : public XCharacter
{
	MDeclareRTTI;
private:
	TCharacterEquipment	m_equipments[ITEMSLOT_MAX];
	RActorNode*			m_pQuiverNode;
	RActorController*	m_pSpineController;

	TTransformControllerManager* m_pTC;
	TBaseAnimationTC *	m_pBaseAnimationTC;

	TAnimationInfoMgr			m_toolAnimationInfoMgr;

protected:
	void AddQuiver();			// ȭ����
	void RemoveQuiver();

public:
	TCharacter(void);
	~TCharacter(void);

	virtual bool Create(const char* szMeshName, bool bBackgroundCreation = false);
	virtual bool Create(RMesh* pMesh, bool bBackgroundCreation);
	virtual void Destroy();

	// Equip / UnEquip item
	// UnEquip�� SyncLoad��, Equip�� MeshLoading�� AsyncLoading����
	//bool EquipItem(const CSItemData& itemData, int nSlot);
	bool EquipItem(const CSItemData& itemData, SH_ITEM_SLOT nEquipedSlot);
	void EquipItem( RMesh* _pMesh, const CSItemData& itemData);
	void EquipItem( RMesh* _pMesh, const CSItemData& itemData, SH_ITEM_SLOT nEquipedSlot);

	void XCharacterSerializeStrategy_CreatePartsNodeAsync( SH_ITEM_SLOT nEquipedSlot, string &strItemMeshName, const CSItemData &itemData );
	void UnequipItem(int nSlot, bool bResetToDefault = true);
	void SetToDefaultPartsMesh(int nSlot);		// �⺻ �޽÷� ���� ����

	TCharacterEquipment* GetEquipItems() { return m_equipments;}

	void	FindDummyHook(int nGroupID, vector<RActorNode*> &DummyHookNode);
	vec3	GetDummyHookPos(int nGroupID, int nSlotID);

	string GetPartsMeshName(const string& strMeshPath, const string& strMeshName, bool bMeshNameBySex);
	string GetWeaponMeshName(const string& strMeshName);

	WEAPON_TYPE	GetCurrentWeaponType();
	CSItemData* GetItem(SH_ITEM_SLOT nSlot);

	XPlayerPartsSlotType Slot2LookSlot( SH_ITEM_SLOT nSlot );

	string	CurrentSexType()
	{
		return GetPureFileName(m_pMesh->GetName());
	}
	bool IsEquipShield();

	//////////////////////////////////////////////////////////////////////////
	// �ִϸ��̼�
	void SetBaseAnimationTC(TBaseAnimationTC* pTC) { m_pBaseAnimationTC = pTC; }
	TTransformControllerManager* GetAnimationTC() { return m_pTC; }
	void UpdateAnimationTC(float fDela);
	void ForceUpdateBaseAnimationTC();

	bool SetAnimation(string& strAniName, bool bForce=false, bool bSameFrame = false, bool bTestPlay = false);
	bool ReSetAnimation();
	RAnimation* GetCurAnimation();

	void SetAnimationSpeed(float fSpeed);
	float GetAnimationSpeed();
	unsigned int  GetAnimationFrame();
	void  SetAnimationFrame(unsigned int nFrame);
	unsigned int  GetAnimationMaxFrame();
	float GetMaxFrameToSecond();
	float GetCurFrameToSecond();


	void InitCurrentAnimation();
	void PauseAnimation();
	void StopAnimation();
	void PlayAnimation();
	void ResetAnimationControl();

	bool IsAnimationLoop();
	bool IsPlayDone();
	bool IsOncePlayDone();

	RPLAYSTATE GetAnimationState();

	void ClearAnimationEvent();
	void AddAnimationEvent(ANIMATION_EVENT& ani_event);
	void ResetAnimationEventByFrame(int nFrame);

	// ���̽� �ִϸ��̼�
	void CreateFaceAnimation(bool bNPC = false);
	void StartFaceAnimation(string& strAniName, bool bEye);
	void EndFaceAnimation(string& strAniName, float fEndBledTime);
	void StopFaceAnimation();

	// ��ü �ִϸ��̼�
	bool SetUpperAnimation(string strAniName);

	// ����ũ ��ư �׽�Ʈ
	void InitFakeBeatenAnimation(tstring& strAniName, float fWeight, float fSpeed);
	void StartFakeBeaten(const string& strCurAni, const float fWeightRate, const float fSpeedRate, const char* szAniPrefix=NULL);


	// �ִϸ��̼� üũ
	bool CheckExistNodeController(string& strNodeName);

	// �ִϸ��̼� ���ø�
	void GetSamplingActorNodeData(string strNodeName, unsigned int nStartFrame, unsigned int nEndFrame, int nSampleInterval, std::vector< RMatrix >& _rOUt);
	RActorNode *  GetAnimatedActorNodeTransform(unsigned int nFrame, string& strNodeName, RMatrix& _rOut, E_TRANSFORMSPACE eTransformSpace);
	RActorNode *  GetAnimatedActorNodeTransform2(unsigned int nFrame, string& strNodeName, RMatrix& _rOut, E_TRANSFORMSPACE eTransformSpace);

	virtual XOverlayAnimationInfo* GetOverlayAnimaionInfo();
	virtual BLEND_DATA GetAnimationBlendData(string strSource, string strTarget);
	virtual XAnimationHitSoundInfo* GetAnimationHitSoundInfo();
	virtual XReActionAnimationInfo* GetReActionAnimationInfo();

	TAnimationInfoMgr* GetAnimationInfoMgr() { return &m_toolAnimationInfoMgr; }

	void InitMeshNodeNoiseRef();
};

