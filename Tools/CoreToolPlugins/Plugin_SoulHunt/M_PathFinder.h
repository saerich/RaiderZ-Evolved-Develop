#pragma once

#include "MPathFinder.h"
#include "MPathEngineExporter.h"
#include "MPath.h"

namespace rs3
{
	class RDevice;
	class RCollisionTree;
}

class RHeightField;
// ��ã�⸦ �׽�Ʈ �ϴ� �ڵ� Ŭ����
class CORETOOL_SOULHUNT_API CPathFinderTest
{
private:
	struct GVertex
	{
		MVector3 pos;
		DWORD color;
	};

	MPath m_currentPath;
	
	// Obstacle
	vector<MVector3> m_arrayObstacle;

	// Navigation Mesh (�浹/������)
	std::vector<GVertex> m_arrayNavVertices;

	bool m_bRenderNavigationMesh;

	void SetRenderState(RDevice *pDevice);
	void ResetRenderState(RDevice *pDevice);
	
	void fillConnectTriangleFlag(iMesh *mesh, int faceIndex, int value, std::vector<int> &vecFaceIndices);

public:
	std::vector<MVector3> m_arrayInvalidPosition;

public:
	MPathFinder m_PathFinder;

	CPathFinderTest();
	virtual ~CPathFinderTest();

	void Init();
	void Destroy();
	
	bool LoadZone(const std::string &strPath, RHeightField *pHeightField);
	int MakeInvalid(const std::vector<MVector3> &invalidArray);
	int MakeNavMesh();

	void Release();
	void ReleasePath();

	void GetPathVertices(std::vector<RVector> *pPathArrayOut);
	void FindPath(const RVector &vStart, const RVector &vEnd);

	// ���� 3DPosition�� ��ȿ����, �׸��� fZOffset ���� ���� �ִ��� �˻�
	bool IsVailid3DPostiion(const RVector &vPosition, float fZOffset = 100.f);
	// ���� 3DPosition���� fZUpOffset ��ŭ ��(Z����)�� ��ġ ������ Navigation mesh ������ 3DPosition ���� ����.
	bool GetValid3DPosition(RVector *pOut, const RVector &vPosition, float fZOffset = 100.f, float fZUpOffset = 100.f);

	void SetRenderableNavigationMesh(bool bFlag);

	void RenderNavMesh(RDevice *pDevice);
	void RenderCurrentPath(RDevice *pDevice);
};

extern CORETOOL_SOULHUNT_API CPathFinderTest g_PathfinderTest;