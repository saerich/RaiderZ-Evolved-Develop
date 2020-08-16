#pragma once

#include "CSCommonLib.h"
#include "CSDef.h"
#include "CSWorldInfo.h"
#include "MXml.h"
#include <map>
#include "MTString.h"
using namespace std;

struct CSZoneFileInfo
{
	int		nZoneID;		// �� ID
	tstring	strName;
	tstring	strFile;
	CSZoneFileInfo()
	{
		nZoneID = -1;
	}
};

struct CSFieldFileInfo
{
	int		nFieldID;
	tstring	strName;
	tstring	strFile;
	bool	bDynamic;
	bool	bAILODEnable;				///< AI LOD�� ������� ����
	int		nDefaultChannelCount;		///< ���� �ε��ÿ� ������ ä�� ����
	int		nCapacity;					///< �� ������ ���� �÷��̾� ��
	bool	bValidation;				///< Validator���� üũ�� �� ����
	bool	bShowMap;					///< Ŭ��. �ʸ���Ʈ�� �������� ����
	CSFieldFileInfo()
	{
		nFieldID = -1;
		bDynamic = false;
		nDefaultChannelCount = DEFAULT_CHANNEL_COUNT;
		nCapacity = CHANNEL_PLAYER_LIMIT;
		bValidation = true;
		bAILODEnable = true;
		bShowMap = false;
	}
};

enum FIELD_TYPE
{
	FIELDTYPE_NONE = 0,
	FIELDTYPE_HUNTING_GROUND,
	FIELDTYPE_TOWN,

	FILEDTYPE_MAX
};


/// �� ����Ʈ (zonelist.xml�� �ִ� ����)
class CSCOMMON_API CSZoneList : public map<int, CSZoneFileInfo* > 
{
private:
	void ParseZone(MXmlElement* pElement, MXml* pXml);
	bool CheckOverlapZoneID(int nZoneID);
public:
	CSZoneList();
	virtual ~CSZoneList();
	void Clear();
	bool Load(const TCHAR* szFileName);
	CSZoneFileInfo* Find(int nID);
};

/// �ʵ� ����Ʈ (fieldlist.xml�� �ִ� ����)
class CSCOMMON_API CSFieldList : public map<int, CSFieldFileInfo* > 
{
private:
	void ParseFieldList( MXmlElement* pElement, MXml* pXml );
	void ParseField(MXmlElement* pElement, MXml* pXml);
	void ParseField(MXmlElement* pElement, MXml* pXml, CSFieldFileInfo*& pNewInfo);
	void ParseFieldGroup(MXmlElement* pElement, MXml* pXml);
public:
	CSFieldList();
	virtual ~CSFieldList();
	void Clear();
	bool Load(const TCHAR* szFileName, bool bIncludeDevFieldList = false);

	void Insert(CSFieldFileInfo* pFieldFileInfo);
	CSFieldFileInfo* Find(int nID);
	bool IsExist(int nID);
};

struct PVP_AREA_INFO
{
	MRect	rcArea;
	uint8	nFaction1;
	uint8	nFaction2;
	tstring	strFactionName1;
	tstring	strFactionName2;

	PVP_AREA_INFO(): nFaction1(0), nFaction2(0) {}
};


/// �ʵ��� �ð� ����
struct CSFieldTimeInfo
{
	bool				bFixed;				///< �ð� ���� ����
	GAME_TIME_TYPE		nFixedTime;			///< ������ �ð�

	CSFieldTimeInfo() : bFixed(false), nFixedTime(TIME_INVALID) {}
	void Clear() { bFixed = false; nFixedTime = TIME_INVALID; }
};


/// Ŭ���̾�Ʈ�� ���� �ʵ� ������ �θ� Ŭ����
class CSCOMMON_API CSFieldInfo
{
public:
	int						m_nFieldID;			///< �ʵ� ID
	int						m_nZoneID;			///< �� ID

	tstring					m_strFieldName;		///< �ʵ� �̸�
	tstring					m_strFieldFile;		///< �ʵ� ���� �̸�

	bool					m_bBoundCheck;		///< ��輱 �˻�
	MRectF					m_rtBoundary;		///< ��輱 ��ǥ

	vector<PVP_AREA_INFO>	m_PvPAreas;
	CSFieldTimeInfo			m_TimeInfo;			///< �ð� ����

	bool					m_bPotralTalentUsable;	///< �ʵ� �̵� �ŷ�Ʈ ��� ���� ����

	CSFieldInfo() : m_nFieldID(0), m_nZoneID(0), m_bBoundCheck(false), m_bPotralTalentUsable(true) {}
	virtual ~CSFieldInfo() {}
};