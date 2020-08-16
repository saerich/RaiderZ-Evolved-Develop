#ifndef _RMeshMgr_h
#define _RMeshMgr_h

#include "MDebug.h"

#include "RMesh.h"
#include <vector>
#include "RResourceManager.h"

#include "RResourceID.h"

namespace rs3 {

class RS_API RMeshMgr : public RResourceManager
{
public:

	// ���ҽ� �Ŵ��� Ÿ�� ����
	DECLARE_RES_MGR_ID( RRESOURCETYPE::RR_MESH );


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	RMeshMgr();
	virtual ~RMeshMgr();

	virtual void				Init( RSceneManager* pSceneManager ){ m_pSceneManager = pSceneManager; }
	virtual void				Destroy();
};

}

#endif//_RMeshMgr_h
