#ifndef _XTARGET_COLLISIONBOX_H
#define _XTARGET_COLLISIONBOX_H

#include "XTargetLine.h"

/// Target Info ������ Ưȭ�Ǿ� �ٸ� ���� ����ϱ� ������
class XTargetCollisionBox: public XTargetLine
{
protected:

	virtual void GetCameraInfo( vec3& vPos, vec3& vDir );
	virtual bool CheckLineTargetObj(XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist);
	virtual bool CheckTargetAllowed( XObject* pObject );
	
public:
	XTargetCollisionBox(XObject* pObject);
	virtual ~XTargetCollisionBox();

	bool	CheckTargetObj( XObject* pObject);
	void	SetIndicatorEffect();
	void	ChangeIndicatorEffectPos();
	void	DeleteIndicatorEffect();
	bool	IsIndicatorEffect();

	virtual TARGET_TYPE			GetTargetType()		{ return XTT_COLLISIONBOX; }
};

#endif // _XTARGET_COLLISIONBOX_H