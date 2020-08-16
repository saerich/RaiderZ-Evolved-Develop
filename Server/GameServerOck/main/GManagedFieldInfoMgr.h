#pragma once

#include "GFieldInfo.h"
#include "STransData_M2G.h"

class GFieldInfoMgr;

/// ������ �����κ��� �����ǰ� �ִ� ���� �ʵ� ����
class GManagedSharedFieldInfo : public MTestMemPool<GManagedSharedFieldInfo>
{
private:
	set<int>				m_setChannelID;
public:
	struct _ChannelInfo
	{
		int		nChannelID;
		MUID	uidField;		///< ������ �����κ��� �Ҵ���� Field UID
	};

	int						m_nFieldID;
	vector<_ChannelInfo>	m_vecChannelInfo;

	bool ExistChannel(int nChannelID);
	void AddChannel(int nChannelID, MUID uidField);
	void DelChannel(MUID uidField);
	MUID GetChannelFieldUID(int nChannelID);
	int GetChannelCount() { return (int)m_setChannelID.size(); }
};

/// ������ �����κ��� �����ǰ� �ִ� �ʵ� ���� ������
class GManagedFieldInfoMgr : public map<int, GManagedSharedFieldInfo*>, public MTestMemPool<GManagedFieldInfoMgr>
{
public:
	GManagedFieldInfoMgr();
	virtual ~GManagedFieldInfoMgr();

	void AssignSharedField(TDMG_MANAGED_SHARED_FIELD* pFIelds, int nCount);
	void AssignAllForStandAlone(GFieldInfoMgr* pFieldInfoMgr);

	void Insert(GManagedSharedFieldInfo* pInfo);
	void Clear();
	GManagedSharedFieldInfo* Find(int nFieldID);
	GManagedSharedFieldInfo* FindByUID(MUID uidField);
	bool Exist(int nFieldID, int nChannelID);
	bool Exist(int nFieldID);
	void AddChannel(MUID uidField, int nFieldID, int nChannelID);
	void DelChannel(MUID uidField);

	void GetChannelID(vector<CHANNELID>& vecChannelID, int nFieldID);
	MUID GetFieldUID(int nFieldID, int nChannelID);

	int GetAllChannelCount();
};
