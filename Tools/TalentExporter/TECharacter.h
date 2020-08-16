#pragma once

#include "RActor.h"
#include "RActorController.h"
#include "CSItemData.h"
#include "XCharacter.h"
#include "TETransformControllerManager.h"
#include "TEBaseAnimationTC.h"

using namespace rs3;

/// SoulHunt �� ĳ���� �޽�, RActor�� ����Ŭ����
// item������ ������� �������� ĳ������ ��Ģ�� ���⿡ ��� ���ǵȴ�.
// Character_Net ������ ���� ��� �ϹǷ� *** ����*** �ƹ��ų� include ���� ����

class TECharacter : public XCharacter
{
	MDeclareRTTI;
private:
	RActorNode*			m_pQuiverNode;
	RActorController*	m_pSpineController;

	TETransformControllerManager* m_pTC;
	TEBaseAnimationTC *	m_pBaseAnimationTC;

protected:
	void AddQuiver();			// ȭ����
	void RemoveQuiver();

public:
	TECharacter(void);
	~TECharacter(void);

	virtual bool Create(const TCHAR* szMeshName, bool bBackgroundCreation = false);
	virtual bool Create(RMesh* pMesh, bool bBackgroundCreation);
	virtual void Destroy();

	//////////////////////////////////////////////////////////////////////////
	// �ִϸ��̼�
	void SetBaseAnimationTC(TEBaseAnimationTC* pTC) { m_pBaseAnimationTC = pTC; }
	TETransformControllerManager* GetAnimationTC() { return m_pTC; }
	void UpdateAnimationTC(float fDela);
	void ForceUpdateBaseAnimationTC();

	bool SetAnimation(tstring& strAniName, bool bForce=false, bool bSameFrame = false, bool bTestPlay = false);
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

	bool IsAnimationLoop();
	bool IsPlayDone();
	bool IsOncePlayDone();

	RPLAYSTATE GetAnimationState();

	void ClearAnimationEvent();
	void AddAnimationEvent(ANIMATION_EVENT& ani_event);
	void ResetAnimationEventByFrame(int nFrame);

	// �ִϸ��̼� üũ
	bool CheckExistNodeController(string& strNodeName);

	// �ִϸ��̼� ���ø�
	void GetSamplingActorNodeData(tstring strNodeName, unsigned int nStartFrame, unsigned int nEndFrame, int nSampleInterval, std::vector< RMatrix >& _rOUt);
	RActorNode *  GetAnimatedActorNodeTransform(unsigned int nFrame, tstring& strNodeName, RMatrix& _rOut, E_TRANSFORMSPACE eTransformSpace);
};

