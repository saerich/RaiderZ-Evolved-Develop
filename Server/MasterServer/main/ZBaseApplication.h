#pragma once

#include "SServerApplication.h"
#include "ZServerInfo.h"
#include "ZGlobal.h"
#include "ZServerSystems.h"
#include "ZServerManagers.h"
#include "MTime.h"
#include "MUID.h"
#include "MDebug.h"


class ZBaseApplication : public SServerApplication
{
protected:
	ZServerSystems	m_Systems;						///< ���� ��ü���� ����ϴ� �ý��� ����
	ZServerManagers	m_Managers;						///< ���� ��ü���� ����ϴ� �Ŵ��� ����

	virtual	bool	OnCreate() override;
	virtual bool	OnUpdate(float fDelta) override;
	virtual void	OnDestroy() override;

	void StartProfiling();
	void AnalysisProfiling();

public:
	ZBaseApplication();
	virtual			~ZBaseApplication();
};