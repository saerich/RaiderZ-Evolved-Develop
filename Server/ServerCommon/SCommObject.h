#pragma once

#include "MUID.h"
#include <map>
using namespace std;

#include "ServerCommonLib.h"


class SCommObjectManager;
class PServer;

/// �÷��̾� ������Ʈ
class SCOMMON_API SCommObject
{	
protected:
	friend class SCommObjectManager;

	bool				m_bCreated;
	MUID				m_UID;				///< �÷��̾� UID. Link UID�� ���� ���̴�.
protected:
	SCommObject() {}

public:
	SCommObject(MUID& uid);
	virtual ~SCommObject();
	virtual void Create();							///< GPlayerObjectManager���� ȣ��
	virtual void Destroy();							///< GPlayerObjectManager���� ȣ��

	const MUID		GetUID(void) const;
};

