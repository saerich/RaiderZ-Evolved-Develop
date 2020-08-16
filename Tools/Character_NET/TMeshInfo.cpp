#include "stdafx.h"
#include "TMeshInfo.h"
#include "MXml.h"
#include "XMeshInfoLoader.h"
#include "XMeshInfoSaver.h"
#include "TFileNameSampling.h"

using namespace rs3;
using namespace System::IO;

TMeshInfoMgr::TMeshInfoMgr()
{
	m_pMeshHitInfo		= NULL;
	m_pMeshColInfo		= NULL;
	m_pMeshAniInfo		= NULL;
	m_pMeshHitEffectInfo = NULL;

	m_pMeshHitInfo		= new TMeshHitInfoMgr(this);
	m_pMeshColInfo		= new TMeshColInfoMgr(this);
	m_pMeshAniInfo		= new TMeshAniInfoMgr(this);
	m_pMeshHitEffectInfo = new TMeshHitEffectInfoMgr(this);
}

TMeshInfoMgr::~TMeshInfoMgr()
{
	SAFE_DELETE(m_pMeshHitInfo);
	SAFE_DELETE(m_pMeshColInfo);
	SAFE_DELETE(m_pMeshAniInfo);
	SAFE_DELETE(m_pMeshHitEffectInfo);

	m_vecFileList.clear();
}

TMeshInfoMgr& TMeshInfoMgr::GetInstance()
{
	static TMeshInfoMgr instance;
	return instance;
}

void TMeshInfoMgr::LoadMeshInfo()
{
	XMeshInfoLoader meshInfoLoader;
	meshInfoLoader.LoadMeshInfo(this, SERVER_FILENAME_TOOL_MESH_INFO);
}

#define  MESHINFO_XML_TAG_MESHINFO		"MeshInfo"

void TMeshInfoMgr::Save()
{
	//------------------------------------------------------------------------
	// ���� ����Ʈ ��������
	vector<FILE_LIST>& vFileList = GetModelFileList();

	//------------------------------------------------------------------------
	// ������ ����
	for(vector<FILE_LIST>::iterator itFile = vFileList.begin(); itFile != vFileList.end(); itFile++)
	{
		RefreshMeshInfo((*itFile).strPureFileName);
	}

	//------------------------------------------------------------------------
	// ����
	XMeshInfoSaver meshInfoSaver;
	meshInfoSaver.SaveMeshInfo(this, FILENAME_TOOL_MESH_INFO);

	// �������� ���Ƿ� ����
	File::Copy(FILENAME_TOOL_MESH_INFO, SERVER_FILENAME_TOOL_MESH_INFO, true);
}

vector<FILE_LIST>& TMeshInfoMgr::GetModelFileList()
{
	if(m_vecFileList.size() <= 0)
	{
		//------------------------------------------------------------------------
		// ���� ����Ʈ ��������
		TFileNameSampling::GetFileList("../../Data/Model/Monster/", m_vecFileList, NULL, NULL, FA_DIRECTORY);
		TFileNameSampling::GetFileList("../../Data/Model/NPC/", m_vecFileList, NULL, NULL, FA_DIRECTORY);
		TFileNameSampling::GetFileList("../../Data/Model/Player/", m_vecFileList, NULL, NULL, FA_DIRECTORY);
		TFileNameSampling::GetFileList("../../Data/Model/MapObject/", m_vecFileList, NULL, NULL, FA_FILE);
	}

	return m_vecFileList;
}

bool TMeshInfoMgr::RefreshMeshInfo(string strModelName)
{
	if(UseModelHitColCapsule(strModelName))
	{
		bool bHitRefresh = m_pMeshHitInfo->RefreshMeshHitInfo(strModelName);
		bool bColRefresh = m_pMeshColInfo->RefreshMeshColInfo(strModelName);
		bool bAniRefresh = m_pMeshAniInfo->RefreshMeshAniInfo(strModelName, GetModelDirPath(strModelName));

		if (bHitRefresh || bColRefresh || bAniRefresh)
			return true;
	}

	return false;
}

bool TMeshInfoMgr::CheckModelList( string strModelName )
{
	vector<FILE_LIST>& vFileList = GetModelFileList();
	for(vector<FILE_LIST>::iterator itFileList = vFileList.begin(); itFileList != vFileList.end(); ++itFileList)
	{
		if(itFileList->strPureFileName == strModelName)
		{
			return true;
		}
	}

	return false;
}


bool TMeshInfoMgr::UseModelHitColCapsule( string strModelName )
{
	// �� �𵨸���Ʈ�� �ִ°�?
	if(CheckModelList(strModelName))
	{
		return true;
	}

	return false;
}

string TMeshInfoMgr::GetModelDirPath( string strModelName )
{
	//------------------------------------------------------------------------
	// ���� ����Ʈ ��������
	vector<FILE_LIST>& vFileList = GetModelFileList();

	//------------------------------------------------------------------------
	// üũ
	for(vector<FILE_LIST>::iterator itFile = vFileList.begin(); itFile != vFileList.end(); itFile++)
	{
		if((*itFile).strPureFileName == strModelName)
		{
			return (*itFile).strPath;
		}
	}

	return "";
}

bool TMeshInfoMgr::CreateMeshInfo( string strModelName )
{
	if(GetInfo(strModelName) != NULL)
	{
		// �̹� �����Ѵ�.
		return false;
	}

	CSMeshInfo info;
	info.m_strMeshName = strModelName;
	m_mapMeshInfo.insert(map<string, CSMeshInfo>::value_type(strModelName, info));

	return true;
}

CSMeshInfo* TMeshInfoMgr::GetMeshInfo( const string& key )
{
	return GetInfo(key);
}