#ifndef _GPATH_FINDER_h
#define _GPATH_FINDER_h

#include "GPathFinderImpl.h"

class GQueryCallBack : public iQueryCallBack
{
private:
	unsigned int m_nStartTime;
	bool m_bAborted;

	tSigned32 _frequency;
	tSigned32 _count;
	tSigned32 __count;
	cPosition _startPos;
	cPosition _destPos;
	iMesh* _mesh;

public:
	GQueryCallBack() : m_bAborted(false) {}
	void Set(tSigned32 frequency, tSigned32 count);
	void reset(iMesh* mesh, cPosition startPos, cPosition destPos);

	tSigned32 desiredCallBackFrequency()
	{
		return _frequency;
	}
	bool abort();
	bool IsAborted() { return m_bAborted; }
};

class GPathFinder : public MTestMemPool<GPathFinder>
{
public:
	enum QueryResult
	{
		QUERY_SUCCESS = 1,
		QUERY_SUCCESS_BUT_ABORTED,
		QUERY_FAILED
	};

private:
	static float OFFSET_Z;
	GPathFinderImpl	m_PathFinder;
	GQueryCallBack	m_QueryCallback;
private:
	QueryResult _FindPath(MPath& outPath, SPathMesh* pPathMesh, const MAgent& agent, const vec3& vEnd);

	iShape* GetShape()
	{
		return m_PathFinder.GetShape();
	}

public:
	GPathFinder();
	virtual ~GPathFinder();

	/**
	* �ش� ����(�׺���̼Ǹ޽�)�� �ش� �� ID�� �ε��Ѵ�.
	*
	* @param	nZoneID		�� ID
	* @param	szFileName	���� �̸�
	*/
	bool AddZone(int nZoneID, const wchar_t* szFileName, RHeightField *pHeightField);
	virtual bool NewPlaceAgent(MAgent &agent, const vec3& position, int nZoneID, SPathMesh* pPathMesh);
	virtual bool NewContext(MContext &context, SPathMesh* pPathMesh);


	/**
	* �ش��ϴ� 3D ��ǥ�� ��ȿ���� �׷��� ������ �˻��Ѵ�. 3D ��ǥ���� Navigation mesh�� ���� Picking�� �ؼ� �Ǵ��Ѵ�.
	*
	* @param	*height	���̸� ���� float�� ������, �ʿ������ NULL�� ������ �ȴ�.
	* @param	nID		�� ID
	* @param	pContext	�浹 ���� ( �� ���� NULL �� �ƴϸ�, context �� ���ϵ� agent�� ���� �浹�˻絵 ���Ѵ� )
	* @param	position	�˻��� 3D ��ġ
	* @param	strShape	Shape �̸� (�⺻���� small)
	* @return	���¿� ���� PATHENGINE_RESULT_TYPE�� ����
	*/
	virtual PATHENGINE_RESULT_TYPE IsValidPosition(float *height, int nID, const MContext *pContext, const MVector3& position);
	virtual PATHENGINE_RESULT_TYPE IsValidPosition(float *height, SPathMesh* pPathMesh, const MContext *pContext, const MVector3& position);

	/**
	* �ش��ϴ� 3D ��ǥ�� ��ȿ���� �׷��� ������ �˻��Ѵ�. 3D ��ǥ���� Navigation mesh�� ���� Picking�� �ؼ� �Ǵ��Ѵ�.
	*
	* @param	*height	���̸� ���� float�� ������, �ʿ������ NULL�� ������ �ȴ�.
	* @param	nID		�� ID
	* @param	position	�˻��� 3D ��ġ
	* @return	��ȿ���� ����
	*/
	bool IsValidPosition(SPathMesh* pPathMesh, const MVector3& position);

	virtual PATHENGINE_RESULT_TYPE GetHeightAtPosition(float *height, SPathMesh* pPathMesh, const MVector3& position);

	/**
	* �ش��ϴ� 3D ��ǥ���� ���� ���� �� ���� ���� +- ���� �̳��� �浹���� �ʴ� ��ǥ�� ã�´�. 
	(Navigation Mesh�� ��� ������ ã�� ���ϸ�, ���� ���� ����� ã�� ���Ѵ�.)

	nHorizoneRange��, nVerticalRange�� ������ ������ ������ ���� �� �ִ�. �浹���� �ʴ��� �ƴ����� ���� ShapeRange ���� �Ἥ ã�Ƴ���.
	���� ���� ����ó�� nHorizonRange�� GetShapeRange ���� ���ͼ� �� �ʿ�� ����.

	���� ����� ���� ã�� ���� �ƴϴ�. ���� ���� �ȿ� �´� ������ ���� ã�� ���̸�, L'Generate and test' ����� ���ٰ� �Ѵ�.
	*
	* @param pOut ã�Ƴ� ��ǥ(3D)�� ������ MVector3 ������
	* @param nID �� ID
	* @param pContext	�浹 ���� ( �� ���� NULL �� �ƴϸ�, context �� ���ϵ� agent�� ���� �浹�˻絵 ���Ѵ� )
	* @param position ���� ��ǥ
	* @param nHorizonRange	X,Y ����(+-)�� �α� ��ǥ�� ã�´�. 
	* @param nVerticalRange	Z ����(+-)�� ��ǥ�� ã�´�.
	* @param strShape Shape ��Ʈ��, ���� small/medium/large �� ����
	*
	* @return ���¿� ���� PATHENGINE_RESULT_TYPE�� ����
	*/
	virtual PATHENGINE_RESULT_TYPE FindNearPosition(MVector3 *pOut, SPathMesh* pPathMesh, const MContext *pContext, const MVector3& position ,int nHorizonRange, int nVerticalRange);


	virtual bool FindPath_NoAgent(SPathMesh* pPathMesh, vec3 vStart, vec3 vEnd, vector<vec3>& vPath);
	virtual GPathFinder::QueryResult FindPath(MPath &outPath, SPathMesh* pPathMesh, const MAgent &agent, MVector3 vEnd);

	virtual float FindPathAway(SPathMesh* pPathMesh, vec3 vStart, vec3 vCenter, float fSize, int nFreq, int nCount, vector<vec3>& vPath);

	
	virtual void					PathTo3DPoints(std::vector<MVector3> &out3DPoints, SPathMesh* pPathMesh, MPath &basePath, int offset = 0);
	virtual void					PathTo3DPoint(MVector3 &outPos, SPathMesh* pPathMesh, MPath &basePath, int index);

	SPathMesh*				GetMesh(int id)
	{
		return m_PathFinder.GetMesh(id);
	}

	static int TEST_PathfindZoneID;
	static cPosition TEST_PathfindStartRawPos;
	static cPosition TEST_PathfindEndRawPos;
	static cPosition TEST_PathfindStartPos;
	static cPosition TEST_PathfindEndPos;
};



#endif // _GPATH_FINDER_h