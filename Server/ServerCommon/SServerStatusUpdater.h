#pragma once

#include "ServerCommonLib.h"
#include "MTstring.h"

class MRegulator;

class SCOMMON_API SServerStatusUpdater
{
public:
	SServerStatusUpdater();
	virtual ~SServerStatusUpdater();

	void			Update(const float fDelta);

public:
	virtual void	Start(const std::wstring& strServerVersion = L"") = 0;
protected:
	virtual void	OnUpdate(const float fDelta) = 0;

private:
	MRegulator* m_pServerStatusUpdateRglt;

};

#define MAX_UPDATE_SERVERSTATUS_ELAPSEDTIME_SEC 30			///< �ֱ������� ������Ʈ �ϴ� �ð�.