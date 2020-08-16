#pragma once

#include "VL_ListenableView.h"
#include "M_EntityObject.h"
#include "afxwin.h"

#include "resource.h"

#include "CoreEffectInfo.h"

//for foo button
#include "ext_lib/FooButton.h"

#include "V_EditCtrl.h"
#include "ext_lib/WndResizer.h"

// CEditEntityObjectList ��ȭ �����Դϴ�.

class CStyleListBox : public CListBox
{
	virtual void DrawItem(LPDRAWITEMSTRUCT);
};

/** @brief Entity Object List Rollup */
class CORETOOL_SOULHUNT_API CRollupEntityObjectList : public CDialog, public CListenableViewImpl_Rollup<CRollupEntityObjectList>
{
	DECLARE_DYNAMIC(CRollupEntityObjectList)

public:
						CRollupEntityObjectList(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual				~CRollupEntityObjectList();						///< �Ҹ���

	void				ResetObjectData();
	void				ResetObjectCheckBox();

	// Listenable View
	virtual void		Listen(CoreMessage& _message);				///< Listen

	void UpdateSectorHeight();
	virtual const char*	GetViewName(){ return "Entity Object List"; }	///< View Name

	void				ResetEntityScript(int _fieldID);
	CEntityObject*		GetCurSelBaseEntityObject();
	CoreEffectInfoMgr*	GetEffectInfoMgr() { return &m_EffectInfoMgr; }

	void				RemoveTrackingSpawnIf( CEntityObject* _pEntityObject );
	void				ChangeTrackingSpawn( CEntitySpawn* _pEntitySpawn ){ m_pTrackingSpawnObject_ = _pEntitySpawn; }

	bool				SetSelectEntityObject(CEntityObject* _pEntityObject, BOOL _bFlag);
	void				SetSelectNone();
	void				SetListLock(bool _bLock);
	inline void			WriteTreeViewMessage(SAttributeAtXML* _pEntityAttribute);

	afx_msg void		OnLbnSelchangeEntityList();
	void				UpdateListBox(bool _bLockAndUnlock, bool _bKeepPreSelected); //Update List Box
	void				SetListBoxUpdateLock( bool _bLock){ if( _bLock ) m_ctlList_.LockWindowUpdate(); else m_ctlList_.UnlockWindowUpdate(); }

	// view handler / object
	bool				IsRenderingOnlySelected()	{ return m_checkViewOnlySelected.GetCheck() == TRUE; }
	bool				IsHideSpawnHandler()		{ return m_checkHideSpawnHandler.GetCheck() == TRUE; }
	bool				IsViewMapObjHandler()		{ return m_checkViewMapObjHandler.GetCheck() == TRUE; }
	void				SetVisibleFieldNPC(bool bRenderingOnlySelected);

	void				SetEditingModeReadOnly(bool _bReadOnly, bool _bShowSpawn);

	void				SetRenderSpawnEntity(bool _bShowSpawn);
	bool				IsRenderSpawnEntity(){ return (m_bVisibleSpawn_ == TRUE); }

	// area
	HTREEITEM			SetAreaCylinderElement(CEntityArea* _pArea);
	void				UpdateAreaCylinderUI(SAttributeAtXML* _pCylinderAttribute);

	// custom object
	bool				IsCustomObjectNewMode();
	void				SetToCustomObjectNewMode();


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_ENTITY_OBJ_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnUpdateCmdUI(CCmdUI* pCmdUI); ///< ������ ��Ʈ���� Ȱ��ȭ �ϱ����� �Լ�
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

	afx_msg void OnBnClickedCheckInvisibleSpawn();
	afx_msg void OnBnClickedCheckInvisibleMarker();
	afx_msg void OnBnClickedCheckInvisibleSensor();
	afx_msg void OnBnClickedCheckInvisibleSound();
	afx_msg void OnBnClickedCheckInvisibleArea();

	afx_msg void OnBnClickedCheckViewOnlySelected();


	afx_msg void OnLbnDblclkEntityEditList();
	afx_msg void OnNMDblclkTreeEntityListPreview(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

	// Edit Control
	void				OnSearchObjectList();


private:
	CStyleListBox	m_ctlList_;
	CTreeCtrl		m_ctlTreePreview_;

	//����Ʈ ��� ���
	CStatic	m_editModeLabel_;
	CFont	m_contentFont_;

	BOOL m_bVisibleSpawn_;
	BOOL m_bVisibleMarker_;
	BOOL m_bVisibleSensor_;
	BOOL m_bVisibleSound_;
	BOOL m_bVisibleArea_;
	BOOL m_bVisibleRoom_;

	BOOL m_bPreVisibleSpawn_;
	BOOL m_bPreViewOnlySelected;

	BOOL m_bVisibleSpawnOld_;
	BOOL m_bVisibleMarkerOld_;
	BOOL m_bVisibleSensorOld_;
	BOOL m_bVisibleSoundOld_;
	BOOL m_bVisibleAreaOld_;
	BOOL m_bVisibleRoomOld_;

	BOOL m_bHideSpawnHandlerOld_;
	BOOL m_bViewMapObjHandlerOld_;

	bool m_bEnableCommonEntity_;
	bool m_bEnableReadOnly_;

	CButton m_checkViewOnlySelected;
	CButton m_checkHideSpawnHandler;
	CButton m_checkViewMapObjHandler;

	void SetEnableEntityUI_(bool bEditField,bool bEnableCommonEntity);
	void UpdateVisible_();
	void UpdateSpawnVisible(bool bForce = false);
	bool LoadBaseEntityObject_();
	void ResetBaseEntityObject_();
	void NoneSelectNewEntityObject_(CEntityObject::TYPE _exceptionType = CEntityObject::ENTITY_OBJ_NUM);

	void EditEntityAttributeValue_(
		SElementAtXML::AttributeList::iterator _itrAttrBegin,
		SElementAtXML::AttributeList::iterator _itrAttrEnd,
		HTREEITEM& _refCurrentTreeItem, bool _bMulti);

	CEntityObject*	m_arrayBaseEntityObject_[CEntityObject::ENTITY_OBJ_NUM];
	CEntityObject*	m_pSensorWarpCustom_;
	CEntityArea*	m_pEntityAreaCustom_;

	CEntityObject* m_pTrackingSpawnObject_;			// �������� ���� ������Ʈ ��ƼƼ

	FooButton m_btnEntityObjectNew_[CEntityObject::ENTITY_OBJ_NUM];
	FooButton* m_pGroupTopBarBtn_;
	std::string m_strScriptFilePath_;

	CEntityObject::EntityAttributeMap m_mapTreeViewAssociatedAttributes_;

	CCoreToolEdit m_editSearchList;

	CWndResizer m_resizer;


	// add element ����
	void OnAddSpawnNpcElement_();
	void OnAddSensorWarpElement_();
	void OnAddAreaCylinderElement_();
	void OnAddRoomCylinderElement_();

	// custom float ����
	void OnCustomFloatEditArea_(CEntityObject* _pArea, SAttributeAtXML* _pEditedAttribute);

	//Tree - Map ���� �Լ�
	inline bool UpdateSelection_(CEntityObject* _pEntityObject);
	inline void ConfirmSelection_(bool _bSameType);
	inline void ClearTreeMap_();

	// VRCutSceneObjectList �� �ִ��� �̻��
	CoreEffectInfoMgr			m_EffectInfoMgr;
	
	// ���� �����/��׷� ��� ������
	float						m_fSectorHeight;

	// ���õ� npc�� ���̴� �ɼ� Ȱ��ȭ�� �۵��Ǵ� ������Ʈ
	void UpdateVisibleSelectedFieldNPC();

	void ParseFindString(string strParse, vector<string>& outObjectList);
	bool FindObjectList(vector<string>& vecFindList,string strObjectName);

	void RegisterResizer();

public:
	afx_msg void OnBnClickedButtonNewSpawn();
	afx_msg void OnBnClickedButtonNewMarker();
	afx_msg void OnBnClickedButtonNewSensor();
	afx_msg void OnBnClickedButtonElementAdd();
	afx_msg void OnBnClickedButtonElementDel();
	afx_msg void OnBnClickedButtonScriptEdit();

	afx_msg void OnTvnSelchangingTreeEntityListPreview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNewSpawnMapObj();
	afx_msg void OnBnClickedButtonNewSound();
	afx_msg void OnNMRClickTreeEntityListPreview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNewSpawnGather();
	afx_msg void OnBnClickedButtonNewArea();
	afx_msg void OnBnClickedButtonNewRoom();
	afx_msg void OnBnClickedCheckInvisibleRoom();
	afx_msg void OnEnChangeEditSectorHeight();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonNpcReload();
};

inline bool CRollupEntityObjectList::UpdateSelection_(CEntityObject* _pEntityObject)
{
	return CEntityObject::UpdateSelectionInfo(_pEntityObject, m_ctlTreePreview_, m_mapTreeViewAssociatedAttributes_);
}

inline void CRollupEntityObjectList::ConfirmSelection_(bool _bSameType)
{
	if(_bSameType == false)
	{
		m_ctlTreePreview_.DeleteAllItems();
		m_ctlTreePreview_.InsertItem("���δٸ� ��ƼƼ ����", TVI_ROOT);
	}
	else
		m_ctlTreePreview_.Select(m_ctlTreePreview_.GetRootItem(), TVGN_FIRSTVISIBLE);
}

inline void CRollupEntityObjectList::ClearTreeMap_()
{
	m_mapTreeViewAssociatedAttributes_.clear();
	m_ctlTreePreview_.EnableWindow(TRUE);
	m_ctlTreePreview_.DeleteAllItems();
}

inline void CRollupEntityObjectList::WriteTreeViewMessage(SAttributeAtXML* _pEntityAttribute)
{
	if(_pEntityAttribute != NULL)
		_pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
}
