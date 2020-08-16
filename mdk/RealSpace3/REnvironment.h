#pragma once

#include "RTypes.h"
#include <map> 
#include <utility>
#include <string>
#include <vector>
#include <queue>
#include "RWeatherEffect.h"

namespace rs3 {

class RSkyManager;
class REffectSceneNode;
class REnvironmentSet;

class RS_API REnvironment
{
public:
	enum ETime
	{
		DAWN = 0,
		DAYTIME,
		SUNSET,
		NIGHT,
		TIME_NUM
	};

	enum EWeather
	{
		SUNNY = 0,
		CLOUD,
		RAINY,
		HEAVY_RANIY,
		SNOW,
		HEAVY_SNOW,
		WEATHER_NUM
	};

protected:
	static char *m_pcTimeTable[TIME_NUM];
	static char *m_pcWeatherTable[WEATHER_NUM];

public:
	struct SEnvSet
	{
		// �ϴ�
		RSkyManager *pSkyManager;
		RTexture	*pEnv_Texture;

		// Light
		RVector4	vAmbient;
		RVector4	vDiffuse;
		float		fPower;
		float		fShadowLuminosity;
		float		fSubLightIntensity;

		// HDR
		RVector3	vHDRBrightPassThreshold;
		float		fHDRBloomBlurDeviation;
		float		fHDRBloomBlurMultiplier;

		// fog
		RVector4 vFogColor;
		bool bFogDisable;

		// near-far
		float fNear;
		float fFar;

		// Wind
		RVector	vWindDirection;
		float fWindStrength;

		// cloud shadow
		bool m_bCloudVisible;
		RVector3 m_vCloudShadowTranslate;
		RVector3 m_vCloudShadowScale;

		// Ǯ
		float		m_fGrassJigglingPeriod;		// �����ֱ�
		float		m_fGrassJigglingStrength;	// �������
		float		m_fGrassIncliningStrength;	// ��￩�����

		// SEnvSet �� �⺻ �� / �⺻ �����ڷμ� �ʱ�ȭ
		SEnvSet()
			: pSkyManager(NULL)
			, vAmbient(.4f, .4f, .4f, .4f)
			, vDiffuse(1,1,1,1)
			, fPower(1.f)
			, fShadowLuminosity(0.1f)
			, vHDRBrightPassThreshold(1.0f, 0.9f, 0.9f)
			, fHDRBloomBlurDeviation(3.f)
			, fHDRBloomBlurMultiplier(2.f)
			, vFogColor(1,1,1,1)
			, bFogDisable(false)
			, fNear(100000.0f)
			, fFar(300000.0f)
			, vWindDirection(0, 0, 0)
			, fWindStrength(0.0f)
			//, pWeather(NULL)
			, m_bCloudVisible(false)
			, m_vCloudShadowScale( 10000.f, 10000.f, 0 )
			, m_vCloudShadowTranslate( 300.f, 300.f, 0 )
			, m_fGrassJigglingPeriod(0.0f)
			, m_fGrassJigglingStrength(0.0f)
			, m_fGrassIncliningStrength(0.0f)
			, pEnv_Texture( NULL )
			, fSubLightIntensity(1.0f)
		{
		}
	};

protected:
	typedef std::pair<int, int> ENVKEY;
	typedef std::map<ENVKEY, SEnvSet*> ENVTABLE;
	ENVTABLE m_kEnvTable;

	// �ܺο����� ������ �ʰ�, ���������θ� ����.
	void Destroy();
	void DelEnv(ETime eTime, EWeather eWeather);

public:
	REnvironment();
	virtual ~REnvironment();

	static const char *ETimeToString(ETime eTime);
	static const char *EWeatherToString(EWeather eWeather);

	static ETime StringToETime(const char *szName);
	static EWeather StringToEWeather(const char *szName);

	void FillEnvFromDefault();
	SEnvSet* GetEnv(ETime eTime, EWeather eWeather);
	SEnvSet* CreateEnv(ETime eTime, EWeather eWeather);

	bool CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	bool ReadEnvSetContents(SEnvSet* pEnvSet, MXmlElement* pChildElement);
	void SaveToXML(MXmlElement &element);

private:
	void LoadEnvironmentTexture( SEnvSet* _pEnvSet, MXmlElement* _pChildElement );
};

};