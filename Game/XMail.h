#pragma once


struct TD_ITEM;

struct XMailContent
{
	/// ó�� ������ �޾ƿ� �� true �� �����ϰ� ���� ������ ���� ���� ������ ��û���� �ʰ� ���� �����͸� �н��ϴ�.
	bool		bFill;

	wstring				strText;
	int					nAppendedMoney;
	vector< TD_ITEM>	vAppendedItems;

	void DelAppendedItem(int nMailSlotID)
	{
		vector< TD_ITEM>::iterator itr = vAppendedItems.begin();
		for (; itr < vAppendedItems.end(); ++itr)
		{
			if ((*itr).m_nSlotID == nMailSlotID)
			{
				vAppendedItems.erase(itr);
				break;
			}
		}
	}

	/// TODO - mailbox : ÷�� ������ ����� ��� �־�� �մϴ�.

	XMailContent()
	: bFill(false)
	, nAppendedMoney(0) {}
};

class XMail
{
public:
	XMail(MUID uidMail
		, MAIL_TYPE eType
		, wstring strSenderName
		, wstring strMailTitle
		, int nAppendedItemID
		, int nAppendedIMoney
		, bool bIsRead
		, int nExpiServerTimeSec);
	~XMail();

	MUID			GetUID()				{ return m_nMailUID; }
	void			SetRead()				{ m_bIsRead = true; }
	bool			IsRead() const			{ return m_bIsRead; }
	wstring			GetSender()				{ return m_strSender; }
	wstring			GetTitle()				{ return m_strTitle; }
	XMailContent&	GetContent()			{ return m_content; }

	int				GetThumbnailItemID()	{ return m_nThumbnailItemID; }
	void			SetThumbnailItemID(int nNewItemID)	{ m_nThumbnailItemID = nNewItemID; }

	void			InitSystemTimeMSForExpiCheck(uint32 nExpiStartTimeMS)	{ m_nExpiCheckStartTimeMS = nExpiStartTimeMS; }
	int				GetMailRemaindDay();	///< ���� ��¥.

private:
	MUID			m_nMailUID;
	MAIL_TYPE		m_eType;

	wstring			m_strSender;
	wstring			m_strTitle;
	int				m_nExpiServerTimeSec;		///< Server ���� ���� ������ ���� ��ȿ�Ⱓ. (��)
	uint32			m_nExpiCheckStartTimeMS;	///< Ŭ���̾�Ʈ�� ��ȿ�Ⱓ üũ ���۽ð�. (Millisecond)

	bool			m_bIsRead;
	int				m_nThumbnailItemID;
	XMailContent	m_content;

};
