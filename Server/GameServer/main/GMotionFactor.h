#ifndef _GMOTION_FACTOR_H
#define _GMOTION_FACTOR_H

#include "MTime.h"
#include "CSMotionFactorInfo.h"
#include "GDef.h"
#include "CSModifier.h"

class GField;
class GEntityActor;


/// ��� ����
class GMotionFactor
{
protected:
	MRegulator			m_DecreaseRegulator;
	MOTION_FACTOR_GROUP m_Factors;												///< ��� ���Ͱ�
	MF_STATE			m_nCurrentMF;
	int					m_nCurrentWeight;
	bool				m_bActive;

protected:
	bool				CheckActivateMF(MF_STATE& outState, int& outWeight, bool bIsSticky=false);	///< Ȱ��ȭ�� ��� ���Ͱ��� �ִ��� Ȯ��
	void				IncreaseVars( MOTION_FACTOR_GROUP &MotionFactorGroup, PerMulVar<short>* pValueModifiers, PerMulVar<short>* pWeightModifiers, int nNotApplyMF, vector<int>* pvecAdditionIgnoreMF );
	void				DecreaseVars();											///< ��� ���Ͱ� ����
	bool				CheckOverridable(MF_STATE nCurrMF, MF_STATE nNewMF);
	bool				CheckFakeMF(MF_STATE& outState, int& outWeight, PerMulVar<short>* pValueModifiers, bool bIsSticky=false);		///< Fake ������Ͱ� �ִ��� Ȯ��
public:
	GMotionFactor();
	virtual ~GMotionFactor(void);
	void Update(float fDelta);
	void ClearVars();												///< ��� ���Ͱ� �ʱ�ȭ
	bool Apply(MOTION_FACTOR_GROUP& MotionFactorGroup, PerMulVar<short>* pValueModifiers, PerMulVar<short>* pWeightModifiers, int nNotApplyMF=MF_NONE, vector<int>* pvecAdditionIgnoreMF=NULL, bool bIsSticky=false );

	void OnReleased();
	void ReleaseFakes();
	void Force(MF_STATE nState, int nWeight);
	MF_STATE	GetCurrMF() const	{ return m_nCurrentMF; }
	int			GetCurrWeight() const	{ return m_nCurrentWeight; }
	wstring GetDebugString();		///< ����� ���� ��ȯ
	MOTION_FACTOR_GROUP& GetFactorsForTest()	{ return m_Factors; }

	void SetActive(bool bActive)	{ m_bActive = bActive; }
	bool IsActive()					{ return m_bActive; }
};




#endif // _GMOTION_FACTOR_H