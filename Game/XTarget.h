#ifndef _XTARGET_H
#define _XTARGET_H

/************************************************************************/
/* Ÿ�� ������ ����Ʈ ���ñ��� �Ѵ�.                                    */
/************************************************************************/
#include "XModuleActorControl.h"
#include "XModuleCollision.h"
#include "XMouseBase.h"

class XObject;
class XNonPlayer;

#define TARGET_EFFECT_FILE_NAME		L"lock"
#define TARGET_CHECK_OBJECT_DIST	1.0f
#define TARGET_CHECK_LINE_DIST		35.0f
#define ICON_EFFECT_MARKER			L"ef_head_icon_marker"

enum TARGET_TYPE
{
	XTT_BASE,
	XTT_LINE,
	XTT_STICKY,
	XTT_GROUND,
	XTT_COLLISIONBOX,
};

//------------------------------------------------------------------------
struct stTargetID
{
	MUID		uidTarget;			// Ÿ�� ������Ʈ UID
	vec3		vTargetPos;			// Ÿ���� ���� �߽� ��ǥ(��, Ÿ���� ��� �� �ٽ� ��ǥ�̴�.)
	int			nTargetGroupID;		// Ÿ�� �׷� ID
	int			nTargetCapsuleID;	// Ÿ�� ĸ�� ID
	MUID		uidEffect;			// Ÿ��ȿ�� uid

	bool		bHitParts;			// �κ� ����
	unsigned long	time;

	stTargetID()
	{
		Init();
	};

	void Init()
	{
		uidTarget			= MUID::ZERO;
		vTargetPos			= vec3::ZERO;
		nTargetGroupID		= -1;
		nTargetCapsuleID	= -1;
		uidEffect			= MUID::ZERO;
		bHitParts			= false;
		time				= 0;
	}

	stTargetID(MUID uidTarget, MUID e) : uidTarget(uidTarget), uidEffect(e) {}
};

//------------------------------------------------------------------------
class XTargetRenderer
{
private:
public:
	XTargetRenderer()	{}
	MUID SetIndicatorEffect(XObject* pObject);
	MUID SetTargetBoneEffect(XObject* pObject, RActorNode * pPartsTargetNode, vec3& pos);
	MUID SetTargetHitPartsEffect(XObject* pObject, vec3& centerPos, vec3& pos);
	void DelTargetEffect(MUID uidEffect);
};

// [1/23/2008 isnara] XTarget �����丵
class XTarget : public XMouseBase
{
protected:
	CSEffectInfo::RELATION	m_nCurrentTargetAllowed;

	XTargetRenderer				m_TargetRenderer;

	float						m_fTargetRange;
	int							m_nOneTargetEn;

	bool						m_bTargetUI;

protected:
	// �������� �˻�
	bool						CheckMe(XObject* pObject);
	bool						CheckNPC(XObject* pObject);											// NPC�� ���ͳ� ����
	virtual bool				CheckTargetAllowed(XObject* pObject);
	bool						CheckExistTargetObjOnScreen(XObject* pObject);
	bool						CheckColTerrain(vec3& vStart, vec3& vTarget, float& fOutDistance, vec3* vOutTargetPos = NULL);
	bool						CheckTotalEN(int nTargetCount);
	bool						CheckEqualFaction(XPlayer* pObject);
	bool						CheckTargetHitCapsule(XObject* pObject);

	// ��Ÿ
	virtual void				GetCameraInfo(vec3& vPos, vec3& vDir);

public:
	XTarget(XObject* pObject);
	virtual ~XTarget();

	virtual void				InitTarget(bool bTargetUI);
	virtual void				ExitTarget();
	virtual void				UpdateTarget()									{}
	virtual void				DelTargetAll()									{}

	void						StartTargetUI();
	void						EndTargetUI();

	virtual void				SetTargetInfo(float fRange, int nEn, CSEffectInfo::RELATION nAllowed = CSEffectInfo::RELATION_ENEMY);
	void						UseTargetUI(bool bUse)		{ m_bTargetUI = bUse; }

	virtual stTargetID*			GetTargetInfo()									{ return NULL; }
	virtual bool				GetTargetIDByUID(MUID uid, int& nOutGroupID, int& nOutDummyID)	{ return false; }

	virtual TARGET_TYPE			GetTargetType()		{ return XTT_BASE; }
};

#endif // _XTARGET_H