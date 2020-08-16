#pragma once

#include "RDecalCollision.h"

namespace rs3 {

// Projection Decal�� �׻� Dir�� -Z�� ī�޶� ����� ������.
// Terrain�� StaticMesh���� ��Į�� �׷������� ����(AABB) ��ŭ�� �������� �����Ѵ�.
// ZFighting�� ���ֱ� ���� Shader�� ����ؼ� �׸���.
class RS_API RDecalProjectionSceneNode : public RSceneNode
{
protected:
	RVector3					m_vPos;						///<
	float						m_fDepth;
	DWORD						m_dwFlags;					///< ����� ������ ���� Flag

	std::vector<RVt_pos>		m_vecVertexTerrain;			///< Terrain ���� ������ ������.
	std::vector<RVt_pos>		m_vecVertexActor;			///< cl2 �浹 �޽����� ������ ������.

	RCollisionTree*				m_pCollisionTree;			///< cl2
	RDecalCollision				m_DecalCollision;

	RTexture*		m_pTexture;					///< �ؽ���.
	float			m_fZBias;					///< ���� ������ �� �ְ�
	
public:
	RDecalProjectionSceneNode();
	virtual ~RDecalProjectionSceneNode();

	virtual void	SetRenderState( RCameraSceneNode* pCurrentCamera);
	virtual void	OnRender( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod) override;

	// Z�� ������ ���� ���� ������ �� �ְ� �Ѵ�.
	bool			Create(rs3::RCollisionTree* pCollisionTree, string strTexName, RVector3 vPos, float fHalfSizeX, float fHalfSizeY, float fHalfSizeZ, float fRotateZ = 0.0f, DWORD dwFlag = DECALFLAG_TERRAIN | DECALFLAG_ACTOR );
	void			Update(RVector3 vPos, float fHalfSizeX, float fHalfSizeY, float fHalfSizeZ);
	void			GetVertexFromBound();

	void			UpdateTextureTransform();
	RVector3		GetInteractionScale(const RBoundingBox& rbb) const;
	// �⺻���� -1.f / 65535.f �̰� ���� ���ڷ� ���е��� �����մϴ�
	// ���ǰ��� ���� ���� ZBias �� �� �ε巯���� �� ���� Offset�� ������ ��, Ŭ���� ���е��� �������ϴ�
	void			SetZBias(float fZBias) { m_fZBias = fZBias; }

	RVector4		m_vTFactor;					///<
	// D3DCOLOR_ARGB(255, 255, 255, 255)������ �����Ѵ�
	void			SetColor(DWORD dwColor) { 
		DWORD2RGBA(dwColor, m_vTFactor.r, m_vTFactor.g, m_vTFactor.b, m_vTFactor.a); 
		m_fVisibility = m_vTFactor.a;
	}
	void			SetColor(RVector4 vColor) { m_vTFactor = vColor; }

	float			m_fRotate;					///< ȸ��
	void			SetRotate(float fAxisZ) { m_fRotate = fAxisZ; }
};

} //namespace rs3
