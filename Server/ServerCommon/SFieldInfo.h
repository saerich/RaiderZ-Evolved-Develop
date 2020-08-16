#pragma once

#include "ServerCommonLib.h"
#include <string>
#include "CSFieldInfo.h"

/// �ʵ� ����
class SCOMMON_API SFieldInfo : public CSFieldInfo
{
public:
	bool					m_bDynamic;		// ����/�ν��Ͻ� ����
	int						m_nDefaultChannelCount;
	int						m_nCapacity;	///< �� ä�δ� �ִ�� �� �� �ִ� �ο���
	bool					m_bValidation;	///< Validator���� üũ�� �� ����
	bool					m_bAILODEnable;	///< AI LOD�� ���� ����


	SFieldInfo() : m_bDynamic(false), m_nDefaultChannelCount(DEFAULT_CHANNEL_COUNT), m_nCapacity(CHANNEL_PLAYER_LIMIT), m_bValidation(true), m_bAILODEnable(true) {}
	virtual ~SFieldInfo() {}
	void Assign(CSFieldFileInfo* pFileInfo);
	std::wstring MakeFieldFileName(const wchar_t* szFieldPath);
};