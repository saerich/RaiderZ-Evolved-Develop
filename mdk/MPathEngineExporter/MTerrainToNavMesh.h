#pragma once

#include "MVector3.h"
#include "RTerrain.h"
#include "../../../sdk/pathengine/i_pathengine.h"

using namespace rs3;
// pathengine������ ������ �̾Ƴ��� Ŭ����

// ������ �Ϻθ� ǥ���ϴ� ���� �̵ �ﰢ��
class MTerrainTriangle
{
protected:
		MTerrainTriangle*	m_pChild1;		// p1-p2-(��������)
		MTerrainTriangle*	m_pChild2;		// p1-p3-(��������)
		MTerrainTriangle*	m_pParent;		// �θ�
		float				m_fError;		// ��Ȯ�� Z���� ���ؼ� �󸶳� ������ �ִ°�

		MTerrainTriangle*	m_pLeft;		// p1-p2�� ���ϴ� �ﰢ��
		MTerrainTriangle*	m_pRight;		// p1-p3�� ���ϴ� �ﰢ��
		MTerrainTriangle*	m_pDiagonal;	// p2-p3 (����) �� ���ϴ� �ﰢ��

		bool				m_bSteep;		// ������� �����Ǿ�� �ϴ°�
		bool				m_bSplited;		// ���ҵǾ��°�?
		int					m_nLevel;		// �� ������ �� �ִ� Ƚ��

		MVector3			p1, p2, p3;		// p1�� ������ = 90��	p2-p3 : ����

		
		// �߰��� ��� �ﰢ���� ���ҵǾ��ٸ� �� �θ����� �� ���ҵ� �ʿ䰡 �ִ�.
		void				SplitParent();	

public:
		MTerrainTriangle();
		virtual ~MTerrainTriangle();

		// �ﰢ���� �����Ѵ�. ���� ū �ﰢ�� �ΰ��� �����Ҷ� ���.
		void				Set(const MVector3& _p1, const MVector3& _p2, const MVector3& _p3, MTerrainTriangle* pDiag, int nLevel);

		// �� �ﰢ�� ������ �ִ� ���� Ƚ�� ���ѱ��� �� ������ �ﰢ�� Ʈ���� �����.
		void				MakeFullTree(RTerrain* pTerrain);

		// �� �ﰢ�� ���� �ﰢ���鿡 ��, ��, �밢�� ������ �ﰢ������ ����� �����Ѵ�.
		void				SetLeftRightDiagonalNode();

		// �� �ﰢ�� ���� �ﰢ���� ��� �ﰢ���� ���� fThreshold �ȿ� �� ������ �����Ѵ�.
		void				Split(float fThreshold);

		// �� �ﰢ�� ���� ��� ��带 �ı��Ѵ�.
		void				Destroy();
		void				GetTriangleList(vector<MVector3>& vec);

		// ����� ��� üũ & �Ӽ� üũ
		void				ClipSteepPlane(RTerrain *pTerrain, float fLimitDot);

		// �����ϰ� �����
		int					Flatten();
};


// ������ ������ �ﰢ�� �޽��� �ٻ��ϴ� Ŭ����
class MExportedTerrain
{
		friend class RTerrain;
protected:
		MTerrainTriangle	tri1;
		MTerrainTriangle	tri2;

		vector<MVector3>	trivec;
		vector<int>			indexVertsRef;

		vector< vector<int> > adjData;

		vector<int>			faceID;
		vector<int>			visited;
		vector<int>			faceGroupSize;

		void				SetInitialTriangles(RTerrain* pTerrain);
		void				Build(RTerrain* pTerrain, float fErrorThreshold);

		void				BuildIndexedForm();
		void				FlipCCWFaces();
		void				BuildAdjFaceList();
		void				FloodFillFaces();
		void				RemoveDisconnect(RTerrain *pTerrain, vector<MVector3> &seeds);
		void				ClipTooSmallChunk(int nLimitSize);
		void				RemoveNoneIndex();
public:
		vector<MVector3>	indexVerts;
		vector<int>			indices;

		void				BuildAll(RTerrain* pTerrain, vector<MVector3> &seeds, float fErrorThreshold, float fLimitDot, int nLimitSize);
		void				DestroyBuffers();

		MExportedTerrain();
		virtual ~MExportedTerrain();
};



class MExportedTerrainMesh : public iFaceVertexMesh
{
public:
		MExportedTerrain eTerrain;

		long faces() const
		{
			return (long)eTerrain.indices.size() / 3;
		}
		long vertices() const
		{
			return (long)eTerrain.indexVerts.size();
		}

		long vertexIndex(long face, long vertexInFace) const
		{
			return (long)eTerrain.indices[face * 3 + (2 - vertexInFace)];
		}
		long vertexX(long vertex) const
		{
			return (long)eTerrain.indexVerts[vertex].x;
		}
		long vertexY(long vertex) const
		{
			return (long)eTerrain.indexVerts[vertex].y;
		}
		float vertexZ(long vertex) const
		{
			return eTerrain.indexVerts[vertex].z;
		}
		long faceAttribute(long face, long attributeIndex) const
		{
			switch ( attributeIndex )
			{
			case PE_FaceAttribute_SectionID:	// ���� Section ID �� 0���� ����
				return 0;
			}

			return -1;
		}

		MExportedTerrainMesh();
		virtual ~MExportedTerrainMesh();
};