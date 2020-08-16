#pragma once

namespace rs3 {

	struct REmitterDescription;
	struct RParticleVariable;

	struct RS_API RParticle
	{
	public:
		float	m_fLife;		// Life
		float	m_fStepLife;	// ������ ���� ��
		float	m_fStepTime;	// ���������� �ð���.
		float	m_fTime;		// ������ �ð�
		float	m_fNext;		// �������� ���� �ð��� 0 ~ 1 ���� ��
		RVector2* m_pInitScales; // �ʱ� ���� Scale

		int		m_iIndex;		// track index
								
		// ( explosion/gather ���� ��� )
		RVector	m_vStartPos;	//������ġ ���, ���� ȿ���� ������ġ���� ��ȭ�� ����.
		RVector m_vRange;		//�����ɶ� �̹�����ǥ�� ���� ���� ��ġ
		RVector m_vCreatePosition;	//�����ɶ� �θ��� ��ġ

		// update �� �� ���
		RVector m_vCurPos;
		RVector m_vCurVelocity;
		RVector m_vPrevPos;

		float	m_fRadian;
		float	m_fRadiansum;

		// linear interpolation �Ǵ� �� (���� �����Ǵ� �ӵ� ��)
		float	m_fCurSpeed;
		float	m_fNextSpeed;

		// sorting �� �� ���
		float	m_fZ;

		// UV������ �ִϸ��̼ǿ� ���
		int		m_nPrevFrame;
		int		m_nCurrentFrame;

		RVector4 m_vColor;

		RMatrix m_WorldRotationTransform;

	public:
		// ��ƼŬ Ǯ�� malloc�� ����ϱ� ������ ������, �Ҹ��ڰ� �Ҹ��� �ʴ´�.
	//	RParticle(void);
	//	~RParticle(void);

		void	Init(const REmitterDescription* pDesc, const RVector& vCreatePosition, const RVector& vRange, const RVector& vDir, float fSpeed, float fLifeTime, float fRadian, RMatrix matWorldTransform);
		void	Init(const REmitterDescription* pDesc);
		void	Destroy();
		void	Clear();

		bool	Update(const REmitterDescription* pDesc, float fElapseTime, const RVector& vGravity);
		void	UpdateParticle(const REmitterDescription* pDesc, float fUpdateTime, const RVector& vGravity);
		
		RVector2 GetParticleScale(const REmitterDescription* pDesc);

		void	GetResultPos( const RMatrix& matTransform, bool bFollowParentPosition, bool bFollowParentRotation, RVector& outPos );

		const RVector&	GetPos()	{	return m_vCurPos; }
		const RVector&	GetVel()	{	return m_vCurVelocity; }

	protected:
		void	UpdateColor( const RVector4& vColorFirst, const RVector4& vColorSecond, float fWeight );
		void	UpdateAlpha( const RVector4& vColorFirst, const RVector4& vColorSecond, float fWeight );
		void	UpdateRotation(bool bRotate, float fUpdateTime);
		void	UpdateSpeed(float fCurMin, float fCurMax, float fNextMin, float fNextMax);
		void	UpdatePosition(const REmitterDescription* pDesc, const RParticleVariable &rVarA, const RParticleVariable &rVarB, float fInterpolationFactor, const RVector& vGravity, float fUpdateTime);

		float	GetInterpolatedValue( float fFirst, float fSecond, float fWeight );

		RVector3 GetVelocity(RVector3 vVelocityA, RVector3 vVelocityB, float fInterpolationFactor, bool bWorldVeolocity);
		RVector3 GetRandomVelocity(float fInterpolationFactor);
		RVector3 GetGForce(float fGRateA, float fGRateB, float fInterpolationFactor, RVector3 vGravity);
		RVector4 GetLastKeyColor(const std::vector<RParticleVariable>& vecParticleVar);

		RVector4 InitColor(RVector4 vFirstKeyColor, RVector4 vLastKeyColor, bool bColorRand, bool bColorRandRange);
		RVector2* InitScale(const std::vector<RParticleVariable>& vecParticleVar);
	};
}	//namespace rs3