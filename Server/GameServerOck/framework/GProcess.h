#ifndef _GPROCESS_H
#define _GPROCESS_H

#include <vector>
using namespace std;

#include "MUID.h"
#include "MCommandManager.h"
#include "MiNetCommon.h"
using namespace minet;

/// ���μ��� ����
enum GProcessStat
{
	PS_STOPPED		= 0,
	PS_RUNNING,
};

/// ���μ���
class GProcess
{
private:
	GProcessStat		m_nStat;
	MUID				m_UID;				///< ���μ����� ID
protected:
	virtual void OnRun() {} 
	virtual void OnTerminate() {}
public:
	GProcess(MUID& uid);
	virtual ~GProcess();
	void Run();
	void Terminate();
	const MUID GetUID(void) const;
	const int GetStat() const		{ return m_nStat; }
};


#endif