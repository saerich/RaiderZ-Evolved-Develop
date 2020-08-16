#include "StdAfx.h"
#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperResourceAddon.h"

#include "VD_DlgInputSensorWarp.h"
#include "VL_MapNew.h"

bool CControlMediatorHelperNewMapResourceImpl::IsKeyEventHandlerAcceptable()
{
	if (CDlgInputSensorWarp::m_isActive)
		return false;

	return true;
}

const char* CControlMediatorHelperNewMapResourceImpl::GetNewMapName()
{
	switch (m_eNewMapType_)
	{
	case E_ZONE:
		{
			return "�ҿ���Ʈ ��";
		}
		break;
	case E_FIELD:
		{
			return "�ҿ���Ʈ �ʵ�";
		}
		break;
	case E_CUTSCENE:
		{
			return "�ҿ���Ʈ �ƾ�";
		}
		break;
	}
	return NULL;
}

void CControlMediatorHelperNewMapResourceImpl::OpenNewMapDlg()
{
	CDlgMapNew* pDlgMapNew = m_pControlMediator_->GetViewPtr<CDlgMapNew>();
	_ASSERT(pDlgMapNew);

	switch (m_eNewMapType_)
	{
	case E_ZONE:
		{
			pDlgMapNew->SetNewType(CDlgMapNew::ZONE);
			pDlgMapNew->Listen(CoreMessage(VIEW_MESSAGE::ACTIVE_VIEW));
			return;
		}
		break;
	case E_FIELD:
		{
			pDlgMapNew->SetNewType(CDlgMapNew::FIELD);
			pDlgMapNew->Listen(CoreMessage(VIEW_MESSAGE::ACTIVE_VIEW));
			return;
		}
		break;
	case E_CUTSCENE:
		{
			pDlgMapNew->SetNewType(CDlgMapNew::CUTSCENE);
			pDlgMapNew->Listen(CoreMessage(VIEW_MESSAGE::ACTIVE_VIEW));
			return;
		}
		break;
	}
	return;
}
