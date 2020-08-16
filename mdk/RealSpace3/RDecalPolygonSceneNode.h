/* Copyright (C) Eric Lengyel, 2001. 
* All rights reserved worldwide.
*
* This software is provided "as is" without express or implied
* warranties. You may freely copy and compile this source into
* applications you distribute provided that the copyright text
* below is included in the resulting source code, for example:
* "Portions Copyright (C) Eric Lengyel, 2001"
*/

#pragma once

#include "RDecalCollision.h"
#include "RSceneNode.h"
#include <deque>

namespace rs3 {

// RTexture ���� �з��س��� Batch Render Set
	class RDecalPolygonRenderSceneNode : public RSceneNode
{
public:
	std::deque<int>				m_deqDecalPolygon;	///< �� ���������� Render DecalPolygon List (����ȭ�� ���� VertexCount�� ������.)
	// �ϳ��� ��Į ��ü�� ���� ������ ������ AABB �����͸� ���� �����Ѵ�.
	// �Ź� AABB ������ �ϵ� Lock �� �� �ʿ� ���� AABB�� ���ϴ� ���길 �ϸ� �ǹǷ� ����� ����.
	std::deque<RBoundingBox>	m_deqDecalAABB;
	
	RTexture*					m_pTexture;

	// Batch Render --------------------------------------------------------
	// End�� Start���� ������ �ι� �׸���.
	//  1. 0 to end
	//  2. start to max
	// �ƴϸ� �ѹ� �׸���.
	//  1. 0 to end
	// ---------------------------------------------------------------------
	int		m_nOffsetStart;
	int		m_nOffsetEnd;
	int		m_nOffsetFree;
	HVERTEXBUFFER					m_hVertexBuffer;

	// 
	int		m_nOffsetDeletedStart;
	int		m_nOffsetDeletedEnd;
	int		m_nOffsetDeletedFree;
	HVERTEXBUFFER					m_hDeletedVertexBuffer;	// �������� �������� �׸� ����
	std::vector<RVt_pos_clr_tex1>	m_vecDeletedVertices;	// ���� �۾����� ����

	int		m_nVertexSize;
	DWORD	m_dwBlendTime;		///< ������ ���� �� �ð�
	int		m_nPriority;		///< Overlay ���� ���ϼ��� ���� ���׷�����.

	RDecalPolygonRenderSceneNode::RDecalPolygonRenderSceneNode(int nPriority, DWORD dwBlendTime, int nVertexSize = 9999 );
	RDecalPolygonRenderSceneNode::~RDecalPolygonRenderSceneNode();

	void Create();
	void AddDecal(std::vector<RVt_pos_clr_tex1>& vecVertex, RTexture* pTexture = NULL);
	void UpdateAABB(std::vector<RVt_pos_clr_tex1>& vecVertex);

	virtual void	SetRenderState( RCameraSceneNode* pCurrentCamera);
	virtual void	OnRender( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod) override;

	RVector4		m_vTFactor;					///< ���� �����ȵ�. ���� �����ϰ� ����..
	// D3DCOLOR_ARGB(255, 255, 255, 255)������ �����Ѵ�
	void			SetColor(DWORD dwColor) { 
		DWORD2RGBA(dwColor, m_vTFactor.r, m_vTFactor.g, m_vTFactor.b, m_vTFactor.a); 
		m_fVisibility = m_vTFactor.a;
	}
	void			SetColor(RVector4 vColor) { m_vTFactor = vColor; }
};

class RS_API RDecalPolygonSceneNode : public RSceneNode
{
	// RenderSet ��ü ����.
	std::map<std::pair<int, RTexture*>, RDecalPolygonRenderSceneNode*>	m_mapDecalPolygonRenderSet;

protected:
	RVector3					m_vPos;						///<
	float						m_fDepth;
	DWORD						m_dwFlags;					///< ����� ������ ���� Flag

	std::vector<RVt_pos>		m_vecVertexTerrain;			///< Terrain ���� ������ ������.
	std::vector<RVt_pos>		m_vecVertexActor;			///< cl2 �浹 �޽����� ������ ������.

	RCollisionTree*				m_pCollisionTree;			///< cl2
	RDecalCollision				m_DecalCollision;

public:
	// Clip Polygon
	float			m_fDecalEpsilon;

	RVector3		m_vDecalCenter;
	RVector3		m_vDecalNormal;

	RVector4		m_vLeftPlane;
	RVector4		m_vRightPlane;
	RVector4		m_vBottomPlane;
	RVector4		m_vTopPlane;
	RVector4		m_vFrontPlane;
	RVector4		m_vBackPlane;

	int				m_nCountVertex;
	int				m_nCountTriangle;

	// �Ʒ� �迭�� ��� DecalPolygon ���������θ� ���ȴ�.
	std::vector<RVector3>			m_vecVertex;
	std::vector<WORD>				m_vecIndex;
	std::vector<RVt_pos_clr_tex1>	m_vecTempVertex;
	std::vector<RVt_pos_clr_tex1>	m_vecDecalVertex;

public:
	RDecalPolygonSceneNode();
	~RDecalPolygonSceneNode();

	bool			Create(rs3::RCollisionTree* pCollisionTree);
	void			Destroy();

	void			CreatePolygon(std::vector<RVt_pos>& vecVertex, RVector3& center, RVector3& normal, RVector3& tangent, float width, float height, float depth);
	bool			AddPolygon(int vertexCount, RVector3* vertex, RVector3* normal, bool *vIsClip);
	void			ClipMesh(int nFaceCount, std::vector<RVt_pos>& vecVertex, RVector3* vecNormal);
	int				ClipPolygon(int vertexCount, RVector3* vecVertex, RVector3* vecNormal, RVector3* newVertex, RVector3 *newNormal, bool *vIsClip);
	static int		ClipPolygonAgainstPlane(RVector4& plane, int vertexCount, RVector3* vecVertex, RVector3* vecNormal, RVector3* newVertex, RVector3* newNormal, bool *vIsClip);

	// nPriority�� Ŭ���� ���� ���� �׸���.
	void			Add(string strTexName, RVector3 vPos, float fHalfSizeX, float fHalfSizeY, float fHalfSizeZ, float fRotateZ = 0.0f, int nPriority = 0, int nVertexBufferSize = 9999, DWORD dwBlendTime = 3000, DWORD dwFlag = DECALFLAG_TERRAIN | DECALFLAG_ACTOR);
	void			DeleteAll(string strTexName);	///< �ش� ������ RenderSet �׸� ��θ� �����Ѵ�.

	bool			GetVertexFromBound(RBoundingBox aabb);
	void			IntersectionTriangle(std::vector<RVt_pos>& vecVertex, float& fPickDistanceSq, int& nTriIndex);

	float			m_fBias;
	float			m_fSlopeBias;
	void			SetDepthBias(float fBias, float fSlopeBias) {
		m_fBias = fBias;
		m_fSlopeBias = fSlopeBias;
	}

	bool			m_bWire;
	void			IsRenderWire(bool bWire) { m_bWire = bWire; }

	void			SetOverlayPriority(string strTexName, int nPriority);
};

} //namespace rs3
