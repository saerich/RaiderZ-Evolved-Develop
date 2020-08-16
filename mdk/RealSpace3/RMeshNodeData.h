#ifndef _RMeshNodeData_h
#define _RMeshNodeData_h

#include "RPreDefine.h"
#include "RMeshUtil.h"

namespace rs3 {

class RMesh;
class RMaterial;

// ���� �� ������ ����

class RS_API RMeshNodeData :public RBaseObject
{
public:
	RMeshNodeData();
	virtual ~RMeshNodeData();

	// bbox

	void		BBoxClear();
	void		CalcLocalBBox();

	int			GetFacePosIndex(int findex,int pindex);
	int			GetFaceNormalIndex(int findex,int pindex);
	int			GetFaceUVIndex(int findex,int pindex);

	RVector		GetFacePos(int findex,int pindex);		// ���� ��ġ��.
	RVector		GetFaceNormal(int findex,int pindex);	// ���� ��ְ�.
	RVector		GetFaceUV(int findex,int pindex);		// ���� uv ��.
	int			GetFaceMtrl(int findex);				// ���� ���� �ε���.

	char*		GetParentName() const { return (char*)m_ParentName.c_str(); }

public:

	// id

	int				m_nID;
	int				m_nUserID;
	int				m_nMtrlID;

	// name

	string			m_ParentName;					// �θ� ��� �̸�
	int				m_nParentNodeID;				// �θ� ��� ID

	// bbox

	RBoundingBox	m_boundingBox;

	// matrix

	RMatrix			m_matLocal;						// ������ġ ��Ʈ����
	RMatrix			m_matResult;					// ������� ��Ʈ����	TODO: ����
	float			m_fBaseVisibility;				// �⺻ visibility
	float			m_fBaseNoiseRef;

	int				m_nMtrlTableInfoCnt;			// ����   ����
	RMtrlTableInfo*	m_pMtrlTableInfo;				// sub-material �ε������� ����

	// ����Ǿ��ִ� ������Ʈ�� ( polygons )
	int				m_nPointCnt;					// ��ġ		����
	RVector*		m_pPointTableOrg;				// ����ġ  ����
	RVector*		m_pPointTable;					// ���� ��ġ ����	--> TODO: pointTableCurrent �� �ٲ���
	int				m_nPointColorCnt;				// �÷�		����
	RVector*		m_pPointColorTable;				// �÷�    ����
	int				m_nNormalTableCnt;				// ��� ���� ����
	RVector*		m_pNormalTable;					// ���    ����

	// ���ؽ����ۿ� �����Ǵ� normal �迭
	int				m_nPointNormalCnt;				// ���		���� 
	RVector*		m_pPointNormalTable;			// ���� point �� ���� ��� ����	// TODO: �̰� ���� �ʿ��ҵ�. 

	int				m_nTangentTanTableCnt;			// ��� ���� ����
	RVector4*		m_pTangentTanTable;				// tangent ���� // version16���� vector4�� ���� w = FaceParity
	int				m_nTangentBinTableCnt;			// ��� ���� ����
	RVector*		m_pTangentBinTable;				// binormal����

	int				m_nTexCoordTableCnt;			// �ؽ��� uv ����
	RVector*		m_pTexCoordTable;				// �ؽ��� uv ����

	int				m_nTexCoordExtraTableCnt;		// ���� �ؽ��� uv ����
	RVector*		m_pTexCoordExtraTable;			// ���� �ؽ��� uv ����

	int				m_nPhysiqueCnt;					// physique ����
	RPhysiqueInfo*	m_pPhysique;					// Physique ����
	int				m_nFaceCnt;						// ��		����	m_pPolygonTable�� ����
	RMeshPolygonData*	m_pPolygonTable;			// polygon ����
	int				m_nBoneTableCnt;				// ��		���� - ������ �������� �Ǵ��ص� �ǳ�? COMMENT OZ 090709
	RMatrix*		m_pBoneTable;					// Bone		����
	WORD*			m_pBoneTableIndex;				

	// �������� ���� ������Ʈ�� ( triangles )
	int				m_nVertexIndexTableCnt;			// ���ؽ� ���� ���� ����
	RVertexIndex*	m_pVertexIndexTable;			// ���ؽ� ���۸� ������ ����
	int				m_nFaceIndexTableCnt;			// �ε������� ����
	WORD*			m_pFaceIndexTable;				// �ε��� ���۸� ������ ����

	int				m_nTotalDegree;					// ����������� degree �� �հ�
	int				m_nTotalTriangles;				// ����������� �ﰢ������ �հ�
#ifdef MESH_STRIP_TEST
	RMtrlTableInfo*	m_pMtrlTableInfoDefault;		// default & optimized �ε������� ����
	RMtrlTableInfo*	m_pMtrlTableInfoStrips;			// strips �ε������� ����
#endif

	RFaceSubData*	m_pAllFaceSubData;				// m_pPolygonTable �ȿ� FaceSubData �� �޸� �Ҵ�
};

//////////////////////////////////////////////////////////////////
// Node Mtrl 
//
// ��� �������� �Լ��� .. 
//		�⺻������ �Ѱ����� �����ϰ�.. 
//			�����͸� ���д�.

class RS_API RMeshNodeMtrl
{
public:
	RMeshNodeMtrl();
	virtual ~RMeshNodeMtrl();

	int			GetMtrlCount();
	RMaterial*	GetMtrl(int i=0);// MtrlTable

public:
	int			m_nMtrlCnt;					// �ڽ��� ��尡 ����ϴ� ������ ����. ������ ��� 1��.
	RMaterial**	m_pMtrlTable;				// ���� ������ ���̺�.
};

}

#endif//_RMeshNodeData_h
