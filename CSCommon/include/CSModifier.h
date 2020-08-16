#pragma once

#include "CSCommonLib.h"
#include "CSMotionFactorInfo.h"
#include "CSTalentInfoEnum.h"
#include "MTypes.h"
#include "CSDef.h"

/// nMod�� fPer�� �ִ� Modifier. fPer�� 100%���� �����ȴ�.
template<typename T>
struct ModVar
{
	T	  nMod; // ��� ����ġ
	float fPer; // ����� ���� ����ġ

	ModVar()	
	{ 
		Clear();
	}
	ModVar(T nMod, float fPer)
	: nMod(nMod), fPer(fPer)
	{ 
		Clear();
	}
	bool IsModified() const
	{
		if (nMod != (T)(0))	return true;
		if (fPer != 0.0f)	return true;
		return false;
	}
	T Calc(T nOriginal) const
	{
		return (T)((nOriginal + nMod) * (1.0f+fPer));
	}
	T CalcDiff(T nOriginal) const
	{
		return (T)(((nOriginal + nMod) * (1.0f+fPer)) - nOriginal);
	}
	void Modify(T& noutVar) const
	{
		noutVar = Calc(noutVar);
	}
	void Clear()
	{
		nMod = (T)(0);
		fPer = 0.0f;
	}
	ModVar&	operator*=( const int& rhs )
	{
		fPer += rhs;
		return *this;
	}
	ModVar&	operator+=( const ModVar& rhs )
	{
		nMod += rhs.nMod;
		fPer += rhs.fPer;
		return *this;
	}
	ModVar&	operator-=( const ModVar& rhs )
	{
		nMod -= rhs.nMod;
		fPer -= rhs.fPer;
		return *this;
	}
	bool operator==( const ModVar& rhs ) const
	{
		if (nMod == rhs.nMod && fPer == rhs.fPer) return true;
		return false;
	}
};

template<typename T>
struct PerModVar
{
	float fPer; // ����� ���� ����ġ

	PerModVar()
	{
		Clear();
	}
	bool IsModified() const
	{
		if (fPer != 0.0f)	return true;
		return false;
	}
	T	Calc(T nOriginal) const
	{
		return (T)(nOriginal * (1.0f+fPer));
	}
	void Modify(T& noutVar) const
	{
		noutVar = Calc(noutVar);
	}
	void Clear()
	{
		fPer = 0.0f;
	}
	PerModVar&	operator+=( const PerModVar& rhs )
	{
		fPer			+= rhs.fPer;
		return *this;
	}
	PerModVar&	operator-=( const PerModVar& rhs )
	{
		fPer			-= rhs.fPer;
		return *this;
	}
	bool		operator==( const PerModVar& rhs ) const
	{
		if (fPer == rhs.fPer) return true;
		return false;
	}
};

template<typename T>
struct PerMulVar
{
	float fPer; // ����� ����ġ. �������� ����

	PerMulVar()
	{
		Clear();
	}
	bool	IsModified() const
	{
		if (fPer != 1.0f)	return true;
		return false;
	}
	T		Calc(T nOriginal) const
	{
		return (T)(nOriginal * fPer);
	}
	void	Modify(T& noutVar) const
	{
		noutVar = Calc(noutVar);
	}
	void	Clear()
	{
		fPer = 1.0f;
	}
	PerMulVar&	operator+=( const PerMulVar& rhs )
	{
		// �������� ����
		return *this;
	}
	PerMulVar&	operator*=( const PerMulVar& rhs )
	{
		fPer			*= rhs.fPer;
		return *this;
	}
	PerMulVar&	operator-=( const PerMulVar& rhs )
	{
		// �������� ����
		return *this;
	}
	bool		operator==( const PerMulVar& rhs ) const
	{
		if (fPer == rhs.fPer) return true;
		return false;
	}
};

class CSCOMMON_API CSInstantModifier
{
public:
	ModVar<int>		nHP;
	ModVar<int>		nEN;
	ModVar<int>		nSTA;	

	CSInstantModifier();

	CSInstantModifier& operator+=( const CSInstantModifier& rhs );
	CSInstantModifier& operator-=( const CSInstantModifier& rhs );

	bool IsModified() const;
	void Clear();
};

class CSCOMMON_API CSActorModifier
{
public:
	ModVar<int>				nHPMax;
	ModVar<int>				nENMax;
	ModVar<int>				nSTAMax;

	float					fMoveSpeed;					// (���)
	ModVar<int>				nHPRegen,					// (����)
							nENRegen,					// (����)
							nSTARegen;					// (����)
	

	// ���� ����
	ModVar<int>				nAP,						///< ����
							nABS;						///< ���� ���

	ModVar<float>			fGuardRate;					///< �����

	float					fMeleeDodgeAmp;
	float					fRangeDodgeAmp;
	float					fMagicDodgeAmp;

	float					fMeleeHitRateAmp;
	float					fRangeHitRateAmp;
	float					fMagicHitRateAmp;

	float					fCriticalAmp;
	float					fCriticalMeleeAmp;
	float					fCriticalRangeAmp;	
	float					fCriticalMagicAmp;

	float					fCriticalMeleeAmpForMe;

	float					fCriticalMeleeDamageAmp;	///< �߰� ġ�� ������� ( 1 = 100% )
	float					fCriticalRangeDamageAmp;	///< �߰� ġ�� ������� ( 1 = 100% )
	float					fCriticalMagicDamageAmp;	///< �߰� ġ�� ������� ( 1 = 100% )

	float					fMeleeDamageAmp;			///< �߰� ���� ������� (1 = 100%)
	float					fRangeDamageAmp;			///< �߰� ���Ÿ� ������� (1 = 100%)
	float					fMagicDamageAmp;			///< �߰� ���� ������� (1 = 100%)


	float					fDefenceAmp[DA_MAX];		///< �߰� �����(1 = 100%)

	ModVar<int>				nResistances[TRT_MAX];		///< ����

	ModVar<short>			MotionFactorValue[MF_PRIORITY_SIZE];
	ModVar<short>			MotionFactorWeight[MF_PRIORITY_SIZE];

	PerModVar<float>		fGatherTimeAmp[MAX_GATHER_TYPE];				///< ä�� �ð� ����ġ


	float					fModMeleePene;				///< ���� ���� ������ ���ʽ�
	float					fModRangePene;				///< ���Ÿ� ���� ������ ���ʽ�
	float					fModMagicPene;				///< ���� ���� ������ ���ʽ�

public:
	CSActorModifier();

	CSActorModifier& operator+=( const CSActorModifier& rhs );
	CSActorModifier& operator-=( const CSActorModifier& rhs );

	bool IsMotionfactorModified() const;
	bool IsModified() const;
	void Clear();
};

class CSCOMMON_API CSPlayerModifier
{
public:
	// �⺻ ����
	ModVar<int>				nSTR,						// (���)-
							nDEX,						// (���)-
							nINT,						// (���)-
							nCHA,						// (���)-
							nCON;						// (���)-							

public:
	CSPlayerModifier();

	CSPlayerModifier& operator+=( const CSPlayerModifier& rhs );
	CSPlayerModifier& operator-=( const CSPlayerModifier& rhs );

	bool IsModified() const;
	void Clear();
	void CutInstantMod();								///< ���ȿ���� �ִ� �Ӽ��� �����
};
