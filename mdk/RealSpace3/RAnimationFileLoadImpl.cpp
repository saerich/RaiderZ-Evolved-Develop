#include "StdAfx.h"
#include "RAnimationFileLoadImpl.h"
#include "RAnimationNode.h"
#include "RD3DVertexUtil.h"
#include <d3dx9.h>
#include "RFile.h"

namespace rs3 {



#define RFILEREAD1(str)															\
{	int _size = 0;																\
	if ( rf.Read( &_size, sizeof( int)) == false)	return false;				\
	str.resize( _size);															\
	if ( rf.Read( &str[ 0], _size) == false)		return false;				\
	if ( _size > 0  &&  str[ _size - 1] == 0)		str.resize( _size - 1);		\
}

#define RFILEREAD2(buff,read)													\
{	if ( rf.Read( (buff), (read)) == false)			return false;				\
}


//////////////////////////////////////////////////////////////////////////
//
void RAnimationFileLoadImpl_v6::LoadVertexAniBoundingBox(RFile &rf, RAnimationNode *pANode)
{
	pANode->m_bboxVertexAni.Initialize();
	for ( int j = 0; j<pANode->m_nVertexCnt; ++j )
	{
		for ( int i = 0; i<pANode->m_nVertexVCnt; ++i )
			pANode->m_bboxVertexAni.Add(pANode->m_vertex[j][i]);
	}
}

bool RAnimationFileLoadImpl_v6::LoadVertexAni(RFile &rf, RAnimationNode *pANode, DWORD dwVersion)
{
	RFILEREAD1( pANode->m_Name);
	RFILEREAD2( &pANode->m_nVertexCnt, 4);	// vertex group (mesh-node) count
	if ( pANode->m_nVertexCnt )
		pANode->m_vertex = new RVector*[pANode->m_nVertexCnt];

	int vcnt = 0;	// vertex point count
	RFILEREAD2( &vcnt, 4);
	pANode->m_nVertexVCnt = vcnt;

	if ( pANode->m_nVertexCnt )
		pANode->m_pVertexFrame = new DWORD[pANode->m_nVertexCnt];

	RFILEREAD2( pANode->m_pVertexFrame, sizeof(DWORD)*pANode->m_nVertexCnt);
	for (int j = 0; j<pANode->m_nVertexCnt; ++j)
	{
		pANode->m_vertex[j] = new RVector[vcnt];
		RFILEREAD2( pANode->m_vertex[j], sizeof(RVector)*vcnt);
	}

	LoadVertexAniBoundingBox(rf, pANode);

	return true;
}

bool RAnimationFileLoadImpl_v6::LoadBoneAni(RFile &rf, RAnimationNode *pANode, DWORD dwVersion)
{
	RFILEREAD1( pANode->m_Name);
	if (dwVersion>=EXPORTER_ANI_VER6)
		RFILEREAD1( pANode->m_parentName);

	RFILEREAD2( &pANode->m_matLocal, sizeof(D3DXMATRIX));


	// �ʱ� �����ӿ� ���� �ִϸ��̼� Ű <- �� �Ĺ���!! �̷��͵� �־��� ������ ������ �Ⱦ��̴� �� - 090709, COMMENT OZ
	// �Ⱦ��µ� �Ͽ� ����ϴ�. - birdkr 2010/05/07
	//pANode->m_matLocal.Decompose(pANode->m_baseScale,pANode->m_baseTranslation,pANode->m_baseRotation);

	int pos_key_num = 0;
	int rot_key_num = 0;

	RFILEREAD2( &pos_key_num, 4);
	if ( pos_key_num )
	{
		pANode->m_positionKeyTrack.Create(pos_key_num+1);
		RFILEREAD2( (void*)&pANode->m_positionKeyTrack[0],sizeof(RPosKey)*pos_key_num);

		pANode->m_positionKeyTrack[pos_key_num] = pANode->m_positionKeyTrack[pos_key_num-1];

	}

	rf.Read(&rot_key_num, 4);
	if ( rot_key_num )
	{
		pANode->m_rotationKeyTrack.Create(rot_key_num+1);
		RFILEREAD2( (void*)&pANode->m_rotationKeyTrack[0],sizeof(RQuatKey)*rot_key_num);
		pANode->m_rotationKeyTrack[rot_key_num] = pANode->m_rotationKeyTrack[rot_key_num-1];

	}

	if ( dwVersion>=EXPORTER_ANI_VER5)
	{
		int nScaleCnt = 0;
		RFILEREAD2( &nScaleCnt,sizeof(DWORD));
		if(nScaleCnt) 
		{
			pANode->m_scaleKeyTrack.Create(nScaleCnt);
			RFILEREAD2( (void*)&pANode->m_scaleKeyTrack[0],sizeof(RScaleKey)*nScaleCnt);
		}
	}

	return true;
}

bool RAnimationFileLoadImpl_v6::LoadVisibilityKey(RFile &rf, RAnimationNode *pANode, DWORD dwVersion)
{
	if( dwVersion>=EXPORTER_ANI_VER5)
	{
		int nVisCnt = 0;
		RFILEREAD2( &nVisCnt, 4);

		if(nVisCnt) 
		{
			pANode->m_visKeyTrack.Create(nVisCnt);
			RFILEREAD2( (void*)&pANode->m_visKeyTrack[0],sizeof(RVisKey)*nVisCnt);

			// Visiblity���� 0���� ���� �� �ִ�. �ƽ����� Off�뵵�� 0���� �������� ���.
			for( int i = 0; i < nVisCnt; ++i )
			{
				if( pANode->m_visKeyTrack[i].v < 0.0f )
					pANode->m_visKeyTrack[i].v = 0.0f;
			}

			return true;
		}
	}
	else
	{
		int nVisCnt = 0;
		RFILEREAD2( &nVisCnt, 4);

		struct ROldVisKey 
		{
			float v;
			int frame;
		} oldKey;

		if(nVisCnt) 
		{
			pANode->m_visKeyTrack.Create(nVisCnt);
			for( int i=0; i<nVisCnt; i++)
			{
				// Visiblity���� 0���� ���� �� �ִ�. �ƽ����� Off�뵵�� 0���� �������� ���.
				RFILEREAD2( &oldKey,sizeof(ROldVisKey));
				if( oldKey.v < 0.0f )
					oldKey.v = 0.0f;

				pANode->m_visKeyTrack[i].frame = oldKey.frame;
				pANode->m_visKeyTrack[i].v = oldKey.v;
			}

			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
//
void RAnimationFileLoadImpl_v7::LoadVertexAniBoundingBox(RFile &rf, RAnimationNode *pANode)
{
	RBoundingBox *pAABB = &pANode->m_bboxVertexAni;
	rf.Read( pAABB->vmin.v, sizeof(float)*3);
	rf.Read( pAABB->vmax.v, sizeof(float)*3);
}

//////////////////////////////////////////////////////////////////////////
//
bool RAnimationFileLoadImpl_v9::LoadVisibilityKey(RFile &rf, RAnimationNode *pANode, DWORD dwVersion)
{
	int nVisCnt = 0;
	RFILEREAD2( &nVisCnt, 4);

	if(nVisCnt) 
	{
		pANode->m_visKeyTrack.Create(nVisCnt);
		RFILEREAD2( (void*)&pANode->m_visKeyTrack[0],sizeof(RVisKey)*nVisCnt);

		return true;
	}

	return false;
};


};