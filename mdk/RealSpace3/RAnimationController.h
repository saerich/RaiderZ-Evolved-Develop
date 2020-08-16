#pragma once

#include <map>

#include "RActorController.h"
#include "RAnimationControllerNode.h"

namespace rs3 {

class RAnimation;
class RActor;
class RActorNode;

enum RPLAYSTATE {
	RPS_STOP = 0,
	RPS_PLAY,
	RPS_PAUSE
};

class RAnimationEvent;

struct ANIMATION_EVENT
{
	RAnimationEvent*	pAnimationEvent;
	bool				bEventDone;			///< �̺�Ʈ�� ����Ǿ����� ����

	ANIMATION_EVENT()
	{
		pAnimationEvent = NULL;
		bEventDone		= false;
	}
};

enum RBLENDMODE
{
	RBLENDMODE_OVERWRITE,
	RBLENDMODE_BLEND,
//	RBLENDMODE_ADDITIVE,		///< �ش� �ִϸ��̼��� �߰� ���� ( additive animation ), ���� ����
};

class RS_API RActorNodeSelection
{
protected:
	typedef set<string>	RACTORNODESET;

	RACTORNODESET		m_nodes;

public:
	RActorNodeSelection( );

	bool IsExistNode(const char* szNodeName);
	bool SelectNode(RActor* pActor, const char* szNodeName);
	bool SelectNodeAndChildren(RActor* pActor, const char* szNodeName, bool bExceptSelectNode = false);

	void clear(){ m_nodes.clear(); }
};

struct RPLAYINFO
{
	RBLENDMODE				blendMode;	///< ���� ���, �������, �������� �����Ѵ�
	float					fWeight;	///< ������ ����� ����ġ 0�� ������� 1�̸� ��ü
	float					fPlaySpeed;	///< ����ӵ� �⺻�� : 1.0
	float					fBlendIn;	///< ���۵ɶ��� ���� �ð�
	float					fBlendOut;	///< �������� ���� �ð�
	RActorNodeSelection*	pNodeSet;	///< ���õ� ��忡�� ������ �ְ� ������, NULL�� ������ ��ü

	RPLAYINFO()
		: blendMode( RBLENDMODE_OVERWRITE )
		, fWeight(1.0f)
		, fPlaySpeed(1.0f)
		, fBlendIn(0.f)
		, fBlendOut(0.f)
		, pNodeSet(NULL)
	{
	}
};

//////////////////////////////////////////////////////////////////////////
// RAnimationFrameController
// �ִϸ��̼��� ������ ����� �ִϸ��̼� event ó���� ����ϴ� ��Ʈ�ѷ�
class RS_API RAnimationFrameController : public RActorController
{
public:

	RAnimationFrameController(RSNCTRL_TYPE eType = RSNCTRL_UPDATE);

	virtual void	Update(RCameraSceneNode* pCamera) override;
	void			SetAnimation( RAnimation *pAnimation );
	void			Reset();

	bool			IsPlayDone() const;
	bool			IsOncePlayDone() const;
	DWORD			GetMaxFrame() const;
	float			GetSpeed() const;
	bool			GetLoop() const;
	RPLAYSTATE		GetState() const;

	DWORD			GetFrame() const;
	void			SetFrame( DWORD nFrame );
	void			SetSpeed( float fSpeed );
	void			SetLoop( bool bLoop );
	void			SetOncePlayDone( bool bOncePlayDone );
	void			SetForceAnimationControlPlayDoneForTest( bool bPlayDone );

	void			Play();
	void			Stop();
	void			Pause();

	vector<ANIMATION_EVENT>& GetAnimationEvent() { return m_AniamtionEvents; }
	void			SetAnimationEvent(vector<ANIMATION_EVENT>& aniamtionEvents);
	void			ResetEvent();
	void			CloseEvent();
	bool			GetEventActive() const;
	void			SetEventActive(bool b);

	void			GetNextFrameAndState(DWORD dwElapsedTime, DWORD& _nOutNextFrame, bool& _bPlayDone) const;

	void			CheckAnimationEventSend();

protected:
	void			UpdateFrame(DWORD dwElapsedTime);

	RPLAYSTATE		m_nState;			///< �÷��� ����
	DWORD			m_nFrame;			///< ���� �ִϸ��̼� ������
	DWORD			m_nMaxFrame;		///< ���� �ִϸ��̼� �ִ� ������
	bool			m_bLoop;			///< loop �������� ����
	bool			m_bPlayDone;		// Loop �϶� �� ���� true �� �ɼ� ����.
	bool			m_bOncePlayDone;	// Loop �϶��� �÷��̰� �ѹ� �Ǹ� true �� �ɼ� �ִ�.
	float			m_fSpeed;			///< ����ӵ�
	bool			m_bEventActive;		///< �̺�Ʈ Ȱ��ȭ ����

	vector<ANIMATION_EVENT> m_AniamtionEvents;
};

//////////////////////////////////////////////////////////////////////////
inline DWORD RAnimationFrameController::GetFrame() const { return m_nFrame; }
inline void RAnimationFrameController::SetFrame(DWORD nFrame) { m_nFrame = min(nFrame,m_nMaxFrame); }
inline float RAnimationFrameController::GetSpeed() const	{ return m_fSpeed; }
inline void RAnimationFrameController::SetSpeed( float fSpeed ) { m_fSpeed = fSpeed; }
inline bool RAnimationFrameController::IsPlayDone()	const	{	return m_bPlayDone;	}
inline bool	RAnimationFrameController::IsOncePlayDone()	const {	return m_bOncePlayDone;	}
inline DWORD RAnimationFrameController::GetMaxFrame() const	{ return m_nMaxFrame; }
inline bool RAnimationFrameController::GetLoop() const		{	return m_bLoop;	}
inline void	RAnimationFrameController::SetLoop( bool bLoop ) { m_bLoop = bLoop; }
inline RPLAYSTATE RAnimationFrameController::GetState() const{ return m_nState;	}
inline void	RAnimationFrameController::SetOncePlayDone( bool bOncePlayDone ) { m_bOncePlayDone = bOncePlayDone; }
inline bool	RAnimationFrameController::GetEventActive() const	{ return m_bEventActive; }
inline void RAnimationFrameController::SetEventActive(bool b)	{ m_bEventActive = b; }


/// AnimationController/ overlay animation controller / blend animation controller �� ��ä�� �� ��Ʈ�ѷ�
class RS_API RAnimationController : public RActorController
{
	friend RActor;

public:

	RAnimationController(RSNCTRL_TYPE eType = RSNCTRL_POSTANIMATION, RSNCTRL_TYPE eFrameControllerType = RSNCTRL_UPDATE);
	virtual ~RAnimationController(void);

	void			Destroy();

	void			Reset();
	RAnimation*		GetAnimation();

	bool			SetAnimation( const char* szAnimation, RPLAYINFO* pPlayInfo = NULL);

	virtual void	Update(RCameraSceneNode* pCamera) override;

	RBLENDMODE		GetBlendMode() const;
	void			SetBlendMode(RBLENDMODE blendMode);
	void			SetWeight( float fWeight );
	float			GetWeight() const;

	void			SetFrame(DWORD nFrame);
	DWORD			GetFrame() const;
	DWORD			GetMaxFrame() const;
	void			SetSpeed(float fSpeed);
	float			GetSpeed() const;

	bool			GetLoop() const;

	RPLAYSTATE		GetState() const;

	bool			IsPlayDone();
	bool			IsOncePlayDone();

	RAnimationControllerNode* GetNodeController(const string& strName);
	RAnimationControllerNode* GetNodeController(int i);
	size_t			GetNodeCount();			///< �ش� actor �� ��ü ��� ��
	size_t			GetActiveNodeCount();	///< ���� ����ϴ� ��� ��

	void			Play();
	void			Stop();
	void			Pause();

	vector<ANIMATION_EVENT>& GetAnimationEvent() { return m_frameController.GetAnimationEvent(); }
	void			ResetEvent();
	void			CloseEvent();
	bool			GetEventActive() const;
	void			SetEventActive(bool b);

	void			GetNextFrameAndState(DWORD dwElapsedTime, DWORD& _nOutNextFrame, bool& _bPlayDone);

	RAnimationFrameController& GetFrameController() { return m_frameController; }

protected:
	void			ConnectAnimation(RAnimation *pAnimation, RActorNodeSelection* pSelection );
	void			Disconnect();

	void			SetActorUseVertexAnimation( RAnimation* pAnimation );

	bool			SetAnimation( RAnimation* pAnimation, RPLAYINFO* pPlayInfo );

	/// ��带 Ȱ��ȭ �Ѵ� �������� ���� ���� ������ ��� ��带 Ȱ��ȭ�Ѵ�
	void			ActivateNodes( RActorNodeSelection* pSelection = NULL);

	void			UpdateAnimation();

	virtual void	OnAdd() override;
	virtual void	OnRemove() override;
	virtual void	ReNew() override;

	vector<RAnimationControllerNode>	m_nodeControllers;		///< ��� ��带 ����ִ�
	vector<RAnimationControllerNode*>	m_activeControllers;	///< Ȱ��ȭ�� ����

	//// playinfo �� ������ ������.,, by pok. playinfo�� ������ �ߴ�.
	//RBLENDMODE				m_blendMode;		///< ���� ���
	//float						m_fWeight;			///< ��ǥ ���̴� �� (0=������ ���� �ȹ�ħ,1=������ ����Ե�)
	//float						m_fBlendIn;			///< ���۵ɶ��� ���� �ð�
	//float						m_fBlendOut;		///< �������� ���� �ð�
	//RActorNodeSelection*		m_pNodeSelection;	///< ���� ���õ� ���
	RActorNodeSelection			m_nodeSelection;
	RPLAYINFO					m_playInfo;

	RAnimation					*m_pAnimation;

	RAnimationFrameController	m_frameController;	///< �ִϸ��̼� ����������� �̺�Ʈ ó��
};

//////////////////////////////////////////////////////////////////////////
// inlines

inline size_t RAnimationController::GetNodeCount() { return m_nodeControllers.size(); }
inline size_t RAnimationController::GetActiveNodeCount() { return m_activeControllers.size(); }
inline RAnimation* RAnimationController::GetAnimation() { return m_pAnimation; }
inline void RAnimationController::SetWeight( float fWeight ) { m_playInfo.fWeight = fWeight; }
inline void	RAnimationController::SetBlendMode(RBLENDMODE blendMode) { m_playInfo.blendMode = blendMode; }

//////////////////////////////////////////////////////////////////////////
// ����.. GetFrameController �ϳ��� ���ְ� �� ���ٱ�...
inline DWORD RAnimationController::GetFrame() const		{	return m_frameController.GetFrame();	}
inline bool RAnimationController::IsPlayDone()			{	return m_frameController.IsPlayDone();	}
inline bool	RAnimationController::IsOncePlayDone()		{	return m_frameController.IsOncePlayDone();	}
inline DWORD RAnimationController::GetMaxFrame() const	{	return m_frameController.GetMaxFrame(); }
inline float RAnimationController::GetSpeed() const		{	return m_frameController.GetSpeed(); }
inline float RAnimationController::GetWeight() const	{	return m_playInfo.fWeight; }
inline RBLENDMODE RAnimationController::GetBlendMode() const		{	return m_playInfo.blendMode; }
inline bool RAnimationController::GetLoop() const		{	return m_frameController.GetLoop();	}
inline RPLAYSTATE RAnimationController::GetState() const{	return m_frameController.GetState();	}
inline void	RAnimationController::ResetEvent()			{ m_frameController.ResetEvent(); }
inline void	RAnimationController::CloseEvent()			{ m_frameController.CloseEvent(); }
inline void	RAnimationController::SetFrame(DWORD nFrame) { m_frameController.SetFrame(nFrame); }
inline void	RAnimationController::SetSpeed(float fSpeed) { m_frameController.SetSpeed(fSpeed); }
	inline void	RAnimationController::Play()				{ m_frameController.Play(); }
inline void	RAnimationController::Stop()				{ m_frameController.Stop(); }
inline void	RAnimationController::Pause()				{ m_frameController.Pause(); }
inline void	RAnimationController::GetNextFrameAndState(DWORD dwElapsedTime, DWORD& _nOutNextFrame, bool& _bPlayDone) { m_frameController.GetNextFrameAndState(dwElapsedTime, _nOutNextFrame, _bPlayDone); }
inline bool	RAnimationController::GetEventActive() const	{ return m_frameController.GetEventActive(); }
inline void RAnimationController::SetEventActive(bool b)	{ m_frameController.SetEventActive(b); }

}