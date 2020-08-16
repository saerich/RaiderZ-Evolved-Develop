#pragma once

//////////////////////////////////////////////////////////////////////////
// 광맥등의 채집 가능 NPC는 채집이 불가능하면 인터랙션(플래그 false) 조차 안된다.
// 그래서 메세지 출력에는 타겟NPC UIID를 사용
//////////////////////////////////////////////////////////////////////////

class XGatherMSG
{
public:
	XGatherMSG(void)	{}
	virtual ~XGatherMSG(void)	{}

	bool	Check(const UIID nUIID);
};
