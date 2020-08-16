#include "stdafx.h"
#include "GItemHandler.h"

GItemHandler GItemHandler::m_stInstance;

GItemHandler::GItemHandler()
{

}

GItemHandler::~GItemHandler()
{
	// ��ϵ� �ڵ鷯 �޸� ������ ���⼭ �Ѵ�.
	for (set<IItemHandler*>::iterator itor = m_HandlerSet.begin(); itor != m_HandlerSet.end(); ++itor)
	{
		delete (*itor);
	}
	m_HandlerSet.clear();
	m_HandlerMap.clear();
}

IItemHandler* GItemHandler::GetHandler(int nItemID)
{

	return NULL;
}

void GItemHandler::RegisterItemHandler(int nItemID, IItemHandler* pHandler)
{
	m_HandlerMap.insert(map<int, IItemHandler*>::value_type(nItemID, pHandler));
	m_HandlerSet.insert(pHandler);		// ���߿� �޸� ������ ���ؼ�..
}

///////////////////////////////////////////////////////////////////////////
vector<int> GPotionItemHandler::m_stItemIds;

GPotionItemHandler::GPotionItemHandler()
{

}

GPotionItemHandler::~GPotionItemHandler()
{

}

void GPotionItemHandler::UseItem(MUID& uidEntity, MUID& uidItem)
{
	mlog("Used potion.\n");
}
