///////////////////////////////////////////////////////////////////
//
// project:     Pluto Game Engine
// class:       pgTerrainPVS
// creator:     Daniel Wagner
// description: 
//
///////////////////////////////////////////////////////////////////
// $Id: pgTerrainPVS.cpp,v 1.1.1.1 2002/10/20 09:33:03 daniel Exp $
///////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "MMath.h"
#include "MFileSystem.h"
#include "RTerrainPVS.h"

namespace rs3 {

	RTerrainPVS::RTerrainPVS() : dataSize(0)
{
	dataX = dataY = dataZ = 0;
	dataDZ = 0;
	minZ = 0;
	maxZ = 0;

	data = NULL;
}

RTerrainPVS::~RTerrainPVS()
{
	SAFE_DELETE_ARRAY(data);
}


void
RTerrainPVS::calc(const float* nMinData, const float* nMaxData
				  , unsigned int nX, unsigned int nY, unsigned int nZ
				  , float fMinZ, float fMaxZ, float fDZ)
				  
{
	dataX = nX;
	dataY = nY;
	dataZ = nZ;
	dataDZ = fDZ;
	minZ = fMinZ;
	maxZ = fMaxZ;

	unsigned int	numData = dataZ * dataY * dataX * dataY * dataX,
					dataCount = 0,
					dstX,dstY, srcX,srcY,srcZ,srcZC;


	dataSize = (numData+7)/8;			// +7 => round upwards
	data = new unsigned char[dataSize];
	memset(data, 0, dataSize);

	for(srcZ=fMinZ + dataDZ-1,srcZC=0;
		srcZC<dataZ; 
		srcZ+=dataDZ,srcZC++)
		for(srcY=0; srcY<dataY; srcY++)
			for(srcX=0; srcX<dataX; srcX++)

				for(dstY=0; dstY<dataY; dstY++)
					for(dstX=0; dstX<dataX; dstX++)
					{
						float hMax = nMaxData[dstX + dstY*dataX];

						float	p, step = 1.0f / (float)sqrt((float)((dstX-srcX)*(dstX-srcX) + (dstY-srcY)*(dstY-srcY))),
								dx = (float)dstX-srcX,
								dy = (float)dstY-srcY,
								dz = (float)hMax-srcZ;

						setVisible(srcX,srcY,srcZC, dstX,dstY, true);
						for(p=0.0f; p<=1.0f; p+=step)
						{
							int x = (int)(srcX + dx*p),
								y = (int)(srcY + dy*p),
								z = (int)(srcZ + dz*p);

							if(z < nMinData[x + y*dataX])
							{
								//vis = 0;
								setVisible(srcX,srcY,srcZC, dstX,dstY, false);
								break;
							}
						}

						dataCount++;
					}

	assert(dataCount == numData);
}


#if 0 
void
RTerrainPVS::shrink()
{
	RTerrainPVS* oldPVS = new RTerrainPVS();
	oldPVS->dataX = dataX;
	oldPVS->dataY = dataY;
	oldPVS->dataZ = dataZ;
	oldPVS->dataDZ = dataDZ;
	oldPVS->data = data;
	oldPVS->dataSize = dataSize;

	dataX /= 2;
	dataY /= 2;

	unsigned int	numData = dataZ * dataY * dataX * dataY * dataX;

	dataSize = (numData+7)/8;			// +7 => round upwards
	data = new unsigned char[dataSize];
	memset(data, 0, dataSize);

	unsigned int srcX,srcY,srcZ,dstX,dstY, xs,ys,xd,yd;

	for(srcZ=0; srcZ<dataZ; srcZ++)
		for(srcY=0; srcY<dataY; srcY++)
			for(srcX=0; srcX<dataX; srcX++)
				for(dstY=0; dstY<dataY; dstY++)
					for(dstX=0; dstX<dataX; dstX++)
					{
						for(ys=0; ys<2; ys++)
							for(xs=0; xs<2; xs++)
								for(yd=0; yd<2; yd++)
									for(xd=0; xd<2; xd++)
										if(oldPVS->isVisible(2*srcX+xs,2*srcY+ys,srcZ, 2*dstX+xd,2*dstY+yd))
										{
											setVisible(srcX,srcY,srcZ, dstX,dstY, true);
											xs = ys = xd = yd = 2;
										}
					}

	delete oldPVS;
}
#endif

bool
RTerrainPVS::save(const char* nFileName)
{
	if(!data)
		return false;


	FILE* fp = fopen(nFileName, "wb");
	unsigned int size;

	if(!fp)
		return false;

	fwrite("TERPVS", 1, 6, fp);

	fputc(dataX, fp);
	fputc(dataY, fp);
	fputc(dataZ, fp);
	fputc(dataDZ, fp);

	size = fwrite(data, 1, dataSize, fp);
	fclose(fp);

	return size==dataSize;
}


bool
RTerrainPVS::load(const char* nFileName)
{
	MFile file;
	if( file.Open( nFileName ) == false )
		return false;

	char start[6];
	file.Read( start, 6 );

	if(strncmp("TERPVS", start, 6))
		return false;

	char temp;
	file.Read( &temp, 1 );
	dataX = temp;

	file.Read( &temp, 1 );
	dataY = temp;

	file.Read( &temp, 1 );
	dataZ = temp;

	file.Read( &temp, 1 );
	dataDZ = temp;

	unsigned int	numData = dataZ * dataY * dataX * dataY * dataX;
	dataSize = (numData+7)/8;			// +7 => round upwards

	if(data)
		SAFE_DELETE_ARRAY(data);

	data = new unsigned char[dataSize];
	return file.Read( data, dataSize );
}


void
RTerrainPVS::setVisible(int nX0, int nY0, int nZ0, int nX1, int nY1, bool nValue)
{
	unsigned int	idx = getIdx(nX0,nY0,nZ0, nX1,nY1);
	int				midx = idx>>3,
					sidx = idx&7;
	unsigned char	mask = (unsigned char)(1 << sidx);

	assert(idx < dataZ * dataY * dataX * dataY * dataX);

	if(nValue)
		data[midx] |= mask;
	else
		data[midx] &= ~mask;
}


bool
RTerrainPVS::isVisible(int nX0, int nY0, int nZ0, int nX1, int nY1) const
{
	if(nX0<0 || nX0>=(signed)dataX || nY0<0 || nY0>=(signed)dataY || nZ0<0 || nZ0>=(signed)dataZ ||
	   nX1<0 || nX1>=(signed)dataX || nY1<0 || nY1>=(signed)dataX)
		return true;

	unsigned int	idx = getIdx(nX0,nY0,nZ0, nX1,nY1);
	int				midx = idx>>3,
					sidx = idx&7;
	unsigned char	mask = (unsigned char)(1 << sidx);

	return (data[midx] & mask) != 0;
}


unsigned int
RTerrainPVS::getIdx(int nX0, int nY0, int nZ0, int nX1, int nY1) const
{
	return nZ0 * dataX*dataY*dataX*dataY + nY0 * dataX*dataX*dataY + nX0 * dataX*dataY + nY1 * dataX + nX1;
}

}