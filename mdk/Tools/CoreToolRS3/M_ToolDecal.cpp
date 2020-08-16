#include "StdAfx.h"
#include "M_ToolDecal.h"

namespace rs3 {

RToolDecal::RToolDecal(void) :
m_pDecal_Polygon(NULL)
{
	m_vecDecal_Projection.clear();
}

RToolDecal::~RToolDecal(void)
{
	Destroy();
}

void RToolDecal::Destroy()
{
	for (std::vector<RDecalProjectionSceneNode*>::iterator itr = m_vecDecal_Projection.begin(); itr != m_vecDecal_Projection.end(); ++itr)
	{
		// ������ �䷸��
		(*itr)->RemoveFromParent();
		delete *itr;
	}
	m_vecDecal_Projection.clear();

	if(m_pDecal_Polygon)
		m_pDecal_Polygon->RemoveFromParent();

	SAFE_DELETE(m_pDecal_Polygon);
}

// http://dev/redmine/projects/engine/wiki/Decalmap
// DecalProjection(DECAL_TYPE_PROJECTION)	: ������ ���� �� ������ ������Ʈ�� �ʿ��� ��Į
// DecalPolygon(DECAL_TYPE_POLYGON)			: �Ѿ�, ���簰�� �ѹ� ������ �׸��⸸ �� ��Į
void RToolDecal::Create(RToolSceneManager* pToolSceneManager, CRollupEtcDecal* pRollupDecal, RVector MousePosVec, CRollupEtcCollision* pCollision)
{
	if( pCollision->GetCollisionTree().GetCollisionTree() == NULL )
		pCollision->GetCollisionTree().Load( pCollision->GetCollisionFileName().c_str() );

	if( pRollupDecal->m_eDecalType == DECAL_TYPE_PROJECTION )
	{	// Projection ��Į�� ��Į ��ü���� ���� �����ؼ� ����մϴ�. �ϳ��� ��Į�� �ϳ��� �����.
		RDecalProjectionSceneNode* pDecal = static_cast< RDecalProjectionSceneNode* >( pToolSceneManager->CreateSceneNode(RSID_DECAL_PROJECTION) );
		pDecal->Create( pCollision->GetCollisionTree().GetCollisionTree(), pRollupDecal->m_strTextureFileName.GetString(), MousePosVec, pRollupDecal->m_fDecalSize_X, pRollupDecal->m_fDecalSize_Y, pRollupDecal->m_fDecalSize_Z, pRollupDecal->m_fDecalRotate_Z );

		m_vecDecal_Projection.push_back(pDecal);
		pToolSceneManager->AddSceneNode(pDecal);
	}
	else if( pRollupDecal->m_eDecalType == DECAL_TYPE_POLYGON )
	{	// PolygonDecal �� ���α׷� ������ �ϳ��� ����带 ������, ����� ���ο��� �ؽ��ĺ��� �˾Ƽ� ��Į�� �������ش�.
		if( !m_pDecal_Polygon )	
		{
			RDecalPolygonSceneNode* pDecal = static_cast< RDecalPolygonSceneNode* >( pToolSceneManager->CreateSceneNode(RSID_DECAL_POLYGON) );
			m_pDecal_Polygon = (pDecal);
			pToolSceneManager->AddSceneNode(pDecal);

			pDecal->Create( pCollision->GetCollisionTree().GetCollisionTree() );	// ó�� �ѹ��� ������ ����
			m_pDecal_Polygon->Add(pRollupDecal->m_strTextureFileName.GetString(), MousePosVec, pRollupDecal->m_fDecalSize_X, pRollupDecal->m_fDecalSize_Y, pRollupDecal->m_fDecalSize_Z, pRollupDecal->m_fDecalRotate_Z);
		}
		else
		{
			m_pDecal_Polygon->Add(pRollupDecal->m_strTextureFileName.GetString(), MousePosVec, pRollupDecal->m_fDecalSize_X, pRollupDecal->m_fDecalSize_Y, pRollupDecal->m_fDecalSize_Z, pRollupDecal->m_fDecalRotate_Z);
		}
	}
}

}
