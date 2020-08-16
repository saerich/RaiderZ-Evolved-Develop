/// @file C_ControlMediator.h
#pragma once

#include <string>
#include <vector>

#include "VL_ListenableView.h"

#include "M_ToolSceneManager.h"
#include "M_ToolCamera.h"
#include "ext_lib/FileWatcher.h"

#include "RDebuggingInfo.h"

//Tool Const
namespace Tool3Const
{
	/** @brief Keyboard factor */
	const float CONST_KEY_MOVE = 0.001f;

	/** @brief Mouse factor */
	const float CONST_MOUSE_RATATE_SAFE_Z = 0.99f;
	const float CONST_MOUSE_PANNING = 0.000017f;
	const float CONST_MOUSE_ROTATE = 0.0001f;

	/** @brief Workbench factor */
	const int   CONST_WB_STATIC_OBJ_TO_CENTER = 2000;
	const int   CONST_WB_ENTITY_OBJ_TO_CENTER = 3000;
	const float CONST_WB_NAVI_HEIGHT = 170.f;
};

//manager class or helper class
class CToolCmdMgr;
class CToolLogicObjectManager;
class CWorkSpaceManager;
class CRubyTestSet;

// Mediator Helper
class CControlMediatorHelperAction;
class CControlMediatorHelperRender;
class CControlMediatorHelperResource;

/**
	@brief ��Ʈ�� �߰���

	����� ��Ʈ���� �߰��ϴ� ��Ʈ�� �߰����Դϴ�. \n
	�������� ��Ʈ�ѷ����� Mediator ������ �߰������� ������ ���մϴ�.
*/
class CORETOOL_API CControlMediator
{

friend class CControlMediatorHelperAction;
friend class CControlMediatorHelperRender;
friend class CControlMediatorHelperResource;

friend class CWorkSpaceManager;
friend class CRubyTestSet;

public:
	enum EDITMODE
	{
		EDITMODE_NONE = 0,
		EDITMODE_ZONE = 1,
		EDITMODE_FIELD = 2,
	};

public:
	CControlMediator();
	~CControlMediator();


	//////////////////////////////////////////////////////////////////////////
	//
	// �� ����
	//
	//////////////////////////////////////////////////////////////////////////

	/** @brief Message Broad Casting */
	inline void BroadCastMessage(VIEW_MESSAGE::ID _message)
	{
		for (std::vector<CListenableView*>::iterator itr = m_vecListenableViews_.begin(); itr != m_vecListenableViews_.end(); ++itr)
			(*itr)->Listen( CoreMessage(_message) );
	}

	/** @brief Message Broad Casting for CoreMessage object*/
	inline void BroadCastMessage(CoreMessage& _message)
	{
		for (std::vector<CListenableView*>::iterator itr = m_vecListenableViews_.begin(); itr != m_vecListenableViews_.end(); ++itr)
			(*itr)->Listen( _message );
	}

	/** @brief Message Casting for specific view */
	inline void CastMessageToView(int nViewID, VIEW_MESSAGE::ID _message)
	{
		m_vecListenableViews_[nViewID]->Listen( CoreMessage(_message) );
	}

	/** @brief View Getter */
	template <typename T>
	inline T* GetViewPtr()
	{
		return static_cast<T*>(GetViewPtr(T::GetID()));
	}

	/** @brief View Getter using ID */
	CListenableView* GetViewPtr(int _nViewId)
	{
		if (_nViewId < 0)
			return NULL;

		CListenableView* pView = m_vecListenableViews_[_nViewId];
		_ASSERT( pView->GetViewID() == _nViewId );
		return pView;
	}

	/** @brief View Adder */
	inline void AddView(CListenableView* _pView)
	{
		m_vecListenableViews_.push_back(_pView);
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// ��ũ�����̽� ����
	//
	//////////////////////////////////////////////////////////////////////////

	MAP_EDIT_TYPE	LoadWorkSpaceFile(const char* _pStrName, const std::string& _rWsKeyword);	///< Load WorkSpace File
	std::string		SaveWorkSpaceFile(const char* _pSzFileName, bool bSaveAs);					///< Save WorkSpace File
	void			CreateWorkSpaceFile(const char* _pSzWorkSpaceName, MAP_EDIT_TYPE _mapType);	///< Create WorkSpace File

	/** @brief WorkSpace Getter */
	template<typename T>
	inline T* GetWorkSpacePtr()
	{
		return static_cast<T*>( m_pWorkSpaceMgr_->GetWorkSpace(T::WS_KEYWORD) );
	}

	/** @brief Current WorkSpace Getter */
	template<typename T>
	inline T* GetCurrentWorkSpaceAs()
	{
		return dynamic_cast<T*>( m_pWorkSpaceMgr_->GetCurrentWorkSpace() );
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// �׼ǰ���
	//
	//////////////////////////////////////////////////////////////////////////

	inline CToolCmdMgr*						GetToolCmdMgr()			{ return m_pCmdMgr_; }				///< Ŀ�ǵ� �Ŵ���
	inline CToolLogicObjectManager*			GetToolLogicObjectMgr()	{ return m_pLogicObjectMgr_; }		///< ���� ������Ʈ �Ŵ���
	inline CWorkSpaceManager*				GetWorkSpaceMgr()		{ return m_pWorkSpaceMgr_; }		///< WorkSpace Manager

	inline CControlMediatorHelperAction*	GetHelperAction()		{ return m_pHelperAction_; }		///< Action Helper
	inline CControlMediatorHelperRender*	GetHelperRender()		{ return m_pHelperRender_;}			///< Render Helper
	inline CControlMediatorHelperResource*	GetHelperResource()		{ return m_pHelperResource_; }		///< Resource Helper


	//////////////////////////////////////////////////////////////////////////
	//
	// ��ũ��ġ State / Mode / Handle / Timer
	//
	//////////////////////////////////////////////////////////////////////////

	/** @brief Workbench Size Getter */
	inline void GetWBSize(LONG& _refWidth, LONG& _refHeight) const
	{
		_refWidth = (LONG)m_wbWidth_;
		_refHeight = (LONG)m_wbHeight_;
	}

	/** @brief Current Workbench State Setter */
	void SetCurrentWBState(WORKBENCH_STATE::STATE _state);

	inline WORKBENCH_STATE::STATE		GetCurrentWBState()	const { return m_wbCurrentState_; }		///< Current Workbench State Getter
	inline WORKBENCH_STATE::STATE		GetPreviousWBState()const { return m_wbPreviousState_; }	///< Previous Workbench State Getter

	inline HWND							GetWBWindowHandle()	const { return m_wbhWnd_; }				///< Workbench Window Handle
	inline DWORD						GetWBTimerTic()		const { return ELAPSE_IN_TIMER; }		///< Workbench Processing Tic

	inline EDITMODE						GetEditMode() const		  { return m_nEditMode; }


	//////////////////////////////////////////////////////////////////////////
	//
	// ��ũ��ġ �׼�
	//
	//////////////////////////////////////////////////////////////////////////

	void								WB_CallBackAction();										///< WorkBench �ݹ�׼�(ī�޶� �׼� ��)
	void								WB_OnTimeAction();											///< ��ũ��ġ ���¿� �����ϴ� �׼� �޼ҵ�


	//////////////////////////////////////////////////////////////////////////
	//
	// ��ġ�� ���õ� �޼ҵ� - ����, ������, ��������
	//
	//////////////////////////////////////////////////////////////////////////

	/**
		@brief  ��ġ�� �������Ŀ� ��鿡�� �˷��ִ� �޼ҵ�
		@param  nWidth  : ������ ��ġ�� ����ũ��
		@param  nHeight : ������ ��ġ�� ����ũ��
		@param  _hWnd   : ������ ��ġ�� ���� ������ �ڵ�
		@param  _workTime : ��ġ�� �ҷ����� �ð�
		@return bool : ��������
	*/
	bool								CreateWBDeviceAndNotify(UINT nWidth, UINT nHeight, HWND _hWnd, UINT _workTime);
	void								DestroyDevice();

	void								RunRS3();													///< �������̽� ������
	inline bool							IsWBNavigation() const { return m_bNavigation_; }			///< (F2�� �̿���) Navigation ������ ����

	void								ResizeWBDevice(UINT _w, UINT _h);							///< ������ ȭ�� �缳��
	inline bool							IsWBDeviceInited() const { return m_bDeviceInited_; }		///< ��ũ��ġ DX Device�� �����Ǿ����� ����


	//////////////////////////////////////////////////////////////////////////
	//
	// RealSpace ���� �ڿ�
	//
	//////////////////////////////////////////////////////////////////////////

	inline rs3::RDeviceD3D*				GetCurrentDevice()			{ return m_pCurrentDevice_; }		///< ��ġ
	inline rs3::REnvironmentSet*		GetCurrentEnvironmentSet()	{ return m_pEnvironmentSet_; }		///< ȯ��

	//Tool ������� null �̸� �ȵȴ�.
	inline rs3::RToolSceneManager*		GetToolSceneManager()		{ return m_pCurrentSceneManager_;}	///< �� �޴���
	inline rs3::RToolCamera*			GetToolCamera()				{ return m_pCurrentCamera_; }		///< ī�޶�

	void								ResetResource();		///< ���ҽ� ����
	void								DestoryResource();		///< ���ҽ� ����


	//////////////////////////////////////////////////////////////////////////
	//
	// ��/�� ����
	//
	//////////////////////////////////////////////////////////////////////////

	MAP_EDIT_TYPE		LoadZone(const char* _pStrName, MAP_EDIT_TYPE _wantedMapType, bool _bWithRelatedConfig);	///< Load Scene
	void				SaveZone(const char* _filename, bool _bSort, bool _bSaveAs);	///< Save Scene
	void				CreateZone();													///< Create Scene

	/** @brief �� ���� ���� */
	struct MAP_INFO {
		MAP_INFO():
			m_bLoaded(false), m_bSceneEditableMap(false), m_bEditSceneObject(false),
			m_width(0.f), m_height(0.f), m_orgX(0.f), m_orgY(0.f)
		{}

		//////////////////////////////////////////////////////////////////////////
		//
		// �⺻����
		//
		bool				m_bLoaded;				///< �� �ε� ����
		bool				m_bSceneEditableMap;	///< �ε��� ���� ���� ���ɿ���
		bool				m_bEditSceneObject;		///< ���� ���� �����Ҽ� �ִ��� ����. (Zone���� �ε��Ǿ� �־ Entity�� �����Ҽ� �� ������ ����)

		MAP_EDIT_TYPE		m_eMapEditType;			///< �ε��� �� Ÿ��
		std::string			m_strScenePath;			///< ���� �ִ� �۾� ���丮
		std::string			m_strSceneFile;			///< �� ���� �̸�


		//////////////////////////////////////////////////////////////////////////
		//
		// �ƿ����� ����
		//
		float				m_width;				///< �� ���� ũ��
		float				m_height;				///< �� ���� ũ��

		float				m_orgX;
		float				m_orgY;

		int					m_rowFace;				///< �� ���� �� ��
		int					m_colFace;				///< �� ���� �� ��
		std::string			m_strTerrainBaseTile;	///< ���� �⺻ Ÿ��


		//////////////////////////////////////////////////////////////////////////
		//
		// �ε��� ����
		//
		std::string			m_strImportSceneFile;
		rs3::RBoundingBox	m_aabb;

	} m_mapInfo;

	//////////////////////////////////////////////////////////////////////////
	//
	// ī�޶� �G ���콺
	//
	//////////////////////////////////////////////////////////////////////////

	/** @brief ī�޶� ������ ���� */
	struct CAMERA_INFO {

		CAMERA_INFO():m_bCameraMode(true), m_bKeyW(false),
			m_bKeyA(false), m_bKeyS(false), m_bKeyD(false), FAR_Z(200000.0f)
		{}

		bool	m_bCameraMode;					///< Alt, Space Ű� ���� ī�޶� ��� ����
		bool	m_bKeyW;						///< W Ű�� ���� ī�޶� �׺���̼� ����
		bool	m_bKeyA;						///< A Ű�� ���� ī�޶� �׺���̼� ����
		bool	m_bKeyS;						///< S Ű�� ���� ī�޶� �׺���̼� ����
		bool	m_bKeyD;						///< D Ű�� ���� ī�޶� �׺���̼� ����
		float	ROTATE_STEP;					///< ȸ���ӵ�
		float	MOVE_STEP;						///< �д׼ӵ�
		float	KEY_STEP;						///< Ű���� �ӵ�
		float	WHEEL_ZOOM_STEP;				///< ���콺 �� �ӵ�
		float	FAR_Z;							///< ī�޶� far_z

	} m_cameraInfo;

	/** @brief ���콺 ���� ���� */
	struct MOUSE_INFO {

		MOUSE_INFO(){}
		LONG m_orgMouseX, m_orgMouseY;			///< �巡�׻����϶� ó�� ���콺 ��ǥ
		LONG m_currentMouseX, m_currentMouseY;	///< ���� ���콺 ��ǥ
		LONG m_movedMouseX  , m_movedMouseY;	///< ������ ���콺 ��ǥ
		LONG m_nWheelDelta;						///< ���콺 �� ��
		bool m_bPopAble;
		bool m_bCaptured;
		bool m_bIgnoreMouseMovedPickingRActor;

		void setIgnoreMouseMovedPickingRActor( bool _bSet ) { m_bIgnoreMouseMovedPickingRActor = _bSet; }
		bool getIgnoreMouseMovedPickingRActor()				{ return m_bIgnoreMouseMovedPickingRActor; }

		/** @ brief ���콺 �Ӽ� ��Ʈ �÷��� */
		struct {
			bool bDrag;
			bool bMDrag;
			bool bLButton;
			bool bRButton;
			bool bMButton;
			bool bEvented;
		} mouseState;

	} m_mouseInfo;

	float m_fClipNearZ;
	float m_fClipFarZ;

	//////////////////////////////////////////////////////////////////////////
	//
	// ����ó��
	//
	//////////////////////////////////////////////////////////////////////////
	static string& GetErrorString();
	static void ClearErrorString();
	static void AddErrorString( const char* );

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� �뺸 ���� ��
	void		Report(const FileWatcher::StringVec& changedFiles);	///< ���� ���� �뺸

private:

	//View Container
	std::vector<CListenableView*>		m_vecListenableViews_;

	//�ڿ�
	rs3::RToolCamera*					m_pCurrentCamera_;
	rs3::RDeviceD3D*					m_pCurrentDevice_;
	rs3::RToolSceneManager*				m_pCurrentSceneManager_;
	rs3::REnvironmentSet*				m_pEnvironmentSet_;

	//tool manager / physx
	CToolCmdMgr*						m_pCmdMgr_;
	CWorkSpaceManager*					m_pWorkSpaceMgr_;
	CToolLogicObjectManager*			m_pLogicObjectMgr_;

	// helper
	CControlMediatorHelperResource*		m_pHelperResource_;
	CControlMediatorHelperAction*		m_pHelperAction_;
	CControlMediatorHelperRender*		m_pHelperRender_;
	FileWatcher*						m_pFileWatcher;		///< ���� ���� ������

	//��ũ��ġ ����
	UINT								m_wbWidth_;
	UINT								m_wbHeight_;
	HWND								m_wbhWnd_;
	WORKBENCH_STATE::STATE				m_wbCurrentState_;
	WORKBENCH_STATE::STATE				m_wbPreviousState_;

	bool								m_bNavigation_;
	bool								m_bDeviceInited_;
	EDITMODE							m_nEditMode;

	// WBActionTable
	void								(*WB_ActionTable_[WORKBENCH_STATE::STATE_NUM])(CControlMediator*);

	// timer : 6, 0.006��, �⺻������ 16�� ElapseTime
	DWORD								ELAPSE_IN_TIMER;

	//���ҽ� ���� �޽��
	bool								LoadSceneAndEnvFromXML_(const char* _pStrZoneName, MAP_EDIT_TYPE _eMapType);

	/// ������ ������Ʈ��
	static string		m_errorString;

	rs3::RDebuggingInfo	m_DebuggingInfo;
};
