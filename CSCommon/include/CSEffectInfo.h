#pragma once

#include "CSCommonLib.h"

class CSCOMMON_API CSEffectInfo
{
public:
	enum POINT
	{
		POINT_NONE=0,

		POINT_SELF,				// �ڱ⸦ �߽�����
		POINT_TARGET,			// Ÿ���� �߽�����
		POINT_CASTER,			// ȿ���� �����Ų ��� �߽�
		POINT_HITCAPSULE,		// �������� �߽�����
		POINT_AREAPOINT,		// Ư�� ��ǥ�� �߽�����

		POINT_MAX
	};

	enum RELATION
	{
		RELATION_NONE=0,

		RELATION_ALL,			// ���
		RELATION_ENEMY,			// ����������
		RELATION_ALLIED,		// ��ȣ������
		RELATION_PARTY,			// ��Ƽ����
		RELATION_ALLIED_DEAD,	// ��ȣ���̸鼭 �׾� �ִ���
		RELATION_SELF,			// �ڱ��ڽ�

		RELATION_MAX
	};

public:
	POINT		m_nPoint;		///< ������ �߽�����
	float		m_fRadius;		///< ���� ������ŭ
	RELATION	m_nRelation;	///< �ڱ�� � ���迡 �ִ� �����
	int			m_nLimit;		///< ��� ��ŭ���� ȿ���� �ش�.

public:
	CSEffectInfo();
	CSEffectInfo(POINT nPoint, float fRadius, RELATION nRelation, int nLimit=INT_MAX);
};
