#pragma once

#include <queue>
#include "RSceneNode.h"
#include "RActor.h"

namespace rs3 {

class RWeatherEffect;
class REffectSceneNode;

//////////////////////////////////////////////////////////////////////////
// REnvironmentSceneNode
// ������ ���� �׸��ڵ��� ����/������Ʈ ���ִ� �ų��
//
class REnvironmentSceneNode : public RSceneNode
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Environment Piece�� �����ϴ� Key : Time �� Weather
	//
	enum ETime
	{
		DAWN = 0,
		DAYTIME,
		SUNSET,
		EVENING,
		NIGHT,
		TIME_NUM
	};

	enum EWheather
	{
		SUNNY = 0,
		CLOUD,
		RAINY,
		HEAVY_RANIY,
		SNOW,
		HEAVY_SNOW,
		CLEAR,
		WHEATHER_NUM
	};

	// enum - string ��ȯ
	static const char*	ETimeToString(ETime eTime);
	static const char*	EWheatherToString(EWheather eWheather);

	static ETime		StringToETime(const char *szName);
	static EWheather	StringToEWheather(const char *szName);


	//////////////////////////////////////////////////////////////////////////
	// REnvironmentPiece
	// REnvironmentPiece�� 5�� Time(DAWN, DAYTIME, ...) �� 7�� Weather(SUNNY, ...) ��
	// 35�� ������ �ϳ��� ����(EnvBunch)�� �Ͽ� REnvironmentSceneNode���� �����մϴ�.
	//
	class REnvironmentPiece
	{
	public:

		// Light
		RVector4		vAmbient;
		RVector4		vDiffuse;
		RVector4		vCharacterAmbient;
		RVector4		vCharacterDiffuse;
		float			fPower;

		// fog
		RVector4		vFogColor;
		bool			bFogDisable;

		// near-far
		float			fNear;
		float			fFar;

		// Wind
		RVector			vWindDirection;
		float			fWindStrength;

		// cloud shadow
		bool			m_bCloudVisible;
		RVector3		m_vCloudShadowTranslate;
		RVector3		m_vCloudShadowScale;


		//////////////////////////////////////////////////////////////////////////
		// Clear EnvironmentPiece
		//
		bool			ReloadEnvPiece(MXmlElement* pChildElement);
		void			Clear(){ ResetEnvPiece(true, true); }


		//////////////////////////////////////////////////////////////////////////
		// sky add / delete
		//
		int				AddSky(const char* _pSzFileName);
		int				AddSky(MXmlElement& xmlElementForSky);
		bool			DeleteSky(int _nIndex);

		// sky layer ordering
		int				GetAllSkiesCount(){ return m_vecSkyArray.size(); }
		void			ChangeSkyOrder( int _nLayerToChange, bool bIncrease );

		// Sky Layer ��ü�� ������ ��ġ�� color
		void			SetSkyMultiplyColorFactor(RVector4 &vec4){ m_vSkyMultiplyColor = vec4; }
		const RVector4&	GetSkyMutiplyColorFactor(){ return m_vSkyMultiplyColor; }

		// ���� sky�� ������ ��ġ�� color
		void			SetSkyColor( int _nIndex, DWORD dwColor );
		void			SetSkySpeed( int _nIndex, float f );
		void			SetFogFactor( int _nIndex, float f );

		//////////////////////////////////////////////////////////////////////////
		// Sky Info
		//
		struct SSKY_INFO
		{
			int			nIndex;
			std::string	strName;

			DWORD		dwColor;
			float		fSpeed;
			float		fFogFactor;
		};
		void			GetSkyInfo(int _nIndex, SSKY_INFO& _refSkyInfo);
		void			GetAllSkiesInfo( std::vector< SSKY_INFO >& _refVecSkyInfo );

	private:
		friend class REnvironmentSceneNode;
		friend class REnvironmentBunch;

		// REnvironmentPiece �� �⺻ ��. REnvironmentBunch/REnvironmentSceneNode�� ���ؼ��� ��������
		REnvironmentPiece() : m_pEnvSceneNode(NULL) { ResetEnvPiece(true, false); }
		~REnvironmentPiece(){ ResetEnvPiece(false, true); }

		// sky color (all skies in layer)
		RVector4			m_vSkyMultiplyColor;

		// sky XML ����
		static const char*	ELEMENT_FILENAME;	
		static const char*	ELEMENT_SPEED;
		static const char*	ELEMENT_FOGFACTOR;
		static const char*	ELEMENT_COLOR;

		// Sky Actor
		class RSkySceneNode : public RActor
		{
			// REnvironmentPiece���� ����Ǹ��� ������.
			friend REnvironmentPiece;

			RSkySceneNode() : RActor(RRP_SKYBOX)
				, m_nOrder(-1)
				, m_dwColor(0xffffffff)
				, m_fUVAniSpeed(1.f)
			{
				m_fFogFactor = 1.0f;
				m_matLocal.MakeIdentity();
				m_bUsingActorPassRenderer = false;
			}
			virtual ~RSkySceneNode(){}

			// RSkySceneNode�� ��ü���� �������� ������Ʈ�� ������.
			virtual void	OnUpdate(){ _ASSERT(!"RSceneSky�� ��ü���� ������Ʈ �ý����� �����ϴ�."); }
			virtual void	OnRender(){ _ASSERT(!"RSceneSky�� ��ü���� ������ �ý����� �����ϴ�."); }
			virtual void	OnRegisterToRender(){ _ASSERT(!"RSceneSky�� ��ü���� ������ ��Ͻý����� �����ϴ�."); }

			DWORD			m_dwColor;		// sky multiply color
			float			m_fFogFactor;	// get/set fog factor
			float			m_fUVAniSpeed;	// UV Animation Speed
			int				m_nOrder;		// layer order sorting �� �� ���ȴ�.
			static bool		eval_order(RSkySceneNode *a, RSkySceneNode *b);

			// ���� / ������Ʈ / ������
			virtual bool	Create(const char *filename, bool bBackgroundCreation = false);
			void			UpdateSky(){ RActor::OnUpdate(); }
			void			RenderSky(const RVector4 &vMultiplyColor);

			// get sky mesh filename
			const char*		GetFilename();
		};
		std::vector<RSkySceneNode*> m_vecSkyArray;

		// Private Method
		void RenderEnvPiece();
		void UpdateEnvPiece();
		void ReOrderSky();

		void OnAddToCurrnetEnvironment(REnvironmentSceneNode* pEnvironmentSceneNode);
		void OnRemoveFromCurrentEnvironment();

		void ResetEnvPiece(bool bResetInitValue, bool bDeepDeleteSky);
		REnvironmentSceneNode* m_pEnvSceneNode;
	};


	//////////////////////////////////////////////////////////////////////////
	// XML���� �б�/XML�� ����ϱ�, ������Ʈ/����
	//
	virtual bool		CreateFromXMLFile(const char *szFilename, bool bBackgroundCreation);
	virtual void		SaveToXMLFile(const char *szFilename);


	//////////////////////////////////////////////////////////////////////////
	// EnvironmentBunch, 35���� EnvPiece �������� �ϳ��� �̸����� ����(Bunch) �����Ѵ�.
	//
	bool				AddEnvBunch(const char* _pSzEnvBunchName);					// �ش� ȯ���� �߰�. �߰��� CurrentEnvBunch�� �ٲܼ� �ִ�.
	bool				DeleteEnvBunch(const char* _pSzEnvBunchName);				// �ش� ȯ�� ����. ������ Default EnvBench�� Currnet�� ����

	void				GetAllEnvBunchesList(std::vector<std::string> &vecArray);	// ȯ�� �̸� ����� ���´�.
	bool				ClearAllEnvBunches();										// ��ü ȯ�湭������ �ʱ�ȭ


	//////////////////////////////////////////////////////////////////////////
	// EnvironmentPiece. �ð�/������ ȯ������(EnvPiece)���� Queue�� �����ϸ�
	// ������ EnvPiece�� ����ɶ� �ڿ������� ȯ���� ����Ǵ°͵��� �����˴ϴ�.
	// (�̷��� ������ EnvPiece�� ���� EnvPiece�� ����Ǵ� ������ EnvProcess��� �մϴ�.)
	//
	REnvironmentPiece*	GetEnvPiece(const char* pSzEnvBunchName, ETime eTime, EWheather eWeather);
	REnvironmentPiece*	GetCurrentEnvPiece(){ return m_pCurrentEnvPiece; }
	bool				SetCurrentEnvPiece(const char* pSzEnvBunchName, ETime eTime, EWheather eWeather, float fFadingTime = -1.0f);


	//////////////////////////////////////////////////////////////////////////
	// cloud shadow
	//
	RTexture* hTillingTexture;
	struct STillingInfo
	{
		float fVisibility;
		RMatrix matTransform;
		RVector vecScale;
		RVector vecTranslate;
	};
	std::vector<STillingInfo> m_vecTilling;


	//////////////////////////////////////////////////////////////////////////
	// weather effect
	//
	bool				LoadWeatherFromXML();
	void				SetRenderWeather(bool bRenderWeatherEffect);


protected:
	// RSceneNode�ν� ���� / ������Ʈ / ������
	virtual bool		CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	virtual void		SaveToXML(MXmlElement &element);

	virtual void		OnUpdate();
	virtual void		OnRender();

private:
	friend class REnvironmentPiece;
	friend class RSingletonSceneManager;

	// �� �Ŵ��������� ����
	REnvironmentSceneNode();
	virtual ~REnvironmentSceneNode();

	// �� �Ŵ����� ����, ���Ŵ����� ��������� �Ҹ�ɶ� �ѹ����� �Ҹ���.
	void	OnAddToSceneManager(RSingletonSceneManager* pSingletionSceneManager);
	void	OnRemoveFromSceneManager();

	// �޽����� ��ũ�� �Ǵ� �ؽ��� Ÿ�ϸ� ����Ʈ �̸�
	static char *TILINGEFFECT_TEXTURE_FILENAME;

	typedef std::pair<int, int>	ENV_KEY;
	typedef std::map<ENV_KEY, REnvironmentPiece*> ENV_BUNCH;
	typedef std::map< std::string, map<ENV_KEY, REnvironmentPiece*> > ENV_TABLE;
	ENV_TABLE m_kEnvTable;

	// ���� ȯ��
	std::string			m_strCurrentEnvBunch;
	ETime				m_eCurrentTime;
	EWheather			m_eCurrentWeather;
	REnvironmentPiece*	m_pCurrentEnvPiece;

	// fading ����
	struct SPTR
	{
		REnvironmentPiece*	pEnvPiece;
		RWeatherEffect*		pWeather;
		float				fFadingTime;
		float				fLifeTime;

		ETime				eTime;
		EWheather			eWeather;

		SPTR() : pEnvPiece(NULL), pWeather(NULL), fFadingTime(-1.0f), fLifeTime(0.0f)
			, eTime(TIME_NUM), eWeather(WHEATHER_NUM) {}
	};

	std::queue<SPTR>	m_queueEnvNexts;		// Queue�� �ִ� ���� ������ ���� ��.
	SPTR				m_currentSet;			// ���� ����Ǵ� ��.
	bool				m_bRenderWeatherEffect;	// ������ ȭ�鿡 �׸��� ����

	void				SetCurrentEnvPieceToDefault(){ SetCurrentEnvPiece("default", DAWN, SUNNY); }
	void				DeleteAllEnvBunch();

	void				ResetEnvProcess();
	void				UpdateEnvProcess();

	void				RemoveQueue();
	void				RemoveCurrent();
	void				Release();

	// ������ WEATHER_NUM ��ŭ�� ������. ���� ���� ����Ʈ ȿ��
	RWeatherEffect*		m_pWeatherEffectTable[WHEATHER_NUM];

	// enum - string data
	static char*		m_pcTimeTable[TIME_NUM];
	static char*		m_pcWheatherTable[WHEATHER_NUM];

	bool				m_bEnvPieceChanged;
};

}
