/**
@brief .grs ���� ������ �δ��� ��Ƴ��� ��
*/

#pragma once

#include "MFileSystem.h"

namespace rs3 {

class RSpeedGrass;
class RGrassSystem;

class RGrassLoader
{
public:
	RGrassLoader() {}

	virtual bool Load(MFile& file, int bc, RSpeedGrass* pGrass, RGrassSystem* pGrassSystem, RTerrain* pTerrain) = 0;

	static RGrassLoader*	CreateLoader( DWORD dwVersion );
	static DWORD			GetCurrentVersion();
};

}