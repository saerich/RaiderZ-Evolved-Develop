#ifndef _GPROCESS_MANAGER_H
#define _GPROCESS_MANAGER_H

#include "GProcess.h"

/// ���μ��� ������
class GProcessManager
{
private:
	vector<GProcess*>		m_vecProcesses;
public:
	GProcessManager();
	~GProcessManager();
	void Register(GProcess* pProcess);

	void Run();
};

#endif