#pragma once

#include "../../../sdk/pathengine/i_pathengine.h"
#include "MPathAgentShape.h"
#include <vector>
#include "MVector3.h"

/////////////////////////////////////////////////////////////////////////////
//

enum PATHENGINE_RESULT_TYPE 
{
	PATH_SUCCESS = 0,
	INVALID_PARAMETER,
	INVALID_POSITION,
	COLLISION,
};

class MAgent;
class MPath;
class MCollisionInfo;
class MContext;
class RHeightField;
class MPathTerrainCallback;

struct SPathMesh
{
	/**
	m_nID�� ���� Ground Mesh (Navigation Mesh)�� ����
	�ϳ��� iMesh�� ShapeSet(���� ���� Shape)�� ����.
	Shape�� ���������� MPathFinder�� ����ְ�, name���� �� SPathMesh ����ü�� ��ϵǾ� ����/�����ȴ�. ( pre-process Path/Collision data )

	m_nID �ϳ��� �浹 ���ؽ�Ʈ�� ���� �ϳ�
	*/
	int	m_nID;
	iMesh *m_pMesh;

	MPathTerrainCallback *m_pTerrainCallback;	
	vector<string> m_vShapeName;

	SPathMesh()
		: m_nID(-1)
		, m_pMesh(NULL)
		, m_pTerrainCallback(NULL)
	{
	}
};

class MPathFinder
{
	/////////////////////////////////////////////////////////////////////////////
	//

	class MPathFinderError : public iErrorHandler
	{
		virtual eAction handle(const char* type, const char* description, const char *const* attributes); 
	};

	/////////////////////////////////////////////////////////////////////////////
	//


	/////////////////////////////////////////////////////////////////////////////
	//

	class MQueryCallBack : public iQueryCallBack
	{
		tSigned32 _frequency;
		tSigned32 _count;
		tSigned32 __count;
	public:
		void Set(tSigned32 frequency, tSigned32 count)
		{
			_frequency = frequency;
			_count = __count = count;
		}

		void reset() 
		{
			_count = __count;
		}

		tSigned32 desiredCallBackFrequency()
		{
			return _frequency;
		}
		bool abort()
		{
			--_count;
			return _count == 0;
		}
	};

	/////////////////////////////////////////////////////////////////////////////

public:
	SPathMesh*				GetMesh(int id);
	iMesh*					GetIMesh(int id);
	iShape*					GetShape(const string& strShape);

	int						GetShapeRange(const string& strShape);
private:

		bool					LoadPathEngine();

		MPathFinderError		m_ErrorHandler;
		
		bool					PreProcessQuery(iMesh* pMesh, MPathAgentShape* pShape, const MVector3& vPos, cPosition &cPos
												,int nHorizonRange = 1000, int nVerticalRange = 500);
		void					BuildPath(iMesh* pMesh, iPath* pPath, vector<MVector3>& ret, int startoffset);

		iPathEngine*			m_pPathEngine;
		MPathAgentShapeSet		m_Shapes;
		HINSTANCE				m_hInstance;

		bool					LoadNavigationMeshFile(int nID, SPathMesh &outPathMesh, char pcOutMD5String[16], const string &strFilename, RHeightField *pHeightField);

		bool					LoadPreprocessFile(SPathMesh &pathMesh, const char pcMD5String[16], const string &strFilename, const vector<string>& vShapes, bool bAutoGeneration);


public:

		vector<SPathMesh>		m_vZoneList;
		MQueryCallBack			m_QueryCallBack;

		bool					Create();
		void					Destroy();

		bool					AddZone(int nID, const string& strFilename, RHeightField *pHeightField = NULL, bool bAutoGeneratePreprocess = true);
		bool					AddZone(int nID, const string& strFilename, const vector<string>& vShapes, RHeightField *pHeightField = NULL, bool bAutoGeneratePreprocess = true);
 
		/**
		��ǥ�� ��ȿ�� �˻� �� ���� ����
		 @param	pContext	�浹 ���� ( �� ���� NULL �� �ƴϸ�, context �� ���ϵ� agent�� ���� �浹�˻絵 ���Ѵ� )
	    */
		PATHENGINE_RESULT_TYPE	IsValidPosition(float *height, int nID, const MContext *pContext, const MVector3& position, const string& strShape = "small");

		/**
		 �α��� ��ȿ�� ��ǥ�� ã�´�.

		 @param pOut ã�Ƴ� ��ǥ(3D)�� ������ MVector3 ������
		 @param nID Mesh ID
		 @param	pContext	�浹 ���� ( �� ���� NULL �� �ƴϸ�, context �� ���ϵ� agent�� ���� �浹�˻絵 ���Ѵ� )
		 @param position ���� ��ǥ
		 @param nHorizoneRange	X,Y ����(+-)�� �α� ��ǥ�� ã�´�. 
		 @param nVerticalRange	Z ����(+-)�� ��ǥ�� ã�´�.
		 @param strShape Shape ��Ʈ��, ���� small/medium/large ������

		 @return ã�µ� �����ϸ� true�� ����, �����ϸ� false�� ����
		 */
		PATHENGINE_RESULT_TYPE	IsNearPosition(MVector3 *pOut, int nID, const MContext *pContext, const MVector3& position
			,int nHorizonRange, int nVerticalRange, const string& strShape = "small");

		// true�� �����ϸ� ���������� MAgent�� ���� ���̴�.
		bool					NewPlaceAgent(MAgent &agent, const MVector3& position, int nID, const string& strShape = "small");

		// true�� �����ϸ� ���������� MContext�� ���� ���̴�.
		bool					NewContext(MContext &context, int nID);
		
		// PATHENGINE_RESULT_TYPE �� ����, SUCCESS = 0 �� �ƴϸ� ���� �߸��� ���̴�.
		PATHENGINE_RESULT_TYPE	AdvancedAlongPath(MCollisionInfo *pCollisionInfoOut, int nID, MAgent &agent, MPath &path, float fDistance);
		
		// ???
		float					FindPath(int nID, const MVector3 &vStart, const MVector3 &vEnd, vector<MVector3>& vPath
										, const string& strShape = "small");
		float					FindPath(int nID, const MVector3 &vStart, const MVector3 &vEnd, vector<MVector3>& vPath
										, int nFreq, int nCount, const string& strShape = "small");

		// true�� �����ϸ� ���������� MPath�� ���� ���̴�.
		// MPath�� ���� FindPath , ������ ���͸� ���� �� ���� ������ ���� �� ���Ƽ� �׽�Ʈ�� ������.
		bool					FindPath(int nID, const MVector3 &vStart, const MVector3 &vEnd, MPath &outPath, MContext *pContext, const string& strShape = "small");

		// agent�� ����ϴ� PathFind
		bool					FindPath(MPath &outPath, int nID, const MAgent &agent, const MVector3 &vEnd
										, const string& strShape = "small");
		bool					FindPath(MPath &outPath, int nID, const MAgent &agent, const MVector3 &vEnd, int nFreq, int nCount
										, const string& strShape = "small");

		// vCenter �߽� fSize ������ ���簢�� ������ Ż���ϴ� �ִܰ��
		// ��ȯ�� : ��� ����
		float					FindPathAway(int nID, const MVector3& vStart, const MVector3& vCenter, float fSize, vector<MVector3>& vPath, const string& strShape = "small");
		float					FindPathAway(int nID, const MVector3& vStart, const MVector3& vCenter, float fSize, int nFreq, int nCount, vector<MVector3>& vPath, const string& strShape = "small");

		// start���� end�� �̵��ҋ� ù �浹 ������ �˷��ش�. ��ȯ���� �浹����
		PATHENGINE_RESULT_TYPE FindFirstCollision(int nID, const MVector3& vStart, const MVector3& vEnd, MVector3& vRet, const string& strShape = "small");
public:

		void					PathTest(int nID, const MVector3& vStart, const MVector3& vEnd, FILE* fp, const string& strShape = "small");

		// MPath�� �������� 3D ���� �迭�� �����´�. ũ�� �ʿ� ���� �� �ʹ�.
		void					PathTo3DPoints(std::vector<MVector3> &out3DPoints, int nID, MPath &basePath, int offset = 0);
		void					PathTo3DPoint(MVector3 &outPos, int nID, MPath &basePath, int index);

		// Navigation Mesh �����͸� Triangle-List ���������� �����Ѵ�.
		int						GetStaticObstacleDebugMesh(int nID, std::vector<MVector3> &triangleArray, float fShapeHeight = 100.f);

		int						GetNavigationMeshFaceCount(int nID);
		int						GetNavigationMeshFaceList(int nID, MVector3 *pVertices, int nFaceCount);

		void					ClearZone();
		iPathEngine*			GetPathEngineInterface()
		{
			return m_pPathEngine;
		}

		MPathFinder();
		~MPathFinder();
};
