// eluValidator.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "shlwapi.h"
#include "MDebug.h"
#include "RMesh.h"
#include "RMeshMgr.h"
#include "REngine.h"
#include "RDeviceD3D.h"
#include "RSceneManager.h"
#include "MFileSystem.h"

using namespace std;
using namespace rs3;

char g_szModuleFileName[_MAX_DIR] = {0,};

bool ValidateEluFileVersion( RMesh* pMesh )
{
	_ASSERT( pMesh );
	_ASSERT( pMesh->GetFileVersion() <= EXPORTER_CURRENT_MESH_VER );

	if( pMesh->GetFileVersion() < EXPORTER_CURRENT_MESH_VER )
	{
		mlog( "\t�ֽ� ���� ������ �ƴ�.\n",
			pMesh->GetName().c_str(), pMesh->GetFileVersion(), EXPORTER_CURRENT_MESH_VER );

		return false;
	}

	return true;
}

bool IsMeshUseMaterial( RMesh* pMesh, RMaterial* pMaterial )
{
	_ASSERT( pMesh );
	_ASSERT( pMaterial );

	const size_t nodeCount = pMesh->GetNodeCount();
	if( 0 == nodeCount )
		return true;

	for( size_t i = 0; i < nodeCount; ++i )
	{
		RMeshNode* pMeshNode = pMesh->GetNode( i );

		size_t materialCount = pMeshNode->GetMtrlCount();
		for( size_t j = 0; j < materialCount; ++j )
		{
			if( pMeshNode->GetMtrl(j) == pMaterial )
				return true;
		}
	}

	return false;
}

// �޽ÿ��� ���׸��� ���ҽ��� ������ ����ϴ��� üũ�Ѵ�
unsigned int g_countValidateEluUsingMaterialFailed = 0;
bool ValidateEluUsingMaterial( RMesh* pMesh )
{
	_ASSERT( pMesh );

	bool bValid = true;

	size_t meshMaterialCount = pMesh->GetMaterialResourceCount();
	for( size_t i = 0; i < meshMaterialCount; ++i )
	{
		RMaterial* pMaterial = pMesh->GetMaterialResource(i)->m_pMaterial;
		if( false == IsMeshUseMaterial( pMesh, pMaterial ) )
		{
			if( bValid )
			{
				g_countValidateEluUsingMaterialFailed++;
				mlog( "### %s\n", pMesh->GetName().c_str() );
			}
			bValid = false;

			mlog( "\t!! ������� �ʴ� ���׸��� %s�� �ֽ��ϴ�.\n", pMaterial->Name.c_str() );
		}
	}

	if( false == bValid )
		mlog( "\n" );

	return bValid;
}

// �޽� ��尡 2�� �̻��� ���׸����� ����ϴ��� üũ.
bool IsMeshUseMultiSubMaterial( RMesh* pMesh )
{
	_ASSERT( pMesh );

	const size_t nodeCount = pMesh->GetNodeCount();
	if( 0 == nodeCount )
		return false;

	bool bUseMultiSubMaterial = false;

	for( size_t i = 0; i < nodeCount; ++i )
	{
		RMeshNode* pMeshNode = pMesh->GetNode( i );

		size_t materialCount = pMeshNode->GetMtrlCount();
		if( materialCount > 1 )
		{
			if( false == bUseMultiSubMaterial )
			{
				mlog( "### %s - ��Ƽ/���� ���׸����� ���.\n", pMesh->GetName().c_str() );
			}
			bUseMultiSubMaterial = true;

			mlog( "\t ���� ��� %s\n", pMeshNode->GetName() );
			for( int j = 0; j < pMeshNode->GetMtrlCount(); ++j )
			{
				mlog( "\t\t ���׸��� %s\n", pMeshNode->GetMtrl( j )->Name.c_str() );
			}
		}
	}

	if( true == bUseMultiSubMaterial )
		mlog( "\n" );

	return bUseMultiSubMaterial;
}

// HIDE �� �浹 �޽� ��尡 �ִ��� üũ
bool IsMeshExistHidingCollisionMeshNode( RMesh* pMesh )
{
	_ASSERT( pMesh );

	const size_t nodeCount = pMesh->GetNodeCount();
	if( 0 == nodeCount )
		return false;

	bool bExist = false;

	for( size_t i = 0; i < nodeCount; ++i )
	{
		RMeshNode* pMeshNode = pMesh->GetNode( i );

		if( false == pMeshNode->isVisibleMesh() && pMeshNode->isCollisionMesh() )
		{
			if( false == bExist )
			{
				mlog( "### %s - ������ �浹 �޽ó�尡 ����.\n", pMesh->GetName().c_str() );
			}
			bExist = true;

			mlog( "\t ������ �浹 �޽� ��� %s\n", pMeshNode->GetName() );
		}
	}

	if( true == bExist )
		mlog( "\n" );

	return bExist;
}

void CreateAndValidateMeshFile( const string& fileName )
{
	RMesh* pMesh = REngine::GetSceneManager().CreateResource<RMesh>( fileName.c_str() );

	if( pMesh->BuildAll() )
	{
	//	ValidateEluFileVersion( pMesh );
		ValidateEluUsingMaterial( pMesh );
	//	IsMeshUseMultiSubMaterial( pMesh );
	//	IsMeshExistHidingCollisionMeshNode( pMesh );
	}
	else
	{
		mlog( "!!! %s ���� �ε� ����\n", fileName.c_str() );
	}

	REngine::GetSceneManager().ReleaseResource( pMesh );
}

void ValidateMeshFiles( const string& strSubDir )
{
	string filter = strSubDir;
	filter += "*";

	bool bResult = true;
	WIN32_FIND_DATA wfd;
	HANDLE hSrch = FindFirstFile( filter.c_str(), &wfd );
	if( hSrch == INVALID_HANDLE_VALUE )
		bResult = false;

	while( bResult )
	{
		if( (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == false )
		{
			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( (strcmp(wfd.cFileName, ".") != 0 ) && (strcmp(wfd.cFileName, "..") != 0 ) )
				{
					string strNewPath( strSubDir );

					if( strSubDir.empty() == false )
						strNewPath += "\\";

					strNewPath += wfd.cFileName;
					strNewPath += "\\";
					ValidateMeshFiles( strNewPath );
				}
			}
			else
			{
				static char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
				_splitpath_s( wfd.cFileName, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );

				if( _strnicmp( ext, ".elu", 4 ) == 0 )
				{
					string strMeshFileName( g_szModuleFileName );
					strMeshFileName += "\\";
					strMeshFileName += strSubDir;
					strMeshFileName += "\\";
					strMeshFileName += wfd.cFileName;
					CreateAndValidateMeshFile( strMeshFileName );
				}
			}
		}

		bResult = FindNextFile(hSrch, &wfd) ? true : false;
	}
	FindClose(hSrch);
}

bool Init()
{
	// ����̽� ����
	HWND hWnd = GetDesktopWindow();
	RDeviceD3D* pDevice = new RDeviceD3D;
	bool bRet = pDevice->Create(hWnd);
	if(!bRet)
	{
		_ASSERT(false);
		delete pDevice;
		return false;
	}
	REngine::RegisterDevice(pDevice);

	// �� �Ŵ��� ����
	RSceneManager* pSceneManager = new RSceneManager;
	REngine::RegisterSceneManager(pSceneManager);
	bRet = pSceneManager->Create(pDevice);
	_ASSERT(bRet == true);
//	bRet = REngine::InitSharedResource();
	if(!bRet)
	{
		return false;
	}
	_ASSERT(bRet == true);
	pSceneManager->SetSSAO( false);

	if(!RMesh::InitSharedResources(REngine::GetDevicePtr()))
		return false;

	return true;
}

void Destroy()
{
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();
	_ASSERT(pSceneManager != NULL);

	// ���Ŵ���
	pSceneManager->Clear();

	RMesh::DestroySharedResources();
//	REngine::DestroySharedResource();
	pSceneManager->Destroy();
	delete pSceneManager;
	REngine::RegisterSceneManager(NULL);

	// ����̽� ����
	RDevice* pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice != NULL);
	pDevice->Destroy();
	delete pDevice;
	REngine::RegisterDevice(NULL);
}

int _tmain(int argc, _TCHAR* argv[])
{
	GetModuleFileName(NULL, g_szModuleFileName, _MAX_DIR);
	// Current Directory�� ����
	PathRemoveFileSpec(g_szModuleFileName);
	SetCurrentDirectory(g_szModuleFileName);

	if( REngine::IsDevelopVersion() )
		MCreateFileSystem( MFILEACCESS_GENERIC, "EngineResDev;Data" );
	else
		MCreateFileSystem( MFILEACCESS_GENERIC, "EngineRes;Data" );

	MInitLog( MLOG_LEVEL_DEBUG, MLOG_FILE | MLOG_DEBUG | MLOG_CONSOLE, NULL, "eluValidator.txt" );

	if( Init() == false )
		return 1;

	ValidateMeshFiles( string( "Data\\" ) );
	mlog( "���׸��� ���ҽ��� �ùٸ��� ���� elu������ ���� - %d\n", g_countValidateEluUsingMaterialFailed );

	Destroy();
	return 0;
}