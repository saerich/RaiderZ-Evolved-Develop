#pragma once

#include "RSceneNode.h"

#include <list>
#include <string>
#include <map>

#include "CoreTypes.h"

class CLogicObject;
class CEntityObject;
class CControlMediator;

class CORETOOL_API CControlMediatorHelperEntityAction
{
public:
	CControlMediatorHelperEntityAction(){}
	virtual ~CControlMediatorHelperEntityAction(){}

	virtual bool OnCheckAddSelection(CEntityObject* _pEntityObject, bool _bUpdateUI) = 0;

	virtual void OnRemoveSelection(CEntityObject* _pEntityObject, bool _bUpdateUI) = 0;
	virtual void OnRemoveSelectionAll(bool _bUpdateUI) = 0;

	virtual void OnDeleteSelectedObject(ENTITY_OBJECTLIST& _list, ENTITY_OBJECTLIST& _copyList) = 0;
	virtual void OnPasteCopiedObject(ENTITY_OBJECTLIST& _list, ENTITY_OBJECTLIST& _copyList) = 0;

	// TODO: �ᱹ logic object �� �Ϲ�ȭ �Ǿ�� ���� ������ 
	// CControlMediatorHelperEntityAction -> CControlMediatorHelperLogicObjectAction
	virtual void OnDeleteSelectedObject(LOGIC_OBJECTLIST& _list, LOGIC_OBJECTLIST& _copyList) {}

	virtual void OnAddToLogicObjectManager(CEntityObject* _pEntityObject, int& _nCnt, int& _nIdx) = 0;
	virtual void OnRemoveFromLogicObjectManager(CEntityObject* _pEntityObject, int& _nCnt, int& _nIdx) = 0;

	virtual void OnGetNavigationDirection(CEntityObject* _pEntityObject, const rs3::RVector& _vPos, rs3::RVector& _outDir) = 0;
	virtual void OnChagnedEnvWeather(const char* _pSzName) = 0;
	virtual void OnToolBarNewEntityClicked() = 0;
	virtual void OnToolBarEntityOnTerrainClicked() = 0;
	virtual void OnToolBarSelectionLocked(bool _bLock) = 0;
};

class CORETOOL_API CControlMediatorHelperAction
{
	friend class CControlMediator;
	friend class CToolLogicObjectManager;

	CControlMediatorHelperAction(void);
	~CControlMediatorHelperAction(void);

	void InitHelperAction(CControlMediator* _pMediator);
	void ResetHelperAction();

public:
	//////////////////////////////////////////////////////////////////////////
	//
	// ��ŷ����
	//
	//////////////////////////////////////////////////////////////////////////

	/** @brief ���콺�� ī�޶� �����ϴ� �������� �� ���� */
	bool GetMouseLine(LONG sx, LONG sy, rs3::RVector& origin, rs3::RVector& to) const;

	/** @brief ���콺�� ī�޶� �����ϴ� �������� �� ���� */
	void GetCurrentMouseLine( const rs3::RVector** ppOrigin, const rs3::RVector** ppTo) const
	{
		if(m_bValidMouseLine_)
		{
			*ppOrigin = &m_vecMouseLineOrg_;
			*ppTo = &m_vecMouseLineTo_;
		}
		else
		{
			*ppOrigin = NULL;
			*ppTo = NULL;
		}
	}

	/** @brief ���콺 ���ΰ� ���� �浹�� ����
		���콺�� �����϶� ���� �Ҹ��� �Լ�
		�ϴ� ���콺 ������ ���ϰ� ���� �浹���� �������߸� ���� �����Ѵ�.
	*/
	bool SetMouseLineAndPickingPosition();
	void PickingCameraPlane( rs3::RVector& _vTerrainContactPos, rs3::RVector& _vMapPickPos, bool& _bRet );

	/** @brief ���콺 ���ʹ�ư ������ */
	void OnLButtonDown();

	/** @brief ���콺 ���ʹ�ư �������� */
	void OnLButtonUp();

	/** @brief ���콺 ��ǥ�� ������ �浹�� ��� */
	inline const rs3::RVector* GetTerrainContactPos() const
	{
		return m_pVecTerrainContactPos_;
	}
	/** @brief ���콺 ��ǥ�� �浹�� ��� */
	inline const rs3::RVector* GetCurrentPickingPosition() const
	{
		return m_pVecCurrentPickingPosition_;
	}

	/** @brief Action Plane�� ������ Action Plane���� �浹�� ����
		Mouse Command Entry �϶��� �ҷ�����.
		MouseCmdEntry �� Ȯ���ؼ� ���̶��, �� �Լ��� ����Ǿ��ٰ� �� �� �ִ�.
		Action Plane�̶�, �׼� ��Ʈ�ѿ��� �׼��� �����Ҷ� �ʿ��� ����̴�.
	*/
	void					SetActionPlaneAndContactPos();
	const rs3::RPlane&		GetActionPlane() const { return m_actionPlane_; }
	void					SetActionPlanePosAs( const rs3::RVector& vecPosition );
	const rs3::RVector&		GetActionPlanePos() const { return m_vecContactActionPlanePos_; }

	/// ���õ� ������Ʈ�� ������Ʈ
	void					UpdateSelectedObjects();

	//////////////////////////////////////////////////////////////////////////
	//
	// Cursor / Navigation / Pause
	//
	//////////////////////////////////////////////////////////////////////////
	
	void					SetCursorToScreenCenter();						///< Cusror�� �߾����� �ű��. FPS��忡�� �ʿ�
	void					GetMovedCursorPosFromCenter(int& _x, int& _y);	///< �߾����κ��� ������ Ŀ�� ��ġ
	void					SetNavigationModeToggle(bool _bXYNavi = false);	///< Navigation Mode Toggle
	inline bool				IsPause(){ return m_bPause_; }					///< Pause ����
	void					SetPauseToggle();								///< Pause Toggle


	//////////////////////////////////////////////////////////////////////////
	//
	// ��ü ���ð� ���õ� �׼�
	//
	//////////////////////////////////////////////////////////////////////////

	void					AddSelectection( CLogicObject* _pLogicObject, bool _bUpdateUI);
	void					RemoveSelection( CLogicObject* _pLogicObject, bool _bUpdateUI);
	void					RemoveAllSelection(bool _bUpdateUI);

	const SCENEOBJECTMAP*	GetSelectedSceneObject()	{ return &m_listSelectedSceneObject_; }
	SCENEOBJECTMAP*			GetSelectedSceneObjectPtr()	{ return &m_listSelectedSceneObject_; }
	ENTITY_OBJECTLIST*		GetSelectedEntityObject()	{ return &m_listSelectedEntityObject_; }
	LOGIC_OBJECTLIST*		GetSelectedLogicObject()	{ return &m_listSelectedLogicObject_; }
	LOGIC_OBJECTLIST*		GetSelectedLogicObjectToCopy()	{ return &m_listSelectedLogicObjectToCopy_; }
	void					GetWBObjectAtCurrentMouse(rs3::RSceneNode** _ppSceneNode, CLogicObject** _ppLogicObject) const;

	void					SetSelectedObjectCenterAndActionFrame();
	const rs3::RMatrix&		GetSelectedObjectActionFrame(){ return m_matSelectedObjectActionFrame_; }
	rs3::RVector*			GetSelectNodeCenter();		///< ���õ� ������ center ( scenenode / logic object ���� )
	rs3::RVector*			GetSelectSceneNodeCenter();
	bool					IsInSelectObjectList( const CLogicObject* _pLogicObject);
	bool					GetSelectedObjectAABBCenter( rs3::RVector& _refOut, rs3::RBoundingBox* _pGetAABB, bool _bPickedPriority = false, const rs3::RVector* _p = NULL, const rs3::RVector* _q = NULL );

	void					SetSelectedLogicObjectCntForDoAction(int nCnt){ m_nSelectedLogicObjCntForDoAction = nCnt; }
	int						GetSelectedLogicObjectCntForDoAction(){ return m_nSelectedLogicObjCntForDoAction; }

	//////////////////////////////////////////////////////////////////////////
	//
	// ��ü Hide / Freeze
	//
	//////////////////////////////////////////////////////////////////////////
	void					SetVisible( CLogicObject* pLogicObject, bool bVisible );
	void					SetEditable( CLogicObject* pLogicObject, bool bEditable );

	//////////////////////////////////////////////////////////////////////////
	//
	// ��ü ���� / ���� / �ٿ��ֱ�
	//
	//////////////////////////////////////////////////////////////////////////

	void					DeleteSelectedObject();
	void					CopyShallowSelectedEntityObject();
	void					PasteCopiedEntityObject();
//	bool					CopyDeepSelectedZoneObject();
	bool					CopyDeepSelectedObject();


	//////////////////////////////////////////////////////////////////////////
	//
	// WorkBench Mouse Action Observer
	//
	//////////////////////////////////////////////////////////////////////////

	enum					WB_MOUSE_ACTION { WB_MOUSE_L_DOWN = 0, WB_MOUSE_L_UP, WB_MOUSE_ACTION_NUM };
	void					AddToWBMouseObserver( WB_MOUSE_ACTION _eAction, int _nId ) { m_vViewIDToObserveMouseAction_[_eAction].push_back( _nId ); }
	void					NotifyWBMouseAction( WB_MOUSE_ACTION _eAction );


	//////////////////////////////////////////////////////////////////////////
	//
	// Entity Action �˸���
	//
	//////////////////////////////////////////////////////////////////////////

	void					NotifyNewEntityToolBarButtonClicked();
	void					NotifyEntityOnTerrainToolBarButtonClicked();
	void					NotifySelectionLockToolBarButtonClicked(bool _bLock);
	
private:
	CControlMediator*		m_pControlMediator_;
	bool					m_bPause_;

	//select node ���õ� Object List
	//for SceneNode
	SCENEOBJECTMAP			m_listSelectedSceneObject_;
	rs3::RVector			m_vSelectNodeSceneCenter_;

	// for EntityObject, LogicObject
	// TODO : ��ƼƼ ������Ʈ�� ���� ������Ʈ�� ���������Ѵ�.
	ENTITY_OBJECTLIST		m_listSelectedEntityObject_;
	ENTITY_OBJECTLIST		m_listSelectedEntityToCopy_;
	rs3::RVector			m_vSelectNodeCenter_;
	rs3::RMatrix			m_matSelectedObjectActionFrame_;
	LOGIC_OBJECTLIST		m_listSelectedLogicObject_;
	LOGIC_OBJECTLIST		m_listSelectedLogicObjectToCopy_;

	// mouse up observer
	std::vector< int >		m_vViewIDToObserveMouseAction_[ WB_MOUSE_ACTION_NUM ];

	rs3::RVector*			m_pVecTerrainContactPos_;
	rs3::RVector*			m_pVecCurrentPickingPosition_;

	rs3::RVector			m_vecContactActionPlanePos_;
	rs3::RPlane				m_actionPlane_;

	//Current MouseRay
	rs3::RVector			m_vecMouseLineOrg_;
	rs3::RVector			m_vecMouseLineTo_;
	bool					m_bValidMouseLine_;

	UINT					m_nOldAxis;	/// gizmo ���� �� ������ ������ ������ �� ����
	int						m_nSelectedLogicObjCntForDoAction;
};
//----------------------------------------------------------------------------------------------------
inline rs3::RVector* CControlMediatorHelperAction::GetSelectSceneNodeCenter()
{
	if(m_listSelectedSceneObject_.empty())
		return NULL;
	else
		return &m_vSelectNodeSceneCenter_;
}
//----------------------------------------------------------------------------------------------------