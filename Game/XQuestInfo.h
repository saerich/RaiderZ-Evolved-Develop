#ifndef _XQUEST_INFO_H
#define _XQUEST_INFO_H

#include "CSQuestInfo.h"
#include "CSQObjectiveInfo.h"
#include "CSQuestInfoMgr.h"

/// ����Ʈ���� ����
class XQObjectiveInfo : public CSQObjectiveInfo
{
private:
	// ���� ���� �����ϴ�. ���ڿ����̺� �����ؾ��մϴ�.
	using	CSQObjectiveInfo::strDesc;
public:
	XQObjectiveInfo();
	virtual ~XQObjectiveInfo();

	vec3 GetPos(int nIndex = 1);
	const TCHAR* GetDesc(void);
};

/// ����Ʈ ����
class XQuestInfo : public CSQuestInfo
{
private:
	// ���� ���� �����ϴ�. ���ڿ����̺� �����ؾ��մϴ�.
	using	CSQuestInfo::strTitle;
	using	CSQuestInfo::strDesc;
	using	CSQuestInfo::strCategory;
public:
	XQuestInfo();
	virtual ~XQuestInfo();

	XQObjectiveInfo* Get(int nID);
	vector<XQObjectiveInfo*> GetAllQObjectiveInfo();
	bool GetIndex( size_t nIndex, XQObjectiveInfo** ppOutInfo);
	int GetObjectiveIndex( size_t nObjectiveID);

	const TCHAR* GetTitle(void);
	const TCHAR* GetDesc(void);
	const TCHAR* GetCategory(void);

private:
	vector<XQObjectiveInfo*> ToXQObjectiveInfo(vector<CSQObjectiveInfo*> vecInfo);
};


/// ����Ʈ ���� ������
class XQuestInfoMgr : public CSQuestInfoMgr
{
public:
	XQuestInfoMgr(void);
	virtual	~XQuestInfoMgr(void);

	virtual CSQuestInfo* NewQuestInfo();
	virtual CSQObjectiveInfo* NewQObjectiveInfo();
	XQuestInfo*	Get(int nQuestID);
	XQuestInfo*	GetRandomQuest();
	vector<XQuestInfo*> GetAllQuestInfo();
protected:
	virtual void Cooking();

private:
	vector<int> m_vecAllQuestID;
	vector<XQuestInfo*> m_vecAllQuestInfo;
};

#endif // _XQUEST_INFO_H