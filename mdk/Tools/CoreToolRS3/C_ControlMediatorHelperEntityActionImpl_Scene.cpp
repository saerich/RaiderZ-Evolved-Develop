#include "StdAfx.h"
#include "C_ControlMediatorHelperEntityActionImpl_Scene.h"
#include "M_LogicObjectCommandBuffer.h"
#include "M_ToolCmdMgr.h"
#include "M_ToolLogicObjectManager.h"
#include "VL_SceneNodeList.h"

void CControlMediatorHelperEntityActionImpl_Scene::OnDeleteSelectedObject(LOGIC_OBJECTLIST& _list, LOGIC_OBJECTLIST& _copyList)
{
// 	CGroupedCommandBuffer* pGroupCmdBuffer = new CGroupedCommandBuffer;
// 
// 	for(LOGIC_OBJECTLIST::iterator itr = _list.begin(); itr != _list.end(); ++itr)
// 	{
// 		CLogicObject* pObject = *itr;
// 
// 		m_pControlMediator_->GetToolLogicObjectMgr()->RemoveFromManager(pObject);
// 
// 		//������ ����� copyList�� ������ copyList ���� ����
// 		std::list<CLogicObject*>::iterator itr_finder = std::find( _copyList.begin(), _copyList.end(), pObject);
// 		if(itr_finder != _copyList.end())
// 			_copyList.clear();
// 
// 		CLogicObject_SceneNodeDeleteCmdBuffer* pCmdBuffer = new CLogicObject_SceneNodeDeleteCmdBuffer(pObject);
// 		pGroupCmdBuffer->AddBuffer(pCmdBuffer);
// 	}
// 
// 	CToolCmdMgr* pCmdMgr = m_pControlMediator_->GetToolCmdMgr();
// 	pCmdMgr->RecordCommand(pGroupCmdBuffer);
// 	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);//�ϴ� UI�� Update ���� �ʴ´�.
// 
// 	m_pControlMediator_->GetViewPtr<CDlgSceneNodeList>()->UpdateTreeControl();
}
