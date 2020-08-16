#ifndef _GPLAYER_DB_H
#define _GPLAYER_DB_H

#include "GDef.h"
#include "GAccountInfo.h"

class GPlayerObject;
class GTalentInfoMgr;

// ���� DB �۾������ϸ� ������ Ŭ����.
// �ϴ� �÷��̾� ������ ���Ͽ� �����صд�.
class GPlayerDB
{
private:
	string GetFullFileName(string strUserID);
public:
	GPlayerDB() {}
	virtual ~GPlayerDB() {}

	virtual bool Save(GPlayerObject* pPlayerObject);	
	virtual bool Load(GPlayerObject* poutPlayerObject);
	virtual bool LoadForAccountCharList(GAccountCharList& outCharList);

	virtual bool Delete(const char* szName);
};





#endif