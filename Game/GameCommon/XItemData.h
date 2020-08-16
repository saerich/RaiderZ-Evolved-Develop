#pragma once

#include "CSItemData.h"

class XItemData: public CSItemData
{
public:
	XItemData();
	virtual ~XItemData();

	const TCHAR* GetName(void);		// ���ڿ����̺� ����
	const TCHAR* GetDesc(void);

private:	
	using CSItemData::m_strName;	// ���� ���� ���ϵ��� �����ϴ�
	using CSItemData::m_strDesc;
};
