#pragma once

#include "CSCommonLib.h"

enum QOBJECTIVE_TYPE;
enum QO_PUBLIC_PROGRESS;

/// ����Ʈ ���� ����
class CSCOMMON_API CSQObjectiveInfo
{
public:
	CSQObjectiveInfo();
	virtual ~CSQObjectiveInfo();

public:
	int						nID;			///< ���� ID
	int						nQuestID;		///< ����Ʈ ID
	tstring					strDesc;		///< ������ ��� ����� �ʵ�
	QOBJECTIVE_TYPE			nType;			///< Ÿ��
	vector<int>				vecParam1;
	int						nParam2;
	int						nFieldID;		///< �ʵ� ID
	int						nMarkerID;		///< ��Ŀ ID
	int						nFieldID2;		///< �ʵ� ID
	int						nMarkerID2;		///< ��Ŀ ID
	QO_PUBLIC_PROGRESS		nPublicProgress;///< ���� ���� ����� ������
};
