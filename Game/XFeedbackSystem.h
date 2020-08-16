#pragma once

class XFeedbackFetcher;
class XFeedbackInfoMgr;

/// �ǵ�� ���� Facade
class XFeedbackSystem
{
protected:
	XFeedbackInfoMgr*		m_pFeedbackInfoMgr;
	XFeedbackFetcher*		m_pFeedbackFetcher;
	float					m_fUpdateObjectsElapsedTime;
	float					m_fUpdateFetchElapsedTime;

	void MakeFeedbackObjects();
	void UpdateFeedbackObjects(float fDelta);
	void UpdateFetch(float fDelta);
public:
	XFeedbackSystem();
	virtual ~XFeedbackSystem();

	void Update(float fDelta);

	/// ������ �ǵ�� xml ������ �����´�.
	void Fetch();

	void SendFeedback(const wchar_t* szText);

	void ReloadFeedbackFromWeb();
	void FetchFeedbackWeb();
	void SetShowFeedback( bool bShow );
};