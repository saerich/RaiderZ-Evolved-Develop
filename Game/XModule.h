#ifndef _XMODULE_H
#define _XMODULE_H

#include "XEvent.h"
#include "XModuleID.h"
#include <map>
#include <set>
using namespace std;

class XModuleContainer;
class XObject;

/// �ƹ����̳� ���� �� �ִ� ��� �⺻ Ŭ����
class XModule {
	friend XModuleContainer;

protected:

	enum E_OWNER_TYPE
	{
		E_OWNER_OBJECT = 0,
		E_OWNER_ACTOR,

		E_OWNER_NPC,
		E_OWNER_NET_PLAYER,
		E_OWNER_MY_PLAYER
	};
	XObject*					m_pOwner;										///< �̸���� ������(���⼭�� XObject Ŭ����)
	virtual E_OWNER_TYPE		DecideOwnerType();								///< ��� �������� Ÿ���� �����ϴ� �Լ�


	XModuleContainer*			m_pContainer;									///< �̸���� ����ִ� container
	void						Subscribe(int nMessageID);						///< �޼��� ����

public:
								XModule(XObject* pOwner=NULL);					///< ������
	virtual						~XModule();										///< �ı���
	void						Active(bool bActive = true);					///< (��)Ȱ��ȭ ��Ų��
	void						Update(float fDelta){ OnUpdate(fDelta); }		///< ������Ʈ
	void						PostUpdate(float fDelta){ OnPostUpdate(fDelta); }	///< ������Ʈ ���Ŀ� �ߵ��Ǵ� ������Ʈ
	XEventResult				Event(XEvent& msg)	{ return OnEvent(msg); }	///< �̺�Ʈ ó��
	XModuleContainer*			GetContainer()		{ return m_pContainer;}		///< �����̳� ��ȯ
	XObject*					GetOwner()			{ return m_pOwner; }		///< ������ ������Ʈ ��ȯ
	virtual int					GetID() = 0;
	void						Initialize()		{ OnInitialized(); }
	void						Finalize()			{ OnFinalized(); }
	bool						IsActivate();


protected:
	
	// �ʿ��� �κ��� ���� ��⿡�� ��ӹ޴´�.
	virtual void				OnAdd()					{}						///< �����̳ʿ� �߰��� �� ȣ��
	virtual void				OnRemove()				{}						///< �����̳ʿ��� ���ŵ� �� ȣ��
	virtual void				OnActivate()			{}						///< Ȱ��ȭ�� �� ȣ��
	virtual void				OnDeactivate()			{}						///< ��Ȱ��ȭ�� �� ȣ��
	virtual void				OnUpdate(float fDelta)	{}						///< �� ƽ���� ȣ��
	virtual void				OnPostUpdate(float fDelta)	{}					///< �� ƽ���� ȣ��, ������Ʈ ���� �ߵ�
	virtual void				OnInitialized()			{}						///< �ʱ�ȭ�� �� ȣ��
	virtual void				OnFinalized()			{}						///< �������� �� ȣ��
	virtual XEventResult		OnEvent(XEvent& msg){ return MR_IGNORED; }		///< �޼��� �ڵ鷯
	virtual void				OnSubscribeEvent()	{}							///< ���� �޼����� �����Ѵ�.
};

/// ����� ���� �� �ִ� �����̳� Ŭ����
class XModuleContainer 
{
private:
	typedef map<int, XModule*>	XModuleMap;
	typedef vector<XModule*>	XModuleVector;
	friend XModule;

	XModuleMap					m_Modules;
	XModuleVector				m_ActiveModules;

	std::set<int>				m_ModuleMessageTypes[MAX_MODULE_ID];
	void						SubscribeEvent(int nModuleID, int nMessageID);


public:

	XModuleContainer();
	virtual ~XModuleContainer();

	//////////////////////////////////////////////////////////////////////////
	// bInitialActive : Ȱ��ȭ �ʱ�ġ
	//
	void						AddModule(XModule *pModule, bool bInitialActive = true);
	void						RemoveModule(XModule *pModule);

	//////////////////////////////////////////////////////////////////////////
	// ��ϵ� ����� nID ����� ã�´�
	//
	XModule*					GetModule(int nID);

	void						ActiveModule(int nID, bool bActive = true);	// Ư�� ����� (��)Ȱ��ȭ ��Ų��
	bool						IsActiveModule(int nID);

	void						UpdateModules(float fDelta);
	void						PostUpdateModules(float fDelta);
	void						Event(XEvent& msg);
};

#define DECLARE_ID(_ID)			int GetID() { return _ID; }




#endif