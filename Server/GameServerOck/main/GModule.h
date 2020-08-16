#ifndef _GMODULE_H
#define _GMODULE_H

#include "GMessage.h"
#include "MTypes.h"
#include "GModuleID.h"
#include <map>
#include <set>
using namespace std;


class GModuleContainer;
class GEntity;

/// �ƹ����̳� ���� �� �ִ� ��� �⺻ Ŭ����
class GModule {
	friend GModuleContainer;
protected:
	GModuleContainer*		m_pContainer;	///< �̸���� ����ִ� container
	GEntity*				m_pOwner;		///< �̸���� ������(���⼭�� GEntitySync Ŭ����)
	void					Subscribe(int nMessageID);
public:
							GModule(GEntity* pOwner=NULL);								///< ������
	virtual					~GModule();												///< �ı���
	void					Active(bool bActive = true);							///< (��)Ȱ��ȭ ��Ų��
	void					Update(float fDelta)	{ OnUpdate(fDelta); }			///< ������Ʈ
	GMessageResult			Message(GMessage& msg)	{ return OnMessage(msg); }			///< �޼��� ó��
	GModuleContainer*		GetContainer()			{ return m_pContainer;}			///< �����̳� ��ȯ
	GEntity*				GetOwner()				{ return m_pOwner; }	///< ������ ������Ʈ ��ȯ - ��Ȱ���Ҷ��� �̰� ����� ��
	virtual int				GetID() = 0;
	void					Initialize()			{ OnInitialized(); }
	void					Finalize()				{ OnFinalized(); }
	bool					IsActivate();
	MUID					GetOwnerUID();

protected:
	// �ʿ��� �κ��� ���� ��⿡�� ��ӹ޴´�.
	virtual void			OnAdd()					{}						///< �����̳ʿ� �߰��� �� ȣ��
	virtual void			OnRemove()				{}						///< �����̳ʿ��� ���ŵ� �� ȣ��
	virtual void			OnActivate()			{}						///< Ȱ��ȭ�� �� ȣ��
	virtual void			OnDeactivate()			{}						///< ��Ȱ��ȭ�� �� ȣ��
	virtual void			OnUpdate(float fDelta)	{}						///< �� ƽ���� ȣ��
	virtual void			OnInitialized()			{}						///< �ʱ�ȭ�� �� ȣ��
	virtual void			OnFinalized()			{}						///< �������� �� ȣ��
	virtual GMessageResult	OnMessage(GMessage& msg){ return MR_IGNORED; }	///< �޼��� �ڵ鷯
	virtual void			OnSubscribeMessage()	{}						///< ���� �޼����� �����Ѵ�.

#define SERVER_MODULE	L"MODULE"
	/// Ŭ���� �̸� ���
	virtual const wchar_t* GetClassName(void){ return SERVER_MODULE; }

};

/// ����� ���� �� �ִ� �����̳� Ŭ����
class GModuleContainer 
{
private:
	typedef map<int, GModule*>	GModuleMap;
	friend GModule;

	GModuleMap			m_Modules;
	vector<GModule*>	m_ActiveModules;

	std::set<int>	m_ModuleMessageTypes[MAX_MODULE_ID];
	void			SubscribeMessage(int nModuleID, int nMessageID);
public:
	GModuleContainer();
	virtual ~GModuleContainer();

	// bInitialActive : Ȱ��ȭ �ʱ�ġ
	void AddModule(GModule *pModule, bool bInitialActive = true);
	void RemoveModule(GModule *pModule);

	// ��ϵ� ����� nID ����� ã�´�
	GModule* GetModule(int nID);

	void ActiveModule(int nID, bool bActive = true);	// Ư�� ����� (��)Ȱ��ȭ ��Ų��
	bool IsActiveModule(int nID);

	void Message(GMessage& msg);
};

#define DECLARE_ID(_ID)		int GetID() { return _ID; }




#endif