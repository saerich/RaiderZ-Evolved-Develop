#pragma once

#include "STypes.h"
#include "CCommandResultTable.h"

struct TD_MAIL_MAILBOX_MAIL_INFO;

class GServerMailRouter
{
public:
	// gmock�� �������̽� Ŭ�������� �Ҹ��ڸ� �䱸�Ѵ�.
	virtual ~GServerMailRouter()	{}

	virtual void RouteNewMailToReceiver(CID nCID, const TD_MAIL_MAILBOX_MAIL_INFO* pMailInfo) = 0;
	virtual void RetryTransNewMail(CID nCID, const TD_MAIL_MAILBOX_MAIL_INFO* pMailInfo) = 0;
};
