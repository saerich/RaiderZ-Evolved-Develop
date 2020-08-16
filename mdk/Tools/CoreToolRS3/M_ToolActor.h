#pragma once

#include "RActor.h"
//class RToolMeshMgr;

namespace rs3 {

class CORETOOL_API RToolActor : public RActor
{
	struct SPointLightSelector
	{
		static bool Selector( RSceneNode* pSceneNode )
		{
			if ( _stricmp( pSceneNode->GetNodeIDString(), RSID_LIGHTSCENENODE ) == 0 )
				return true;
			else
				return false;
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����Ʈ ����� Selector
	// by pok. �� ����������.. ���� ����� ���⵵ �ϰ�..-_- ���ټ� ������ ���ִ°� ������ �����ϴ�.
	// 09.02.09
	//
	struct EffectSelector
	{
		static bool Selector( RSceneNode* pSceneNode )
		{
			if ( _stricmp(pSceneNode->GetNodeIDString(), RSID_EFFECT ) == 0 )
				return true;
			return false;
		}
	};


public:
	RToolActor(CControlMediator* pControlMediator);
	virtual ~RToolActor();

	int GetPartsNum();
	void GetPartsMatrix(int nIndex, RMatrix *pMat);
	void DrawParts(int nIndex);
	void SetCreationType(bool bEluInstance);

	void SetTexColor(DWORD _dwColor);

	virtual RSceneNode* Clone(); 	// ���� ����
	virtual void CollectStaticCollisionPolygons(RSceneNodePolygonCollector* pCollector) const;

	void	SetCullState( bool _bSet = false );

public:
	bool			m_bEluInstanceType;

protected:
	// ����̽� �������� �κ� �����ÿ� ������ �ʿ��� ���� - ������ �ִ� ����Ʈ����Ʈ�� �̸��̶���� - �� �߰��Ѵ�.
	virtual void CreateDeviceDependentPart(RMesh* pMesh);

	// ����Ʈ ����Ʈ�� �̸��� ����
	void SetPointLightName(const char* _pSzDelim);

	virtual void	OnAdd();
	virtual void	OnRemove();

	std::vector< RSceneNode* >	m_vecEffectSceneNode;
	CControlMediator*	m_pControlMediator;
};

}
