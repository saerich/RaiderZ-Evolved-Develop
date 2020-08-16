#pragma once

#include "XModEffector.h"

#define MIN_MOVE_SPEED		0.1f

// class XBuffAttribute
class XBuffAttribute
{
private:
	
public:
	union
	{
		unsigned int	m_nAttribute;				// bit32 : ���� �Ӽ� ������
		struct
		{
			bool	bDisabledTalent			: 1;	// bit0  : Ż��Ʈ ���� ���� �Ӽ�
			bool	bDisabledMelee			: 1;	// bit1  : Melee�� Ż��Ʈ ���� �Ӽ�
			bool	bDisabledRange			: 1;	// bit2  : Range�� Ż��Ʈ ���� �Ӽ�
			bool	bDisabledMagic			: 1;	// bit3  : Magic�� Ż��Ʈ ���� �Ӽ�
			bool	bInvincibled			: 1;	// bit4  : ���� �Ӽ�
			bool	bRooted					: 1;	// bit5  : Root �Ӽ�
			bool	bSleeped				: 1;	// bit6  : Sleep �Ӽ�
			bool	bTotem					: 1;	// bit7  : B-Parts ���
		};
	};

	float m_fMoveSpeedRatio;						// ������ ���� �̵� ��� �ӵ� ����
	bool m_bMounted;

public:
	XBuffAttribute() : m_nAttribute( 0), m_fMoveSpeedRatio( 1.0f), m_bMounted(false) {}

	void SetAttribute( unsigned int nAttribute)			{ m_nAttribute = nAttribute;	}
	unsigned int GetAttribute()							{ return m_nAttribute;			}

	void ParseBuffAttr( const XBuffInfo* pBuffInfo, int nStackCount = 1);


	// �ش� Ż��Ʈ�� ����� �� �ִ��� Ȯ��
	bool IsUsableTalent( XTalentInfo* pInfo);
	bool IsUsableTalent( int nTalentID);

	// Ȱ�� ������ �������� Ȯ��
	bool IsMesmerizable()			{ return ( bRooted || bSleeped);					}

	// Melee ������ ������ �������� Ȯ��
	bool IsAttackableMelee()		{ return ( !IsMesmerizable() && !IsDisabledMelee());	}

	// Range ������ ������ �������� Ȯ��
	bool IsAttackableRange()		{ return ( !IsMesmerizable() && !IsDisabledRange());	}

	// Magic ������ ������ �������� Ȯ��
	bool IsAttackableMagic()		{ return ( !IsMesmerizable() && !IsDisabledMagic());	}

	// �̵� �������� Ȯ��
	bool IsMovable()				{ return !IsMesmerizable();								}

	// ���� ���� ���������
	bool IsTotemable()				{ return bTotem;}

	// ������ ���� �̵� ��� �ӵ� ������ ����
	float GetMoveSpeedRatio()		{ return max( m_fMoveSpeedRatio, MIN_MOVE_SPEED);	}



	// ���� ���� �˻�
	bool IsDisabledTalent()			{ return bDisabledTalent;							}
	bool IsDisabledMelee()			{ return ( bDisabledTalent || bDisabledMelee);		}
	bool IsDisabledRange()			{ return ( bDisabledTalent || bDisabledRange);		}
	bool IsDisabledMagic()			{ return ( bDisabledTalent || bDisabledMagic);		}
	bool IsInvincibled()			{ return bInvincibled;								}
};
