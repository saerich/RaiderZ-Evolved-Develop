#ifndef _CS_STRINGS_H
#define _CS_STRINGS_H

#include <string>
#include "MTstring.h"
#include <vector>
using namespace std;

#include "MTypes.h"
#include "CSCommonLib.h"
#include "CSChrInfo.h"


// �������� ����ϴ� ��Ʈ�� ����
class CSCOMMON_API CSStrings
{
protected:
	static vector<tstring>	m_vecStrGameTime;				///< �ð�
	static vector<tstring>	m_vecStrGameWeather;			///< ����
	static vector<tstring>	m_vecStrGameWeatherSimple;		///< ����
	static vector<tstring>	m_vecStrMotionFactor;			///< �������
	static vector<tstring>	m_vecStrDamageAttrib;			///< ������Ÿ��
	static vector<tstring>	m_vecStrItemTier;				///< ������ ���
	static vector<tstring>	m_vecStrItemType;				///< ������ Ÿ��
	static vector<tstring>	m_vecStrWeaponType;				///< ���� Ÿ��
	static vector<tstring>	m_vecStrArmorType;				///< �� Ÿ��
	static vector<tstring>	m_vecStrUsableType;				///< ��뼺 ������ Ÿ��
	static vector<tstring>	m_vecStrTalentStyle;			///< �ŷ�Ʈ ��Ÿ��
	static vector<tstring>	m_vecStrSoulType;				///< ��ȥ ����			
	static vector<tstring>	m_vecStrRace;					///< ���� 
	static vector<tstring>	m_vecStrSex;					///< ����
	static vector<tstring>	m_vecStrEffectPoint;			///< ȿ�� ��ġ
	static vector<tstring>	m_vecStrEffectRelation;		///< ȿ�� ������� ����
	static vector<tstring>	m_vecStrWeaponReference;		///< �ŷ�Ʈ ����Ҷ� �����Ǵ� ����
	static vector<tstring>	m_vecStrQuestObjectiveType;		///< ����Ʈ ���� Ÿ��
	static vector<tstring>	m_vecStrQuestShareType;			///< ����Ʈ ���� Ÿ��
	static vector<tstring>	m_vecStrGateType;				///< Ʈ���̾� �ʵ� ����Ʈ Ÿ��
	static vector<tstring>	m_vecStrWarpType;				///< ���� Ÿ��
	static vector<tstring>	m_vecStrQuestTrialType;			///< ����Ʈ Ʈ���̾� Ÿ��
	static vector<tstring>	m_vecStrAAType;
	static vector<tstring>	m_vecStrQuestRewardType;
	static vector<tstring>	m_vecStrFactionRelationType;	///< �Ѽ� ���� Ÿ��
	static vector<tstring>	m_vecStrQOPublicProgress;		///< ����Ʈ �������� ������
	static vector<tstring>	m_vecStrIElementType;			///< ���ͷ��� Ÿ��

	
	
	
	static vector<tstring>	m_vecStrChatSoulType;

	static vector<tstring>	m_vecStrConditionType;
	static vector<tstring>	m_vecStrConditionQuestState;	///< ����Ʈ ����

	static vector<tstring>	m_vecStrGatherType;				///< ä�� Ÿ��
	
	static vector<tstring>	m_vecStrResistType;				///< ���� Ÿ��
	static vector<tstring>	m_vecStrResistMethod;			///< ���� �޼ҵ�
	static vector<tstring>	m_vecStrAttackableType;

protected:
	static int			StringToEnum(const TCHAR* szValue, const vector<tstring>& vecStrEnum);	
	static const TCHAR*	EnumToString(const int nEnum, const vector<tstring>& vecStrEnum);

public:
	static bool			Init();

	static bool			IsString$$(const TCHAR* szValue);

	static tstring		StringToLower(const tstring& strValue);

	static bool			StringToBool(const TCHAR* szValue);
	static void			StringToFieldIDAndMarerID(const TCHAR* szValue, int& outnFieldID, DWORD& outdwMakrerID);
	static void			StringToFieldIDAndVec2(const TCHAR* szValue, int& outnFieldID, vec3& vecPos);
	static void			StringToFieldIDAndNPCID(const TCHAR* szValue, int& outnFieldID, int& outnNPCID);
	static void			StringsToStringVector(const TCHAR* szValue, vector<tstring>& vecOut);
	static void			StringNumbersToIntVector(const tstring& strValue, vector<int>& vecOut);
	static void			IntVectorToStringNumbers(const vector<int>& vecInt, tstring& outstrValue);	

	static const TCHAR*	WeaponType(int nWeaponType);
	static const TCHAR*	GameTime(int nGameTime);
	static const TCHAR*	Weather(int nWeather);
	static const TCHAR*	WeatherSimple(int nWeather);
	static const TCHAR*	MotionFactor(int  nMF);
	static const TCHAR*	Stance(int nStance);
	static const TCHAR*	Sex(SEX nSex);

	static int			StringToDamageAttrib(const TCHAR* szValue);
	static void			StringToDamageAttrib(const TCHAR* szValue, tstring& outParamValue);
	static int			StringToMotionfactorType(const TCHAR* szValue);
	static void			StringToMotionfactorType(const TCHAR* szValue, tstring& outParamValue);	

	static int			StringToItemType(const TCHAR* szValue);
	static int			StringToItemTier(const TCHAR* szValue);
	static int			StringToWeaponType(const TCHAR* szValue);
	static int			StringToArmorType(const TCHAR* szValue);
	static int			StringToUsableType(const TCHAR* szValue);
	static int			StringToTalentStyle(const TCHAR* szValue);
	static int			StringToSoulType(const TCHAR* szValue);		
	static int			StringToRace(const TCHAR* szValue);
	static int			StringToSex(const TCHAR* szValue);	
	static int			StringToEffectPoint(const TCHAR* szValue);	
	static int			StringToEffectRelation(const TCHAR* szValue);
	static int			StringToWeapnReference(const TCHAR* szValue);
	static int			StringToQuestObjectiveType(const TCHAR* szValue);
	static int			StringToQuestShareType(const TCHAR* szValue);
	static int			StringToGateType(const TCHAR* szValue);
	static int			StringToWarpType(const TCHAR* szValue);
	static int			StringToQuestTrialType(const TCHAR* szValue);
	static int			StringToAAType(const TCHAR* szValue);
	static int			StringToQuestRewardType(const TCHAR* szValue);
	static int			StringToFactionRelationType(const TCHAR* szValue);
	static int			StringToQOPublicProgress(const TCHAR* szValue);
	static int			StringToIElementType(const TCHAR* szValue);	

	static const TCHAR*	ItemTypeToString(int itemType);
	static const TCHAR*	ItemTierToString(int nItemTier);
	static const TCHAR*	ArmorTypeToString(int nArmorType);
	static const TCHAR*	SoulTypeToString(int nSoulType);	
	static const TCHAR*	TalentStyleToString(int nValue);
	static const TCHAR*  QuestTiralTypeToString(int nTrialType);
	static const TCHAR*	GateTypeToString(int nGateType);
	static const TCHAR*	QuestRewardTypeToString(int nType);
	static const TCHAR*	FacionRelationTypeToString(int nType);	



	static void			StringsToItemTypeVector(const TCHAR* szValues, vector<int>& outvecItemType);	
	static void			StringsToWeaponAndArmorTypeVector(const TCHAR* szValues, vector<int>& outvecWeaponType, vector<int>& outvecEquipmentType);

	static tstring		XmlEscape(tstring const& value);

	static void			ReplaceChar(tstring& str, TCHAR const c, tstring const& replacement);

	static const TCHAR*	SoulTypeToChatString(int nSoulType);

	// condition
	static int			StringToConditionType(const TCHAR* szValue);
	static int			StringToConditionQuestState(const TCHAR* szValue);	

	// ä��
	static int StringToGatherType(const TCHAR* szValue);
	static const TCHAR* GatherTypeToString(int nGatherType);

	// ����Ÿ��
	static int StringToResistType(const TCHAR* szValue);
	static const TCHAR* ResistTypeToString(int nResistType);

	// ���׸޼ҵ�
	static int StringToResistMethod(const TCHAR* szValue);
	static const TCHAR* ResistMethodToString(int nResistMethod);

	static int			StringToAttackableType(const TCHAR* szValue);

	// ����ܾ� ������
	static wstring		Plural(wstring strSingular);
};


#endif
