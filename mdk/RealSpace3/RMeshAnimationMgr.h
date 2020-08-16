/**
@brief RMeshAnimationMgr
elu �޽��� �ִϸ��̼��� �����ϴ� Ŭ���� �Դϴ�
RAnimation , RAnimationManager �� ���ҽ�/�����ڸ� �����մϴ�
*/

#pragma once

#include "RAnimation.h"
#include "RAnimationBlendTable.h"
#include "RResourceManager.h"

namespace rs3 {

typedef RHashList<RAnimation*>					RAnimationHashList;
typedef RHashList<RAnimation*>::iterator		RAnimationHashList_Iter;

class RS_API RMeshAnimationMgr : public RResource::LoadingController {
public:
	RMeshAnimationMgr();
	~RMeshAnimationMgr(); 

	RAnimation* Add(const char* name, const char* filename, bool bBackground = false, bool bLoadMonitoring = false, RResource::LoadingController* pLoadingController = NULL);

	void Delete(const char* name);
	void DelAll(); 

	RAnimation* GetAnimation(const char* name);
	RAnimation* GetAnimation(int nID);

	/// Ư�� �� �ε�� �ڵ����� �Ҹ��� ���ϸ��̼� ���� ��� �����ϱ�
	bool AddXmlDesc(const char* xmlFileName, bool bBackground = false, bool bLoadMonitoring = false, bool bExtern = false, const char* szFilter = "");
	bool SaveToXml(const char* szFileName);

	RAnimationBlendTable& GetAnimationBlendTable() { return m_AnimationBlendTable; }
	BLENDDATA GetAnimationBlendType( const char* source, const char* target );
	BLENDDATA GetAnimationBlendType( int sourceId, int targetId );

	bool IsLoadingCompleted();

	bool SaveAnimation(const char* szFileName);
	bool SaveAnimationEvent(const char* szFileName);

	bool LoadAnimationInformationsToAnimationMgr(const string& EluFileName,bool bExistAutoAnimation,bool bBackgournd);
public:
	RAnimationHashList		m_NodeHashList;
	RAnimationBlendTable	m_AnimationBlendTable;

private:
	int	m_nIDCount;

	virtual void	OnLoadingComplete(RResource*);
};

}