#pragma once

#include "MPathFinder.h"
#include "MPathAgentShape.h"
#include <vector>
#include "MVector3.h"

/////////////////////////////////////////////////////////////////////////////
//

class MAgent;
class MPath;
class MCollisionInfo;
class MContext;
class RHeightField;
class MPathTerrainCallback;
class GPathResMgr;

class GPathFinderImpl
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
	GPathResMgr*			m_pPathResMgr;

	SPathMesh*				GetMesh(int id);
	iMesh*					GetIMesh(int id);
	iShape*					GetShape();
private:

		bool					LoadPathEngine();

		MPathFinderError		m_ErrorHandler;
		
		bool					PreProcessQuery(iMesh* pMesh, iShape* pShape, const MVector3& vPos, cPosition &cPos
												,int nHorizonRange = 0, int nVerticalRange = 10);
		void					BuildPath(iMesh* pMesh, iPath* pPath, vector<MVector3>& ret, int startoffset);

		iPathEngine*			m_pPathEngine;
public:
		MQueryCallBack			m_QueryCallBack;

		bool					Create();
		bool					AddZone(int nID, const wstring& strFilename, RHeightField *pHeightField = NULL);
 
		/**
		��ǥ�� ��ȿ�� �˻� �� ���� ����
		 @param	pContext	�浹 ���� ( �� ���� NULL �� �ƴϸ�, context �� ���ϵ� agent�� ���� �浹�˻絵 ���Ѵ� )
	    */
		PATHENGINE_RESULT_TYPE	IsValidPosition(float *height, int nID, const MContext *pContext, const MVector3& position);
		PATHENGINE_RESULT_TYPE	IsValidPosition(float *height, SPathMesh* pPathMesh, const MContext *pContext, const MVector3& position);

		/**
		��ǥ�� ��ȿ�� �˻� �� ���� ����
		*/
		bool IsValidPosition(SPathMesh* pPathMesh, const MVector3& position);

		PATHENGINE_RESULT_TYPE	GetHeightAtPosition(float *height, SPathMesh* pPathMesh, const MVector3& position);
		static PATHENGINE_RESULT_TYPE	GetHeightAtPosition(float *height, SPathMesh* pPathMesh, const cPosition& position);

		/**
		 �α��� ��ȿ�� ��ǥ�� ã�´�.

		 @param pOut ã�Ƴ� ��ǥ(3D)�� ������ MVector3 ������
		 @param nID Mesh ID
		 @param	pContext	�浹 ���� ( �� ���� NULL �� �ƴϸ�, context �� ���ϵ� agent�� ���� �浹�˻絵 ���Ѵ� )
		 @param position ���� ��ǥ
		 @param nHorizonRange	X,Y ����(+-)�� �α� ��ǥ�� ã�´�. 
		 @param nVerticalRange	Z ����(+-)�� ��ǥ�� ã�´�.
		 @param strShape Shape ��Ʈ��, ���� small/medium/large ������

		 @return ã�µ� �����ϸ� true�� ����, �����ϸ� false�� ����
		 */
		PATHENGINE_RESULT_TYPE	FindNearPosition(MVector3 *pOut, SPathMesh* pPathMesh, const MContext *pContext, const MVector3& position
			,int nHorizonRange, int nVerticalRange);

		// true�� �����ϸ� ���������� MAgent�� ���� ���̴�.
		bool					NewPlaceAgent(MAgent &agent, const MVector3& position, int nZoneID, SPathMesh* pPathMesh);

		// true�� �����ϸ� ���������� MContext�� ���� ���̴�.
		bool					NewContext(MContext &context, SPathMesh* pPathMesh);
		
		// PATHENGINE_RESULT_TYPE �� ����, SUCCESS = 0 �� �ƴϸ� ���� �߸��� ���̴�.
		PATHENGINE_RESULT_TYPE	AdvancedAlongPath(MCollisionInfo *pCollisionInfoOut, int nID, MAgent &agent, MPath &path, float fDistance);
		
		// ???
		float					FindPath(SPathMesh* pPathMesh, const MVector3 &vStart, const MVector3 &vEnd, vector<MVector3>& vPath);


		// vCenter �߽� fSize ������ ���簢�� ������ Ż���ϴ� �ִܰ��
		// ��ȯ�� : ��� ����
		float					FindPathAway(SPathMesh* pPathMesh, const MVector3& vStart, const MVector3& vCenter, float fSize, int nFreq, int nCount, vector<MVector3>& vPath);

public:

		// MPath�� �������� 3D ���� �迭�� �����´�. ũ�� �ʿ� ���� �� �ʹ�.
		void					PathTo3DPoints(std::vector<MVector3> &out3DPoints, SPathMesh* pPathMesh, MPath &basePath, int offset = 0);
		void					PathTo3DPoint(MVector3 &outPos, SPathMesh* pPathMesh, MPath &basePath, int index);

		GPathFinderImpl();
		~GPathFinderImpl();
		
};
