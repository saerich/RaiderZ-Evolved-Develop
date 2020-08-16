#pragma once

#include "MPathFinder.h"
#include "RSceneManager.h"
#include <vector>
#include <string>

using namespace rs3;

/////////////////////////////////////////////////////////////////////////////
//

class MPathEngineExporter
{
	/// Exporter Error Handler
	class MPathEngineExporterError : public iErrorHandler
	{
		virtual eAction handle(const char* type, const char* description, const char *const* attributes);
	};
private:
	/**
	 	iFaceVertex �� �м��ؼ� ����� mlog������ ����Ʈ �Ѵ�. (debug�� �ϱ� ���� �Լ�)
	 */
	void GroundMeshLogging(const iFaceVertexMesh *pFaceVertex);

	/**
	 RActor Array�� �޾Ƽ� iContentChunkInstance ���� �����͸� �����.
	 @param pPathEngine	
	 @param chunkArray
	 @param chunkInstanceArray
	 @param vActorArray
	 @param vExActorArray
	*/

	// Actor path�� ��ȸ�ϰ�,
	void	MakeChunkInstanceListActors(MPathFinder *pPathEngine
		, std::vector<iContentChunk*> &chunkArray
		, std::vector<iContentChunkInstance*> &chunkInstanceArray
		, std::vector<RSceneNode*> &chunkInstanceSources
		, std::vector<RSceneNode*> &vActorArray
		, std::vector<RSceneNode*> *pExActorArray = NULL
		);

	// Zone path ���θ� �˻��Ѵ�.
	void	MakeChunkInstanceListStaticMesh(MPathFinder *pPathEngine
		, const std::string &strZoneFilePath
		, std::vector<iContentChunk*> &chunkArray
		, std::vector<iContentChunkInstance*> &chunkInstanceArray
		, std::vector<RSceneNode*> &vStaticMeshArray);

	static bool GetAllActor(rs3::RSceneNode *pSceneNode);
	static bool GetAllStaticMesh(rs3::RSceneNode *pSceneNode);

	static std::string m_strErrorBuffer;

	bool ReadObstacleFile(std::vector<std::vector<MVector3> > &outArray, const char *filename);

	// ��ȿ���� ���� ex_mesh ���� (���� ����)
public:
	std::vector<MVector3> m_invaildExPosition;

public:
	/// NavigationMesh(Ground mesh)�� �����.
	bool	MakeNavMesh(MPathFinder* pPathEngine, RSceneManager* pManager, std::vector<MVector3> &seeds);
	/// Speed Tree �� Object�� static - obstacles �� �����.
	void	ExportTrees(MPathFinder* pPathEngine, RSceneManager* pManager);
	/// RActor �� ���� .obstacles�� static - obstacles �� �����. (��, collision �÷��װ� true �� �� �͸�)
	void	ExportActor(MPathFinder* pPathEngine, std::vector<RSceneNode*> *pNodeArray);
	/// RStaticMesh�� .obstacles�� static - obstacles �� �����.
	void	ExportStaticMesh(MPathFinder* pPathEngine, std::vector<RSceneNode*> *pNodeArray, const string &strPath);

	void	SaveToFile(MPathFinder* pPathEngine, const string& strFilename);
	void	SaveToFile(MPathFinder* pPathEngine, const string& strFilename, const vector<string> &vShapes);

	/// vPos�� ����� ���� iMesh���� �����´�.
	static	cPosition	GetClosestPosition(iMesh *pMesh,MVector3& vPos);
	/// vPos�� cPos�� �Ÿ��� threshold ���� ���̳�?
	static	bool		IsPositionInThreshold(iMesh *pMesh,MVector3& vPos,cPosition& cPos);

	iMesh	*m_pRawMesh;
	iPathEngine *m_pEngine;

	const std::string& GetErrorMessage()
	{
		return m_strErrorBuffer;
	}

	MPathEngineExporter();
	virtual ~MPathEngineExporter();
};

/////////////////////////////////////////////////////////////////////////////
