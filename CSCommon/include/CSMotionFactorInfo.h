#ifndef _CSMOTION_FACTOR_INFO_H
#define _CSMOTION_FACTOR_INFO_H

#include "CSCommonLib.h"
#include <tchar.h>

/// ��� ���� ����
enum MF_STATE
{
	MF_NONE					= -1,
	MF_BEATEN				= 0,
	MF_KNOCKBACK			= 1,
	MF_STUN					= 2,
	MF_DEFLECT				= 3,	
	MF_THROWUP				= 4,
	MF_KNOCKDOWN			= 5,
	MF_UPPERED				= 6,

	// -- fake
	MF_FAKE_BEATEN		= 7,
	MF_FAKE_KNOCKBACK	= 8,

	// ���⼭���ʹ� �켱������ �������� ���� ---

	// -- npc�� ���
	MF_GRAPPLED				= 9,	// ���
	MF_SWALLOWED			= 10,	// �Ա�
	MF_DRAG					= 11,	// ���� ����
	MF_SIZE,
};

#define MF_PRIORITY_SIZE	9	///< �켱 ������ ������ ��� ���� ����
#define MF_VALUABLE_SIZE	7	///< Value ����� �ϴ� ��� ���� ����

static const TCHAR* MF_STATE_NAME[] = 
{
	_T("MF_BEATEN"),
	_T("MF_KNOCKBACK"),
	_T("MF_STUN"),
	_T("MF_DEFLECT"),	
	_T("MF_THROWUP"),
	_T("MF_KNOCKDOWN"),
	_T("MF_UPPERED"),
	_T("MF_FAKE_BEATEN"),
	_T("MF_FAKE_KNOCKBACK"),
	_T("MF_GRAPPLED"),
	_T("MF_SWALLOWED"),
	_T("MF_DRAG")
};	

/// ��� ������ �켱����
static const MF_STATE MotionFactorPriority[MF_PRIORITY_SIZE] =
{
	MF_THROWUP,
	MF_KNOCKDOWN,
	MF_KNOCKBACK,
	MF_STUN,
	MF_UPPERED,
	MF_DEFLECT,
	MF_BEATEN,
	MF_FAKE_KNOCKBACK,
	MF_FAKE_BEATEN
};


/// ��� ���Ͱ��� ����
struct CSCOMMON_API MOTION_FACTOR_VAR
{
	short nValue;
	short nWeight;

	MOTION_FACTOR_VAR()
	{
		nValue = 0;
		nWeight = 0;
	}

	MOTION_FACTOR_VAR& MOTION_FACTOR_VAR::operator+=( const MOTION_FACTOR_VAR& rhs );
};

struct CSCOMMON_API MOTION_FACTOR_GROUP
{
	MOTION_FACTOR_VAR	var[MF_PRIORITY_SIZE];
	
	MOTION_FACTOR_GROUP& MOTION_FACTOR_GROUP::operator+=( const MOTION_FACTOR_GROUP& rhs );

	MOTION_FACTOR_VAR* Get(int nType)		
	{ 
		if (nType <= MF_NONE || nType >= MF_PRIORITY_SIZE)		return NULL;
		return &var[nType];
	}

	void Adjust(int nType, int nValue, int nWeight)		
	{ 
		if (nType <= MF_NONE || nType >= MF_PRIORITY_SIZE)		return;
		var[nType].nValue += nValue;
		var[nType].nWeight += nWeight;
	}

	bool IsModified() const
	{
		for (int i=0; i<MF_PRIORITY_SIZE; i++)
			if (var[i].nValue > 0)	return true;
		return false;
	}

	void Clear()
	{
		for (int i=0; i<MF_PRIORITY_SIZE; i++)
		{
			var[i].nValue = 0;
			var[i].nWeight = 0;
		}
	}
};

#define KNOCKBACK_TIME		(0.1f)		// �˹� 1���ʹ� �ɸ��� �ð� (����:��)
#define DRAG_TIME			(0.1f)		// �˹� 1���ʹ� �ɸ��� �ð� (����:��)



#endif