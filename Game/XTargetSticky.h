#ifndef _XTARGET_STICKY_H
#define _XTARGET_STICKY_H

#include "XTargetLine.h"

class XTargetSticky : public XTargetLine
{
private:
	// Ÿ�� ������Ʈ
	void						UpdateStickyTarget(vec3& pos, vec3& dir);

	// Ÿ�� ���� ���
	bool						CheckLineTarget(XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist);
	virtual bool				CheckLineTargetObj(XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist);

	// �������� �˻�
	bool						CheckTargetObject(XObject* pObject);
	
public:
	XTargetSticky(XObject* pObject);
	virtual ~XTargetSticky();

	virtual void				UpdateTarget();
	virtual TARGET_TYPE			GetTargetType()		{ return XTT_STICKY; }
};

#endif // _XTARGET_STICKY_H