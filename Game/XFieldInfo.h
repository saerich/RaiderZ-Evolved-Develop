#ifndef _XFIELD_INFO_H
#define _XFIELD_INFO_H

#include "XDef.h"
#include "CSFieldInfo.h"
#include "XSensorInfo.h"
#include "XCylinder.h"


class XFieldInfoMgr;
namespace rs3 { class RResource; }

struct AREA_INFO
{
	int			nID;	
	bool		bPath;
	bool		bEnterLocal;
	vector<XCylinder>	veccylinder;

	wstring		GetName();

private:
	friend		XFieldInfoMgr;
	wstring		strName;		// ���ڿ�Ű�Դϴ�. ���� ������ �����ϴ�.
};


struct FIELDSOUND_INFO
{
	wstring		strName;
	vec3		vPos;
	float		fMinDist;
	float		fMaxDist;
	bool		bMuteAmbient;
	int			nReverb;
	bool		bContact;

	FIELDSOUND_INFO() : vPos( vec3::ZERO), fMinDist( 100.0f), fMaxDist( 1000.0f), bMuteAmbient( false), nReverb( -1), bContact( false)		{}
};

struct NPC_UIMAP_INFO
{
	enum INFO_TYPE
	{
		TYPE_CRAFT = 0,
		TYPE_SHOP,

		TYPE_CNT
	};

	int		nNPCID;
	vec3	vPos;

	wstring strTypeName;
	wstring strIconName;

	string strLookPart;
	string strLookState;

	NPC_UIMAP_INFO() : vPos( vec3::ZERO)	{}
};

typedef map<unsigned long, MARKER_INFO>		MARKER_INFO_MAP_BY_ID;
typedef map<wstring, MARKER_INFO>			MARKER_INFO_MAP_BY_NAME;
typedef map<unsigned long, XSensorInfo>		SENSOR_INFO_MAP_BY_ID;
typedef vector<AREA_INFO>					AREA_INFO_VECTOR;
typedef vector<NPC_UIMAP_INFO>				NPC_UIMAP_INFO_VECTOR;



/// �ʵ� ����( �� ���� + ��ƼƼ ���� )
class XFieldInfo : public CSFieldInfo
{
private:
	friend class XFieldInfoMgr;
	friend class MockWorld;
	using CSFieldInfo::m_strFieldName;

	static XSensorInfo	m_NullSensorInfo;

	void Cook_PvPAreas();
	void Cook_Sensors();
public:
	wstring			m_strZoneName;		///< �� �̸�
	wstring			m_strZoneFile;		///< �� ���� �̸�

	float			m_fMinX;
	float			m_fMaxX;
	float			m_fMinY;
	float			m_fMaxY;

	bool			m_bInstance;		///< �ν��Ͻ� �ʵ����� ����
	bool			m_bWeatherEffect;	///< ȯ�� ����Ʈ�� �������� ����(��, �� ��)
	bool			m_bTutorial;		///< Ʃ�丮�� �ʵ����� ����(���� ǥ��, ä�� �̵� �Ұ�)


	float			fGravity;
	wstring			strSkyModelName;
//	MAP_FOG_INFO	Fog;

	vector< int>	m_vFloors;

	wstring			strBGMSoundName[BGM_MAX];

	int				m_nSoundReverbType;


	MARKER_INFO_MAP_BY_ID	m_mapMarkerByID;
	SENSOR_INFO_MAP_BY_ID	m_mapSensorByID;
	map<wstring, vector<int>>		m_mapGroupSensor;

	AREA_INFO_VECTOR			m_Areas;
	vector< FIELDSOUND_INFO>	m_vecSound;
	NPC_UIMAP_INFO_VECTOR		m_NpcUiInfo[NPC_UIMAP_INFO::TYPE_CNT];
	std::vector< std::string >	m_ParsedPreLoadResources;

	XFieldInfo()  
	{ 
		m_fMinX = 0.0f;
		m_fMinY = 0.0f;
		m_fMaxX = 600000.0f;
		m_fMaxY = 600000.0f;
		fGravity = GRAVITY;
		m_bInstance = false;
		m_bWeatherEffect = true;
		m_nSoundReverbType = 0;
		m_bTutorial = false;
	}
	virtual ~XFieldInfo()
	{
		// TODO : �ε��Ǿ��ִ� �ʵ� ��ũ��Ʈ ����.
		if (global.script) 
			global.script->GetGlueFieldEvent().SetNULL(this);

		m_vFloors.clear();
	}

	void Cook()
	{
		Cook_PvPAreas();
		Cook_Sensors();
	}

	XSensorInfo& GetSensorInfo(unsigned long nID);

	MPoint		GetSectorIndex(float fPosX, float fPosY);

	vec3 GetNPCPos( int nNPCID, bool bFilter=true );
	wstring GetName();
};


/// �ʵ� ���� ������
class XFieldInfoMgr
{
private:
	CSZoneList		m_ZoneList;
	CSFieldList		m_FieldList;
	std::map< int, XFieldInfo*> m_mapSimpleFieldInfo;

	std::map< std::wstring, rs3::RResource* > m_PreLoadResources;

	void ParseFieldInfo(MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo);
	void ParseNpcFieldInfo(MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo){}
	void ParseMarker(MXmlElement* pElement, MXml* pXml, XFieldInfo* pAreaInfo);
	void ParseSensor(MXmlElement* pElement, MXml* pXml, XFieldInfo* pAreaInfo);
	void ParseEnv(MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo);
	void ParsePvparea( MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo );
	void ParseArea( MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo );
	void ParseBGM(MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo);
	void ParseFloors(MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo);
	void ParseSoundReverbType(MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo);
	void ParsePreLoad(MXmlElement* pElement, MXml* pXml, XFieldInfo* pFieldInfo);

	XFieldInfoMgr(XFieldInfoMgr& other);

	bool LoadField(const wchar_t* szFileName, XFieldInfo* pFieldInfo);

	XFieldInfo* GetFieldInfo(int nFieldID);

public:
	XFieldInfoMgr();
	virtual ~XFieldInfoMgr();
	void Clear();
	bool Load(const wchar_t* szZoneList, const wchar_t* szFieldList);

	XFieldInfo* NewFieldInfo(int nFieldID);
	XFieldInfo* NewFieldInfo(const wchar_t* szFieldName);
	XFieldInfo* NewFieldInfoFromZone(const wchar_t* szZoneName);

	bool LoadFieldSound( XFieldInfo* pFieldInfo);

	XFieldInfo* GetSimpleFieldInfo( int nFieldID);		///< �������� �ε��մϴ�. ���ÿ� �������ּ���.
	wstring		GetZoneNameFromFieldID( int nFieldID);

	CSFieldList& GetFieldList()	{ return m_FieldList; }

	const wchar_t*		GetFieldName( int nFieldID );

	void LoadFieldPreLoadResources(XFieldInfo* pFieldInfo);
	bool IsFieldPreLoadResource(const wstring& _resName);
	void ClearFieldPreLoadResources();
};

#endif