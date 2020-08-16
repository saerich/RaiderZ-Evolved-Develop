#pragma once

#include <string>
#include "RTypes.h"
#include "RPathSplitter.h"

using namespace std;

namespace rs3 {

class RS_API RConfiguration
{
private:
	string  m_strDataPath;

	float	m_fSpeedTreeCompositeTextureMipmapLodBias;	///< SpeedTree Texture Mipmap bias
	float	m_fSpeedGrassTextureMipmapLodBias;			///< SpeedGrass Texture Mipmap bias

	RTEXTUREFILTERTYPE	m_TextureFilter;
	DWORD				m_dwTextureMaxAnisotropy;

	string MakePath(const char *szName);

	void SetVideo(MXmlElement *pElement);
public:
	RConfiguration();

	bool	m_bFullScreen;
	int		m_nInitX;				///< �������忡�� ������ �ʱ� X��ġ
	int		m_nInitY;				///< �������忡�� ������ �ʱ� Y��ġ
	int		m_nWidth;
	int		m_nHeight;
	int		m_nTextureReduction;	///< �ؽ����ػ� ���� ���� �ɼ�
	float	m_fGlobalTextureMipmapLodBias;	///< ���� �ؽ��� Mipmap bias
	bool	m_bUsingShader;

	bool	m_bLogResource;						///< ���ҽ� �ε� �ð��� üũ�� �� ����
	bool	m_bUsingCompiledShader;				///< ���� �����ϵ� ���̴��� ������ �����ϵ� ���̴��κ��� �о���� ����
	bool	m_bUsingCompiledShaderCaching;		///< �����ϵ� ���̴� ���� ĳ������ ����
	string	m_strCompiledShaderCachePath;

	bool	m_bBackgroundCreateShaderComposite;	///< ���̴� ���� ���ҽ��� ��׶��� ���� �� ���ΰ�.

	RFORMAT	pixelFormat;
	RFORMAT	depthFormat;

	void SetDataPath(const char* szPath);
 	string GetDataPath( const char *DataFilename);

	float	GetSpeedTreeCompositeTextureMipmapLodBias() const { return m_fSpeedTreeCompositeTextureMipmapLodBias; }
	void	SetSpeedTreeCompositeTextureMipmapLodBias(float val) { m_fSpeedTreeCompositeTextureMipmapLodBias = val; }

	float	GetSpeedGrassTextureMipmapLodBias() const { return m_fSpeedGrassTextureMipmapLodBias; }
	void	SetSpeedGrassTextureMipmapLodBias(float val) { m_fSpeedGrassTextureMipmapLodBias = val; }

	void				SetTextureFilter( RTEXTUREFILTERTYPE filter)	{ m_TextureFilter = filter; }
	RTEXTUREFILTERTYPE	GetTextureFilter()	{ return m_TextureFilter; }

	void	SetTextureMaxAnisotropy( DWORD maxAnisotropy)	{ m_dwTextureMaxAnisotropy = maxAnisotropy; }
	DWORD	GetTextureMaxAnisotropy()	{ return m_dwTextureMaxAnisotropy; }

	bool LoadFromXMLFile(const char *filename);
};

#define COMPILED_SHADER_FOLDER_PATH			"obj/"
#define COMPILED_SHADER_FILENAME_EXT		".fxo"

}
