#pragma once

#include "GSharedField.h"
#include "GWeatherMgr.h"
#include "MMemPool.h"

class GSharedFieldNavigator;
class GFieldMgr;	// ���� �Ǿ����

typedef map<CHANNELID, GSharedField*> SHAREDFIELD_MAP;



//////////////////////////////////////////////////////////////////////////
// 
// GSharedFieldMaster
//
//////////////////////////////////////////////////////////////////////////

class GSharedFieldMaster
{
public:
						GSharedFieldMaster(GFieldInfo* pFieldInfo);
	virtual				~GSharedFieldMaster();

	SHAREDFIELD_MAP&	GetChannels();
	void				GetChannels(vector<int>& vecChannelID);
	GSharedField*		GetChannel(int nChannelID);

	int					GetFieldID()		{ return m_nFieldID; }
	int					NewChannelID();

	GSharedField* CreateSharedField(MUID uidNewField);
	bool AddChannel(GSharedField* pField, CHANNELID nNewChannelID);
	GSharedField*		DeleteChannel(int nChannelID); // �����Ǵ� ä���� ��ȯ
	CHANNELID			GetEnterableChannelID();

private:
	void				Update(float fDelta);
	
	GSharedField*		GetEnterableChannel();
	int					GetDefaultChannelCount()		{ return m_nDefaultChannelCount; }

	void				Clear();	
	
private:
	friend GFieldMgr;	// ���� �Ǿ����
		
	int					m_nFieldID;
	int					m_nDefaultChannelCount;
	SHAREDFIELD_MAP		m_ChannelMap;		///< ä�� �ʵ� �� (first: ä�� ���̵�)
	GWeatherMgr			m_WeatherMgr;		///< ���� ����
	GSharedFieldNavigator*	m_pNavigator;	///< ä�� ���԰���
};

class GSharedFieldMasterMLeaf : public GSharedFieldMaster, public MMemPool<GSharedFieldMasterMLeaf>
{
public:
	GSharedFieldMasterMLeaf(GFieldInfo* pFieldInfo): GSharedFieldMaster(pFieldInfo) {}
	virtual ~GSharedFieldMasterMLeaf() {}
};