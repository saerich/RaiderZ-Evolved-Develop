#pragma once

/**
 Preprocess �տ� �ΰ� ������ �ٿ� ��ü ������ �����.

 file format
 {
	// header info
	DWORD dwHeader;
	DWORD dwVersion;
	DWORD dwCount;
	
	// shape info
	DWORD dwShapeSize
	MVector2 *pVertex2DArray;
 }
*/

class iMesh;
class iShape;
class MPathFindPreprocess
{
private:
	struct PATH_HEADER
	{
		DWORD dwHeader;
		DWORD dwVersion;
	};

	// Mesh-data�� MD5 �� �Ǵ��Ѵ�.
	// Shape �Ǻ� ����
	struct PATH_SHAPEINFO
	{
		DWORD dwShapeSize;
		long *pVertex2DiArray;
	};

	static DWORD CURRENT_VERSION;
	static DWORD CURRENT_HEADER;
	
	static bool IsVaildHeader(PATH_HEADER *pHeader);
	static bool IsValidShapeInfo(iShape *shape, PATH_SHAPEINFO *pShapeInfo);
public:
	static void GenerateFile(iMesh *mesh, const char *pcStringMD5, iShape *shape
		,const char *pathfindFilename
		,const char *collisionFilename
		,const char *const* CollisionAttributes = NULL
		,const char *const* PathfindAttribute = NULL);
	static bool LoadPreprocess(iPathEngine *pathengine, iMesh *mesh, const char *pcStringMD5, iShape *shape
		,const char *pathfindFilename
		,const char *collisionFilename
		);
};

/**
sample code
{
	// ������ �� ...
	MPathFindPreprocess::generateFile(mesh, shapeA, "shapeFile.small.navigationmesh");

	// �о���� �� ...
	bool bVaild = MPathFindPreprocess::loadPreprocess(mesh, shapeA);
	if (!bValid)	// shape ���İ� ����ġ !!
		MPathFindPreprocess::generateFile(mesh, shapeA, "shapeFile.small.navigationmesh");
}
*/