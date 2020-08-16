#ifndef _GQUEST_INFO_H
#define _GQUEST_INFO_H

#include "CSQuestInfo.h"
#include "MXml.h"

class GItemAddRemoveInfo;
class GQObjectiveInfo;
class CSQObjectiveInfo;

/// ����Ʈ ����
class GQuestInfo : public CSQuestInfo, public MTestMemPool<GQuestInfo>
{
public:
	GQuestInfo();
	virtual ~GQuestInfo();	

	GQObjectiveInfo* Get(int nQuestID);

	vector<GQObjectiveInfo*> GetAllQObjectiveInfo();
	vector<GQObjectiveInfo*> GetActQObjectiveInfo();
	vector<GQObjectiveInfo*> GetEscortQObjectiveInfo();
	vector<GQObjectiveInfo*> GetQObjectiveInfo(QOBJECTIVE_TYPE nType);

private:
	vector<GQObjectiveInfo*> ToGQObjectiveInfo(vector<CSQObjectiveInfo*> vecInfo);

public:		
	GItemAddRemoveInfo*	pItemAddRemoveInfo;	///< ����Ʈ �����Ҷ� ���޹ްų� ���ŵǴ� ������

	bool				bChallengerQuest;	///< ����������Ʈ���� ����

};


#endif // _GQUEST_INFO_H