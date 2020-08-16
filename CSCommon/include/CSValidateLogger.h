#ifndef _CS_VALIDATE_LOGGER_H_
#define _CS_VALIDATE_LOGGER_H_

#include "CSCommonLib.h"

#include <deque>
#include <string>
#include "MTstring.h"
using namespace std;

#define PERCENT_S				_T("%s")
#define PERCENT_D				_T("%d")
#define IS						_T("��(��) ")
#define NOT_IN					_T("�� �����ϴ�.\n")
#define EMPTY					_T(" ��� �ֽ��ϴ�.\n")
#define ZERO_OR_MINUS			_T(" 0�̰ų� ���� �Դϴ�.\n")
#define MINUS					_T(" ���� �Դϴ�.\n")
#define INVALID_TYPE			_T(" �߸��� ���� �Դϴ�.\n")
#define ALREADY_EXIST			_T(" �̹� �ֽ��ϴ�.\n")
#define TOO_BIG					_T(" �ʹ� Ů�ϴ�.\n")
#define TOO_SMALL				_T(" �ʹ� �۽��ϴ�..\n")
#define NOT_TRUE				_T(" true�� �ƴմϴ�.\n")
#define NOT_FALSE				_T(" false�� �ƴմϴ�.\n")
#define FILED_OR_MARKER_OF		_T("�� �ʵ峪 ��Ŀ�� ")
#define NPCID_OF				_T("�� NPCID�� ")
#define OVER_MAX_STACK_AMOUNT	_T("������ �ִ� ���� ������ �ѽ��ϴ�.")
#define TRADABLEITEM			_T("�ŷ� ������ �������Դϴ�.\n")

#define IS_NOT_IN_FIELDGROUPLIST	IS FILENAME_FIELDGROUPLIST NOT_IN
#define IS_NOT_IN_FIELDLIST			IS PATH_SYSTEM PURE_FILENAME_FIELDLIST NOT_IN
#define IS_NOT_IN_FIELEXT			IS FILEEXT_FIELD NOT_IN
#define IS_NOT_IN_CONDITIONS		IS FILENAME_CONDITIONS NOT_IN
#define IS_NOT_IN_DIALOG			IS FILENAME_DIALOG NOT_IN
#define IS_NOT_IN_NPC				IS FILENAME_NPCINFO NOT_IN
#define IS_NOT_IN_ITEM				IS FILENAME_ITEMINFO NOT_IN
#define IS_NOT_IN_TALENT			IS FILENAME_TALENTINFO NOT_IN
#define IS_NOT_IN_LOOT				IS FILENAME_LOOT_INFO NOT_IN
#define IS_NOT_IN_TRADE				IS FILENAME_NPCSHOP NOT_IN
#define IS_NOT_IN_TRAINING			IS FILENAME_TRAINING NOT_IN
#define IS_NOT_IN_QUEST				IS FILENAME_QUEST NOT_IN
#define IS_NOT_IN_INN				IS FILENAME_INN NOT_IN
#define IS_NOT_IN_SOULBINDING		IS FILENAME_SOULBINDING NOT_IN
#define IS_NOT_IN_BUFFID			IS FILENAME_BUFFINFO NOT_IN
#define IS_NOT_IN_FACTION			IS FILENAME_FACTION NOT_IN
#define IS_NOT_IN_RECIPE			IS FILENAME_RECIPE NOT_IN

#define IS_NOT_IN_PERCENT_D_FILEEXT_FIELD		IS PERCENT_D FILEEXT_FIELD NOT_IN

#define IS_ZERO_OR_MINUS			IS ZERO_OR_MINUS
#define IS_MINUS					IS MINUS
#define IS_INVALID_TYPE				IS INVALID_TYPE
#define IS_ALREADY_EXIST			IS ALREADY_EXIST
#define IS_TOO_BIG					IS TOO_BIG
#define IS_TOO_SMALL				IS TOO_SMALL
#define IS_EMPTY					IS EMPTY
#define IS_NOT_TRUE					IS NOT_TRUE
#define IS_NOT_FALSE				IS NOT_FALSE
#define IS_OVER_MAX_STACK_AMOUNT	IS OVER_MAX_STACK_AMOUNT


#define BIGGER_THAN						_T("%s��(��) %s���� �� Ů�ϴ�.\n")
#define MUCH_THAN						_T("%s��(��) %s���� �� �����ϴ�.\n")
#define BIGGER_OR_EQUAL_THAN			_T("%s��(��) %s���� �� ũ�ų� �����ϴ�.\n")
#define TEXT_EXCEEDED_ALLOWED_LENGTH	_T("%s�� ���̰� %d(bytes)�� �ʰ��Ͽ����ϴ�.\n")
#define THE_VALUE_MUST_BE_IN_MINMAX		_T("%s�� %.3f��(��) %.3f(min)�� %.3f(max) ���̰��̾�� �մϴ�.\n")
#define THE_VALUE_MUST_BE_IN_MINMAX_D	_T("%s�� %d��(��) %d(min)�� %d(max) ���̰��̾�� �մϴ�.\n")


class CSLogListener;

class CSCOMMON_API CSValidateLogger
{
public:
	struct ELEMENT
	{
		ELEMENT(tstring strTag)
		: m_strTag(strTag)
		{			
		}

		struct ATTRIBUTE
		{
			ATTRIBUTE(tstring strName, tstring strValue)
			: m_strName(strName)
			, m_strValue(strValue)
			{
			};

			ATTRIBUTE(tstring strName, int nValue)
			: m_strName(strName)
			{
				TCHAR szNumber[512];
				_itot_s(nValue, szNumber, 512, 10);				
				m_strValue = szNumber;
			};

			tstring m_strName;
			tstring m_strValue;
		};

		void Insert(const ATTRIBUTE& attribute)
		{
			m_qAttribute.push_back(attribute);
		}

		tstring m_strTag;
		deque<ATTRIBUTE> m_qAttribute;
	};
private:
	CSLogListener*	m_pListener;
	bool m_bValidateResult;
	bool m_bLogging;
protected:
	deque<ELEMENT> m_qElement;
	void PushElement(const ELEMENT& element);
	void PopElement();
	tstring m_strFileName;

	void SetValidateResultFailed() { m_bValidateResult = false; }
public:
	CSValidateLogger();
	virtual ~CSValidateLogger();

	void Init(const TCHAR* szFileName);
	void SetListener(CSLogListener* pListener) { m_pListener = pListener; }
	void Log(const TCHAR* szAddtionLog, ...);
	bool GetValidateResult() { return m_bValidateResult; }
	void SetLogging(bool bLogging) { m_bLogging = bLogging; }

	void ValidateMinMax(const TCHAR* szValueName, float fValue, float fMinValue, float fMaxValue);
	void ValidateMinMax( const TCHAR* szValueName, int nValue, int nMinValue, int nMaxValue );
	void ValidateMinus(int nValue, const TCHAR* szName);
	void ValidateMinus(float fValue, const TCHAR* szName);
	void ValidateMinusORZero(int nValue, const TCHAR* szName);
	void ValidateMinusORZero(float fValue, const TCHAR* szName);
	void ValidateBiggerThan(int nSmallValue, int nBigValue, const TCHAR* szSmallName, const TCHAR* szBigName);
	void ValidateBiggerThan(float fSmallValue, float fBigValue, const TCHAR* szSmallName, const TCHAR* szBigName);
	void ValidateBiggerOrEqualThan(float fSmallValue, float fBigValue, const TCHAR* szSmallName, const TCHAR* szBigName);
	void ValidateMuchThan(int nSmallValue, int nBigValue, const TCHAR* szSmallName, const TCHAR* szBigName);	
	
	void ValidateEmptyText(const tstring& strText, const TCHAR* szName);
	void ValidateLengthText(const tstring& strText, int nMaxLength, const TCHAR* szName);	

	void ValidateSex(int nValue, const TCHAR* szName);
	void ValidateRace(int nValue, const TCHAR* szName);

	void ValidateTrue(bool bBool, const TCHAR* szName);
	void ValidateFalse(bool bBool, const TCHAR* szName);

	void ValidateTooBig(int nValue, int nMaxValue, const TCHAR* szValueName);
	void ValidateID(int nID);
};


#endif//_CS_VALIDATE_LOGGER_H_