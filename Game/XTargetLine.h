#ifndef _XTARGET_LINE_H
#define _XTARGET_LINE_H

#include "XTarget.h"

class XTargetLine : public XTarget
{
protected:
	stTargetID*					m_pTarget;

protected:
	// Ÿ�� ������Ʈ
	void						UpdateTarget(vec3& pos, vec3& dir);

	// Ÿ�� ���� ���
	bool						CheckLineTarget(XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist);
	virtual bool				CheckLineTargetObj(XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist);

	// �������� �˻�
	void						CheckExistTargetObj();
	bool						CheckTargetObject(XObject* pObject, vec3& pos);
	bool						CheckOverlapDummy(stTargetID& targetObject);				// �ߺ� üũ
	
public:
	XTargetLine(XObject* pObject);
	virtual ~XTargetLine();

	virtual void				UpdateTarget();
	virtual void				DelTargetAll();

	virtual stTargetID*			GetTargetInfo()									{ return m_pTarget; }
	virtual bool				GetTargetIDByUID(MUID uid, int& nOutGroupID, int& nOutDummyID);
	virtual TARGET_TYPE			GetTargetType()		{ return XTT_LINE; }
};

#endif // _XTARGET_LINE_H