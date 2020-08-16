#pragma once

class XGlobalInfoInstance;
class XGlobalInstance;

class XGameInitializer
{
protected:
	bool	m_bInited_Pre_CreatingWindow;
	bool	m_bInited_Post_CreatingWindow;
	bool	m_bInited_While_LoadingLoginCampaign;
	bool	m_bInited_Pre_LoadingLoginCampaign;

	void InitConfig(XGlobalInfoInstance* pGlobalInfoInstance);
	void ReadCountryLanguages(XGlobalInfoInstance* pGlobalInfoInstance);
	void InitRS3();
	bool InitResources(XGlobalInstance* pGlobalInstance);
public:
	XGameInitializer();
	/// ������ �����Ǳ� ���� �ʱ�ȭ
	bool Init_Pre_CreatingWindow(XGlobalInfoInstance* pGlobalInfoInstance, XGlobalInstance* pGlobalInstance);

	/// ������ ������ �Ŀ� �ʱ�ȭ
	bool Init_Post_CreatingWindow(XBaseApplication* pApp, XGlobalInfoInstance* pGlobalInfoInstance, XGlobalInstance* pGlobalInstance);

	/// ĳ���� ���� �ϱ� ���� �ʱ�ȭ
	bool Init_Pre_LoadingLoginCampaign();

	/// ĳ���� ���� �ϱ� ���� �ε� ȭ�鿡�� �ʱ�ȭ
	bool Init_While_LoadingLoginCampaign();

#ifndef _PUBLISH
	/// ���� ��� �ؽ�Ʈ �ٽ� �б�
	void ReloadCountryLanguages(XGlobalInfoInstance* pGlobalInfoInstance)
	{
		ReadCountryLanguages( pGlobalInfoInstance);
	}
#endif
};

