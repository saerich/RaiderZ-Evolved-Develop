#pragma once

#include "CSWorldInfo.h"
#include "CSTriggerInfo.h"
#include "STransData_M2G.h"
#include "GPlayerFieldEntry.h"

class GEntityActor;
class GEntityPlayer;
class GField;
class GPlayerFieldListener;
class GDynamicField;
class GSharedField;
class GDynamicFieldMaster;
struct FIELD_GROUP_INFO;
enum DYNAMIC_FIELD_TYPE;


/// �÷��̾��� �ʵ� ���� ó��
class GPlayerFieldEntry
{
	friend class GPlayerField;
private:
	struct WarpReservedInfo
	{
		int nField;
		MUID uidField;
		vec3 vPos;
		vec3 vDir;
		WarpReservedInfo() {Reset();}
		void Reset()
		{
			nField = 0;
			uidField = 0;
			vPos = vec3::ZERO;
			vDir = vec3::AXISY;
		}
	};
private:
	GPlayerFieldListener*		m_pLisener;					///< ������
	WarpReservedInfo			m_WarpReservedInfo;			///< �� �ʵ� UID, ��ġ, ����

	void SetLisener(GPlayerFieldListener* pLisener)		{ m_pLisener = pLisener; }
public:
	GPlayerFieldEntry() {}
	~GPlayerFieldEntry() {}
	void ReserveWarp(MUID uidField, vec3 vPos, vec3 vDir);
	void ReserveWarp(int nField, vec3 vPos);
	void ClearReserveWarp();

	// �ٸ� �ʵ�� �̵��Ϸ�
	bool OnEnterFieldComplete();

	// �ʵ��̵� ���� ��ȯ
	MUID	GetReservedWarpField()		{ return m_WarpReservedInfo.uidField; }
	int		GetReservedWarpFieldID()	{ return m_WarpReservedInfo.nField; }
	vec3	GetReservedWarpPos()		{ return m_WarpReservedInfo.vPos; }

	bool	IsReserved();
};
