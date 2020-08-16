#pragma once

#include "RResource.h"
#include "RHeightField.h"
#include "RVType.h"

namespace rs3 {

class RTerrainQuadTree;
class RTerrainPatch;
class RTerrainPVS;
class RGrassSystem;
class RTerrain;

class RS_API RTerrainResource : public RResource
{
public:
	RTerrainResource(void);
	virtual ~RTerrainResource(void);

	static int PATCH_SIZE;
	static int LOD_LEVEL;

	bool FillDisplacementMap( const char* filename, const RVector& _refVecMapSize );
	bool FillLayerAlpha(int nIndex, const char *filename);
	bool CreateLayerAlpha(int nIndex);
	void FillLayerTile(int iIndex, const char* pTileFileName, const char* pNormalFileName, float fXSize, float fYSzie, bool bUseToSpecular);

	enum DETAIL_LEVEL
	{
		ONLY_BAKED = 0,
		BAKED_AND_SPLATTING,
		ONLY_SPLATTING
	};

	struct LayerInfo 
	{
		DWORD dwAlphaPatchWidth;	/// ��ġ �� ���� �ؽ��� �ػ�
		DWORD dwAlphaPatchHeight;	/// ��ġ �� ���� �ؽ��� �ػ�
		DWORD dwAlphaWidth;			/// �� ��ü�� ���� �ؽ��� �ػ�
		DWORD dwAlphaHeight;		/// �� ��ü�� ���� �ؽ��� �ػ�

		std::string alphaMapName;
		std::string tileMapName;
		std::string normalMapName;
		
		RTexture* *phAlphaMask;		/// ���̾�,��ġ �� ���� ����ŷ �ؽ���
		bool* pbDataUsed;			/// ���̾�, ��ġ �� ��� ����. multi-pass������ phAlphaMask�� ���簡 �̸� ����������, one-pass������ phAlphaMask�� ������� �ʴ´�.
		bool* pbAlphaMaskCreated;	/// ���̴�,��ġ �� ���� ����ŷ �ؽ��� ���� ó�� ����
		RTexture* hTileMap;
		RTexture* hNormalMap;

		RVector vTilingUV;
		bool bAlphaMaskForSpec;
		BYTE *pAlphaTextureRaw;	// BGRA Texture ( 4byte ���� )
		RTexture *pAlphaTexture;
		bool bNeedUpdateAlphaTexture;

		LayerInfo() 
			: phAlphaMask(NULL)
			, hTileMap(R_NONE)
			, hNormalMap(R_NONE)
			, bAlphaMaskForSpec(false)
			, dwAlphaWidth(0)
			, dwAlphaHeight(0)
			, pAlphaTextureRaw(NULL)
			, pAlphaTexture(NULL)
			, bNeedUpdateAlphaTexture(true)
			, pbDataUsed(NULL)
			, pbAlphaMaskCreated(NULL)
			, dwAlphaPatchWidth( 0 )
			, dwAlphaPatchHeight( 0 )
		{}
	};

protected:
	void SetResourceName(const char *pName);

	virtual bool		Fill() override;
	virtual LOAD_RESULT Load() override;

	struct DISPLACEMENTMAP_AND_GRASS_INFO
	{
		std::string		m_strDisplacementFileName;
		std::string		m_strGrassFileName;
		RVector			m_vMapSize;
		int				m_nPatchSize;
		int				m_nLodLevel;
		bool			m_bFillDone;
		bool			m_bLoadDone;

	} m_InfoDisplacementAndGrass;

	struct ALPHA_MAP_INFO
	{
		ALPHA_MAP_INFO( int nIndex, const char* pAlphaMapFileName)
			: m_nIndex(nIndex)
			, m_strAlphaMapFileName(pAlphaMapFileName)
		{}

		int				m_nIndex;
		std::string		m_strAlphaMapFileName;
	};
	std::deque< ALPHA_MAP_INFO > m_deqInfoAlphaMap;

	struct TILE_MAP_INFO
	{
		TILE_MAP_INFO( int nIndex, const char* pTileMapFileName, const char* pNormalMapFileName, const RVector& vMapSize, bool bSpecularTest )
			: m_nIndex(nIndex)
			, m_strTileMapFileName(pTileMapFileName)
			, m_strNormalMapFileName(pNormalMapFileName)
			, m_vMapSize(vMapSize)
			, m_bSpecularTest(bSpecularTest)
		{}

		int				m_nIndex;
		std::string		m_strTileMapFileName;
		std::string		m_strNormalMapFileName;
		RVector			m_vMapSize;
		bool			m_bSpecularTest;
	};
	std::deque< TILE_MAP_INFO > m_deqInfoTileMap;



	bool	m_bNeedToBake;
	int m_nLODPatchCount;

	static const unsigned int TERRAIN_HEIGHT_STEP = 32;
	std::string m_strPathName;

	enum SIDE_FLAG 
	{
		LEFT = 0x01,
		RIGHT = 0x02,
		TOP = 0x04,
		BOTTOM = 0x08
	};

	// QuadTree
	RTerrainQuadTree *m_pQuadTree;
	RTerrainPatch *m_pPatches;

	// PVS
	RTerrainPVS *m_pPVS;

	// Horizon Cull
	//Horizon *m_pHorizonCull;

	// Cull Patch Index
	std::vector<RTerrainPatch*> m_arrCulledPatch;
	std::vector<RTerrainPatch*> m_arrCulledDetailHighPatch;
	std::vector<RTerrainPatch*> m_arrCulledDetailLowPatch;
	std::vector<RTerrainPatch*> m_arrCulledSimplePatch;

	RVector m_vStep;
	RHeightField *m_pHeightField;
	RVector *m_pNormalMap;

	// LOD���� Index ( �� Patch�� �������, LOD-Level�� ���� �����ϹǷ� Terrain�� �־��� )
	// 0 �� 0, 1 ���ʹ� (( Level - 1 ) << 2 ) & Flag
	HINDEXBUFFER *m_phIndexBuffer;
	int *m_pPrimitiveNum;	// �� ��ġ�� Triangle ����

	int m_iIndexNum;
	int m_iLODLevel;

	// Displacement Map
	std::string m_strDisplacementMapName;

	// grass Name
	std::string m_strGrassName;

	int m_iLayerNum;
	LayerInfo *m_pLayerInfo;
	

	int m_iPatchNum;	/// �� ��ġ ��
	int m_nPatchSize;
	int m_iCount;		/// ���� ���� ��ġ ��

	// BaseMap
	std::string	m_strBaseMapName;
	RTexture* m_hBaseMap;
	int m_nBaseMapWidth;
	int m_nBaseMapHeight;

	// Grass System
	RGrassSystem *m_pGrassSystem;

	float m_fGrassNear;
	float m_fGrassFar;

	// Filtering
	bool m_bAnisotropyFilter;

	DWORD m_dwAmbientColor;
	DWORD m_dwDiffuseColor;

	RTerrain*	m_pTerrain;		// RTerrain�� RTerrainResource�� ��� �ޱ⿡ RTerrainResource == RTerrain������ �ν��Ͻ��� RTerrain�� �ƴ� RTerrainResource�θ� ���� �� ���� �����ϱ� �����ͷ� ���� �ּҸ� ���� �Ѵ�.

	int GetLodIndex(int nLevel, int nFlag)
	{
		if ( nLevel == 0 )	return 0;
		--nLevel;
		int nHead = nLevel << 4;
		return ( nHead | nFlag ) + 1;
	}

	static DWORD m_dwVersion;

	void Initialize();
	void UpdateAlphaTexture(int nPatchIndex, int nLayerIndex);
	void UpdateAlphaTextureOfLayer(int nIndex);
	void UpdateAlphaTextureOfPatch(int nIndex);
	void NeedUpdateAlphaTexture( int layerIdx);
	//void UpdateMaskTexture();
	void InitAlphaTexture(int nIndex, int nWidth, int nHeight);
	bool IsAlphaDataInTexture(void *pData, int nWidth, int nHeight, RECT *pRect, int nPitch, int nBpp);
	bool IsAlphaDataInTexture(int nPatchIndex, int nLayerIndex);
	void DeleteAlphaTexture(int nIndex);
	// Raw Data
	void DeleteAlphaTextureRaw( int nLayerIdx);
	bool IsAlphaTextureUsed( int nPatchIdx, int nLayerIdx);
	void SetAlphaTextureRaw( int nLayerIdx, BYTE* pData);

	void GetPatchXYZ(int *pX, int *pY, int *pZ, const RVector *pCameraPos);

	bool CreateIndexContact(HINDEXBUFFER *phIndexBuffer, int *pPrimitiveNum, int nPatchWidth, int nPatchHeight, int nLevel, int nFlag);
	bool CreateIndexNonCont(HINDEXBUFFER *phIndexBuffer, int *pPrimitiveNum, int nPatchWidth, int nPatchHeight, int nLevel);

	bool CreateDisplacementmapPatch(float fXSize, float fYSize, int nPatchSize, int nLODLevel);
	bool CreateNormalFieldMap(int nImageWidth, int nImageHeight, float fXSize, float fYSize);

public:
	//virtual bool ImportDisplacementmapFromBMPFile(bool _bMainThreadCall, const char *filename, int nPatchSize, int nLODLevel, float fXSize, float fYSize, float fZSize
	//	, int nBase = 0);
	virtual bool LoadDisplacementmapFromR32File(bool _bMainThreadCall, const char *filename, int nPatchSize, int nLODLevel, float fXSize, float fYSize
		, float fZScale = 1.0f, float fZOffset = 0.0f);

	virtual void CreateLayer(int nLayerNum, int nPatchNum);

	void UpdateHeightField(int w, int h);
	void UpdateHeightField(RECT *pRect, int w, int h, int nPitch);

	virtual int GetVertexFromBound(std::vector<RVt_pos> &vecVertex, const RBoundingBox &aabb);

	float GetHeightFromHeightMap(int x, int y) { return m_pHeightField->GetHeightAtXYIndex(x, y); }

	int GetLayerNum()		{	return m_iLayerNum;	}
	const LayerInfo* GetLayerInfo(int nIndex);

	int GetWidthCount()		{	return m_iCount;	}
	int GetHeightCount()	{	return m_iCount;	}

	float GetXScale()		{	return m_pHeightField->GetXScale(); }
	float GetYScale()		{	return m_pHeightField->GetYScale(); }
	float GetHSize()		{	return m_pHeightField->GetMaxZ() - m_pHeightField->GetMinZ(); }

	int GetWidthMap()		{	return m_pHeightField->GetWidthMap(); }
	int GetHeightMap()		{	return m_pHeightField->GetHeightMap(); }
	int GetHeightMapPitch()	{	return m_pHeightField->GetHeightMapPitch(); }

	float GetWidthSize()	{	return m_pHeightField->GetWidthSize(); }
	float GetHeightSize()	{	return m_pHeightField->GetHeightSize(); }

	RHeightField*	GetHeightFieldPtr()		{	return m_pHeightField; }

	void SetLOD(int nPatchCount)					{	m_nLODPatchCount = nPatchCount; }

	void SetGrassLodParams(float _fNear, float _fFar);
	void SetPath(const char *pPathName);

	void AddObject(RSceneNode *pNode);
	void RemoveObject(RSceneNode *pNode);

	bool CreateGrassSystem();
	bool CreateAndLoadGrass(const std::string& _strFileName);

	RGrassSystem* GetGrassSystem() { return m_pGrassSystem; }
	// Raw Data
	void ToDoWhenUpdateAlphaTextureRaw( int nLayerIdx);

	/// ���� Ŭ������ ���õǾ� �ִ� ��ΰ��� ���´�.
	const char* GetPath()			{	return m_strPathName.c_str();  }
	/// FP32�� ����Ǿ� �ִ� ���� �� ������ ���´�.
	float* GetHeightMapPtr()		{	return m_pHeightField->GetPtr(); }
	/// X ĭ, Y ĭ�� ���� ���� float�� ���´�.
	float GetHeight(int nX, int nY)	{	return m_pHeightField->GetHeightAtXYIndex(nX, nY); }

	RTexture* GetTileTexture(int nIndex)
	{
		assert(m_iLayerNum>=nIndex);
		return m_pLayerInfo[nIndex].hTileMap;
	}

	RTexture* GetNormalTexture(int nIndex)
	{
		assert(m_iLayerNum>=nIndex);
		return m_pLayerInfo[nIndex].hNormalMap;
	}

	RVector* GetTileScale(int nIndex)
	{
		assert(m_iLayerNum>=nIndex);
		return &m_pLayerInfo[nIndex].vTilingUV;
	}

	// alpha mask setter / getter for specular map
	void SetAlphaMaskForSpecular(int nIndex, bool _bAlphaForSpec)
	{
		assert(nIndex<m_iLayerNum);
		m_pLayerInfo[nIndex].bAlphaMaskForSpec = _bAlphaForSpec;
	}

	bool IsSpecularAlphaMask(int nIndex)
	{
		assert(m_iLayerNum>=nIndex);
		return m_pLayerInfo[nIndex].bAlphaMaskForSpec;
	}

	BYTE* GetAlphaTexturePtr(int nIndex)
	{
		return m_pLayerInfo[nIndex].pAlphaTextureRaw;
	}

	DWORD GetAlphaTextureWidth(int nIndex)
	{
		return m_pLayerInfo[nIndex].dwAlphaWidth;
	}

	DWORD GetAlphaTextureHeight(int nIndex)
	{
		return m_pLayerInfo[nIndex].dwAlphaHeight;
	}


	const char* const GetDisplacementMapName()
	{
		return m_strDisplacementMapName.c_str();
	}

	const char* const GetAlphaMapName(int nIndex)
	{
		assert(m_iLayerNum>nIndex);
		return m_pLayerInfo[nIndex].alphaMapName.c_str();
	}

	const char* const GetTilemapName(int nIndex)
	{
		assert(m_iLayerNum>nIndex);
		return m_pLayerInfo[nIndex].tileMapName.c_str();
	}

	const char* const GetNormalmapName(int nIndex)
	{
		assert(m_iLayerNum>nIndex);
		return m_pLayerInfo[nIndex].normalMapName.c_str();
	}

	const char* const GetBaseMapName()
	{
		return m_strBaseMapName.c_str();
	}

	void SetBaseMapName( std::string strBaseMapName)
	{
		m_strBaseMapName = strBaseMapName;
	}

};


}
