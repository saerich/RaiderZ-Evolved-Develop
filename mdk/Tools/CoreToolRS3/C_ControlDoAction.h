/// @file C_ControlDoAction.h
#pragma once

#include <vector>
#include "M_ToolLogicObjectManager.h"
#include "M_ToolFunctor.h"

namespace rs3{ class RSceneNode; }
class CControlMediator;
class CToolCmdMgr;
class CGroupedCommandBuffer;
class CWorkSpaceManager;
class CListenableView;

//////////////////////////////////////////////////////////////////////////
//
// Popup Action
//
//////////////////////////////////////////////////////////////////////////

class CORETOOL_API CControlPopupActionRepository
{
public:
	CControlPopupActionRepository(){}
	virtual ~CControlPopupActionRepository()
	{
		for (std::vector<CCoreToolFunctor*>::iterator itr = m_vecResFunctor.begin(); itr != m_vecResFunctor.end(); ++itr)
		{
			delete *itr;
		}

		for (std::vector<CCoreToolFunctor*>::iterator itr = m_vecToolEtcFunctor.begin(); itr != m_vecToolEtcFunctor.end(); ++itr)
		{
			delete *itr;
		}
	}

	void AddManagedResNewFunctor(CCoreToolFunctor* _pFunctor)
	{
		m_vecResFunctor.push_back(_pFunctor);
	}

	void AddManagedToolEtcFunctor(CCoreToolFunctor* _pFunctor)
	{
		m_vecToolEtcFunctor.push_back(_pFunctor);
	}

private:
	friend class CWorkSpaceManager;

	std::vector< CCoreToolFunctor* > m_vecResFunctor;
	std::vector< CCoreToolFunctor* > m_vecToolEtcFunctor;
};

//////////////////////////////////////////////////////////////////////////
//
// DoAction
//
//////////////////////////////////////////////////////////////////////////

typedef void (*WBActionTable)(CControlMediator*); //�Լ� ������ ���̺�
/**
	@brief �� �׼� ���� Ŭ����(�Լ� ������ ���̺�)

	CControlMediator Ŭ������ ģ���Դϴ�. \n
	CControlMediator �� �Լ������� ���·� static �޼ҵ���� ����Ǿ� �ִٰ� �ڽ��� State�� �´� ���� �մϴ�.
*/

class CORETOOL_API CControlDoAction
{
public:

	enum OBJECT_EDIT_TYPE
	{
		EDIT_INIT = 0,
		OBJECT_SELECT,
		OBJECT_EDIT,
		OBJECT_COPY,
	};
	static const OBJECT_EDIT_TYPE GetObjectEditTypeAndPreAction(CControlMediator* _pController);
	static bool IsEditableSceneNode( rs3::RSceneNode* pSceneNode );
	static bool MakeRotationMatrixAndInfoText(rs3::RMatrix& _rOut, CControlMediator* _pController, UINT nAxis);

	/**
		@brief  CallBackAction ���
		@param  _wbState : ���� ��ũ��ġ ���� */
	static bool IsCallBackAction(WORKBENCH_STATE::STATE _wbState)
	{
		if( _wbState == WORKBENCH_STATE::OBJECT_MOVE ||
			_wbState == WORKBENCH_STATE::OBJECT_ROTATE ||
			_wbState == WORKBENCH_STATE::OBJECT_SELECT ||
			_wbState == WORKBENCH_STATE::OBJECT_SCALE ||
			_wbState == WORKBENCH_STATE::ETC_CAMERA) //�̷������� �߰��մϴ�.
			return true;
		else
			return false;
	}

	/**
		@brief  ������ ���°� ������Ʈ �������������� �˾Ƴ��� �Լ�
		@param  _wbState : ���� ��ũ��ġ ���� */
	static bool IsObjectEditState(WORKBENCH_STATE::STATE _wbState)
	{
		if( _wbState == WORKBENCH_STATE::OBJECT_MOVE ||
			_wbState == WORKBENCH_STATE::OBJECT_ROTATE ||
			_wbState == WORKBENCH_STATE::OBJECT_SELECT ||
			_wbState == WORKBENCH_STATE::OBJECT_SCALE ) //�̷������� �߰��մϴ�.
			return true;
		else
			return false;
	}

	/**
		@brief  �׼� ���̺� �ʱ�ȭ �� �׼� ���
		@param  _arrayTable : �׼� ���̺�(�Լ� ������ �迭) */
	static void InitActionTable(WBActionTable* _arrayTable)
	{
		//���
		_arrayTable[WORKBENCH_STATE::TERRAIN_HEIGHT] = CControlDoAction::TerrainHeight;
		_arrayTable[WORKBENCH_STATE::TERRAIN_AVER]   = CControlDoAction::TerrainAver;
		_arrayTable[WORKBENCH_STATE::TERRAIN_FLAT]   = CControlDoAction::TerrainFlat;
		_arrayTable[WORKBENCH_STATE::TERRAIN_SPLAT]  = CControlDoAction::TerrainSplat;
		_arrayTable[WORKBENCH_STATE::TERRAIN_RELAX]  = CControlDoAction::TerrainRelax;
		_arrayTable[WORKBENCH_STATE::TERRAIN_GRASS]  = CControlDoAction::TerrainGrass;
		//_arrayTable[WORKBENCH_STATE::TERRAIN_USERMATERIAL] = CControlDoAction::TerrainUserMaterial;

		_arrayTable[WORKBENCH_STATE::OBJECT_NEW]    = CControlDoAction::ObjectNew;
		_arrayTable[WORKBENCH_STATE::OBJECT_SELECT] = CControlDoAction::ObjectSelect;
		_arrayTable[WORKBENCH_STATE::OBJECT_MOVE]   = CControlDoAction::ObjectMove;
		_arrayTable[WORKBENCH_STATE::OBJECT_ROTATE] = CControlDoAction::ObjectRotate;
		_arrayTable[WORKBENCH_STATE::OBJECT_SCALE]  = CControlDoAction::ObjectScale;

		_arrayTable[WORKBENCH_STATE::ETC_PATH_SELECT] = CControlDoAction::EtcPathSelect;
		_arrayTable[WORKBENCH_STATE::ETC_CAMERA]      = CControlDoAction::EtcCamera;
		_arrayTable[WORKBENCH_STATE::ETC_MAP_VIEW]    = CControlDoAction::EtcMapView;
		_arrayTable[WORKBENCH_STATE::ETC_DEV]         = CControlDoAction::EtcDev;
		_arrayTable[WORKBENCH_STATE::ETC_DECAL]       = CControlDoAction::EtcDecal;
#ifdef _DEBUG
		for( int i = 0; i < WORKBENCH_STATE::STATE_NUM; ++i)
		{
			_ASSERT( _arrayTable[i] != NULL );
		}
#endif _DEBUG
	}
	
	static void TerrainHeight(CControlMediator* _pController);
	static void TerrainAver(CControlMediator* _pController);
	static void TerrainFlat(CControlMediator* _pController);
	static void TerrainSplat(CControlMediator* _pController);
	static void TerrainRelax(CControlMediator* _pController);
	static void TerrainGrass(CControlMediator* _pController);
	static void TerrainUserMaterial(CControlMediator* _pController);

	static void ObjectNew(CControlMediator* _pController);
	static void ObjectSelect(CControlMediator* _pController);
	static void ObjectMove(CControlMediator* _pController);
	static void ObjectRotate(CControlMediator* _pController);
	static void ObjectScale(CControlMediator* _pController);

	static void EtcPathSelect(CControlMediator* _pController);
	static void EtcCamera(CControlMediator* _pController);
	static void EtcMapView(CControlMediator* _pController);
	static void EtcDev(CControlMediator* _pController);
	static void EtcDecal(CControlMediator* _pController);

protected:

	/// ������ ������ ���ϴ� �������
	static rs3::RVector GetMoveVector(AXIS_STATE nAxisType, const rs3::RVector &vPosition, const rs3::RMatrix& localFrame
		, const rs3::RVector &p1, const rs3::RVector &q1, const rs3::RVector &q2);

	static void UpdateDragText(CControlMediator* _pController, const char* szDragText);


private: ///Ŭ���� �ν��Ͻ� ����
	CControlDoAction(void){}
	~CControlDoAction(void){}
};

//////////////////////////////////////////////////////////////////////////
//
// Object DoAction
//
//////////////////////////////////////////////////////////////////////////

//! DoAction - ObjectNew
class CORETOOL_API CDoActionObjectNew
{
public:
							CDoActionObjectNew(){}
	virtual					~CDoActionObjectNew(){}

	virtual bool			DoPreAction(CControlMediator* _pController) = 0;

	virtual CLogicObject*	OnGetReferenceObjectAndSetCursor(CControlMediator* _pController, CLogicObject* _pOldReference) = 0;
	virtual bool			OnCheckValidLocatePosition(CControlMediator* _pController, rs3::RVector& _currentCandidatePos) = 0;

	virtual void			OnEditSawedObject(CControlMediator* _pController, CLogicObject* _pSawedObject) = 0;
	virtual void			OnObjectCloned(CControlMediator* _pController, CLogicObject* _pSawedObject) {}

	/// new action �� ���õ� ������ ����Ǿ������ �뺸, pSender �� ������ ����� ��
	virtual void			OnReferenceChanged(CControlMediator* _pController, CListenableView* pSender = NULL ) {}
};

//! DoAction - ObjectSelect
class CORETOOL_API CDoActionObjectSelect
{
public:
							CDoActionObjectSelect(){}
	virtual					~CDoActionObjectSelect(){}

	virtual bool			DoPreAction(CControlMediator* _pController) = 0;

	virtual void			OnSelectChanged(CControlMediator* _pController, bool _IsEntity) = 0;
};

//! DoAction - ObjectMove
class CORETOOL_API CDoActionObjectMove
{
public:
							CDoActionObjectMove(){}
	virtual					~CDoActionObjectMove(){}

	virtual bool			DoPreAction(CControlMediator* _pController) = 0;

	virtual void			OnMove( CControlMediator* _pController, ENTITY_OBJECTLIST* _pList, const rs3::RVector3& _moveVec, CGroupedCommandBuffer* _pCmdBufferForActionEntered ) = 0;
};

//! DoAction - ObjectRotate
class CORETOOL_API CDoActionObjectRotate
{
public:
							CDoActionObjectRotate(){}
	virtual					~CDoActionObjectRotate(){}

	virtual bool			DoPreAction(CControlMediator* _pController) = 0;

	virtual void			OnRotate( CControlMediator* _pController, ENTITY_OBJECTLIST* _pList, rs3::RMatrix& _tmForRot, UINT _nAxis, CGroupedCommandBuffer* _pCmdBufferForActionEntered ) = 0;
};

//! DoAction - ObjectScale
class CORETOOL_API CDoActionObjectScale
{
public:
							CDoActionObjectScale(){}
	virtual					~CDoActionObjectScale(){}

	virtual bool			DoPreAction(CControlMediator* _pController) = 0;

	virtual void			OnScale( CControlMediator* _pController, ENTITY_OBJECTLIST* _pList, float _fScaleFactor, CGroupedCommandBuffer* _pCmdBufferForActionEntered ) = 0;
};

//////////////////////////////////////////////////////////////////////////
//
// Etc DoAction
//
//////////////////////////////////////////////////////////////////////////

//! DoAction - EtcPathSelect
class CORETOOL_API CDoActionEtcPathSelect
{
public:
							CDoActionEtcPathSelect(){}
	virtual					~CDoActionEtcPathSelect(){}

	virtual bool			DoPreAction(CControlMediator* _pController) = 0;
	virtual void			Act( CControlMediator* _pController ) = 0;
};
