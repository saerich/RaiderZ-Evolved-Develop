#pragma once

#include "SServerApplication.h"
#include "LServerSystems.h"
#include "LServerManagers.h"

class LBaseApplication : public SServerApplication
{
protected:
	LServerSystems	m_Systems;						///< ���� ��ü���� ����ϴ� �ý��� ����
	LServerManagers	m_Managers;						///< ���� ��ü���� ����ϴ� �Ŵ��� ����

	virtual	bool	OnCreate() override;
	virtual void	OnDestroy() override;
	virtual bool	OnUpdate(float fDelta) override;

	void AnalysisProfiling();
	void StartProfiling();


public:
	LBaseApplication();
	virtual			~LBaseApplication();

};