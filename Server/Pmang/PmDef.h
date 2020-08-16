#pragma once

#include "STransData.h"
#include "IPMSObject.h"
#include "PMSCodes.h"
#include "MTstring.h"
#include <tchar.h>
#include <atlconv.h>
#include <atlexcept.h>


enum PmRegionCode
{
	PRC_GYEONGGI = 0,		// ���
	PRC_SEOUL,				// ����
	PRC_INCHEON,			// ��õ
	PRC_GANGWON,			// ����
	PRC_CHUNGNAM,			// �泲
	PRC_CHUNGBUK,			// ���
	PRC_DAEJEON,			// ����
	PRC_GYEONGBUK,			// ���
	PRC_GYEONGNAM,			// �泲
	PRC_DAEGU,				// �뱸
	PRC_BUSAN,				// �λ�
	PRC_ULSAN,				// ���
	PRC_JEONBUK,			// ����
	PRC_JEONNAM,			// ����
	PRC_GWANGJU,			// ����
	PRC_JEJU,				// ����
	PRC_FOREIGN,			// �ؿ�
	
	MAX_PRC,
};

enum PmAgeCode
{
	PAC_0TO13 = 0,	// 0�� ~ 13��
	PAC_14TO16,		// 14�� ~ 16��
	PAC_17TO19,		// 17�� ~ 19��
	PAC_20TO24,		// 20�� ~ 25��
	PAC_25TO29,		// 26�� ~ 29��
	PAC_30TO39,		// 30�� ~ 39��
	PAC_40TO99,		// 40�� ~ 99��

	MAX_PAC
};

enum PmGenderCode
{
	PGC_MALE = 0,	// ����
	PGC_FEMALE,		// ����

	MAX_PGC
};

struct PmStatIndex
{
	PmRegionCode	nRegionCode;
	PmAgeCode		nAgeCode;
	PmGenderCode	nGenderCode;
	int				nYearOfBirth;

	PmStatIndex& operator=(const PmStatIndex& r)
	{
		nRegionCode = r.nRegionCode;
		nAgeCode = r.nAgeCode;
		nGenderCode = r.nGenderCode;
		nYearOfBirth = r.nYearOfBirth;

		return *this;
	}

	bool IsValid() const
	{
		if (nRegionCode < PRC_GYEONGGI	|| MAX_PRC <= nRegionCode)	return false;
		if (nAgeCode	< PAC_0TO13		|| MAX_PAC <= nAgeCode)		return false;
		if (nGenderCode < PGC_MALE		|| MAX_PGC <= nGenderCode)	return false;

		if (nYearOfBirth < 0)	return false;

		return true;
	}
};

struct PmUserData
{
	wstring strUserSerial;
	wstring strUserID;	
	PmStatIndex	statIndex;

	wstring strPCBID;
	wstring strServiceType;
	int nUserType;


	// shallow copy�ص� ��������.
	PmUserData& operator=(const PmUserData& r)
	{
		strUserSerial = r.strUserSerial;
		strUserID = r.strUserID;
		statIndex = r.statIndex;

		strPCBID = r.strPCBID;
		strServiceType = r.strServiceType;
		nUserType = r.nUserType;

		return *this;
	}

	bool IsValid() const
	{
		if (strUserSerial.empty())	return false;
		if (strUserID.empty())		return false;

		if (!statIndex.IsValid())	return false;
		if (nUserType < 0)		return false;

		return true;
	}

	void Export(TD_PMANG_USER_DATA* pTd) const
	{
		wcsncpy_s(pTd->szUserSerial, strUserSerial.c_str(), _TRUNCATE);		
		wcsncpy_s(pTd->szUserID, strUserID.c_str(), _TRUNCATE);

		pTd->nRegionCode = (int8)statIndex.nRegionCode;
		pTd->nAgeCode = (int8)statIndex.nAgeCode;
		pTd->nGenderCode = (int8)statIndex.nGenderCode;
		pTd->nYearOfBirth = (int16)statIndex.nYearOfBirth;

		wcsncpy_s(pTd->szPCBID, strPCBID.c_str(), _TRUNCATE);
		wcsncpy_s(pTd->szServiceType, strServiceType.c_str(), _TRUNCATE);
		pTd->nUserType = (int16)nUserType;
	}	

	void Import(const TD_PMANG_USER_DATA* pTd)
	{
		strUserSerial = pTd->szUserSerial;
		strUserID = pTd->szUserID;

		statIndex.nRegionCode = (PmRegionCode)pTd->nRegionCode;
		statIndex.nAgeCode = (PmAgeCode)pTd->nAgeCode;
		statIndex.nGenderCode = (PmGenderCode)pTd->nGenderCode;
		statIndex.nYearOfBirth = pTd->nYearOfBirth;

		strPCBID = pTd->szPCBID;
		strServiceType = pTd->szServiceType;
		nUserType = pTd->nUserType;
	}
};
