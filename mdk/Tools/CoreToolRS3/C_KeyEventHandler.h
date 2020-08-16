#pragma once

class CControlMediator;
class CDlgMapNew;
class CCoreToolToolBar;

//Terrain Edit
class CRollupTerrainHeight;
class CRollupTerrainAverage;
class CRollupTerrainFlat;
class CRollupTerrainSplat;
class CRollupTerrainRelax;
class CRollupTerrainGrass;
class CRollupTerrainUserMaterial;

//Static Object
class CDlgSceneNodeList;

//MapView
class CRollupEtcMapView;

class CRollupEtcDecal;

//develop
class CRollupEtcDev;

class CORETOOL_API CKeyEventHandlerHelper
{
public:
	CKeyEventHandlerHelper(){}
	virtual ~CKeyEventHandlerHelper(){}

	virtual void OnPressedCtrlCompositeHotKey(UINT _charParam, CControlMediator* _pMediator) = 0;
	virtual void OnPressedNoneCompositeHotKey(UINT _charParam, CControlMediator* _pMediator) = 0;
};

class CORETOOL_API CKeyEventHandler
{
public:
	~CKeyEventHandler(void);
	/**
		@brief  �ڵ鷯 �ʱ�ȭ �޼ҵ�
		@param  _pControlMediator : ��Ʈ�� �߰��� */
	void InitHandler(CControlMediator* _pControlMediator);
	/**
		@brief  ���������� �ҷ����� Ű���� ����Ű �ڵ� �޼ҵ�
		@param  _charParam : �����ִ� Ű
		@param  _bKeyDown : �������� */
	void CallBackAppKeyEvent(UINT _charParam, bool _bKeyDown, bool& _ref_bKeyIntercepted);

	void SysKeyEvent(UINT _charParam, bool _bKeyDown, bool& _ref_bKeyIntercepted);

	bool IsFocusWindowEditable();

	static CKeyEventHandler& GetReference();

	void CancelMenuKey();

private:
	CKeyEventHandler(void); //�̱���

	CControlMediator*  m_pControlMediator_;
	CCoreToolToolBar*  m_pToolBar_;

	CRollupTerrainHeight*  m_pEditTerrainHeight_;
	CRollupTerrainAverage* m_pEditTerrainAver_;
	CRollupTerrainFlat*    m_pEditTerrainFlat_;
	CRollupTerrainSplat*   m_pEditTerrainSplat_;
	CRollupTerrainGrass*   m_pEditTerrainGrass_;
	CRollupTerrainRelax*   m_pEditTerrainRelax_;
	CDlgSceneNodeList*     m_pEditObjectList_;

	CRollupEtcMapView*		m_pEditMapView_;
	CRollupEtcDecal*		m_pEtcDecal_;
	CRollupEtcDev*			m_pEtcDev_;

	bool					m_bMenuKeyCanceled;	///< alt Ű ���� ����

	CString m_cstrStatusMessage_;

	void MapEditAreaControl_(bool _positive);
	template<typename T>
	inline void T_MapStrengthControl_(T* _type, bool _positive, float _fRate)
	{
		int str = _type->m_sliderCtrlStr.GetPos();
		if(_positive)
		{
			str += 1;
		}else
		{
			str -= 1;
		}
		_type->m_sliderCtrlStr.SetPos(str);
		_type->m_fStr = (float)_type->m_sliderCtrlStr.GetPos() * _fRate;
		static_cast<CWnd*>(_type)->UpdateData(FALSE);
	}
};