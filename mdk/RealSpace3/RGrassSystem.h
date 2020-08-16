#pragma once

#include <vector>
#include "RSpeedGrass.h"

class RHeightField;

namespace rs3
{

	class RShaderFX;
	class RSceneManager;
	class RDevice;
	class RSceneManager;
	class RSpeedGrassGPUInstance;
	class RTerrain;

	class RGrassSystem_WindStrengthAmplifier
	{
	private:
		float m_fElapsedTime;
		float m_fAmplifier;
		float m_fNextUpdateTime;

	protected:
		float m_fStrengthCoefficient;

		virtual float GetNewAmplifier();
		virtual float GetNewUpdateTime();
	public:
		RGrassSystem_WindStrengthAmplifier();

		float Strength(float fDeltaTime, float fWindStrength);
	};

	class RS_API RGrassSystem
	{
		friend RSpeedGrassGPUInstance;

	protected:
		RSpeedGrass*			m_pGrass;
		RDevice*				m_pDevice;

		float					m_fTime;

		RTexture*				m_hGrassCompositeTexture;
		string					m_strTextureFilename;
		string					m_strTextureInfoFilename;

		bool					m_bRendering;
		bool					m_bNeedUpdate;

		int						m_nRenderGrassCount;

		RSceneManager* m_pSceneManager;
		RGrassSystem_WindStrengthAmplifier	m_WindStrengthAmplifier;
	public:
		string					m_strGrassPath;
		/**
		* GrassSystem�� �ʱ�ȭ �Ѵ�.
		* @param	dev		RDevice�� ������
		* @return	�ʱ�ȭ �����ÿ� true�� �����Ѵ�.
		*/
		bool					Create(RDevice* dev, RSceneManager* pSceneManager);
		/**
		* GrassSystem�� ���� �ڿ��� �����Ѵ�.
		*/
		void					Destroy();

		/**
		* GrassSystem�� ���� �ø� ������ Region�� �����Ѵ�. 
		* �� Region �Ʒ��� Ǯ�� ���� ������ �ٵ��� �Ǿ� �����Ƿ�, ����ϱ� ���� �ݵ�� ������ �Ѵ�.
		* @param	row		�� ��
		* @param	col		�� ��
		* @param	fWidth	���� ����
		* @param	fHeight ���� ����
		*/
		void					CreateRegions(int row, int col, float fWidth, float fHeight);
		/**
		* Ǯ�� �߰��Ѵ�.
		* @param	RGassBlade	Ǯ�� ���� ����ü(RGassBlade)
		*/
		bool					AddBlade(const RGrassBlade& b);	// ��� �����ϰ� ���� - 090610, OZ
		/**
		* ���� ����(��) ���� Ǯ�� �����.
		* @param	x	���� �߽� x��ǥ
		* @param	y	���� �߽� y��ǥ
		* @param	fRadius		���� ������
		*/
		void					DeleteBlade(float x, float y, float fRadius);
		/**
		* ���� Ǯ�� ��ġ ������ �����´�.
		* @param	outBlades	��ġ ������ ���� vector ����
		*/
		void					GetBladesArray(vector<RGrassBlade> &outBlades);
		/**
		* ��� Ǯ�� �����Ѵ�.
		*/		
		void					DeleteAllBlade();
		/**
		* ����� Ǯ�� �ݿ��Ѵ�. AddBlde, DeleteBlade�Ŀ��� ȣ���Ͽ� �ݿ����־�� ����� �������� �ȴ�.
		*/
		void					UpdateDXBuffers( RTerrain* pTerrain);

		const char*				GetLoadedTextureName();
		void					LoadTexture(const char* filename);
		void					Render(RSceneManager* pManager, RCameraSceneNode* pCamera);
		void					Update(RSceneManager* pManager);
		int						GetCurrentRenderGrassCount();

		//void					SetLodParam(float farlod, float lodtransition) { m_fFarLod = farlod; m_fLodTransition = lodtransition; }

		bool					SaveToFile(const string& filename);
		bool					LoadFromFile( const string& filename, RTerrain* pTerrain);

		void					SetRenderable(bool r) { m_bRendering = r; }

		void					RenderGStage( RRENDERINGMETHOD method, RCameraSceneNode* pCamera);
		void					RenderMStage( RCameraSceneNode* pCamera);
		/**
		* Ǯ�� ���� ���̿� ���� ������Ʈ
		*/
		class ICallback
		{
		public:
			virtual void OnBlade(RGrassBlade &b) = 0;
		};
		void					UpdateOnCallback(ICallback *piCallback);

		void					LoadGrassListFromXML(MXmlElement &xmlRoot, const char *szNameA, const char *szNameB);

		void					SetWindDirection(const RVector& windDir) { m_pGrass->SetWindDirection(windDir); }
		void					SetWindStrength(float windStr) { m_pGrass->SetWindStrength(windStr); }				// 0(�ٶ�����)~1(�ִ�)


		void					SetGrassJigglingPeriod(float f)		{	m_pGrass->SetGrassJigglingPeriod(f);	}
		void					SetGrassJigglingStrength(float f)	{	m_pGrass->SetGrassJigglingStrength(f);	}
		void					SetGrassIncliningStrength(float f)	{	m_pGrass->SetGrassIncliningStrength(f);	}

		int						GetGrassID(const char *szFilename);

		int						GetGrassWidth(int nIndex);
		int						GetGrassHeight(int nIndex);

		void					SetAmbientColor(RVector4 &vColor);
		RVector4				GetAmbientColor();

		void					SetDiffuseColor(RVector4 &VColor); 
		RVector4				GetDiffuseColor();

		RGrassSystem();
		virtual ~RGrassSystem();
	};
}
