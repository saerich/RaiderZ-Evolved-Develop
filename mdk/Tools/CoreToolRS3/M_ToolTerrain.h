
/**
 * RealSpace3�� RTerrain�� ��ӹ޾�, ������ ����� �� �ֵ��� ���� Ŭ����
 */
	
#pragma once

#include "RTerrain.h"
#include "RDirectionalLightSceneNode.h"
#include "RD3DVertexUtil.h"

#include "C_ControlSceneProperty.h"

namespace rs3 {

class RTreeInstance;
class RFont;
class CORETOOL_API RToolTerrain : public RTerrain
{
	MDeclareRTTI;
protected:
	// �׸��带 �׸� �� ����ϴ� �ؽ���
	RTexture* m_hGridTexture;
	RTexture* m_hMarkingTexture;
	RTexture* m_hNoneTexture;

	 // Alpha - Mask ���� ���� ������ �����ϴ� ����ü
	struct LayerInfoAttend
	{
		bool bMapChanged;

		LayerInfoAttend()
			: bMapChanged(true)
		{
		}
	};
	
	// Displacement-Map�� ���� ���� ���濡 ���� Flag
	bool m_bDisplacementMapChanged;

	// Alpha Map ������ ��� �ִ� �迭
	LayerInfoAttend *m_pAttendInfo;
	int m_nTotalAlphaTextureNum;

	HVERTEXBUFFER m_hGuideVertex;
	RTexture *m_hMaterialMap;
	DWORD *m_pMaterialMap;

	bool m_bObjectLight;

	/**
	 * LightMap ����(m_pLightMap)�� Alpha-Mask �ʿ� ��ģ��.
	 *
	 * @param	nIndex	Alpha-Mask �� ���̾� �ε���
	 */
	template< typename T_ContainerToPick >
	bool NodeContainerPicking(T_ContainerToPick &nodeContainer, RVector &orign, RVector &to, RPICKINFO *pInfo);

	/**
	 * Grid�� �׸� �� ���Ǵ� Texture�� �����Ѵ�.
	 *
	 * @return  �����ϸ� true, �����ϸ� false
	 */
	bool CreateUtlityTexture();
	/**
	 * �ش� UV ������ Alpha Pixel(RGBA 32bit format)�� �ִ��� �˻��Ѵ�.
	 * @return	�� �ȼ��̶� ref���� ���� ���̸� false, �׷��� ������ true
	 */
	bool IsAlphaPixel(int nIndex, float left, float top, float right, float bottom, long ref = 0xff);
	/**
	 * �ش� UV �������� ���� ������ ���� �����ϴ� Alpha Mask�� ID
	 * @return	Mask�� ID, ������ -1�� �ȴ�.
	 */
	int	ActuallyAlphaMask(float left, float top, float right, float bottom);

public:
	void SetBaseMapName();

protected:
	//������ Terrain �����۾� ����
	void ApplyTerrainVertex_(RECT *pRect = NULL);

	/**
	 * AlphaMaskMap�� Resize�ϴ� �κ�. index�� Resize ������ �Է¹޴´�.
	 * @param	nIndex	Alpha MaskMap�� �ε���
	 * @param	nPower	����
	 * @param	bMultiply	true�� nPower��ŭ ���ϰ�, false�� nPower��ŭ ������.
	 * @return	Resize ������ �¾Ƽ� �ϰԵǸ� true, �ƹ� �͵� ���� ������ false�� �����Ѵ�.
	 */
	bool ResizeLayerAlpha(int nIndex ,int nPower, bool bMultiply = true);
	bool FloodFillMapValidMaker(int nX, int nY, float fSlopeDot);
	void UpdateMaterialMap();

	///	DisplacementMap, AlphaMaskMap, LightMap, GrassFile �� �����Ѵ�.
	class CreateName
	{
	public:
		static std::string DisplacementMapFile(RResource *pRes);
		static std::string BaseMapFile(RResource *pRes);
		static std::string GrassFile(RResource *pRes);
		static std::string AlphaLayerFile(RResource *pRes, int nIndex);
		static std::string LightMapFile(RResource *pRes, int nIndex);
	};

	void SaveDataFiles();
	
	bool AddLayer(RTexture *pTileTexture, RTexture* pNormalTexture, bool bFillAlpha);
public:
	static DWORD GRID_TEXTURE_COLOR;
	static DWORD MARKING_TEXTURE_COLOR;

	/// Seed Position�� �ش����� �ʴ� ���� ��ȿȭ ó���Ѵ�.
	void UpdateDescriptMapMakedFromSeed(const MVector3 seedPosition, float fSlopeDot);
	void UpdateDescriptMapMakedAll(bool bFlag);
	void UpdateDescriptMapMakedBySlope(float fSlopeDot);
	/// Splatting mask-map(��� layer)�� �����ؼ� displacement map - desc �迭�� ��ũ�Ѵ�.
	/// Alpha Splatting Mask�� �����͸� �����ؼ� Material ID�� ����Ѵ�.
	void UpdateDescriptMapAndMaterial(RECT rect);
	/// ������ �˻��Ͽ�, �������� �ƴ��� �˻��Ѵ�.
	void UpdateDescriptMapUnderWater();

	/// ���. �ʱ�ȭ ó�� ���� ������ ...
	virtual bool CreateFromXML(MXmlElement &element, bool bBackgroundCreation);

	// ���ʿ��� Alpha mask �� pixel�� �����Ѵ�.
	int RebuildAlphaMask();

	bool AddLayer(const char *pTileName, bool bFillAlpha = false);
	void DeleteLayer(int nIndex);
	bool DeleteNormalMap(int nIndex);
	bool ChangeTileMap(int nIndex, const char *pTileName);
	bool ChangeNormalMap(int nIndex, const char *pTileName);
	bool SwapLayer(int nIndex1, int nIndex2);
	void CreateAlphaTexture(int nIndex, bool bFillAlpha = false, DWORD dwWidth = 0, DWORD dwHeight = 0);
	void UpdateAlphaTexture(int nIndex);
	void CountAlphaTexture();

	bool ResizeLayerAlphaMul2X(int nIndex);
	bool ResizeLayerAlphaDiv2X(int nIndex);

	void ReloadTexture();

	void SetTiling(int nIndex, rs3::RVector &uvw);



	virtual bool LoadDisplacementmapFromR32File(bool _bMainThreadCall, const char *filename, int nPatchSize, int nLODLevel, float fXSize, float fYSize
		, float fZScale /* = 1.0f */, float fZOffset);

	virtual void CreateLayer(int nNum, int nPatchNum);

	bool CreateNewTerrain(const char *pPath, const char *pResourceName, const char *pBaseTileName, int nWidth, int nHeight, float fXSize, float fYSize);
	bool UpdateNormal(int nWidth, int nHeight, float fXSize, float fYSize);
	bool CreateDisplacementmapPlane(int nWidth, int nHeight, float fXSize, float fYSize);
	bool SaveDisplacementmapToFile();
	bool SaveAlphaMapToFile();
	bool RecreateAndSaveBakedMap(); // baked map ���� �����ϰ� ����

	bool CreateGrass();

	void CreateGuideVertex();

	int RenderGuideMap();
	int RenderPatchInfo(RFont* pFont);
	
	virtual void RenderMStage( RCameraSceneNode* pCurrentCamera,RRENDERINGMETHOD renderMethod) override;
	virtual void OnUpdate() override;

	RToolTerrain(RDevice *pDevice);
	virtual ~RToolTerrain();

	// .obj ���� Export/import
	bool ExportObjFile(int nLevel, int nWidthPartition, int nHeightPartition);
	bool ImportObjFile(const char *filename, std::string *pErrMessage);

	virtual void SaveToXML(MXmlElement &element);

	void AllChangedFlag(bool bFlag);

	// baseMap
	bool SaveBaseMapToFile();

	// grassMap
	void UpdateGrassOnTerrain();
	bool SaveGrassToFile();

	void ApplyCircleActionWithFuncPtr (
		const rs3::RVector* pVecCenterPos,					//������ Terrain Circle Area�� �߽���
		float fSize, float fScale, float fFactor,			//Circle ���� ũ��, �ൿ�� ����, �ൿ�� ����
		float (*funcPtrAction)(float, float, float, float),	//���� ���� �ൿ�� ����(�Լ� ������)
		bool bTerrainVertexUpdate = true);					//Terrain Vertex Update ����

	void ApplyRelaxAction(float _fStrength,				//����
		float _fSizeFactor = 0.0f, RECT* pRect = NULL);	//rect���� �������� ���� �� rect ����. NULL �̸� ��ü

	// TOOL ���� ����ϴ� Picking
	bool GetPickLine(RVector &rOut, RVector *pNormal, const RVector &p, const RVector &q);

	// static serialize function
	static bool GetAllWaterPlane(RSceneNode *pSceneNode);
};

template< typename T_ContainerToPick >
inline bool RToolTerrain::NodeContainerPicking(T_ContainerToPick &nodeContainer, RVector &orign, RVector &to, RPICKINFO *pInfo)
{
	pInfo->inPickOrigin = orign;
	pInfo->inPickTo = to;	// by dubble. pick ȣ�� ����� �ٲ�����ϴ�.

	for ( T_ContainerToPick::iterator it = nodeContainer.begin(); it != nodeContainer.end(); ++it )
	{
		RSceneNode *pNode = (RSceneNode*)(*it);
		if ( pNode->Pick(*pInfo))
			return true;
	}
	return false;
}

}