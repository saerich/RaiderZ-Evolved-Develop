#include "stdafx.h"
#include "XModule.h"
#include "crtdbg.h"

XModule::XModule(XObject* pOwner) : m_pContainer(NULL), m_pOwner(pOwner)
{

}

XModule::~XModule()
{

}

void XModule::Active(bool bActive)
{
	m_pContainer->ActiveModule(GetID(), bActive);
}

void XModule::Subscribe(int nMessageID)
{
	m_pContainer->SubscribeEvent(GetID(), nMessageID);
}

bool XModule::IsActivate()
{
	return (m_pContainer->IsActiveModule(GetID()));
}

XModule::E_OWNER_TYPE XModule::DecideOwnerType()
{
	E_OWNER_TYPE eType = E_OWNER_ACTOR;;
	if (m_pOwner->GetEntityType() == ETID_NPC)
	{
		eType = E_OWNER_NPC;
	}
	else if (m_pOwner->GetEntityType() == ETID_PLAYER)
	{
		if ( XGetMyUID() == m_pOwner->GetUID())
			eType = E_OWNER_MY_PLAYER;
		else
			eType = E_OWNER_NET_PLAYER;
	}

	return eType;
}

//////////////////////////////////////////////
//// XModuleContainer
XModuleContainer::XModuleContainer()
{


}

XModuleContainer::~XModuleContainer()
{

}

XModule *XModuleContainer::GetModule(int nID)
{
	XModuleMap::iterator i = m_Modules.find(nID);

	if(m_Modules.end()!=i) {
		return i->second;
	}

	return NULL;
}

void XModuleContainer::AddModule(XModule *pModule,bool bInitialActive)
{
	XModuleMap::iterator i = m_Modules.find(pModule->GetID());
	if(i!=m_Modules.end()) {
		_ASSERT(0);	// �ߺ��Ǿ���
	}

	m_Modules.insert(XModuleMap::value_type(pModule->GetID(),pModule));
	if(bInitialActive)
		ActiveModule(pModule->GetID());

	pModule->m_pContainer = this;
	pModule->OnSubscribeEvent();

	pModule->OnAdd();
}

void XModuleContainer::RemoveModule(XModule *pModule)
{
	if (pModule == NULL) return;		// �ΰ��̸� ����
	XModuleMap::iterator i = m_Modules.find(pModule->GetID());
	if(i==m_Modules.end()) return;	// ���� ����̴�

	ActiveModule(pModule->GetID(),false);

	m_Modules.erase(i);
	pModule->OnRemove();
}

void XModuleContainer::ActiveModule(int nID, bool bActive)	// Ư�� ����� (��)Ȱ��ȭ ��Ų��
{
	// Ȱ��ȭ ��Ų��
	if(bActive) {
		if(IsActiveModule(nID)) return;	// �̹� Ȱ��ȭ �Ǿ��ִ�


		XModuleMap::iterator i = m_Modules.find(nID);
		if(i!= m_Modules.end()) {
			i->second->OnActivate();
		}

		// ��� ���� ������ �ٽ� �ִ´�.
		m_ActiveModules.clear();
		for(XModuleMap::iterator it = m_Modules.begin(); it!= m_Modules.end(); ++it)
		{
			m_ActiveModules.push_back(it->second);
		}

	}else{
	// ��Ȱ��ȭ ��Ų��
		for(XModuleVector::iterator i = m_ActiveModules.begin(); i != m_ActiveModules.end(); ++i)
		{
			if((*i)->GetID() == nID)
			{
				(*i)->OnDeactivate();
				m_ActiveModules.erase(i);
				break;
			}
		}
	}
}

bool XModuleContainer::IsActiveModule(int nID)
{
	for(XModuleVector::iterator i = m_ActiveModules.begin(); i != m_ActiveModules.end(); ++i)
	{
		if((*i)->GetID() == nID)
		{
			return true;
		}
	}

	return false;
}


void XModuleContainer::UpdateModules(float fDelta)
{
	// Ȱ��ȭ �Ǿ��ִ� ��⸸ ������Ʈ�� �ҷ��ش�
	for(XModuleVector::iterator i = m_ActiveModules.begin(); i != m_ActiveModules.end(); ++i)
	{
		(*i)->Update(fDelta);
	}
}

void XModuleContainer::SubscribeEvent(int nModuleID, int nMessageID)
{
	m_ModuleMessageTypes[nModuleID].insert(nMessageID);
}

void XModuleContainer::Event(XEvent& msg)
{
	for(XModuleVector::iterator i = m_ActiveModules.begin(); i != m_ActiveModules.end(); ++i)
	{
		if (m_ModuleMessageTypes[(*i)->GetID()].find(msg.m_nID) != m_ModuleMessageTypes[(*i)->GetID()].end())
		{
			(*i)->Event(msg);
		}
	}
}

void XModuleContainer::PostUpdateModules( float fDelta )
{
	// Ȱ��ȭ �Ǿ��ִ� ��⸸ ������Ʈ�� �ҷ��ش�
	for(XModuleVector::iterator i = m_ActiveModules.begin(); i != m_ActiveModules.end(); ++i)
	{
		(*i)->PostUpdate(fDelta);
	}
}