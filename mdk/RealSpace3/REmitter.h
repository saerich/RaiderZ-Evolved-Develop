#pragma once

#include "RSceneNodeID.h"
#include "RParticle.h"
#include "REmitterType.h"
#include "RParticle_Create.h"
#include "RVType.h"
#include "RPType.h"
#include <vector>
#include <string>
#include "REffectBase.h"
#include "RTimer.h"

#include "RCardinal.h"

#include "MinMax.h"
#include "RPCollection.h"
#include "REffectManager.h"


namespace rs3 {

struct RParticle;
class REffect;
class RParticleShader;

/**
Base Emitter : �⺻ �߻���, �������� �͸� ...
*/

struct RParticleHandle
{
	RParticleHandle( UINT nHandle ) : m_Handle(nHandle), m_bAlive( false ), m_fLifeTime( 0.f ) {}
	RParticleHandle( UINT nHandle, float _fLifeTime ) : m_Handle(nHandle), m_bAlive( false ), m_fLifeTime( _fLifeTime ) {}

	UINT	m_Handle;
	bool	m_bAlive;
	float	m_fLifeTime;

	bool operator < ( const RParticleHandle& rhs ) const
	{
		return ( REffectManager::GetParticle(m_Handle)->m_fZ < REffectManager::GetParticle(rhs.m_Handle)->m_fZ );
	}
};

struct REmitterDescription
{
	EMITTER_TYPE eEmitterType;
	BILLBOARD_TYPE eBillboardType;
	EMITTER_UVMOD_TYPE eUVModifierType;

	// emitter parameter
	bool bLooping;
	bool bSorting;
	bool bRotate;
	bool bFixAmount;
	bool bColorRand;
	bool bColorRandRange;
	bool bVelRand;
	bool bExplosion;
	bool bGather;
	bool bFollowParentPosition;
	bool bFollowParentRotation;
	bool bWorldVeolocity;
	float fEmitterLife;
	CMinMax<float> fRoatateRad;

	// particle parameter
	CMinMax<float> fParticleLife;
	float fParticleScale;

	RVector4 vTexUV;
	RVector2 vTexUVPivot;

	std::vector<REmitterVariable> vecEmitterVar;
	std::vector<RParticleVariable> vecParticleVar;

	void reset();
};

class RS_API REmitter : public REffectBase
{
public:
	MDeclareRTTI;
	DECLARE_NODE_ID_STRING(RSID_EMITTER);

public:
	//////////////////////////////////////////////////////////////////////////
	// ���� ��ƼŬ�� ��� ���������� �����ϱ� ���� �˰��� �������̽�.
	//
	class REmitterRenderer
	{
	public:
		virtual int		FillParticle(REmitter *pEmitter, list<RParticleHandle> *pArray, RMatrix *pWorldViewMatrix) = 0;
		static RVector*	GetRandomVector(RVector *pOut);
	};

	//////////////////////////////////////////////////////////////////////////
	// ��ƼŬ UV ������
	//
	struct REmitterUVAniDesc
	{
		int		nCountX;
		int		nCountY;
		int		nMaxFrame;
		float	fFrameInterval;
		bool	bLoop;
	};

	class REmitterUVAniModifier
	{
	public:
		REmitterUVAniModifier( REmitter* pEmitter );

		RVector4	GetVertexScale( const RVector2 *pTexPivot );

		PVERTEXFORMAT* FillVertex(PVERTEXFORMAT *pVertex, const RVector* pPos, RParticle* pParticle);

		bool		SaveToXML( MXmlElement &element );
		bool		CreateFromXML( MXmlElement &element );

		void		SetDesc( const REmitterUVAniDesc& desc );
		void		GetDesc( REmitterUVAniDesc& desc );

	private:
		void		Update( RParticle* pParticle );

		REmitter*	m_pEmitter;

		int			m_nCountX;
		int			m_nCountY;
		int			m_nMaxFrame;
		float		m_fFrameInterval;
		bool		m_bLoop;

		float		m_ScaleX;
		float		m_ScaleY;

		float		m_fBlend;

		RVector4	m_UV;
		RVector4	m_UV2;
	};

	//////////////////////////////////////////////////////////////////////////
	// Emitter Play State
	//
	enum PLAY_STATE
	{
		NOT_PLAY = 0,
		PLAY_NORMAL,
		PLAY_PAUSE,
		PLAY_REST
	};


	void	TBBPostUpdate();
	void	SetCurrentUpdate( bool _bUpdate )	{ m_bCurrentUpdate = _bUpdate; }
	bool	GetCurrentUpdate()					{ return m_bCurrentUpdate; }
	void	TBBPreCreateParticle();
	void	TBBDeleteParticle();
	void	SetCardinal( float _fUpdateTime );
	void	SetCardinal();

	

private:
	bool	CheckUseSoftParticle();
	void	TBBUpdate(float fElapseTime);
	void	TBBParticleInit( DWORD _dwNum );
	void	TBBUpdateParticles(float fElapsedTime);
	DWORD	TBBAllocateHandle( int _nNum );

	bool	StartTimeCheck( float fElapseTime );
	bool	IsPlayDisable();
	void	CalcUpdateTime( float& _fUpdateTime, float& _fElapseTime );
	void	SetParticleAmount( int& _nNum, float& _fUpdateTime );
	RVector3 CalcRange();
	void	InitParticle( RParticle* _pParticle, float _fLifeTime, float fPositionInterpolationFactor );

	void	UpdateAABB();

	RVector3 GetParticleInitDirection(bool bFollowParentPosition);
	RVector3 GetParticleInitPosition(bool bFollowParentPosition, bool bFollowParentRotation);
	RVector3 GetInterpolatedWorldPosition(float fInterpolationFactor);

	// control �� �ϱ� ���� ��
	float				m_fElapsedAfterStart;
	float				m_fStartTime;
	bool				m_bOncePlayed;
	bool				m_bCurrentUpdate;
	DWORD				m_dwCreateNum;

protected:
	virtual void		OnPostUpdate();
	virtual void		OnRender( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod) override;

	DWORD	Update(float fElapseTime);

	DWORD	Render(  RCameraSceneNode* pCurrentCamera, RMatrix *pWorldViewMatrix);
	DWORD	RenderPrimitiveMStage(  RCameraSceneNode* pCurrentCamera);

	int		CreateParticle(int nNum);

	RParticle*	GetNewParticle();	// ��ƼŬ Ǯ���� ��ƼŬ �ν��Ͻ��� ���´�.

	RTimer	m_kTimer;

	DWORD	m_dwParticleNum;					// ����׿�
	float	m_fAmountUnder;						// ��������
	float	m_fMaxParticleScale;				// AABB ����ϱ� ����

	bool	m_bCreateParticle;

	REmitterDescription	m_sEmitterDesc;

	// �÷��� ����
	PLAY_STATE m_eOldPlayState;
	PLAY_STATE m_eCurrentPlayState;

	// �߷� ���� ( Localtransform ���� = withme �� ���� ������ ��ȯ�Ǿ����� �ϹǷ� ������ ���� )
	RVector m_vGravityVector;

	RCardinal		m_CrdL;						// Position �� �����̴� ��� Ŭ���� ( 3�� �̻��� ��� Cardinal Spline���� ������ ���ø� ���̺��� ������. )


	// Emitter Track�� �����ϱ� ���� ��
	float			m_fTime;		// ������ �ð�
	float			m_fNext;		// 0 ~ 1 ���� ��
	float			m_fStepTime;	// ���������� �ð���.
	float			m_fStepLife;	// ������ ���� ��
	int				m_iIndex;		// what da??

	// �̹��� ������
	REmitterRenderer	*m_pRenderer;
	// �̹��� ����
	RParticleCreator		*m_pParticleCreator;
	// �̹��� UV ������
	REmitterUVAniModifier *m_pUVModifier;

	const REmitterVariable *m_pVarCurrent;	// ���� Emitter Track ��
	const REmitterVariable *m_pVarNext;		// ���� Emitter Track ��
	
	int m_iAmount;				// ������ �߻���
	float m_fAngle;				// ������ �߻� ����
	RVector m_vRange;			// ������ �߻� ����
	CMinMax<float> m_fSpeed;	// ������ �߻� �ӷ�

	///////////////////////////////////////////////////////////////////////////
	// ��ƼŬ ����
	list<RParticleHandle> m_listParticles; // ������ ��ƼŬ ���
	void UpdateParticles( float fElapsedTime );
	void ParticleSort( RMatrix* pViewMatrix );

	// Update Variable
	void UpdateEmitterVariable();
	void Reset();
	
	bool	IsEmitterLifeValid();
	void	GenerateMovingTable();
	void	CalcMaxParticleScale( const REmitterDescription * pDesc );
	DWORD	GetVisibilityAppliedColor( const RVector4& vColor, float fVisibility );

public:
	bool	m_bPath;		// emitter track�� ���� �����̴� ��θ� ������ �� ���ΰ� ? ( ������ ��� )

	bool	IsOncePlayDone(){ return m_bOncePlayed; }
	void	SetCreateParticle( bool bCreateParticle ) { m_bCreateParticle = bCreateParticle; }

public:
	REmitter();
	virtual ~REmitter();

	virtual bool	IsLoadingCompleted();

	virtual bool	CreateFromXML(MXmlElement &element, bool bBackgroundCreation);

	virtual RSceneNode* Clone();

	virtual void	SaveToXML(MXmlElement &element);

	const REmitterDescription* GetDesc() const;

	void SortingEmitterVar();
	void SortingParticleVar();

	void	Render(  RCameraSceneNode* pCurrentCamera);

	virtual void Start() override;
	virtual void Stop() override;
	virtual void Pause() override;
	virtual bool IsAlive() override;

	int		GetCurrentCount();
	float	GetTime()							{ return m_fTime;}
	PLAY_STATE  GetCurrentPlayState()			{ return m_eCurrentPlayState; }

	DWORD	GetRenderParticleNum()				{ return m_dwParticleNum;}			

	void	DeleteAllParticles();	// ��ƮŬ Ǯ�� ���ҽ��� �����ش�.

	bool	EmitterUpdate(float fUpdateTime);
	bool	PreEmitterUpdate(float fUpdateTime);

	int		GetEmittertimetrackNum(float time);
	int		GetParticletimetrackNum(float time);

	void	SetStartTime(float fStartTime)	{ m_fStartTime = fStartTime; }
	float	GetStartTime()					{ return m_fStartTime; }

	const RVector GetGravityVector() const	{ return m_vGravityVector; }

	// get/set ~er
	void	SetCurrentLife(float fn)	{	m_fTime = fn; }
	float	GetCurrentLife()			{	return m_fTime; }

	void	SetBillboardType(BILLBOARD_TYPE eType);
	BILLBOARD_TYPE GetBillboardType();

	void	SetEmitterType(EMITTER_TYPE eType);
	EMITTER_TYPE GetEmitterType();
	
	void	SetUVModifierDefault();
	void	SetUVModifierAnimation( const REmitterUVAniDesc* pDesc = NULL );
	bool	GetUVModifierAnimationDesc( REmitterUVAniDesc& desc );
	EMITTER_UVMOD_TYPE GetUVModifierType();

	void	SetEmitLife(float fn);
	float	GetEmitLife();

	void	SetLooping(bool loop);
	bool	IsLooping();

	void	SetSorting(bool b);
	bool	IsSorting();

	void	SetRotate(bool b);
	bool	IsRotate();

	void	SetRotationRadian(const CMinMax<float> &data);
	CMinMax<float>	GetRotationRadian() const;

	void	SetPScale(float fn);
	float	GetPScale();

	void	SetParticleLife(const CMinMax<float> data);
	CMinMax<float> GetParticleLife(void) const;

	void	SetRandomColor(bool b);
	bool	IsRandomColor();

	void	SetRandomColorRange(bool b);
	bool	IsRandomColorRange();

	void	SetRandomVel(bool b);
	bool	IsRandomVel();

	void	SetGather(bool b);
	bool	IsGather();

	void	SetExplosion(bool b);
	bool	IsExplosion();

	void	SetWithMe(bool b);
	bool	IsWithMe();

	void	SetFollowParentRotation(bool b);
	bool	IsFollowParentRotation();

	void	SetFixAmount(bool b);
	bool	IsFixAmount();

	bool	IsUVAniBlending();

	void SetTexUV(const RVector4 &v );
	const RVector4* GetTexUV();

	void SetTextUVPivot(const RVector2 &v);
	const RVector2* GetTexUVPivot();

	void SetWorldVelocity(bool b) { m_sEmitterDesc.bWorldVeolocity = b; }
	bool IsWorldVelocity()	{ return m_sEmitterDesc.bWorldVeolocity; }


	RVector4 GetVertexScale();

	PVERTEXFORMAT* GetVertexArray(int particleNum, int& outVertexCount);
	PVERTEXFORMAT* FillVertex( PVERTEXFORMAT* pVertex, const RVector* pPos, RParticle* pParticle );
	PVERTEXFORMAT* FillParticleVertexApplyVisibilityResult( PVERTEXFORMAT* pVertex, const RVector* pPos,
		const RVector4* pTexUV, const RVector4* pTexUV2, const RVector4& vColor, float fBlend );

	int		GetVertexCountPerParticle();

	std::vector<REmitterVariable>&		GetEmitterTrack();
	std::vector<RParticleVariable>&		GetParticleTrack();

	// REmitter���� ������Ʈ �Ǵ� ���� Get
	const RVector* GetRange()	{	return &m_vRange; }

	bool		UpdateResource();
	void		Init();
	void		UpdateEmitterVar();
	void		UpdateParticleVar();

	// tool �� ���� ���
	bool		CreateNew();
	void		InitAllParticle();		// ���� �ִ� ��ƼŬ�� ��� ����. ������ ȣ���ϱ� ���� �뵵�� �������. ���ӷ�Ÿ�ӽ��� ȣ���� �����ȵ�. ���ǿ��  - 090603, OZ
};

inline int REmitter::GetCurrentCount()
{
	return m_listParticles.size();
}

inline const REmitterDescription *REmitter::GetDesc() const
{
	return &m_sEmitterDesc;
}

}
