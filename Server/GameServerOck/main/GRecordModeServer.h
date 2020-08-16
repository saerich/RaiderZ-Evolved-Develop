#pragma once

#include "GNetServer.h"
#include "MCommandProfiler.h"

class GRecordModeServer;

//////////////////////////////////////////////////////////////////////////
//
//	GCommandRecroder
//
//////////////////////////////////////////////////////////////////////////

class GCommandRecroder : public MTestMemPool<GCommandRecroder>
{
public:
	GCommandRecroder();
	~GCommandRecroder();
	
	void OnUpdate(float fDelta);

	void SerializeCommand( MCommand* pCommand );
private:
	ofstream* m_pOstream;
	float  m_fElapsedTime;
};

//////////////////////////////////////////////////////////////////////////
//
//	GCommandReplayer
//
//////////////////////////////////////////////////////////////////////////

class GCommandReplayer : public MTestMemPool<GCommandReplayer>
{
	struct ReplayData
	{
		float fElapsedTime;
		MCommand* pCommand;
	};
public:
	GCommandReplayer(GRecordModeServer* pOwner);
	~GCommandReplayer();
	
	void OnUpdate(float fDelta);

private:
	void Load();
	bool DeserializeCommand(ifstream& s, ReplayData& data);
private:
	float  m_fElapsedTime;
	deque<ReplayData> m_qReplayDatas;
	size_t				m_nMaxReplayData;
	GRecordModeServer* m_pOwner;
};


//////////////////////////////////////////////////////////////////////////
//
//	GRecordModeServer
//
//////////////////////////////////////////////////////////////////////////

class GRecordModeServer : public GNetServer
{
public:
	GRecordModeServer(const MNetServerDesc& desc);
	~GRecordModeServer(void);

	MCommandProfiler& GetCommandProfiler() 	{ return *m_pCommandProfiler; }

	/// Ŀ�´������Ͱ� Ŀ�ǵ带 ó���ϱ� ���� �Ҹ��� �Լ�
	virtual MCommandResult OnCommand(MCommand* pCommand) override;
private:
	/// Ŀ�´������� ����
	virtual void OnRun(void) override;
private:
	void OnUpdate(float fDelta);
private:
	uint32 m_nLastTime;
	GCommandRecroder* m_pRecorder;
	GCommandReplayer* m_pReplayer;
};
