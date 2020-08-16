#pragma once

#include "SServerApplication.h"
#include "PServerSystems.h"
#include "PServerManagers.h"


class PBaseApplication : public SServerApplication
{
protected:
	PServerSystems	m_Systems;						///< ���� ��ü���� ����ϴ� �ý��� ����
	PServerManagers	m_Managers;						///< ���� ��ü���� ����ϴ� �Ŵ��� ����

	virtual	bool	OnCreate() override;
	virtual void	OnDestroy() override;
	virtual bool	OnUpdate(float fDelta) override;

	void AnalysisProfiling();
	void StartProfiling();


public:
	PBaseApplication();
	virtual			~PBaseApplication();

};