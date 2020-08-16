#ifndef _MREFERENCE_COUNTED_H
#define _MREFERENCE_COUNTED_H

#include "RLib.h"
#include <map>

/// ���۷��� ī����
/// - ���۷��� ī���� ����� �ʿ��� Ŭ������ �� Ŭ������ ��ӹ����� �ȴ�.
class CML2_API MReferenceCounted
{
private:
	mutable long m_nReferenceCounter;
public:
	MReferenceCounted();
	virtual ~MReferenceCounted();
	void AddRef() const;
	bool Drop() const;
	long GetReferenceCount() const;
};



class CML2_API MRefMgr
{
private:
	typedef std::map<const MReferenceCounted*,int> MAP_REF;
	MAP_REF		m_RefObjs;
public:
	~MRefMgr();

	void AddRef(const MReferenceCounted* pObj);
	void Drop(const MReferenceCounted* pObj);
};


#endif // _MREFERENCE_COUNTED_H