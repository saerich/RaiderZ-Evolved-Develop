#include "stdafx.h"
#include "RShaderFX.h"
#include "RResourceID.h"
#include "FileInfo.h"

//#define DEBUG_VS  

namespace rs3
{

interface RShaderFileIncluder : public ID3DXInclude
{
	HRESULT __stdcall Open( D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes )
	{
		_ASSERT( NULL == *ppData );
		_ASSERT( 0 == *pBytes );

		MFile file;
		if( false == file.Open( pFileName ) )
		{
			return E_FAIL;
		}

		size_t readSize = file.GetLength();
		size_t bufferSize = readSize + 1;
		char* pBuffer = new char[bufferSize];
		if( NULL == pBuffer )
		{
			return E_OUTOFMEMORY;
		}
		//pBuffer[readSize] = 0;

		*ppData = pBuffer;
		*pBytes = bufferSize;

		if( false == file.ReadAll( pBuffer, readSize ) )
		{
			_ASSERT( false );
			return E_FAIL;
		}

		pBuffer[readSize] = '\n';

		return S_OK;
	}

	HRESULT __stdcall Close( LPCVOID pData )
	{
		delete [] pData;
		return S_OK;
	}
};

///////////////////////////////////////////////////////////////////////  
///////////////////////////////////////////////////////////////////////  

RShaderFX::RShaderFX( LPDIRECT3DDEVICE9 pDevice )
: RResource()
, m_pEffect(NULL)
, m_pd3dDevice(pDevice)
, m_pCompiledEffectBuffer(NULL)
, m_pErrorMsgs(NULL)
, m_dwShaderFlags(0)
, m_pShaderMacros(NULL)
, m_bOpenCompiledShaderFile(false)
, m_pCompiledEffectFileBuffer(NULL)
, m_nCompiledEffectFileBufferSize(0)
{
	m_nResID = RRESOURCETYPE::RR_SHADERFX;
}


///////////////////////////////////////////////////////////////////////  
//  RShaderFX::~RShaderFX

RShaderFX::~RShaderFX( )
{
	//TODO: ref count ó��
	m_nRefCount =0;

	SAFE_DELETE_ARRAY(m_pCompiledEffectFileBuffer);
	SAFE_DELETE_ARRAY(m_pShaderMacros);
	SAFE_RELEASE(m_pCompiledEffectBuffer);
	SAFE_RELEASE(m_pErrorMsgs);
	SAFE_RELEASE(m_pEffect);
}


//////////////////////////////////////////////////////////////////////////
// Resource Loading

// SetFromString �Ǵ� SetFromFile ���߿� �ϳ��κ��� ����
bool RShaderFX::SetFromString(const string& strShader, const vector<string>& vShaderDefines, RShaderFXPool *pShaderPool, const char* szCachingFile)
{
	if(!m_strFilename.empty())
	{
		_ASSERT( false );
		return false;
	}

	m_strShaderString = strShader;
	m_vShaderDefines = vShaderDefines;
	m_pShaderPool = pShaderPool;
	
	if (szCachingFile)
	{
		m_strCachingFileName = REngine::GetConfig().m_strCompiledShaderCachePath + szCachingFile;
	}

	return true;
}

bool RShaderFX::SetFromFile(const string& strFilename, const vector<string>& vShaderDefines, RShaderFXPool *pShaderPool)
{
	if(!m_strShaderString.empty())
	{
		_ASSERT( false );
		return false;
	}

	m_strFilename = strFilename;
	m_vShaderDefines = vShaderDefines;
	m_pShaderPool = pShaderPool;
	
	return true;
}

bool RShaderFX::Fill()
{
	SetupShaderFlags();
	SetupShaderMacros();

	if (IsSettingFromFile())
	{
		if (LoadShaderFile() == false)
		{
			return false;
		}
	}
	else
	{
		if (REngine::GetConfig().m_bUsingCompiledShaderCaching)
		{
			// ���յ� ���̴��ε� ���� ĳ���� �����ϵ� ���̴� ������ ������ �д´�.
			LoadCachingFile();
		}
	}

	if (CompileEffect() == false)
	{
		return false;
	}

	return true;
}

RShaderFX::LOAD_RESULT RShaderFX::Load()
{
	LPD3DXEFFECTPOOL pEffectPool = (m_pShaderPool != NULL ? m_pShaderPool->GetEffectPool() : NULL );

	RShaderFileIncluder shaderFileIncluder;

#ifdef DEBUG_VS
	
	// ���Ͽ��� �����ϴ� ��쿡�� ������� ����
	if( m_strFullFilename.size() )
	{
		// PIX���� *.fx ���� ������� �ϱ� ���� D3DXCreateEffectFromFile �Լ��� ������ �о� ���δ�.
		if (FAILED(::D3DXCreateEffectFromFile(m_pd3dDevice, m_strFullFilename.c_str(), m_pShaderMacros, &shaderFileIncluder, m_dwShaderFlags, pEffectPool, &m_pEffect, &m_pErrorMsgs) ) )
		{
			if (m_pErrorMsgs)
			{
				const char* pCompileErrors = (const char*)m_pErrorMsgs->GetBufferPointer( );
				mlog(pCompileErrors);
				SAFE_RELEASE(m_pErrorMsgs);
			}
			return WRONG;
			_ASSERT(false);
		}
	}else
	{
		if (FAILED(D3DXCreateEffect(m_pd3dDevice, m_strShaderString.c_str(), m_strShaderString.length() + 1, 
			m_pShaderMacros, &shaderFileIncluder, m_dwShaderFlags, pEffectPool, &m_pEffect, &m_pErrorMsgs)) )
		{
			if (m_pErrorMsgs)
			{
				const char* pCompileErrors = (const char*)m_pErrorMsgs->GetBufferPointer( );
				mlog(pCompileErrors);
				SAFE_RELEASE(m_pErrorMsgs);
			}
			_ASSERT(false);
			SAFE_DELETE_ARRAY(m_pShaderMacros);
			return WRONG;
		}
	}
#else
	LPCVOID pSrcData;
	UINT nSrcDataLen;

	if (m_bOpenCompiledShaderFile)
	{
		pSrcData = m_pCompiledEffectFileBuffer;
		nSrcDataLen = m_nCompiledEffectFileBufferSize;
	}
	else
	{
		pSrcData = m_pCompiledEffectBuffer->GetBufferPointer();
		nSrcDataLen = m_pCompiledEffectBuffer->GetBufferSize();
	}


	if (FAILED(D3DXCreateEffect(m_pd3dDevice, pSrcData, nSrcDataLen, 
											m_pShaderMacros, NULL, m_dwShaderFlags, pEffectPool, &m_pEffect, &m_pErrorMsgs)) )
	{
		if (m_pErrorMsgs)
		{
			const char* pCompileErrors = (const char*)m_pErrorMsgs->GetBufferPointer( );
			mlog(pCompileErrors);
			SAFE_RELEASE(m_pErrorMsgs);
		}
		_ASSERT(false);
		SAFE_DELETE_ARRAY(m_pShaderMacros);
		SAFE_RELEASE(m_pCompiledEffectBuffer);
		SAFE_DELETE_ARRAY(m_pCompiledEffectFileBuffer);
		return WRONG;
	}

#endif

	SAFE_DELETE_ARRAY(m_pShaderMacros);
	SAFE_RELEASE(m_pCompiledEffectBuffer);
	SAFE_DELETE_ARRAY(m_pCompiledEffectFileBuffer);
	return LOADED;
}

RTechnique RShaderFX::GetTechniqueByIndex(unsigned int index) const
{
	RTechnique cTechnique;
	_ASSERT(m_pEffect!=NULL);

	if (m_pEffect)
	{
		cTechnique.m_pEffect = m_pEffect;
		cTechnique.m_hTechnique = m_pEffect->GetTechnique(index);
		_ASSERT(cTechnique.m_hTechnique);
	}
	return cTechnique;
}

RTechnique RShaderFX::GetTechnique(const char* pName) const
{
	RTechnique cTechnique;
	_ASSERT(m_pEffect!=NULL);

	if (m_pEffect)
	{
		cTechnique.m_pEffect = m_pEffect;
		cTechnique.m_hTechnique = m_pEffect->GetTechniqueByName(pName);
		_ASSERT(cTechnique.m_hTechnique);
	}
	return cTechnique;
}


///////////////////////////////////////////////////////////////////////  
//  RShaderFX::GetConstant

RShaderConstant RShaderFX::GetConstant(const char* pName) const
{
	RShaderConstant cParameter;

	if (m_pEffect)
	{
		cParameter.m_pEffect = m_pEffect;
		cParameter.m_hParameter = m_pEffect->GetParameterByName(NULL, pName);
		cParameter.m_strName = pName;

		_ASSERT(cParameter.m_hParameter);
#ifdef _DEBUG
		D3DXPARAMETER_DESC descParam;
		HRESULT hResult = m_pEffect->GetParameterDesc(cParameter.m_hParameter,&descParam);
		if( SUCCEEDED(hResult) )
		{
			_ASSERT(strcmp(descParam.Name, pName)==0);
		}
		else
		{
			_ASSERT(false);
		}
#endif
	}

	return cParameter;
}


///////////////////////////////////////////////////////////////////////  
//  RShaderFX::CommitChanges

bool RShaderFX::CommitChanges(void) const
{
	bool bSuccess = false;
	bSuccess = SUCCEEDED(m_pEffect->CommitChanges( ));
	return bSuccess;
}

void RShaderFX::OnLostDevice()
{
	if(m_pEffect)
		m_pEffect->OnLostDevice();
}

void RShaderFX::OnResetDevice()
{
	if(m_pEffect)
		m_pEffect->OnResetDevice();
}

const char* RShaderFX::GetDebugName()
{
	return m_strFilename.c_str();
}

void RShaderFX::SetupShaderFlags()
{
	//////////////////////////////////////////////////////////////////////////
	// �÷��׵� ���� ����
#ifdef DEBUG_VS
	m_dwShaderFlags |= D3DXSHADER_DEBUG;// | D3DXSHADER_SKIPOPTIMIZATION;
#endif

#ifdef SHADER_USE_LEGACY_D3DX9_31
	m_dwShaderFlags |= D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;
#endif

	m_dwShaderFlags |= D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;
}

void RShaderFX::SetupShaderMacros()
{
	// setup shader #defines
	_ASSERT((m_vShaderDefines.size( ) % 2) == 0); // must be an even number of strings

	// ���̴� ��ũ�� ���� --------------------------
	m_pShaderMacros = new D3DXMACRO[m_vShaderDefines.size( ) / 2 + 1];
	int i = 0;
	for ( ; i < int(m_vShaderDefines.size( )); i += 2)
	{
		m_pShaderMacros[i / 2].Name = m_vShaderDefines[i].c_str( );
		m_pShaderMacros[i / 2].Definition = m_vShaderDefines[i + 1].c_str( );
	}
	// terminate the shader array
	m_pShaderMacros[i / 2].Name = NULL;
	m_pShaderMacros[i / 2].Definition = NULL;
}

bool RShaderFX::LoadShaderFile()
{
	//////////////////////////////////////////////////////////////////////////
	// ���Ͽ��� �д� ���̸� �о m_strShaderString �� ä��
	MFile file;
	file.SetAllowAccessUncompFile(true);
	if( OpenShaderFile( file ) == false )
		return false;

#ifdef DEBUG_VS
	// ������϶��� ���Ͽ��� �о �����ؾ� �ϹǷ� �о �������ϴ� �κ� ����
	return true;
#endif

	MakeShaderStringFromFile( file );
	return true;
}

bool RShaderFX::OpenShaderFile( MFile& file )
{
	string FileNameWithRelativePath;

	if (rs3::REngine::GetConfig().m_bUsingCompiledShader)
	{
		string strCompiledFileFullPathName;
		string strPureFileName = MGetPureFileName(m_strFilename);
		strCompiledFileFullPathName = strPureFileName + COMPILED_SHADER_FILENAME_EXT;

		if( file.Open( strCompiledFileFullPathName.c_str() ) )
		{
			m_bOpenCompiledShaderFile = true;
			m_strFullFilename = FileNameWithRelativePath;
			return true;
		}
	}

	if( file.Open( m_strFilename.c_str() ) )
	{
		m_strFullFilename = FileNameWithRelativePath;
		return true;
	}
	

	mlog("%s shader file open error.\n",m_strFilename.c_str());
	return false;
}

void RShaderFX::MakeShaderStringFromFile( MFile& file )
{
	unsigned int size = file.GetLength();

	if (m_bOpenCompiledShaderFile)
	{
		m_pCompiledEffectFileBuffer = new char[size + 1];
		m_nCompiledEffectFileBufferSize = size;
		file.ReadAll( m_pCompiledEffectFileBuffer, size );
	}
	else
	{
		m_strShaderString.resize( size + 1 );
		file.ReadAll( &m_strShaderString[0], size );
		m_strShaderString[size] = '\0';
	}
}

bool RShaderFX::CompileEffect()
{
	// �̹� �����ϵ� ���̴��� �о����� ���������� �ʴ´�.
	if (m_bOpenCompiledShaderFile == true) return true;

	//////////////////////////////////////////////////////////////////////////
	// compile effect
	RShaderFileIncluder shaderFileIncluder;
	LPD3DXEFFECTCOMPILER pEffectCompiler;
	if(S_OK!=D3DXCreateEffectCompiler( m_strShaderString.c_str(), m_strShaderString.length() + 1, m_pShaderMacros, &shaderFileIncluder, m_dwShaderFlags, &pEffectCompiler, &m_pErrorMsgs))
	{
		if (m_pErrorMsgs)
		{
			const char* pCompileErrors = (const char*)m_pErrorMsgs->GetBufferPointer( );
			mlog(pCompileErrors);
			SAFE_RELEASE(m_pErrorMsgs);
		}

		if( REngine::IsDevelopVersion() )
		{
			FILE *fp = fopen("create_error.fx","wt");
			if ( fp )
			{
				fprintf(fp,"%s\n", m_strShaderString.c_str());
				fclose(fp);
			}
		}
		return false;
	}


	if(S_OK!=pEffectCompiler->CompileEffect(m_dwShaderFlags, &m_pCompiledEffectBuffer, &m_pErrorMsgs))
	{
		if (m_pErrorMsgs)
		{
			const char* pCompileErrors = (const char*)m_pErrorMsgs->GetBufferPointer( );
			mlog(pCompileErrors);
			SAFE_RELEASE(m_pErrorMsgs);
		}
		if( REngine::IsDevelopVersion() )
		{
			FILE *fp = fopen("compile_error.fx","wt");
			if ( fp )
			{
				fprintf(fp,"%s\n", m_strShaderString.c_str());
				fclose(fp);
			}
		}
		SAFE_RELEASE( pEffectCompiler );
		return false;
	}

	SAFE_RELEASE( pEffectCompiler );

	// �������� ���� ĳ�� ���Ϸ� �����Ѵ�.
	SaveCacheFile();

	return true;
}

bool RShaderFX::IsSettingFromFile()
{
	return (!m_strFilename.empty());
}

void RShaderFX::LoadCachingFile()
{
	if (REngine::GetConfig().m_bUsingCompiledShaderCaching == false) return;
	if (m_strCachingFileName.empty()) return;

	m_bOpenCompiledShaderFile = false;

	string strPureName = MGetPureFileName(m_strCachingFileName);
	string strFullFileName = REngine::GetConfig().m_strCompiledShaderCachePath + strPureName + COMPILED_SHADER_FILENAME_EXT;

	// ������ ������ ����
	MFile file;
	file.SetAllowAccessUncompFile(true);
	if( file.Open( strFullFileName.c_str(), true) == false )
	{
		return;
	}

	// ĳ�� ������ �д´�.
	m_bOpenCompiledShaderFile = true;
	MakeShaderStringFromFile( file );
}

void RShaderFX::SaveCacheFile()
{
	if (REngine::GetConfig().m_bUsingCompiledShaderCaching == false) return;
	if (m_strCachingFileName.empty()) return;

	string strPureName = MGetPureFileName(m_strCachingFileName);
	string strFullFileName = REngine::GetConfig().m_strCompiledShaderCachePath + strPureName + COMPILED_SHADER_FILENAME_EXT;

	FILE *file;
	file = fopen(strFullFileName.c_str(), "wb");
	if( file )
	{
		fwrite(m_pCompiledEffectBuffer->GetBufferPointer() , 1 , m_pCompiledEffectBuffer->GetBufferSize() , file );
		fclose(file);
	}
	else
	{
		errno_t err;
		_get_errno( &err );
		mlog( "���̴� ĳ�� ���� ���� ����. �����ڵ� - %d\n", err );
		_ASSERT( false );
	}
}

//////////////////////////////////////////////////////////////////////////
//

RShaderFXPool::RShaderFXPool()
{
	HRESULT hr = D3DXCreateEffectPool(&m_pEffectPool);
	_ASSERT(SUCCEEDED(hr));
}

RShaderFXPool::~RShaderFXPool()
{
	SAFE_RELEASE(m_pEffectPool);
}

LPD3DXEFFECTPOOL RShaderFXPool::GetEffectPool()
{
	return m_pEffectPool;
}

};