#pragma once

#include "ServerCommonLib.h"

class MRegulator;

class SCOMMON_API SServableChecker
{
public:
	static float TIME_UPDATE_SERVABLECHECKE_ELAPSEDTIME_SEC;
public:
	SServableChecker();
	virtual ~SServableChecker();

	bool IsServable() { return m_bIsServable; }
	void Update(const float fDelta);

protected:
	/// @desc			override �ؼ� �ۼ��մϴ�.
	///					���ϰ��� ���� Servable �Ӽ��� ���õ˴ϴ�.
	///
	/// @return	true	Servable �Ӽ��� true �� �����˴ϴ�.
	///			false	Servable �Ӽ��� false �� �����˴ϴ�.
	virtual bool Check(const float fDelta) = 0;

private:
	void SetServable(bool bIs) { m_bIsServable = bIs; }

	MRegulator* m_pServableCheckerRglt;
	bool m_bIsServable;

};
