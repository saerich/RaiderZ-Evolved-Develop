#ifndef _XCONST_H
#define _XCONST_H

#include "MTypes.h"

/// �����
class CORETOOL_API XCONST
{
public:
	// ���� ��� --------------------------
	static int			CAMERA_FOV;							// FOV (����)
	static float		CAMERA_NEAR_Z;
	static float		CAMERA_FAR_Z;

	static float		TREE_LOD_NEAR;
	static float		TREE_LOD_FAR;

	static float		GRASS_LOD_NEAR;
	static float		GRASS_LOD_FAR;


	// ���� ������ --------------------------
	static int			GAME_CAMERA_FOV;
	static float		GAME_CAMERA_NEAR_Z;
	static float		GAME_CAMERA_FAR_Z;

	static float		GAME_TREE_LOD_NEAR;
	static float		GAME_TREE_LOD_FAR;

	static float		GAME_GRASS_LOD_NEAR;
	static float		GAME_GRASS_LOD_FAR;


	static bool Load(const char* szFileName);
};




#endif