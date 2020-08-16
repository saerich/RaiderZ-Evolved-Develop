#include "stdafx.h"
#include "M_ToolTerrain.h"
#include "RGrassSystem.h"
#include <string>
#include "UnitTest++.h"

// WB �� �ִ� �Լ��� �����ͼ� �׽�Ʈ
namespace WB_TerrainAction_NS {
	float fEditHeight(float fHeight, float fRange, float fScale, float fFactor);
};

SUITE(Terrain)
{
	TEST(TestTerrainEdit)
	{
		rs3::RToolTerrain* pTerrain = new rs3::RToolTerrain(rs3::REngine::GetDevicePtr());
		
		// �׽�Ʈ �Ҽ� �ִ� �ּ� ������ 32 �ε�.
		const int CHECK_SIZE = 32;
		bool bCreated = pTerrain->CreateDisplacementmapPlane(CHECK_SIZE + 1, CHECK_SIZE + 1, CHECK_SIZE, CHECK_SIZE);
		CHECK(bCreated);

		// ������ 10���� 10��ŭ �÷�����
		MVector3 mouseVec( 16,16,0 );
		pTerrain->ApplyCircleActionWithFuncPtr(&mouseVec, 10.f, 10.f, 0.0f, WB_TerrainAction_NS::fEditHeight);

		// �ٿ�� �ڽ��� �� �˻�
		RHeightField* pHeightField = pTerrain->GetHeightFieldPtr();
		CHECK_CLOSE( 0, pHeightField->GetMinZ(), 0.1f );
		CHECK_CLOSE( 10, pHeightField->GetMaxZ(), 0.1f );

		delete pTerrain;
	}

	TEST(TerrainCreateXML)
	{
		using namespace rs3;

		class RTestToolTerrain : public RToolTerrain
		{
		public:
			const char * GetGrassName()
			{
				return m_strGrassName.c_str();
			}

			bool CreateAddDummyLayer()
			{
				RTexture *pNullTexture = NULL;
				return AddLayer(pNullTexture, pNullTexture, false);
			}

			RTestToolTerrain(RDevice *pDevice)
				:RToolTerrain(pDevice)
			{
			}

			using RToolTerrain::SetResourceName;
		};

		const char *RESOURCE_NAME = "UNIT_TEST";

		// Dummy Terrain�� �ϳ� �����Ѵ�.
		RTestToolTerrain *pTerrain = NULL;
		pTerrain = new RTestToolTerrain(REngine::GetDevicePtr());
		CHECK(pTerrain);
		pTerrain->SetResourceName(RESOURCE_NAME);
		bool bCreateTerrain = pTerrain->CreateDisplacementmapPlane(33, 33, 100.f, 100.f);
		CHECK(bCreateTerrain);

		bool bCreateGrass = pTerrain->CreateGrass();
		CHECK(bCreateGrass);

		bool bCreateLayer = pTerrain->CreateAddDummyLayer();
		CHECK(bCreateLayer);

		// xml�� �ϳ� �����, Save XML Element
		MXml xml;
		MXmlDeclaration* dec = xml.CreateDeclaration( "1.0", "UTF-8", "" );
		xml.Doc()->LinkEndChild( dec );

		MXmlElement *pElement = new MXmlElement( "PROPERTY" );
		xml.Doc()->LinkEndChild( pElement );

		pTerrain->SaveToElement(pElement);
		delete pTerrain;
		pTerrain = NULL;

		pTerrain = new RTestToolTerrain(REngine::GetDevicePtr());
		CHECK(pTerrain);

		bool bLoadTerrainElement = pTerrain->LoadFromXMLElement(pElement, false);
		CHECK(bLoadTerrainElement);

		CHECK( strcmp(pTerrain->GetDisplacementMapName(),	"UNIT_TEST_dis.dds")==0 );
		CHECK( strcmp(pTerrain->GetGrassName(),				"UNIT_TEST_grass.grs")==0 );
		CHECK( pTerrain->GetLayerNum() == 1 );
		CHECK( pTerrain->GetLayerInfo(0)->alphaMapName ==	"UNIT_TEST_mask0.dds");

		if ( pTerrain )
		{
			delete pTerrain;
			pTerrain = NULL;
		}
	}
}
