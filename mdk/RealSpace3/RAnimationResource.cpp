#include "StdAfx.h"
#include "RAnimationResource.h"
#include "RResourceID.h"
#include "RAnimationNode.h"
#include "RAnimationFileLoadImpl.h"

namespace rs3 {

/////////////////////////////////////////////////////////////////////////

RAnimationResource::RAnimationResource(const char* _pSzResourceName, RSceneManager* pSceneManager)
: RResource( _pSzResourceName, RRESOURCETYPE::RR_ANIMATION )
{
	//	m_isLoadDone = false;

	m_pAniNodeTable	= NULL;
	m_nAniNodeCnt	= 0;
	m_AniType		= RAniType_Bone;
	m_pBipRootNode	= NULL;

	m_nMaxFrame		= 0;
	m_pMaxAniBoundingBox = NULL;

	m_bheaderLoadComplete	= false;
}

RAnimationResource::~RAnimationResource() 
{
	SAFE_DELETE(m_pMaxAniBoundingBox);

	if( m_pAniNodeTable ) 
	{
		for(int i=0;i<m_nAniNodeCnt;i++) 
			delete m_pAniNodeTable[i];

		delete[] m_pAniNodeTable;
		m_pAniNodeTable = NULL;
	}
}

// void RAnimationResource::SetFileName(const char* name)
// {
// 	m_filename = name;
// }
// 
// char* RAnimationResource::GetFileName()
// {
// 	return (char*)m_filename.c_str();
// }

// void RAnimationResource::SetLoadDone(bool b) 
// {
// 	m_isLoadDone = b;
// }
// 
// bool RAnimationResource::IsLoadDone() 
// {
// 	return m_isLoadDone;
// }

// bool RAnimationResource::LoadAni(const char* filename)
// {
// 	RAnimationFile* pAnimationFile = RGetAnimationFileMgr()->Add(filename);
// 
// 	if(pAnimationFile==NULL ) 
// 		return false;
// 
// 	m_pAniData = pAnimationFile;
// 
// 	return true;
// }

bool RAnimationResource::Fill()
{
	bool ret = LoadAni( GetName().c_str() );

	return ret;
}

RAnimationNode* RAnimationResource::GetNode(const string& strName)
{
	/*
	for(int i=0;i<m_nAniNodeCnt;i++) 
	{
	if(m_pAniNodeTable[i]->CheckName(name))
	return m_pAniNodeTable[i];
	}
	*/

	int nIndex = GetNodeIndex(strName);
	if(nIndex<0 || nIndex>=m_nAniNodeCnt)
		return NULL;

	return m_pAniNodeTable[nIndex];
}

bool RAnimationResource::LoadAni(const char* filename)
{
	RFile rf;

	if(!rf.Open(filename)) 
	{
		mlog_tool("%s file not found!! \n", filename );
		return false;
	}


	bool bResult = false;

	// �ִϸ��̼� ��� �ε�
	bResult = LoadAniHeader(rf, filename);

	if(bResult)
	{
		// �ִϸ��̼� ��� �ε�
		bResult = LoadAniNode(rf, filename);
	}


	rf.Close();


	return bResult;
}

bool RAnimationResource::SaveAni()
{
	FILE *fp = NULL;
	if ( fopen_s(&fp, GetName().c_str(), "wb") != 0 )
	{
		mlog("cant' save .ani file\n");
		return false;
	}

	m_header.ver = EXPORTER_CURRENT_ANI_VER;
	fwrite(&m_header, sizeof(ex_ani_t), 1, fp); 

	if( m_header.ani_type == RAniType_Vertex) 
	{//Vertex Animation

		RAnimationNode* pANode = NULL;
		for( int i = 0; i < m_nAniNodeCnt; i++ ) 
		{ // node ŭ

			pANode = m_pAniNodeTable[i];

			int nSize = int(pANode->m_Name.size() + 1);
			fwrite(&nSize, sizeof(int), 1, fp);
			if(nSize)
				fwrite(pANode->m_Name.c_str(), nSize, 1, fp);

			fwrite(&pANode->m_nVertexCnt, sizeof(pANode->m_nVertexCnt), 1, fp);
			fwrite(&pANode->m_nVertexVCnt, sizeof(pANode->m_nVertexVCnt), 1, fp);
			fwrite(pANode->m_pVertexFrame, sizeof(DWORD) * pANode->m_nVertexCnt, 1, fp);

			for (int j = 0; j<pANode->m_nVertexCnt; ++j)
			{
				fwrite(pANode->m_vertex[j], sizeof(RVector)*pANode->m_nVertexVCnt, 1, fp);
			}

			// AABB ����
			RBoundingBox *pAABB = &pANode->m_bboxVertexAni;
			fwrite(&pAABB->vmin.v, sizeof(float)*3, 1, fp);
			fwrite(&pAABB->vmax.v, sizeof(float)*3, 1, fp);

			int nTrackSize = (int)pANode->m_visKeyTrack.size();
			fwrite(&nTrackSize, sizeof(nTrackSize), 1, fp);
			if(nTrackSize)
				fwrite(&pANode->m_visKeyTrack[0], sizeof(RVisKey) * pANode->m_visKeyTrack.size(), 1, fp);
		}
	} 
	else if( m_header.ani_type == RAniType_Bone )
	{// trans + bone

		RAnimationNode* pANode = NULL;
		for( int i = 0; i < m_nAniNodeCnt; i++ ) 
		{ // node ŭ

			pANode = m_pAniNodeTable[i];

			int nSize = int(pANode->m_Name.size() + 1);
			fwrite(&nSize, sizeof(int), 1, fp);
			if(nSize)
				fwrite(pANode->m_Name.c_str(), nSize, 1, fp);

			nSize = int(pANode->m_parentName.size() + 1);
			fwrite(&nSize, sizeof(int), 1, fp);
			if(nSize)
				fwrite(pANode->m_parentName.c_str(), nSize, 1, fp);

			fwrite(&pANode->m_matLocal, sizeof(RMatrix), 1, fp);

			int nPosKeySize = (int)pANode->m_positionKeyTrack.size();
			fwrite(&nPosKeySize, sizeof(nPosKeySize), 1, fp);
			fwrite(&pANode->m_positionKeyTrack[0], sizeof(RPosKey) * pANode->m_positionKeyTrack.size(), 1, fp);

			int nRotKeySize = (int)pANode->m_rotationKeyTrack.size();
			fwrite(&nRotKeySize, sizeof(nRotKeySize), 1, fp);
			fwrite(&pANode->m_rotationKeyTrack[0], sizeof(RQuatKey) * pANode->m_rotationKeyTrack.size(), 1, fp);

			int nScaleSize = (int)pANode->m_scaleKeyTrack.size();
			fwrite(&nScaleSize, sizeof(nScaleSize), 1, fp);
			fwrite(&pANode->m_scaleKeyTrack[0], sizeof(RScaleKey) * pANode->m_scaleKeyTrack.size(), 1, fp);

			int nTrackSize = (int)pANode->m_visKeyTrack.size();
			fwrite(&nTrackSize, sizeof(nTrackSize), 1, fp);
			if(nTrackSize)
				fwrite(&pANode->m_visKeyTrack[0], sizeof(RVisKey) * pANode->m_visKeyTrack.size(), 1, fp);
		}
	}

	// Max Ani AABB - EXPORTER_ANI_VER8 �߰� ����
	fwrite(&m_pMaxAniBoundingBox->vmin.v, sizeof(float)*3, 1, fp);
	fwrite(&m_pMaxAniBoundingBox->vmax.v, sizeof(float)*3, 1, fp);

	fclose(fp);

	return true;
}

bool RAnimationResource::SetMaxAniBoundingBox(RBoundingBox& box)
{
	if(m_pMaxAniBoundingBox == NULL)
		m_pMaxAniBoundingBox = new RBoundingBox;

	(*m_pMaxAniBoundingBox) = box;

	return true;
}


// ���� ���˰��� ȣȯ������...
bool RAnimationResource::LoadVisibilityKey(RFile& rf, RAnimationNode* pANode)
{
	if(GetFileVersion()>=EXPORTER_ANI_VER5)
	{
		int nVisCnt;
		rf.Read(&nVisCnt,sizeof(DWORD) );

		if(nVisCnt) {
			//			pANode->m_visKeyTrack.resize(nVisCnt);
			pANode->m_visKeyTrack.Create(nVisCnt);
			rf.Read((void*)&pANode->m_visKeyTrack[0],sizeof(RVisKey)*nVisCnt);
			return true;
		}
	}else
	{
		int nVisCnt;
		rf.Read(&nVisCnt,sizeof(DWORD) );

		struct ROldVisKey {
			float v;
			int frame;
		} oldKey;
		if(nVisCnt) {
			//			pANode->m_visKeyTrack.reserve(nVisCnt);
			pANode->m_visKeyTrack.Create(nVisCnt);
			for(int i=0;i<nVisCnt;i++)
			{
				rf.Read(&oldKey,sizeof(ROldVisKey));
				pANode->m_visKeyTrack[i].frame = oldKey.frame;
				pANode->m_visKeyTrack[i].v = oldKey.v;
			}
			return true;
		}
	}
	return false;
}

int RAnimationResource::GetNodeIndex( const string& strName )
{
	std::unordered_map<string,int>::iterator itr = m_nodeIndexMap.find(strName);
	if(m_nodeIndexMap.end()!=itr)
		return itr->second;

	return -1;
}

bool RAnimationResource::LoadAniHeader( RFile& rf, const char* filename )
{
	rf.Read(&m_header, sizeof(ex_ani_t));

	if(m_header.ver!=EXPORTER_CURRENT_ANI_VER)
	{
		mlog_tool("%s is not latest file version\n",filename);
	}

	// 0,2 �� ���� ���� tm ��������
	// 1 �� ���ؽ�
	// 3 �� ��Ʈ���� ���ø�

	m_nAniNodeCnt = m_header.model_num;

	const int TICKSPERFRAME = 160;
	m_nMaxFrame = TICKSPERFRAME * m_header.maxframe;

	if( m_nAniNodeCnt == 0 ) 
	{
		mlog_tool("Ű�� ���� %s ���ϸ��̼� ���\n",filename);
		return false;
	}

	m_AniType = (AnimationType)m_header.ani_type;

	m_bheaderLoadComplete = true;

	return true;
}

bool RAnimationResource::LoadAniNode( RFile& rf, const char* filename )
{
	if(m_bheaderLoadComplete == false)
	{
		mlog_tool("%s �ִϸ��̼� ���� ����� �ε��� �Ǿ� ���� �ʽ��ϴ�.\n",filename);
		return false;
	}

	static RAnimationFileLoadImpl_v6 animFileLoadImpl_v6;
	static RAnimationFileLoadImpl_v7 animFileLoadImpl_v7;
	static RAnimationFileLoadImpl_v9 animFileLoadImpl_v9;

	RAnimationFileLoadImpl *pImpl = NULL;
	if ( m_header.ver == EXPORTER_ANI_VER9 )
		pImpl = &animFileLoadImpl_v9;
	else if ( m_header.ver == EXPORTER_ANI_VER8 )
		pImpl = &animFileLoadImpl_v7;
	else if ( m_header.ver == EXPORTER_ANI_VER7 )
		pImpl = &animFileLoadImpl_v7;
	else if ( m_header.ver <= EXPORTER_ANI_VER6 )
		pImpl = &animFileLoadImpl_v6;
	else if ( m_header.ver > EXPORTER_CURRENT_ANI_VER )
	{
		mlog_tool("%s �ִϸ��̼� ���� ������ ��Ÿ�� �ֽ� �������� �����ϴ�.\n",filename);
		return false;
	}

	m_pAniNodeTable = new RAnimationNode*[m_nAniNodeCnt];
	RAnimationNode* pANode = NULL;

	for ( int i = 0; i<m_nAniNodeCnt; ++i )
	{
		// Node ����
		m_pAniNodeTable[i] = new RAnimationNode;
		pANode = m_pAniNodeTable[i];

		switch ( m_AniType )
		{
		case RAniType_Vertex:
			pImpl->LoadVertexAni(rf, pANode, m_header.ver);
			break;
		case RAniType_Bone:
			pImpl->LoadBoneAni(rf, pANode, m_header.ver);
			if ( strcmp(pANode->GetName(), "Bip01") == 0)
				m_pBipRootNode = pANode;
			break;
		default:
			{
				mlog("mode type �� �߸��Ǿ����ϴ�.\n");
				_ASSERT("mode type �� �߸��Ǿ����ϴ�.");
			}
			
		}


		pImpl->LoadVisibilityKey(rf, pANode, m_header.ver);

		m_nodeIndexMap.insert(std::make_pair(pANode->GetName(), i));
		
	}


	if( m_header.ver >= EXPORTER_ANI_VER8 )	// Max Ani BoundingBox
	{
		m_pMaxAniBoundingBox = new RBoundingBox;
		rf.Read(m_pMaxAniBoundingBox->vmin.v, sizeof(float)*3);
		rf.Read(m_pMaxAniBoundingBox->vmax.v, sizeof(float)*3);

		if(!m_pMaxAniBoundingBox->IsValid())
		{
			m_pMaxAniBoundingBox->vmin = RVector(-100.f, -100.f,-100.f);
			m_pMaxAniBoundingBox->vmax = RVector( 100.f,  100.f, 100.f);
			dlog("Animation AABB is invalid: %s\n", m_strName.c_str());
		}
	}

	return true;
}

}