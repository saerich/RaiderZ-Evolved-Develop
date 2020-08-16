#pragma once

namespace rs3 {

struct RRESOURCETYPE
{
	// -----------------------------------------
	// �� �Ŵ����κ��� �����Ǵ°�
	// -----------------------------------------
	enum SCENE_MANAGER_MANAGED
	{
		RR_MESH = 0,
		RR_TREE,
		RR_WATER,
		RR_MATERIAL,
		RR_ANIMATION,
		RR_GENERAL,

		// ��
		RR_MAX
	};

	// -----------------------------------------
	// �� �Ŵ����κ��� �������� �ʴ� ���ҽ�
	// -----------------------------------------
	enum UNMANAGED_RESOURCE
	{
		RR_UNKNOWN		= RR_MAX,
		RR_TEXTURE,			// RDevice���� �����ȴ�.
		RR_SHADERFX,		// RDevice���� �����ȴ�.
		RR_TERRAIN,
		RR_STATICMESH,

		RR_UNMANAGED_MAX
	};

	static const char* GetResoruceIDString( int _nId )
	{
		if( _nId<0 || _nId>RR_UNMANAGED_MAX)
			return NULL;

		static const char * pSzResourceID[] = {

			//-------------- managed
			"mesh",
			"tree",
			"water",
			"material",
			"animation",
			"general",

			"_managed_max",

			//-------------- unmanaged
			"texture",
			"shaderfx",
			"terrain",
			"staticmesh",

			"_unmanaged_max"
		};
		return pSzResourceID[ _nId ];
	}
};


}
