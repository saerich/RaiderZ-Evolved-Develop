#pragma once

struct SENSOR_WARP_INFO;
class GEntityPlayer;

class GWarpInfoChecker : public MTestMemPool<GWarpInfoChecker>
{
public:
	enum Result
	{
		FULLHOUSE = -1,		// �ش� �ʵ尡 ����
		FAILURE = 0,		// ������ ���� ����
		SUCCESS,			// ������ ���� ����
	};

public:
	GWarpInfoChecker(const SENSOR_WARP_INFO& warpInfo);
	virtual ~GWarpInfoChecker();

	virtual bool CheckCondition(GEntityPlayer* pPlayer) const;
	virtual Result CheckQuestField(GEntityPlayer* pPlayer) const;	

	bool IsTrialWarp(void) const;
	bool IsSharedWarp(void) const;
	bool IsAutoWarp(void) const;
	bool IsManualWarp(void) const;
	bool IsAutoTrialWarp(void) const;
	bool IsManualTrialWarp(void) const;
	bool IsAutoSharedWarp(void) const;
	bool IsManualSharedWarp(void) const;
	bool IsSingleQuestWarp(void) const;
	int GetQuestLimitPersonnel(void) const;
	int GetFieldID(void) const;
	int GetFieldGroupID(void) const;
	int GetMarkerID(void) const;
	const SENSOR_WARP_INFO* GetSensorWarpInfo(void) const;

	bool SameFieldWarp(const GWarpInfoChecker* pChecker) const;	
	int GetAllowEnterPlayerCount(GEntityPlayer* pPlayer) const;

private:
	const SENSOR_WARP_INFO& m_warpInfo;	
};
