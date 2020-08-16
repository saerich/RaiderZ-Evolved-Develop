#pragma once

#include "PmDef.h"
#include "PMPMSObject.h"

class LPMSObject : public PMPMSObject
{
public:
	LPMSObject(PMPMSConnector* pPMS);
	virtual ~LPMSObject(void);

	virtual BOOL OnRegionInfoReq(IPMSRegionInfoList* plstRegionInfo) override;
	virtual BOOL OnStatInfoReq( IPMSStatInfoList* plstStatInfo ) override;
	virtual BOOL OnRegionInfoPCReq(IPMSRegionInfoListPC* plstRegionInfo) override;
	virtual BOOL OnStatInfoPCReq(IPMSStatInfoListPC* plstStatInfo) override;
};
