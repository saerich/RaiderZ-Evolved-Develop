#ifndef _RSCREENEFFECTMANAGER_H
#define _RSCREENEFFECTMANAGER_H

#include "RListener.h"
//#include "RActor.h"
#include "REffectActor.h"
#include "RAnimation.h"

namespace rs3 {

// ��ũ�� ����Ʈ ���� �Ӽ�
enum RSCREENEFFECT_EXIST_ATT
{
	EXIST_ONE,								// ���� �ϳ�
	EXIST_MULTI								// ������
};

// ���� ���
enum RSCREENEFFECT_LOCATION_TYPE
{
	LOCATION_LEFT,							// ���� ����
	LOCATION_RIGHT,							// ������ ����
	LOCATION_CENTER							// �߾� ����
};

// ���� �Ӽ�
enum RSCREENEFFECT_CHARACTER_ATT
{
	CHARACTER_NORMAL,						// ���� �Ϲ�(�̹��� �ϳ�)
	CHARACTER_WORD							// ���� ����
};

// �ִϸ��̼� ���� �� ó�� �Ӽ�
enum RSCREENEFFECT_ANIMATION_END_ATT
{
	ANIMATION_END_SHOW = 0,					// �ִϸ��̼� ������ �����ֱ�
	ANIMATION_END_HIDE,						// �ִϸ��̼� ������ �����
	ANIMATION_END_DEL,						// �ִϸ��̼� ������ ����
};

class RScreenEffect;
class RScreenEffectUnit;

typedef map<string, RScreenEffect*>			CompositeScreenEfeectList;
typedef CompositeScreenEfeectList::iterator	CompositeScreenEfeectList_Iter;
typedef vector<RScreenEffect*>				SingleScreenEfeectList;
typedef SingleScreenEfeectList::iterator	SingleScreenEfeectList_Iter;

typedef vector<RScreenEffectUnit*>			ScreenEfeectUnitList;
typedef ScreenEfeectUnitList::iterator		ScreenEfeectUnitList_Iter;

//////////////////////////////////////////////////////////////////////////
// EffectActor �� ������ ��ü������ ������ �Ͽ� �����н��� �ٸ���
class RScreenEffectActor : public REffectActor
{
public:
	RScreenEffectActor();

	void Update()
	{
		RActor::UpdateAnimation();
	}
};

//////////////////////////////////////////////////////////////////////////
// ��ũ�� ����Ʈ ����
class RScreenEffectUnit : public RListener
{
private:
	RMesh *							m_pMesh;
	RAnimation *					m_pAnimation;

	RScreenEffectActor				m_RActor;
	RSceneNodeUpdateVisitor			m_ActorUpdator;
	RVector							m_Offset;

	bool							m_bVisible;

public:
	RScreenEffectUnit();
	~RScreenEffectUnit();

	bool							Render();
	void							Update(DWORD dwElapsed);
	bool							IsAnimationEnd();
	void							Create(RMesh* pMesh, 
											RAnimation* pAnimation, 
											vec3 offset = vec3(0,0,0));
	void							Destroy();

	RMesh *							GetMesh() {return m_pMesh; }
	RActor *						GetActor() { return &m_RActor; }
	bool							GetbVisible() { return m_bVisible; }

	void							SetOffset(vec3& offset) { m_Offset = offset; }
	void							SetbVisible(bool bVisible);
	void							SetAnimation(char* pOrder);
	void							SetAnimation(RAnimation* pAnimation);
	void							SetAnimation(char * pOrder, AnimationLoopType nAniLoop);
	void							SetPosition(vec3 vPos);
	void							AnimationStop();
	void							SetVisibility(float f);
	void							SetLastFrame();
};

//////////////////////////////////////////////////////////////////////////
// ��ũ�� ����Ʈ ���̽�
class RScreenEffect
{
protected:
	RSCREENEFFECT_LOCATION_TYPE		m_ScreenEffectLocation;
	RSCREENEFFECT_EXIST_ATT			m_ScreenEffectExist;
	RSCREENEFFECT_CHARACTER_ATT		m_ScreenEffectChar;
	RSCREENEFFECT_ANIMATION_END_ATT	m_ScreenEffectAniEndAtt;
	AnimationLoopType				m_ScreenEffectLoop;

	string							m_strScreenEffectName;
	string							m_strBaseFileName;

	vec3							m_vPosEffect;

	bool							m_bAnimationEnd;
	RSceneManager *					m_pSceneManager;

public:
	RScreenEffect(RSceneManager* pSceneManager);
	virtual ~RScreenEffect();

	virtual void					Create(string strScreenEffectName, 
											string strBaseFileName, 
											RSCREENEFFECT_EXIST_ATT existAtt,
											RSCREENEFFECT_CHARACTER_ATT charAtt,
											AnimationLoopType aniLoop);
	virtual void					Destroy();
	
	virtual bool					Render( RCameraSceneNode* pCurrenCamera);	// dubble. �������� �����丵����
	virtual bool					Update(DWORD dwElapsed);

	// ���ڿ� �Է�
	virtual void					SetString(string strContents);
	// ��ġ ����
	virtual void					SetPosition(vec3 vPosEffect, RSCREENEFFECT_LOCATION_TYPE scrBase);
	// �ִϸ��̼� ����
	virtual void					ChangeAnimation(string strAnimation, RSCREENEFFECT_ANIMATION_END_ATT aniDel);
	// �׸��� ����
	virtual void					SetDraw(bool bVisible);

	virtual void					AnimationStop()	{}

	virtual bool					IsAnimationEnd() { return false; }

	// ��ũ�� ����Ʈ ���� ����
	bool							SetScreenEffectUnit(RScreenEffectUnit* pUnit, string strFileName, vec3 vPosEffect);

	virtual void					SetVisibility(float f) {}
	virtual void					SetLastFrame() {}
};

//////////////////////////////////////////////////////////////////////////
// �̱ۿ� ��ũ�� ����Ʈ
class RSingleScreenEffect : public RScreenEffect
{
private:
	RScreenEffectUnit *				m_pUnit;

	void							SetScreenEffect();

public:
	RSingleScreenEffect(RSceneManager* pSceneManager);
	virtual ~RSingleScreenEffect();

	virtual void					Create(string strScreenEffectName, 
											string strBaseFileName, 
											RSCREENEFFECT_EXIST_ATT existAtt,
											RSCREENEFFECT_CHARACTER_ATT charAtt,
											AnimationLoopType aniLoop);
	virtual void					Destroy();

	virtual bool					Render( RCameraSceneNode* pCurrenCamera);	// dubble. �������� �����丵����
	virtual bool					Update(DWORD dwElapsedTime);
	
	// ��ġ ����
	virtual void					SetPosition(vec3 vPosEffect, RSCREENEFFECT_LOCATION_TYPE scrBase);
	// �ִϸ��̼� ����
	virtual void					ChangeAnimation(string strAnimation, RSCREENEFFECT_ANIMATION_END_ATT aniDel);
	// �׸��� ����
	virtual void					SetDraw(bool bVisible);
	virtual	void					AnimationStop();
	virtual bool					IsAnimationEnd();

	virtual void					SetVisibility(float f);
	virtual void					SetLastFrame();
};

//////////////////////////////////////////////////////////////////////////
// �ܾ� ���տ� ��ũ�� ����Ʈ
class RCompositeScreenEffect : public RScreenEffect
{
private:
	string							m_strContents;

	ScreenEfeectUnitList			m_vecUnit;

	void							SetScreenEffect();
	void							ReSetScreenEffect();
	vec3							SetWordPosition(const vec3& vBasePosEffect, int nCount, int nTotalCount, RSCREENEFFECT_LOCATION_TYPE scrBase);

public:
	RCompositeScreenEffect(RSceneManager* pSceneManager);
	virtual ~RCompositeScreenEffect();

	virtual void					Create(string strScreenEffectName, 
											string strBaseFileName, 
											RSCREENEFFECT_EXIST_ATT existAtt,
											RSCREENEFFECT_CHARACTER_ATT charAtt,
											AnimationLoopType aniLoop);
	virtual void					Destroy();

	virtual bool					Render( RCameraSceneNode* pCurrentCamera);	// dubble. �������� �����丵����
	virtual bool					Update(DWORD dwElapsedTime);

	// ���ڿ� �Է�
	virtual void					SetString(string strContents);
	// ��ġ ����
	virtual void					SetPosition(vec3 vPosEffect, RSCREENEFFECT_LOCATION_TYPE scrBase);
	// �ִϸ��̼� ����
	virtual void					ChangeAnimation(string strAnimation, RSCREENEFFECT_ANIMATION_END_ATT aniDel);
	// �׸��� ����
	virtual void					SetDraw(bool bVisible);
	virtual void					AnimationStop();
};

//////////////////////////////////////////////////////////////////////////
// ��ũ�� ����Ʈ �Ŵ���
class RS_API RScreenEffectManager
{
private:
	RSceneManager *					m_pSceneManager;
	RScreenEffect*					m_pLastInputSingleScreenEffect;	
	CompositeScreenEfeectList		m_mapCompositeScreenEffect;
	SingleScreenEfeectList			m_vecSingleScreenEffect;

public:
	RScreenEffectManager();
	~RScreenEffectManager();

	void							AddScreenEffect(string strListFileName, 
													RSCREENEFFECT_CHARACTER_ATT charAtt = CHARACTER_NORMAL, 
													AnimationLoopType aniLoop = RAniLoopType_HoldLastFrame);
	void							RegisterScreenEffect(string strScreenEffectName, 
														string strListFileName,
														RSCREENEFFECT_CHARACTER_ATT charAtt = CHARACTER_NORMAL,
														AnimationLoopType aniLoop = RAniLoopType_HoldLastFrame);
	void							Destroy();

	// ���ڿ� �Է�
	void							SetString(string strScreenEffectName, string strContents);
	// ��ġ ����
	void							SetPosition(string strScreenEffectName, vec3 vPosEffect, RSCREENEFFECT_LOCATION_TYPE scrBase);
	// SceneManager �ν��Ͻ� ����
	void							SetSceneManager(RSceneManager * pSceneManager) { m_pSceneManager = pSceneManager; }
	// �׸��� ����
	void							SetDraw(string strScreenEffectName, bool bVisible);

	void							SetVisibility(string strScreenEffectName, float f);
	void							SetLastFrame(string strScreenEffectName);

	// �ִϸ��̼� ����
	void							ChangeAnimation(string strScreenEffectName, string strAnimation, RSCREENEFFECT_ANIMATION_END_ATT aniDel);	

	void							Update();
	void							Render();

	// ��ũ�� ����Ʈ �ִ°�?
	bool							IsScreenEffect();
	void							AnimationStop(string strScreenEffectName);
	bool							IsAnimationEnd(string strScreenEffectName);
};

}

#endif