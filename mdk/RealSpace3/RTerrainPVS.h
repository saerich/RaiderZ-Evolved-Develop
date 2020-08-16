///////////////////////////////////////////////////////////////////
//
// project:     Pluto Game Engine
// class:       pgTerrainPVS
// creator:     Daniel Wagner
// description: 
//
///////////////////////////////////////////////////////////////////
// $Id: pgTerrainPVS.h,v 1.1.1.1 2002/10/20 09:33:03 daniel Exp $
///////////////////////////////////////////////////////////////////

/**
 ShaderX^2 Tip&Trick - �ҽ��ڵ� ���
*/

#pragma once

namespace rs3 {

class RTerrainPVS
{
public:
	RTerrainPVS();
	~RTerrainPVS();

	void calc(const float* nMinData,
			  const float* nMaxData,
			  unsigned int nX, unsigned int nY, unsigned int nZ
			  , float fMinZ, float fMaxZ, float fDZ);

	void shrink();

	bool save(const char* nFileName);
	bool load(const char* nFileName);

	/// From -> To �� ���̴°� ?
	bool isVisible(int nFromX, int nFromY, int nFromZ, int nDestX, int nDestY) const;

	/// X,Y,Z �� grid ����
	int getDataX() const  {  return dataX;  }
	int getDataY() const  {  return dataY;  }
	int getDataZ() const  {  return dataZ;  }

	/// Delta-Z , ������ ����
	float getDataDZ() const  {  return dataDZ;  }

private:
	/// n?0 -> n?1 �� ���� �� ���������� �ƴ����� �����Ѵ�.
	void setVisible(int nX0, int nY0, int nZ0, int nX1, int nY1, bool nValue);
	/// n?0 -> n?1 �� ���� �� �ش��ϴ� �ε���
	unsigned int getIdx(int nX0, int nY0, int nZ0, int nX1, int nY1) const;

	/// PVS ������
	unsigned int dataX,dataY,dataZ;
	float dataDZ;
	float minZ;
	float maxZ;

	unsigned char* data;
	unsigned int dataSize;

};

};