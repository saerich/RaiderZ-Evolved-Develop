//////////////////////////////////////////////////////////////////////////
// REnvironmentSet
// ������ ���� �׸��ڵ��� ����/������Ʈ ���ִ� �ų��

#pragma once

#include "RSceneNode.h"
#include "REnvironment.h"
#include "MSingleton.h"

namespace rs3 {

class REventBroadcaster;
class RWeatherEffectSet;
class RCollisionTree;

class RS_API REnvironmentSet : public RSceneNode	//, public MSingleton<REnvironmentSet>
{
	// fading ����
	struct EnvironmentFadingInfo
	{
		REnvironment::SEnvSet *pEnvSet;			///< ȯ�� ����
		RWeatherEffect *pWeather;				///< ���� ����Ʈ

		REnvironment::ETime eTime;
		REnvironment::EWeather eWeather;

		EnvironmentFadingInfo() : pEnvSet(NULL), pWeather(NULL)
			, eTime(REnvironment::TIME_NUM), eWeather(REnvironment::WEATHER_NUM) {}

		bool Create(RSceneNode* pSceneNode);
		void Destroy();
	};

protected://�����κ��� ������
	virtual void				OnAddScene()	override;

public:
	RTexture* hTillingTexture;
	struct STillingInfo
	{
		float fVisibility;
		RMatrix matTransform;
		RVector vecScale;
		RVector vecTranslate;
	};

	std::vector<STillingInfo> m_vecTilling;

	REnvironmentSet();
	REnvironmentSet(RWeatherEffectSet* pWeatherEffectSet);
	virtual ~REnvironmentSet();

	REnvironment::SEnvSet* REnvironmentSet::GetCurrentEnvSet();

	REnvironment* GetCurrentEnvironment();						// ���� ȯ�� ����ü�� �����͸� �����´�.
	REnvironment* CreateEnvironment(const std::string &strName);// �ش� ȯ���� �����Ѵ�.

	void SetCurrentEnvironment(const std::string &strName);		// ���� ȯ�� (�⺻���� default) �̸��� �����Ѵ�.
	void DeleteEnvironment(const std::string &strName);			// �ش� ȯ���� �����Ѵ�.

	void GetNames(std::vector<std::string> &vecArray);			// ȯ�� �̸� ����� ���´�.

	// ������ �б�
	bool CreateFromFile(const char *szFilename, bool bBackgroundCreation);
	bool CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	bool LoadWeatherFromXML();

	// ������ ����
	void SaveToFile(const char *szFilename);
	void SaveToXML(MXmlElement &element);

	// ETime, EWeather�� ������ �⺻ ���� �����Ѵ�. �׻� Set �Ŀ� CommitEnvSet�� �� �־�� ���� ȯ���� ����ȴ�.
	void SetDefaultEnvSet();

	// �ش� ETime, EWeather�� ���� EnvSet���� �����Ѵ�.
	bool SetCurrentEnvSet(REnvironment::ETime eTime, REnvironment::EWeather eWeather, bool bFade = false, float fFadingTime = 0.0f, bool bBackgroundLoading = true, RCollisionTree* pCollisionTree = NULL);

	// ������ �׸��� ���� ����
	void SetRenderWeather(bool bRenderWeatherEffect);
	void SetApplyFogEnable(bool bApplyFogEnable ) { m_bApplyFog = bApplyFogEnable; }

	RVector3 GetHDRBrightPassThreshold() { return m_vHDRBrightPassThreshold; }
	float GetHDRBloomBlurDeviation() { return m_fHDRBloomBlurDeviation; }
	float GetHDRBloomBlurMultiplier() { return m_fHDRBloomBlurMultiplier; }

	float GetSubLightIntensity() { return m_fSubLightIntensity; }


	void Update( RCollisionTree* pCollisionTree );

	void CommitEnvSet();

	bool IsBlending();		///< ���� ���ΰ�
	void EndBlending();		///< ���� ����
	float GetBlendingFactor();				///< ���� ���� 1->0 ���� ����

	// ���� Ŭ���̾�Ʈ �ڵ�� ȣȯ �۵� �ǰ�...
	static REnvironmentSet*	m_pCurrentSetOwn;	// ȯ�� ���� ���� ���� ���� ���¿��� GetInstancePtr�� ȣ�� �Ǹ� ���� �ӽ÷� �����ؼ� �����ش�. 
	static REnvironmentSet*	m_pCurrentSet;
	void					SetCurrent();
	static REnvironmentSet* GetInstancePtr();
	static void				DeleteOwnSet();


	REnvironment::SEnvSet	GetEnvSet()	{ return m_DefaultSet; }
	

protected:
	// ������ WEATHER_NUM ��ŭ�� ������. ���� ���� ����Ʈ ȿ��
	RWeatherEffectSet*		m_pWeatherEffectSet;
	bool					m_bExternalWeatherEffectSet;

	// ������ ȯ���� ���� �����Ų��. 
	// ( bChangeEnv�� ���� eTime, eWeather, Environment Name �� �ٲ���� ��  true�� �ִ´�. SEnvSet ��ü�� ���� �ٲ���� ���� false�� �ִ´�.)

	void UpdateLoading();
	void UpdateBlending();
	void UpdateWeather( float fTime, RCollisionTree* pCollisionTree );
	void UpdateCloud( float fTime );

	void ClearAll();		// current blending queue �� ��� Ŭ����
	void Release();			// ���ҽ����� ����

	void Init();

private:
	void CommitWeatherEnv(float fBlendFactor);
	void CommitWindEnv(float fBlendFactor);
	void CommitFogEnv(float fBlendFactor);
	void CommitSkyEnv(float fBlendFactor);
	void CommitLightEnv(float fBlendFactor);
	void CommitHDREnv(float fBlendFactor);
	void CommitCloudShadow(float fBlendFactor);


	// �޽����� ��ũ�� �Ǵ� �ؽ��� Ÿ�ϸ� ����Ʈ �̸�
	static char *TILINGEFFECT_TEXTURE_FILENAME;

	typedef std::map<std::string, REnvironment*> ENVIRONMENTSET;
	ENVIRONMENTSET m_EnvironmentSetTable;

	// ���� ���� ���� ������
	REnvironment *m_pCurrentEnvironment;

	// �⺻ ��
	REnvironment::SEnvSet m_DefaultSet;

	// ���� �ð�/������ ������ ��
	REnvironment::ETime m_eCurrentTime;
	REnvironment::EWeather m_eCurrentWeather;

	EnvironmentFadingInfo m_currentSet;				// ���� ����Ǵ� ��.
	EnvironmentFadingInfo m_blendingSet;			// �ٷ� ������ ȯ�� �����̸�, ���� ���� ���� ��.
	bool				m_bBlendingEnabled;			///< ������ Ȱ��ȭ �Ǿ��ִ��� ����
	float				m_fFadingTime;				///< �� ���� �ð�
	float				m_fLifeTime;				///< ���� ���� �ð�

	bool		m_bRenderWeatherEffect;	// ������ ȭ�鿡 �׸��� ����
	bool		m_bApplyFog;		///< ���׸� ȭ�鿡 �������� ����

	RVector3	m_vHDRBrightPassThreshold;	///< HDR ��� �����Ʈ �н� ��
	float		m_fHDRBloomBlurDeviation;	///< HDR ��� �� �л���
	float		m_fHDRBloomBlurMultiplier;	///< HDR ��� �� ����ġ

	float		m_fSubLightIntensity;
};

}
