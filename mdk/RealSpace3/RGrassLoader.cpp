#include "StdAfx.h"
#include "RGrassLoader.h"
#include "RSpeedGrass.h"
#include "RGrassSystem.h"
#include "RTerrain.h"

namespace rs3 {

const DWORD	GRS_VERSION3 = 0x00000003;
const DWORD GRS_VERSION4 = 0x00000004;
const DWORD GRS_VERSION5 = 0x00000005;
const DWORD GRS_VERSION6 = 0x00000006;

const DWORD GRS_VERSION = GRS_VERSION6;

// Ǯ ��鸲 ũ�⸦ ���Ͽ� ����� ���� �����ϰ� �������� - charmke
// TODO: ���� ������ �ٲٴ���.. ����ϰ� ������ �ʿ�. - charmke, ��� ���� ? - rOseria
class RGrassLoaderImpl_V4 : public RGrassLoader
{
public:
	virtual bool Load(MFile& file, int bc, RSpeedGrass* pGrass, RGrassSystem* pGrassSystem, RTerrain* pTerrain)
	{
		// packed-blade to blade
		RGrassBlade b;

		CSpeedGrassRT::SPackedBlade4* PackedBlade = new CSpeedGrassRT::SPackedBlade4[bc];
		file.Read( PackedBlade, sizeof(CSpeedGrassRT::SPackedBlade4)*bc );

		for (int i=0; i<bc; i++)
		{
			PackedBlade[i].Unpack(b);
			b.m_fThrow = RandomNumber( c_fMinBladeThrow, c_fMaxBladeThrow );
			// �븻 ����
			// float[]�� RVector�� �ٷ� �Ѱܵ� �ȴ�.
			if( pTerrain)
				pTerrain->GetNormalVector( (RVector*)b.m_afPos, (RVector*)b.m_afNorm);
			pGrassSystem->AddBlade(b);
		}
		delete[] PackedBlade;
		return true;
	}
};

class RGrassLoaderImpl_V5 : public RGrassLoader
{
public:
	virtual bool Load(MFile& file, int bc, RSpeedGrass* pGrass, RGrassSystem* pGrassSystem, RTerrain* pTerrain)
	{
		// packed-blade to blade
		RGrassBlade b;

		CSpeedGrassRT::SPackedBlade5* PackedBlade = new CSpeedGrassRT::SPackedBlade5[bc];
		file.Read( PackedBlade, sizeof(CSpeedGrassRT::SPackedBlade5)*bc );

		for (int i=0; i<bc; i++)
		{
			PackedBlade[i].Unpack(b);
			b.m_fThrow = RandomNumber( c_fMinBladeThrow, c_fMaxBladeThrow);
			// �븻 ����
			// float[]�� RVector�� �ٷ� �Ѱܵ� �ȴ�.
			if( pTerrain)
				pTerrain->GetNormalVector( (RVector*)b.m_afPos, (RVector*)b.m_afNorm);
			pGrassSystem->AddBlade(b);
		}

		delete [] PackedBlade;
		return true;
	}
};

// �븻 ���� �߰�
class RGrassLoaderImpl_V6 : public RGrassLoader
{
public:
	virtual bool Load(MFile& file, int bc, RSpeedGrass* pGrass, RGrassSystem* pGrassSystem, RTerrain* pTerrain)
	{
		// packed-blade to blade
		RGrassBlade b;

		CSpeedGrassRT::SPackedBlade6* PackedBlade = new CSpeedGrassRT::SPackedBlade6[bc];
		file.Read( PackedBlade, sizeof(CSpeedGrassRT::SPackedBlade6)*bc );

		for (int i=0; i<bc; i++)
		{
			PackedBlade[i].Unpack(b);
			b.m_fThrow = RandomNumber( c_fMinBladeThrow, c_fMaxBladeThrow);
			pGrassSystem->AddBlade(b);
		}

		delete [] PackedBlade;
		return true;
	}
};

RGrassLoader* RGrassLoader::CreateLoader( DWORD dwVersion )
{
	if ( dwVersion != GRS_VERSION )
		mlog("grass version is not lastest\n");

	if ( dwVersion == GRS_VERSION4 )
		return new RGrassLoaderImpl_V4;
	else if ( dwVersion == GRS_VERSION5 )
		return new RGrassLoaderImpl_V5;
	else if ( dwVersion == GRS_VERSION6 )
		return new RGrassLoaderImpl_V6;

	mlog("grass version error\n");
	return NULL;
}

DWORD RGrassLoader::GetCurrentVersion()
{
	return GRS_VERSION;
}

}