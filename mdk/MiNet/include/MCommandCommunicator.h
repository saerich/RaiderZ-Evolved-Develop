#ifndef MCOMMANDCOMMUNICATOR_H
#define MCOMMANDCOMMUNICATOR_H

#include "MiNetLib.h"
#include "MiNetCommon.h"
#include "MiNetCommonDef.h"
#include "MCommandManager.h"
#include "MUID.h"
#include "MPacketCrypter.h"
#include "MCommandHandler.h"
#include "MCommandHistory.h"
#include "MThread2.h"

namespace minet {

class MCommandStream;
class MHandlerBase;
class MCommandProfiler;


/// Ŀ�ǵ� ����� ����� ���������� Ŀ�´�������
class MINET_API MCommandCommunicator
{
private:
	enum CrowdState					
	{
		MCS_COMFORTABLE = 0,
		MCS_CROWDED,
		MCS_OVERFULL
	};

private:
	cml2::MCriticalSection		m_csSafeCmdQueue;
	MCommandList				m_SafeCmdQueue;

	cml2::MCriticalSection		m_csNewUID;
protected:
	MUIDGenerator				m_uidGenerator;

	MCommandProfiler*			m_pCommandProfiler;
	MCommandHistory				m_CommandHistory;

	MCommandManager				m_CommandManager;					///< Ŀ�ǵ� �Ŵ���
	MUID						m_This;								///< �ڱ� Ŀ�´������� UID
		
	CrowdState					m_nCrowdState;

protected:
	virtual MCommand* GetCommand();

	/// Low-Level Command Transfer Function. ���߿� ��Ƶξ��ٰ� �� ���۵��� �����ϰ� ���� �� �ִ�.
	virtual void SendCommand(MCommand* pCommand) {}
	/// Low-Level Command Transfer Function. ���߿� ��Ƶξ��ٰ� �� ���۵��� �����ϰ� ���� �� �ִ�.
	virtual void ReceiveCommand(MCommand* pCommand);

	/// Ŀ�´������Ͱ� Ŀ�ǵ带 ó���ϱ� ���� �Ҹ��� �Լ�
	virtual MCommandResult OnCommand(MCommand* pCommand);
	/// Ŀ�´������� ���� �� �غ�
	virtual void OnPrepareRun(void);
	/// Ŀ�ǵ带 ó���ϱ� ����
	virtual void OnPrepareCommand(MCommand* pCommand);
	/// Ŀ�´������� ����
	virtual void OnRun(void) {}
	void Clear();

	/// ���⵵
	virtual void OnCheckCrowded()	{}

	void SetCrowded();
	void SetOverfull();
	bool IsOverfull();
	bool IsCrowded();
	bool IsComfortable();

public:
	MCommandCommunicator(void);
	virtual ~MCommandCommunicator(void);

	/// �������Ϸ� Ȱ��ȭ
	void InitProfiler();
	void ProfileAnalysis(const char* szFolder, const char* szFileName, bool bSaveCSV);

	/// This UID ����
	void SetUID(MUID& uidThis)		{ m_This = uidThis; }

	/// Ŀ�ǵ� �Է� - �����忡 ������.
	virtual bool Post(MCommand* pCommand);

	/// ���� ( Ŀ�ǵ� ���۹� ó�� )
	void Run(void);

	/// Ŀ�ǵ� �Ŵ��� ���
	MCommandManager* GetCommandManager(void)	{ return &m_CommandManager; }

	/// Ŀ�ǵ� ����
	MCommand* NewCommand(int nCmdID, MUID TargetUID);
	MCommand* NewLocalCommand(int nCmdID);

	/// Ŀ�ǵ� �ڵ鷯 ����
	virtual void SetCommandHandler(int nCmdID, MCommandHandler* pHandler, MCommandHanderFunc* fnFunc);

	/// �ڽ��� UID�� ����.
	MUID GetUID(void){ return m_This; }

	MUID NewUID();

	/// Ŀ�ǵ� �������Ϸ� ����. delete�� ���⼭ ���ش�.
	void AttachCommandProfiler(MCommandProfiler* pCommandProfiler)
	{
		m_pCommandProfiler = pCommandProfiler;		
	}
};


} // namespace minet

#endif
