#pragma once

#include <vector>
#include "MVector2.h"
#include "MSphere.h"

/// �⺻���� ���� �ʵ� �Ӽ��� ��� �ִ� Ŭ����
class RHeightField
{
public:
	enum TERRAIN_DESC_BOOLEAN_ATTRIB	// 32bit signed
	{
		NONE = 0x00,
		VALID_POS	= 0x20000000,
		UNDER_WATER = 0x40000000,
		INVALID_POS	= 0x80000000		// ����� �� ���� ��ġ��
	};

private:
	// ���� ����
	float *m_pHeightMap;
	// ��Ÿ ���� (flag ���� ��)
	struct SDesc
	{
		DWORD dwAttribute;
		DWORD dwMaterial;
	};

	SDesc *m_pHeightMapDesc;

	int m_nHeightMapArraySize;
	float m_fMinZ, m_fMaxZ;

	struct SDescHeader
	{
		char pcHeader[4];
		DWORD dwVersion;
	};

	struct SGridInfo
	{
		MBox m_aabb;
		RECT m_kRect;
	};
	/// ��ó���� ���� GridMap ( AABB�� ����)
	SGridInfo *m_pGridMap;

	/// GridMap �� ����, ���� ĭ ��
	int m_nGridWidth;
	int m_nGridHeight;
	/// GridMap�� ��ĭ�� ��ĭ�� HeightFieldMap���� �Ǿ��ִ°� 
	int m_nGridSize;

	/// HeightMap�� ���� ���� ( Pitch�� �� ���� ũ��, Map�� ĭ(���� �ƴ�) ����
	int m_nHeightMapPitch;
	int m_nWidthMap;
	int m_nHeightMap;

	/// �� ĭ�� �����ϴ� x, y ũ��
	MVector2 m_vScale;
	/// ��ü AABB ����
	MBox m_aabb;

	void UpdateHeightField();
	void GetGridAABB(MBox *pAABB, const RECT *pRect);

	/// param nGridSize	�׸��� �� ĭ�� ũ��
	void CreateGridInfo(int nGridSize);
	
	void CreateDesc();

	/// return vP���� ��ŷ�� ���������� �Ÿ��� ���Ѵ�. pIndex*�� ��ŷ�� �ﰢ���� �ε���
	float GetPickLineRect(MVector3 *pOut, const MVector3 &vP, const MVector3 &vQ, const MVector3 &vDir, const RECT *pRect
		, MVector3 *pTriangle, TERRAIN_DESC_BOOLEAN_ATTRIB NonPickingAttrib);

	/// ����ȭ �뵵, ���������� �ش��ϴ� Grid�� �����س���. P1, P2�� �ݵ�� Ŭ���� �Ǿ� �־�� �Ѵ�.
	void GetLineIntersectGrid(std::vector<std::pair<int, int> > &vecArray, const MVector2 &P1, const MVector2 &P2, float fXScale, float fYScale);

	static std::vector<std::pair<int, int> > m_vecGridArray;
	static std::vector<std::pair<int, int> > m_vecTriangleArray;

	static int GRID_SIZE;
	static int DESC_VERSION;

	/// dds fp32 ���� �д� �κ� ( ���̸� ���� ���� )
	float*	LoadR32FDDSFromFile(const char *filename, int *outWidth, int *outHeight, float fZScale, float fZOffset);

protected:
public:
	RHeightField();
	virtual ~RHeightField();

	bool LoadHeightFieldDescFile(const char *filename);
	void SaveHeightFieldDescFile(const char *filename);

	bool LoadHeightFieldmapFromR32F(const char *filename, const MVector2 &vSize);
	void SaveHeightFieldmapToR32F(const char *filename);

	bool Create(int nImageWidth, int nImageHeight, MVector2 &vSize);

	void GetPositionAtIndexXY(MVector3 *pOut, int w, int h);
	bool GetHeightAt(float *height, MVector3 *pNormal, float x, float y);
	bool GetHeightAtWithHole(float *height, MVector3 *pNormal, float x, float y); // Hole�� �κ��� ������ ���� ���� �����´�.

	// ���⼭ x, y�� displacement map�� x,y �ε����̴�. ( x : 0 ~ GetWidthMap(), y : 0 ~ GetHeightMap() )
	float GetHeightAtXYIndex(int x, int y);

	/**
	 @param		x	x ��ǥ[0-GetWidthSize)
	 @param		y	y ��ǥ[0-GetHeightSize)
	 @param		param	�Ӽ�(bit flag�� ��ġ)
	 @param		bFlag	�Ӽ��� true�� �� ������, falsg�� �� ������
	*/
	void SetAttribute(int nx, int ny, TERRAIN_DESC_BOOLEAN_ATTRIB attrib, bool bFlag);
	void SetMaterial(int nx, int ny, DWORD dwID);
	/**
	@param		x	x ��ǥ[0-GetWidthSize)
	@param		y	y ��ǥ[0-GetHeightSize)
	@param		param	�Ӽ�(bit flag�� ��ġ)
	@return		�Ӽ����� true����, false����
	*/
	bool GetAttribute(float x, float y, TERRAIN_DESC_BOOLEAN_ATTRIB attrib);
	bool GetAttribute(int x, int y, TERRAIN_DESC_BOOLEAN_ATTRIB attrib);

	/**
	 	����� ����
	 */
	DWORD GetMaterial(int x, int y);
	DWORD GetMaterial(float x, float y);

	void ClearDescriptMap();

	void UpdateAABB();
	bool GetPickLine(MVector3 *pOut, MVector3 *pOutNormal, float *pPickDistance, const MVector3 &vP, const MVector3 &vQ, TERRAIN_DESC_BOOLEAN_ATTRIB NonPickingAttrib = INVALID_POS);

	/**
	@brief		AABB �� �浹 �˻�
	@param		bbox	AABB ����ü
	@return		�浹 ������ true, �׷��� ������ false�� �����Ѵ�
	*/
	bool IsIntersect(const MBox &bbox);

	/// ���� HeightField �����͸� �����´�. �����Ŀ��� UpdateAABB �ݵ�� ȣ�� �� �� ��
	inline float* GetPtr();

	/// GetScale().x, GetScale().y �� ����
	inline float GetXScale();
	inline float GetYScale();

	/// GetAABB�� vmin.z, vmax.z �� ����
	inline float GetMinZ();
	inline float GetMaxZ();

	/// ����, ���� ���� �����ϴ� ũ��
	inline float GetWidthSize();
	inline float GetHeightSize();

	inline int GetWidthMap();
	inline int GetHeightMap();
	inline int GetHeightMapPitch();

	inline const MBox& GetAABB();
	inline const MVector2& GetScale();
};

float* RHeightField::GetPtr()
{
	return m_pHeightMap;
}

float RHeightField::GetXScale()
{
	return m_vScale.x;
}

float RHeightField::GetYScale()
{
	return m_vScale.y;
}

float RHeightField::GetMinZ()
{
	return m_fMinZ;
}

float RHeightField::GetMaxZ()
{
	return m_fMaxZ;
}

int RHeightField::GetWidthMap()
{
	return m_nWidthMap;
}

int RHeightField::GetHeightMap()
{
	return m_nHeightMap;
}

int RHeightField::GetHeightMapPitch()
{
	return m_nHeightMapPitch;
}

const MBox& RHeightField::GetAABB()
{
	return m_aabb;
}

const MVector2& RHeightField::GetScale()
{
	return m_vScale;
}

float RHeightField::GetWidthSize()
{
	return ( m_vScale.x * m_nWidthMap );
}

float RHeightField::GetHeightSize()
{
	return ( m_vScale.y * m_nHeightMap );
}
